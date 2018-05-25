//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_TX_PAYMENT_H_INCLUDED
#define SDCHAIN_TX_PAYMENT_H_INCLUDED

#include <sdchain/app/paths/SDChainCalc.h>
#include <sdchain/app/tx/impl/Transactor.h>
#include <sdchain/basics/Log.h>
#include <sdchain/protocol/TxFlags.h>

namespace sdchain {

// See https://sdchain.io/wiki/Transaction_Format#Payment_.280.29

class Payment
    : public Transactor
{
    /* The largest number of paths we allow */
    static std::size_t const MaxPathSize = 6;

    /* The longest path we allow */
    static std::size_t const MaxPathLength = 8;

public:
    Payment (ApplyContext& ctx)
        : Transactor(ctx)
    {
    }

    static
    SDAAmount
    calculateMaxSpend(STTx const& tx);

    static
    TER
    preflight (PreflightContext const& ctx);

    static
    TER
    preclaim(PreclaimContext const& ctx);

    TER doApply () override;
};

} // sdchain

#endif
