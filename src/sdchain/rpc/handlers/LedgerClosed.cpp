//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/ledger/LedgerMaster.h>
#include <sdchain/app/misc/NetworkOPs.h>
#include <sdchain/json/json_value.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/rpc/Context.h>

namespace sdchain {

Json::Value doLedgerClosed (RPC::Context& context)
{
    auto ledger = context.ledgerMaster.getClosedLedger ();
    assert (ledger);

    Json::Value jvResult;
    jvResult[jss::ledger_index] = ledger->info().seq;
    jvResult[jss::ledger_hash] = to_string (ledger->info().hash);

    return jvResult;
}

} // sdchain
