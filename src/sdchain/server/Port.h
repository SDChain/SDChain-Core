//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_SERVER_PORT_H_INCLUDED
#define SDCHAIN_SERVER_PORT_H_INCLUDED

#include <sdchain/basics/BasicConfig.h>
#include <sdchain/beast/net/IPEndpoint.h>
#include <beast/core/string.hpp>
#include <beast/websocket/option.hpp>
#include <boost/asio/ip/address.hpp>
#include <cstdint>
#include <memory>
#include <set>
#include <string>

namespace boost { namespace asio { namespace ssl { class context; } } }

namespace sdchain {

/** Configuration information for a Server listening port. */
struct Port
{
    std::string name;
    boost::asio::ip::address ip;
    std::uint16_t port = 0;
    std::set<std::string, beast::iless> protocol;
    std::vector<beast::IP::Address> admin_ip;
    std::vector<beast::IP::Address> secure_gateway_ip;
    std::string user;
    std::string password;
    std::string admin_user;
    std::string admin_password;
    std::string ssl_key;
    std::string ssl_cert;
    std::string ssl_chain;
    std::string ssl_ciphers;
    beast::websocket::permessage_deflate pmd_options;
    std::shared_ptr<boost::asio::ssl::context> context;

    // How many incoming connections are allowed on this
    // port in the range [0, 65535] where 0 means unlimited.
    int limit = 0;

    // Websocket disconnects if send queue exceeds this limit
    std::uint16_t ws_queue_limit;

    // Returns `true` if any websocket protocols are specified
    bool websockets() const;

    // Returns `true` if any secure protocols are specified
    bool secure() const;

    // Returns a string containing the list of protocols
    std::string protocols() const;
};

std::ostream&
operator<< (std::ostream& os, Port const& p);

//------------------------------------------------------------------------------

struct ParsedPort
{
    std::string name;
    std::set<std::string, beast::iless> protocol;
    std::string user;
    std::string password;
    std::string admin_user;
    std::string admin_password;
    std::string ssl_key;
    std::string ssl_cert;
    std::string ssl_chain;
    std::string ssl_ciphers;
    beast::websocket::permessage_deflate pmd_options;
    int limit = 0;
    std::uint16_t ws_queue_limit;

    boost::optional<boost::asio::ip::address> ip;
    boost::optional<std::uint16_t> port;
    boost::optional<std::vector<beast::IP::Address>> admin_ip;
    boost::optional<std::vector<beast::IP::Address>> secure_gateway_ip;
};

void
parse_Port (ParsedPort& port, Section const& section, std::ostream& log);

} // sdchain

#endif
