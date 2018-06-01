//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_TX_CANCELOFFER_H_INCLUDED
#define SDCHAIN_TX_CANCELOFFER_H_INCLUDED

#include <sdchain/app/tx/impl/Transactor.h>
#include <sdchain/basics/Log.h>
#include <sdchain/protocol/Indexes.h>
#include <sdchain/protocol/TxFlags.h>

namespace sdchain {

class CancelOffer
    : public Transactor
{
public:
    CancelOffer (ApplyContext& ctx)
        : Transactor(ctx)
    {
    }

    static
    TER
    preflight (PreflightContext const& ctx);

    static
    TER
    preclaim(PreclaimContext const& ctx);

    TER doApply () override;
};

}

#endif
