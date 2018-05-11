//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_RESOURCE_KIND_H_INCLUDED
#define SDCHAIN_RESOURCE_KIND_H_INCLUDED

namespace sdchain {
namespace Resource {

/**
 * Kind of consumer.
 * kindInbound:   Inbound connection.
 * kindOutbound:  Outbound connection.
 * kindUnlimited: Inbound connection with no resource limits, but could be
 *                subjected to administrative restrictions, such as
 *                use of some RPC commands like "stop".
 */
enum Kind
{
     kindInbound
    ,kindOutbound
    ,kindUnlimited
};

}
}

#endif
