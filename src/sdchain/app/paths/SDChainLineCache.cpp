//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/paths/SDChainLineCache.h>
#include <sdchain/ledger/OpenView.h>

namespace sdchain {

SDChainLineCache::SDChainLineCache(
    std::shared_ptr <ReadView const> const& ledger)
{
    // We want the caching that OpenView provides
    // And we need to own a shared_ptr to the input view
    // VFALCO TODO This should be a CachedLedger
    mLedger = std::make_shared<OpenView>(&*ledger, ledger);
}

std::vector<SDChainState::pointer> const&
SDChainLineCache::getSDChainLines (AccountID const& accountID)
{
    AccountKey key (accountID, hasher_ (accountID));

    std::lock_guard <std::mutex> sl (mLock);

    auto it = lines_.emplace (key,
        std::vector<SDChainState::pointer>());

    if (it.second)
        it.first->second = getSDChainStateItems (
            accountID, *mLedger);

    return it.first->second;
}

} // sdchain
