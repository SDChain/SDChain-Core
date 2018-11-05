//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/ledger/InboundLedgers.h>
#include <sdchain/app/ledger/LedgerToJson.h>
#include <sdchain/app/ledger/LedgerMaster.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/net/RPCErr.h>
#include <sdchain/resource/Fees.h>
#include <sdchain/rpc/Context.h>
#include <sdchain/protocol/ErrorCodes.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/rpc/impl/Tuning.h>

namespace sdchain {

// {
//   ledger_hash : <ledger>
//   ledger_index : <ledger_index>
// }
Json::Value doLedgerRequest (RPC::Context& context)
{
    auto const hasHash = context.params.isMember (jss::ledger_hash);
    auto const hasIndex = context.params.isMember (jss::ledger_index);
    std::uint32_t ledgerIndex = 0;

    auto& ledgerMaster = context.app.getLedgerMaster();
    LedgerHash ledgerHash;

    if ((hasHash && hasIndex) || !(hasHash || hasIndex))
    {
        return RPC::make_param_error(
            "Exactly one of ledger_hash and ledger_index can be set.");
    }

    context.loadType = Resource::feeHighBurdenRPC;

    if (hasHash)
    {
        auto const& jsonHash = context.params[jss::ledger_hash];
        if (!jsonHash.isString() || !ledgerHash.SetHex (jsonHash.asString ()))
            return RPC::invalid_field_error (jss::ledger_hash);
    }
    else
    {
        auto const& jsonIndex = context.params[jss::ledger_index];
        if (!jsonIndex.isInt())
            return RPC::invalid_field_error (jss::ledger_index);

        // We need a validated ledger to get the hash from the sequence
        if (ledgerMaster.getValidatedLedgerAge() >
            RPC::Tuning::maxValidatedLedgerAge)
            return rpcError (rpcNO_CURRENT);

        ledgerIndex = jsonIndex.asInt();
        auto ledger = ledgerMaster.getValidatedLedger();

        if (ledgerIndex >= ledger->info().seq)
            return RPC::make_param_error("Ledger index too large");
        if (ledgerIndex <= 0)
            return RPC::make_param_error("Ledger index too small");

        auto const j = context.app.journal("RPCHandler");
        // Try to get the hash of the desired ledger from the validated ledger
        auto neededHash = hashOfSeq(*ledger, ledgerIndex, j);
        if (! neededHash)
        {
            // Find a ledger more likely to have the hash of the desired ledger
            auto const refIndex = getCandidateLedger(ledgerIndex);
            auto refHash = hashOfSeq(*ledger, refIndex, j);
            assert(refHash);

            ledger = ledgerMaster.getLedgerByHash (*refHash);
            if (! ledger)
            {
                // We don't have the ledger we need to figure out which ledger
                // they want. Try to get it.

                if (auto il = context.app.getInboundLedgers().acquire (
                        *refHash, refIndex, InboundLedger::fcGENERIC))
                {
                    Json::Value jvResult = RPC::make_error(
                        rpcLGR_NOT_FOUND,
                            "acquiring ledger containing requested index");
                    jvResult[jss::acquiring] = getJson (LedgerFill (*il));
                    return jvResult;
                }

                if (auto il = context.app.getInboundLedgers().find (*refHash))
                {
                    Json::Value jvResult = RPC::make_error(
                        rpcLGR_NOT_FOUND,
                            "acquiring ledger containing requested index");
                    jvResult[jss::acquiring] = il->getJson (0);
                    return jvResult;
                }

                // Likely the app is shutting down
                return Json::Value();
            }

            neededHash = hashOfSeq(*ledger, ledgerIndex, j);
        }
        assert (neededHash);
        ledgerHash = neededHash ? *neededHash : zero; // kludge
    }

    // Try to get the desired ledger
    // Verify all nodes even if we think we have it
    auto ledger = context.app.getInboundLedgers().acquire (
        ledgerHash, ledgerIndex, InboundLedger::fcGENERIC);

    // In standalone mode, accept the ledger from the ledger cache
    if (! ledger && context.app.config().standalone())
        ledger = ledgerMaster.getLedgerByHash (ledgerHash);

    if (ledger)
    {
        // We already had the entire ledger verified/acquired
        Json::Value jvResult;
        jvResult[jss::ledger_index] = ledger->info().seq;
        addJson (jvResult, {*ledger, 0});
        return jvResult;
    }

    if (auto il = context.app.getInboundLedgers().find (ledgerHash))
        return il->getJson (0);

    return RPC::make_error (
        rpcNOT_READY, "findCreate failed to return an inbound ledger");
}

} // sdchain
