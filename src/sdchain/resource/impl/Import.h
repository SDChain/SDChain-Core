//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_RESOURCE_IMPORT_H_INCLUDED
#define SDCHAIN_RESOURCE_IMPORT_H_INCLUDED

#include <sdchain/resource/Consumer.h>

namespace sdchain {
namespace Resource {

/** A set of imported consumer data from a gossip origin. */
struct Import
{
    struct Item
    {
        int balance;
        Consumer consumer;
    };

    // Dummy argument required for zero-copy construction
    Import (int = 0)
        : whenExpires ()
    {
    }

    // When the imported data expires
    clock_type::time_point whenExpires;

    // List of remote entries
    std::vector <Item> items;
};

}
}

#endif
