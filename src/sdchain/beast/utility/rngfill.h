//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2014, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#ifndef BEAST_RANDOM_RNGFILL_H_INCLUDED
#define BEAST_RANDOM_RNGFILL_H_INCLUDED

#include <array>
#include <cstdint>
#include <cstring>
#include <type_traits>

namespace beast {

template <class Generator>
void
rngfill (void* buffer, std::size_t bytes,
    Generator& g)
{
    using result_type =
        typename Generator::result_type;
    while (bytes >= sizeof(result_type))
    {
        auto const v = g();
        std::memcpy(buffer, &v, sizeof(v));
        buffer = reinterpret_cast<
            std::uint8_t*>(buffer) + sizeof(v);
        bytes -= sizeof(v);
    }
    if (bytes > 0)
    {
        auto const v = g();
        std::memcpy(buffer, &v, bytes);
    }
}

template <class Generator, std::size_t N,
    class = std::enable_if_t<
        N % sizeof(typename Generator::result_type) == 0>>
void
rngfill (std::array<std::uint8_t, N>& a, Generator& g)
{
    using result_type =
        typename Generator::result_type;
    auto i = N / sizeof(result_type);
    result_type* p =
        reinterpret_cast<result_type*>(a.data());
    while (i--)
        *p++ = g();
}

} // beast

#endif
