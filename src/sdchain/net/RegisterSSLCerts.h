//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2016 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_NET_REGISTER_SSL_CERTS_H_INCLUDED
#define SDCHAIN_NET_REGISTER_SSL_CERTS_H_INCLUDED

#include <boost/asio/ssl/context.hpp>
#include <sdchain/basics/Log.h>

namespace sdchain {
/** Register default SSL certificates.

    Register the system default SSL root certificates. On linux/mac,
    this just calls asio's `set_default_verify_paths` to look in standard
    operating system locations. On windows, it uses the OS certificate
    store accessible via CryptoAPI.
*/
void
registerSSLCerts(
    boost::asio::ssl::context&,
    boost::system::error_code&,
    beast::Journal j);

}  // namespace sdchain

#endif
