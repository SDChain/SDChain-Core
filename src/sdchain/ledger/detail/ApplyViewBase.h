//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_LEDGER_APPLYVIEWBASE_H_INCLUDED
#define SDCHAIN_LEDGER_APPLYVIEWBASE_H_INCLUDED

#include <sdchain/ledger/ApplyView.h>
#include <sdchain/ledger/CashDiff.h>
#include <sdchain/ledger/OpenView.h>
#include <sdchain/ledger/ReadView.h>
#include <sdchain/ledger/detail/ApplyStateTable.h>
#include <sdchain/protocol/SDAAmount.h>

namespace sdchain {
namespace detail {

class ApplyViewBase
    : public ApplyView
    , public RawView
{
public:
    ApplyViewBase() = delete;
    ApplyViewBase (ApplyViewBase const&) = delete;
    ApplyViewBase& operator= (ApplyViewBase&&) = delete;
    ApplyViewBase& operator= (ApplyViewBase const&) = delete;


    ApplyViewBase (ApplyViewBase&&) = default;

    ApplyViewBase(
        ReadView const* base, ApplyFlags flags);

    // ReadView
    bool
    open() const override;

    LedgerInfo const&
    info() const override;

    Fees const&
    fees() const override;

    Rules const&
    rules() const override;

    bool
    exists (Keylet const& k) const override;

    boost::optional<key_type>
    succ (key_type const& key, boost::optional<
        key_type> const& last = boost::none) const override;

    std::shared_ptr<SLE const>
    read (Keylet const& k) const override;

    std::unique_ptr<sles_type::iter_base>
    slesBegin() const override;

    std::unique_ptr<sles_type::iter_base>
    slesEnd() const override;

    std::unique_ptr<sles_type::iter_base>
    slesUpperBound(uint256 const& key) const override;

    std::unique_ptr<txs_type::iter_base>
    txsBegin() const override;

    std::unique_ptr<txs_type::iter_base>
    txsEnd() const override;

    bool
    txExists (key_type const& key) const override;

    tx_type
    txRead (key_type const& key) const override;

    // ApplyView

    ApplyFlags
    flags() const override;

    std::shared_ptr<SLE>
    peek (Keylet const& k) override;

    void
    erase (std::shared_ptr<
        SLE> const& sle) override;

    void
    insert (std::shared_ptr<
        SLE> const& sle) override;

    void
    update (std::shared_ptr<
        SLE> const& sle) override;

    // RawView

    void
    rawErase (std::shared_ptr<
        SLE> const& sle) override;

    void
    rawInsert (std::shared_ptr<
        SLE> const& sle) override;

    void
    rawReplace (std::shared_ptr<
        SLE> const& sle) override;

    void
    rawDestroySDA (
        SDAAmount const& feeDrops) override;

    friend
    CashDiff cashFlowDiff (
        CashFilter lhsFilter, ApplyViewBase const& lhs,
        CashFilter rhsFilter, ApplyViewBase const& rhs);

protected:
    ApplyFlags flags_;
    ReadView const* base_;
    detail::ApplyStateTable items_;
};

} // detail
} // sdchain

#endif
