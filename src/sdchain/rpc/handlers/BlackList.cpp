//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/resource/ResourceManager.h>
#include <sdchain/rpc/Context.h>

namespace sdchain {

Json::Value doBlackList (RPC::Context& context)
{
    auto& rm = context.app.getResourceManager();
    if (context.params.isMember(jss::threshold))
        return rm.getJson(context.params[jss::threshold].asInt());
    else
        return rm.getJson();
}

} // sdchain
