//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2015 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/crypto/KeyType.h>

namespace sdchain {

KeyType keyTypeFromString (std::string const& s)
{
    if (s == "secp256k1")  return KeyType::secp256k1;
    if (s == "ed25519"  )  return KeyType::ed25519;

    return KeyType::invalid;
}

const char* to_string (KeyType type)
{
    return   type == KeyType::secp256k1 ? "secp256k1"
           : type == KeyType::ed25519   ? "ed25519"
           :                              "INVALID";
}

}
