//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/ledger/LedgerMaster.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/json/json_value.h>
#include <sdchain/rpc/Context.h>
#include <sdchain/rpc/impl/Handler.h>

namespace sdchain {

Json::Value doLedgerCleaner (RPC::Context& context)
{
    context.app.getLedgerMaster().doLedgerCleaner (context.params);
    return RPC::makeObjectValue ("Cleaner configured");
}


} // sdchain
