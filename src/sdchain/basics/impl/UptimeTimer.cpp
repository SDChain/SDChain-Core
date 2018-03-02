//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/basics/UptimeTimer.h>

#include <atomic>

namespace sdchain {

UptimeTimer::UptimeTimer ()
    : m_elapsedTime (0)
    , m_startTime (::time (0))
    , m_isUpdatingManually (false)
{
}

UptimeTimer::~UptimeTimer ()
{
}

int UptimeTimer::getElapsedSeconds () const
{
    int result;

    if (m_isUpdatingManually)
    {
        std::atomic_thread_fence (std::memory_order_seq_cst);
        result = m_elapsedTime;
    }
    else
    {
        // VFALCO TODO use time_t instead of int return
        result = static_cast <int> (::time (0) - m_startTime);
    }

    return result;
}

void UptimeTimer::beginManualUpdates ()
{
    //assert (!m_isUpdatingManually);

    m_isUpdatingManually = true;
}

void UptimeTimer::endManualUpdates ()
{
    //assert (m_isUpdatingManually);

    m_isUpdatingManually = false;
}

void UptimeTimer::incrementElapsedTime ()
{
    //assert (m_isUpdatingManually);
    ++m_elapsedTime;
}

UptimeTimer& UptimeTimer::getInstance ()
{
    static UptimeTimer instance;

    return instance;
}

} // sdchain
