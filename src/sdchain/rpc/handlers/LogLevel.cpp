//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/basics/Log.h>
#include <sdchain/json/json_value.h>
#include <sdchain/net/RPCErr.h>
#include <sdchain/protocol/ErrorCodes.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/rpc/Context.h>
#include <boost/algorithm/string/predicate.hpp>

namespace sdchain {

Json::Value doLogLevel (RPC::Context& context)
{
    // log_level
    if (!context.params.isMember (jss::severity))
    {
        // get log severities
        Json::Value ret (Json::objectValue);
        Json::Value lev (Json::objectValue);

        lev[jss::base] =
            Logs::toString(Logs::fromSeverity(context.app.logs().threshold()));
        std::vector< std::pair<std::string, std::string> > logTable (
            context.app.logs().partition_severities());
        using stringPair = std::map<std::string, std::string>::value_type;
        for (auto const& it : logTable)
            lev[it.first] = it.second;

        ret[jss::levels] = lev;
        return ret;
    }

    LogSeverity const sv (
        Logs::fromString (context.params[jss::severity].asString ()));

    if (sv == lsINVALID)
        return rpcError (rpcINVALID_PARAMS);

    auto severity = Logs::toSeverity(sv);
    // log_level severity
    if (!context.params.isMember (jss::partition))
    {
        // set base log threshold
        context.app.logs().threshold(severity);
        return Json::objectValue;
    }

    // log_level partition severity base?
    if (context.params.isMember (jss::partition))
    {
        // set partition threshold
        std::string partition (context.params[jss::partition].asString ());

        if (boost::iequals (partition, "base"))
            context.app.logs().threshold (severity);
        else
            context.app.logs().get(partition).threshold(severity);

        return Json::objectValue;
    }

    return rpcError (rpcINVALID_PARAMS);
}

} // sdchain
