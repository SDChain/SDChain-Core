//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_SERVER_SIMPLEWRITER_H_INCLUDED
#define SDCHAIN_SERVER_SIMPLEWRITER_H_INCLUDED

#include <sdchain/server/Writer.h>
#include <beast/core/multi_buffer.hpp>
#include <beast/core/ostream.hpp>
#include <beast/http/message.hpp>
#include <beast/http/write.hpp>
#include <utility>

namespace sdchain {

/// Deprecated: Writer that serializes a HTTP/1 message
class SimpleWriter : public Writer
{
    beast::multi_buffer sb_;

public:
    template<bool isRequest, class Body, class Headers>
    explicit
    SimpleWriter(beast::http::message<
        isRequest, Body, Headers> const& msg)
    {
        beast::ostream(sb_) << msg;
    }

    bool
    complete() override
    {
        return sb_.size() == 0;
    }

    void
    consume (std::size_t bytes) override
    {
        sb_.consume(bytes);
    }

    bool
    prepare(std::size_t bytes,
        std::function<void(void)>) override
    {
        return true;
    }

    std::vector<boost::asio::const_buffer>
    data() override
    {
        auto const& buf = sb_.data();
        std::vector<boost::asio::const_buffer> result;
        result.reserve(std::distance(buf.begin(), buf.end()));
        for (auto const& b : buf)
            result.push_back(b);
        return result;
    }
};

} // sdchain

#endif
