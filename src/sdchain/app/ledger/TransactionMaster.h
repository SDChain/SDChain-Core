//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_LEDGER_TRANSACTIONMASTER_H_INCLUDED
#define SDCHAIN_APP_LEDGER_TRANSACTIONMASTER_H_INCLUDED

#include <sdchain/shamap/SHAMapItem.h>
#include <sdchain/shamap/SHAMapTreeNode.h>

namespace sdchain {

class Application;
class Transaction;
class STTx;

// Tracks all transactions in memory

class TransactionMaster
{
public:
    TransactionMaster (Application& app);
    TransactionMaster (TransactionMaster const&) = delete;
    TransactionMaster& operator= (TransactionMaster const&) = delete;

    std::shared_ptr<Transaction>
    fetch (uint256 const& , bool checkDisk);

    std::shared_ptr<STTx const>
    fetch (std::shared_ptr<SHAMapItem> const& item,
        SHAMapTreeNode::TNType type, bool checkDisk,
            std::uint32_t uCommitLedger);

    // return value: true = we had the transaction already
    bool inLedger (uint256 const& hash, std::uint32_t ledger);

    void canonicalize (std::shared_ptr<Transaction>* pTransaction);

    void sweep (void);

    TaggedCache <uint256, Transaction>&
    getCache();

private:
    Application& mApp;
    TaggedCache <uint256, Transaction> mCache;
};

} // sdchain

#endif
