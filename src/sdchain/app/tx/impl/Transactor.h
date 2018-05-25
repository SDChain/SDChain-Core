//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_TX_TRANSACTOR_H_INCLUDED
#define SDCHAIN_APP_TX_TRANSACTOR_H_INCLUDED

#include <sdchain/app/tx/impl/ApplyContext.h>
#include <sdchain/protocol/SDAAmount.h>
#include <sdchain/beast/utility/Journal.h>
#include <boost/optional.hpp>

namespace sdchain {

/** State information when preflighting a tx. */
struct PreflightContext
{
public:
    Application& app;
    STTx const& tx;
    Rules const rules;
    ApplyFlags flags;
    beast::Journal j;

    PreflightContext(Application& app_, STTx const& tx_,
        Rules const& rules_, ApplyFlags flags_,
            beast::Journal j_);

    PreflightContext& operator=(PreflightContext const&) = delete;
};

/** State information when determining if a tx is likely to claim a fee. */
struct PreclaimContext
{
public:
    Application& app;
    ReadView const& view;
    TER preflightResult;
    STTx const& tx;
    ApplyFlags flags;
    beast::Journal j;

    PreclaimContext(Application& app_, ReadView const& view_,
        TER preflightResult_, STTx const& tx_,
            ApplyFlags flags_, beast::Journal j_ = {})
        : app(app_)
        , view(view_)
        , preflightResult(preflightResult_)
        , tx(tx_)
        , flags(flags_)
        , j(j_)
    {
    }

    PreclaimContext& operator=(PreclaimContext const&) = delete;
};

struct TxConsequences;
struct PreflightResult;

class Transactor
{
protected:
    ApplyContext& ctx_;
    beast::Journal j_;

    AccountID     account_;
    SDAAmount     mFeeDue;
    SDAAmount     mPriorBalance;  // Balance before fees.
    SDAAmount     mSourceBalance; // Balance after fees.

public:
    /** Process the transaction. */
    std::pair<TER, bool>
    operator()();

    ApplyView&
    view()
    {
        return ctx_.view();
    }

    ApplyView const&
    view() const
    {
        return ctx_.view();
    }

    /////////////////////////////////////////////////////
    /*
    These static functions are called from invoke_preclaim<Tx>
    using name hiding to accomplish compile-time polymorphism,
    so derived classes can override for different or extra
    functionality. Use with care, as these are not really
    virtual and so don't have the compiler-time protection that
    comes with it.
    */

    static
    TER
    checkSeq (PreclaimContext const& ctx);

    static
    TER
    checkFee (PreclaimContext const& ctx, std::uint64_t baseFee);

    static
    TER
    checkSign (PreclaimContext const& ctx);

    // Returns the fee in fee units, not scaled for load.
    static
    std::uint64_t
    calculateBaseFee (
        PreclaimContext const& ctx);

    static
    bool
    affectsSubsequentTransactionAuth(STTx const& tx)
    {
        return false;
    }

    static
    SDAAmount
    calculateFeePaid(STTx const& tx);

    static
    SDAAmount
    calculateMaxSpend(STTx const& tx);

    static
    TER
    preclaim(PreclaimContext const &ctx)
    {
        // Most transactors do nothing
        // after checkSeq/Fee/Sign.
        return tesSUCCESS;
    }
    /////////////////////////////////////////////////////

protected:
    TER
    apply();

    explicit
    Transactor (ApplyContext& ctx);

    virtual void preCompute();

    virtual TER doApply () = 0;

private:
    void setSeq ();
    TER payFee ();
    void claimFee (SDAAmount& fee, TER terResult, std::vector<uint256> const& removedOffers);
    static TER checkSingleSign (PreclaimContext const& ctx);
    static TER checkMultiSign (PreclaimContext const& ctx);
};

/** Performs early sanity checks on the txid */
TER
preflight0(PreflightContext const& ctx);

/** Performs early sanity checks on the account and fee fields */
TER
preflight1 (PreflightContext const& ctx);

/** Checks whether the signature appears valid */
TER
preflight2 (PreflightContext const& ctx);

}

#endif
