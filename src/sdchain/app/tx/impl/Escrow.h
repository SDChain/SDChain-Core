//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_TX_ESCROW_H_INCLUDED
#define SDCHAIN_TX_ESCROW_H_INCLUDED

#include <sdchain/app/tx/impl/Transactor.h>

namespace sdchain {

class EscrowCreate
    : public Transactor
{
public:
    explicit
    EscrowCreate (ApplyContext& ctx)
        : Transactor(ctx)
    {
    }

    static
    SDAAmount
    calculateMaxSpend(STTx const& tx);

    static
    TER
    preflight (PreflightContext const& ctx);

    TER
    doApply() override;
};

//------------------------------------------------------------------------------

class EscrowFinish
    : public Transactor
{
public:
    explicit
    EscrowFinish (ApplyContext& ctx)
        : Transactor(ctx)
    {
    }

    static
    TER
    preflight (PreflightContext const& ctx);

    static
    std::uint64_t
    calculateBaseFee (PreclaimContext const& ctx);

    TER
    doApply() override;
};

//------------------------------------------------------------------------------

class EscrowCancel
    : public Transactor
{
public:
    explicit
    EscrowCancel (ApplyContext& ctx)
        : Transactor(ctx)
    {
    }

    static
    TER
    preflight (PreflightContext const& ctx);

    TER
    doApply() override;
};

} // sdchain

#endif
