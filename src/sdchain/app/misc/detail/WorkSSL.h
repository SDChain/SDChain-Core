//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2016 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_MISC_DETAIL_WORKSSL_H_INCLUDED
#define SDCHAIN_APP_MISC_DETAIL_WORKSSL_H_INCLUDED

#include <sdchain/app/misc/detail/WorkBase.h>
#include <sdchain/net/RegisterSSLCerts.h>
#include <sdchain/basics/contract.h>
#include <boost/asio/ssl.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>

namespace sdchain {

namespace detail {

class SSLContext : public boost::asio::ssl::context
{
public:
    SSLContext(beast::Journal j)
    : boost::asio::ssl::context(boost::asio::ssl::context::sslv23)
    {
        boost::system::error_code ec;
        registerSSLCerts(*this, ec, j);
        if (ec)
        {
            Throw<std::runtime_error> (
                boost::str (boost::format (
                    "Failed to set_default_verify_paths: %s") %
                    ec.message ()));
        }
    }
};

// Work over SSL
class WorkSSL : public WorkBase<WorkSSL>
    , public std::enable_shared_from_this<WorkSSL>
{
    friend class WorkBase<WorkSSL>;

private:
    using stream_type = boost::asio::ssl::stream<socket_type&>;

    SSLContext context_;
    stream_type stream_;

public:
    WorkSSL(
        std::string const& host,
        std::string const& path,
        std::string const& port,
        boost::asio::io_service& ios,
        beast::Journal j,
        callback_type cb);
    ~WorkSSL() = default;

private:
    stream_type&
    stream()
    {
        return stream_;
    }

    void
    onConnect(error_code const& ec);

    void
    onHandshake(error_code const& ec);

    static bool
    rfc2818_verify(
        std::string const& domain,
        bool preverified,
        boost::asio::ssl::verify_context& ctx)
    {
        return boost::asio::ssl::rfc2818_verification(domain)(preverified, ctx);
    }
};

//------------------------------------------------------------------------------

WorkSSL::WorkSSL(
    std::string const& host,
    std::string const& path,
    std::string const& port,
    boost::asio::io_service& ios,
    beast::Journal j,
    callback_type cb)
    : WorkBase(host, path, port, ios, cb)
    , context_(j)
    , stream_(socket_, context_)
{
    // Set SNI hostname
    SSL_set_tlsext_host_name(stream_.native_handle(), host.c_str());
    stream_.set_verify_mode (boost::asio::ssl::verify_peer);
    stream_.set_verify_callback(    std::bind (
            &WorkSSL::rfc2818_verify, host_,
            std::placeholders::_1, std::placeholders::_2));
}

void
WorkSSL::onConnect(error_code const& ec)
{
    if (ec)
        return fail(ec);

    stream_.async_handshake(
        boost::asio::ssl::stream_base::client,
        strand_.wrap (boost::bind(&WorkSSL::onHandshake, shared_from_this(),
            boost::asio::placeholders::error)));
}

void
WorkSSL::onHandshake(error_code const& ec)
{
    if (ec)
        return fail(ec);

    onStart ();
}

} // detail

} // sdchain

#endif
