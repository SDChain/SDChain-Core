//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#ifndef BEAST_ASIO_SSL_ERROR_H_INCLUDED
#define BEAST_ASIO_SSL_ERROR_H_INCLUDED

// The first two includes must come first to work around boost 1.62 asio header bugs
#ifdef _MSC_VER
#  include <winsock2.h>
#endif
#include <boost/asio/ssl/detail/openssl_types.hpp>
#include <boost/asio.hpp>
//#include <boost/asio/error.hpp> // Causes error with WinSock.h
#include <boost/asio/ssl/error.hpp>
#include <boost/lexical_cast.hpp>

namespace beast {

/** Returns a human readable message if the error code is SSL related. */
template<class = void>
std::string
error_message_with_ssl(boost::system::error_code const& ec)
{
    std::string error;

    if (ec.category() == boost::asio::error::get_ssl_category())
    {
        error = " ("
            + boost::lexical_cast<std::string>(ERR_GET_LIB (ec.value ()))
            + ","
            + boost::lexical_cast<std::string>(ERR_GET_FUNC (ec.value ()))
            + ","
            + boost::lexical_cast<std::string>(ERR_GET_REASON (ec.value ()))
            + ") ";

        // This buffer must be at least 120 bytes, most examples use 256.
        // https://www.openssl.org/docs/crypto/ERR_error_string.html
        char buf[256];
        ::ERR_error_string_n(ec.value (), buf, sizeof(buf));
        error += buf;
    }
    else
    {
        error = ec.message();
    }

    return error;
}

/** Returns `true` if the error code is a SSL "short read." */
inline
bool
is_short_read(boost::system::error_code const& ec)
{
#ifdef SSL_R_SHORT_READ
    return (ec.category() == boost::asio::error::get_ssl_category())
        && (ERR_GET_REASON(ec.value()) == SSL_R_SHORT_READ);
#else
    return false;
#endif
}

} // beast

#endif
