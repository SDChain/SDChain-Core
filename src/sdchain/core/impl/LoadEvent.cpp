//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/core/LoadEvent.h>
#include <sdchain/core/LoadMonitor.h>
#include <cassert>
#include <iomanip>

namespace sdchain {

LoadEvent::LoadEvent (
        LoadMonitor& monitor,
        std::string const& name,
        bool shouldStart)
    : monitor_ (monitor)
    , running_ (shouldStart)
    , name_ (name)
    , mark_ { std::chrono::steady_clock::now() }
    , timeWaiting_ {}
    , timeRunning_ {}
{
}

LoadEvent::~LoadEvent ()
{
    if (running_)
        stop ();
}

std::string const& LoadEvent::name () const
{
    return name_;
}

std::chrono::steady_clock::duration
LoadEvent::waitTime() const
{
    return timeWaiting_;
}

std::chrono::steady_clock::duration
LoadEvent::runTime() const
{
    return timeRunning_;
}

void LoadEvent::reName (std::string const& name)
{
    name_ = name;
}

void LoadEvent::start ()
{
    auto const now = std::chrono::steady_clock::now();

    // If we had already called start, this call will
    // replace the previous one. Any time accumulated will
    // be counted as "waiting".
    timeWaiting_ += now - mark_;
    mark_ = now;
    running_ = true;
}

void LoadEvent::stop ()
{
    assert (running_);

    auto const now = std::chrono::steady_clock::now();

    timeRunning_ += now - mark_;
    mark_ = now;
    running_ = false;

    monitor_.addLoadSample (*this);
}

} // sdchain
