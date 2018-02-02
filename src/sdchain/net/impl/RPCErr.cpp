//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/net/RPCErr.h>
#include <sdchain/protocol/ErrorCodes.h>

namespace sdchain {

struct RPCErr;

// VFALCO NOTE Deprecated function
Json::Value rpcError (int iError, Json::Value jvResult)
{
    RPC::inject_error (iError, jvResult);
    return jvResult;
}

// VFALCO NOTE Deprecated function
bool isRpcError (Json::Value jvResult)
{
    return jvResult.isObject () && jvResult.isMember (jss::error);
}

} // sdchain
