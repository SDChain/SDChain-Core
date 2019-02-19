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
#include <sdchain/protocol/ErrorCodes.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/rpc/impl/RPCHelpers.h>
#include <sdchain/rpc/Context.h>

namespace sdchain {

// {
//   'ident' : <indent>,
// }
Json::Value doOwnerInfo (RPC::Context& context)
{
    if (!context.params.isMember (jss::account) &&
        !context.params.isMember (jss::ident))
    {
        return RPC::missing_field_error (jss::account);
    }

    std::string strIdent = context.params.isMember (jss::account)
            ? context.params[jss::account].asString ()
            : context.params[jss::ident].asString ();
    Json::Value ret;

    // Get info on account.

    auto const& closedLedger = context.ledgerMaster.getClosedLedger ();
    AccountID accountID;
    auto jAccepted = RPC::accountFromString (accountID, strIdent);

    ret[jss::accepted] = ! jAccepted ?
            context.netOps.getOwnerInfo (closedLedger, accountID) : jAccepted;

    auto const& currentLedger = context.ledgerMaster.getCurrentLedger ();
    auto jCurrent = RPC::accountFromString (accountID, strIdent);

    ret[jss::current] = ! jCurrent ?
            context.netOps.getOwnerInfo (currentLedger, accountID) : jCurrent;
    return ret;
}

} // sdchain
