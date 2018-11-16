//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/misc/NetworkOPs.h>
#include <sdchain/json/json_value.h>
#include <sdchain/net/RPCErr.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/rpc/Context.h>
#include <sdchain/rpc/impl/TransactionSign.h>
#include <sdchain/rpc/Role.h>

namespace sdchain {

Json::Value doServerInfo (RPC::Context& context)
{
    Json::Value ret (Json::objectValue);

    ret[jss::info] = context.netOps.getServerInfo (
        true, context.role == Role::ADMIN);

    return ret;
}

} // sdchain
