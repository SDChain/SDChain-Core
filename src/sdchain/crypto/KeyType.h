//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2015 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_CRYPTO_KEYTYPE_H_INCLUDED
#define SDCHAIN_CRYPTO_KEYTYPE_H_INCLUDED

#include <string>

namespace sdchain {

enum class KeyType
{
    invalid = -1,
    unknown = invalid,

    secp256k1 = 0,
    ed25519   = 1,
};

KeyType keyTypeFromString (std::string const& s);

const char* to_string (KeyType type);

template <class Stream>
inline
Stream& operator<<(Stream& s, KeyType type)
{
    return s << to_string(type);
}

}

#endif
