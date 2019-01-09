//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/app/misc/NetworkOPs.h>
#include <sdchain/json/json_value.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/rpc/Context.h>
#include <sdchain/basics/make_lock.h>

namespace sdchain {

Json::Value doConsensusInfo (RPC::Context& context)
{
    Json::Value ret (Json::objectValue);

    {
        auto lock = make_lock(context.app.getMasterMutex());
        ret[jss::info] = context.netOps.getConsensusInfo ();
    }

    return ret;
}

} // sdchain
