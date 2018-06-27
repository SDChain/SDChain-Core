//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/app/ledger/AcceptedLedgerTx.h>
#include <sdchain/basics/Log.h>
#include <sdchain/basics/StringUtilities.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/protocol/types.h>

namespace sdchain {

AcceptedLedgerTx::AcceptedLedgerTx (
    std::shared_ptr<ReadView const> const& ledger,
    std::shared_ptr<STTx const> const& txn,
    std::shared_ptr<STObject const> const& met,
    AccountIDCache const& accountCache,
    Logs& logs)
    : mLedger (ledger)
    , mTxn (txn)
    , mMeta (std::make_shared<TxMeta> (
        txn->getTransactionID(), ledger->seq(), *met, logs.journal ("View")))
    , mAffected (mMeta->getAffectedAccounts ())
    , accountCache_ (accountCache)
    , logs_ (logs)
{
    assert (! ledger->open());

    mResult = mMeta->getResultTER ();

    Serializer s;
    met->add(s);
    mRawMeta = std::move (s.modData());

    buildJson ();
}

AcceptedLedgerTx::AcceptedLedgerTx (
    std::shared_ptr<ReadView const> const& ledger,
    std::shared_ptr<STTx const> const& txn,
    TER result,
    AccountIDCache const& accountCache,
    Logs& logs)
    : mLedger (ledger)
    , mTxn (txn)
    , mResult (result)
    , mAffected (txn->getMentionedAccounts ())
    , accountCache_ (accountCache)
    , logs_ (logs)
{
    assert (ledger->open());
    buildJson ();
}

std::string AcceptedLedgerTx::getEscMeta () const
{
    assert (!mRawMeta.empty ());
    return sqlEscape (mRawMeta);
}

void AcceptedLedgerTx::buildJson ()
{
    mJson = Json::objectValue;
    mJson[jss::transaction] = mTxn->getJson (0);

    if (mMeta)
    {
        mJson[jss::meta] = mMeta->getJson (0);
        mJson[jss::raw_meta] = strHex (mRawMeta);
    }

    mJson[jss::result] = transHuman (mResult);

    if (! mAffected.empty ())
    {
        Json::Value& affected = (mJson[jss::affected] = Json::arrayValue);
        for (auto const& account: mAffected)
            affected.append (accountCache_.toBase58(account));
    }

    if (mTxn->getTxnType () == ttOFFER_CREATE)
    {
        auto const& account = mTxn->getAccountID(sfAccount);
        auto const amount = mTxn->getFieldAmount (sfTakerGets);

        // If the offer create is not self funded then add the owner balance
        if (account != amount.issue ().account)
        {
            auto const ownerFunds = accountFunds(*mLedger,
                account, amount, fhIGNORE_FREEZE, logs_.journal ("View"));
            mJson[jss::transaction][jss::owner_funds] = ownerFunds.getText ();
        }
    }
}

} // sdchain
