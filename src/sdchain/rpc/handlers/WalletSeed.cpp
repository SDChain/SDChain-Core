//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/json/json_value.h>
#include <sdchain/net/RPCErr.h>
#include <sdchain/protocol/ErrorCodes.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/protocol/Seed.h>
#include <sdchain/rpc/Context.h>

namespace sdchain {

// {
//   secret: <string>
// }
Json::Value doWalletSeed (RPC::Context& context)
{
    boost::optional<Seed> seed;

    bool bSecret = context.params.isMember (jss::secret);

    if (bSecret)
        seed = parseGenericSeed (context.params[jss::secret].asString ());
    else
        seed = randomSeed ();

    if (!seed)
        return rpcError (rpcBAD_SEED);

    Json::Value obj (Json::objectValue);
    obj[jss::seed]     = toBase58(*seed);
    obj[jss::key]      = seedAs1751(*seed);
    obj[jss::deprecated] = "Use wallet_propose instead";
    return obj;
}

} // sdchain
