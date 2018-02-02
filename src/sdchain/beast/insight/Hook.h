//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#ifndef BEAST_INSIGHT_HOOK_H_INCLUDED
#define BEAST_INSIGHT_HOOK_H_INCLUDED

#include <sdchain/beast/insight/Base.h>
#include <sdchain/beast/insight/HookImpl.h>

#include <memory>

namespace beast {
namespace insight {

/** A reference to a handler for performing polled collection. */
class Hook : public Base
{
public:
    /** Create a null hook.
        A null hook has no associated handler.
    */
    Hook ()
        { }

    /** Create a hook referencing the specified implementation.
        Normally this won't be called directly. Instead, call the appropriate
        factory function in the Collector interface.
        @see Collector.
    */
    explicit Hook (std::shared_ptr <HookImpl> const& impl)
        : m_impl (impl)
        { }

    std::shared_ptr <HookImpl> const& impl () const
    {
        return m_impl;
    }

private:
    std::shared_ptr <HookImpl> m_impl;
};

}
}

#endif
