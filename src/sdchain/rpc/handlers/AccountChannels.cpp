//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/ledger/ReadView.h>
#include <sdchain/ledger/View.h>
#include <sdchain/net/RPCErr.h>
#include <sdchain/protocol/ErrorCodes.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/protocol/PublicKey.h>
#include <sdchain/protocol/STAccount.h>
#include <sdchain/resource/Fees.h>
#include <sdchain/rpc/Context.h>
#include <sdchain/rpc/impl/RPCHelpers.h>
#include <sdchain/rpc/impl/Tuning.h>
namespace sdchain {

void addChannel (Json::Value& jsonLines, SLE const& line)
{
    Json::Value& jDst (jsonLines.append (Json::objectValue));
    jDst[jss::channel_id] = to_string (line.key ());
    jDst[jss::account] = to_string (line[sfAccount]);
    jDst[jss::destination_account] = to_string (line[sfDestination]);
    jDst[jss::amount] = line[sfAmount].getText ();
    jDst[jss::balance] = line[sfBalance].getText ();
    if (publicKeyType(line[sfPublicKey]))
    {
        PublicKey const pk (line[sfPublicKey]);
        jDst[jss::public_key] = toBase58 (TokenType::TOKEN_ACCOUNT_PUBLIC, pk);
        jDst[jss::public_key_hex] = strHex (pk);
    }
    jDst[jss::settle_delay] = line[sfSettleDelay];
    if (auto const& v = line[~sfExpiration])
        jDst[jss::expiration] = *v;
    if (auto const& v = line[~sfCancelAfter])
        jDst[jss::cancel_after] = *v;
    if (auto const& v = line[~sfSourceTag])
        jDst[jss::source_tag] = *v;
    if (auto const& v = line[~sfDestinationTag])
        jDst[jss::destination_tag] = *v;
}

// {
//   account: <account>|<account_public_key>
//   ledger_hash : <ledger>
//   ledger_index : <ledger_index>
//   limit: integer                 // optional
//   marker: opaque                 // optional, resume previous query
// }
Json::Value doAccountChannels (RPC::Context& context)
{
    auto const& params (context.params);
    if (! params.isMember (jss::account))
        return RPC::missing_field_error (jss::account);

    std::shared_ptr<ReadView const> ledger;
    auto result = RPC::lookupLedger (ledger, context);
    if (! ledger)
        return result;

    std::string strIdent (params[jss::account].asString ());
    AccountID accountID;

    result = RPC::accountFromString (accountID, strIdent);
    if (result)
        return result;

    if (! ledger->exists(keylet::account (accountID)))
        return rpcError (rpcACT_NOT_FOUND);

    std::string strDst;
    if (params.isMember (jss::destination_account))
        strDst = params[jss::destination_account].asString ();
    auto hasDst = ! strDst.empty ();

    AccountID raDstAccount;
    if (hasDst)
    {
        result = RPC::accountFromString (raDstAccount, strDst);
        if (result)
            return result;
    }

    unsigned int limit;
    if (auto err = readLimitField(limit, RPC::Tuning::accountChannels, context))
        return *err;

    Json::Value jsonChannels{Json::arrayValue};
    struct VisitData
    {
        std::vector <std::shared_ptr<SLE const>> items;
        AccountID const& accountID;
        bool hasDst;
        AccountID const& raDstAccount;
    };
    VisitData visitData = {{}, accountID, hasDst, raDstAccount};
    unsigned int reserve (limit);
    uint256 startAfter;
    std::uint64_t startHint;

    if (params.isMember (jss::marker))
    {
        // We have a start point. Use limit - 1 from the result and use the
        // very last one for the resume.
        Json::Value const& marker (params[jss::marker]);

        if (! marker.isString ())
            return RPC::expected_field_error (jss::marker, "string");

        startAfter.SetHex (marker.asString ());
        auto const sleChannel = ledger->read({ltPAYCHAN, startAfter});

        if (! sleChannel)
            return rpcError (rpcINVALID_PARAMS);

        if (sleChannel->getFieldAmount (sfLowLimit).getIssuer () == accountID)
            startHint = sleChannel->getFieldU64 (sfLowNode);
        else if (sleChannel->getFieldAmount (sfHighLimit).getIssuer () == accountID)
            startHint = sleChannel->getFieldU64 (sfHighNode);
        else
            return rpcError (rpcINVALID_PARAMS);

        addChannel (jsonChannels, *sleChannel);
        visitData.items.reserve (reserve);
    }
    else
    {
        startHint = 0;
        // We have no start point, limit should be one higher than requested.
        visitData.items.reserve (++reserve);
    }

    if (! forEachItemAfter(*ledger, accountID,
            startAfter, startHint, reserve,
        [&visitData](std::shared_ptr<SLE const> const& sleCur)
        {

            if (sleCur && sleCur->getType () == ltPAYCHAN &&
                (! visitData.hasDst ||
                 visitData.raDstAccount == (*sleCur)[sfDestination]))
            {
                visitData.items.emplace_back (sleCur);
                return true;
            }

            return false;
        }))
    {
        return rpcError (rpcINVALID_PARAMS);
    }

    if (visitData.items.size () == reserve)
    {
        result[jss::limit] = limit;

        result[jss::marker] = to_string (visitData.items.back()->key());
        visitData.items.pop_back ();
    }

    result[jss::account] = context.app.accountIDCache().toBase58 (accountID);

    for (auto const& item : visitData.items)
        addChannel (jsonChannels, *item);

    context.loadType = Resource::feeMediumBurdenRPC;
    result[jss::channels] = std::move(jsonChannels);
    return result;
}

} // sdchain
