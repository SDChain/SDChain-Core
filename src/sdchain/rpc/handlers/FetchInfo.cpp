//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/misc/NetworkOPs.h>
#include <sdchain/json/json_value.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/rpc/Context.h>

namespace sdchain {

Json::Value doFetchInfo (RPC::Context& context)
{
    Json::Value ret (Json::objectValue);

    if (context.params.isMember(jss::clear) && context.params[jss::clear].asBool())
    {
        context.netOps.clearLedgerFetch();
        ret[jss::clear] = true;
    }

    ret[jss::info] = context.netOps.getLedgerFetchInfo();

    return ret;
}

} // sdchain
