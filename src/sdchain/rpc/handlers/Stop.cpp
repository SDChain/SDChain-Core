//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/json/json_value.h>
#include <sdchain/rpc/impl/Handler.h>
#include <sdchain/basics/make_lock.h>

namespace sdchain {

namespace RPC {
struct Context;
}

Json::Value doStop (RPC::Context& context)
{
    auto lock = make_lock(context.app.getMasterMutex());
    context.app.signalStop ();

    return RPC::makeObjectValue (systemName () + " server stopping");
}

} // sdchain
