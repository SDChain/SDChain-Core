//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/paths/cursor/PathCursor.h>
#include <sdchain/basics/Log.h>
#include <sdchain/ledger/View.h>
#include <tuple>

namespace sdchain {
namespace path {

// Calculate a node and its previous nodes.  The eventual goal is to determine1
// how much input currency we need in the forward direction to satisfy the
// output.
//
// From the destination work in reverse towards the source calculating how much
// must be asked for.  As we move backwards, individual nodes may further limit
// the amount of liquidity available.
//
// This is just a controlling loop that sets things up and then hands the work
// off to either reverseLiquidityForAccount or
// reverseLiquidityForOffer.
//
// Later on the result of this will be used to work forward, figuring out how
// much can actually be delivered.
//
// <-- resultCode: tesSUCCESS or tecPATH_DRY
// <-> pnNodes:
//     --> [end]saWanted.mAmount
//     --> [all]saWanted.mCurrency
//     --> [all]saAccount
//     <-> [0]saWanted.mAmount : --> limit, <-- actual

TER PathCursor::reverseLiquidity () const
{
    // Every account has a transfer rate for its issuances.

    // TOMOVE: The account charges
    // a fee when third parties transfer that account's own issuances.

    // Cache the output transfer rate for this node.
    node().transferRate_ = transferRate (view(), node().issue_.account);

    if (node().isAccount ())
        return reverseLiquidityForAccount ();

    // Otherwise the node is an Offer.
    if (isSDA (nextNode().account_))
    {
        JLOG (j_.trace())
            << "reverseLiquidityForOffer: "
            << "OFFER --> offer: nodeIndex_=" << nodeIndex_;
        return tesSUCCESS;

        // This control structure ensures deliverNodeReverse is only called for the
        // rightmost offer in a chain of offers - which means that
        // deliverNodeReverse has to take all of those offers into consideration.
    }

    // Next is an account node, resolve current offer node's deliver.
    STAmount saDeliverAct;

    JLOG (j_.trace())
        << "reverseLiquidityForOffer: OFFER --> account:"
        << " nodeIndex_=" << nodeIndex_
        << " saRevDeliver=" << node().saRevDeliver;

    // The next node wants the current node to deliver this much:
    return deliverNodeReverse (
        nextNode().account_,
        node().saRevDeliver,
        saDeliverAct);
}

} // path
} // sdchain
