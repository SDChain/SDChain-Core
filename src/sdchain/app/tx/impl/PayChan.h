//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_TX_PAYCHAN_H_INCLUDED
#define SDCHAIN_TX_PAYCHAN_H_INCLUDED

#include <sdchain/app/tx/impl/Transactor.h>

namespace sdchain {

class PayChanCreate
    : public Transactor
{
public:
    explicit
    PayChanCreate (ApplyContext& ctx)
        : Transactor(ctx)
    {
    }

    static
    TER
    preflight (PreflightContext const& ctx);

    static
    TER
    preclaim(PreclaimContext const &ctx);

    TER
    doApply() override;
};

//------------------------------------------------------------------------------

class PayChanFund
    : public Transactor
{
public:
    explicit
    PayChanFund (ApplyContext& ctx)
        : Transactor(ctx)
    {
    }

    static
    TER
    preflight (PreflightContext const& ctx);

    TER
    doApply() override;
};

//------------------------------------------------------------------------------

class PayChanClaim
    : public Transactor
{
public:
    explicit
    PayChanClaim (ApplyContext& ctx)
        : Transactor(ctx)
    {
    }

    static
    TER
    preflight (PreflightContext const& ctx);

    TER
    doApply() override;
};

}  // sdchain

#endif
