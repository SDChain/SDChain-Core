//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_SECP256K1_H_INCLUDED
#define SDCHAIN_PROTOCOL_SECP256K1_H_INCLUDED

#include <secp256k1/include/secp256k1.h>

namespace sdchain {

template <class = void>
secp256k1_context const*
secp256k1Context()
{
    struct holder
    {
        secp256k1_context* impl;
        holder()
            : impl (secp256k1_context_create(
                SECP256K1_CONTEXT_VERIFY |
                SECP256K1_CONTEXT_SIGN))
        {
        }

        ~holder()
        {
            secp256k1_context_destroy(impl);
        }
    };
    static holder const h;
    return h.impl;
}

} // sdchain

#endif
