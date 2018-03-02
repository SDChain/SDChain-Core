//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2011 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file license.txt or http://www.opensource.org/licenses/mit-license.php.

#include <BeastConfig.h>
#include <sdchain/basics/contract.h>
#include <sdchain/crypto/impl/ec_key.h>
#include <openssl/ec.h>

namespace sdchain  {
namespace openssl {

static inline EC_KEY* get_EC_KEY (const ec_key& that)
{
    return (EC_KEY*) that.get();
}

ec_key::ec_key (const ec_key& that)
{
    if (that.ptr == nullptr)
    {
        ptr = nullptr;
        return;
    }

    ptr = (pointer_t) EC_KEY_dup (get_EC_KEY (that));

    if (ptr == nullptr)
        Throw<std::runtime_error> ("ec_key::ec_key() : EC_KEY_dup failed");

    EC_KEY_set_conv_form (get_EC_KEY (*this), POINT_CONVERSION_COMPRESSED);
}

void ec_key::destroy()
{
    if (ptr != nullptr)
    {
        EC_KEY_free (get_EC_KEY (*this));
        ptr = nullptr;
    }
}

} // openssl
} // sdchain
