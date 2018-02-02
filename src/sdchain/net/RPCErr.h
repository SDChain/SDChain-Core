//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_NET_RPCERR_H_INCLUDED
#define SDCHAIN_NET_RPCERR_H_INCLUDED

#include <sdchain/json/json_value.h>

namespace sdchain {

// VFALCO NOTE these are deprecated
bool isRpcError (Json::Value jvResult);
Json::Value rpcError (int iError,
                      Json::Value jvResult = Json::Value (Json::objectValue));

} // sdchain

#endif
