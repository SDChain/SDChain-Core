//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#ifndef BEAST_CONTAINER_AGED_CONTAINER_UTILITY_H_INCLUDED
#define BEAST_CONTAINER_AGED_CONTAINER_UTILITY_H_INCLUDED

#include <sdchain/beast/container/aged_container.h>

#include <type_traits>

namespace beast {

/** Expire aged container items past the specified age. */
template <class AgedContainer, class Rep, class Period>
typename std::enable_if <
    is_aged_container <AgedContainer>::value,
    std::size_t
>::type
expire (AgedContainer& c, std::chrono::duration <Rep, Period> const& age)
{
    std::size_t n (0);
    auto const expired (c.clock().now() - age);
    for (auto iter (c.chronological.cbegin());
        iter != c.chronological.cend() &&
            iter.when() <= expired;)
    {
        iter = c.erase (iter);
        ++n;
    }
    return n;
}

}

#endif
