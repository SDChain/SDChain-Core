//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#ifndef BEAST_INSIGHT_GROUPS_H_INCLUDED
#define BEAST_INSIGHT_GROUPS_H_INCLUDED

#include <sdchain/beast/insight/Collector.h>
#include <sdchain/beast/insight/Group.h>

#include <memory>
#include <string>

namespace beast {
namespace insight {

/** A container for managing a set of metric groups. */
class Groups
{
public:
    virtual ~Groups() = 0;

    /** Find or create a new collector with a given name. */
    /** @{ */
    virtual
    Group::ptr const&
    get (std::string const& name) = 0;

    Group::ptr const&
    operator[] (std::string const& name)
    {
        return get (name);
    }
    /** @} */
};

/** Create a group container that uses the specified collector. */
std::unique_ptr <Groups> make_Groups (Collector::ptr const& collector);

}
}

#endif
