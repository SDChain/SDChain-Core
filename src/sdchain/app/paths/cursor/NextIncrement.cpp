//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/paths/cursor/SDChainLiquidity.h>
#include <sdchain/basics/contract.h>
#include <sdchain/basics/Log.h>

namespace sdchain {
namespace path {

// Calculate the next increment of a path.
//
// The increment is what can satisfy a portion or all of the requested output at
// the best quality.
//
// <-- pathState.uQuality
//
// This is the wrapper that restores a checkpointed version of the ledger so we
// can write all over it without consequence.

void PathCursor::nextIncrement () const
{
    // The next state is what is available in preference order.
    // This is calculated when referenced accounts changed.

    auto status = liquidity();

    if (status == tesSUCCESS)
    {
        if (pathState_.isDry())
        {
            JLOG (j_.debug())
                << "nextIncrement: success on dry path:"
                << " outPass=" << pathState_.outPass()
                << " inPass=" << pathState_.inPass();
            Throw<std::runtime_error> ("Made no progress.");
        }

        // Calculate relative quality.
        pathState_.setQuality(getRate (
            pathState_.outPass(), pathState_.inPass()));
    }
    else
    {
        pathState_.setQuality(0);
    }
    pathState_.setStatus (status);
}

} // path
} // sdchain
