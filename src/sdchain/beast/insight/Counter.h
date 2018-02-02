//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#ifndef BEAST_INSIGHT_COUNTER_H_INCLUDED
#define BEAST_INSIGHT_COUNTER_H_INCLUDED

#include <sdchain/beast/insight/Base.h>
#include <sdchain/beast/insight/CounterImpl.h>

#include <memory>

namespace beast {
namespace insight {

/** A metric for measuring an integral value.

    A counter is a gauge calculated at the server. The owner of the counter
    may increment and decrement the value by an amount.

    This is a lightweight reference wrapper which is cheap to copy and assign.
    When the last reference goes away, the metric is no longer collected.
*/
class Counter : public Base
{
public:
    using value_type = CounterImpl::value_type;

    /** Create a null metric.
        A null metric reports no information.
    */
    Counter ()
    {
    }

    /** Create the metric reference the specified implementation.
        Normally this won't be called directly. Instead, call the appropriate
        factory function in the Collector interface.
        @see Collector.
    */
    explicit Counter (std::shared_ptr <CounterImpl> const& impl)
        : m_impl (impl)
    {
    }

    /** Increment the counter. */
    /** @{ */
    void
    increment (value_type amount) const
    {
        if (m_impl)
            m_impl->increment (amount);
    }

    Counter const&
    operator+= (value_type amount) const
    {
        increment (amount);
        return *this;
    }

    Counter const&
    operator-= (value_type amount) const
    {
        increment (-amount);
        return *this;
    }

    Counter const&
    operator++ () const
    {
        increment (1);
        return *this;
    }

    Counter const&
    operator++ (int) const
    {
        increment (1);
        return *this;
    }

    Counter const&
    operator-- () const
    {
        increment (-1);
        return *this;
    }

    Counter const&
    operator-- (int) const
    {
        increment (-1);
        return *this;
    }
    /** @} */

    std::shared_ptr <CounterImpl> const&
    impl () const
    {
        return m_impl;
    }

private:
    std::shared_ptr <CounterImpl> m_impl;
};

}
}

#endif
