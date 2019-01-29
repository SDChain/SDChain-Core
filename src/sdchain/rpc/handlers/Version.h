//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIND_SDCHAIN_RPC_HANDLERS_VERSION_H
#define SDCHAIND_SDCHAIN_RPC_HANDLERS_VERSION_H

#include <sdchain/rpc/impl/RPCHelpers.h>

namespace sdchain {
namespace RPC {

class VersionHandler
{
public:
    explicit VersionHandler (Context&) {}

    Status check()
    {
        return Status::OK;
    }

    template <class Object>
    void writeResult (Object& obj)
    {
        setVersion (obj);
    }

    static const char* const name()
    {
        return "version";
    }

    static Role role()
    {
        return Role::USER;
    }

    static Condition condition()
    {
        return NO_CONDITION;
    }
};

} // RPC
} // sdchain

#endif
