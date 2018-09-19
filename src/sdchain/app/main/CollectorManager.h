//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_MAIN_COLLECTORMANAGER_H_INCLUDED
#define SDCHAIN_APP_MAIN_COLLECTORMANAGER_H_INCLUDED

#include <sdchain/basics/BasicConfig.h>
#include <sdchain/beast/insight/Insight.h>

namespace sdchain {

/** Provides the beast::insight::Collector service. */
class CollectorManager
{
public:
    static std::unique_ptr<CollectorManager> New (
        Section const& params, beast::Journal journal);

    virtual ~CollectorManager () = 0;
    virtual beast::insight::Collector::ptr const& collector () = 0;
    virtual beast::insight::Group::ptr const& group (
        std::string const& name) = 0;
};

}

#endif
