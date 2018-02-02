//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_SERVER_BASEPEER_H_INCLUDED
#define SDCHAIN_SERVER_BASEPEER_H_INCLUDED

#include <sdchain/server/Port.h>
#include <sdchain/server/impl/io_list.h>
#include <sdchain/beast/utility/WrappedSink.h>
#include <boost/asio.hpp>
#include <atomic>
#include <cassert>
#include <functional>
#include <string>

namespace sdchain {

// Common part of all peers
template<class Handler, class Impl>
class BasePeer
    : public io_list::work
{
protected:
    using clock_type = std::chrono::system_clock;
    using error_code = boost::system::error_code;
    using endpoint_type = boost::asio::ip::tcp::endpoint;
    using waitable_timer = boost::asio::basic_waitable_timer <clock_type>;

    Port const& port_;
    Handler& handler_;
    endpoint_type remote_address_;
    beast::WrappedSink sink_;
    beast::Journal j_;

    boost::asio::io_service::work work_;
    boost::asio::io_service::strand strand_;

public:
    BasePeer(Port const& port, Handler& handler,
        endpoint_type remote_address,
            boost::asio::io_service& io_service,
                beast::Journal journal);

    void
    close() override;

private:
    Impl&
    impl()
    {
        return *static_cast<Impl*>(this);
    }
};

//------------------------------------------------------------------------------

template<class Handler, class Impl>
BasePeer<Handler, Impl>::
BasePeer(Port const& port, Handler& handler,
    endpoint_type remote_address,
        boost::asio::io_service& io_service,
            beast::Journal journal)
    : port_(port)
    , handler_(handler)
    , remote_address_(remote_address)
    , sink_(journal.sink(),
        []
        {
            static std::atomic<unsigned> id{0};
            return "##" + std::to_string(++id) + " ";
        }())
    , j_(sink_)
    , work_(io_service)
    , strand_(io_service)
{
}

template<class Handler, class Impl>
void
BasePeer<Handler, Impl>::
close()
{
    if (! strand_.running_in_this_thread())
        return strand_.post(std::bind(
            &BasePeer::close, impl().shared_from_this()));
    error_code ec;
    impl().ws_.lowest_layer().close(ec);
}

} // sdchain

#endif
