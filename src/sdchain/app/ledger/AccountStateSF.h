//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_LEDGER_ACCOUNTSTATESF_H_INCLUDED
#define SDCHAIN_APP_LEDGER_ACCOUNTSTATESF_H_INCLUDED

#include <sdchain/app/ledger/AbstractFetchPackContainer.h>
#include <sdchain/shamap/SHAMapSyncFilter.h>
#include <sdchain/shamap/Family.h>

namespace sdchain {

// This class is only needed on add functions
// sync filter for account state nodes during ledger sync
class AccountStateSF
    : public SHAMapSyncFilter
{
private:
    Family& f_;
    AbstractFetchPackContainer& fp_;

public:
    AccountStateSF(Family&, AbstractFetchPackContainer&);

    // Note that the nodeData is overwritten by this call
    void gotNode (bool fromFilter,
                  SHAMapHash const& nodeHash,
                  Blob&& nodeData,
                  SHAMapTreeNode::TNType) const override;

    boost::optional<Blob>
    getNode(SHAMapHash const& nodeHash) const override;
};

} // sdchain

#endif
