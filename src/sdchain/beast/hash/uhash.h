//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2014, Howard Hinnant <howard.hinnant@gmail.com>,
        Vinnie Falco <vinnie.falco@gmail.com



*/
//==============================================================================

#ifndef BEAST_HASH_UHASH_H_INCLUDED
#define BEAST_HASH_UHASH_H_INCLUDED

#include <sdchain/beast/hash/hash_append.h>
#include <sdchain/beast/hash/spooky.h>

namespace beast {

// Universal hash function
template <class Hasher = spooky>
struct uhash
{
    using result_type = typename Hasher::result_type;

    template <class T>
    result_type
    operator()(T const& t) const noexcept
    {
        Hasher h;
        hash_append (h, t);
        return static_cast<result_type>(h);
    }
};

} // beast

#endif
