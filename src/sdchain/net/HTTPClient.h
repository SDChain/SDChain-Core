//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_NET_HTTPCLIENT_H_INCLUDED
#define SDCHAIN_NET_HTTPCLIENT_H_INCLUDED

#include <sdchain/core/Config.h>
#include <boost/asio/io_service.hpp>
#include <boost/asio/streambuf.hpp>
#include <chrono>

namespace sdchain {

/** Provides an asynchronous HTTP client implementation with optional SSL.
*/
class HTTPClient
{
public:
    enum
    {
        maxClientHeaderBytes = 32 * 1024
    };

    static void initializeSSLContext (Config const& config, beast::Journal j);

    static void get (
        bool bSSL,
        boost::asio::io_service& io_service,
        std::deque <std::string> deqSites,
        const unsigned short port,
        std::string const& strPath,
        std::size_t responseMax,    // if no Content-Length header
        std::chrono::seconds timeout,
        std::function <bool (const boost::system::error_code& ecResult, int iStatus, std::string const& strData)> complete,
        beast::Journal& j);

    static void get (
        bool bSSL,
        boost::asio::io_service& io_service,
        std::string strSite,
        const unsigned short port,
        std::string const& strPath,
        std::size_t responseMax,    // if no Content-Length header
        std::chrono::seconds timeout,
        std::function <bool (const boost::system::error_code& ecResult, int iStatus, std::string const& strData)> complete,
        beast::Journal& j);

    static void request (
        bool bSSL,
        boost::asio::io_service& io_service,
        std::string strSite,
        const unsigned short port,
        std::function <void (boost::asio::streambuf& sb, std::string const& strHost)> build,
        std::size_t responseMax,    // if no Content-Length header
        std::chrono::seconds timeout,
        std::function <bool (const boost::system::error_code& ecResult, int iStatus, std::string const& strData)> complete,
        beast::Journal& j);
};

} // sdchain

#endif
