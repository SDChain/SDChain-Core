//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/ledger/LedgerToJson.h>
#include <sdchain/ledger/ReadView.h>
#include <sdchain/protocol/ErrorCodes.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/protocol/LedgerFormats.h>
#include <sdchain/rpc/impl/RPCHelpers.h>
#include <sdchain/rpc/impl/Tuning.h>
#include <sdchain/rpc/Context.h>
#include <sdchain/rpc/Role.h>

namespace sdchain {

// Get state nodes from a ledger
//   Inputs:
//     limit:        integer, maximum number of entries
//     marker:       opaque, resume point
//     binary:       boolean, format
//     type:         string // optional, defaults to all ledger node types
//   Outputs:
//     ledger_hash:  chosen ledger's hash
//     ledger_index: chosen ledger's index
//     state:        array of state nodes
//     marker:       resume point, if any
Json::Value doLedgerData (RPC::Context& context)
{
    std::shared_ptr<ReadView const> lpLedger;
    auto const& params = context.params;

    auto jvResult = RPC::lookupLedger(lpLedger, context);
    if (!lpLedger)
        return jvResult;

    bool const isMarker = params.isMember (jss::marker);
    ReadView::key_type key = ReadView::key_type();
    if (isMarker)
    {
        Json::Value const& jMarker = params[jss::marker];
        if (! (jMarker.isString () && key.SetHex (jMarker.asString ())))
            return RPC::expected_field_error (jss::marker, "valid");
    }

    bool const isBinary = params[jss::binary].asBool();

    int limit = -1;
    if (params.isMember (jss::limit))
    {
        Json::Value const& jLimit = params[jss::limit];
        if (!jLimit.isIntegral ())
            return RPC::expected_field_error (jss::limit, "integer");

        limit = jLimit.asInt ();
    }

    auto maxLimit = RPC::Tuning::pageLength(isBinary);
    if ((limit < 0) || ((limit > maxLimit) && (! isUnlimited (context.role))))
        limit = maxLimit;

    jvResult[jss::ledger_hash] = to_string (lpLedger->info().hash);
    jvResult[jss::ledger_index] = lpLedger->info().seq;

    if (! isMarker)
    {
        // Return base ledger data on first query
        jvResult[jss::ledger] = getJson (
            LedgerFill (*lpLedger, isBinary ?
                LedgerFill::Options::binary : 0));
    }

    auto type = RPC::chooseLedgerEntryType(params);
    if (type.first)
    {
        jvResult.clear();
        type.first.inject(jvResult);
        return jvResult;
    }
    Json::Value& nodes = jvResult[jss::state];

    auto e = lpLedger->sles.end();
    for (auto i = lpLedger->sles.upper_bound(key); i != e; ++i)
    {
        auto sle = lpLedger->read(keylet::unchecked((*i)->key()));
        if (limit-- <= 0)
        {
            // Stop processing before the current key.
            auto k = sle->key();
            jvResult[jss::marker] = to_string(--k);
            break;
        }

        if (type.second == ltINVALID || sle->getType () == type.second)
        {
            if (isBinary)
            {
                Json::Value& entry = nodes.append (Json::objectValue);
                entry[jss::data] = serializeHex(*sle);
                entry[jss::index] = to_string(sle->key());
            }
            else
            {
                Json::Value& entry = nodes.append (sle->getJson (0));
                entry[jss::index] = to_string(sle->key());
            }
        }
    }

    return jvResult;
}

} // sdchain
