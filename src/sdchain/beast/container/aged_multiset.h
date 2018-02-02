//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#ifndef BEAST_CONTAINER_AGED_MULTISET_H_INCLUDED
#define BEAST_CONTAINER_AGED_MULTISET_H_INCLUDED

#include <sdchain/beast/container/detail/aged_ordered_container.h>

#include <chrono>
#include <functional>
#include <memory>

namespace beast {

template <
    class Key,
    class Clock = std::chrono::steady_clock,
    class Compare = std::less <Key>,
    class Allocator = std::allocator <Key>
>
using aged_multiset = detail::aged_ordered_container <
    true, false, Key, void, Clock, Compare, Allocator>;

}

#endif
