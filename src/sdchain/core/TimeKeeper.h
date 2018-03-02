//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_CORE_TIMEKEEPER_H_INCLUDED
#define SDCHAIN_CORE_TIMEKEEPER_H_INCLUDED

#include <sdchain/beast/clock/abstract_clock.h>
#include <sdchain/beast/utility/Journal.h>
#include <sdchain/basics/chrono.h>
#include <string>
#include <vector>

namespace sdchain {

/** Manages various times used by the server. */
class TimeKeeper
    : public beast::abstract_clock<NetClock>
{
public:
    virtual ~TimeKeeper() = default;

    /** Launch the internal thread.

        The internal thread synchronizes local network time
        using the provided list of SNTP servers.
    */
    virtual
    void
    run (std::vector<std::string> const& servers) = 0;

    /** Returns the estimate of wall time, in network time.

        The network time is wall time adjusted for the SDChain
        epoch, the beginning of January 1st, 2000. Each server
        can compute a different value for network time. Other
        servers value for network time is not directly observable,
        but good guesses can be made by looking at validators'
        positions on close times.

        Servers compute network time by adjusting a local wall
        clock using SNTP and then adjusting for the epoch.
    */
    virtual
    time_point
    now() const = 0;

    /** Returns the close time, in network time.

        Close time is the time the network would agree that
        a ledger closed, if a ledger closed right now.

        The close time represents the notional "center"
        of the network. Each server assumes its clock
        is correct, and tries to pull the close time towards
        its measure of network time.
    */
    virtual
    time_point
    closeTime() const = 0;

    /** Adjust the close time.

        This is called in response to received validations.
    */
    virtual
    void
    adjustCloseTime (std::chrono::duration<std::int32_t> amount) = 0;

    // This may return a negative value
    virtual
    std::chrono::duration<std::int32_t>
    nowOffset() const = 0;

    // This may return a negative value
    virtual
    std::chrono::duration<std::int32_t>
    closeOffset() const = 0;
};

extern
std::unique_ptr<TimeKeeper>
make_TimeKeeper(beast::Journal j);

} // sdchain

#endif
