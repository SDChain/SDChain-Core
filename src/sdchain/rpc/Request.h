//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012, 2013 SDChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef SDCHAIN_RPC_REQUEST_H_INCLUDED
#define SDCHAIN_RPC_REQUEST_H_INCLUDED

#include <sdchain/resource/Charge.h>
#include <sdchain/resource/Fees.h>
#include <sdchain/json/json_value.h>
#include <beast/utility/Journal.h>

namespace sdchain {

class Application;

namespace RPC {

struct Request
{
    explicit Request (
        beast::Journal journal_,
        std::string const& method_,
        Json::Value& params_,
        Application& app_)
        : journal (journal_)
        , method (method_)
        , params (params_)
        , fee (Resource::feeReferenceRPC)
        , app (app_)
    {
    }

    // [in] The Journal for logging
    beast::Journal journal;

    // [in] The JSON-RPC method
    std::string method;

    // [in] The SDChain-specific "params" object
    Json::Value params;

    // [in, out] The resource cost for the command
    Resource::Charge fee;

    // [out] The JSON-RPC response
    Json::Value result;

    // [in] The Application instance
    Application& app;

private:
    Request& operator= (Request const&);
};

}
}

#endif
