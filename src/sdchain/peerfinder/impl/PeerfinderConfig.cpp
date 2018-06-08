//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/peerfinder/PeerfinderManager.h>
#include <sdchain/peerfinder/impl/Tuning.h>

namespace sdchain {
namespace PeerFinder {

Config::Config()
    : maxPeers (Tuning::defaultMaxPeers)
    , outPeers (calcOutPeers ())
    , wantIncoming (true)
    , autoConnect (true)
    , listeningPort (0)
    , ipLimit (0)
{
}

double Config::calcOutPeers () const
{
    return std::max (
        maxPeers * Tuning::outPercent * 0.01,
            double (Tuning::minOutCount));
}

void Config::applyTuning ()
{
    if (maxPeers < Tuning::minOutCount)
        maxPeers = Tuning::minOutCount;
    outPeers = calcOutPeers ();

    auto const inPeers = maxPeers - outPeers;

    if (ipLimit == 0)
    {
        // Unless a limit is explicitly set, we allow between
        // 2 and 5 connections from non RFC-1918 "private"
        // IP addresses.
        ipLimit = 2;

        if (inPeers > Tuning::defaultMaxPeers)
            ipLimit += std::min(5,
                static_cast<int>(inPeers / Tuning::defaultMaxPeers));
    }

    // We don't allow a single IP to consume all incoming slots,
    // unless we only have one incoming slot available.
    ipLimit = std::max(1,
        std::min(ipLimit, static_cast<int>(inPeers / 2)));
}

void Config::onWrite (beast::PropertyStream::Map &map)
{
    map ["max_peers"]       = maxPeers;
    map ["out_peers"]       = outPeers;
    map ["want_incoming"]   = wantIncoming;
    map ["auto_connect"]    = autoConnect;
    map ["port"]            = listeningPort;
    map ["features"]        = features;
    map ["ip_limit"]        = ipLimit;
}

}
}
