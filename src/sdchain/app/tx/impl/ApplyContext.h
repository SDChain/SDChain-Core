//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_TX_APPLYCONTEXT_H_INCLUDED
#define SDCHAIN_TX_APPLYCONTEXT_H_INCLUDED

#include <sdchain/app/main/Application.h>
#include <sdchain/ledger/ApplyViewImpl.h>
#include <sdchain/core/Config.h>
#include <sdchain/protocol/STTx.h>
#include <sdchain/protocol/SDAAmount.h>
#include <sdchain/beast/utility/Journal.h>
#include <boost/optional.hpp>
#include <utility>

namespace sdchain {

// tx_enable_test

/** State information when applying a tx. */
class ApplyContext
{
public:
    explicit
    ApplyContext (Application& app, OpenView& base,
        STTx const& tx, TER preclaimResult,
            std::uint64_t baseFee, ApplyFlags flags,
                    beast::Journal = {});

    Application& app;
    STTx const& tx;
    TER const preclaimResult;
    std::uint64_t const baseFee;
    beast::Journal const journal;

    ApplyView&
    view()
    {
        return *view_;
    }

    ApplyView const&
    view() const
    {
        return *view_;
    }

    // VFALCO Unfortunately this is necessary
    RawView&
    rawView()
    {
        return *view_;
    }

    /** Sets the DeliveredAmount field in the metadata */
    void
    deliver (STAmount const& amount)
    {
        view_->deliver(amount);
    }

    /** Discard changes and start fresh. */
    void
    discard();

    /** Apply the transaction result to the base. */
    void
    apply (TER);

    /** Get the number of unapplied changes. */
    std::size_t
    size ();

    /** Visit unapplied changes. */
    void
    visit (std::function <void (
        uint256 const& key,
        bool isDelete,
        std::shared_ptr <SLE const> const& before,
        std::shared_ptr <SLE const> const& after)> const& func);

    void
    destroySDA (SDAAmount const& fee)
    {
        view_->rawDestroySDA(fee);
    }

    TER
    checkInvariants(TER);

private:
    template<std::size_t... Is>
    TER checkInvariantsHelper(TER terResult, std::index_sequence<Is...>);

    OpenView& base_;
    ApplyFlags flags_;
    boost::optional<ApplyViewImpl> view_;
};

} // sdchain

#endif
