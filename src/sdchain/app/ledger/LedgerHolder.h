//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_LEDGER_LEDGERHOLDER_H_INCLUDED
#define SDCHAIN_APP_LEDGER_LEDGERHOLDER_H_INCLUDED

#include <sdchain/basics/contract.h>
#include <mutex>

namespace sdchain {

// Can std::atomic<std::shared_ptr>> make this lock free?

// VFALCO NOTE This class can be replaced with atomic<shared_ptr<...>>

/** Hold a ledger in a thread-safe way.

    VFALCO TODO The constructor should require a valid ledger, this
                way the object always holds a value. We can use the
                genesis ledger in all cases.
*/
class LedgerHolder
{
public:
    // Update the held ledger
    void set (std::shared_ptr<Ledger const> ledger)
    {
        if(! ledger)
            LogicError("LedgerHolder::set with nullptr");
        if(! ledger->isImmutable())
            LogicError("LedgerHolder::set with mutable Ledger");
        std::lock_guard <std::mutex> sl (m_lock);
        m_heldLedger = std::move(ledger);
    }

    // Return the (immutable) held ledger
    std::shared_ptr<Ledger const> get ()
    {
        std::lock_guard <std::mutex> sl (m_lock);
        return m_heldLedger;
    }

    bool empty ()
    {
        std::lock_guard <std::mutex> sl (m_lock);
        return m_heldLedger == nullptr;
    }

private:
    std::mutex m_lock;
    std::shared_ptr<Ledger const> m_heldLedger;
};

} // sdchain

#endif
