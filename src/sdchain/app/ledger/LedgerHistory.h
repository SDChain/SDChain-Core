//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_LEDGER_LEDGERHISTORY_H_INCLUDED
#define SDCHAIN_APP_LEDGER_LEDGERHISTORY_H_INCLUDED

#include <sdchain/app/ledger/Ledger.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/protocol/SDChainLedgerHash.h>
#include <sdchain/beast/insight/Collector.h>
#include <sdchain/beast/insight/Event.h>

namespace sdchain {

// VFALCO TODO Rename to OldLedgers ?

/** Retains historical ledgers. */
class LedgerHistory
{
public:
    LedgerHistory (beast::insight::Collector::ptr const& collector,
        Application& app);

    /** Track a ledger
        @return `true` if the ledger was already tracked
    */
    bool insert (std::shared_ptr<Ledger const> ledger,
        bool validated);

    /** Get the ledgers_by_hash cache hit rate
        @return the hit rate
    */
    float getCacheHitRate ()
    {
        return m_ledgers_by_hash.getHitRate ();
    }

    /** Get a ledger given its squence number */
    std::shared_ptr<Ledger const>
    getLedgerBySeq (LedgerIndex ledgerIndex);

    /** Retrieve a ledger given its hash */
    std::shared_ptr<Ledger const>
    getLedgerByHash (LedgerHash const& ledgerHash);

    /** Get a ledger's hash given its sequence number
        @param ledgerIndex The sequence number of the desired ledger
        @return The hash of the specified ledger
    */
    LedgerHash getLedgerHash (LedgerIndex ledgerIndex);

    /** Set the history cache's paramters
        @param size The target size of the cache
        @param age The target age of the cache, in seconds
    */
    void tune (int size, int age);

    /** Remove stale cache entries
    */
    void sweep ()
    {
        m_ledgers_by_hash.sweep ();
        m_consensus_validated.sweep ();
    }

    /** Report that we have locally built a particular ledger */
    void builtLedger (
        std::shared_ptr<Ledger const> const&,
        Json::Value);

    /** Report that we have validated a particular ledger */
    void validatedLedger (
        std::shared_ptr<Ledger const> const&);

    /** Repair a hash to index mapping
        @param ledgerIndex The index whose mapping is to be repaired
        @param ledgerHash The hash it is to be mapped to
        @return `true` if the mapping was repaired
    */
    bool fixIndex(LedgerIndex ledgerIndex, LedgerHash const& ledgerHash);

    void clearLedgerCachePrior (LedgerIndex seq);

private:

    /** Log details in the case where we build one ledger but
        validate a different one.
        @param built The hash of the ledger we built
        @param valid The hash of the ledger we deemed fully valid
        @param consensus The status of the consensus round
    */
    void handleMismatch (LedgerHash const& built, LedgerHash const& valid,
        Json::Value const& consensus);

    Application& app_;
    beast::insight::Collector::ptr collector_;
    beast::insight::Counter mismatch_counter_;

    using LedgersByHash = TaggedCache <LedgerHash, Ledger const>;

    LedgersByHash m_ledgers_by_hash;

    // Maps ledger indexes to the corresponding hashes
    // For debug and logging purposes
    struct cv_entry
    {
        boost::optional<LedgerHash> built;
        boost::optional<LedgerHash> validated;
        boost::optional<Json::Value> consensus;
    };
    using ConsensusValidated = TaggedCache <LedgerIndex, cv_entry>;
    ConsensusValidated m_consensus_validated;


    // Maps ledger indexes to the corresponding hash.
    std::map <LedgerIndex, LedgerHash> mLedgersByIndex; // validated ledgers

    beast::Journal j_;
};

} // sdchain

#endif
