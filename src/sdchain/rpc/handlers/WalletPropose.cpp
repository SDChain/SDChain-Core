//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/basics/strHex.h>
#include <sdchain/crypto/KeyType.h>
#include <sdchain/net/RPCErr.h>
#include <sdchain/protocol/ErrorCodes.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/protocol/PublicKey.h>
#include <sdchain/protocol/SecretKey.h>
#include <sdchain/protocol/Seed.h>
#include <sdchain/rpc/Context.h>
#include <sdchain/rpc/impl/RPCHelpers.h>
#include <sdchain/rpc/handlers/WalletPropose.h>
#include <ed25519-donna/ed25519.h>
#include <boost/optional.hpp>
#include <cmath>
#include <map>

namespace sdchain {

double
estimate_entropy (std::string const& input)
{
    // First, we calculate the Shannon entropy. This gives
    // the average number of bits per symbol that we would
    // need to encode the input.
    std::map<int, double> freq;

    for (auto const& c : input)
        freq[c]++;

    double se = 0.0;

    for (auto const& f : freq)
    {
        auto x = f.second / input.length();
        se += (x) * log2(x);
    }

    // We multiply it by the length, to get an estimate of
    // the number of bits in the input. We floor because it
    // is better to be conservative.
    return std::floor (-se * input.length());
}

// {
//  passphrase: <string>
// }
Json::Value doWalletPropose (RPC::Context& context)
{
    return walletPropose (context.params);
}

Json::Value walletPropose (Json::Value const& params)
{
    boost::optional<Seed> seed;

    KeyType keyType = KeyType::secp256k1;

    if (params.isMember (jss::key_type))
    {
        if (! params[jss::key_type].isString())
        {
            return RPC::expected_field_error (
                jss::key_type, "string");
        }

        keyType = keyTypeFromString (
            params[jss::key_type].asString());

        if (keyType == KeyType::invalid)
            return rpcError(rpcINVALID_PARAMS);
    }

    if (params.isMember (jss::passphrase) ||
        params.isMember (jss::seed) ||
        params.isMember (jss::seed_hex))
    {
        Json::Value err;
        seed = RPC::getSeedFromRPC (params, err);
        if (!seed)
            return err;
    }
    else
    {
        seed = randomSeed ();
    }

    auto const publicKey = generateKeyPair (keyType, *seed).first;

    Json::Value obj (Json::objectValue);

    auto const seed1751 = seedAs1751 (*seed);
    auto const seedHex = strHex (seed->data(), seed->size());
    auto const seedBase58 = toBase58 (*seed);

    obj[jss::master_seed] = seedBase58;
    obj[jss::master_seed_hex] = seedHex;
    obj[jss::master_key] = seed1751;
    obj[jss::account_id] = toBase58(calcAccountID(publicKey));
    obj[jss::public_key] = toBase58(TOKEN_ACCOUNT_PUBLIC, publicKey);
    obj[jss::key_type] = to_string (keyType);
    obj[jss::public_key_hex] = strHex (publicKey.data(), publicKey.size());

    // If a passphrase was specified, and it was hashed and used as a seed
    // run a quick entropy check and add an appropriate warning, because
    // "brain wallets" can be easily attacked.
    if (params.isMember (jss::passphrase))
    {
        auto const passphrase = params[jss::passphrase].asString();

        if (passphrase != seed1751 &&
            passphrase != seedBase58 &&
            passphrase != seedHex)
        {
            // 80 bits of entropy isn't bad, but it's better to
            // err on the side of caution and be conservative.
            if (estimate_entropy (passphrase) < 80.0)
                obj[jss::warning] =
                    "This wallet was generated using a user-supplied "
                    "passphrase that has low entropy and is vulnerable "
                    "to brute-force attacks.";
            else
                obj[jss::warning] =
                    "This wallet was generated using a user-supplied "
                    "passphrase. It may be vulnerable to brute-force "
                    "attacks.";
        }
    }

    return obj;
}

} // sdchain
