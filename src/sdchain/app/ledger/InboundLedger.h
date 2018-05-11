//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_LEDGER_INBOUNDLEDGER_H_INCLUDED
#define SDCHAIN_APP_LEDGER_INBOUNDLEDGER_H_INCLUDED

#include <sdchain/app/main/Application.h>
#include <sdchain/app/ledger/Ledger.h>
#include <sdchain/overlay/PeerSet.h>
#include <sdchain/basics/CountedObject.h>
#include <mutex>
#include <set>
#include <utility>

namespace sdchain {

// A ledger we are trying to acquire
class InboundLedger
    : public PeerSet
    , public std::enable_shared_from_this <InboundLedger>
    , public CountedObject <InboundLedger>
{
public:
    static char const* getCountedObjectName () { return "InboundLedger"; }

    using PeerDataPairType = std::pair<
        std::weak_ptr<Peer>,
        std::shared_ptr<protocol::TMLedgerData>>;

    // These are the reasons we might acquire a ledger
    enum fcReason
    {
        fcHISTORY,      // Acquiring past ledger
        fcGENERIC,      // Generic other reasons
        fcVALIDATION,   // Validations suggest this ledger is important
        fcCURRENT,      // This might be the current ledger
        fcCONSENSUS,    // We believe the consensus round requires this ledger
    };

public:
    InboundLedger(Application& app,
        uint256 const& hash, std::uint32_t seq, fcReason reason, clock_type&);

    ~InboundLedger ();

    // Called when the PeerSet timer expires
    void execute () override;

    // Called when another attempt is made to fetch this same ledger
    void update (std::uint32_t seq);

    std::shared_ptr<Ledger const>
    getLedger() const
    {
        return mLedger;
    }
    std::uint32_t getSeq () const
    {
        return mSeq;
    }

    bool checkLocal ();
    void init (ScopedLockType& collectionLock);

    bool gotData (std::weak_ptr<Peer>, std::shared_ptr<protocol::TMLedgerData>);

    using neededHash_t =
        std::pair <protocol::TMGetObjectByHash::ObjectType, uint256>;

    /** Return a Json::objectValue. */
    Json::Value getJson (int);

    void runData ();

private:
    enum class TriggerReason
    {
        added,
        reply,
        timeout
    };

    void filterNodes (
        std::vector<std::pair<SHAMapNodeID, uint256>>& nodes,
        TriggerReason reason);

    void trigger (std::shared_ptr<Peer> const&, TriggerReason);

    std::vector<neededHash_t> getNeededHashes ();

    void addPeers ();
    bool tryLocal ();

    void done ();

    void onTimer (bool progress, ScopedLockType& peerSetLock) override;

    void newPeer (std::shared_ptr<Peer> const& peer) override
    {
        // For historical nodes, do not trigger too soon
        // since a fetch pack is probably coming
        if (mReason != fcHISTORY)
            trigger (peer, TriggerReason::added);
    }

    std::weak_ptr <PeerSet> pmDowncast () override;

    int processData (std::shared_ptr<Peer> peer, protocol::TMLedgerData& data);

    bool takeHeader (std::string const& data);
    bool takeTxNode (const std::vector<SHAMapNodeID>& IDs,
                     const std::vector<Blob>& data,
                     SHAMapAddNode&);
    bool takeTxRootNode (Slice const& data, SHAMapAddNode&);

    // VFALCO TODO Rename to receiveAccountStateNode
    //             Don't use acronyms, but if we are going to use them at least
    //             capitalize them correctly.
    //
    bool takeAsNode (const std::vector<SHAMapNodeID>& IDs,
                     const std::vector<Blob>& data,
                     SHAMapAddNode&);
    bool takeAsRootNode (Slice const& data, SHAMapAddNode&);

    std::vector<uint256>
    neededTxHashes (
        int max, SHAMapSyncFilter* filter) const;

    std::vector<uint256>
    neededStateHashes (
        int max, SHAMapSyncFilter* filter) const;

    LedgerInfo
    deserializeHeader (
        Slice data,
        bool hasPrefix);

private:
    std::shared_ptr<Ledger> mLedger;
    bool               mHaveHeader;
    bool               mHaveState;
    bool               mHaveTransactions;
    bool               mSignaled;
    bool               mByHash;
    std::uint32_t      mSeq;
    fcReason           mReason;

    std::set <uint256> mRecentNodes;

    SHAMapAddNode      mStats;

    // Data we have received from peers
    std::mutex mReceivedDataLock;
    std::vector <PeerDataPairType> mReceivedData;
    bool mReceiveDispatched;
};

} // sdchain

#endif
