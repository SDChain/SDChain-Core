//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#ifndef BEAST_CONTAINER_DETAIL_AGED_ASSOCIATIVE_CONTAINER_H_INCLUDED
#define BEAST_CONTAINER_DETAIL_AGED_ASSOCIATIVE_CONTAINER_H_INCLUDED

#include <type_traits>

namespace beast {
namespace detail {

// Extracts the key portion of value
template <bool maybe_map>
struct aged_associative_container_extract_t
{
    template <class Value>
    decltype (Value::first) const&
    operator() (Value const& value) const
    {
        return value.first;
    }
};

template <>
struct aged_associative_container_extract_t <false>
{
    template <class Value>
    Value const&
    operator() (Value const& value) const
    {
        return value;
    }
};

}
}

#endif
