//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/paths/Credit.h>
#include <sdchain/app/paths/PathState.h>
#include <sdchain/basics/Log.h>
#include <sdchain/json/to_string.h>
#include <sdchain/protocol/Indexes.h>
#include <sdchain/protocol/JsonFields.h>
#include <boost/lexical_cast.hpp>

namespace sdchain {

// OPTIMIZE: When calculating path increment, note if increment consumes all
// liquidity. No need to revisit path in the future if all liquidity is used.
//

class SDChainCalc; // for logging

void PathState::clear()
{
    saInPass = saInReq.zeroed();
    saOutPass = saOutReq.zeroed();
    unfundedOffers_.clear ();
    umReverse.clear ();

    for (auto& node: nodes_)
        node.clear();
}

void PathState::reset(STAmount const& in, STAmount const& out)
{
    clear();

    // Update to current amount processed.
    saInAct = in;
    saOutAct = out;

    if (inReq() > zero && inAct() >= inReq())
    {
        JLOG (j_.warn())
            <<  "sdchainCalc: DONE:"
            << " inAct()=" << inAct()
            << " inReq()=" << inReq();
    }

    assert (inReq() < zero || inAct() < inReq());
    // Error if done.

    if (outAct() >= outReq())
    {
        JLOG (j_.warn())
            << "sdchainCalc: ALREADY DONE:"
            << " saOutAct=" << outAct()
            << " saOutReq=" << outReq();
    }

    assert(outAct() < outReq());
    assert (nodes().size () >= 2);
}

// Return true, iff lhs has less priority than rhs.
bool PathState::lessPriority (PathState const& lhs, PathState const& rhs)
{
    // First rank is quality.
    if (lhs.uQuality != rhs.uQuality)
        return lhs.uQuality > rhs.uQuality;     // Bigger is worse.

    // Second rank is best quantity.
    if (lhs.saOutPass != rhs.saOutPass)
        return lhs.saOutPass < rhs.saOutPass;   // Smaller is worse.

    // Third rank is path index.
    return lhs.mIndex > rhs.mIndex;             // Bigger is worse.
}

// Make sure last path node delivers to account_: currency from.issue_.account.
//
// If the unadded next node as specified by arguments would not work as is, then
// add the necessary nodes so it would work.
// PRECONDITION: the PathState must be non-empty.
//
// Rules:
// - Currencies must be converted via an offer.
// - A node names its output.

// - A sdchain nodes output issuer must be the node's account or the next node's
//   account.
// - Offers can only go directly to another offer if the currency and issuer are
//   an exact match.
// - Real issuers must be specified for non-SDA.
TER PathState::pushImpliedNodes (
    AccountID const& account,    // --> Delivering to this account.
    Currency const& currency,  // --> Delivering this currency.
    AccountID const& issuer)    // --> Delivering this issuer.
{
    TER resultCode = tesSUCCESS;

     JLOG (j_.trace()) << "pushImpliedNodes>" <<
         " " << account <<
         " " << currency <<
         " " << issuer;

    if (nodes_.back ().issue_.currency != currency)
    {
        // Currency is different, need to convert via an offer from an order
        // book.  sdaAccount() does double duty as signaling "this is an order
        // book".

        // Corresponds to "Implies an offer directory" in the diagram, currently
        // at http://goo.gl/Uj3HAB.

        auto type = isSDA(currency) ? STPathElement::typeCurrency
            : STPathElement::typeCurrency | STPathElement::typeIssuer;

        // The offer's output is what is now wanted.
        // sdaAccount() is a placeholder for offers.
        resultCode = pushNode (type, sdaAccount(), currency, issuer);
    }


    // For sdchain, non-SDA, ensure the issuer is on at least one side of the
    // transaction.
    if (resultCode == tesSUCCESS
        && !isSDA(currency)
        && nodes_.back ().account_ != issuer
        // Previous is not issuing own IOUs.
        && account != issuer)
        // Current is not receiving own IOUs.
    {
        // Need to sdchain through issuer's account.
        // Case "Implies an another node: (pushImpliedNodes)" in the document.
        // Intermediate account is the needed issuer.
        resultCode = pushNode (
            STPathElement::typeAll, issuer, currency, issuer);
    }

    JLOG (j_.trace())
        << "pushImpliedNodes< : " << transToken (resultCode);

    return resultCode;
}

// Append a node, then create and insert before it any implied nodes.  Order
// book nodes may go back to back.
//
// For each non-matching pair of IssuedCurrency, there's an order book.
//
// <-- resultCode: tesSUCCESS, temBAD_PATH, terNO_ACCOUNT, terNO_AUTH,
//                 terNO_LINE, tecPATH_DRY
TER PathState::pushNode (
    const int iType,
    AccountID const& account,    // If not specified, means an order book.
    Currency const& currency,  // If not specified, default to previous.
    AccountID const& issuer)     // If not specified, default to previous.
{
    path::Node node;
    const bool pathIsEmpty = nodes_.empty ();

    // TODO(tom): if pathIsEmpty, we probably don't need to do ANYTHING below.
    // Indeed, we might just not even call pushNode in the first place!

    auto const& backNode = pathIsEmpty ? path::Node () : nodes_.back ();

    // true, iff node is a sdchain account. false, iff node is an offer node.
    const bool hasAccount = (iType & STPathElement::typeAccount);

    // Is currency specified for the output of the current node?
    const bool hasCurrency = (iType & STPathElement::typeCurrency);

    // Issuer is specified for the output of the current node.
    const bool hasIssuer = (iType & STPathElement::typeIssuer);

    TER resultCode = tesSUCCESS;

    JLOG (j_.trace())
         << "pushNode> " << iType << ": "
         << (hasAccount ? to_string(account) : std::string("-")) << " "
         << (hasCurrency ? to_string(currency) : std::string("-")) << "/"
         << (hasIssuer ? to_string(issuer) : std::string("-")) << "/";

    node.uFlags = iType;
    node.issue_.currency = hasCurrency ?
            currency : backNode.issue_.currency;

    // TODO(tom): we can probably just return immediately whenever we hit an
    // error in these next pages.

    if (iType & ~STPathElement::typeAll)
    {
        // Of course, this could never happen.
        JLOG (j_.debug()) << "pushNode: bad bits.";
        resultCode = temBAD_PATH;
    }
    else if (hasIssuer && isSDA (node.issue_))
    {
        JLOG (j_.debug()) << "pushNode: issuer specified for SDA.";

        resultCode = temBAD_PATH;
    }
    else if (hasIssuer && !issuer)
    {
        JLOG (j_.debug()) << "pushNode: specified bad issuer.";

        resultCode = temBAD_PATH;
    }
    else if (!hasAccount && !hasCurrency && !hasIssuer)
    {
        // You can't default everything to the previous node as you would make
        // no progress.
        JLOG (j_.debug())
            << "pushNode: offer must specify at least currency or issuer.";
        resultCode = temBAD_PATH;
    }
    else if (hasAccount)
    {
        // Account link
        node.account_ = account;
        node.issue_.account = hasIssuer ? issuer :
                (isSDA (node.issue_) ? sdaAccount() : account);
        // Zero value - for accounts.
        node.saRevRedeem = STAmount ({node.issue_.currency, account});
        node.saRevIssue = node.saRevRedeem;

        // For order books only - zero currency with the issuer ID.
        node.saRevDeliver = STAmount (node.issue_);
        node.saFwdDeliver = node.saRevDeliver;

        if (pathIsEmpty)
        {
            // The first node is always correct as is.
        }
        else if (!account)
        {
            JLOG (j_.debug())
                << "pushNode: specified bad account.";
            resultCode = temBAD_PATH;
        }
        else
        {
            // Add required intermediate nodes to deliver to current account.
            JLOG (j_.trace())
                << "pushNode: imply for account.";

            resultCode = pushImpliedNodes (
                node.account_,
                node.issue_.currency,
                isSDA(node.issue_.currency) ? sdaAccount() : account);

            // Note: backNode may no longer be the immediately previous node.
        }

        if (resultCode == tesSUCCESS && !nodes_.empty ())
        {
            auto const& backNode = nodes_.back ();
            if (backNode.isAccount())
            {
                auto sleSDChainState = view().peek(
                    keylet::line(backNode.account_, node.account_, backNode.issue_.currency));

                // A "SDChainState" means a balance betweeen two accounts for a
                // specific currency.
                if (!sleSDChainState)
                {
                    JLOG (j_.trace())
                            << "pushNode: No credit line between "
                            << backNode.account_ << " and " << node.account_
                            << " for " << node.issue_.currency << "." ;

                    JLOG (j_.trace()) << getJson ();

                    resultCode   = terNO_LINE;
                }
                else
                {
                    JLOG (j_.trace())
                            << "pushNode: Credit line found between "
                            << backNode.account_ << " and " << node.account_
                            << " for " << node.issue_.currency << "." ;

                    auto sleBck  = view().peek (
                        keylet::account(backNode.account_));
                    // Is the source account the highest numbered account ID?
                    bool bHigh = backNode.account_ > node.account_;

                    if (!sleBck)
                    {
                        JLOG (j_.warn())
                            << "pushNode: delay: can't receive IOUs from "
                            << "non-existent issuer: " << backNode.account_;

                        resultCode   = terNO_ACCOUNT;
                    }
                    else if ((sleBck->getFieldU32 (sfFlags) & lsfRequireAuth) &&
                             !(sleSDChainState->getFieldU32 (sfFlags) &
                                  (bHigh ? lsfHighAuth : lsfLowAuth)) &&
                             sleSDChainState->getFieldAmount(sfBalance) == zero)
                    {
                        JLOG (j_.warn())
                                << "pushNode: delay: can't receive IOUs from "
                                << "issuer without auth.";

                        resultCode   = terNO_AUTH;
                    }

                    if (resultCode == tesSUCCESS)
                    {
                        STAmount saOwed = creditBalance (view(),
                            node.account_, backNode.account_,
                            node.issue_.currency);
                        STAmount saLimit;

                        if (saOwed <= zero)
                        {
                            saLimit = creditLimit (view(),
                                node.account_,
                                backNode.account_,
                                node.issue_.currency);
                            if (-saOwed >= saLimit)
                            {
                                JLOG (j_.debug()) <<
                                    "pushNode: dry:" <<
                                    " saOwed=" << saOwed <<
                                    " saLimit=" << saLimit;

                                resultCode   = tecPATH_DRY;
                            }
                        }
                    }
                }
            }
        }

        if (resultCode == tesSUCCESS)
            nodes_.push_back (node);
    }
    else
    {
        // Offer link.
        //
        // Offers bridge a change in currency and issuer, or just a change in
        // issuer.
        if (hasIssuer)
            node.issue_.account = issuer;
        else if (isSDA (node.issue_.currency))
            node.issue_.account = sdaAccount();
        else if (isSDA (backNode.issue_.account))
            node.issue_.account = backNode.account_;
        else
            node.issue_.account = backNode.issue_.account;

        node.saRevDeliver = STAmount (node.issue_);
        node.saFwdDeliver = node.saRevDeliver;

        if (!isConsistent (node.issue_))
        {
            JLOG (j_.debug())
                << "pushNode: currency is inconsistent with issuer.";

            resultCode = temBAD_PATH;
        }
        else if (backNode.issue_ == node.issue_)
        {
            JLOG (j_.debug()) <<
                "pushNode: bad path: offer to same currency and issuer";
            resultCode = temBAD_PATH;
        }
        else {
            JLOG (j_.trace()) << "pushNode: imply for offer.";

            // Insert intermediary issuer account if needed.
            resultCode   = pushImpliedNodes (
                sdaAccount(), // Rippling, but offers don't have an account.
                backNode.issue_.currency,
                backNode.issue_.account);
        }

        if (resultCode == tesSUCCESS)
            nodes_.push_back (node);
    }

    JLOG (j_.trace()) << "pushNode< : " << transToken (resultCode);
    return resultCode;
}

// Set this object to be an expanded path from spSourcePath - take the implied
// nodes and makes them explicit.  It also sanitizes the path.
//
// There are only two types of nodes: account nodes and order books nodes.
//
// You can infer some nodes automatically.  If you're paying me bitstamp USD,
// then there must be an intermediate bitstamp node.
//
// If you have accounts A and B, and they're delivery currency issued by C, then
// there must be a node with account C in the middle.
//
// If you're paying USD and getting bitcoins, there has to be an order book in
// between.
//
// terStatus = tesSUCCESS, temBAD_PATH, terNO_LINE, terNO_ACCOUNT, terNO_AUTH,
// or temBAD_PATH_LOOP
TER PathState::expandPath (
    STPath const& spSourcePath,
    AccountID const& uReceiverID,
    AccountID const& uSenderID)
{
    uQuality = 1;            // Mark path as active.

    Currency const& uMaxCurrencyID = saInReq.getCurrency ();
    AccountID const& uMaxIssuerID = saInReq.getIssuer ();

    Currency const& currencyOutID = saOutReq.getCurrency ();
    AccountID const& issuerOutID = saOutReq.getIssuer ();
    AccountID const& uSenderIssuerID
        = isSDA(uMaxCurrencyID) ? sdaAccount() : uSenderID;
    // Sender is always issuer for non-SDA.

    JLOG (j_.trace())
        << "expandPath> " << spSourcePath.getJson (0);

    terStatus = tesSUCCESS;

    // SDA with issuer is malformed.
    if ((isSDA (uMaxCurrencyID) && !isSDA (uMaxIssuerID))
        || (isSDA (currencyOutID) && !isSDA (issuerOutID)))
    {
        JLOG (j_.debug())
            << "expandPath> issuer with SDA";
        terStatus   = temBAD_PATH;
    }

    // Push sending node.
    // For non-SDA, issuer is always sending account.
    // - Trying to expand, not-compact.
    // - Every issuer will be traversed through.
    if (terStatus == tesSUCCESS)
    {
        terStatus   = pushNode (
            !isSDA(uMaxCurrencyID)
            ? STPathElement::typeAccount | STPathElement::typeCurrency |
              STPathElement::typeIssuer
            : STPathElement::typeAccount | STPathElement::typeCurrency,
            uSenderID,
            uMaxCurrencyID, // Max specifies the currency.
            uSenderIssuerID);
    }

    JLOG (j_.debug())
        << "expandPath: pushed:"
        << " account=" << uSenderID
        << " currency=" << uMaxCurrencyID
        << " issuer=" << uSenderIssuerID;

    // Issuer was not same as sender.
    if (tesSUCCESS == terStatus && uMaxIssuerID != uSenderIssuerID)
    {
        // May have an implied account node.
        // - If it was SDA, then issuers would have matched.

        // Figure out next node properties for implied node.
        const auto uNxtCurrencyID  = spSourcePath.size ()
                ? Currency(spSourcePath.front ().getCurrency ())
                // Use next node.
                : currencyOutID;
                // Use send.

        // TODO(tom): complexify this next logic further in case someone
        // understands it.
        const auto nextAccountID   = spSourcePath.size ()
                ? AccountID(spSourcePath. front ().getAccountID ())
                : !isSDA(currencyOutID)
                ? (issuerOutID == uReceiverID)
                ? AccountID(uReceiverID)
                : AccountID(issuerOutID)                      // Use implied node.
                : sdaAccount();

        JLOG (j_.debug())
            << "expandPath: implied check:"
            << " uMaxIssuerID=" << uMaxIssuerID
            << " uSenderIssuerID=" << uSenderIssuerID
            << " uNxtCurrencyID=" << uNxtCurrencyID
            << " nextAccountID=" << nextAccountID;

        // Can't just use push implied, because it can't compensate for next
        // account.
        if (!uNxtCurrencyID
            // Next is SDA, offer next. Must go through issuer.
            || uMaxCurrencyID != uNxtCurrencyID
            // Next is different currency, offer next...
            || uMaxIssuerID != nextAccountID)
            // Next is not implied issuer
        {
            JLOG (j_.debug())
                << "expandPath: sender implied:"
                << " account=" << uMaxIssuerID
                << " currency=" << uMaxCurrencyID
                << " issuer=" << uMaxIssuerID;

            // Add account implied by SendMax.
            terStatus = pushNode (
                !isSDA(uMaxCurrencyID)
                    ? STPathElement::typeAccount | STPathElement::typeCurrency |
                      STPathElement::typeIssuer
                    : STPathElement::typeAccount | STPathElement::typeCurrency,
                uMaxIssuerID,
                uMaxCurrencyID,
                uMaxIssuerID);
        }
    }

    for (auto & speElement: spSourcePath)
    {
        if (terStatus == tesSUCCESS)
        {
            JLOG (j_.trace()) << "expandPath: element in path";
            terStatus = pushNode (
                speElement.getNodeType (), speElement.getAccountID (),
                speElement.getCurrency (), speElement.getIssuerID ());
        }
    }

    if (terStatus == tesSUCCESS
        && !isSDA(currencyOutID)               // Next is not SDA
        && issuerOutID != uReceiverID)         // Out issuer is not receiver
    {
        assert (!nodes_.empty ());

        auto const& backNode = nodes_.back ();

        if (backNode.issue_.currency != currencyOutID  // Previous will be offer
            || backNode.account_ != issuerOutID)       // Need implied issuer
        {
            // Add implied account.
            JLOG (j_.debug())
                << "expandPath: receiver implied:"
                << " account=" << issuerOutID
                << " currency=" << currencyOutID
                << " issuer=" << issuerOutID;

            terStatus   = pushNode (
                !isSDA(currencyOutID)
                    ? STPathElement::typeAccount | STPathElement::typeCurrency |
                      STPathElement::typeIssuer
                    : STPathElement::typeAccount | STPathElement::typeCurrency,
                issuerOutID,
                currencyOutID,
                issuerOutID);
        }
    }

    if (terStatus == tesSUCCESS)
    {
        // Create receiver node.
        // Last node is always an account.

        terStatus   = pushNode (
            !isSDA(currencyOutID)
                ? STPathElement::typeAccount | STPathElement::typeCurrency |
                   STPathElement::typeIssuer
               : STPathElement::typeAccount | STPathElement::typeCurrency,
            uReceiverID,                                    // Receive to output
            currencyOutID,                                 // Desired currency
            uReceiverID);
    }

    if (terStatus == tesSUCCESS)
    {
        // Look for first mention of source in nodes and detect loops.
        // Note: The output is not allowed to be a source.
        unsigned int index = 0;
        for (auto& node: nodes_)
        {
            AccountIssue accountIssue (node.account_, node.issue_);
            if (!umForward.insert ({accountIssue, index++}).second)
            {
                // Failed to insert. Have a loop.
                JLOG (j_.debug()) <<
                    "expandPath: loop detected: " << getJson ();

                terStatus = temBAD_PATH_LOOP;
                break;
            }
        }
    }

    JLOG (j_.trace())
        << "expandPath:"
        << " in=" << uMaxCurrencyID
        << "/" << uMaxIssuerID
        << " out=" << currencyOutID
        << "/" << issuerOutID
        << ": " << getJson ();
    return terStatus;
}


/** Check if an expanded path violates freeze rules */
void PathState::checkFreeze()
{
    assert (nodes_.size() >= 2);

    // A path with no intermediaries -- pure issue/redeem
    // cannot be frozen.
    if (nodes_.size() == 2)
        return;

    SLE::pointer sle;

    for (std::size_t i = 0; i < (nodes_.size() - 1); ++i)
    {
        // Check each order book for a global freeze
        if (nodes_[i].uFlags & STPathElement::typeIssuer)
        {
            sle = view().peek (keylet::account(nodes_[i].issue_.account));

            if (sle && sle->isFlag (lsfGlobalFreeze))
            {
                terStatus = terNO_LINE;
                return;
            }
        }

        // Check each account change to make sure funds can leave
        if (nodes_[i].uFlags & STPathElement::typeAccount)
        {
            Currency const& currencyID = nodes_[i].issue_.currency;
            AccountID const& inAccount = nodes_[i].account_;
            AccountID const& outAccount = nodes_[i+1].account_;

            if (inAccount != outAccount)
            {
                sle = view().peek (keylet::account(outAccount));

                if (sle && sle->isFlag (lsfGlobalFreeze))
                {
                    terStatus = terNO_LINE;
                    return;
                }

                sle = view().peek (keylet::line(inAccount,
                        outAccount, currencyID));

                if (sle && sle->isFlag (
                    (outAccount > inAccount) ? lsfHighFreeze : lsfLowFreeze))
                {
                    terStatus = terNO_LINE;
                    return;
                }
            }
        }
    }
}

/** Check if a sequence of three accounts violates the no sdchain constrains
    [first] -> [second] -> [third]
    Disallowed if 'second' set no sdchain on [first]->[second] and
    [second]->[third]
*/
TER PathState::checkNoSDChain (
    AccountID const& firstAccount,
    AccountID const& secondAccount,
    // This is the account whose constraints we are checking
    AccountID const& thirdAccount,
    Currency const& currency)
{
    // fetch the sdchain lines into and out of this node
    SLE::pointer sleIn = view().peek (
        keylet::line(firstAccount, secondAccount, currency));
    SLE::pointer sleOut = view().peek (
        keylet::line(secondAccount, thirdAccount, currency));

    if (!sleIn || !sleOut)
    {
        terStatus = terNO_LINE;
    }
    else if (
        sleIn->getFieldU32 (sfFlags) &
            ((secondAccount > firstAccount) ? lsfHighNoSDChain : lsfLowNoSDChain) &&
        sleOut->getFieldU32 (sfFlags) &
            ((secondAccount > thirdAccount) ? lsfHighNoSDChain : lsfLowNoSDChain))
    {
        JLOG (j_.info())
            << "Path violates noSDChain constraint between "
            << firstAccount << ", "
            << secondAccount << " and "
            << thirdAccount;

        terStatus = terNO_SDCHAIN;
    }
    return terStatus;
}

// Check a fully-expanded path to make sure it doesn't violate no-SDChain
// settings.
TER PathState::checkNoSDChain (
    AccountID const& uDstAccountID,
    AccountID const& uSrcAccountID)
{
    // There must be at least one node for there to be two consecutive sdchain
    // lines.
    if (nodes_.size() == 0)
       return terStatus;

    if (nodes_.size() == 1)
    {
        // There's just one link in the path
        // We only need to check source-node-dest
        if (nodes_[0].isAccount() &&
            (nodes_[0].account_ != uSrcAccountID) &&
            (nodes_[0].account_ != uDstAccountID))
        {
            if (saInReq.getCurrency() != saOutReq.getCurrency())
            {
                terStatus = terNO_LINE;
            }
            else
            {
                terStatus = checkNoSDChain (
                    uSrcAccountID, nodes_[0].account_, uDstAccountID,
                    nodes_[0].issue_.currency);
            }
        }
        return terStatus;
    }

    // Check source <-> first <-> second
    if (nodes_[0].isAccount() &&
        nodes_[1].isAccount() &&
        (nodes_[0].account_ != uSrcAccountID))
    {
        if ((nodes_[0].issue_.currency != nodes_[1].issue_.currency))
        {
            terStatus = terNO_LINE;
            return terStatus;
        }
        else
        {
            terStatus = checkNoSDChain (
                uSrcAccountID, nodes_[0].account_, nodes_[1].account_,
                nodes_[0].issue_.currency);
            if (terStatus != tesSUCCESS)
                return terStatus;
        }
    }

    // Check second_from_last <-> last <-> destination
    size_t s = nodes_.size() - 2;
    if (nodes_[s].isAccount() &&
        nodes_[s + 1].isAccount() &&
        (uDstAccountID != nodes_[s+1].account_))
    {
        if ((nodes_[s].issue_.currency != nodes_[s+1].issue_.currency))
        {
            terStatus = terNO_LINE;
            return terStatus;
        }
        else
        {
            terStatus = checkNoSDChain (
                nodes_[s].account_, nodes_[s+1].account_,
                uDstAccountID, nodes_[s].issue_.currency);
            if (tesSUCCESS != terStatus)
                return terStatus;
        }
    }

    // Loop through all nodes that have a prior node and successor nodes
    // These are the nodes whose no sdchain constraints could be violated
    for (int i = 1; i < nodes_.size() - 1; ++i)
    {
        if (nodes_[i - 1].isAccount() &&
            nodes_[i].isAccount() &&
            nodes_[i + 1].isAccount())
        { // Two consecutive account-to-account links

            auto const& currencyID = nodes_[i].issue_.currency;
            if ((nodes_[i-1].issue_.currency != currencyID) ||
                (nodes_[i+1].issue_.currency != currencyID))
            {
                terStatus = temBAD_PATH;
                return terStatus;
            }
            terStatus = checkNoSDChain (
                nodes_[i-1].account_, nodes_[i].account_, nodes_[i+1].account_,
                currencyID);
            if (terStatus != tesSUCCESS)
                return terStatus;
        }

        if (!nodes_[i - 1].isAccount() &&
            nodes_[i].isAccount() &&
            nodes_[i + 1].isAccount() &&
            nodes_[i -1].issue_.account != nodes_[i].account_)
        { // offer -> account -> account
            auto const& currencyID = nodes_[i].issue_.currency;
            terStatus = checkNoSDChain (
                nodes_[i-1].issue_.account, nodes_[i].account_, nodes_[i+1].account_,
                currencyID);
            if (terStatus != tesSUCCESS)
                return terStatus;
        }
    }

    return tesSUCCESS;
}

// This is for debugging not end users. Output names can be changed without
// warning.
Json::Value PathState::getJson () const
{
    Json::Value jvPathState (Json::objectValue);
    Json::Value jvNodes (Json::arrayValue);

    for (auto const &pnNode: nodes_)
        jvNodes.append (pnNode.getJson ());

    jvPathState[jss::status]   = terStatus;
    jvPathState[jss::index]    = mIndex;
    jvPathState[jss::nodes]    = jvNodes;

    if (saInReq)
        jvPathState["in_req"]   = saInReq.getJson (0);

    if (saInAct)
        jvPathState["in_act"]   = saInAct.getJson (0);

    if (saInPass)
        jvPathState["in_pass"]  = saInPass.getJson (0);

    if (saOutReq)
        jvPathState["out_req"]  = saOutReq.getJson (0);

    if (saOutAct)
        jvPathState["out_act"]  = saOutAct.getJson (0);

    if (saOutPass)
        jvPathState["out_pass"] = saOutPass.getJson (0);

    if (uQuality)
        jvPathState["uQuality"] = boost::lexical_cast<std::string>(uQuality);

    return jvPathState;
}

} // sdchain
