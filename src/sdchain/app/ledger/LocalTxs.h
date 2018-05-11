//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_LEDGER_LOCALTXS_H_INCLUDED
#define SDCHAIN_APP_LEDGER_LOCALTXS_H_INCLUDED

#include <sdchain/app/misc/CanonicalTXSet.h>
#include <sdchain/ledger/ReadView.h>
#include <memory>

namespace sdchain {

// Track transactions issued by local clients
// Ensure we always apply them to our open ledger
// Hold them until we see them in a fully-validated ledger

class LocalTxs
{
public:
    virtual ~LocalTxs () = default;

    // Add a new local transaction
    virtual void push_back (LedgerIndex index, std::shared_ptr<STTx const> const& txn) = 0;

    // Return the set of local transactions to a new open ledger
    virtual CanonicalTXSet getTxSet () = 0;

    // Remove obsolete transactions based on a new fully-valid ledger
    virtual void sweep (ReadView const& view) = 0;

    virtual std::size_t size () = 0;
};

std::unique_ptr<LocalTxs>
make_LocalTxs ();

} // sdchain

#endif
