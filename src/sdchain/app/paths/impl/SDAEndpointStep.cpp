//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/paths/Credit.h>
#include <sdchain/app/paths/impl/AmountSpec.h>
#include <sdchain/app/paths/impl/Steps.h>
#include <sdchain/app/paths/impl/StepChecks.h>
#include <sdchain/basics/Log.h>
#include <sdchain/ledger/PaymentSandbox.h>
#include <sdchain/protocol/IOUAmount.h>
#include <sdchain/protocol/Quality.h>
#include <sdchain/protocol/SDAAmount.h>

#include <boost/container/flat_set.hpp>

#include <numeric>
#include <sstream>

namespace sdchain {

template <class TDerived>
class SDAEndpointStep : public StepImp<
    SDAAmount, SDAAmount, SDAEndpointStep<TDerived>>
{
private:
    AccountID acc_;
    bool const isLast_;
    beast::Journal j_;

    // Since this step will always be an endpoint in a strand
    // (either the first or last step) the same cache is used
    // for cachedIn and cachedOut and only one will ever be used
    boost::optional<SDAAmount> cache_;

    boost::optional<EitherAmount>
    cached () const
    {
        if (!cache_)
            return boost::none;
        return EitherAmount (*cache_);
    }

public:
    SDAEndpointStep (
        StrandContext const& ctx,
        AccountID const& acc)
            : acc_(acc)
            , isLast_(ctx.isLast)
            , j_ (ctx.j) {}

    AccountID const& acc () const
    {
        return acc_;
    };

    boost::optional<std::pair<AccountID,AccountID>>
    directStepAccts () const override
    {
        if (isLast_)
            return std::make_pair(sdaAccount(), acc_);
        return std::make_pair(acc_, sdaAccount());
    }

    boost::optional<EitherAmount>
    cachedIn () const override
    {
        return cached ();
    }

    boost::optional<EitherAmount>
    cachedOut () const override
    {
        return cached ();
    }

    boost::optional<Quality>
    qualityUpperBound(ReadView const& v, bool& redeems) const override;

    std::pair<SDAAmount, SDAAmount>
    revImp (
        PaymentSandbox& sb,
        ApplyView& afView,
        boost::container::flat_set<uint256>& ofrsToRm,
        SDAAmount const& out);

    std::pair<SDAAmount, SDAAmount>
    fwdImp (
        PaymentSandbox& sb,
        ApplyView& afView,
        boost::container::flat_set<uint256>& ofrsToRm,
        SDAAmount const& in);

    std::pair<bool, EitherAmount>
    validFwd (
        PaymentSandbox& sb,
        ApplyView& afView,
        EitherAmount const& in) override;

    // Check for errors and violations of frozen constraints.
    TER check (StrandContext const& ctx) const;

protected:
    SDAAmount
    sdaLiquidImpl (ReadView& sb, std::int32_t reserveReduction) const
    {
        return sdchain::sdaLiquid (sb, acc_, reserveReduction, j_);
    }

    std::string logStringImpl (char const* name) const
    {
        std::ostringstream ostr;
        ostr <<
            name << ": " <<
            "\nAcc: " << acc_;
        return ostr.str ();
    }

private:
    template <class P>
    friend bool operator==(
        SDAEndpointStep<P> const& lhs,
        SDAEndpointStep<P> const& rhs);

    friend bool operator!=(
        SDAEndpointStep const& lhs,
        SDAEndpointStep const& rhs)
    {
        return ! (lhs == rhs);
    }

    bool equal (Step const& rhs) const override
    {
        if (auto ds = dynamic_cast<SDAEndpointStep const*> (&rhs))
        {
            return *this == *ds;
        }
        return false;
    }
};

//------------------------------------------------------------------------------

// Flow is used in two different circumstances for transferring funds:
//  o Payments, and
//  o Offer crossing.
// The rules for handling funds in these two cases are almost, but not
// quite, the same.

// Payment SDAEndpointStep class (not offer crossing).
class SDAEndpointPaymentStep : public SDAEndpointStep<SDAEndpointPaymentStep>
{
public:
    using SDAEndpointStep<SDAEndpointPaymentStep>::SDAEndpointStep;

    SDAAmount
    sdaLiquid (ReadView& sb) const
    {
        return sdaLiquidImpl (sb, 0);;
    }

