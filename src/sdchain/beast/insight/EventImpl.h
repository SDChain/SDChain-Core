//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#ifndef BEAST_INSIGHT_EVENTIMPL_H_INCLUDED
#define BEAST_INSIGHT_EVENTIMPL_H_INCLUDED

#include <sdchain/beast/insight/BaseImpl.h>

namespace beast {
namespace insight {

class Event;

class EventImpl
    : public std::enable_shared_from_this <EventImpl>
    , public BaseImpl
{
public:
    using value_type = std::chrono::milliseconds;

    virtual ~EventImpl () = 0;
    virtual void notify (value_type const& value) = 0;
};

}
}

#endif
