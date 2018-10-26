//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/app/paths/SDChainState.h>
#include <sdchain/ledger/ReadView.h>
#include <sdchain/net/RPCErr.h>
#include <sdchain/protocol/ErrorCodes.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/resource/Fees.h>
#include <sdchain/rpc/Context.h>
#include <sdchain/rpc/impl/RPCHelpers.h>
#include <sdchain/rpc/impl/Tuning.h>

namespace sdchain {

struct VisitData
{
    std::vector <SDChainState::pointer> items;
    AccountID const& accountID;
    bool hasPeer;
    AccountID const& raPeerAccount;
};

void addLine (Json::Value& jsonLines, SDChainState const& line)
{
    STAmount const& saBalance (line.getBalance ());
    STAmount const& saLimit (line.getLimit ());
    STAmount const& saLimitPeer (line.getLimitPeer ());
    Json::Value& jPeer (jsonLines.append (Json::objectValue));

    jPeer[jss::account] = to_string (line.getAccountIDPeer ());
    // Amount reported is positive if current account holds other
    // account's IOUs.
    //
    // Amount reported is negative if other account holds current
    // account's IOUs.
    jPeer[jss::balance] = saBalance.getText ();
    jPeer[jss::currency] = to_string (saBalance.issue ().currency);
    jPeer[jss::limit] = saLimit.getText ();
    jPeer[jss::limit_peer] = saLimitPeer.getText ();
    jPeer[jss::quality_in] = line.getQualityIn ().value;
    jPeer[jss::quality_out] = line.getQualityOut ().value;
    if (line.getAuth ())
        jPeer[jss::authorized] = true;
    if (line.getAuthPeer ())
        jPeer[jss::peer_authorized] = true;
    if (line.getNoSDChain ())
        jPeer[jss::no_sdchain] = true;
    if (line.getNoSDChainPeer ())
        jPeer[jss::no_sdchain_peer] = true;
    if (line.getFreeze ())
        jPeer[jss::freeze] = true;
    if (line.getFreezePeer ())
        jPeer[jss::freeze_peer] = true;
}

// {
//   account: <account>|<account_public_key>
//   ledger_hash : <ledger>
//   ledger_index : <ledger_index>
//   limit: integer                 // optional
//   marker: opaque                 // optional, resume previous query
// }
Json::Value doAccountLines (RPC::Context& context)
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

    if (auto jv = RPC::accountFromString (accountID, strIdent))
    {
        for (auto it = jv.begin (); it != jv.end (); ++it)
            result[it.memberName ()] = *it;
        return result;
    }

    if (! ledger->exists(keylet::account (accountID)))
        return rpcError (rpcACT_NOT_FOUND);

    std::string strPeer;
    if (params.isMember (jss::peer))
        strPeer = params[jss::peer].asString ();
    auto hasPeer = ! strPeer.empty ();

    AccountID raPeerAccount;
    if (hasPeer)
    {
        if (auto jv = RPC::accountFromString (raPeerAccount, strPeer))
        {
            for (auto it = jv.begin (); it != jv.end (); ++it)
                result[it.memberName ()] = *it;
            return result;
        }
    }

    unsigned int limit;
    if (auto err = readLimitField(limit, RPC::Tuning::accountLines, context))
        return *err;

    Json::Value& jsonLines (result[jss::lines] = Json::arrayValue);
    VisitData visitData = {{}, accountID, hasPeer, raPeerAccount};
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
        auto const sleLine = ledger->read({ltSDCHAIN_STATE, startAfter});

        if (! sleLine)
            return rpcError (rpcINVALID_PARAMS);

        if (sleLine->getFieldAmount (sfLowLimit).getIssuer () == accountID)
            startHint = sleLine->getFieldU64 (sfLowNode);
        else if (sleLine->getFieldAmount (sfHighLimit).getIssuer () == accountID)
            startHint = sleLine->getFieldU64 (sfHighNode);
        else
            return rpcError (rpcINVALID_PARAMS);

        // Caller provided the first line (startAfter), add it as first result
        auto const line = SDChainState::makeItem (accountID, sleLine);
        if (line == nullptr)
            return rpcError (rpcINVALID_PARAMS);

        addLine (jsonLines, *line);
        visitData.items.reserve (reserve);
    }
    else
    {
        startHint = 0;
        // We have no start point, limit should be one higher than requested.
        visitData.items.reserve (++reserve);
    }

    {
        if (! forEachItemAfter(*ledger, accountID,
                startAfter, startHint, reserve,
            [&visitData](std::shared_ptr<SLE const> const& sleCur)
            {
                auto const line =
                    SDChainState::makeItem (visitData.accountID, sleCur);
                if (line != nullptr &&
                    (! visitData.hasPeer ||
                     visitData.raPeerAccount == line->getAccountIDPeer ()))
                {
                    visitData.items.emplace_back (line);
                    return true;
                }

                return false;
            }))
        {
            return rpcError (rpcINVALID_PARAMS);
        }
    }

    if (visitData.items.size () == reserve)
    {
        result[jss::limit] = limit;

        SDChainState::pointer line (visitData.items.back ());
        result[jss::marker] = to_string (line->key());
        visitData.items.pop_back ();
    }

    result[jss::account] = context.app.accountIDCache().toBase58 (accountID);

    for (auto const& item : visitData.items)
        addLine (jsonLines, *item.get ());

    context.loadType = Resource::feeMediumBurdenRPC;
    return result;
}

} // sdchain
