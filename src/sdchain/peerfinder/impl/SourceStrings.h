//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PEERFINDER_SOURCESTRINGS_H_INCLUDED
#define SDCHAIN_PEERFINDER_SOURCESTRINGS_H_INCLUDED

#include <sdchain/peerfinder/impl/Source.h>
#include <sdchain/beast/core/SharedPtr.h>

namespace sdchain {
namespace PeerFinder {

/** Provides addresses from a static set of strings. */
class SourceStrings : public Source
{
public:
    using Strings = std::vector <std::string>;

    static beast::SharedPtr <Source> New (std::string const& name, Strings const& strings);
};

}
}

#endif
