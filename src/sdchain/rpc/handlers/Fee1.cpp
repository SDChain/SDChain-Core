//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2015 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/ledger/OpenLedger.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/app/misc/TxQ.h>
#include <sdchain/rpc/Context.h>
#include <sdchain/protocol/ErrorCodes.h>
#include <sdchain/protocol/Feature.h>

namespace sdchain
{
    Json::Value doFee(RPC::Context& context)
    {
        // Bail if fee escalation is not enabled.
        auto const view = context.app.openLedger().current();
        if (!view || !view->rules().enabled(featureFeeEscalation))
        {
            RPC::inject_error(rpcNOT_ENABLED, context.params);
            return context.params;
        }

        auto result = context.app.getTxQ().doRPC(context.app);
        if (result.type() == Json::objectValue)
            return result;
        assert(false);
        RPC::inject_error(rpcINTERNAL, context.params);
        return context.params;
    }
} // sdchain
