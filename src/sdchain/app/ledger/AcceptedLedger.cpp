//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/ledger/AcceptedLedger.h>
#include <sdchain/basics/Log.h>
#include <sdchain/basics/chrono.h>

namespace sdchain {

AcceptedLedger::AcceptedLedger (
    std::shared_ptr<ReadView const> const& ledger,
    AccountIDCache const& accountCache, Logs& logs)
    : mLedger (ledger)
{
    for (auto const& item : ledger->txs)
    {
        insert (std::make_shared<AcceptedLedgerTx>(
            ledger, item.first, item.second, accountCache, logs));
    }
}

void AcceptedLedger::insert (AcceptedLedgerTx::ref at)
{
    assert (mMap.find (at->getIndex ()) == mMap.end ());
    mMap.insert (std::make_pair (at->getIndex (), at));
}

AcceptedLedgerTx::pointer AcceptedLedger::getTxn (int i) const
{
    map_t::const_iterator it = mMap.find (i);

    if (it == mMap.end ())
        return AcceptedLedgerTx::pointer ();

    return it->second;
}

} // sdchain
