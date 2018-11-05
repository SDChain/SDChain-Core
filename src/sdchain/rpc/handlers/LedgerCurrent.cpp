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
#include <sdchain/net/RPCErr.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/rpc/Context.h>

namespace sdchain {

Json::Value doLedgerCurrent (RPC::Context& context)
{
    Json::Value jvResult;
    jvResult[jss::ledger_current_index] =
        context.ledgerMaster.getCurrentLedgerIndex ();
    return jvResult;
}

} // sdchain
