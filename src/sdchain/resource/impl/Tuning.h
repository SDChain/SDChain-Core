//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_RESOURCE_TUNING_H_INCLUDED
#define SDCHAIN_RESOURCE_TUNING_H_INCLUDED

#include <chrono>

namespace sdchain {
namespace Resource {

/** Tunable constants. */
enum
{
    // Balance at which a warning is issued
     warningThreshold           = 500

    // Balance at which the consumer is disconnected
    ,dropThreshold              = 1500

    // The number of seconds in the exponential decay window
    // (This should be a power of two)
    ,decayWindowSeconds         = 32

    // The minimum balance required in order to include a load source in gossip
    ,minimumGossipBalance       = 100
};

// The number of seconds until an inactive table item is removed
std::chrono::seconds constexpr secondsUntilExpiration{300};

// Number of seconds until imported gossip expires
std::chrono::seconds constexpr gossipExpirationSeconds{30};

}
}

#endif
