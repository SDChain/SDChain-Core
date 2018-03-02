//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_SHAMAP_SHAMAPMISSINGNODE_H_INCLUDED
#define SDCHAIN_SHAMAP_SHAMAPMISSINGNODE_H_INCLUDED

#include <sdchain/basics/base_uint.h>
#include <sdchain/shamap/SHAMapTreeNode.h>
#include <iosfwd>
#include <stdexcept>

namespace sdchain {

enum class SHAMapType
{
    TRANSACTION  = 1,    // A tree of transactions
    STATE        = 2,    // A tree of state nodes
    FREE         = 3,    // A tree not part of a ledger
};

class SHAMapMissingNode
    : public std::runtime_error
{
private:
    SHAMapType mType;
    SHAMapHash mNodeHash;
    uint256    mNodeID;
public:
    SHAMapMissingNode (SHAMapType t,
                       SHAMapHash const& nodeHash)
        : std::runtime_error ("SHAMapMissingNode")
        , mType (t)
        , mNodeHash (nodeHash)
    {
    }

    SHAMapMissingNode (SHAMapType t,
                       uint256 const& nodeID)
        : std::runtime_error ("SHAMapMissingNode")
        , mType (t)
        , mNodeID (nodeID)
    {
    }

    friend std::ostream& operator<< (std::ostream&, SHAMapMissingNode const&);
};

} // sdchain

#endif
