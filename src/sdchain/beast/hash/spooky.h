//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2014, Howard Hinnant <howard.hinnant@gmail.com>,
        Vinnie Falco <vinnie.falco@gmail.com



*/
//==============================================================================

#ifndef BEAST_HASH_SPOOKY_H_INCLUDED
#define BEAST_HASH_SPOOKY_H_INCLUDED

#include <sdchain/beast/hash/endian.h>
#include <sdchain/beast/hash/impl/spookyv2.h>

namespace beast {

// See http://burtleburtle.net/bob/hash/spooky.html
class spooky
{
private:
    SpookyHash state_;

public:
    using result_type = std::size_t;
    static beast::endian const endian = beast::endian::native;

    spooky (std::size_t seed1 = 1, std::size_t seed2 = 2) noexcept
    {
        state_.Init (seed1, seed2);
    }

    void
    operator() (void const* key, std::size_t len) noexcept
    {
        state_.Update (key, len);
    }

    explicit
    operator std::size_t() noexcept
    {
        std::uint64_t h1, h2;
        state_.Final (&h1, &h2);
        return static_cast <std::size_t> (h1);
    }
};

} // beast

#endif
