//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_NODESTORE_TUNING_H_INCLUDED
#define SDCHAIN_NODESTORE_TUNING_H_INCLUDED

namespace sdchain {
namespace NodeStore {

enum
{
    // Target cache size of the TaggedCache used to hold nodes
    cacheTargetSize     = 16384

    // Expiration time for cached nodes
    ,cacheTargetSeconds = 300

    // Fraction of the cache one query source can take
    ,asyncDivider = 8
};

}
}

#endif
