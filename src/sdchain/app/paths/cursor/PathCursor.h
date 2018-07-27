//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_PATHS_CURSOR_PATHCURSOR_H_INCLUDED
#define SDCHAIN_APP_PATHS_CURSOR_PATHCURSOR_H_INCLUDED

#include <sdchain/app/paths/SDChainCalc.h>

namespace sdchain {
namespace path {

// The next section contains methods that compute the liquidity along a path,
// either backward or forward.
//
// We need to do these computations twice - once backward to figure out the
// maximum possible liqiudity along a path, and then forward to compute the
// actual liquidity of the paths we actually chose.
//
// Some of these routines use recursion to loop over all nodes in a path.
// TODO(tom): replace this recursion with a loop.

class PathCursor
{
public:
    PathCursor(
        SDChainCalc& sdchainCalc,
        PathState& pathState,
        bool multiQuality,
        beast::Journal j,
        NodeIndex nodeIndex = 0)
            : sdchainCalc_(sdchainCalc),
              pathState_(pathState),
              multiQuality_(multiQuality),
              nodeIndex_(restrict(nodeIndex)),
              j_ (j)
    {
    }

    void nextIncrement() const;

private:
    PathCursor(PathCursor const&) = default;

    PathCursor increment(int delta = 1) const
    {
        return {sdchainCalc_, pathState_, multiQuality_, j_, nodeIndex_ + delta};
    }

    TER liquidity() const;
    TER reverseLiquidity () const;
    TER forwardLiquidity () const;

    TER forwardLiquidityForAccount () const;
    TER reverseLiquidityForOffer () const;
    TER forwardLiquidityForOffer () const;
    TER reverseLiquidityForAccount () const;

    // To send money out of an account.
    /** advanceNode advances through offers in an order book.
        If needed, advance to next funded offer.
     - Automatically advances to first offer.
     --> bEntryAdvance: true, to advance to next entry. false, recalculate.
      <-- uOfferIndex : 0=end of list.
    */
    TER advanceNode (bool reverse) const;
    TER advanceNode (STAmount const& amount, bool reverse, bool callerHasLiquidity) const;

    // To deliver from an order book, when computing
    TER deliverNodeReverse (
        AccountID const& uOutAccountID,
        STAmount const& saOutReq,
        STAmount& saOutAct) const;

    // To deliver from an order book, when computing
    TER deliverNodeReverseImpl (
        AccountID const& uOutAccountID,
        STAmount const& saOutReq,
        STAmount& saOutAct,
        bool callerHasLiquidity) const;

    TER deliverNodeForward (
        AccountID const& uInAccountID,
        STAmount const& saInReq,
        STAmount& saInAct,
        STAmount& saInFees,
        bool callerHasLiquidity) const;

    // VFALCO TODO Rename this to view()
    PaymentSandbox&
    view() const
    {
        return pathState_.view();
    }

    NodeIndex nodeSize() const
    {
        return pathState_.nodes().size();
    }

    NodeIndex restrict(NodeIndex i) const
    {
        return std::min (i, nodeSize() - 1);
    }

    Node& node(NodeIndex i) const
    {
        return pathState_.nodes()[i];
    }

    Node& node() const
    {
        return node (nodeIndex_);
    }

    Node& previousNode() const
    {
        return node (restrict (nodeIndex_ - 1));
    }

    Node& nextNode() const
    {
        return node (restrict (nodeIndex_ + 1));
    }

    SDChainCalc& sdchainCalc_;
    PathState& pathState_;
    bool multiQuality_;
    NodeIndex nodeIndex_;
    beast::Journal j_;
};

} // path
} // sdchain

#endif
