//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_LEDGER_INBOUNDLEDGERS_H_INCLUDED
#define SDCHAIN_APP_LEDGER_INBOUNDLEDGERS_H_INCLUDED

#include <sdchain/app/ledger/InboundLedger.h>
#include <sdchain/protocol/SDChainLedgerHash.h>
#include <sdchain/core/Stoppable.h>
#include <memory>

namespace sdchain {

/** Manages the lifetime of inbound ledgers.

    @see InboundLedger
*/
class InboundLedgers
{
public:
    using clock_type = beast::abstract_clock <std::chrono::steady_clock>;

    virtual ~InboundLedgers() = 0;

    // VFALCO TODO Should this be called findOrAdd ?
    //
    virtual
    std::shared_ptr<Ledger const>
    acquire (uint256 const& hash,
        std::uint32_t seq, InboundLedger::fcReason) = 0;

    virtual std::shared_ptr<InboundLedger> find (LedgerHash const& hash) = 0;

    // VFALCO TODO Remove the dependency on the Peer object.
    //
    virtual bool gotLedgerData (LedgerHash const& ledgerHash,
        std::shared_ptr<Peer>,
        std::shared_ptr <protocol::TMLedgerData>) = 0;

    virtual void doLedgerData (LedgerHash hash) = 0;

    virtual void gotStaleData (
        std::shared_ptr <protocol::TMLedgerData> packet) = 0;

    virtual int getFetchCount (int& timeoutCount) = 0;

    virtual void logFailure (uint256 const& h, std::uint32_t seq) = 0;

    virtual bool isFailure (uint256 const& h) = 0;

    virtual void clearFailures() = 0;

    virtual Json::Value getInfo() = 0;

    /** Returns the rate of historical ledger fetches per minute. */
    virtual std::size_t fetchRate() = 0;

    /** Called when a complete ledger is obtained. */
    virtual void onLedgerFetched (InboundLedger::fcReason why) = 0;

    virtual void gotFetchPack () = 0;
    virtual void sweep () = 0;

    virtual void onStop() = 0;
};

std::unique_ptr<InboundLedgers>
make_InboundLedgers (Application& app,
    InboundLedgers::clock_type& clock, Stoppable& parent,
    beast::insight::Collector::ptr const& collector);


} // sdchain

#endif
