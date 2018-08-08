//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_MISC_SHAMAPSTORE_H_INCLUDED
#define SDCHAIN_APP_MISC_SHAMAPSTORE_H_INCLUDED

#include <sdchain/app/ledger/Ledger.h>
#include <sdchain/nodestore/Manager.h>
#include <sdchain/protocol/ErrorCodes.h>
#include <sdchain/core/Stoppable.h>

namespace sdchain {

class TransactionMaster;

/**
 * class to create database, launch online delete thread, and
 * related sqlite databse
 */
class SHAMapStore
    : public Stoppable
{
public:
    struct Setup
    {
        bool standalone = false;
        std::uint32_t deleteInterval = 0;
        bool advisoryDelete = false;
        std::uint32_t ledgerHistory = 0;
        Section nodeDatabase;
        std::string databasePath;
        std::uint32_t deleteBatch = 100;
        std::uint32_t backOff = 100;
        std::int32_t ageThreshold = 60;
    };

    SHAMapStore (Stoppable& parent) : Stoppable ("SHAMapStore", parent) {}

    /** Called by LedgerMaster every time a ledger validates. */
    virtual void onLedgerClosed(std::shared_ptr<Ledger const> const& ledger) = 0;

    virtual void rendezvous() const = 0;

    virtual std::uint32_t clampFetchDepth (std::uint32_t fetch_depth) const = 0;

    virtual std::unique_ptr <NodeStore::Database> makeDatabase (
            std::string const& name,
            std::int32_t readThreads, Stoppable& parent) = 0;

    /** Highest ledger that may be deleted. */
    virtual LedgerIndex setCanDelete (LedgerIndex canDelete) = 0;

    /** Whether advisory delete is enabled. */
    virtual bool advisoryDelete() const = 0;

    /** Last ledger which was copied during rotation of backends. */
    virtual LedgerIndex getLastRotated() = 0;

    /** Highest ledger that may be deleted. */
    virtual LedgerIndex getCanDelete() = 0;

    /** The number of files that are needed. */
    virtual int fdlimit() const = 0;
};

//------------------------------------------------------------------------------

SHAMapStore::Setup
setup_SHAMapStore(Config const& c);

std::unique_ptr<SHAMapStore>
make_SHAMapStore(
    Application& app,
    SHAMapStore::Setup const& s,
    Stoppable& parent,
    NodeStore::Scheduler& scheduler,
    beast::Journal journal,
    beast::Journal nodeStoreJournal,
    TransactionMaster& transactionMaster,
    BasicConfig const& conf);
}

#endif
