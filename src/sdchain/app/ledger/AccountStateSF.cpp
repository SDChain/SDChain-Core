//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/ledger/AccountStateSF.h>

namespace sdchain {

AccountStateSF::AccountStateSF(Family& f, AbstractFetchPackContainer& fp)
    : f_(f)
    , fp_(fp)
{
}

void AccountStateSF::gotNode (bool fromFilter,
                              SHAMapHash const& nodeHash,
                              Blob&& nodeData,
                              SHAMapTreeNode::TNType) const
{
    f_.db().store(hotACCOUNT_NODE, std::move(nodeData),
        nodeHash.as_uint256());
}

boost::optional<Blob>
AccountStateSF::getNode(SHAMapHash const& nodeHash) const
{
    return fp_.getFetchPack(nodeHash.as_uint256());
}

} // sdchain
