//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_LEDGER_PAYMENTSANDBOX_H_INCLUDED
#define SDCHAIN_LEDGER_PAYMENTSANDBOX_H_INCLUDED

#include <sdchain/ledger/RawView.h>
#include <sdchain/ledger/Sandbox.h>
#include <sdchain/ledger/detail/ApplyViewBase.h>
#include <sdchain/protocol/AccountID.h>
#include <map>
#include <utility>

namespace sdchain {

namespace detail {

// VFALCO TODO Inline this implementation
//        into the PaymentSandbox class itself
class DeferredCredits
{
public:
    struct Adjustment
    {
        Adjustment (STAmount const& d, STAmount const& c, STAmount const& b)
            : debits (d), credits (c), origBalance (b) {}
        STAmount debits;
        STAmount credits;
        STAmount origBalance;
    };

    // Get the adjustments for the balance between main and other.
    // Returns the debits, credits and the original balance
    boost::optional<Adjustment> adjustments (
        AccountID const& main,
        AccountID const& other,
        Currency const& currency) const;

    void credit (AccountID const& sender,
        AccountID const& receiver,
        STAmount const& amount,
        STAmount const& preCreditSenderBalance);

    void ownerCount (AccountID const& id,
        std::uint32_t cur,
            std::uint32_t next);

    // Get the adjusted owner count. Since DeferredCredits is meant to be used
    // in payments, and payments only decrease owner counts, return the max
    // remembered owner count.
    boost::optional<std::uint32_t>
    ownerCount (AccountID const& id) const;

    void apply (DeferredCredits& to);
private:
    // lowAccount, highAccount
    using Key = std::tuple<
        AccountID, AccountID, Currency>;
    struct Value
    {
        STAmount lowAcctCredits;
        STAmount highAcctCredits;
        STAmount lowAcctOrigBalance;
    };

    static
    Key
    makeKey (AccountID const& a1,
        AccountID const& a2,
            Currency const& c);

    std::map<Key, Value> credits_;
    std::map<AccountID, std::uint32_t> ownerCounts_;
};

} // detail

//------------------------------------------------------------------------------

/** A wrapper which makes credits unavailable to balances.

    This is used for payments and pathfinding, so that consuming
    liquidity from a path never causes portions of that path or
    other paths to gain liquidity.

    The behavior of certain free functions in the ApplyView API
    will change via the balanceHook and creditHook overrides
    of PaymentSandbox.

    @note Presented as ApplyView to clients
*/
class PaymentSandbox final
    : public detail::ApplyViewBase
{
public:
    PaymentSandbox() = delete;
    PaymentSandbox (PaymentSandbox const&) = delete;
    PaymentSandbox& operator= (PaymentSandbox&&) = delete;
    PaymentSandbox& operator= (PaymentSandbox const&) = delete;

    PaymentSandbox (PaymentSandbox&&) = default;

    PaymentSandbox (ReadView const* base, ApplyFlags flags)
        : ApplyViewBase (base, flags)
    {
    }

    PaymentSandbox (ApplyView const* base)
        : ApplyViewBase (base, base->flags())
    {
    }

    /** Construct on top of existing PaymentSandbox.

        The changes are pushed to the parent when
        apply() is called.

        @param parent A non-null pointer to the parent.

        @note A pointer is used to prevent confusion
              with copy construction.
    */
    // VFALCO If we are constructing on top of a PaymentSandbox,
    //        or a PaymentSandbox-derived class, we MUST go through
    //        one of these constructors or invariants will be broken.
    /** @{ */
    explicit
    PaymentSandbox (PaymentSandbox const* base)
        : ApplyViewBase(base, base->flags())
        , ps_ (base)
    {
    }

    explicit
    PaymentSandbox (PaymentSandbox* base)
        : ApplyViewBase(base, base->flags())
        , ps_ (base)
    {
    }
    /** @} */

    STAmount
    balanceHook (AccountID const& account,
        AccountID const& issuer,
            STAmount const& amount) const override;

    void
    creditHook (AccountID const& from,
        AccountID const& to,
            STAmount const& amount,
                STAmount const& preCreditBalance) override;

    void
    adjustOwnerCountHook (AccountID const& account,
        std::uint32_t cur,
            std::uint32_t next) override;

    std::uint32_t
    ownerCountHook (AccountID const& account,
        std::uint32_t count) const override;

    /** Apply changes to base view.

        `to` must contain contents identical to the parent
        view passed upon construction, else undefined
        behavior will result.
    */
    /** @{ */
    void apply (RawView& to);

    void
    apply (PaymentSandbox& to);
    /** @} */

    // Return a map of balance changes on trust lines. The low account is the
    // first account in the key. If the two accounts are equal, the map contains
    // the total changes in currency regardless of issuer. This is useful to get
    // the total change in SDA balances.
    std::map<std::tuple<AccountID, AccountID, Currency>, STAmount>
    balanceChanges (ReadView const& view) const;

    SDAAmount sdaDestroyed () const;

private:
    detail::DeferredCredits tab_;
    PaymentSandbox const* ps_ = nullptr;
};

}  // sdchain

#endif
