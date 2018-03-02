//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <sdchain/core/Stoppable.h>
#include <cassert>

namespace sdchain {

Stoppable::Stoppable (std::string name, RootStoppable& root)
    : m_name (std::move (name))
    , m_root (root)
    , m_child (this)
{
}

Stoppable::Stoppable (std::string name, Stoppable& parent)
    : m_name (std::move (name))
    , m_root (parent.m_root)
    , m_child (this)
{
    // Must not have stopping parent.
    assert (! parent.isStopping());

    parent.m_children.push_front (&m_child);
}

Stoppable::~Stoppable ()
{
    // Either we must not have started, or Children must be stopped.
    assert (!m_root.started() || m_childrenStopped);
}

bool Stoppable::isStopping() const
{
    return m_root.isStopping();
}

bool Stoppable::isStopped () const
{
    return m_stopped;
}

bool Stoppable::areChildrenStopped () const
{
    return m_childrenStopped;
}

void Stoppable::stopped ()
{
    m_stoppedEvent.signal();
}

void Stoppable::onPrepare ()
{
}

void Stoppable::onStart ()
{
}

void Stoppable::onStop ()
{
    stopped();
}

void Stoppable::onChildrenStopped ()
{
}

//------------------------------------------------------------------------------

void Stoppable::prepareRecursive ()
{
    for (Children::const_iterator iter (m_children.cbegin ());
        iter != m_children.cend(); ++iter)
        iter->stoppable->prepareRecursive ();
    onPrepare ();
}

void Stoppable::startRecursive ()
{
    onStart ();
    for (Children::const_iterator iter (m_children.cbegin ());
        iter != m_children.cend(); ++iter)
        iter->stoppable->startRecursive ();
}

void Stoppable::stopAsyncRecursive (beast::Journal j)
{
    using namespace std::chrono;
    auto const start = high_resolution_clock::now();
    onStop ();
    auto const ms = duration_cast<milliseconds>(
        high_resolution_clock::now() - start);

#ifdef NDEBUG
    using namespace std::chrono_literals;
    if (ms >= 10ms)
        if (auto stream = j.fatal())
            stream << m_name << "::onStop took " << ms.count() << "ms";
#else
    (void)ms;
#endif

    for (Children::const_iterator iter (m_children.cbegin ());
        iter != m_children.cend(); ++iter)
        iter->stoppable->stopAsyncRecursive(j);
}

void Stoppable::stopRecursive (beast::Journal j)
{
    // Block on each child from the bottom of the tree up.
    //
    for (Children::const_iterator iter (m_children.cbegin ());
        iter != m_children.cend(); ++iter)
        iter->stoppable->stopRecursive (j);

    // if we get here then all children have stopped
    //
    m_childrenStopped = true;
    onChildrenStopped ();

    // Now block on this Stoppable.
    //
    bool const timedOut (! m_stoppedEvent.wait (1 * 1000)); // milliseconds
    if (timedOut)
    {
        if (auto stream = j.error())
            stream << "Waiting for '" << m_name << "' to stop";
        m_stoppedEvent.wait ();
    }

    // once we get here, we know the stoppable has stopped.
    m_stopped = true;
}

//------------------------------------------------------------------------------

RootStoppable::RootStoppable (std::string name)
    : Stoppable (std::move (name), *this)
{
}

RootStoppable::~RootStoppable ()
{
    using namespace std::chrono_literals;
    jobCounter_.join(m_name.c_str(), 1s, debugLog());
}

bool RootStoppable::isStopping() const
{
    return m_calledStop;
}

void RootStoppable::prepare ()
{
    if (m_prepared.exchange (true) == false)
        prepareRecursive ();
}

void RootStoppable::start ()
{
    // Courtesy call to prepare.
    if (m_prepared.exchange (true) == false)
        prepareRecursive ();

    if (m_started.exchange (true) == false)
        startRecursive ();
}

void RootStoppable::stop (beast::Journal j)
{
    // Must have a prior call to start()
    assert (m_started);

    if (stopAsync (j))
        stopRecursive (j);
}

bool RootStoppable::stopAsync (beast::Journal j)
{
    bool alreadyCalled;
    {
        // Even though m_calledStop is atomic, we change its value under a
        // lock.  This removes a small timing window that occurs if the
        // waiting thread is handling a spurious wakeup while m_calledStop
        // changes state.
        std::unique_lock<std::mutex> lock (m_);
        alreadyCalled = m_calledStop.exchange (true);
    }
    if (alreadyCalled)
    {
        if (auto stream = j.warn())
            stream << "Stoppable::stop called again";
        return false;
    }

    // Wait until all in-flight JobQueue Jobs are completed.
    using namespace std::chrono_literals;
    jobCounter_.join (m_name.c_str(), 1s, j);

    c_.notify_all();
    stopAsyncRecursive(j);
    return true;
}

}
