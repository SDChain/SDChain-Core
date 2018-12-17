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
#include <sdchain/app/misc/Transaction.h>
#include <sdchain/ledger/ReadView.h>
#include <sdchain/net/RPCErr.h>
#include <sdchain/protocol/ErrorCodes.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/resource/Fees.h>
#include <sdchain/rpc/Context.h>
#include <sdchain/rpc/impl/RPCHelpers.h>
#include <sdchain/rpc/Role.h>

namespace sdchain {

// {
//   account: account,
//   ledger_index_min: ledger_index,
//   ledger_index_max: ledger_index,
//   binary: boolean,              // optional, defaults to false
//   count: boolean,               // optional, defaults to false
//   descending: boolean,          // optional, defaults to false
//   offset: integer,              // optional, defaults to 0
//   limit: integer                // optional
// }
Json::Value doAccountTxOld (RPC::Context& context)
{
    std::uint32_t offset
            = context.params.isMember (jss::offset)
            ? context.params[jss::offset].asUInt () : 0;
    int limit = context.params.isMember (jss::limit)
            ? context.params[jss::limit].asUInt () : -1;
    bool bBinary = context.params.isMember (jss::binary)
            && context.params[jss::binary].asBool ();
    bool bDescending = context.params.isMember (jss::descending)
            && context.params[jss::descending].asBool ();
    bool bCount = context.params.isMember (jss::count)
            && context.params[jss::count].asBool ();
    std::uint32_t   uLedgerMin;
    std::uint32_t   uLedgerMax;
    std::uint32_t   uValidatedMin;
    std::uint32_t   uValidatedMax;
    bool bValidated  = context.ledgerMaster.getValidatedRange (
        uValidatedMin, uValidatedMax);

    if (!context.params.isMember (jss::account))
        return rpcError (rpcINVALID_PARAMS);

    auto const raAccount = parseBase58<AccountID>(
        context.params[jss::account].asString());
    if (! raAccount)
        return rpcError (rpcACT_MALFORMED);

    if (offset > 3000)
        return rpcError (rpcATX_DEPRECATED);

    context.loadType = Resource::feeHighBurdenRPC;

    // DEPRECATED
    if (context.params.isMember (jss::ledger_min))
    {
        context.params[jss::ledger_index_min]   = context.params[jss::ledger_min];
        bDescending = true;
    }

    // DEPRECATED
    if (context.params.isMember (jss::ledger_max))
    {
        context.params[jss::ledger_index_max]   = context.params[jss::ledger_max];
        bDescending = true;
    }

    if (context.params.isMember (jss::ledger_index_min)
        || context.params.isMember (jss::ledger_index_max))
    {
        std::int64_t iLedgerMin  = context.params.isMember (jss::ledger_index_min)
                ? context.params[jss::ledger_index_min].asInt () : -1;
        std::int64_t iLedgerMax  = context.params.isMember (jss::ledger_index_max)
                ? context.params[jss::ledger_index_max].asInt () : -1;

        if (!bValidated && (iLedgerMin == -1 || iLedgerMax == -1))
        {
            // Don't have a validated ledger range.
            return rpcError (rpcLGR_IDXS_INVALID);
        }

        uLedgerMin  = iLedgerMin == -1 ? uValidatedMin : iLedgerMin;
        uLedgerMax  = iLedgerMax == -1 ? uValidatedMax : iLedgerMax;

        if (uLedgerMax < uLedgerMin)
        {
            return rpcError (rpcLGR_IDXS_INVALID);
        }
    }
    else
    {
        std::shared_ptr<ReadView const> ledger;
        auto ret = RPC::lookupLedger (ledger, context);

        if (!ledger)
            return ret;

        if (! ret[jss::validated].asBool() ||
            (ledger->info().seq > uValidatedMax) ||
            (ledger->info().seq < uValidatedMin))
        {
            return rpcError (rpcLGR_NOT_VALIDATED);
        }

        uLedgerMin = uLedgerMax = ledger->info().seq;
    }

    int count = 0;

#ifndef BEAST_DEBUG

    try
    {
#endif

        Json::Value ret (Json::objectValue);

        ret[jss::account] = context.app.accountIDCache().toBase58(*raAccount);
        Json::Value& jvTxns = (ret[jss::transactions] = Json::arrayValue);

        if (bBinary)
        {
            auto txns = context.netOps.getAccountTxsB (
                *raAccount, uLedgerMin, uLedgerMax, bDescending, offset, limit,
                isUnlimited (context.role));

            for (auto it = txns.begin (), end = txns.end (); it != end; ++it)
            {
                ++count;
                Json::Value& jvObj = jvTxns.append (Json::objectValue);

                std::uint32_t  uLedgerIndex = std::get<2> (*it);
                jvObj[jss::tx_blob]            = std::get<0> (*it);
                jvObj[jss::meta]               = std::get<1> (*it);
                jvObj[jss::ledger_index]       = uLedgerIndex;
                jvObj[jss::validated]
                        = bValidated
                        && uValidatedMin <= uLedgerIndex
                        && uValidatedMax >= uLedgerIndex;

            }
        }
        else
        {
            auto txns = context.netOps.getAccountTxs (
                *raAccount, uLedgerMin, uLedgerMax, bDescending, offset, limit,
                isUnlimited (context.role));

            for (auto it = txns.begin (), end = txns.end (); it != end; ++it)
            {
                ++count;
                Json::Value&    jvObj = jvTxns.append (Json::objectValue);

                if (it->first)
                    jvObj[jss::tx]             = it->first->getJson (1);

                if (it->second)
                {
                    std::uint32_t uLedgerIndex = it->second->getLgrSeq ();

                    auto meta = it->second->getJson(0);
                    addPaymentDeliveredAmount(meta, context, it->first, it->second);
                    jvObj[jss::meta] = std::move(meta);

                    jvObj[jss::validated]
                            = bValidated
                            && uValidatedMin <= uLedgerIndex
                            && uValidatedMax >= uLedgerIndex;
                }

            }
        }

        //Add information about the original query
        ret[jss::ledger_index_min] = uLedgerMin;
        ret[jss::ledger_index_max] = uLedgerMax;
        ret[jss::validated]
                = bValidated
                && uValidatedMin <= uLedgerMin
                && uValidatedMax >= uLedgerMax;
        ret[jss::offset]           = offset;

        // We no longer return the full count but only the count of returned
        // transactions. Computing this count was two expensive and this API is
        // deprecated anyway.
        if (bCount)
            ret[jss::count]        = count;

        if (context.params.isMember (jss::limit))
            ret[jss::limit]        = limit;


        return ret;
#ifndef BEAST_DEBUG
    }
    catch (std::exception const&)
    {
        return rpcError (rpcINTERNAL);
    }

#endif
}

} // sdchain
