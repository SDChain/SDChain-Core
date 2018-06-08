//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2014 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_BOOK_TAKER_H_INCLUDED
#define SDCHAIN_APP_BOOK_TAKER_H_INCLUDED

#include <sdchain/app/tx/impl/Offer.h>
#include <sdchain/core/Config.h>
#include <sdchain/ledger/View.h>
#include <sdchain/protocol/Quality.h>
#include <sdchain/protocol/Rate.h>
#include <sdchain/protocol/TER.h>
#include <sdchain/protocol/TxFlags.h>
#include <sdchain/beast/utility/Journal.h>
#include <functional>

namespace sdchain {

/** The flavor of an offer crossing */
enum class CrossType
{
    SdaToIou,
    IouToSda,
    IouToIou
};

/** State for the active party during order book or payment operations. */
class BasicTaker
{
private:
    AccountID account_;
    Quality quality_;
    Quality threshold_;

    bool sell_;

    // The original in and out quantities.
    Amounts const original_;

    // The amounts still left over for us to try and take.
    Amounts remaining_;

    // The issuers for the input and output
    Issue const& issue_in_;
    Issue const& issue_out_;

    // The rates that will be paid when the input and output currencies are
    // transfered and the currency issuer isn't involved:
    Rate const m_rate_in;
    Rate const m_rate_out;

    // The type of crossing that we are performing
    CrossType cross_type_;

protected:
    beast::Journal journal_;

    struct Flow
    {
        Amounts order;
        Amounts issuers;

        bool sanity_check () const
        {
            if (isSDA (order.in) && isSDA (order.out))
                return false;

            return order.in >= zero &&
                order.out >= zero &&
                issuers.in >= zero &&
                issuers.out >= zero;
        }
    };

private:
    void
    log_flow (char const* description, Flow const& flow);

    Flow
    flow_sda_to_iou (Amounts const& offer, Quality quality,
        STAmount const& owner_funds, STAmount const& taker_funds,
        Rate const& rate_out);

    Flow
    flow_iou_to_sda (Amounts const& offer, Quality quality,
        STAmount const& owner_funds, STAmount const& taker_funds,
        Rate const& rate_in);

    Flow
    flow_iou_to_iou (Amounts const& offer, Quality quality,
        STAmount const& owner_funds, STAmount const& taker_funds,
        Rate const& rate_in, Rate const& rate_out);

    // Calculates the transfer rate that we should use when calculating
    // flows for a particular issue between two accounts.
    static
    Rate
    effective_rate (Rate const& rate, Issue const &issue,
        AccountID const& from, AccountID const& to);

    // The transfer rate for the input currency between the given accounts
    Rate
    in_rate (AccountID const& from, AccountID const& to) const
    {
        return effective_rate (m_rate_in, original_.in.issue (), from, to);
    }

    // The transfer rate for the output currency between the given accounts
    Rate
    out_rate (AccountID const& from, AccountID const& to) const
    {
        return effective_rate (m_rate_out, original_.out.issue (), from, to);
    }

public:
    BasicTaker () = delete;
    BasicTaker (BasicTaker const&) = delete;

    BasicTaker (
        CrossType cross_type, AccountID const& account, Amounts const& amount,
        Quality const& quality, std::uint32_t flags, Rate const& rate_in,
        Rate const& rate_out, beast::Journal journal = beast::Journal ());

    virtual ~BasicTaker () = default;

    /** Returns the amount remaining on the offer.
        This is the amount at which the offer should be placed. It may either
        be for the full amount when there were no crossing offers, or for zero
        when the offer fully crossed, or any amount in between.
        It is always at the original offer quality (quality_)
    */
    Amounts
    remaining_offer () const;

    /** Returns the amount that the offer was originally placed at. */
    Amounts const&
    original_offer () const;

    /** Returns the account identifier of the taker. */
    AccountID const&
    account () const noexcept
    {
        return account_;
    }

    /** Returns `true` if the quality does not meet the taker's requirements. */
    bool
    reject (Quality const& quality) const noexcept
    {
        return quality < threshold_;
    }

    /** Returns the type of crossing that is being performed */
    CrossType
    cross_type () const
    {
        return cross_type_;
    }

    /** Returns the Issue associated with the input of the offer */
    Issue const&
    issue_in () const
    {
        return issue_in_;
    }

    /** Returns the Issue associated with the output of the offer */
    Issue const&
    issue_out () const
    {
        return issue_out_;
    }

    /** Returns `true` if the taker has run out of funds. */
    bool
    unfunded () const;

    /** Returns `true` if order crossing should not continue.
        Order processing is stopped if the taker's order quantities have
        been reached, or if the taker has run out of input funds.
    */
    bool
    done () const;

    /** Perform direct crossing through given offer.
        @return an `Amounts` describing the flow achieved during cross
    */
    BasicTaker::Flow
    do_cross (Amounts offer, Quality quality, AccountID const& owner);

    /** Perform bridged crossing through given offers.
        @return a pair of `Amounts` describing the flow achieved during cross
    */
    std::pair<BasicTaker::Flow, BasicTaker::Flow>
    do_cross (
        Amounts offer1, Quality quality1, AccountID const& owner1,
        Amounts offer2, Quality quality2, AccountID const& owner2);

    virtual
    STAmount
    get_funds (AccountID const& account, STAmount const& funds) const = 0;
};

//------------------------------------------------------------------------------

class Taker
    : public BasicTaker
{
public:
    Taker () = delete;
    Taker (Taker const&) = delete;

    Taker (CrossType cross_type, ApplyView& view,
        AccountID const& account, Amounts const& offer,
            std::uint32_t flags,
                beast::Journal journal);
    ~Taker () = default;

    void
    consume_offer (Offer& offer, Amounts const& order);

    STAmount
    get_funds (AccountID const& account, STAmount const& funds) const;

    STAmount const&
    get_sda_flow () const
    {
        return sda_flow_;
    }

    std::uint32_t
    get_direct_crossings () const
    {
        return direct_crossings_;
    }

    std::uint32_t
    get_bridge_crossings () const
    {
        return bridge_crossings_;
    }

    /** Perform a direct or bridged offer crossing as appropriate.
        Funds will be transferred accordingly, and offers will be adjusted.
        @return tesSUCCESS if successful, or an error code otherwise.
    */
    /** @{ */
    TER
    cross (Offer& offer);

    TER
    cross (Offer& leg1, Offer& leg2);
    /** @} */

private:
    static
    Rate
    calculateRate (ApplyView const& view,
        AccountID const& issuer,
            AccountID const& account);

    TER
    fill (BasicTaker::Flow const& flow, Offer& offer);

    TER
    fill (
        BasicTaker::Flow const& flow1, Offer& leg1,
        BasicTaker::Flow const& flow2, Offer& leg2);

    TER
    transferSDA (AccountID const& from, AccountID const& to, STAmount const& amount);

    TER
    redeemIOU (AccountID const& account, STAmount const& amount, Issue const& issue);

    TER
    issueIOU (AccountID const& account, STAmount const& amount, Issue const& issue);

private:
    // The underlying ledger entry we are dealing with
    ApplyView& view_;

    // The amount of SDA that flowed if we were autobridging
    STAmount sda_flow_;

    // The number direct crossings that we performed
    std::uint32_t direct_crossings_;

    // The number autobridged crossings that we performed
    std::uint32_t bridge_crossings_;
};

}

#endif
