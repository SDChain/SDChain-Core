//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/ledger/TransactionStateSF.h>

namespace sdchain {

TransactionStateSF::TransactionStateSF(Family& f,
    AbstractFetchPackContainer& fp)
        : f_(f)
        , fp_(fp)
{
}

void TransactionStateSF::gotNode (bool fromFilter,
                                  SHAMapHash const& nodeHash,
                                  Blob&& nodeData,
                                  SHAMapTreeNode::TNType type) const
{
    assert(type !=
        SHAMapTreeNode::tnTRANSACTION_NM);
    f_.db().store(hotTRANSACTION_NODE,
        std::move (nodeData), nodeHash.as_uint256());
}

boost::optional<Blob>
TransactionStateSF::getNode(SHAMapHash const& nodeHash) const
{
    return fp_.getFetchPack(nodeHash.as_uint256());
}

} // sdchain
