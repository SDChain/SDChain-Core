//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_PEERS_PEERSET_H_INCLUDED
#define SDCHAIN_APP_PEERS_PEERSET_H_INCLUDED

#include <sdchain/app/main/Application.h>
#include <sdchain/beast/clock/abstract_clock.h>
#include <sdchain/beast/utility/Journal.h>
#include <sdchain/overlay/Peer.h>
#include <boost/asio/basic_waitable_timer.hpp>
#include <mutex>
#include <set>

namespace sdchain {

/** Supports data retrieval by managing a set of peers.

    When desired data (such as a ledger or a transaction set)
    is missing locally it can be obtained by querying connected
    peers. This class manages common aspects of the retrieval.
    Callers maintain the set by adding and removing peers depending
    on whether the peers have useful information.

    This class is an "active" object. It maintains its own timer
    and dispatches work to a job queue. Implementations derive
    from this class and override the abstract hook functions in
    the base.

    The data is represented by its hash.
*/
class PeerSet
{
public:
    using clock_type = beast::abstract_clock <std::chrono::steady_clock>;

    /** Returns the hash of the data we want. */
    uint256 const& getHash () const
    {
        return mHash;
    }

    /** Returns true if we got all the data. */
    bool isComplete () const
    {
        return mComplete;
    }

    /** Returns false if we failed to get the data. */
    bool isFailed () const
    {
        return mFailed;
    }

    /** Returns the number of times we timed out. */
    int getTimeouts () const
    {
        return mTimeouts;
    }

    bool isActive ();

    /** Called to indicate that forward progress has been made. */
    void progress ()
    {
        mProgress = true;
    }

    void touch ()
    {
        mLastAction = m_clock.now();
    }

    clock_type::time_point getLastAction () const
    {
        return mLastAction;
    }

    /** Insert a peer to the managed set.
        This will call the derived class hook function.
        @return `true` If the peer was added
    */
    bool insert (std::shared_ptr<Peer> const&);

    virtual bool isDone () const
    {
        return mComplete || mFailed;
    }

    Application&
    app()
    {
        return app_;
    }

protected:
    using ScopedLockType = std::unique_lock <std::recursive_mutex>;

    PeerSet (Application& app, uint256 const& hash, std::chrono::milliseconds interval,
        clock_type& clock, beast::Journal journal);

    virtual ~PeerSet() = 0;

    virtual void newPeer (std::shared_ptr<Peer> const&) = 0;

    virtual void onTimer (bool progress, ScopedLockType&) = 0;

    virtual void execute () = 0;

    virtual std::weak_ptr<PeerSet> pmDowncast () = 0;

    bool isProgress ()
    {
        return mProgress;
    }

    void setComplete ()
    {
        mComplete = true;
    }
    void setFailed ()
    {
        mFailed = true;
    }

    void invokeOnTimer ();

    void sendRequest (const protocol::TMGetLedger& message);

    void sendRequest (const protocol::TMGetLedger& message, std::shared_ptr<Peer> const& peer);

    void setTimer ();

    std::size_t getPeerCount () const;

protected:
    Application& app_;
    beast::Journal m_journal;
    clock_type& m_clock;

    std::recursive_mutex mLock;

    uint256 mHash;
    std::chrono::milliseconds mTimerInterval;
    int mTimeouts;
    bool mComplete;
    bool mFailed;
    clock_type::time_point mLastAction;
    bool mProgress;

    // VFALCO TODO move the responsibility for the timer to a higher level
    boost::asio::basic_waitable_timer<std::chrono::steady_clock> mTimer;

    // The identifiers of the peers we are tracking.
    std::set <Peer::id_t> mPeers;
};

} // sdchain

#endif
