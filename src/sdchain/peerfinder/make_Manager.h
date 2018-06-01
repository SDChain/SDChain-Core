//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PEERFINDER_MAKE_MANAGER_H_INCLUDED
#define SDCHAIN_PEERFINDER_MAKE_MANAGER_H_INCLUDED

#include <sdchain/peerfinder/PeerfinderManager.h>
#include <boost/asio/io_service.hpp>
#include <memory>

namespace sdchain {
namespace PeerFinder {

/** Create a new Manager. */
std::unique_ptr<Manager>
make_Manager (Stoppable& parent, boost::asio::io_service& io_service,
        clock_type& clock, beast::Journal journal, BasicConfig const& config);

}
}

#endif
