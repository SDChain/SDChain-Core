//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2014, Howard Hinnant <howard.hinnant@gmail.com>



*/
//==============================================================================

#ifndef BEAST_TYPE_NAME_H_INCLUDED
#define BEAST_TYPE_NAME_H_INCLUDED

#include <type_traits>
#include <typeinfo>
#include <iostream>
#ifndef _MSC_VER
#include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <cstdlib>
#include <vector>

namespace beast {

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4127) // conditional expression is constant
#endif

template <typename T>
std::string
type_name()
{
    using TR = typename std::remove_reference<T>::type;
    std::unique_ptr<char, void(*)(void*)> own (
    #ifndef _MSC_VER
        abi::__cxa_demangle (typeid(TR).name(), nullptr,
            nullptr, nullptr),
    #else
            nullptr,
    #endif
            std::free
    );
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}

#ifdef _MSC_VER
#pragma warning (pop)
#endif

} // beast

#endif
