//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/ledger/LedgerMaster.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/app/misc/AmendmentTable.h>
#include <sdchain/protocol/ErrorCodes.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/net/RPCErr.h>
#include <sdchain/rpc/Context.h>
#include <sdchain/beast/core/LexicalCast.h>

namespace sdchain {


// {
//   feature : <feature>
//   vetoed : true/false
// }
Json::Value doFeature (RPC::Context& context)
{

    // Get majority amendment status
    majorityAmendments_t majorities;

    if (auto const valLedger = context.ledgerMaster.getValidatedLedger())
        majorities = getMajorityAmendments (*valLedger);

    auto& table = context.app.getAmendmentTable ();

    if (!context.params.isMember (jss::feature))
    {
        auto features = table.getJson(0);

        for (auto const& m : majorities)
        {
            features[to_string(m.first)][jss::majority] =
                m.second.time_since_epoch().count();
        }

        Json::Value jvReply = Json::objectValue;
        jvReply[jss::features] = features;
        return jvReply;
    }

    auto feature = table.find (
        context.params[jss::feature].asString());

    if (!feature &&
        !feature.SetHexExact (context.params[jss::feature].asString ()))
        return rpcError (rpcBAD_FEATURE);

    if (context.params.isMember (jss::vetoed))
    {
        if (context.params[jss::vetoed].asBool ())
            context.app.getAmendmentTable().veto (feature);
        else
            context.app.getAmendmentTable().unVeto(feature);
    }

    Json::Value jvReply = table.getJson(feature);

    auto m = majorities.find (feature);
    if (m != majorities.end())
        jvReply [jss::majority] =
            m->second.time_since_epoch().count();

    return jvReply;
}


} // sdchain
