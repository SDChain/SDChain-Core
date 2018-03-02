//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_BASICS_CHRONO_H_INCLUDED
#define SDCHAIN_BASICS_CHRONO_H_INCLUDED

#include <sdchain/beast/clock/abstract_clock.h>
#include <sdchain/beast/clock/basic_seconds_clock.h>
#include <sdchain/beast/clock/manual_clock.h>
#include <chrono>
#include <cstdint>
#include <string>

namespace sdchain {

// A few handy aliases

using days = std::chrono::duration
    <int, std::ratio_multiply<
        std::chrono::hours::period,
            std::ratio<24>>>;

using weeks = std::chrono::duration
    <int, std::ratio_multiply<
        days::period, std::ratio<7>>>;

/** Clock for measuring SDChain Network Time.

    The epoch is January 1, 2000
    epoch_offset = days(10957);  // 2000-01-01
*/
class NetClock
{
public:
    using rep        = std::uint32_t;
    using period     = std::ratio<1>;
    using duration   = std::chrono::duration<rep, period>;
    using time_point = std::chrono::time_point<NetClock>;

    static bool const is_steady = false;
};

std::string to_string(NetClock::time_point tp);
std::string to_string(std::chrono::system_clock::time_point tp);

/** A clock for measuring elapsed time.

    The epoch is unspecified.
*/
using Stopwatch =
    beast::abstract_clock<
        std::chrono::steady_clock>;

/** A manual Stopwatch for unit tests. */
using TestStopwatch =
    beast::manual_clock<
        std::chrono::steady_clock>;

/** Returns an instance of a wall clock. */
inline
Stopwatch&
stopwatch()
{
    return beast::get_abstract_clock<
        std::chrono::steady_clock,
            beast::basic_seconds_clock<
                std::chrono::steady_clock>>();
}

} // sdchain

#endif
