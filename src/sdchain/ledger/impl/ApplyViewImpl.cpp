//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/ledger/ApplyViewImpl.h>
#include <sdchain/basics/contract.h>
#include <cassert>

namespace sdchain {

ApplyViewImpl::ApplyViewImpl(
    ReadView const* base, ApplyFlags flags)
    : ApplyViewBase (base, flags)
{
}

void
ApplyViewImpl::apply (OpenView& to,
    STTx const& tx, TER ter,
        beast::Journal j)
{
    items_.apply(to, tx, ter, deliver_, j);
}

std::size_t
ApplyViewImpl::size ()
{
    return items_.size ();
}

void
ApplyViewImpl::visit (
    OpenView& to,
    std::function <void (
        uint256 const& key,
        bool isDelete,
        std::shared_ptr <SLE const> const& before,
        std::shared_ptr <SLE const> const& after)> const& func)
{
    items_.visit (to, func);
}

} // sdchain
