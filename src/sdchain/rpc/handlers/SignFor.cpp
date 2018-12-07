//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2015 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/ledger/LedgerMaster.h>
#include <sdchain/protocol/ErrorCodes.h>
#include <sdchain/protocol/Feature.h>
#include <sdchain/resource/Fees.h>
#include <sdchain/rpc/Context.h>
#include <sdchain/rpc/impl/TransactionSign.h>

namespace sdchain {

// {
//   tx_json: <object>,
//   account: <signing account>
//   secret: <secret of signing account>
// }
Json::Value doSignFor (RPC::Context& context)
{
    // Bail if multisign is not enabled.
    if (! context.app.getLedgerMaster().getValidatedRules().
        enabled (featureMultiSign))
    {
        RPC::inject_error (rpcNOT_ENABLED, context.params);
        return context.params;
    }
    context.loadType = Resource::feeHighBurdenRPC;
    auto const failHard = context.params[jss::fail_hard].asBool();
    auto const failType = NetworkOPs::doFailHard (failHard);

    return RPC::transactionSignFor (
        context.params,
        failType,
        context.role,
        context.ledgerMaster.getValidatedLedgerAge(),
        context.app);
}

} // sdchain
