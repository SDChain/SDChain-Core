//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2016 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_MISC_DETAIL_WORKBASE_H_INCLUDED
#define SDCHAIN_APP_MISC_DETAIL_WORKBASE_H_INCLUDED

#include <sdchain/app/misc/detail/Work.h>
#include <sdchain/protocol/BuildInfo.h>
#include <beast/core/multi_buffer.hpp>
#include <beast/http/empty_body.hpp>
#include <beast/http/read.hpp>
#include <beast/http/write.hpp>
#include <boost/asio.hpp>

namespace sdchain {

namespace detail {

template <class Impl>
class WorkBase
    : public Work
{
protected:
    using error_code = boost::system::error_code;

public:
    using callback_type =
        std::function<void(error_code const&, response_type&&)>;
protected:
    using socket_type = boost::asio::ip::tcp::socket;
    using endpoint_type = boost::asio::ip::tcp::endpoint;
    using resolver_type = boost::asio::ip::tcp::resolver;
    using query_type = resolver_type::query;
    using request_type =
        beast::http::request<beast::http::empty_body>;

    std::string host_;
    std::string path_;
    std::string port_;
    callback_type cb_;
    boost::asio::io_service& ios_;
    boost::asio::io_service::strand strand_;
    resolver_type resolver_;
    socket_type socket_;
    request_type req_;
    response_type res_;
    beast::multi_buffer read_buf_;

public:
    WorkBase(
        std::string const& host, std::string const& path,
        std::string const& port,
        boost::asio::io_service& ios, callback_type cb);
    ~WorkBase();

    Impl&
    impl()
    {
        return *static_cast<Impl*>(this);
    }

    void run() override;

    void cancel() override;

    void
    fail(error_code const& ec);

    void
    onResolve(error_code const& ec, resolver_type::iterator it);

    void
    onStart();

    void
    onRequest(error_code const& ec);

    void
    onResponse(error_code const& ec);
};

//------------------------------------------------------------------------------

template<class Impl>
WorkBase<Impl>::WorkBase(std::string const& host,
    std::string const& path, std::string const& port,
    boost::asio::io_service& ios, callback_type cb)
    : host_(host)
    , path_(path)
    , port_(port)
    , cb_(std::move(cb))
    , ios_(ios)
    , strand_(ios)
    , resolver_(ios)
    , socket_(ios)
{
}

template<class Impl>
WorkBase<Impl>::~WorkBase()
{
    if (cb_)
        cb_ (make_error_code(boost::system::errc::not_a_socket),
            std::move(res_));
}

template<class Impl>
void
WorkBase<Impl>::run()
{
    if (! strand_.running_in_this_thread())
        return ios_.post(strand_.wrap (std::bind(
            &WorkBase::run, impl().shared_from_this())));

    resolver_.async_resolve(
        query_type{host_, port_},
        strand_.wrap (std::bind(&WorkBase::onResolve, impl().shared_from_this(),
            std::placeholders::_1,
                std::placeholders::_2)));
}

template<class Impl>
void
WorkBase<Impl>::cancel()
{
    if (! strand_.running_in_this_thread())
    {
        return ios_.post(strand_.wrap (std::bind(
            &WorkBase::cancel, impl().shared_from_this())));
    }

    error_code ec;
    resolver_.cancel();
    socket_.cancel (ec);
}

template<class Impl>
void
WorkBase<Impl>::fail(error_code const& ec)
{
    if (cb_)
    {
        cb_(ec, std::move(res_));
        cb_ = nullptr;
    }
}

template<class Impl>
void
WorkBase<Impl>::onResolve(error_code const& ec, resolver_type::iterator it)
{
    if (ec)
        return fail(ec);

    socket_.async_connect(*it,
        strand_.wrap (std::bind(&Impl::onConnect, impl().shared_from_this(),
            std::placeholders::_1)));
}

template<class Impl>
void
WorkBase<Impl>::onStart()
{
    req_.method(beast::http::verb::get);
    req_.target(path_.empty() ? "/" : path_);
    req_.version = 11;
    req_.set (
        "Host", host_ + ":" + port_);
    req_.set ("User-Agent", BuildInfo::getFullVersionString());
    req_.prepare_payload();
    beast::http::async_write(impl().stream(), req_,
        strand_.wrap (std::bind (&WorkBase::onRequest,
            impl().shared_from_this(), std::placeholders::_1)));
}

template<class Impl>
void
WorkBase<Impl>::onRequest(error_code const& ec)
{
    if (ec)
        return fail(ec);

    beast::http::async_read (impl().stream(), read_buf_, res_,
        strand_.wrap (std::bind (&WorkBase::onResponse,
            impl().shared_from_this(), std::placeholders::_1)));
}

template<class Impl>
void
WorkBase<Impl>::onResponse(error_code const& ec)
{
    if (ec)
        return fail(ec);

    assert(cb_);
    cb_(ec, std::move(res_));
    cb_ = nullptr;
}

} // detail

} // sdchain

#endif
