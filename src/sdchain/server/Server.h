//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_SERVER_SERVER_H_INCLUDED
#define SDCHAIN_SERVER_SERVER_H_INCLUDED

#include <sdchain/server/Port.h>
#include <sdchain/server/impl/ServerImpl.h>
#include <sdchain/beast/utility/Journal.h>
#include <sdchain/beast/utility/PropertyStream.h>
#include <boost/asio/io_service.hpp>

namespace sdchain {

/** Create the HTTP server using the specified handler. */
template<class Handler>
std::unique_ptr<Server>
make_Server(Handler& handler,
    boost::asio::io_service& io_service, beast::Journal journal)
{
    return std::make_unique<ServerImpl<Handler>>(
        handler, io_service, journal);
}

} // sdchain

#endif
