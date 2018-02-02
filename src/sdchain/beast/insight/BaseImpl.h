//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#ifndef BEAST_INSIGHT_BASEIMPL_H_INCLUDED
#define BEAST_INSIGHT_BASEIMPL_H_INCLUDED

#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>

namespace beast {
namespace insight {

/** Base for all metrics and hook implementations. */
class BaseImpl
{
public:
    using ptr = std::shared_ptr <BaseImpl>;

    virtual ~BaseImpl () = 0;
};

}
}

#endif
