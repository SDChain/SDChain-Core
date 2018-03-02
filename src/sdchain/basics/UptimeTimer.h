//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_BASICS_UPTIMETIMER_H_INCLUDED
#define SDCHAIN_BASICS_UPTIMETIMER_H_INCLUDED

#include <ctime>

namespace sdchain {

/** Tracks program uptime.

    The timer can be switched to a manual system of updating, to reduce
    system calls. (?)
*/
// VFALCO TODO determine if the non-manual timing is actually needed
class UptimeTimer
{
private:
    UptimeTimer ();
    ~UptimeTimer ();

public:
    int getElapsedSeconds () const;

    void beginManualUpdates ();
    void endManualUpdates ();

    void incrementElapsedTime ();

    static UptimeTimer& getInstance ();

private:
    // VFALCO DEPRECATED, Use a memory barrier instead of forcing a cache line
    int volatile m_elapsedTime;

    time_t m_startTime;

    bool m_isUpdatingManually;
};

} // sdchain

#endif
