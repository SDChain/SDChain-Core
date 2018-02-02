//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_JSON_OUTPUT_H_INCLUDED
#define SDCHAIN_JSON_OUTPUT_H_INCLUDED

#include <beast/core/string.hpp>
#include <functional>

namespace Json {

class Value;

using Output = std::function <void (beast::string_view const&)>;

inline
Output stringOutput (std::string& s)
{
    return [&](beast::string_view const& b) { s.append (b.data(), b.size()); };
}

/** Writes a minimal representation of a Json value to an Output in O(n) time.

    Data is streamed right to the output, so only a marginal amount of memory is
    used.  This can be very important for a very large Json::Value.
 */
void outputJson (Json::Value const&, Output const&);

/** Return the minimal string representation of a Json::Value in O(n) time.

    This requires a memory allocation for the full size of the output.
    If possible, use outputJson().
 */
std::string jsonAsString (Json::Value const&);

} // Json

#endif
