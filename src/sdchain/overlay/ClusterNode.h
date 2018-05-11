//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_PEERS_CLUSTERNODESTATUS_H_INCLUDED
#define SDCHAIN_APP_PEERS_CLUSTERNODESTATUS_H_INCLUDED

#include <sdchain/basics/chrono.h>
#include <sdchain/protocol/PublicKey.h>
#include <cstdint>
#include <string>

namespace sdchain {

class ClusterNode
{
public:
    ClusterNode() = delete;

    ClusterNode(
            PublicKey const& identity,
            std::string const& name,
            std::uint32_t fee = 0,
            NetClock::time_point rtime = NetClock::time_point{})
        : identity_ (identity)
        , name_(name)
        , mLoadFee(fee)
        , mReportTime(rtime)
    { }

    std::string const& name() const
    {
        return name_;
    }

    std::uint32_t getLoadFee() const
    {
        return mLoadFee;
    }

    NetClock::time_point getReportTime() const
    {
        return mReportTime;
    }

    PublicKey const&
    identity () const
    {
        return identity_;
    }

private:
    PublicKey const identity_;
    std::string name_;
    std::uint32_t mLoadFee = 0;
    NetClock::time_point mReportTime = {};
};

} // sdchain

#endif
