//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/ledger/LedgerMaster.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/app/misc/NetworkOPs.h>
#include <sdchain/app/paths/PathRequests.h>
#include <sdchain/net/RPCErr.h>
#include <sdchain/protocol/ErrorCodes.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/resource/Fees.h>
#include <sdchain/rpc/Context.h>

namespace sdchain {

Json::Value doPathFind (RPC::Context& context)
{
    if (context.app.config().PATH_SEARCH_MAX == 0)
        return rpcError (rpcNOT_SUPPORTED);

    auto lpLedger = context.ledgerMaster.getClosedLedger();

    if (!context.params.isMember (jss::subcommand) ||
        !context.params[jss::subcommand].isString ())
    {
        return rpcError (rpcINVALID_PARAMS);
    }

    if (!context.infoSub)
        return rpcError (rpcNO_EVENTS);

    auto sSubCommand = context.params[jss::subcommand].asString ();

    if (sSubCommand == "create")
    {
        context.loadType = Resource::feeHighBurdenRPC;
        context.infoSub->clearPathRequest ();
        return context.app.getPathRequests().makePathRequest (
            context.infoSub, lpLedger, context.params);
    }

    if (sSubCommand == "close")
    {
        PathRequest::pointer request = context.infoSub->getPathRequest ();

        if (!request)
            return rpcError (rpcNO_PF_REQUEST);

        context.infoSub->clearPathRequest ();
        return request->doClose (context.params);
    }

    if (sSubCommand == "status")
    {
        PathRequest::pointer request = context.infoSub->getPathRequest ();

        if (!request)
            return rpcError (rpcNO_PF_REQUEST);

        return request->doStatus (context.params);
    }

    return rpcError (rpcINVALID_PARAMS);
}

} // sdchain
