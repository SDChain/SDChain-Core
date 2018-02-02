//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#ifndef BEAST_CXX17_TYPE_TRAITS_H_INCLUDED
#define BEAST_CXX17_TYPE_TRAITS_H_INCLUDED

#include <tuple>
#include <type_traits>
#include <utility>

namespace std {

#ifndef _MSC_VER

template<class...>
using void_t = void;

template<bool B>
using bool_constant = std::integral_constant<bool, B>;

#endif

// Ideas from Howard Hinnant
//
// Specializations of is_constructible for pair and tuple which
// work around an apparent defect in the standard that causes well
// formed expressions involving pairs or tuples of non default-constructible
// types to generate compile errors.
//
template <class T, class U>
struct is_constructible <pair <T, U>>
    : integral_constant <bool,
        is_default_constructible <T>::value &&
        is_default_constructible <U>::value>
{
};

} // std

#endif
