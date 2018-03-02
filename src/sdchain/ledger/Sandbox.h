//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_LEDGER_SANDBOX_H_INCLUDED
#define SDCHAIN_LEDGER_SANDBOX_H_INCLUDED

#include <sdchain/ledger/RawView.h>
#include <sdchain/ledger/detail/ApplyViewBase.h>

namespace sdchain {

/** Discardable, editable view to a ledger.

    The sandbox inherits the flags of the base.

    @note Presented as ApplyView to clients.
*/
class Sandbox
    : public detail::ApplyViewBase
{
public:
    Sandbox() = delete;
    Sandbox (Sandbox const&) = delete;
    Sandbox& operator= (Sandbox&&) = delete;
    Sandbox& operator= (Sandbox const&) = delete;

    Sandbox (Sandbox&&) = default;

    Sandbox (ReadView const* base, ApplyFlags flags)
        : ApplyViewBase (base, flags)
    {
    }

    Sandbox (ApplyView const* base)
        : Sandbox(base, base->flags())
    {
    }

    void
    apply (RawView& to)
    {
        items_.apply(to);
    }
};

} // sdchain

#endif
