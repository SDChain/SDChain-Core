//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_RPC_SERVERHANDLERIMP_H_INCLUDED
#define SDCHAIN_RPC_SERVERHANDLERIMP_H_INCLUDED

#include <sdchain/core/JobQueue.h>
#include <sdchain/rpc/impl/WSInfoSub.h>
#include <sdchain/server/Server.h>
#include <sdchain/server/Session.h>
#include <sdchain/server/WSSession.h>
#include <sdchain/rpc/RPCHandler.h>
#include <sdchain/app/main/CollectorManager.h>
#include <sdchain/json/Output.h>
#include <map>
#include <mutex>
#include <vector>

namespace sdchain {

inline
bool operator< (Port const& lhs, Port const& rhs)
{
    return lhs.name < rhs.name;
}

class ServerHandlerImp
    : public Stoppable
{
public:
    struct Setup
    {
        std::vector<Port> ports;

        // Memberspace
        struct client_t
        {
            bool secure = false;
            std::string ip;
            std::uint16_t port = 0;
            std::string user;
            std::string password;
            std::string admin_user;
            std::string admin_password;
        };

        // Configuration when acting in client role
        client_t client;

        // Configuration for the Overlay
        struct overlay_t
        {
            boost::asio::ip::address ip;
            std::uint16_t port = 0;
        };

        overlay_t overlay;

        void
        makeContexts();
    };

private:

    Application& app_;
    Resource::Manager& m_resourceManager;
    beast::Journal m_journal;
    NetworkOPs& m_networkOPs;
    std::unique_ptr<Server> m_server;
    Setup setup_;
    JobQueue& m_jobQueue;
    beast::insight::Counter rpc_requests_;
    beast::insight::Event rpc_size_;
    beast::insight::Event rpc_time_;
    std::mutex countlock_;
    std::map<std::reference_wrapper<Port const>, int> count_;

public:
    ServerHandlerImp (Application& app, Stoppable& parent,
        boost::asio::io_service& io_service, JobQueue& jobQueue,
            NetworkOPs& networkOPs, Resource::Manager& resourceManager,
                CollectorManager& cm);

    ~ServerHandlerImp();

    using Output = Json::Output;

    void
    setup (Setup const& setup, beast::Journal journal);

    Setup const&
    setup() const
    {
        return setup_;
    }

    //
    // Stoppable
    //

    void
    onStop();

    //
    // Handler
    //

    bool
    onAccept (Session& session,
        boost::asio::ip::tcp::endpoint endpoint);

    Handoff
    onHandoff (Session& session,
        std::unique_ptr <beast::asio::ssl_bundle>&& bundle,
            http_request_type&& request,
                boost::asio::ip::tcp::endpoint remote_address);

    Handoff
    onHandoff (Session& session, boost::asio::ip::tcp::socket&& socket,
        http_request_type&& request,
            boost::asio::ip::tcp::endpoint remote_address);
    void
    onRequest (Session& session);

    void
    onWSMessage(std::shared_ptr<WSSession> session,
        std::vector<boost::asio::const_buffer> const& buffers);

    void
    onClose (Session& session,
        boost::system::error_code const&);

    void
    onStopped (Server&);

private:
    Json::Value
    processSession(
        std::shared_ptr<WSSession> const& session,
            std::shared_ptr<JobQueue::Coro> const& coro,
                Json::Value const& jv);

    void
    processSession (std::shared_ptr<Session> const&,
        std::shared_ptr<JobQueue::Coro> coro);

    void
    processRequest (Port const& port, std::string const& request,
        beast::IP::Endpoint const& remoteIPAddress, Output&&,
        std::shared_ptr<JobQueue::Coro> coro,
        std::string forwardedFor, std::string user);

    Handoff
    statusResponse(http_request_type const& request) const;


};

}

#endif
