//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_CORE_JOBTYPEDATA_H_INCLUDED
#define SDCHAIN_CORE_JOBTYPEDATA_H_INCLUDED

#include <sdchain/basics/Log.h>
#include <sdchain/core/JobTypeInfo.h>
#include <sdchain/beast/insight/Collector.h>

namespace sdchain
{

struct JobTypeData
{
private:
    LoadMonitor m_load;

    /* Support for insight */
    beast::insight::Collector::ptr m_collector;

public:
    /* The job category which we represent */
    JobTypeInfo const& info;

    /* The number of jobs waiting */
    int waiting;

    /* The number presently running */
    int running;

    /* And the number we deferred executing because of job limits */
    int deferred;

    /* Notification callbacks */
    beast::insight::Event dequeue;
    beast::insight::Event execute;

    JobTypeData (JobTypeInfo const& info_,
            beast::insight::Collector::ptr const& collector, Logs& logs) noexcept
        : m_load (logs.journal ("LoadMonitor"))
        , m_collector (collector)
        , info (info_)
        , waiting (0)
        , running (0)
        , deferred (0)
    {
        m_load.setTargetLatency (
            info.getAverageLatency (),
            info.getPeakLatency());

        if (!info.special ())
        {
            dequeue = m_collector->make_event (info.name () + "_q");
            execute = m_collector->make_event (info.name ());
        }
    }

    /* Not copy-constructible or assignable */
    JobTypeData (JobTypeData const& other) = delete;
    JobTypeData& operator= (JobTypeData const& other) = delete;

    std::string name () const
    {
        return info.name ();
    }

    JobType type () const
    {
        return info.type ();
    }

    LoadMonitor& load ()
    {
        return m_load;
    }

    LoadMonitor::Stats stats ()
    {
        return m_load.getStats ();
    }
};

}

#endif
