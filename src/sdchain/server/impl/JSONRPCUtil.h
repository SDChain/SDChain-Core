//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_SERVER_JSONRPCUTIL_H_INCLUDED
#define SDCHAIN_SERVER_JSONRPCUTIL_H_INCLUDED

#include <sdchain/json/json_value.h>
#include <sdchain/json/Output.h>

namespace sdchain {

void HTTPReply (
    int nStatus, std::string const& strMsg, Json::Output const&, beast::Journal j);

} // sdchain

#endif
