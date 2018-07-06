//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/protocol/Seed.h>
#include <sdchain/basics/Buffer.h>
#include <sdchain/protocol/AccountID.h>
#include <sdchain/protocol/PublicKey.h>
#include <sdchain/protocol/SecretKey.h>
#include <sdchain/protocol/digest.h>
#include <sdchain/basics/contract.h>
#include <sdchain/crypto/RFC1751.h>
#include <sdchain/crypto/csprng.h>
#include <sdchain/beast/crypto/secure_erase.h>
#include <sdchain/beast/utility/rngfill.h>
#include <algorithm>
#include <cstring>
#include <iterator>

namespace sdchain {

Seed::~Seed()
{
    beast::secure_erase(buf_.data(), buf_.size());
}

Seed::Seed (Slice const& slice)
{
    if (slice.size() != buf_.size())
        LogicError("Seed::Seed: invalid size");
    std::memcpy(buf_.data(),
        slice.data(), buf_.size());
}

Seed::Seed (uint128 const& seed)
{
    if (seed.size() != buf_.size())
        LogicError("Seed::Seed: invalid size");
    std::memcpy(buf_.data(),
        seed.data(), buf_.size());
}

//------------------------------------------------------------------------------

Seed
randomSeed()
{
    std::array <std::uint8_t, 16> buffer;
    beast::rngfill (
        buffer.data(),
        buffer.size(),
        crypto_prng());
    Seed seed (makeSlice (buffer));
    beast::secure_erase(buffer.data(), buffer.size());
    return seed;
}

Seed
generateSeed (std::string const& passPhrase)
{
    sha512_half_hasher_s h;
    h(passPhrase.data(), passPhrase.size());
    auto const digest =
        sha512_half_hasher::result_type(h);
    return Seed({ digest.data(), 16 });
}

template <>
boost::optional<Seed>
parseBase58 (std::string const& s)
{
    auto const result = decodeBase58Token(
         s, TokenType::TOKEN_FAMILY_SEED);
    if (result.empty())
        return boost::none;
    if (result.size() != 16)
        return boost::none;
    return Seed(makeSlice(result));
}

boost::optional<Seed>
parseGenericSeed (std::string const& str)
{
    if (str.empty ())
        return boost::none;

    if (parseBase58<AccountID>(str) ||
        parseBase58<PublicKey>(TokenType::TOKEN_NODE_PUBLIC, str) ||
        parseBase58<PublicKey>(TokenType::TOKEN_ACCOUNT_PUBLIC, str) ||
        parseBase58<SecretKey>(TokenType::TOKEN_NODE_PRIVATE, str) ||
        parseBase58<SecretKey>(TokenType::TOKEN_ACCOUNT_SECRET, str))
    {
        return boost::none;
    }

    {
        uint128 seed;

        if (seed.SetHexExact (str))
            return Seed { Slice(seed.data(), seed.size()) };
    }

    if (auto seed = parseBase58<Seed> (str))
        return seed;

    {
        std::string key;
        if (RFC1751::getKeyFromEnglish (key, str) == 1)
        {
            Blob const blob (key.rbegin(), key.rend());
            return Seed{ uint128{blob} };
        }
    }

    return generateSeed (str);
}

std::string
seedAs1751 (Seed const& seed)
{
    std::string key;

    std::reverse_copy (
        seed.data(),
        seed.data() + 16,
        std::back_inserter(key));

    std::string encodedKey;
    RFC1751::getEnglishFromKey (encodedKey, key);
    return encodedKey;
}

}
