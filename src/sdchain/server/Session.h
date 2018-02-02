//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_SERVER_SESSION_H_INCLUDED
#define SDCHAIN_SERVER_SESSION_H_INCLUDED

#include <sdchain/server/Writer.h>
#include <sdchain/server/WSSession.h>
#include <beast/http/message.hpp>
#include <sdchain/beast/net/IPEndpoint.h>
#include <sdchain/beast/utility/Journal.h>
#include <functional>
#include <memory>
#include <ostream>
#include <vector>

namespace sdchain {

/** Persistent state information for a connection session.
    These values are preserved between calls for efficiency.
    Some fields are input parameters, some are output parameters,
    and all only become defined during specific callbacks.
*/
class Session
{
public:
    Session() = default;
    Session (Session const&) = delete;

    /** A user-definable pointer.
        The initial value is always zero.
        Changes to the value are persisted between calls.
    */
    void* tag = nullptr;

    /** Returns the Journal to use for logging. */
    virtual
    beast::Journal
    journal() = 0;

    /** Returns the Port settings for this connection. */
    virtual
    Port const&
    port() = 0;

    /** Returns the remote address of the connection. */
    virtual
    beast::IP::Endpoint
    remoteAddress() = 0;

    /** Returns the current HTTP request. */
    virtual
    http_request_type&
    request() = 0;

    /** Send a copy of data asynchronously. */
    /** @{ */
    void
    write (std::string const& s)
    {
        if (! s.empty())
            write (&s[0],
                std::distance (s.begin(), s.end()));
    }

    template <typename BufferSequence>
    void
    write (BufferSequence const& buffers)
    {
        for (typename BufferSequence::const_iterator iter (buffers.begin());
            iter != buffers.end(); ++iter)
        {
            typename BufferSequence::value_type const& buffer (*iter);
            write (boost::asio::buffer_cast <void const*> (buffer),
                boost::asio::buffer_size (buffer));
        }
    }

    virtual
    void
    write (void const* buffer, std::size_t bytes) = 0;

    virtual
    void
    write (std::shared_ptr <Writer> const& writer,
        bool keep_alive) = 0;

    /** @} */

    /** Detach the session.
        This holds the session open so that the response can be sent
        asynchronously. Calls to io_service::run made by the server
        will not return until all detached sessions are closed.
    */
    virtual
    std::shared_ptr<Session>
    detach() = 0;

    /** Indicate that the response is complete.
        The handler should call this when it has completed writing
        the response. If Keep-Alive is indicated on the connection,
        this will trigger a read for the next request; else, the
        connection will be closed when all remaining data has been sent.
    */
    virtual
    void
    complete() = 0;

    /** Close the session.
        This will be performed asynchronously. The session will be
        closed gracefully after all pending writes have completed.
        @param graceful `true` to wait until all data has finished sending.
    */
    virtual
    void
    close (bool graceful) = 0;

    /** Convert the connection to WebSocket. */
    virtual
    std::shared_ptr<WSSession>
    websocketUpgrade() = 0;
};

}  // sdchain

#endif
