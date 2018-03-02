//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_SHAMAP_TREENODECACHE_H_INCLUDED
#define SDCHAIN_SHAMAP_TREENODECACHE_H_INCLUDED

#include <sdchain/shamap/TreeNodeCache.h>
#include <sdchain/shamap/SHAMapTreeNode.h>

namespace sdchain {

class SHAMapAbstractNode;

using TreeNodeCache = TaggedCache <uint256, SHAMapAbstractNode>;

} // sdchain

#endif