    std::string logString () const override
    {
        return logStringImpl ("SDAEndpointPaymentStep");
    }
};

// Offer crossing SDAEndpointStep class (not a payment).
class SDAEndpointOfferCrossingStep :
    public SDAEndpointStep<SDAEndpointOfferCrossingStep>
{
private:

    // For historical reasons, offer crossing is allowed to dig further
    // into the SDA reserve than an ordinary payment.  (I believe it's
    // because the trust line was created after the SDA was removed.)
    // Return how much the reserve should be reduced.
    //
    // Note that reduced reserve only happens if the trust line does not
    // currently exist.
    static std::int32_t computeReserveReduction (
        StrandContext const& ctx, AccountID const& acc)
    {
        if (ctx.isFirst &&
            !ctx.view.read (keylet::line (acc, ctx.strandDeliver)))
                return -1;
        return 0;
    }

public:
    SDAEndpointOfferCrossingStep (
        StrandContext const& ctx, AccountID const& acc)
    : SDAEndpointStep<SDAEndpointOfferCrossingStep> (ctx, acc)
    , reserveReduction_ (computeReserveReduction (ctx, acc))
    {
    }

    SDAAmount
    sdaLiquid (ReadView& sb) const
    {
        return sdaLiquidImpl (sb, reserveReduction_);
    }

    std::string logString () const override
    {
        return logStringImpl ("SDAEndpointOfferCrossingStep");
    }

private:
    std::int32_t const reserveReduction_;
};

//------------------------------------------------------------------------------

template <class TDerived>
inline bool operator==(SDAEndpointStep<TDerived> const& lhs,
    SDAEndpointStep<TDerived> const& rhs)
{
    return lhs.acc_ == rhs.acc_ && lhs.isLast_ == rhs.isLast_;
}

template <class TDerived>
boost::optional<Quality>
SDAEndpointStep<TDerived>::qualityUpperBound(
    ReadView const& v, bool& redeems) const
{
    redeems = this->redeems(v, true);
    return Quality{STAmount::uRateOne};
}


template <class TDerived>
std::pair<SDAAmount, SDAAmount>
SDAEndpointStep<TDerived>::revImp (
    PaymentSandbox& sb,
    ApplyView& afView,
    boost::container::flat_set<uint256>& ofrsToRm,
    SDAAmount const& out)
{
    auto const balance = static_cast<TDerived const*>(this)->sdaLiquid (sb);

    auto const result = isLast_ ? out : std::min (balance, out);

    auto& sender = isLast_ ? sdaAccount() : acc_;
    auto& receiver = isLast_ ? acc_ : sdaAccount();
    auto ter   = accountSend (sb, sender, receiver, toSTAmount (result), j_);
    if (ter != tesSUCCESS)
        return {SDAAmount{beast::zero}, SDAAmount{beast::zero}};

    cache_.emplace (result);
    return {result, result};
}

template <class TDerived>
std::pair<SDAAmount, SDAAmount>
SDAEndpointStep<TDerived>::fwdImp (
    PaymentSandbox& sb,
    ApplyView& afView,
    boost::container::flat_set<uint256>& ofrsToRm,
    SDAAmount const& in)
{
    assert (cache_);
    auto const balance = static_cast<TDerived const*>(this)->sdaLiquid (sb);

    auto const result = isLast_ ? in : std::min (balance, in);

    auto& sender = isLast_ ? sdaAccount() : acc_;
    auto& receiver = isLast_ ? acc_ : sdaAccount();
    auto ter   = accountSend (sb, sender, receiver, toSTAmount (result), j_);
    if (ter != tesSUCCESS)
        return {SDAAmount{beast::zero}, SDAAmount{beast::zero}};

    cache_.emplace (result);
    return {result, result};
}

template <class TDerived>
std::pair<bool, EitherAmount>
SDAEndpointStep<TDerived>::validFwd (
    PaymentSandbox& sb,
    ApplyView& afView,
    EitherAmount const& in)
{
    if (!cache_)
    {
        JLOG (j_.error()) << "Expected valid cache in validFwd";
        return {false, EitherAmount (SDAAmount (beast::zero))};
    }

    assert (in.native);

    auto const& sdaIn = in.sda;
    auto const balance = static_cast<TDerived const*>(this)->sdaLiquid (sb);

    if (!isLast_ && balance < sdaIn)
    {
        JLOG (j_.error()) << "SDAEndpointStep: Strand re-execute check failed."
            << " Insufficient balance: " << to_string (balance)
            << " Requested: " << to_string (sdaIn);
        return {false, EitherAmount (balance)};
    }

    if (sdaIn != *cache_)
    {
        JLOG (j_.error()) << "SDAEndpointStep: Strand re-execute check failed."
            << " ExpectedIn: " << to_string (*cache_)
            << " CachedIn: " << to_string (sdaIn);
    }
    return {true, in};
}

template <class TDerived>
TER
SDAEndpointStep<TDerived>::check (StrandContext const& ctx) const
{
    if (!acc_)
    {
        JLOG (j_.debug()) << "SDAEndpointStep: specified bad account.";
        return temBAD_PATH;
    }

    auto sleAcc = ctx.view.read (keylet::account (acc_));
    if (!sleAcc)
    {
        JLOG (j_.warn()) << "SDAEndpointStep: can't send or receive SDA from "
                             "non-existent account: "
                          << acc_;
        return terNO_ACCOUNT;
    }

    if (!ctx.isFirst && !ctx.isLast)
    {
        return temBAD_PATH;
    }

    auto& src = isLast_ ? sdaAccount () : acc_;
    auto& dst = isLast_ ? acc_ : sdaAccount();
    auto ter = checkFreeze (ctx.view, src, dst, sdaCurrency ());
    if (ter != tesSUCCESS)
        return ter;

    return tesSUCCESS;
}

//------------------------------------------------------------------------------

namespace test
{
// Needed for testing
bool sdaEndpointStepEqual (Step const& step, AccountID const& acc)
{
    if (auto xs =
        dynamic_cast<SDAEndpointStep<SDAEndpointPaymentStep> const*> (&step))
    {
        return xs->acc () == acc;
    }
    return false;
}
}

//------------------------------------------------------------------------------

std::pair<TER, std::unique_ptr<Step>>
make_SDAEndpointStep (
    StrandContext const& ctx,
    AccountID const& acc)
{
    TER ter = tefINTERNAL;
    std::unique_ptr<Step> r;
    if (ctx.offerCrossing)
    {
        auto offerCrossingStep =
            std::make_unique<SDAEndpointOfferCrossingStep> (ctx, acc);
        ter = offerCrossingStep->check (ctx);
        r = std::move (offerCrossingStep);
    }
    else // payment
    {
        auto paymentStep =
            std::make_unique<SDAEndpointPaymentStep> (ctx, acc);
        ter = paymentStep->check (ctx);
        r = std::move (paymentStep);
    }
    if (ter != tesSUCCESS)
        return {ter, nullptr};

    return {tesSUCCESS, std::move (r)};
}

} // sdchain
