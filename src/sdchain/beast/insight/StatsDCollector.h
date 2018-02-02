//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#ifndef BEAST_INSIGHT_STATSDCOLLECTOR_H_INCLUDED
#define BEAST_INSIGHT_STATSDCOLLECTOR_H_INCLUDED

#include <sdchain/beast/insight/Collector.h>

#include <sdchain/beast/utility/Journal.h>
#include <sdchain/beast/net/IPEndpoint.h>

namespace beast {
namespace insight {

/** A Collector that reports metrics to a StatsD server.
    Reference:
        https://github.com/b/statsd_spec
*/
class StatsDCollector : public Collector
{
public:
    /** Create a StatsD collector.
        @param address The IP address and port of the StatsD server.
        @param prefix A string pre-pended before each metric name.
        @param journal Destination for logging output.
    */
    static
    std::shared_ptr <StatsDCollector>
    New (IP::Endpoint const& address,
        std::string const& prefix, Journal journal);
};

}
}

#endif
