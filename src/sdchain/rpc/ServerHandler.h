//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_RPC_SERVERHANDLER_H_INCLUDED
#define SDCHAIN_RPC_SERVERHANDLER_H_INCLUDED

#include <sdchain/basics/BasicConfig.h>
#include <sdchain/core/Config.h>
#include <sdchain/core/JobQueue.h>
#include <sdchain/core/Stoppable.h>
#include <sdchain/server/Port.h>
#include <sdchain/resource/ResourceManager.h>
#include <sdchain/rpc/impl/ServerHandlerImp.h>
#include <sdchain/beast/utility/Journal.h>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/address.hpp>
#include <memory>
#include <vector>

namespace sdchain {

using ServerHandler = ServerHandlerImp;

ServerHandler::Setup
setup_ServerHandler (
    Config const& c,
    std::ostream&& log);

std::unique_ptr <ServerHandler>
make_ServerHandler (Application& app, Stoppable& parent, boost::asio::io_service&,
    JobQueue&, NetworkOPs&, Resource::Manager&,
        CollectorManager& cm);

} // sdchain

#endif
