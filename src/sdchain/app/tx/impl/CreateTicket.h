//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2014 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_TX_CREATETICKET_H_INCLUDED
#define SDCHAIN_TX_CREATETICKET_H_INCLUDED

#include <sdchain/app/ledger/Ledger.h>
#include <sdchain/app/tx/impl/Transactor.h>
#include <sdchain/basics/Log.h>
#include <sdchain/protocol/Indexes.h>

namespace sdchain {

class CreateTicket
    : public Transactor
{
public:
    CreateTicket (ApplyContext& ctx)
        : Transactor(ctx)
    {
    }

    static
    TER
    preflight (PreflightContext const& ctx);

    TER doApply () override;
};

}

#endif
