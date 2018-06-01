//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_PROTOCOL_H_INCLUDED
#define SDCHAIN_PROTOCOL_PROTOCOL_H_INCLUDED

#include <sdchain/basics/base_uint.h>
#include <cstdint>

namespace sdchain {

/** Protocol specific constants, types, and data.

    This information is, implicitly, part of the SDChain
    protocol.

    @note Changing these values without adding code to the
          server to detect "pre-change" and "post-change"
          will result in a hard fork.
*/
/** Smallest legal byte size of a transaction. */
std::size_t constexpr txMinSizeBytes = 32;

/** Largest legal byte size of a transaction. */
std::size_t constexpr txMaxSizeBytes = 1024 * 1024;

/** The maximum number of unfunded offers to delete at once */
std::size_t constexpr unfundedOfferRemoveLimit = 1000;

/** The maximum number of metadata entries allowed in one transaction */
std::size_t constexpr oversizeMetaDataCap = 5200;

/** The maximum number of entries per directory page */
std::size_t constexpr dirNodeMaxEntries = 32;

/** The maximum number of pages allowed in a directory */
std::uint64_t constexpr dirNodeMaxPages = 262144;

/** A ledger index. */
using LedgerIndex = std::uint32_t;

/** A transaction identifier.
    The value is computed as the hash of the
    canonicalized, serialized transaction object.
*/
using TxID = uint256;

using TxSeq = std::uint32_t;

} // sdchain

#endif
