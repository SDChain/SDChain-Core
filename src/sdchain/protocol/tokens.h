//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_TOKENS_H_INCLUDED
#define SDCHAIN_PROTOCOL_TOKENS_H_INCLUDED

#include <boost/optional.hpp>
#include <cstdint>
#include <string>

namespace sdchain {

enum TokenType
{
    TOKEN_NONE              = 1,
    TOKEN_NODE_PUBLIC       = 28,
    TOKEN_NODE_PRIVATE      = 32,
    TOKEN_ACCOUNT_ID        = 0,
    TOKEN_ACCOUNT_PUBLIC    = 35,
    TOKEN_ACCOUNT_SECRET    = 34,
    TOKEN_FAMILY_GENERATOR  = 41,
    TOKEN_FAMILY_SEED       = 33
};

template <class T>
boost::optional<T>
parseBase58 (std::string const& s);

template<class T>
boost::optional<T>
parseBase58 (TokenType type, std::string const& s);

template <class T>
boost::optional<T>
parseHex (std::string const& s);

template <class T>
boost::optional<T>
parseHexOrBase58 (std::string const& s);

// Facilities for converting SDChain tokens
// to and from their human readable strings

/*  Base-58 encode a SDChain Token

    SDChain Tokens have a one-byte prefx indicating
    the type of token, followed by the data for the
    token, and finally a 4-byte checksum.

    Tokens include the following:

        Wallet Seed
        Account Public Key
        Account ID

    @param type A single byte representing the TokenType
    @param token A pointer to storage of not
                 less than 2*(size+6) bytes
    @param size the size of the token buffer in bytes
*/
std::string
base58EncodeToken (std::uint8_t type,
    void const* token, std::size_t size);

/*  Base-58 encode a Bitcoin Token
 *
 *  provided here for symmetry, but should never be needed
 *  except for testing.
 *
 *  @see base58EncodeToken for format description.
 *
 */
std::string
base58EncodeTokenBitcoin (std::uint8_t type,
    void const* token, std::size_t size);

/** Decode a Base58 token

    The type and checksum must match or an
    empty string is returned.
*/
std::string
decodeBase58Token(
    std::string const& s, int type);

/** Decode a Base58 token using Bitcoin alphabet

    The type and checksum must match or an
    empty string is returned.

    This is used to detect user error. Specifically,
    when an AccountID is specified using the wrong
    base58 alphabet, so that a better error message
    may be returned.
*/
std::string
decodeBase58TokenBitcoin(
    std::string const& s, int type);

} // sdchain

#endif
