//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#ifndef BEAST_INSIGHT_GAUGEIMPL_H_INCLUDED
#define BEAST_INSIGHT_GAUGEIMPL_H_INCLUDED

#include <sdchain/beast/insight/BaseImpl.h>

namespace beast {
namespace insight {

class Gauge;

class GaugeImpl
    : public std::enable_shared_from_this <GaugeImpl>
    , public BaseImpl
{
public:
    using value_type = std::uint64_t;
    using difference_type = std::int64_t;

    virtual ~GaugeImpl () = 0;
    virtual void set (value_type value) = 0;
    virtual void increment (difference_type amount) = 0;
};

}
}

#endif
