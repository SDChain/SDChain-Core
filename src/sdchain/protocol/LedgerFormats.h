//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_LEDGERFORMATS_H_INCLUDED
#define SDCHAIN_PROTOCOL_LEDGERFORMATS_H_INCLUDED

#include <sdchain/protocol/KnownFormats.h>

namespace sdchain {

/** Ledger entry types.

    These are stored in serialized data.

    @note Changing these values results in a hard fork.

    @ingroup protocol
*/
// Used as the type of a transaction or the type of a ledger entry.
enum LedgerEntryType
{
    /** Special type, anything
        This is used when the type in the Keylet is unknown,
        such as when building metadata.
    */
    ltANY = -3,

    /** Special type, anything not a directory
        This is used when the type in the Keylet is unknown,
        such as when iterating
    */
    ltCHILD             = -2,

    ltINVALID           = -1,

    //---------------------------------------------------------------------------

    ltACCOUNT_ROOT      = 'a',

    /** Directory node.

        A directory is a vector 256-bit values. Usually they represent
        hashes of other objects in the ledger.

        Used in an append-only fashion.

        (There's a little more information than this, see the template)
    */
    ltDIR_NODE          = 'd',

    ltSDCHAIN_STATE      = 'r',

    ltTICKET            = 'T',

    ltSIGNER_LIST       = 'S',

    ltOFFER             = 'o',

    ltLEDGER_HASHES     = 'h',

    ltAMENDMENTS        = 'f',

    ltFEE_SETTINGS      = 's',

    ltESCROW            = 'u',

    // Simple unidirection sda channel
    ltPAYCHAN           = 'x',

    // No longer used or supported. Left here to prevent accidental
    // reassignment of the ledger type.
    ltNICKNAME          = 'n',

    ltNotUsed01         = 'c',
};

/**
    @ingroup protocol
*/
// Used as a prefix for computing ledger indexes (keys).
enum LedgerNameSpace
{
    spaceAccount        = 'a',
    spaceDirNode        = 'd',
    spaceGenerator      = 'g',
    spaceSDChain         = 'r',
    spaceOffer          = 'o',  // Entry for an offer.
    spaceOwnerDir       = 'O',  // Directory of things owned by an account.
    spaceBookDir        = 'B',  // Directory of order books.
    spaceContract       = 'c',
    spaceSkipList       = 's',
    spaceEscrow         = 'u',
    spaceAmendment      = 'f',
    spaceFee            = 'e',
    spaceTicket         = 'T',
    spaceSignerList     = 'S',
    spaceSDAUChannel    = 'x',

    // No longer used or supported. Left here to reserve the space and
    // avoid accidental reuse of the space.
    spaceNickname       = 'n',
};

/**
    @ingroup protocol
*/
enum LedgerSpecificFlags
{
    // ltACCOUNT_ROOT
    lsfPasswordSpent    = 0x00010000,   // True, if password set fee is spent.
    lsfRequireDestTag   = 0x00020000,   // True, to require a DestinationTag for payments.
    lsfRequireAuth      = 0x00040000,   // True, to require a authorization to hold IOUs.
    lsfDisallowSDA      = 0x00080000,   // True, to disallow sending SDA.
    lsfDisableMaster    = 0x00100000,   // True, force regular key
    lsfNoFreeze         = 0x00200000,   // True, cannot freeze sdchain states
    lsfGlobalFreeze     = 0x00400000,   // True, all assets frozen
    lsfDefaultSDChain    = 0x00800000,   // True, trust lines allow rippling by default

    // ltOFFER
    lsfPassive          = 0x00010000,
    lsfSell             = 0x00020000,   // True, offer was placed as a sell.

    // ltSDCHAIN_STATE
    lsfLowReserve       = 0x00010000,   // True, if entry counts toward reserve.
    lsfHighReserve      = 0x00020000,
    lsfLowAuth          = 0x00040000,
    lsfHighAuth         = 0x00080000,
    lsfLowNoSDChain      = 0x00100000,
    lsfHighNoSDChain     = 0x00200000,
    lsfLowFreeze        = 0x00400000,   // True, low side has set freeze flag
    lsfHighFreeze       = 0x00800000,   // True, high side has set freeze flag
};

//------------------------------------------------------------------------------

/** Holds the list of known ledger entry formats.
*/
class LedgerFormats : public KnownFormats <LedgerEntryType>
{
private:
    LedgerFormats ();

public:
    static LedgerFormats const& getInstance ();

private:
    void addCommonFields (Item& item);
};

} // sdchain

#endif
