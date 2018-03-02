//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_CORE_SEMAPHORE_H_INCLUDED
#define SDCHAIN_CORE_SEMAPHORE_H_INCLUDED

#include <condition_variable>
#include <mutex>

namespace sdchain {

template <class Mutex, class CondVar>
class basic_semaphore
{
private:
    using scoped_lock = std::unique_lock <Mutex>;

    Mutex m_mutex;
    CondVar m_cond;
    std::size_t m_count;

public:
    using size_type = std::size_t;

    /** Create the semaphore, with an optional initial count.
        If unspecified, the initial count is zero.
    */
    explicit basic_semaphore (size_type count = 0)
        : m_count (count)
    {
    }

    /** Increment the count and unblock one waiting thread. */
    void notify ()
    {
        scoped_lock lock (m_mutex);
        ++m_count;
        m_cond.notify_one ();
    }

    /** Block until notify is called. */
    void wait ()
    {
        scoped_lock lock (m_mutex);
        while (m_count == 0)
            m_cond.wait (lock);
        --m_count;
    }

    /** Perform a non-blocking wait.
        @return `true` If the wait would be satisfied.
    */
    bool try_wait ()
    {
        scoped_lock lock (m_mutex);
        if (m_count == 0)
            return false;
        --m_count;
        return true;
    }
};

using semaphore = basic_semaphore <std::mutex, std::condition_variable>;

} // sdchain

#endif

