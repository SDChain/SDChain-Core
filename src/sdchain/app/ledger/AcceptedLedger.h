//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_LEDGER_ACCEPTEDLEDGER_H_INCLUDED
#define SDCHAIN_APP_LEDGER_ACCEPTEDLEDGER_H_INCLUDED

#include <sdchain/app/ledger/AcceptedLedgerTx.h>
#include <sdchain/protocol/AccountID.h>

namespace sdchain {

/** A ledger that has become irrevocable.

    An accepted ledger is a ledger that has a sufficient number of
    validations to convince the local server that it is irrevocable.

    The existence of an accepted ledger implies all preceding ledgers
    are accepted.
*/
/* VFALCO TODO digest this terminology clarification:
    Closed and accepted refer to ledgers that have not passed the
    validation threshold yet. Once they pass the threshold, they are
    "Validated". Closed just means its close time has passed and no
    new transactions can get in. "Accepted" means we believe it to be
    the result of the a consensus process (though haven't validated
    it yet).
*/
class AcceptedLedger
{
public:
    using pointer        = std::shared_ptr<AcceptedLedger>;
    using ret            = const pointer&;
    using map_t          = std::map<int, AcceptedLedgerTx::pointer>;
    // mapt_t must be an ordered map!
    using value_type     = map_t::value_type;
    using const_iterator = map_t::const_iterator;

public:
    std::shared_ptr<ReadView const> const& getLedger () const
    {
        return mLedger;
    }
    const map_t& getMap () const
    {
        return mMap;
    }

    int getTxnCount () const
    {
        return mMap.size ();
    }

    AcceptedLedgerTx::pointer getTxn (int) const;

    AcceptedLedger (
        std::shared_ptr<ReadView const> const& ledger,
        AccountIDCache const& accountCache, Logs& logs);

private:
    void insert (AcceptedLedgerTx::ref);

    std::shared_ptr<ReadView const> mLedger;
    map_t mMap;
};

} // sdchain

#endif
