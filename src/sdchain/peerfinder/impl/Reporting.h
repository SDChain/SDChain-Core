//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PEERFINDER_REPORTING_H_INCLUDED
#define SDCHAIN_PEERFINDER_REPORTING_H_INCLUDED

namespace sdchain {
namespace PeerFinder {

/** Severity levels for test reporting.
    This allows more fine grained control over reporting for diagnostics.
*/
struct Reporting
{
    // Report simulation parameters
    static bool const params = true;

    // Report simulation crawl time-evolution
    static bool const crawl = true;

    // Report nodes aggregate statistics
    static bool const nodes = true;

    // Report nodes detailed information
    static bool const dump_nodes = false;
};

}
}

#endif
