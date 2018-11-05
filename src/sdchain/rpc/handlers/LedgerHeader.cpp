//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/ledger/LedgerToJson.h>
#include <sdchain/basics/strHex.h>
#include <sdchain/ledger/ReadView.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/rpc/impl/RPCHelpers.h>

namespace sdchain {

// {
//   ledger_hash : <ledger>
//   ledger_index : <ledger_index>
// }
Json::Value doLedgerHeader (RPC::Context& context)
{
    std::shared_ptr<ReadView const> lpLedger;
    auto jvResult = RPC::lookupLedger (lpLedger, context);

    if (!lpLedger)
        return jvResult;

    Serializer  s;
    addRaw (lpLedger->info(), s);
    jvResult[jss::ledger_data] = strHex (s.peekData ());

    // This information isn't verified: they should only use it if they trust
    // us.
    addJson (jvResult, {*lpLedger, 0});

    return jvResult;
}


} // sdchain
