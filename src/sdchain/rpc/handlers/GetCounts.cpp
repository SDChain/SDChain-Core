//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/ledger/AcceptedLedger.h>
#include <sdchain/app/ledger/InboundLedgers.h>
#include <sdchain/app/ledger/LedgerMaster.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/app/misc/NetworkOPs.h>
#include <sdchain/basics/UptimeTimer.h>
#include <sdchain/core/DatabaseCon.h>
#include <sdchain/json/json_value.h>
#include <sdchain/ledger/CachedSLEs.h>
#include <sdchain/net/RPCErr.h>
#include <sdchain/nodestore/Database.h>
#include <sdchain/protocol/ErrorCodes.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/rpc/Context.h>

namespace sdchain {

static
void textTime (
    std::string& text, int& seconds, const char* unitName, int unitVal)
{
    int i = seconds / unitVal;

    if (i == 0)
        return;

    seconds -= unitVal * i;

    if (!text.empty ())
        text += ", ";

    text += std::to_string(i);
    text += " ";
    text += unitName;

    if (i > 1)
        text += "s";
}

// {
//   min_count: <number>  // optional, defaults to 10
// }
Json::Value doGetCounts (RPC::Context& context)
{
    int minCount = 10;

    if (context.params.isMember (jss::min_count))
        minCount = context.params[jss::min_count].asUInt ();

    auto objectCounts = CountedObjects::getInstance ().getCounts (minCount);

    Json::Value ret (Json::objectValue);

    for (auto const& it : objectCounts)
    {
        ret [it.first] = it.second;
    }

    int dbKB = getKBUsedAll (context.app.getLedgerDB ().getSession ());

    if (dbKB > 0)
        ret[jss::dbKBTotal] = dbKB;

    dbKB = getKBUsedDB (context.app.getLedgerDB ().getSession ());

    if (dbKB > 0)
        ret[jss::dbKBLedger] = dbKB;

    dbKB = getKBUsedDB (context.app.getTxnDB ().getSession ());

    if (dbKB > 0)
        ret[jss::dbKBTransaction] = dbKB;

    {
        std::size_t c = context.app.getOPs().getLocalTxCount ();
        if (c > 0)
            ret[jss::local_txs] = static_cast<Json::UInt> (c);
    }

    ret[jss::write_load] = context.app.getNodeStore ().getWriteLoad ();

    ret[jss::historical_perminute] = static_cast<int>(
        context.app.getInboundLedgers().fetchRate());
    ret[jss::SLE_hit_rate] = context.app.cachedSLEs().rate();
    ret[jss::node_hit_rate] = context.app.getNodeStore ().getCacheHitRate ();
    ret[jss::ledger_hit_rate] = context.app.getLedgerMaster ().getCacheHitRate ();
    ret[jss::AL_hit_rate] = context.app.getAcceptedLedgerCache ().getHitRate ();

    ret[jss::fullbelow_size] = static_cast<int>(context.app.family().fullbelow().size());
    ret[jss::treenode_cache_size] = context.app.family().treecache().getCacheSize();
    ret[jss::treenode_track_size] = context.app.family().treecache().getTrackSize();

    std::string uptime;
    int s = UptimeTimer::getInstance ().getElapsedSeconds ();
    textTime (uptime, s, "year", 365 * 24 * 60 * 60);
    textTime (uptime, s, "day", 24 * 60 * 60);
    textTime (uptime, s, "hour", 60 * 60);
    textTime (uptime, s, "minute", 60);
    textTime (uptime, s, "second", 1);
    ret[jss::uptime] = uptime;

    ret[jss::node_writes] = context.app.getNodeStore().getStoreCount();
    ret[jss::node_reads_total] = context.app.getNodeStore().getFetchTotalCount();
    ret[jss::node_reads_hit] = context.app.getNodeStore().getFetchHitCount();
    ret[jss::node_written_bytes] = context.app.getNodeStore().getStoreSize();
    ret[jss::node_read_bytes] = context.app.getNodeStore().getFetchSize();

    return ret;
}

} // sdchain
