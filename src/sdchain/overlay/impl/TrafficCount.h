//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_OVERLAY_TRAFFIC_H_INCLUDED
#define SDCHAIN_OVERLAY_TRAFFIC_H_INCLUDED

#include "sdchain.pb.h"

#include <atomic>
#include <map>

namespace sdchain {

class TrafficCount
{

public:

    using count_t = std::atomic <unsigned long>;

    class TrafficStats
    {
        public:

        count_t bytesIn;
        count_t bytesOut;
        count_t messagesIn;
        count_t messagesOut;

        TrafficStats() : bytesIn(0), bytesOut(0),
            messagesIn(0), messagesOut(0)
        { ; }

        TrafficStats(const TrafficStats& ts)
            : bytesIn (ts.bytesIn.load())
            , bytesOut (ts.bytesOut.load())
            , messagesIn (ts.messagesIn.load())
            , messagesOut (ts.messagesOut.load())
        { ; }

        operator bool () const
        {
            return messagesIn || messagesOut;
        }
    };


    enum class category
    {
        CT_base,           // basic peer overhead, must be first
        CT_overlay,        // overlay management
        CT_transaction,
        CT_proposal,
        CT_validation,
        CT_get_ledger,     // ledgers we try to get
        CT_share_ledger,   // ledgers we share
        CT_get_trans,      // transaction sets we try to get
        CT_share_trans,    // transaction sets we get
        CT_unknown         // must be last
    };

    static const char* getName (category c);

    static category categorize (
        ::google::protobuf::Message const& message,
        int type, bool inbound);

    void addCount (category cat, bool inbound, int number)
    {
        if (inbound)
        {
            counts_[cat].bytesIn += number;
            ++counts_[cat].messagesIn;
        }
        else
        {
            counts_[cat].bytesOut += number;
            ++counts_[cat].messagesOut;
        }
    }

    TrafficCount()
    {
        for (category i = category::CT_base;
            i <= category::CT_unknown;
            i = static_cast<category>(static_cast<int>(i) + 1))
        {
            counts_[i];
        }
    }

    std::map <std::string, TrafficStats>
    getCounts () const
    {
        std::map <std::string, TrafficStats> ret;

        for (auto& i : counts_)
        {
            if (i.second)
                ret.emplace (std::piecewise_construct,
                    std::forward_as_tuple (getName (i.first)),
                    std::forward_as_tuple (i.second));
        }

        return ret;
    }

    protected:

    std::map <category, TrafficStats> counts_;
};

}
#endif
