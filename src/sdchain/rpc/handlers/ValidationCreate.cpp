//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/basics/Log.h>
#include <sdchain/net/RPCErr.h>
#include <sdchain/protocol/ErrorCodes.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/protocol/Seed.h>
#include <sdchain/rpc/Context.h>

namespace sdchain {

static
boost::optional<Seed>
validationSeed (Json::Value const& params)
{
    if (!params.isMember (jss::secret))
        return randomSeed ();

    return parseGenericSeed (params[jss::secret].asString ());
}

// {
//   secret: <string>   // optional
// }
//
// This command requires Role::ADMIN access because it makes
// no sense to ask an untrusted server for this.
Json::Value doValidationCreate (RPC::Context& context)
{
    Json::Value     obj (Json::objectValue);

    auto seed = validationSeed(context.params);

    if (!seed)
        return rpcError (rpcBAD_SEED);

    auto const private_key = generateSecretKey (KeyType::secp256k1, *seed);

    obj[jss::validation_public_key] = toBase58 (
        TokenType::TOKEN_NODE_PUBLIC,
        derivePublicKey (KeyType::secp256k1, private_key));

    obj[jss::validation_private_key] = toBase58 (
        TokenType::TOKEN_NODE_PRIVATE, private_key);

    obj[jss::validation_seed] = toBase58 (*seed);
    obj[jss::validation_key] = seedAs1751 (*seed);

    return obj;
}

} // sdchain
