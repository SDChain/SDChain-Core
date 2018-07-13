//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012, 2013 SDChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef SDCHAIN_PEERFINDER_PRIVATETYPES_H_INCLUDED
#define SDCHAIN_PEERFINDER_PRIVATETYPES_H_INCLUDED

namespace sdchain {
namespace PeerFinder {

/** Indicates the action the logic will take after a handshake. */
enum HandshakeAction
{
    doActivate,
    doRedirect,
    doClose
};

}
}

#endif
