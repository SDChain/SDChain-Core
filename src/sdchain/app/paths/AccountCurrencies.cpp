//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/paths/AccountCurrencies.h>

namespace sdchain {

hash_set<Currency> accountSourceCurrencies (
    AccountID const& account,
    std::shared_ptr<SDChainLineCache> const& lrCache,
    bool includeSDA)
{
    hash_set<Currency> currencies;

    // YYY Only bother if they are above reserve
    if (includeSDA)
        currencies.insert (sdaCurrency());

    // List of sdchain lines.
    auto& sdchainLines = lrCache->getSDChainLines (account);

    for (auto const& item : sdchainLines)
    {
        auto rspEntry = (SDChainState*) item.get ();
        assert (rspEntry);
        if (!rspEntry)
            continue;

        auto& saBalance = rspEntry->getBalance ();

        // Filter out non
        if (saBalance > zero
            // Have IOUs to send.
            || (rspEntry->getLimitPeer ()
                // Peer extends credit.
                && ((-saBalance) < rspEntry->getLimitPeer ()))) // Credit left.
        {
            currencies.insert (saBalance.getCurrency ());
        }
    }

    currencies.erase (badCurrency());
    return currencies;
}

hash_set<Currency> accountDestCurrencies (
    AccountID const& account,
    std::shared_ptr<SDChainLineCache> const& lrCache,
    bool includeSDA)
{
    hash_set<Currency> currencies;

    if (includeSDA)
        currencies.insert (sdaCurrency());
    // Even if account doesn't exist

    // List of sdchain lines.
    auto& sdchainLines = lrCache->getSDChainLines (account);

    for (auto const& item : sdchainLines)
    {
        auto rspEntry = (SDChainState*) item.get ();
        assert (rspEntry);
        if (!rspEntry)
            continue;

        auto& saBalance  = rspEntry->getBalance ();

        if (saBalance < rspEntry->getLimit ())                  // Can take more
            currencies.insert (saBalance.getCurrency ());
    }

    currencies.erase (badCurrency());
    return currencies;
}

} // sdchain
