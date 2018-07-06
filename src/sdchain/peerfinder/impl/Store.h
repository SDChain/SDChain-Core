//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PEERFINDER_STORE_H_INCLUDED
#define SDCHAIN_PEERFINDER_STORE_H_INCLUDED

namespace sdchain {
namespace PeerFinder {

/** Abstract persistence for PeerFinder data. */
class Store
{
public:
    virtual ~Store () { }

    // load the bootstrap cache
    using load_callback = std::function <void (beast::IP::Endpoint, int)>;
    virtual std::size_t load (load_callback const& cb) = 0;

    // save the bootstrap cache
    struct Entry
    {
        beast::IP::Endpoint endpoint;
        int valence;
    };
    virtual void save (std::vector <Entry> const& v) = 0;
};

}
}

#endif
