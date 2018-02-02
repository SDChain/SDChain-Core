//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_RPC_CONTEXT_H_INCLUDED
#define SDCHAIN_RPC_CONTEXT_H_INCLUDED

#include <sdchain/core/Config.h>
#include <sdchain/core/JobQueue.h>
#include <sdchain/net/InfoSub.h>
#include <sdchain/rpc/Role.h>

#include <sdchain/beast/utility/Journal.h>

namespace sdchain {

class Application;
class NetworkOPs;
class LedgerMaster;

namespace RPC {

/** The context of information needed to call an RPC. */
struct Context
{
    /**
     * Data passed in from HTTP headers.
     */
    struct Headers
    {
        std::string user;
        std::string forwardedFor;
    };

    beast::Journal j;
    Json::Value params;
    Application& app;
    Resource::Charge& loadType;
    NetworkOPs& netOps;
    LedgerMaster& ledgerMaster;
    Resource::Consumer& consumer;
    Role role;
    std::shared_ptr<JobQueue::Coro> coro;
    InfoSub::pointer infoSub;
    Headers headers;
};

} // RPC
} // sdchain

#endif
