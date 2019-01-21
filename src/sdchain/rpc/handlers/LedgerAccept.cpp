//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/ledger/LedgerMaster.h>
#include <sdchain/app/misc/NetworkOPs.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/core/Config.h>
#include <sdchain/net/RPCErr.h>
#include <sdchain/protocol/ErrorCodes.h>
#include <sdchain/protocol/Indexes.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/rpc/Context.h>
#include <sdchain/basics/make_lock.h>

namespace sdchain {

Json::Value doLedgerAccept (RPC::Context& context)
{
    auto lock = make_lock(context.app.getMasterMutex());
    Json::Value jvResult;

    if (!context.app.config().standalone())
    {
        jvResult[jss::error] = "notStandAlone";
    }
    else
    {
        context.netOps.acceptLedger ();

        jvResult[jss::ledger_current_index] =
            context.ledgerMaster.getCurrentLedgerIndex ();
    }

    return jvResult;
}

} // sdchain
