//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/app/misc/Transaction.h>
#include <sdchain/core/DatabaseCon.h>
#include <sdchain/core/SociDB.h>
#include <sdchain/net/RPCErr.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/protocol/ErrorCodes.h>
#include <sdchain/resource/Fees.h>
#include <sdchain/rpc/Context.h>
#include <sdchain/rpc/Role.h>
#include <boost/format.hpp>

namespace sdchain {

// {
//   start: <index>
// }
Json::Value doTxHistory (RPC::Context& context)
{
    context.loadType = Resource::feeMediumBurdenRPC;

    if (!context.params.isMember (jss::start))
        return rpcError (rpcINVALID_PARAMS);

    unsigned int startIndex = context.params[jss::start].asUInt ();

    if ((startIndex > 10000) &&  (! isUnlimited (context.role)))
        return rpcError (rpcNO_PERMISSION);

    Json::Value obj;
    Json::Value txs;

    obj[jss::index] = startIndex;

    std::string sql =
        boost::str (boost::format (
            "SELECT LedgerSeq, Status, RawTxn "
            "FROM Transactions ORDER BY LedgerSeq desc LIMIT %u,20;")
                    % startIndex);

    {
        auto db = context.app.getTxnDB ().checkoutDb ();

        boost::optional<std::uint64_t> ledgerSeq;
        boost::optional<std::string> status;
        soci::blob sociRawTxnBlob (*db);
        soci::indicator rti;
        Blob rawTxn;

        soci::statement st = (db->prepare << sql,
                              soci::into (ledgerSeq),
                              soci::into (status),
                              soci::into (sociRawTxnBlob, rti));

        st.execute ();
        while (st.fetch ())
        {
            if (soci::i_ok == rti)
                convert(sociRawTxnBlob, rawTxn);
            else
                rawTxn.clear ();

            if (auto trans = Transaction::transactionFromSQL (
                    ledgerSeq, status, rawTxn, context.app))
                txs.append (trans->getJson (0));
        }
    }

    obj[jss::txs] = txs;

    return obj;
}

} // sdchain
