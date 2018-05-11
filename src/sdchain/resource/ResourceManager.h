//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_RESOURCE_MANAGER_H_INCLUDED
#define SDCHAIN_RESOURCE_MANAGER_H_INCLUDED

#include <sdchain/json/json_value.h>
#include <sdchain/resource/Consumer.h>
#include <sdchain/resource/Gossip.h>
#include <sdchain/beast/insight/Collector.h>
#include <sdchain/beast/net/IPEndpoint.h>
#include <sdchain/beast/utility/Journal.h>
#include <sdchain/beast/utility/PropertyStream.h>

namespace sdchain {
namespace Resource {

/** Tracks load and resource consumption. */
class Manager : public beast::PropertyStream::Source
{
protected:
    Manager ();

public:
    virtual ~Manager() = 0;

    /** Create a new endpoint keyed by inbound IP address. */
    virtual Consumer newInboundEndpoint (beast::IP::Endpoint const& address) = 0;

    /** Create a new endpoint keyed by outbound IP address and port. */
    virtual Consumer newOutboundEndpoint (beast::IP::Endpoint const& address) = 0;

    /** Create a new endpoint keyed by name. */
    virtual Consumer newUnlimitedEndpoint (std::string const& name) = 0;

    /** Extract packaged consumer information for export. */
    virtual Gossip exportConsumers () = 0;

    /** Extract consumer information for reporting. */
    virtual Json::Value getJson () = 0;
    virtual Json::Value getJson (int threshold) = 0;

    /** Import packaged consumer information.
        @param origin An identifier that unique labels the origin.
    */
    virtual void importConsumers (std::string const& origin, Gossip const& gossip) = 0;
};

//------------------------------------------------------------------------------

std::unique_ptr <Manager> make_Manager (
    beast::insight::Collector::ptr const& collector,
        beast::Journal journal);

}
}

#endif
