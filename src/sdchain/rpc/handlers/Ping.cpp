//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/json/json_value.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/rpc/Role.h>
#include <sdchain/rpc/Context.h>

namespace sdchain {

namespace RPC {
struct Context;
} // RPC

Json::Value doPing (RPC::Context& context)
{
    // For testing connection privileges.
    if (isUnlimited(context.role))
    {
        Json::Value ret;

        switch (context.role)
        {
            case Role::ADMIN:
                ret[jss::role] = "admin";
                break;
            case Role::IDENTIFIED:
                ret[jss::role] = "identified";
                break;
            default:
                ;
        }

        return ret;
    }
    else
    {
        return Json::Value (Json::objectValue);
    }
}

} // sdchain
