//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_BASICS_RESOLVERASIO_H_INCLUDED
#define SDCHAIN_BASICS_RESOLVERASIO_H_INCLUDED

#include <sdchain/basics/Resolver.h>
#include <sdchain/beast/utility/Journal.h>
#include <boost/asio/io_service.hpp>

namespace sdchain {

class ResolverAsio : public Resolver
{
public:
    static
    std::unique_ptr<ResolverAsio> New (
        boost::asio::io_service&, beast::Journal);
};

}

#endif
