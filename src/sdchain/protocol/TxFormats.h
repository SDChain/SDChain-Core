//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_TXFORMATS_H_INCLUDED
#define SDCHAIN_PROTOCOL_TXFORMATS_H_INCLUDED

#include <sdchain/protocol/KnownFormats.h>

namespace sdchain {

/** Transaction type identifiers.

    These are part of the binary message format.

    @ingroup protocol
*/
enum TxType
{
    ttINVALID           = -1,

    ttPAYMENT           = 0,
    ttESCROW_CREATE     = 1,
    ttESCROW_FINISH     = 2,
    ttACCOUNT_SET       = 3,
    ttESCROW_CANCEL     = 4,
    ttREGULAR_KEY_SET   = 5,
    ttNICKNAME_SET      = 6, // open
    ttOFFER_CREATE      = 7,
    ttOFFER_CANCEL      = 8,
    no_longer_used      = 9,
    ttTICKET_CREATE     = 10,
    ttTICKET_CANCEL     = 11,
    ttSIGNER_LIST_SET   = 12,
    ttPAYCHAN_CREATE    = 13,
    ttPAYCHAN_FUND      = 14,
    ttPAYCHAN_CLAIM     = 15,

    ttTRUST_SET         = 20,

    ttAMENDMENT         = 100,
    ttFEE               = 101,
};

/** Manages the list of known transaction formats.
*/
class TxFormats : public KnownFormats <TxType>
{
private:
    void addCommonFields (Item& item);

public:
    /** Create the object.
        This will load the object will all the known transaction formats.
    */
    TxFormats ();

    static TxFormats const& getInstance ();
};

} // sdchain

#endif
