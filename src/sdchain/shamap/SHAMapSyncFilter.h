//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_SHAMAP_SHAMAPSYNCFILTER_H_INCLUDED
#define SDCHAIN_SHAMAP_SHAMAPSYNCFILTER_H_INCLUDED

#include <sdchain/shamap/SHAMapNodeID.h>
#include <sdchain/shamap/SHAMapTreeNode.h>

/** Callback for filtering SHAMap during sync. */
namespace sdchain {

class SHAMapSyncFilter
{
public:
    virtual ~SHAMapSyncFilter () = default;
    SHAMapSyncFilter() = default;
    SHAMapSyncFilter(SHAMapSyncFilter const&) = delete;
    SHAMapSyncFilter& operator=(SHAMapSyncFilter const&) = delete;

    // Note that the nodeData is overwritten by this call
    virtual void gotNode (bool fromFilter,
                          SHAMapHash const& nodeHash,
                          Blob&& nodeData,
                          SHAMapTreeNode::TNType type) const = 0;

    virtual
    boost::optional<Blob>
    getNode(SHAMapHash const& nodeHash) const = 0;
};

}

#endif
