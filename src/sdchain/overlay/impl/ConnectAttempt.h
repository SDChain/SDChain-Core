//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_OVERLAY_CONNECTATTEMPT_H_INCLUDED
#define SDCHAIN_OVERLAY_CONNECTATTEMPT_H_INCLUDED

#include "sdchain.pb.h"
#include <sdchain/overlay/impl/OverlayImpl.h>
#include <sdchain/overlay/impl/Tuning.h>

namespace sdchain {

/** Manages an outbound connection attempt. */
class ConnectAttempt
    : public OverlayImpl::Child
    , public std::enable_shared_from_this<ConnectAttempt>
{
private:
    using error_code = boost::system::error_code;

    using endpoint_type = boost::asio::ip::tcp::endpoint;

    using request_type =
        beast::http::request<beast::http::empty_body>;

    using response_type =
        beast::http::response<beast::http::dynamic_body>;

    Application& app_;
    std::uint32_t const id_;
    beast::WrappedSink sink_;
    beast::Journal journal_;
    endpoint_type remote_endpoint_;
    Resource::Consumer usage_;
    boost::asio::io_service::strand strand_;
    boost::asio::basic_waitable_timer<std::chrono::steady_clock> timer_;
    std::unique_ptr<beast::asio::ssl_bundle> ssl_bundle_;
    beast::asio::ssl_bundle::socket_type& socket_;
    beast::asio::ssl_bundle::stream_type& stream_;
    beast::multi_buffer read_buf_;
    response_type response_;
    PeerFinder::Slot::ptr slot_;
    request_type req_;

public:
    ConnectAttempt (Application& app, boost::asio::io_service& io_service,
        endpoint_type const& remote_endpoint, Resource::Consumer usage,
            beast::asio::ssl_bundle::shared_context const& context,
                std::uint32_t id, PeerFinder::Slot::ptr const& slot,
                    beast::Journal journal, OverlayImpl& overlay);

    ~ConnectAttempt();

    void
    stop() override;

    void
    run();

private:

    void close();
    void fail (std::string const& reason);
    void fail (std::string const& name, error_code ec);
    void setTimer();
    void cancelTimer();
    void onTimer (error_code ec);
    void onConnect (error_code ec);
    void onHandshake (error_code ec);
    void onWrite (error_code ec);
    void onRead (error_code ec);
    void onShutdown (error_code ec);

    static
    request_type
    makeRequest (bool crawl,
        boost::asio::ip::address const& remote_address);

    void processResponse();

    template <class = void>
    static
    boost::asio::ip::tcp::endpoint
    parse_endpoint (std::string const& s, boost::system::error_code& ec)
    {
        beast::IP::Endpoint bep;
        std::istringstream is(s);
        is >> bep;
        if (is.fail())
        {
            ec = boost::system::errc::make_error_code(
                boost::system::errc::invalid_argument);
            return boost::asio::ip::tcp::endpoint{};
        }

        return beast::IPAddressConversion::to_asio_endpoint(bep);
    }
};

}

#endif
