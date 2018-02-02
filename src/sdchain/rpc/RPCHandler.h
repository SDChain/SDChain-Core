//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_RPC_RPCHANDLER_H_INCLUDED
#define SDCHAIN_RPC_RPCHANDLER_H_INCLUDED

#include <sdchain/core/Config.h>
#include <sdchain/net/InfoSub.h>
#include <sdchain/rpc/Context.h>
#include <sdchain/rpc/Status.h>

namespace sdchain {
namespace RPC {

struct Context;

/** Execute an RPC command and store the results in a Json::Value. */
Status doCommand (RPC::Context&, Json::Value&);

Role roleRequired (std::string const& method );

} // RPC
} // sdchain

#endif
