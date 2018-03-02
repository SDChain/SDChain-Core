//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_BASICS_COUNTEDOBJECT_H_INCLUDED
#define SDCHAIN_BASICS_COUNTEDOBJECT_H_INCLUDED

#include <atomic>
#include <string>
#include <utility>
#include <vector>

namespace sdchain {

/** Manages all counted object types. */
class CountedObjects
{
public:
    static CountedObjects& getInstance ();

    using Entry = std::pair <std::string, int>;
    using List = std::vector <Entry>;

    List getCounts (int minimumThreshold) const;

public:
    /** Implementation for @ref CountedObject.

        @internal
    */
    class CounterBase
    {
    public:
        CounterBase ();

        virtual ~CounterBase ();

        int increment () noexcept
        {
            return ++m_count;
        }

        int decrement () noexcept
        {
            return --m_count;
        }

        int getCount () const noexcept
        {
            return m_count.load ();
        }

        CounterBase* getNext () const noexcept
        {
            return m_next;
        }

        virtual char const* getName () const = 0;

    private:
        virtual void checkPureVirtual () const = 0;

    protected:
        std::atomic <int> m_count;
        CounterBase* m_next;
    };

private:
    CountedObjects ();
    ~CountedObjects ();

private:
    std::atomic <int> m_count;
    std::atomic <CounterBase*> m_head;
};

//------------------------------------------------------------------------------

/** Tracks the number of instances of an object.

    Derived classes have their instances counted automatically. This is used
    for reporting purposes.

    @ingroup sdchain_basics
*/
template <class Object>
class CountedObject
{
public:
    CountedObject ()
    {
        getCounter ().increment ();
    }

    CountedObject (CountedObject const&)
    {
        getCounter ().increment ();
    }

    CountedObject& operator=(CountedObject const&) = default;

    ~CountedObject ()
    {
        getCounter ().decrement ();
    }

private:
    class Counter : public CountedObjects::CounterBase
    {
    public:
        Counter () { }

        char const* getName () const
        {
            return Object::getCountedObjectName ();
        }

        void checkPureVirtual () const { }
    };

private:
    static Counter& getCounter()
    {
        static Counter c;
        return c;
    }
};

} // sdchain

#endif
