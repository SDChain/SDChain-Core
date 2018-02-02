//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012, 2013 SDChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef SDCHAIN_RPC_OUTPUT_H_INCLUDED
#define SDCHAIN_RPC_OUTPUT_H_INCLUDED

#include <boost/utility/string_ref.hpp>

namespace sdchain {
namespace RPC {

using Output = std::function <void (boost::string_ref const&)>;

inline
Output stringOutput (std::string& s)
{
    return [&](boost::string_ref const& b) { s.append (b.data(), b.size()); };
}

} // RPC
} // sdchain

#endif
