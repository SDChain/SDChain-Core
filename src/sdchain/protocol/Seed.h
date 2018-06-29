//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_SEED_H_INCLUDED
#define SDCHAIN_PROTOCOL_SEED_H_INCLUDED

#include <sdchain/basics/base_uint.h>
#include <sdchain/basics/Slice.h>
#include <sdchain/protocol/tokens.h>
#include <boost/optional.hpp>
#include <array>

namespace sdchain {

/** Seeds are used to generate deterministic secret keys. */
class Seed
{
private:
    std::array<uint8_t, 16> buf_;

public:
    Seed() = delete;

    Seed (Seed const&) = default;
    Seed& operator= (Seed const&) = default;

    /** Destroy the seed.
        The buffer will first be securely erased.
    */
    ~Seed();

    /** Construct a seed */
    /** @{ */
    explicit Seed (Slice const& slice);
    explicit Seed (uint128 const& seed);
    /** @} */

    std::uint8_t const*
    data() const
    {
        return buf_.data();
    }

    std::size_t
    size() const
    {
        return buf_.size();
    }
};

//------------------------------------------------------------------------------

/** Create a seed using secure random numbers. */
Seed
randomSeed();

/** Generate a seed deterministically.

    The algorithm is specific to SDChain:

        The seed is calculated as the first 128 bits
        of the SHA512-Half of the string text excluding
        any terminating null.

    @note This will not attempt to determine the format of
          the string (e.g. hex or base58).
*/
Seed
generateSeed (std::string const& passPhrase);

/** Parse a Base58 encoded string into a seed */
template <>
boost::optional<Seed>
parseBase58 (std::string const& s);

/** Attempt to parse a string as a seed */
boost::optional<Seed>
parseGenericSeed (std::string const& str);

/** Encode a Seed in RFC1751 format */
std::string
seedAs1751 (Seed const& seed);

/** Format a seed as a Base58 string */
inline
std::string
toBase58 (Seed const& seed)
{
    return base58EncodeToken(
        TOKEN_FAMILY_SEED, seed.data(), seed.size());
}

}

#endif
