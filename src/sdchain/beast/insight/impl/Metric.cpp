//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#include <sdchain/beast/insight/Base.h>
#include <sdchain/beast/insight/BaseImpl.h>
#include <sdchain/beast/insight/CounterImpl.h>
#include <sdchain/beast/insight/EventImpl.h>
#include <sdchain/beast/insight/GaugeImpl.h>
#include <sdchain/beast/insight/MeterImpl.h>

namespace beast {
namespace insight {

Base::~Base ()
{
}

BaseImpl::~BaseImpl ()
{
}

CounterImpl::~CounterImpl ()
{
}

EventImpl::~EventImpl ()
{
}

GaugeImpl::~GaugeImpl ()
{
}

MeterImpl::~MeterImpl ()
{
}

}
}
