//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_PATHS_SDCHAINLINECACHE_H_INCLUDED
#define SDCHAIN_APP_PATHS_SDCHAINLINECACHE_H_INCLUDED

#include <sdchain/app/ledger/Ledger.h>
#include <sdchain/app/paths/SDChainState.h>
#include <sdchain/basics/hardened_hash.h>
#include <cstddef>
#include <memory>
#include <mutex>
#include <vector>

namespace sdchain {

// Used by Pathfinder
class SDChainLineCache
{
public:
    explicit
    SDChainLineCache (
        std::shared_ptr <ReadView const> const& l);

    std::shared_ptr <ReadView const> const&
    getLedger () const
    {
        return mLedger;
    }

    std::vector<SDChainState::pointer> const&
    getSDChainLines (AccountID const& accountID);

private:
    std::mutex mLock;

    sdchain::hardened_hash<> hasher_;
    std::shared_ptr <ReadView const> mLedger;

    struct AccountKey
    {
        AccountID account_;
        std::size_t hash_value_;

        AccountKey (AccountID const& account, std::size_t hash)
            : account_ (account)
            , hash_value_ (hash)
        { }

        AccountKey (AccountKey const& other) = default;

        AccountKey&
        operator=(AccountKey const& other) = default;

        bool operator== (AccountKey const& lhs) const
        {
            return hash_value_ == lhs.hash_value_ && account_ == lhs.account_;
        }

        std::size_t
        get_hash () const
        {
            return hash_value_;
        }

        struct Hash
        {
            std::size_t
            operator () (AccountKey const& key) const noexcept
            {
                return key.get_hash ();
            }
        };
    };

    hash_map <
        AccountKey,
        std::vector <SDChainState::pointer>,
        AccountKey::Hash> lines_;
};

} // sdchain

#endif
