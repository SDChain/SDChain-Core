//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2014, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#ifndef BEAST_HASH_XXHASHER_H_INCLUDED
#define BEAST_HASH_XXHASHER_H_INCLUDED

#ifndef BEAST_NO_XXHASH
#define BEAST_NO_XXHASH 0
#endif

#if ! BEAST_NO_XXHASH

#include <sdchain/beast/hash/endian.h>
#include <sdchain/beast/hash/impl/xxhash.h>
#include <type_traits>
#include <cstddef>

namespace beast {

class xxhasher
{
private:
    // requires 64-bit std::size_t
    static_assert(sizeof(std::size_t)==8, "");

    detail::XXH64_state_t state_;

public:
    using result_type = std::size_t;

    static beast::endian const endian = beast::endian::native;

    xxhasher() noexcept
    {
        detail::XXH64_reset (&state_, 1);
    }

    template <class Seed,
        std::enable_if_t<
            std::is_unsigned<Seed>::value>* = nullptr>
    explicit
    xxhasher (Seed seed)
    {
        detail::XXH64_reset (&state_, seed);
    }

    template <class Seed,
        std::enable_if_t<
            std::is_unsigned<Seed>::value>* = nullptr>
    xxhasher (Seed seed, Seed)
    {
        detail::XXH64_reset (&state_, seed);
    }

    void
    operator()(void const* key, std::size_t len) noexcept
    {
        detail::XXH64_update (&state_, key, len);
    }

    explicit
    operator std::size_t() noexcept
    {
        return detail::XXH64_digest(&state_);
    }
};

} // beast

#endif

#endif
