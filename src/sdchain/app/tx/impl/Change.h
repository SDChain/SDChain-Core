//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_TX_CHANGE_H_INCLUDED
#define SDCHAIN_TX_CHANGE_H_INCLUDED

#include <sdchain/app/main/Application.h>
#include <sdchain/app/misc/AmendmentTable.h>
#include <sdchain/app/misc/NetworkOPs.h>
#include <sdchain/app/tx/impl/Transactor.h>
#include <sdchain/basics/Log.h>
#include <sdchain/protocol/Indexes.h>

namespace sdchain {

class Change
    : public Transactor
{
public:
    Change (ApplyContext& ctx)
        : Transactor(ctx)
    {
    }

    static
    TER
    preflight (PreflightContext const& ctx);

    TER doApply () override;
    void preCompute() override;

    static
    std::uint64_t
    calculateBaseFee (
        PreclaimContext const& ctx)
    {
        return 0;
    }

    static
    TER
    preclaim(PreclaimContext const &ctx);

private:
    TER applyAmendment ();

    TER applyFee ();
};

}

#endif
