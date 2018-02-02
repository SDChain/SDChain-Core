//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#include <sdchain/beast/net/IPAddressConversion.h>

namespace beast {
namespace IP {

Endpoint from_asio (boost::asio::ip::address const& address)
{
    if (address.is_v4 ())
    {
        boost::asio::ip::address_v4::bytes_type const bytes (
            address.to_v4().to_bytes());
        return Endpoint (AddressV4 (
            bytes [0], bytes [1], bytes [2], bytes [3]));
    }

    // VFALCO TODO IPv6 support
    assert(false);
    return Endpoint();
}

Endpoint from_asio (boost::asio::ip::tcp::endpoint const& endpoint)
{
    return from_asio (endpoint.address()).at_port (endpoint.port());
}

boost::asio::ip::address to_asio_address (Endpoint const& endpoint)
{
    if (endpoint.address().is_v4())
    {
        return boost::asio::ip::address (
            boost::asio::ip::address_v4 (
                endpoint.address().to_v4().value));
    }

    // VFALCO TODO IPv6 support
    assert(false);
    return boost::asio::ip::address (
        boost::asio::ip::address_v6 ());
}

boost::asio::ip::tcp::endpoint to_asio_endpoint (Endpoint const& endpoint)
{
    return boost::asio::ip::tcp::endpoint (
        to_asio_address (endpoint), endpoint.port());
}

}
}
