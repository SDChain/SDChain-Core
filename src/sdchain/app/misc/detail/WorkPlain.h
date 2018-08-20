//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2016 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_MISC_DETAIL_WORKPLAIN_H_INCLUDED
#define SDCHAIN_APP_MISC_DETAIL_WORKPLAIN_H_INCLUDED

#include <sdchain/app/misc/detail/WorkBase.h>

namespace sdchain {

namespace detail {

// Work over TCP/IP
class WorkPlain : public WorkBase<WorkPlain>
    , public std::enable_shared_from_this<WorkPlain>
{
    friend class WorkBase<WorkPlain>;

public:
    WorkPlain(
        std::string const& host,
        std::string const& path, std::string const& port,
        boost::asio::io_service& ios, callback_type cb);
    ~WorkPlain() = default;

private:
    void
    onConnect(error_code const& ec);

    socket_type&
    stream()
    {
        return socket_;
    }
};

//------------------------------------------------------------------------------

WorkPlain::WorkPlain(
    std::string const& host,
    std::string const& path, std::string const& port,
    boost::asio::io_service& ios, callback_type cb)
    : WorkBase (host, path, port, ios, cb)
{
}

void
WorkPlain::onConnect(error_code const& ec)
{
    if (ec)
        return fail(ec);

    onStart ();
}

} // detail

} // sdchain

#endif
