//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_LEDGER_TRANSACTIONACQUIRE_H_INCLUDED
#define SDCHAIN_APP_LEDGER_TRANSACTIONACQUIRE_H_INCLUDED

#include <sdchain/app/main/Application.h>
#include <sdchain/overlay/PeerSet.h>
#include <sdchain/shamap/SHAMap.h>

namespace sdchain {

// VFALCO TODO rename to PeerTxRequest
// A transaction set we are trying to acquire
class TransactionAcquire
    : public PeerSet
    , public std::enable_shared_from_this <TransactionAcquire>
    , public CountedObject <TransactionAcquire>
{
public:
    static char const* getCountedObjectName () { return "TransactionAcquire"; }

    using pointer = std::shared_ptr<TransactionAcquire>;

public:
    TransactionAcquire (Application& app, uint256 const& hash, clock_type& clock);
    ~TransactionAcquire ();

    std::shared_ptr<SHAMap> const& getMap ()
    {
        return mMap;
    }

    SHAMapAddNode takeNodes (const std::list<SHAMapNodeID>& IDs,
                             const std::list< Blob >& data, std::shared_ptr<Peer> const&);

    void init (int startPeers);

    void stillNeed ();

private:

    std::shared_ptr<SHAMap> mMap;
    bool                    mHaveRoot;
    beast::Journal          j_;

    void execute () override;

    void onTimer (bool progress, ScopedLockType& peerSetLock) override;


    void newPeer (std::shared_ptr<Peer> const& peer) override
    {
        trigger (peer);
    }

    void done ();

    // Tries to add the specified number of peers
    void addPeers (int num);

    void trigger (std::shared_ptr<Peer> const&);
    std::weak_ptr<PeerSet> pmDowncast () override;
};

} // sdchain

#endif
