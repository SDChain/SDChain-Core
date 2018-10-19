//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_RPC_WSINFOSUB_H
#define SDCHAIN_RPC_WSINFOSUB_H

#include <sdchain/server/WSSession.h>
#include <sdchain/net/InfoSub.h>
#include <sdchain/beast/net/IPAddressConversion.h>
#include <sdchain/json/json_writer.h>
#include <sdchain/rpc/Role.h>
#include <memory>
#include <string>

namespace sdchain {

class WSInfoSub : public InfoSub
{
    std::weak_ptr<WSSession> ws_;
    std::string user_;
    std::string fwdfor_;

public:
    WSInfoSub(Source& source, std::shared_ptr<WSSession> const& ws)
        : InfoSub(source)
        , ws_(ws)
    {
        auto const& h = ws->request();
        auto it = h.find("X-User");
        if (it != h.end() &&
            isIdentified(
                ws->port(), beast::IPAddressConversion::from_asio(
                    ws->remote_endpoint()).address(), it->value().to_string()))
        {
            user_ = it->value().to_string();
            it = h.find("X-Forwarded-For");
            if (it != h.end())
                fwdfor_ = it->value().to_string();
        }
    }

    std::string
    user() const
    {
        return user_;
    }

    std::string
    forwarded_for() const
    {
        return fwdfor_;
    }

    void
    send(Json::Value const& jv, bool)
    {
        auto sp = ws_.lock();
        if(! sp)
            return;
        beast::multi_buffer sb;
        Json::stream(jv,
            [&](void const* data, std::size_t n)
            {
                sb.commit(boost::asio::buffer_copy(
                    sb.prepare(n), boost::asio::buffer(data, n)));
            });
        auto m = std::make_shared<
            StreambufWSMsg<decltype(sb)>>(
                std::move(sb));
        sp->send(m);
    }
};

} // sdchain

#endif
