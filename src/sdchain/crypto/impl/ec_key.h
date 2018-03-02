//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2014 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_CRYPTO_EC_KEY_H_INCLUDED
#define SDCHAIN_CRYPTO_EC_KEY_H_INCLUDED

#include <sdchain/basics/base_uint.h>
#include <cstdint>

namespace sdchain {
namespace openssl {

class ec_key
{
public:
    using pointer_t = struct opaque_EC_KEY*;

    ec_key () : ptr(nullptr)
    {
    }

    ec_key (pointer_t raw) : ptr(raw)
    {
    }

    ~ec_key()
    {
        destroy();
    }

    bool valid() const
    {
        return ptr != nullptr;
    }

    pointer_t get() const  { return ptr; }

    ec_key            (const ec_key&);

    pointer_t release()
    {
        pointer_t released = ptr;

        ptr = nullptr;

        return released;
    }

private:
    pointer_t ptr;

    void destroy();

    ec_key& operator= (const ec_key&) = delete;
};

} // openssl
} // sdchain

#endif
