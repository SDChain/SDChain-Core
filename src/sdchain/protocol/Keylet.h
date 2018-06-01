//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_KEYLET_H_INCLUDED
#define SDCHAIN_PROTOCOL_KEYLET_H_INCLUDED

#include <sdchain/protocol/LedgerFormats.h>
#include <sdchain/basics/base_uint.h>

namespace sdchain {

class STLedgerEntry;

/** A pair of SHAMap key and LedgerEntryType.

    A Keylet identifies both a key in the state map
    and its ledger entry type.

    @note Keylet is a portmanteau of the words key
          and LET, an acronym for LedgerEntryType.
*/
struct Keylet
{
    LedgerEntryType type;
    uint256 key;

    Keylet (LedgerEntryType type_,
            uint256 const& key_)
        : type(type_)
        , key(key_)
    {
    }

    /** Returns true if the SLE matches the type */
    bool
    check (STLedgerEntry const&) const;
};

}

#endif
