//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#ifndef BEAST_CRYPTO_SHA2_H_INCLUDED
#define BEAST_CRYPTO_SHA2_H_INCLUDED

#include <sdchain/beast/crypto/detail/mac_facade.h>
#include <sdchain/beast/crypto/detail/sha2_context.h>

namespace beast {

using sha256_hasher = detail::mac_facade<
    detail::sha256_context, false>;

// secure version
using sha256_hasher_s = detail::mac_facade<
    detail::sha256_context, true>;

using sha512_hasher = detail::mac_facade<
    detail::sha512_context, false>;

// secure version
using sha512_hasher_s = detail::mac_facade<
    detail::sha512_context, true>;

}

#endif
