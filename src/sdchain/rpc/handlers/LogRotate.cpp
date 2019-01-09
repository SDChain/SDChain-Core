//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/basics/Log.h>
#include <sdchain/rpc/impl/Handler.h>

namespace sdchain {

Json::Value doLogRotate (RPC::Context& context)
{
    return RPC::makeObjectValue (context.app.logs().rotate());
}

} // sdchain
