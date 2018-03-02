//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_CORE_JOBTYPEINFO_H_INCLUDED
#define SDCHAIN_CORE_JOBTYPEINFO_H_INCLUDED

namespace sdchain
{

/** Holds all the 'static' information about a job, which does not change */
class JobTypeInfo
{
private:
    JobType const m_type;
    std::string const m_name;

    /** The limit on the number of running jobs for this job type. */
    int const m_limit;

    /** Special jobs are not dispatched via the job queue */
    bool const m_special;

    /** Average and peak latencies for this job type. 0 is none specified */
    std::chrono::milliseconds const m_avgLatency;
    std::chrono::milliseconds const m_peakLatency;

public:
    // Not default constructible
    JobTypeInfo () = delete;

    JobTypeInfo (JobType type, std::string name, int limit,
            bool special, std::chrono::milliseconds avgLatency,
            std::chrono::milliseconds peakLatency)
        : m_type (type)
        , m_name (name)
        , m_limit (limit)
        , m_special (special)
        , m_avgLatency (avgLatency)
        , m_peakLatency (peakLatency)
    {

    }

    JobType type () const
    {
        return m_type;
    }

    std::string name () const
    {
        return m_name;
    }

    int limit () const
    {
        return m_limit;
    }

    bool special () const
    {
        return m_special;
    }

    std::chrono::milliseconds getAverageLatency () const
    {
        return m_avgLatency;
    }

    std::chrono::milliseconds getPeakLatency () const
    {
        return m_peakLatency;
    }
};

}

#endif
