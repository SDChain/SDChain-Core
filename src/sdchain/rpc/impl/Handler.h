//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_RPC_HANDLER_H_INCLUDED
#define SDCHAIN_RPC_HANDLER_H_INCLUDED

#include <sdchain/core/Config.h>
#include <sdchain/rpc/RPCHandler.h>
#include <sdchain/rpc/Status.h>

namespace Json {
class Object;
}

namespace sdchain {
namespace RPC {

// Under what condition can we call this RPC?
enum Condition {
    NO_CONDITION     = 0,
    NEEDS_NETWORK_CONNECTION  = 1,
    NEEDS_CURRENT_LEDGER  = 2 + NEEDS_NETWORK_CONNECTION,
    NEEDS_CLOSED_LEDGER   = 4 + NEEDS_NETWORK_CONNECTION,
};

struct Handler
{
    template <class JsonValue>
    using Method = std::function <Status (Context&, JsonValue&)>;

    const char* name_;
    Method<Json::Value> valueMethod_;
    Role role_;
    RPC::Condition condition_;
};

const Handler* getHandler (std::string const&);

/** Return a Json::objectValue with a single entry. */
template <class Value>
Json::Value makeObjectValue (
    Value const& value, Json::StaticString const& field = jss::message)
{
    Json::Value result (Json::objectValue);
    result[field] = value;
    return result;
}

} // RPC
} // sdchain

#endif
