//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/peerfinder/PeerfinderManager.h>

namespace sdchain {
namespace PeerFinder {

Endpoint::Endpoint ()
    : hops (0)
{
}

Endpoint::Endpoint (beast::IP::Endpoint const& ep, int hops_)
    : hops (hops_)
    , address (ep)
{
}

bool operator< (Endpoint const& lhs, Endpoint const& rhs)
{
    return lhs.address < rhs.address;
}

}
}
