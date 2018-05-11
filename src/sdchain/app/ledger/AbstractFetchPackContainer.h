//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_LEDGER_ABSTRACTFETCHPACKCONTAINER_H_INCLUDED
#define SDCHAIN_APP_LEDGER_ABSTRACTFETCHPACKCONTAINER_H_INCLUDED

#include <sdchain/basics/base_uint.h>
#include <sdchain/basics/Blob.h>
#include <boost/optional.hpp>

namespace sdchain {

/** An interface facilitating retrieval of fetch packs without
    an application or ledgermaster object.
*/
class AbstractFetchPackContainer
{
public:
    virtual ~AbstractFetchPackContainer() = default;

    /** Retrieves partial ledger data of the coresponding hash from peers.`

        @param nodeHash The 256-bit hash of the data to fetch.
        @return `boost::none` if the hash isn't cached,
            otherwise, the hash associated data.
    */
    virtual boost::optional<Blob> getFetchPack(uint256 const& nodeHash) = 0;
};

} // sdchain

#endif
