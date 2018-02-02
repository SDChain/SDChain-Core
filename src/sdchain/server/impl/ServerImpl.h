//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_SERVER_SERVERIMPL_H_INCLUDED
#define SDCHAIN_SERVER_SERVERIMPL_H_INCLUDED

#include <sdchain/basics/chrono.h>
#include <sdchain/server/Server.h>
#include <sdchain/server/impl/Door.h>
#include <sdchain/server/impl/io_list.h>
#include <sdchain/beast/core/List.h>
#include <boost/asio.hpp>
#include <boost/optional.hpp>
#include <array>
#include <chrono>
#include <mutex>

namespace sdchain {

using Endpoints = std::vector<boost::asio::ip::tcp::endpoint>;

/** A multi-protocol server.

    This server maintains multiple configured listening ports,
    with each listening port allows for multiple protocols including
    HTTP, HTTP/S, WebSocket, Secure WebSocket, and the Peer protocol.
*/
class Server
{
public:
    /** Destroy the server.
        The server is closed if it is not already closed. This call
        blocks until the server has stopped.
    */
    virtual
    ~Server() = default;

    /** Returns the Journal associated with the server. */
    virtual
    beast::Journal
    journal() = 0;

    /** Set the listening port settings.
        This may only be called once.
    */
    virtual
    Endpoints
    ports (std::vector<Port> const& v) = 0;

    /** Close the server.
        The close is performed asynchronously. The handler will be notified
        when the server has stopped. The server is considered stopped when
        there are no pending I/O completion handlers and all connections
        have closed.
        Thread safety:
            Safe to call concurrently from any thread.
    */
    virtual
    void
    close() = 0;
};

template<class Handler>
class ServerImpl : public Server
{
private:
    using clock_type = std::chrono::system_clock;

    enum
    {
        historySize = 100
    };

    Handler& handler_;
    beast::Journal j_;
    boost::asio::io_service& io_service_;
    boost::asio::io_service::strand strand_;
    boost::optional <boost::asio::io_service::work> work_;

    std::mutex m_;
    std::vector<Port> ports_;
    std::vector<std::weak_ptr<Door<Handler>>> list_;
    int high_ = 0;
    std::array <std::size_t, 64> hist_;

    io_list ios_;

public:
    ServerImpl(Handler& handler,
        boost::asio::io_service& io_service, beast::Journal journal);

    ~ServerImpl();

    beast::Journal
    journal() override
    {
        return j_;
    }

    Endpoints
    ports (std::vector<Port> const& ports) override;

    void
    close() override;

    io_list&
    ios()
    {
        return ios_;
    }

    boost::asio::io_service&
    get_io_service()
    {
        return io_service_;
    }

    bool
    closed();

private:
    static
    int
    ceil_log2 (unsigned long long x);
};

template<class Handler>
ServerImpl<Handler>::
ServerImpl(Handler& handler,
        boost::asio::io_service& io_service, beast::Journal journal)
    : handler_(handler)
    , j_(journal)
    , io_service_(io_service)
    , strand_(io_service_)
    , work_(io_service_)
{
}

template<class Handler>
ServerImpl<Handler>::
~ServerImpl()
{
    // Handler::onStopped will not be called
    work_ = boost::none;
    ios_.close();
    ios_.join();
}

template<class Handler>
Endpoints
ServerImpl<Handler>::
ports (std::vector<Port> const& ports)
{
    if (closed())
        Throw<std::logic_error> ("ports() on closed Server");
    ports_.reserve(ports.size());
    Endpoints eps;
    eps.reserve(ports.size());
    for(auto const& port : ports)
    {
        ports_.push_back(port);
        if(auto sp = ios_.emplace<Door<Handler>>(handler_,
            io_service_, ports_.back(), j_))
        {
            list_.push_back(sp);
            eps.push_back(sp->get_endpoint());
            sp->run();
        }
    }
    return eps;
}

template<class Handler>
void
ServerImpl<Handler>::
close()
{
    ios_.close(
    [&]
    {
        work_ = boost::none;
        handler_.onStopped(*this);
    });
}

template<class Handler>
bool
ServerImpl<Handler>::
closed()
{
    return ios_.closed();
}
} // sdchain

#endif
