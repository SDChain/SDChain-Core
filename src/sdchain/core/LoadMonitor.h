//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_CORE_LOADMONITOR_H_INCLUDED
#define SDCHAIN_CORE_LOADMONITOR_H_INCLUDED

#include <sdchain/core/LoadEvent.h>
#include <sdchain/beast/utility/Journal.h>
#include <chrono>
#include <mutex>

namespace sdchain {

// Monitors load levels and response times

// VFALCO TODO Rename this. Having both LoadManager and LoadMonitor is confusing.
//
class LoadMonitor
{
public:
    explicit
    LoadMonitor (beast::Journal j);

    void addLoadSample (LoadEvent const& sample);

    void addSamples (int count, std::chrono::milliseconds latency);

    void setTargetLatency (std::chrono::milliseconds avg,
                           std::chrono::milliseconds pk);

    bool isOverTarget (std::chrono::milliseconds avg,
                       std::chrono::milliseconds peak);

    // VFALCO TODO make this return the values in a struct.
    struct Stats
    {
        Stats();

        std::uint64_t             count;
        std::chrono::milliseconds latencyAvg;
        std::chrono::milliseconds latencyPeak;
        bool                      isOverloaded;
    };

    Stats getStats ();

    bool isOver ();

private:
    void update ();

    std::mutex mutex_;

    std::uint64_t             mCounts;
    int                       mLatencyEvents;
    std::chrono::milliseconds mLatencyMSAvg;
    std::chrono::milliseconds mLatencyMSPeak;
    std::chrono::milliseconds mTargetLatencyAvg;
    std::chrono::milliseconds mTargetLatencyPk;
    int                       mLastUpdate;
    beast::Journal j_;
};

} // sdchain

#endif
