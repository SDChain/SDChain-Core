//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_RESOURCE_GOSSIP_H_INCLUDED
#define SDCHAIN_RESOURCE_GOSSIP_H_INCLUDED

#include <sdchain/beast/net/IPEndpoint.h>

namespace sdchain {
namespace Resource {

/** Data format for exchanging consumption information across peers. */
struct Gossip
{
    /** Describes a single consumer. */
    struct Item
    {
        int balance;
        beast::IP::Endpoint address;
    };

    std::vector <Item> items;
};

}
}

#endif
