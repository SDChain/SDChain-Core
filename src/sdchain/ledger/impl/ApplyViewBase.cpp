//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/ledger/detail/ApplyViewBase.h>
#include <sdchain/basics/contract.h>
#include <sdchain/ledger/CashDiff.h>

namespace sdchain {
namespace detail {

ApplyViewBase::ApplyViewBase(
    ReadView const* base, ApplyFlags flags)
    : flags_ (flags)
    , base_ (base)
{
}

//---

bool
ApplyViewBase::open() const
{
    return base_->open();
}

LedgerInfo const&
ApplyViewBase::info() const
{
    return base_->info();
}

Fees const&
ApplyViewBase::fees() const
{
    return base_->fees();
}

Rules const&
ApplyViewBase::rules() const
{
    return base_->rules();
}

bool
ApplyViewBase::exists (Keylet const& k) const
{
    return items_.exists(*base_, k);
}

auto
ApplyViewBase::succ (key_type const& key,
    boost::optional<key_type> const& last) const ->
        boost::optional<key_type>
{
    return items_.succ(*base_, key, last);
}

std::shared_ptr<SLE const>
ApplyViewBase::read (Keylet const& k) const
{
    return items_.read(*base_, k);
}

auto
ApplyViewBase::slesBegin() const ->
    std::unique_ptr<sles_type::iter_base>
{
    return base_->slesBegin();
}

auto
ApplyViewBase::slesEnd() const ->
    std::unique_ptr<sles_type::iter_base>
{
    return base_->slesEnd();
}

auto
ApplyViewBase::slesUpperBound(uint256 const& key) const ->
    std::unique_ptr<sles_type::iter_base>
{
    return base_->slesUpperBound(key);
}

auto
ApplyViewBase::txsBegin() const ->
    std::unique_ptr<txs_type::iter_base>
{
    return base_->txsBegin();
}

auto
ApplyViewBase::txsEnd() const ->
    std::unique_ptr<txs_type::iter_base>
{
    return base_->txsEnd();
}

bool
ApplyViewBase::txExists (key_type const& key) const
{
    return base_->txExists(key);
}

auto
ApplyViewBase::txRead(
    key_type const& key) const ->
        tx_type
{
    return base_->txRead(key);
}

//---

ApplyFlags
ApplyViewBase::flags() const
{
    return flags_;
}

std::shared_ptr<SLE>
ApplyViewBase::peek (Keylet const& k)
{
    return items_.peek(*base_, k);
}

void
ApplyViewBase::erase(
    std::shared_ptr<SLE> const& sle)
{
    items_.erase(*base_, sle);
}

void
ApplyViewBase::insert(
    std::shared_ptr<SLE> const& sle)
{
    items_.insert(*base_, sle);
}

void
ApplyViewBase::update(
    std::shared_ptr<SLE> const& sle)
{
    items_.update(*base_, sle);
}

//---

void
ApplyViewBase::rawErase(
    std::shared_ptr<SLE> const& sle)
{
    items_.rawErase(*base_, sle);
}

void
ApplyViewBase::rawInsert(
    std::shared_ptr<SLE> const& sle)
{
    items_.insert(*base_, sle);
}

void
ApplyViewBase::rawReplace(
    std::shared_ptr<SLE> const& sle)
{
    items_.replace(*base_, sle);
}

void
ApplyViewBase::rawDestroySDA(
    SDAAmount const& fee)
{
    items_.destroySDA(fee);
}

//---

CashDiff cashFlowDiff (
    CashFilter lhsFilter, ApplyViewBase const& lhs,
    CashFilter rhsFilter, ApplyViewBase const& rhs)
{
    assert (lhs.base_ == rhs.base_);
    return CashDiff {
        *lhs.base_, lhsFilter, lhs.items_, rhsFilter, rhs.items_ };
}

} // detail
} // sdchain
