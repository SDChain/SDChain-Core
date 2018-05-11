//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/ledger/TransactionMaster.h>
#include <sdchain/app/misc/Transaction.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/protocol/STTx.h>
#include <sdchain/basics/Log.h>
#include <sdchain/basics/chrono.h>

namespace sdchain {

TransactionMaster::TransactionMaster (Application& app)
    : mApp (app)
    , mCache ("TransactionCache", 65536, 1800, stopwatch(),
        mApp.journal("TaggedCache"))
{
}

bool TransactionMaster::inLedger (uint256 const& hash, std::uint32_t ledger)
{
    auto txn = mCache.fetch (hash);

    if (!txn)
        return false;

    txn->setStatus (COMMITTED, ledger);
    return true;
}

std::shared_ptr<Transaction>
TransactionMaster::fetch (uint256 const& txnID, bool checkDisk)
{
    auto txn = mCache.fetch (txnID);

    if (!checkDisk || txn)
        return txn;

    txn = Transaction::load (txnID, mApp);

    if (!txn)
        return txn;

    mCache.canonicalize (txnID, txn);

    return txn;
}

std::shared_ptr<STTx const>
TransactionMaster::fetch (std::shared_ptr<SHAMapItem> const& item,
    SHAMapTreeNode::TNType type,
        bool checkDisk, std::uint32_t uCommitLedger)
{
    std::shared_ptr<STTx const>  txn;
    auto iTx = fetch (item->key(), false);

    if (!iTx)
    {

        if (type == SHAMapTreeNode::tnTRANSACTION_NM)
        {
            SerialIter sit (item->slice());
            txn = std::make_shared<STTx const> (std::ref (sit));
        }
        else if (type == SHAMapTreeNode::tnTRANSACTION_MD)
        {
            auto blob = SerialIter{item->data(), item->size()}.getVL();
            txn = std::make_shared<STTx const>(SerialIter{blob.data(), blob.size()});
        }
    }
    else
    {
        if (uCommitLedger)
            iTx->setStatus (COMMITTED, uCommitLedger);

        txn = iTx->getSTransaction ();
    }

    return txn;
}

void
TransactionMaster::canonicalize(std::shared_ptr<Transaction>* pTransaction)
{
    uint256 const tid = (*pTransaction)->getID();
    if (tid != zero)
    {
        auto txn = *pTransaction;
        // VFALCO NOTE canonicalize can change the value of txn!
        mCache.canonicalize(tid, txn);
        *pTransaction = txn;
    }
}

void TransactionMaster::sweep (void)
{
    mCache.sweep ();
}

TaggedCache <uint256, Transaction>& TransactionMaster::getCache()
{
    return mCache;
}

} // sdchain
