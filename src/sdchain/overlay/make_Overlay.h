//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_OVERLAY_MAKE_OVERLAY_H_INCLUDED
#define SDCHAIN_OVERLAY_MAKE_OVERLAY_H_INCLUDED

#include <sdchain/rpc/ServerHandler.h>
#include <sdchain/overlay/Overlay.h>
#include <sdchain/resource/ResourceManager.h>
#include <sdchain/basics/Resolver.h>
#include <sdchain/core/Stoppable.h>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ssl/context.hpp>

namespace sdchain {

Overlay::Setup
setup_Overlay (BasicConfig const& config);

/** Creates the implementation of Overlay. */
std::unique_ptr <Overlay>
make_Overlay (
    Application& app,
    Overlay::Setup const& setup,
    Stoppable& parent,
    ServerHandler& serverHandler,
    Resource::Manager& resourceManager,
    Resolver& resolver,
    boost::asio::io_service& io_service,
    BasicConfig const& config);

} // sdchain

#endif
