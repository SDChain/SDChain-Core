//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_SERVER_SSLWSPEER_H_INCLUDED
#define SDCHAIN_SERVER_SSLWSPEER_H_INCLUDED

#include <sdchain/server/impl/BaseHTTPPeer.h>
#include <sdchain/server/WSSession.h>
#include <sdchain/beast/asio/ssl_bundle.h>
#include <beast/websocket/ssl.hpp>
#include <memory>

namespace sdchain {

template<class Handler>
class SSLWSPeer
    : public BaseWSPeer<Handler, SSLWSPeer<Handler>>
    , public std::enable_shared_from_this<SSLWSPeer<Handler>>
{
    friend class BasePeer<Handler, SSLWSPeer>;
    friend class BaseWSPeer<Handler, SSLWSPeer>;

    using clock_type = std::chrono::system_clock;
    using error_code = boost::system::error_code;
    using endpoint_type = boost::asio::ip::tcp::endpoint;
    using waitable_timer =
        boost::asio::basic_waitable_timer <clock_type>;

    std::unique_ptr<beast::asio::ssl_bundle> ssl_bundle_;
    beast::websocket::stream<
        beast::asio::ssl_bundle::stream_type&> ws_;

public:
    template<class Body, class Headers>
    SSLWSPeer(
        Port const& port,
        Handler& handler,
        endpoint_type remote_endpoint,
        beast::http::request<Body, Headers>&& request,
        std::unique_ptr<
            beast::asio::ssl_bundle>&& ssl_bundle,
        beast::Journal journal);
};

//------------------------------------------------------------------------------

template<class Handler>
template<class Body, class Headers>
SSLWSPeer<Handler>::
SSLWSPeer(
    Port const& port,
    Handler& handler,
    endpoint_type remote_endpoint,
    beast::http::request<Body, Headers>&& request,
    std::unique_ptr<
        beast::asio::ssl_bundle>&& ssl_bundle,
    beast::Journal journal)
    : BaseWSPeer<Handler, SSLWSPeer>(port, handler,
        remote_endpoint, std::move(request),
            ssl_bundle->socket.get_io_service(), journal)
    , ssl_bundle_(std::move(ssl_bundle))
    , ws_(ssl_bundle_->stream)
{
}

} // sdchain

#endif
