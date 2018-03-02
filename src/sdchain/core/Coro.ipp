//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_CORE_COROINL_H_INCLUDED
#define SDCHAIN_CORE_COROINL_H_INCLUDED

namespace sdchain {

template <class F>
JobQueue::Coro::
Coro(Coro_create_t, JobQueue& jq, JobType type,
    std::string const& name, F&& f)
    : jq_(jq)
    , type_(type)
    , name_(name)
    , running_(false)
    , coro_(
        [this, fn = std::forward<F>(f)]
        (boost::coroutines::asymmetric_coroutine<void>::push_type& do_yield)
        {
            yield_ = &do_yield;
            yield();
            fn(shared_from_this());
#ifndef NDEBUG
            finished_ = true;
#endif
        }, boost::coroutines::attributes (1024 * 1024))
{
}

inline
JobQueue::Coro::
~Coro()
{
#ifndef NDEBUG
    assert(finished_);
#endif
}

inline
void
JobQueue::Coro::
yield() const
{
    {
        std::lock_guard<std::mutex> lock(jq_.m_mutex);
        ++jq_.nSuspend_;
    }
    (*yield_)();
}

inline
bool
JobQueue::Coro::
post()
{
    {
        std::lock_guard<std::mutex> lk(mutex_run_);
        running_ = true;
    }

    // sp keeps 'this' alive
    if (jq_.addJob(type_, name_,
        [this, sp = shared_from_this()](Job&)
        {
            resume();
        }))
    {
        return true;
    }

    // The coroutine will not run.  Clean up running_.
    std::lock_guard<std::mutex> lk(mutex_run_);
    running_ = false;
    cv_.notify_all();
    return false;
}

inline
void
JobQueue::Coro::
resume()
{
    {
        std::lock_guard<std::mutex> lk(mutex_run_);
        running_ = true;
    }
    {
        std::lock_guard<std::mutex> lock(jq_.m_mutex);
        --jq_.nSuspend_;
    }
    auto saved = detail::getLocalValues().release();
    detail::getLocalValues().reset(&lvs_);
    std::lock_guard<std::mutex> lock(mutex_);
    assert (coro_);
    coro_();
    detail::getLocalValues().release();
    detail::getLocalValues().reset(saved);
    std::lock_guard<std::mutex> lk(mutex_run_);
    running_ = false;
    cv_.notify_all();
}

inline
bool
JobQueue::Coro::
runnable() const
{
    return static_cast<bool>(coro_);
}

inline
void
JobQueue::Coro::
expectEarlyExit()
{
#ifndef NDEBUG
    if (! finished_)
#endif
    {
        // expectEarlyExit() must only ever be called from outside the
        // Coro's stack.  It you're inside the stack you can simply return
        // and be done.
        //
        // That said, since we're outside the Coro's stack, we need to
        // decrement the nSuspend that the Coro's call to yield caused.
        std::lock_guard<std::mutex> lock(jq_.m_mutex);
        --jq_.nSuspend_;
#ifndef NDEBUG
        finished_ = true;
#endif
    }
}

inline
void
JobQueue::Coro::
join()
{
    std::unique_lock<std::mutex> lk(mutex_run_);
    cv_.wait(lk,
        [this]()
        {
            return running_ == false;
        });
}

} // sdchain

#endif
