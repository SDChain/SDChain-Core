//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/basics/CountedObject.h>

namespace sdchain {

CountedObjects& CountedObjects::getInstance ()
{
    static CountedObjects instance;

    return instance;
}

CountedObjects::CountedObjects ()
    : m_count (0)
    , m_head (nullptr)
{
}

CountedObjects::~CountedObjects ()
{
}

CountedObjects::List CountedObjects::getCounts (int minimumThreshold) const
{
    List counts;

    // When other operations are concurrent, the count
    // might be temporarily less than the actual count.
    int const count = m_count.load ();

    counts.reserve (count);

    CounterBase* counter = m_head.load ();

    while (counter != nullptr)
    {
        if (counter->getCount () >= minimumThreshold)
        {
            Entry entry;

            entry.first = counter->getName ();
            entry.second = counter->getCount ();

            counts.push_back (entry);
        }

        counter = counter->getNext ();
    }

    return counts;
}

//------------------------------------------------------------------------------

CountedObjects::CounterBase::CounterBase ()
    : m_count (0)
{
    // Insert ourselves at the front of the lock-free linked list

    CountedObjects& instance = CountedObjects::getInstance ();
    CounterBase* head;

    do
    {
        head = instance.m_head.load ();
        m_next = head;
    }
    while (instance.m_head.exchange (this) != head);

    ++instance.m_count;
}

CountedObjects::CounterBase::~CounterBase ()
{
    // VFALCO NOTE If the counters are destroyed before the singleton,
    //             undefined behavior will result if the singleton's member
    //             functions are called.
}

} // sdchain
