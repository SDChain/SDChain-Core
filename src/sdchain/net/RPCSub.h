//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_NET_RPCSUB_H_INCLUDED
#define SDCHAIN_NET_RPCSUB_H_INCLUDED

#include <sdchain/core/JobQueue.h>
#include <sdchain/net/InfoSub.h>
#include <sdchain/core/Stoppable.h>
#include <boost/asio/io_service.hpp>

namespace sdchain {

/** Subscription object for JSON RPC. */
class RPCSub : public InfoSub
{
public:
    virtual void setUsername (std::string const& strUsername) = 0;
    virtual void setPassword (std::string const& strPassword) = 0;

protected:
    explicit RPCSub (InfoSub::Source& source);
};

// VFALCO Why is the io_service needed?
std::shared_ptr<RPCSub> make_RPCSub (
    InfoSub::Source& source, boost::asio::io_service& io_service,
    JobQueue& jobQueue, std::string const& strUrl,
    std::string const& strUsername, std::string const& strPassword,
    Logs& logs);

} // sdchain

#endif
