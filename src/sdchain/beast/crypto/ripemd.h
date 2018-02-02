//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#ifndef BEAST_CRYPTO_RIPEMD_H_INCLUDED
#define BEAST_CRYPTO_RIPEMD_H_INCLUDED

#include <sdchain/beast/crypto/detail/mac_facade.h>
#include <sdchain/beast/crypto/detail/ripemd_context.h>

namespace beast {

using ripemd160_hasher = detail::mac_facade<
    detail::ripemd160_context, false>;

// secure version
using ripemd160_hasher_s = detail::mac_facade<
    detail::ripemd160_context, true>;

}

#endif
