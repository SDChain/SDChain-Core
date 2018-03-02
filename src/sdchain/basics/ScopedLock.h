//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.

    Portions of this file are from JUCE.
    Copyright (c) 2013 - Raw Material Software Ltd.
    Please visit http://www.juce.com



*/
//==============================================================================

#ifndef SDCHAIN_BASICS_SCOPEDLOCK_H_INCLUDED
#define SDCHAIN_BASICS_SCOPEDLOCK_H_INCLUDED

namespace sdchain
{

//==============================================================================
/**
    Automatically unlocks and re-locks a mutex object.

    This is the reverse of a std::lock_guard object - instead of locking the mutex
    for the lifetime of this object, it unlocks it.

    Make sure you don't try to unlock mutexes that aren't actually locked!

    e.g. @code

    std::mutex mut;

    for (;;)
    {
        std::lock_guard<std::mutex> myScopedLock{mut};
        // mut is now locked

        ... do some stuff with it locked ..

        while (xyz)
        {
            ... do some stuff with it locked ..

            GenericScopedUnlock<std::mutex> unlocker{mut};

            // mut is now unlocked for the remainder of this block,
            // and re-locked at the end.

            ...do some stuff with it unlocked ...
        }  // mut gets locked here.

    }  // mut gets unlocked here
    @endcode

*/
template <class MutexType>
class GenericScopedUnlock
{
    MutexType& lock_;
public:
    /** Creates a GenericScopedUnlock.

        As soon as it is created, this will unlock the CriticalSection, and
        when the ScopedLock object is deleted, the CriticalSection will
        be re-locked.

        Make sure this object is created and deleted by the same thread,
        otherwise there are no guarantees what will happen! Best just to use it
        as a local stack object, rather than creating one with the new() operator.
    */
    explicit GenericScopedUnlock (MutexType& lock) noexcept
        : lock_ (lock)
    {
        lock.unlock();
    }

    GenericScopedUnlock (GenericScopedUnlock const&) = delete;
    GenericScopedUnlock& operator= (GenericScopedUnlock const&) = delete;

    /** Destructor.

        The CriticalSection will be unlocked when the destructor is called.

        Make sure this object is created and deleted by the same thread,
        otherwise there are no guarantees what will happen!
    */
    ~GenericScopedUnlock() noexcept(false)
    {
        lock_.lock();
    }
};

} // sdchain
#endif

