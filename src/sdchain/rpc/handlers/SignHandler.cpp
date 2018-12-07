//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/ledger/LedgerMaster.h>
#include <sdchain/protocol/ErrorCodes.h>
#include <sdchain/resource/Fees.h>
#include <sdchain/rpc/Context.h>
#include <sdchain/rpc/impl/TransactionSign.h>

namespace sdchain {

// {
//   tx_json: <object>,
//   secret: <secret>
// }
Json::Value doSign (RPC::Context& context)
{
    context.loadType = Resource::feeHighBurdenRPC;
    NetworkOPs::FailHard const failType =
        NetworkOPs::doFailHard (
            context.params.isMember (jss::fail_hard)
            && context.params[jss::fail_hard].asBool ());

    return RPC::transactionSign (
        context.params,
        failType,
        context.role,
        context.ledgerMaster.getValidatedLedgerAge(),
        context.app);
}

} // sdchain
