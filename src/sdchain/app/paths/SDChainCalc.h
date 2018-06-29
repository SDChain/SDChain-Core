//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_PATHS_SDCHAINCALC_H_INCLUDED
#define SDCHAIN_APP_PATHS_SDCHAINCALC_H_INCLUDED

#include <sdchain/ledger/PaymentSandbox.h>
#include <sdchain/app/paths/PathState.h>
#include <sdchain/basics/Log.h>
#include <sdchain/protocol/STAmount.h>
#include <sdchain/protocol/TER.h>

#include <boost/container/flat_set.hpp>

namespace sdchain {
class Config;
namespace path {

namespace detail {
struct FlowDebugInfo;
}

/** SDChainCalc calculates the quality of a payment path.

    Quality is the amount of input required to produce a given output along a
    specified path - another name for this is exchange rate.
*/
class SDChainCalc
{
public:
    struct Input
    {
        bool partialPaymentAllowed = false;
        bool defaultPathsAllowed = true;
        bool limitQuality = false;
        bool isLedgerOpen = true;
    };
    struct Output
    {
        // The computed input amount.
        STAmount actualAmountIn;

        // The computed output amount.
        STAmount actualAmountOut;

        // Collection of offers found expired or unfunded. When a payment
        // succeeds, unfunded and expired offers are removed. When a payment
        // fails, they are not removed. This vector contains the offers that
        // could have been removed but were not because the payment fails. It is
        // useful for offer crossing, which does remove the offers.
        boost::container::flat_set<uint256> removableOffers;
    private:
        TER calculationResult_ = temUNKNOWN;

    public:
        TER result () const
        {
            return calculationResult_;
        }
        void setResult (TER const value)
        {
            calculationResult_ = value;
        }

    };

    static
    Output
    sdchainCalculate(
        PaymentSandbox& view,

        // Compute paths using this ledger entry set.  Up to caller to actually
        // apply to ledger.

        // Issuer:
        //      SDA: sdaAccount()
        //  non-SDA: uSrcAccountID (for any issuer) or another account with
        //           trust node.
        STAmount const& saMaxAmountReq,             // --> -1 = no limit.

        // Issuer:
        //      SDA: sdaAccount()
        //  non-SDA: uDstAccountID (for any issuer) or another account with
        //           trust node.
        STAmount const& saDstAmountReq,

        AccountID const& uDstAccountID,
        AccountID const& uSrcAccountID,

        // A set of paths that are included in the transaction that we'll
        // explore for liquidity.
        STPathSet const& spsPaths,
        Logs& l,
        Input const* const pInputs = nullptr);

    // The view we are currently working on
    PaymentSandbox& view;

    // If the transaction fails to meet some constraint, still need to delete
    // unfunded offers in a deterministic order (hence the ordered container).
    //
    // Offers that were found unfunded.
    boost::container::flat_set<uint256> permanentlyUnfundedOffers_;

    // First time working in reverse a funding source was mentioned.  Source may
    // only be used there.

    // Map of currency, issuer to node index.
    AccountIssueToNodeIndex mumSource_;
    beast::Journal j_;
    Logs& logs_;

private:
    SDChainCalc (
        PaymentSandbox& view_,
        STAmount const& saMaxAmountReq,             // --> -1 = no limit.
        STAmount const& saDstAmountReq,

        AccountID const& uDstAccountID,
        AccountID const& uSrcAccountID,
        STPathSet const& spsPaths,
        Logs& l)
            : view (view_),
              j_ (l.journal ("SDChainCalc")),
              logs_ (l),
              saDstAmountReq_(saDstAmountReq),
              saMaxAmountReq_(saMaxAmountReq),
              uDstAccountID_(uDstAccountID),
              uSrcAccountID_(uSrcAccountID),
              spsPaths_(spsPaths)
    {
    }

    /** Compute liquidity through these path sets. */
    TER sdchainCalculate (detail::FlowDebugInfo* flowDebugInfo=nullptr);

    /** Add a single PathState.  Returns true on success.*/
    bool addPathState(STPath const&, TER&);

    STAmount const& saDstAmountReq_;
    STAmount const& saMaxAmountReq_;
    AccountID const& uDstAccountID_;
    AccountID const& uSrcAccountID_;
    STPathSet const& spsPaths_;

    // The computed input amount.
    STAmount actualAmountIn_;

    // The computed output amount.
    STAmount actualAmountOut_;

    // Expanded path with all the actual nodes in it.
    // A path starts with the source account, ends with the destination account
    // and goes through other acounts or order books.
    PathState::List pathStateList_;

    Input inputFlags;
};

} // path
} // sdchain

#endif
