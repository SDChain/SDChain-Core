//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_BASICS_MAKE_SSLCONTEXT_H_INCLUDED
#define SDCHAIN_BASICS_MAKE_SSLCONTEXT_H_INCLUDED

#include <boost/asio/ssl/context.hpp>
#include <string>

namespace sdchain {

/** Create a self-signed SSL context that allows anonymous Diffie Hellman. */
std::shared_ptr<boost::asio::ssl::context>
make_SSLContext(
    std::string const& cipherList);

/** Create an authenticated SSL context using the specified files. */
std::shared_ptr<boost::asio::ssl::context>
make_SSLContextAuthed (
    std::string const& keyFile,
    std::string const& certFile,
    std::string const& chainFile,
    std::string const& cipherList);


}

#endif
