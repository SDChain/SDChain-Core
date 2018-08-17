//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/misc/Transaction.h>
#include <sdchain/app/tx/apply.h>
#include <sdchain/basics/Log.h>
#include <sdchain/core/DatabaseCon.h>
#include <sdchain/app/ledger/LedgerMaster.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/app/misc/HashRouter.h>
#include <sdchain/protocol/Feature.h>
#include <sdchain/protocol/JsonFields.h>
#include <boost/optional.hpp>

namespace sdchain {

Transaction::Transaction (std::shared_ptr<STTx const> const& stx,
    std::string& reason, Application& app)
    noexcept
    : mTransaction (stx)
    , mApp (app)
    , j_ (app.journal ("Ledger"))
{
    try
    {
        mTransactionID  = mTransaction->getTransactionID ();
    }
    catch (std::exception& e)
    {
        reason = e.what();
        return;
    }

    mStatus = NEW;
}

//
// Misc.
//

void Transaction::setStatus (TransStatus ts, std::uint32_t lseq)
{
    mStatus     = ts;
    mInLedger   = lseq;
}

TransStatus Transaction::sqlTransactionStatus(
    boost::optional<std::string> const& status)
{
    char const c = (status) ? (*status)[0] : TXN_SQL_UNKNOWN;

    switch (c)
    {
    case TXN_SQL_NEW:       return NEW;
    case TXN_SQL_CONFLICT:  return CONFLICTED;
    case TXN_SQL_HELD:      return HELD;
    case TXN_SQL_VALIDATED: return COMMITTED;
    case TXN_SQL_INCLUDED:  return INCLUDED;
    }

    assert (c == TXN_SQL_UNKNOWN);
    return INVALID;
}

Transaction::pointer Transaction::transactionFromSQL (
    boost::optional<std::uint64_t> const& ledgerSeq,
    boost::optional<std::string> const& status,
    Blob const& rawTxn,
    Application& app)
{
    std::uint32_t const inLedger =
        rangeCheckedCast<std::uint32_t>(ledgerSeq.value_or (0));

    SerialIter it (makeSlice(rawTxn));
    auto txn = std::make_shared<STTx const> (it);
    std::string reason;
    auto tr = std::make_shared<Transaction> (
        txn, reason, app);

    tr->setStatus (sqlTransactionStatus (status));
    tr->setLedger (inLedger);
    return tr;
}

Transaction::pointer Transaction::transactionFromSQLValidated(
    boost::optional<std::uint64_t> const& ledgerSeq,
    boost::optional<std::string> const& status,
    Blob const& rawTxn,
    Application& app)
{
    auto ret = transactionFromSQL(ledgerSeq, status, rawTxn, app);

    if (checkValidity(app.getHashRouter(),
            *ret->getSTransaction(), app.
                getLedgerMaster().getValidatedRules(),
                    app.config()).first !=
                        Validity::Valid)
        return {};

    return ret;
}

Transaction::pointer Transaction::load(uint256 const& id, Application& app)
{
    std::string sql = "SELECT LedgerSeq,Status,RawTxn "
            "FROM Transactions WHERE TransID='";
    sql.append (to_string (id));
    sql.append ("';");

    boost::optional<std::uint64_t> ledgerSeq;
    boost::optional<std::string> status;
    Blob rawTxn;
    {
        auto db = app.getTxnDB ().checkoutDb ();
        soci::blob sociRawTxnBlob (*db);
        soci::indicator rti;

        *db << sql, soci::into (ledgerSeq), soci::into (status),
                soci::into (sociRawTxnBlob, rti);
        if (!db->got_data () || rti != soci::i_ok)
            return {};

        convert(sociRawTxnBlob, rawTxn);
    }

    return Transaction::transactionFromSQLValidated (
        ledgerSeq, status, rawTxn, app);
}

// options 1 to include the date of the transaction
Json::Value Transaction::getJson (int options, bool binary) const
{
    Json::Value ret (mTransaction->getJson (0, binary));

    if (mInLedger)
    {
        ret[jss::inLedger] = mInLedger;        // Deprecated.
        ret[jss::ledger_index] = mInLedger;

        if (options == 1)
        {
            auto ct = mApp.getLedgerMaster().
                getCloseTimeBySeq (mInLedger);
            if (ct)
                ret[jss::date] = ct->time_since_epoch().count();
        }
    }

    return ret;
}

} // sdchain
