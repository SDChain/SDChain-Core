//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_PATHS_NODE_H_INCLUDED
#define SDCHAIN_APP_PATHS_NODE_H_INCLUDED

#include <sdchain/app/paths/NodeDirectory.h>
#include <sdchain/app/paths/Types.h>
#include <sdchain/protocol/Rate.h>
#include <sdchain/protocol/UintTypes.h>
#include <boost/optional.hpp>

namespace sdchain {
namespace path {

struct Node
{
    using List = std::vector<Node>;

    inline bool isAccount() const
    {
        return (uFlags & STPathElement::typeAccount);
    }

    Json::Value getJson () const;

    bool operator == (Node const&) const;

    std::uint16_t uFlags;       // --> From path.

    AccountID account_;         // --> Accounts: Receiving/sending account.

    Issue issue_;               // --> Accounts: Receive and send, Offers: send.
                                // --- For offer's next has currency out.

    boost::optional<Rate> transferRate_;         // Transfer rate for issuer.

    // Computed by Reverse.
    STAmount saRevRedeem;        // <-- Amount to redeem to next.
    STAmount saRevIssue;         // <-- Amount to issue to next, limited by
                                 //     credit and outstanding IOUs.  Issue
                                 //     isn't used by offers.
    STAmount saRevDeliver;       // <-- Amount to deliver to next regardless of
                                 // fee.

    // Computed by forward.
    STAmount saFwdRedeem;        // <-- Amount node will redeem to next.
    STAmount saFwdIssue;         // <-- Amount node will issue to next.
                                 //     Issue isn't used by offers.
    STAmount saFwdDeliver;       // <-- Amount to deliver to next regardless of
                                 // fee.

    // For offers:
    boost::optional<Rate> rateMax;

    // The nodes are partitioned into a buckets called "directories".
    //
    // Each "directory" contains nodes with exactly the same "quality" (meaning
    // the conversion rate between one corrency and the next).
    //
    // The "directories" are ordered in "increasing" "quality" value, which
    // means that the first "directory" has the "best" (i.e. numerically least)
    // "quality".
    // https://sdchain.io/wiki/Ledger_Format#Prioritizing_a_continuous_key_space

    NodeDirectory directory;

    STAmount saOfrRate;          // For correct ratio.

    // PaymentNode
    bool bEntryAdvance;          // Need to advance entry.
    unsigned int uEntry;
    uint256 offerIndex_;
    SLE::pointer sleOffer;
    AccountID offerOwnerAccount_;

    // Do we need to refresh saOfferFunds, saTakerPays, & saTakerGets?
    bool bFundsDirty;
    STAmount saOfferFunds;
    STAmount saTakerPays;
    STAmount saTakerGets;

    /** Clear input and output amounts. */
    void clear()
    {
        saRevRedeem.clear ();
        saRevIssue.clear ();
        saRevDeliver.clear ();
        saFwdDeliver.clear ();
    }
};

} // path
} // sdchain

#endif
