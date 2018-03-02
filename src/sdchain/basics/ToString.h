//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_BASICS_TOSTRING_H_INCLUDED
#define SDCHAIN_BASICS_TOSTRING_H_INCLUDED

#include <string>
#include <type_traits>

namespace sdchain {

/** to_string() generalizes std::to_string to handle bools, chars, and strings.

    It's also possible to provide implementation of to_string for a class
    which needs a string implementation.
 */

template <class T>
typename std::enable_if<std::is_arithmetic<T>::value,
                        std::string>::type
to_string(T t)
{
    return std::to_string(t);
}

inline std::string to_string(bool b)
{
    return b ? "true" : "false";
}

inline std::string to_string(char c)
{
    return std::string(1, c);
}

inline std::string to_string(std::string s)
{
    return s;
}

inline std::string to_string(char const* s)
{
    return s;
}

} // sdchain

#endif
