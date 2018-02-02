//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_SERVER_ROLE_H_INCLUDED
#define SDCHAIN_SERVER_ROLE_H_INCLUDED

#include <sdchain/server/Port.h>
#include <sdchain/json/json_value.h>
#include <sdchain/resource/ResourceManager.h>
#include <sdchain/beast/net/IPEndpoint.h>
#include <vector>

namespace sdchain {

/** Indicates the level of administrative permission to grant.
 * IDENTIFIED role has unlimited resources but cannot perform some
 *            RPC commands.
 * ADMIN role has unlimited resources and is able to perform all RPC
 *            commands.
 */
enum class Role
{
    GUEST,
    USER,
    IDENTIFIED,
    ADMIN,
    FORBID
};

/** Return the allowed privilege role.
    jsonRPC must meet the requirements of the JSON-RPC
    specification. It must be of type Object, containing the key params
    which is an array with at least one object. Inside this object
    are the optional keys 'admin_user' and 'admin_password' used to
    validate the credentials.
*/
Role
requestRole (Role const& required, Port const& port,
    Json::Value const& jsonRPC, beast::IP::Endpoint const& remoteIp,
    std::string const& user);

Resource::Consumer
requestInboundEndpoint (Resource::Manager& manager,
    beast::IP::Endpoint const& remoteAddress,
        Port const& port, std::string const& user);

/**
 * Check if the role entitles the user to unlimited resources.
 */
bool
isUnlimited (Role const& role);

/**
 * If the HTTP header X-User exists with a non-empty value was passed by an IP
 * configured as secure_gateway, then the user can be positively identified.
 */
bool
isIdentified (Port const& port, beast::IP::Address const& remoteIp,
        std::string const& user);

} // sdchain

#endif
