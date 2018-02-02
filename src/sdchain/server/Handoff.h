//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_SERVER_HANDOFF_H_INCLUDED
#define SDCHAIN_SERVER_HANDOFF_H_INCLUDED

#include <sdchain/server/Writer.h>
#include <beast/http/message.hpp>
#include <beast/http/dynamic_body.hpp>
#include <memory>

namespace sdchain {

using http_request_type =
    beast::http::request<beast::http::dynamic_body>;

using http_response_type =
    beast::http::response<beast::http::dynamic_body>;

/** Used to indicate the result of a server connection handoff. */
struct Handoff
{
    // When `true`, the Session will close the socket. The
    // Handler may optionally take socket ownership using std::move
    bool moved = false;

    // If response is set, this determines the keep alive
    bool keep_alive = false;

    // When set, this will be sent back
    std::shared_ptr<Writer> response;

    bool handled() const
    {
        return moved || response;
    }
};

} // sdchain

#endif
