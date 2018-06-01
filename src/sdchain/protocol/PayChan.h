//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_PAYCHAN_H_INCLUDED
#define SDCHAIN_PROTOCOL_PAYCHAN_H_INCLUDED

#include <sdchain/basics/base_uint.h>
#include <sdchain/protocol/HashPrefix.h>
#include <sdchain/protocol/Serializer.h>
#include <sdchain/protocol/SDAAmount.h>

namespace sdchain {

inline
void
serializePayChanAuthorization (
    Serializer& msg,
    uint256 const& key,
    SDAAmount const& amt)
{
    msg.add32 (HashPrefix::paymentChannelClaim);
    msg.add256 (key);
    msg.add64 (amt.drops ());
}

} // sdchain

#endif
