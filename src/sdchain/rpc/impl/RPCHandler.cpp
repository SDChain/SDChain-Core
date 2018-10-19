//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/rpc/RPCHandler.h>
#include <sdchain/rpc/impl/Tuning.h>
#include <sdchain/rpc/impl/Handler.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/app/ledger/LedgerMaster.h>
#include <sdchain/app/misc/NetworkOPs.h>
#include <sdchain/basics/contract.h>
#include <sdchain/basics/Log.h>
#include <sdchain/core/Config.h>
#include <sdchain/core/JobQueue.h>
#include <sdchain/json/Object.h>
#include <sdchain/json/to_string.h>
#include <sdchain/net/InfoSub.h>
#include <sdchain/net/RPCErr.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/resource/Fees.h>
#include <sdchain/rpc/Role.h>
#include <sdchain/resource/Fees.h>

namespace sdchain {
namespace RPC {

namespace {

/**
   This code is called from both the HTTP RPC handler and Websockets.

   The form of the Json returned is somewhat different between the two services.

   HTML:
     Success:
        {
           "result" : {
              "ledger" : {
                 "accepted" : false,
                 "transaction_hash" : "..."
              },
              "ledger_index" : 10300865,
              "validated" : false,
              "status" : "success"  # Status is inside the result.
           }
        }

     Failure:
        {
           "result" : {
              "error" : "noNetwork",
              "error_code" : 16,
              "error_message" : "Not synced to SDChain network.",
              "request" : {
                 "command" : "ledger",
                 "ledger_index" : 10300865
              },
              "status" : "error"
           }
        }

   Websocket:
     Success:
        {
           "result" : {
              "ledger" : {
                 "accepted" : false,
                 "transaction_hash" : "..."
              },
              "ledger_index" : 10300865,
              "validated" : false
           }
           "type": "response",
           "status": "success",   # Status is OUTside the result!
           "id": "client's ID",   # Optional
           "warning": 3.14        # Optional
        }

     Failure:
        {
          "error" : "noNetwork",
          "error_code" : 16,
          "error_message" : "Not synced to SDChain network.",
          "request" : {
             "command" : "ledger",
             "ledger_index" : 10300865
          },
          "type": "response",
          "status" : "error",
          "id": "client's ID"   # Optional
        }

 */

error_code_i fillHandler (Context& context,
                          Handler const * & result)
{
    if (! isUnlimited (context.role))
    {
        // VFALCO NOTE Should we also add up the jtRPC jobs?
        //
        int jc = context.app.getJobQueue ().getJobCountGE (jtCLIENT);
        if (jc > Tuning::maxJobQueueClients)
        {
            JLOG (context.j.debug()) << "Too busy for command: " << jc;
            return rpcTOO_BUSY;
        }
    }

    if (!context.params.isMember(jss::command) && !context.params.isMember(jss::method))
        return rpcCOMMAND_MISSING;
    if (context.params.isMember(jss::command) && context.params.isMember(jss::method))
    {
        if (context.params[jss::command].asString() !=
            context.params[jss::method].asString())
            return rpcUNKNOWN_COMMAND;
    }

    std::string strCommand  = context.params.isMember(jss::command) ?
                              context.params[jss::command].asString() :
                              context.params[jss::method].asString();

    JLOG (context.j.trace()) << "COMMAND:" << strCommand;
    JLOG (context.j.trace()) << "REQUEST:" << context.params;
    auto handler = getHandler(strCommand);

    if (!handler)
        return rpcUNKNOWN_COMMAND;

    if (handler->role_ == Role::ADMIN && context.role != Role::ADMIN)
        return rpcNO_PERMISSION;

    if ((handler->condition_ & NEEDS_NETWORK_CONNECTION) &&
        (context.netOps.getOperatingMode () < NetworkOPs::omSYNCING))
    {
        JLOG (context.j.info())
            << "Insufficient network mode for RPC: "
            << context.netOps.strOperatingMode ();

        return rpcNO_NETWORK;
    }

    if (context.app.getOPs().isAmendmentBlocked() &&
         (handler->condition_ & NEEDS_CURRENT_LEDGER ||
          handler->condition_ & NEEDS_CLOSED_LEDGER))
    {
        return rpcAMENDMENT_BLOCKED;
    }

    if (!context.app.config().standalone() &&
        handler->condition_ & NEEDS_CURRENT_LEDGER)
    {
        if (context.ledgerMaster.getValidatedLedgerAge () >
            Tuning::maxValidatedLedgerAge)
        {
            return rpcNO_CURRENT;
        }

        auto const cID = context.ledgerMaster.getCurrentLedgerIndex ();
        auto const vID = context.ledgerMaster.getValidLedgerIndex ();

        if (cID + 10 < vID)
        {
            JLOG (context.j.debug()) << "Current ledger ID(" << cID <<
                ") is less than validated ledger ID(" << vID << ")";
            return rpcNO_CURRENT;
        }
    }

    if ((handler->condition_ & NEEDS_CLOSED_LEDGER) &&
        !context.ledgerMaster.getClosedLedger ())
    {
        return rpcNO_CLOSED;
    }

    result = handler;
    return rpcSUCCESS;
}

template <class Object, class Method>
Status callMethod (
    Context& context, Method method, std::string const& name, Object& result)
{
    try
    {
        auto v = context.app.getJobQueue().makeLoadEvent(
            jtGENERIC, "cmd:" + name);
        return method (context, result);
    }
    catch (std::exception& e)
    {
        JLOG (context.j.info()) << "Caught throw: " << e.what ();

        if (context.loadType == Resource::feeReferenceRPC)
            context.loadType = Resource::feeExceptionRPC;

        inject_error (rpcINTERNAL, result);
        return rpcINTERNAL;
    }
}

template <class Method, class Object>
void getResult (
    Context& context, Method method, Object& object, std::string const& name)
{
    auto&& result = Json::addObject (object, jss::result);
    if (auto status = callMethod (context, method, name, result))
    {
        JLOG (context.j.debug()) << "rpcError: " << status.toString();
        result[jss::status] = jss::error;
        result[jss::request] = context.params;
    }
    else
    {
        result[jss::status] = jss::success;
    }
}

} // namespace

Status doCommand (
    RPC::Context& context, Json::Value& result)
{
    Handler const * handler = nullptr;
    if (auto error = fillHandler (context, handler))
    {
        inject_error (error, result);
        return error;
    }

    if (auto method = handler->valueMethod_)
    {
        if (! context.headers.user.empty() ||
            ! context.headers.forwardedFor.empty())
        {
            JLOG(context.j.debug()) << "start command: " << handler->name_ <<
                ", X-User: " << context.headers.user << ", X-Forwarded-For: " <<
                    context.headers.forwardedFor;

            auto ret = callMethod (context, method, handler->name_, result);

            JLOG(context.j.debug()) << "finish command: " << handler->name_ <<
                ", X-User: " << context.headers.user << ", X-Forwarded-For: " <<
                    context.headers.forwardedFor;

            return ret;
        }
        else
        {
            return callMethod (context, method, handler->name_, result);
        }
    }

    return rpcUNKNOWN_COMMAND;
}

Role roleRequired (std::string const& method)
{
    auto handler = RPC::getHandler(method);

    if (!handler)
        return Role::FORBID;

    return handler->role_;
}

} // RPC
} // sdchain
