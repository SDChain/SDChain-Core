//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/json/json_writer.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/ledger/ReadView.h>
#include <sdchain/net/RPCErr.h>
#include <sdchain/protocol/ErrorCodes.h>
#include <sdchain/protocol/Indexes.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/protocol/LedgerFormats.h>
#include <sdchain/resource/Fees.h>
#include <sdchain/rpc/Context.h>
#include <sdchain/rpc/impl/RPCHelpers.h>
#include <sdchain/rpc/impl/Tuning.h>

#include <string>
#include <sstream>

namespace sdchain {

/** General RPC command that can retrieve objects in the account root.
    {
      account: <account>|<account_public_key>
      ledger_hash: <string> // optional
      ledger_index: <string | unsigned integer> // optional
      type: <string> // optional, defaults to all account objects types
      limit: <integer> // optional
      marker: <opaque> // optional, resume previous query
    }
*/

Json::Value doAccountObjects (RPC::Context& context)
{
    auto const& params = context.params;
    if (! params.isMember (jss::account))
        return RPC::missing_field_error (jss::account);

    std::shared_ptr<ReadView const> ledger;
    auto result = RPC::lookupLedger (ledger, context);
    if (ledger == nullptr)
        return result;

    AccountID accountID;
    {
        auto const strIdent = params[jss::account].asString ();
        if (auto jv = RPC::accountFromString (accountID, strIdent))
        {
            for (auto it = jv.begin (); it != jv.end (); ++it)
                result[it.memberName ()] = *it;

            return result;
        }
    }

    if (! ledger->exists(keylet::account (accountID)))
        return rpcError (rpcACT_NOT_FOUND);

    auto type = RPC::chooseLedgerEntryType(params);
    if (type.first)
    {
        result.clear();
        type.first.inject(result);
        return result;
    }

    unsigned int limit;
    if (auto err = readLimitField(limit, RPC::Tuning::accountObjects, context))
        return *err;

    uint256 dirIndex;
    uint256 entryIndex;
    if (params.isMember (jss::marker))
    {
        auto const& marker = params[jss::marker];
        if (! marker.isString ())
            return RPC::expected_field_error (jss::marker, "string");

        std::stringstream ss (marker.asString ());
        std::string s;
        if (!std::getline(ss, s, ','))
            return RPC::invalid_field_error (jss::marker);

        if (! dirIndex.SetHex (s))
            return RPC::invalid_field_error (jss::marker);

        if (! std::getline (ss, s, ','))
            return RPC::invalid_field_error (jss::marker);

        if (! entryIndex.SetHex (s))
            return RPC::invalid_field_error (jss::marker);
    }

    if (! RPC::getAccountObjects (*ledger, accountID, type.second,
        dirIndex, entryIndex, limit, result))
    {
        result[jss::account_objects] = Json::arrayValue;
    }

    result[jss::account] = context.app.accountIDCache().toBase58 (accountID);
    context.loadType = Resource::feeMediumBurdenRPC;
    return result;
}

} // sdchain
