//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_TX_SETACCOUNT_H_INCLUDED
#define SDCHAIN_TX_SETACCOUNT_H_INCLUDED

#include <sdchain/app/tx/impl/Transactor.h>
#include <sdchain/basics/Log.h>
#include <sdchain/core/Config.h>
#include <sdchain/protocol/Indexes.h>
#include <sdchain/protocol/Quality.h>
#include <sdchain/protocol/TxFlags.h>

namespace sdchain {

class SetAccount
    : public Transactor
{
    static std::size_t const DOMAIN_BYTES_MAX = 256;

public:
    SetAccount (ApplyContext& ctx)
        : Transactor(ctx)
    {
    }

    static
    bool
    affectsSubsequentTransactionAuth(STTx const& tx);

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
