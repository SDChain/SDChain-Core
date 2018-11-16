//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/json/JsonPropertyStream.h>
#include <sdchain/json/json_value.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/rpc/Context.h>

namespace sdchain {

Json::Value doPrint (RPC::Context& context)
{
    JsonPropertyStream stream;
    if (context.params.isObject()
        && context.params[jss::params].isArray()
        && context.params[jss::params][0u].isString ())
    {
        context.app.write (stream, context.params[jss::params][0u].asString());
    }
    else
    {
        context.app.write (stream);
    }

    return stream.top();
}

} // sdchain
