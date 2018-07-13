//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PEERFINDER_SOURCE_H_INCLUDED
#define SDCHAIN_PEERFINDER_SOURCE_H_INCLUDED

#include <sdchain/peerfinder/PeerfinderManager.h>
#include <sdchain/beast/core/SharedObject.h>
#include <boost/system/error_code.hpp>

namespace sdchain {
namespace PeerFinder {

/** A static or dynamic source of peer addresses.
    These are used as fallbacks when we are bootstrapping and don't have
    a local cache, or when none of our addresses are functioning. Typically
    sources will represent things like static text in the config file, a
    separate local file with addresses, or a remote HTTPS URL that can
    be updated automatically. Another solution is to use a custom DNS server
    that hands out peer IP addresses when name lookups are performed.
*/
class Source : public beast::SharedObject
{
public:
    /** The results of a fetch. */
    struct Results
    {
        // error_code on a failure
        boost::system::error_code error;

        // list of fetched endpoints
        IPAddresses addresses;
    };

    virtual ~Source () { }
    virtual std::string const& name () = 0;
    virtual void cancel () { }
    virtual void fetch (Results& results, beast::Journal journal) = 0;
};

}
}

#endif
