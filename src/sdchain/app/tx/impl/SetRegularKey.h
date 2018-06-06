//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_TX_SETREGULARKEY_H_INCLUDED
#define SDCHAIN_TX_SETREGULARKEY_H_INCLUDED

#include <sdchain/app/tx/impl/Transactor.h>
#include <sdchain/basics/Log.h>
#include <sdchain/protocol/TxFlags.h>
#include <sdchain/protocol/types.h>

namespace sdchain {

class SetRegularKey
    : public Transactor
{
public:
    SetRegularKey (ApplyContext& ctx)
        : Transactor(ctx)
    {
    }

    static
    bool
    affectsSubsequentTransactionAuth(STTx const& tx)
    {
        return true;
    }

    static
    TER
    preflight (PreflightContext const& ctx);

    static
    std::uint64_t
    calculateBaseFee (
        PreclaimContext const& ctx);

    TER doApply () override;
};

} // sdchain

#endif

