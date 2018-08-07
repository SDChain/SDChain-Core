//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/paths/cursor/PathCursor.h>
#include <sdchain/basics/Log.h>
#include <tuple>

namespace sdchain {
namespace path {

TER PathCursor::forwardLiquidity () const
{
    if (node().isAccount())
        return forwardLiquidityForAccount ();

    // Otherwise, node is an offer.
    if (previousNode().account_ == zero)
        return tesSUCCESS;

    // Previous is an account node, resolve its deliver.
    STAmount saInAct;
    STAmount saInFees;

    auto resultCode = deliverNodeForward (
        previousNode().account_,
        previousNode().saFwdDeliver, // Previous is sending this much.
        saInAct,
        saInFees,
        false);

    assert (resultCode != tesSUCCESS ||
            previousNode().saFwdDeliver == saInAct + saInFees);
    return resultCode;
}

} // path
} // sdchain

// Original comments:

// Called to drive the from the first offer node in a chain.
//
// - Offer input is in issuer/limbo.
// - Current offers consumed.
//   - Current offer owners debited.
//   - Transfer fees credited to issuer.
//   - Payout to issuer or limbo.
// - Deliver is set without transfer fees.
