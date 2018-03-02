//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2015 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_SHAMAP_FAMILY_H_INCLUDED
#define SDCHAIN_SHAMAP_FAMILY_H_INCLUDED

#include <sdchain/basics/Log.h>
#include <sdchain/shamap/FullBelowCache.h>
#include <sdchain/shamap/TreeNodeCache.h>
#include <sdchain/nodestore/Database.h>
#include <sdchain/beast/utility/Journal.h>
#include <cstdint>

namespace sdchain {

class Family
{
public:
    virtual ~Family() = default;

    virtual
    beast::Journal const&
    journal() = 0;

    virtual
    FullBelowCache&
    fullbelow() = 0;

    virtual
    FullBelowCache const&
    fullbelow() const = 0;

    virtual
    TreeNodeCache&
    treecache() = 0;

    virtual
    TreeNodeCache const&
    treecache() const = 0;

    virtual
    NodeStore::Database&
    db() = 0;

    virtual
    NodeStore::Database const&
    db() const = 0;

    virtual
    void
    missing_node (std::uint32_t refNum) = 0;

    virtual
    void
    missing_node (uint256 const& refHash) = 0;
};

} // sdchain

#endif
