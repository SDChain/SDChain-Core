//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2015 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_LEDGER_INBOUNDTRANSACTIONS_H_INCLUDED
#define SDCHAIN_APP_LEDGER_INBOUNDTRANSACTIONS_H_INCLUDED

#include <sdchain/overlay/Peer.h>
#include <sdchain/shamap/SHAMap.h>
#include <sdchain/beast/clock/abstract_clock.h>
#include <sdchain/core/Stoppable.h>
#include <memory>

namespace sdchain {

class Application;

/** Manages the acquisition and lifetime of transaction sets.
*/

class InboundTransactions
{
public:
    using clock_type = beast::abstract_clock <std::chrono::steady_clock>;

    InboundTransactions() = default;
    InboundTransactions(InboundTransactions const&) = delete;
    InboundTransactions& operator=(InboundTransactions const&) = delete;

    virtual ~InboundTransactions() = 0;

    /** Retrieves a transaction set by hash
    */
    virtual std::shared_ptr <SHAMap> getSet (
        uint256 const& setHash,
        bool acquire) = 0;

    /** Gives data to an inbound transaction set
    */
    virtual void gotData (uint256 const& setHash,
        std::shared_ptr <Peer>,
        std::shared_ptr <protocol::TMLedgerData>) = 0;

    /** Gives set to the container
    */
    virtual void giveSet (uint256 const& setHash,
        std::shared_ptr <SHAMap> const& set,
        bool acquired) = 0;

    /** Informs the container if a new consensus round
    */
    virtual void newRound (std::uint32_t seq) = 0;

    virtual Json::Value getInfo() = 0;

    virtual void onStop() = 0;
};

std::unique_ptr <InboundTransactions>
make_InboundTransactions (
    Application& app,
    InboundTransactions::clock_type& clock,
    Stoppable& parent,
    beast::insight::Collector::ptr const& collector,
    std::function
        <void (std::shared_ptr <SHAMap> const&,
            bool)> gotSet);


} // sdchain

#endif
