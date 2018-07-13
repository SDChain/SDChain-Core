//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PEERFINDER_SIM_PARAMS_H_INCLUDED
#define SDCHAIN_PEERFINDER_SIM_PARAMS_H_INCLUDED

namespace sdchain {
namespace PeerFinder {
namespace Sim {

/** Defines the parameters for a network simulation. */
struct Params
{
    Params ()
        : steps (50)
        , nodes (10)
        , maxPeers (20)
        , outPeers (9.5)
        , firewalled (0)
    {
    }

    int steps;
    int nodes;
    int maxPeers;
    double outPeers;
    double firewalled; // [0, 1)
};

}
}
}

#endif
