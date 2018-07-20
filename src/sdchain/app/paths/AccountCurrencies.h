//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_PATHS_ACCOUNTCURRENCIES_H_INCLUDED
#define SDCHAIN_APP_PATHS_ACCOUNTCURRENCIES_H_INCLUDED

#include <sdchain/app/paths/SDChainLineCache.h>
#include <sdchain/protocol/types.h>

namespace sdchain {

hash_set<Currency>
accountDestCurrencies(
    AccountID const& account,
        std::shared_ptr<SDChainLineCache> const& cache,
            bool includeSDA);

hash_set<Currency>
accountSourceCurrencies(
    AccountID const& account,
        std::shared_ptr<SDChainLineCache> const& lrLedger,
             bool includeSDA);

} // sdchain

#endif
