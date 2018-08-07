//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/paths/cursor/SDChainLiquidity.h>
#include <sdchain/basics/Log.h>
#include <sdchain/protocol/Quality.h>

namespace sdchain {
namespace path {

// The reverse pass has been narrowing by credit available and inflating by fees
// as it worked backwards.  Now, for the current account node, take the actual
// amount from previous and adjust forward balances.
//
// Perform balance adjustments between previous and current node.
// - The previous node: specifies what to push through to current.
// - All of previous output is consumed.
//
// Then, compute current node's output for next node.
// - Current node: specify what to push through to next.
// - Output to next node is computed as input minus quality or transfer fee.
// - If next node is an offer and output is non-SDA then we are the issuer and
//   do not need to push funds.
// - If next node is an offer and output is SDA then we need to deliver funds to
//   limbo.
TER PathCursor::forwardLiquidityForAccount () const
{
    TER resultCode   = tesSUCCESS;
    auto const lastNodeIndex       = pathState_.nodes().size () - 1;
    auto viewJ = sdchainCalc_.logs_.journal ("View");

    std::uint64_t uRateMax = 0;

    AccountID const& previousAccountID =
            previousNode().isAccount() ? previousNode().account_ :
            node().account_;
    // Offers are always issue.
    AccountID const& nextAccountID =
            nextNode().isAccount() ? nextNode().account_ : node().account_;

    auto const qualityIn = nodeIndex_
        ? quality_in (view(),
            node().account_,
            previousAccountID,
            node().issue_.currency)
        : parityRate;

    auto const qualityOut = (nodeIndex_ == lastNodeIndex)
        ? quality_out (view(),
            node().account_,
            nextAccountID,
            node().issue_.currency)
        : parityRate;

    // When looking backward (prv) for req we care about what we just
    // calculated: use fwd.
    // When looking forward (cur) for req we care about what was desired: use
    // rev.

    // For nextNode().isAccount()
    auto saPrvRedeemAct = previousNode().saFwdRedeem.zeroed();
    auto saPrvIssueAct = previousNode().saFwdIssue.zeroed();

    // For !previousNode().isAccount()
    auto saPrvDeliverAct = previousNode().saFwdDeliver.zeroed ();

    JLOG (j_.trace())
        << "forwardLiquidityForAccount> "
        << "nodeIndex_=" << nodeIndex_ << "/" << lastNodeIndex
        << " previousNode.saFwdRedeem:" << previousNode().saFwdRedeem
        << " saPrvIssueReq:" << previousNode().saFwdIssue
        << " previousNode.saFwdDeliver:" << previousNode().saFwdDeliver
        << " node.saRevRedeem:" << node().saRevRedeem
        << " node.saRevIssue:" << node().saRevIssue
        << " node.saRevDeliver:" << node().saRevDeliver;

    // SDChain through account.

    if (previousNode().isAccount() && nextNode().isAccount())
    {
        // Next is an account, must be rippling.

        if (!nodeIndex_)
        {
            // ^ --> ACCOUNT --> account

            // For the first node, calculate amount to sdchain based on what is
            // available.
            node().saFwdRedeem = node().saRevRedeem;

            if (pathState_.inReq() >= zero)
            {
                // Limit by send max.
                node().saFwdRedeem = std::min (
                    node().saFwdRedeem, pathState_.inReq() - pathState_.inAct());
            }

            pathState_.setInPass (node().saFwdRedeem);

            node().saFwdIssue = node().saFwdRedeem == node().saRevRedeem
                // Fully redeemed.
                ? node().saRevIssue : STAmount (node().saRevIssue);

            if (node().saFwdIssue && pathState_.inReq() >= zero)
            {
                // Limit by send max.
                node().saFwdIssue = std::min (
                    node().saFwdIssue,
                    pathState_.inReq() - pathState_.inAct() - node().saFwdRedeem);
            }

            pathState_.setInPass (pathState_.inPass() + node().saFwdIssue);

            JLOG (j_.trace())
                << "forwardLiquidityForAccount: ^ --> "
                << "ACCOUNT --> account :"
                << " saInReq=" << pathState_.inReq()
                << " saInAct=" << pathState_.inAct()
                << " node.saFwdRedeem:" << node().saFwdRedeem
                << " node.saRevIssue:" << node().saRevIssue
                << " node.saFwdIssue:" << node().saFwdIssue
                << " pathState_.saInPass:" << pathState_.inPass();
        }
        else if (nodeIndex_ == lastNodeIndex)
        {
            // account --> ACCOUNT --> $
            JLOG (j_.trace())
                << "forwardLiquidityForAccount: account --> "
                << "ACCOUNT --> $ :"
                << " previousAccountID="
                << to_string (previousAccountID)
                << " node.account_="
                << to_string (node().account_)
                << " previousNode.saFwdRedeem:" << previousNode().saFwdRedeem
                << " previousNode.saFwdIssue:" << previousNode().saFwdIssue;

            // Last node. Accept all funds. Calculate amount actually to credit.

            auto& saCurReceive = pathState_.outPass();
            STAmount saIssueCrd = qualityIn >= parityRate
                    ? previousNode().saFwdIssue  // No fee.
                    : multiplyRound (
                          previousNode().saFwdIssue,
                          qualityIn,
                          true); // Amount to credit.

            // Amount to credit. Credit for less than received as a surcharge.
            pathState_.setOutPass (previousNode().saFwdRedeem + saIssueCrd);

            if (saCurReceive)
            {
                // Actually receive.
                resultCode = sdchainCredit(view(),
                    previousAccountID,
                    node().account_,
                    previousNode().saFwdRedeem + previousNode().saFwdIssue,
                    false, viewJ);
            }
            else
            {
                // After applying quality, total payment was microscopic.
                resultCode   = tecPATH_DRY;
            }
        }
        else
        {
            // account --> ACCOUNT --> account
            JLOG (j_.trace())
                << "forwardLiquidityForAccount: account --> "
                << "ACCOUNT --> account";

            node().saFwdRedeem.clear (node().saRevRedeem);
            node().saFwdIssue.clear (node().saRevIssue);

            // Previous redeem part 1: redeem -> redeem
            if (previousNode().saFwdRedeem && node().saRevRedeem)
                // Previous wants to redeem.
            {
                // Rate : 1.0 : quality out
                sdchainLiquidity (
                    sdchainCalc_,
                    parityRate,
                    qualityOut,
                    previousNode().saFwdRedeem,
                    node().saRevRedeem,
                    saPrvRedeemAct,
                    node().saFwdRedeem,
                    uRateMax);
            }

            // Previous issue part 1: issue -> redeem
            if (previousNode().saFwdIssue != saPrvIssueAct
                // Previous wants to issue.
                && node().saRevRedeem != node().saFwdRedeem)
                // Current has more to redeem to next.
            {
                // Rate: quality in : quality out
                sdchainLiquidity (
                    sdchainCalc_,
                    qualityIn,
                    qualityOut,
                    previousNode().saFwdIssue,
                    node().saRevRedeem,
                    saPrvIssueAct,
                    node().saFwdRedeem,
                    uRateMax);
            }

            // Previous redeem part 2: redeem -> issue.
            if (previousNode().saFwdRedeem != saPrvRedeemAct
                // Previous still wants to redeem.
                && node().saRevRedeem == node().saFwdRedeem
                // Current redeeming is done can issue.
                && node().saRevIssue)
                // Current wants to issue.
            {
                // Rate : 1.0 : transfer_rate
                sdchainLiquidity (
                    sdchainCalc_,
                    parityRate,
                    transferRate (view(), node().account_),
                    previousNode().saFwdRedeem,
                    node().saRevIssue,
                    saPrvRedeemAct,
                    node().saFwdIssue,
                    uRateMax);
            }

            // Previous issue part 2 : issue -> issue
            if (previousNode().saFwdIssue != saPrvIssueAct
                // Previous wants to issue.
                && node().saRevRedeem == node().saFwdRedeem
                // Current redeeming is done can issue.
                && node().saRevIssue)
                // Current wants to issue.
            {
                // Rate: quality in : 1.0
                sdchainLiquidity (
                    sdchainCalc_,
                    qualityIn,
                    parityRate,
                    previousNode().saFwdIssue,
                    node().saRevIssue,
                    saPrvIssueAct,
                    node().saFwdIssue,
                    uRateMax);
            }

            STAmount saProvide = node().saFwdRedeem + node().saFwdIssue;

            // Adjust prv --> cur balance : take all inbound
            resultCode = saProvide
                ? sdchainCredit(view(),
                    previousAccountID,
                    node().account_,
                    previousNode().saFwdRedeem + previousNode().saFwdIssue,
                    false, viewJ)
                : tecPATH_DRY;
        }
    }
    else if (previousNode().isAccount() && !nextNode().isAccount())
    {
        // Current account is issuer to next offer.
        // Determine deliver to offer amount.
        // Don't adjust outbound balances- keep funds with issuer as limbo.
        // If issuer hold's an offer owners inbound IOUs, there is no fee and
        // redeem/issue will transparently happen.

        if (nodeIndex_)
        {
            // Non-SDA, current node is the issuer.
            JLOG (j_.trace())
                << "forwardLiquidityForAccount: account --> "
                << "ACCOUNT --> offer";

            node().saFwdDeliver.clear (node().saRevDeliver);

            // redeem -> issue/deliver.
            // Previous wants to redeem.
            // Current is issuing to an offer so leave funds in account as
            // "limbo".
            if (previousNode().saFwdRedeem)
                // Previous wants to redeem.
            {
                // Rate : 1.0 : transfer_rate
                // XXX Is having the transfer rate here correct?
                sdchainLiquidity (
                    sdchainCalc_,
                    parityRate,
                    transferRate (view(), node().account_),
                    previousNode().saFwdRedeem,
                    node().saRevDeliver,
                    saPrvRedeemAct,
                    node().saFwdDeliver,
                    uRateMax);
            }

            // issue -> issue/deliver
            if (previousNode().saFwdRedeem == saPrvRedeemAct
                // Previous done redeeming: Previous has no IOUs.
                && previousNode().saFwdIssue)
                // Previous wants to issue. To next must be ok.
            {
                // Rate: quality in : 1.0
                sdchainLiquidity (
                    sdchainCalc_,
                    qualityIn,
                    parityRate,
                    previousNode().saFwdIssue,
                    node().saRevDeliver,
                    saPrvIssueAct,
                    node().saFwdDeliver,
                    uRateMax);
            }

            // Adjust prv --> cur balance : take all inbound
            resultCode   = node().saFwdDeliver
                ? sdchainCredit(view(),
                    previousAccountID, node().account_,
                    previousNode().saFwdRedeem + previousNode().saFwdIssue,
                    false, viewJ)
                : tecPATH_DRY;  // Didn't actually deliver anything.
        }
        else
        {
            // Delivering amount requested from downstream.
            node().saFwdDeliver = node().saRevDeliver;

            // If limited, then limit by send max and available.
            if (pathState_.inReq() >= zero)
            {
                // Limit by send max.
                node().saFwdDeliver = std::min (
                    node().saFwdDeliver, pathState_.inReq() - pathState_.inAct());

                // Limit SDA by available. No limit for non-SDA as issuer.
                if (isSDA (node().issue_))
                    node().saFwdDeliver = std::min (
                        node().saFwdDeliver,
                        accountHolds(view(),
                            node().account_,
                            sdaCurrency(),
                            sdaAccount(),
                            fhIGNORE_FREEZE, viewJ)); // SDA can't be frozen

            }

            // Record amount sent for pass.
            pathState_.setInPass (node().saFwdDeliver);

            if (!node().saFwdDeliver)
            {
                resultCode   = tecPATH_DRY;
            }
            else if (!isSDA (node().issue_))
            {
                // Non-SDA, current node is the issuer.
                // We could be delivering to multiple accounts, so we don't know
                // which sdchain balance will be adjusted.  Assume just issuing.

                JLOG (j_.trace())
                    << "forwardLiquidityForAccount: ^ --> "
                    << "ACCOUNT -- !SDA --> offer";

                // As the issuer, would only issue.
                // Don't need to actually deliver. As from delivering leave in
                // the issuer as limbo.
            }
            else
            {
                JLOG (j_.trace())
                    << "forwardLiquidityForAccount: ^ --> "
                    << "ACCOUNT -- SDA --> offer";

                // Deliver SDA to limbo.
                resultCode = accountSend(view(),
                    node().account_, sdaAccount(), node().saFwdDeliver, viewJ);
            }
        }
    }
    else if (!previousNode().isAccount() && nextNode().isAccount())
    {
        if (nodeIndex_ == lastNodeIndex)
        {
            // offer --> ACCOUNT --> $
            JLOG (j_.trace())
                << "forwardLiquidityForAccount: offer --> "
                << "ACCOUNT --> $ : "
                << previousNode().saFwdDeliver;

            // Amount to credit.
            pathState_.setOutPass (previousNode().saFwdDeliver);

            // No income balance adjustments necessary.  The paying side inside
            // the offer paid to this account.
        }
        else
        {
            // offer --> ACCOUNT --> account
            JLOG (j_.trace())
                << "forwardLiquidityForAccount: offer --> "
                << "ACCOUNT --> account";

            node().saFwdRedeem.clear (node().saRevRedeem);
            node().saFwdIssue.clear (node().saRevIssue);

            // deliver -> redeem
            if (previousNode().saFwdDeliver && node().saRevRedeem)
                // Previous wants to deliver and can current redeem.
            {
                // Rate : 1.0 : quality out
                sdchainLiquidity (
                    sdchainCalc_,
                    parityRate,
                    qualityOut,
                    previousNode().saFwdDeliver,
                    node().saRevRedeem,
                    saPrvDeliverAct,
                    node().saFwdRedeem,
                    uRateMax);
            }

            // deliver -> issue
            // Wants to redeem and current would and can issue.
            if (previousNode().saFwdDeliver != saPrvDeliverAct
                // Previous still wants to deliver.
                && node().saRevRedeem == node().saFwdRedeem
                // Current has more to redeem to next.
                && node().saRevIssue)
                // Current wants issue.
            {
                // Rate : 1.0 : transfer_rate
                sdchainLiquidity (
                    sdchainCalc_,
                    parityRate,
                    transferRate (view(), node().account_),
                    previousNode().saFwdDeliver,
                    node().saRevIssue,
                    saPrvDeliverAct,
                    node().saFwdIssue,
                    uRateMax);
            }

            // No income balance adjustments necessary.  The paying side inside
            // the offer paid and the next link will receive.
            STAmount saProvide = node().saFwdRedeem + node().saFwdIssue;

            if (!saProvide)
                resultCode = tecPATH_DRY;
        }
    }
    else
    {
        // offer --> ACCOUNT --> offer
        // deliver/redeem -> deliver/issue.
        JLOG (j_.trace())
            << "forwardLiquidityForAccount: offer --> ACCOUNT --> offer";

        node().saFwdDeliver.clear (node().saRevDeliver);

        if (previousNode().saFwdDeliver && node().saRevDeliver)
        {
            // Rate : 1.0 : transfer_rate
            sdchainLiquidity (
                sdchainCalc_,
                parityRate,
                transferRate (view(), node().account_),
                previousNode().saFwdDeliver,
                node().saRevDeliver,
                saPrvDeliverAct,
                node().saFwdDeliver,
                uRateMax);
        }

        // No income balance adjustments necessary.  The paying side inside the
        // offer paid and the next link will receive.
        if (!node().saFwdDeliver)
            resultCode   = tecPATH_DRY;
    }

    return resultCode;
}

} // path
} // sdchain
