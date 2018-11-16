//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/app/misc/LoadFeeTrack.h>
#include <sdchain/core/TimeKeeper.h>
#include <sdchain/overlay/Cluster.h>
#include <sdchain/overlay/Overlay.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/rpc/Context.h>
#include <sdchain/basics/make_lock.h>

namespace sdchain {

Json::Value doPeers (RPC::Context& context)
{
    Json::Value jvResult (Json::objectValue);

    {
        auto lock = make_lock(context.app.getMasterMutex());

        jvResult[jss::peers] = context.app.overlay ().json ();

        auto const now = context.app.timeKeeper().now();
        auto const self = context.app.nodeIdentity().first;

        Json::Value& cluster = (jvResult[jss::cluster] = Json::objectValue);
        std::uint32_t ref = context.app.getFeeTrack().getLoadBase();

        context.app.cluster().for_each ([&cluster, now, ref, &self]
            (ClusterNode const& node)
            {
                if (node.identity() == self)
                    return;

                Json::Value& json = cluster[
                    toBase58(
                        TokenType::TOKEN_NODE_PUBLIC,
                        node.identity())];

                if (!node.name().empty())
                    json[jss::tag] = node.name();

                if ((node.getLoadFee() != ref) && (node.getLoadFee() != 0))
                    json[jss::fee] = static_cast<double>(node.getLoadFee()) / ref;

                if (node.getReportTime() != NetClock::time_point{})
                    json[jss::age] = (node.getReportTime() >= now)
                        ? 0
                        : (now - node.getReportTime()).count();
            });
    }

    return jvResult;
}

} // sdchain
