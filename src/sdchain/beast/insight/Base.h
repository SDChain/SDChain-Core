//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#ifndef BEAST_INSIGHT_BASE_H_INCLUDED
#define BEAST_INSIGHT_BASE_H_INCLUDED

#include <memory>

namespace beast {
namespace insight {

/** Base for all metrics and hooks. */
class Base
{
public:
    virtual ~Base () = 0;
    Base() = default;
    Base(Base const&) = default;
    Base& operator=(Base const&) = default;
};

}
}

#endif
