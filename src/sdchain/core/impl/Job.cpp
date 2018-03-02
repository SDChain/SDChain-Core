//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/core/Job.h>
#include <sdchain/beast/core/CurrentThreadName.h>
#include <cassert>

namespace sdchain {

Job::Job ()
    : mType (jtINVALID)
    , mJobIndex (0)
{
}

Job::Job (JobType type, std::uint64_t index)
    : mType (type)
    , mJobIndex (index)
{
}

Job::Job (JobType type,
          std::string const& name,
          std::uint64_t index,
          LoadMonitor& lm,
          std::function <void (Job&)> const& job,
          CancelCallback cancelCallback)
    : m_cancelCallback (cancelCallback)
    , mType (type)
    , mJobIndex (index)
    , mJob (job)
    , mName (name)
    , m_queue_time (clock_type::now ())
{
    m_loadEvent = std::make_shared <LoadEvent> (std::ref (lm), name, false);
}

JobType Job::getType () const
{
    return mType;
}

Job::CancelCallback Job::getCancelCallback () const
{
    assert (m_cancelCallback);
    return m_cancelCallback;
}

Job::clock_type::time_point const& Job::queue_time () const
{
    return m_queue_time;
}

bool Job::shouldCancel () const
{
    if (m_cancelCallback)
        return m_cancelCallback ();
    return false;
}

void Job::doJob ()
{
    beast::setCurrentThreadName ("doJob: " + mName);
    m_loadEvent->start ();
    m_loadEvent->reName (mName);

    mJob (*this);

    // Destroy the lambda, otherwise we won't include
    // its duration in the time measurement
    mJob = nullptr;
}

void Job::rename (std::string const& newName)
{
    mName = newName;
}

bool Job::operator> (const Job& j) const
{
    if (mType < j.mType)
        return true;

    if (mType > j.mType)
        return false;

    return mJobIndex > j.mJobIndex;
}

bool Job::operator>= (const Job& j) const
{
    if (mType < j.mType)
        return true;

    if (mType > j.mType)
        return false;

    return mJobIndex >= j.mJobIndex;
}

bool Job::operator< (const Job& j) const
{
    if (mType < j.mType)
        return false;

    if (mType > j.mType)
        return true;

    return mJobIndex < j.mJobIndex;
}

bool Job::operator<= (const Job& j) const
{
    if (mType < j.mType)
        return false;

    if (mType > j.mType)
        return true;

    return mJobIndex <= j.mJobIndex;
}

}
