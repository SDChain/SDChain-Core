//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/rpc/Role.h>

namespace sdchain {

bool
passwordUnrequiredOrSentCorrect (Port const& port,
                                 Json::Value const& params) {

    assert(! port.admin_ip.empty ());
    bool const passwordRequired = (!port.admin_user.empty() ||
                                   !port.admin_password.empty());

    return !passwordRequired  ||
            ((params["admin_password"].isString() &&
              params["admin_password"].asString() == port.admin_password) &&
             (params["admin_user"].isString() &&
              params["admin_user"].asString() == port.admin_user));
}

bool
ipAllowed (beast::IP::Address const& remoteIp,
           std::vector<beast::IP::Address> const& adminIp)
{
    return std::find_if (adminIp.begin (), adminIp.end (),
        [&remoteIp](beast::IP::Address const& ip) { return ip.is_any () ||
            ip == remoteIp; }) != adminIp.end ();
}

bool
isAdmin (Port const& port, Json::Value const& params,
         beast::IP::Address const& remoteIp)
{
    return ipAllowed (remoteIp, port.admin_ip) &&
        passwordUnrequiredOrSentCorrect (port, params);
}

Role
requestRole (Role const& required, Port const& port,
             Json::Value const& params, beast::IP::Endpoint const& remoteIp,
             std::string const& user)
{
    if (isAdmin(port, params, remoteIp.address()))
        return Role::ADMIN;

    if (required == Role::ADMIN)
        return Role::FORBID;

    if (isIdentified(port, remoteIp.address(), user))
        return Role::IDENTIFIED;

    return Role::GUEST;
}

/**
 * ADMIN and IDENTIFIED roles shall have unlimited resources.
 */
bool
isUnlimited (Role const& required, Port const& port,
    Json::Value const&params, beast::IP::Endpoint const& remoteIp,
    std::string const& user)
{
    Role role = requestRole(required, port, params, remoteIp, user);

    if (role == Role::ADMIN || role == Role::IDENTIFIED)
        return true;
    else
        return false;
}

bool
isUnlimited (Role const& role)
{
    return role == Role::ADMIN || role == Role::IDENTIFIED;
}

Resource::Consumer
requestInboundEndpoint (Resource::Manager& manager,
    beast::IP::Endpoint const& remoteAddress,
        Port const& port, std::string const& user)
{
    if (isUnlimited (Role::GUEST, port, Json::Value(), remoteAddress, user))
        return manager.newUnlimitedEndpoint (to_string (remoteAddress));

    return manager.newInboundEndpoint(remoteAddress);
}

bool
isIdentified (Port const& port, beast::IP::Address const& remoteIp,
        std::string const& user)
{
    return ! user.empty() && ipAllowed (remoteIp, port.secure_gateway_ip);
}

}
