//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_NET_RPCCALL_H_INCLUDED
#define SDCHAIN_NET_RPCCALL_H_INCLUDED

#include <sdchain/core/Config.h>
#include <sdchain/json/json_value.h>
#include <boost/asio/io_service.hpp>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace sdchain {

// This a trusted interface, the user is expected to provide valid input to
// perform valid requests. Error catching and reporting is not a requirement of
// the command line interface.
//
// Improvements to be more strict and to provide better diagnostics are welcome.

/** Processes SDChain RPC calls. */
namespace RPCCall {

int fromCommandLine (
    Config const& config,
    const std::vector<std::string>& vCmd,
    Logs& logs);

void fromNetwork (
    boost::asio::io_service& io_service,
    std::string const& strIp, const std::uint16_t iPort,
    std::string const& strUsername, std::string const& strPassword,
    std::string const& strPath, std::string const& strMethod,
    Json::Value const& jvParams, const bool bSSL, bool quiet,
    Logs& logs,
    std::function<void (Json::Value const& jvInput)> callbackFuncP = std::function<void (Json::Value const& jvInput)> ());
}

/** Given a sdchaind command line, return the corresponding JSON.
*/
Json::Value
cmdLineToJSONRPC (std::vector<std::string> const& args, beast::Journal j);

/** Internal invocation of RPC client.
*/
std::pair<int, Json::Value>
rpcClient(std::vector<std::string> const& args,
    Config const& config, Logs& logs);

} // sdchain

#endif
