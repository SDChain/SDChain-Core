//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_LEDGER_CONSENSUSTRANSSETSF_H_INCLUDED
#define SDCHAIN_APP_LEDGER_CONSENSUSTRANSSETSF_H_INCLUDED

#include <sdchain/app/main/Application.h>
#include <sdchain/shamap/SHAMapSyncFilter.h>
#include <sdchain/basics/TaggedCache.h>

namespace sdchain {

// Sync filters allow low-level SHAMapSync code to interact correctly with
// higher-level structures such as caches and transaction stores

// This class is needed on both add and check functions
// sync filter for transaction sets during consensus building
class ConsensusTransSetSF : public SHAMapSyncFilter
{
public:
    using NodeCache = TaggedCache <SHAMapHash, Blob>;

    ConsensusTransSetSF (Application& app, NodeCache& nodeCache);

    // Note that the nodeData is overwritten by this call
    void gotNode (bool fromFilter,
                  SHAMapHash const& nodeHash,
                  Blob&& nodeData,
                  SHAMapTreeNode::TNType) const override;

    boost::optional<Blob>
    getNode (SHAMapHash const& nodeHash) const override;

private:
    Application& app_;
    NodeCache& m_nodeCache;
    beast::Journal j_;
};

} // sdchain

#endif
