//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_RPC_JSON_BODY_H
#define SDCHAIN_RPC_JSON_BODY_H

#include <sdchain/json/json_value.h>
#include <beast/core/multi_buffer.hpp>
#include <beast/http/message.hpp>

namespace sdchain {

/// Body that holds JSON
struct json_body
{
    using value_type = Json::Value;

    class reader
    {
        using dynamic_buffer_type = beast::multi_buffer;
            
        dynamic_buffer_type buffer_;

    public:
        using const_buffers_type =
            typename dynamic_buffer_type::const_buffers_type;

        using is_deferred = std::false_type;

        template<bool isRequest, class Fields>
        explicit
        reader(beast::http::message<
            isRequest, json_body, Fields> const& m)
        {
            stream(m.body,
                [&](void const* data, std::size_t n)
                {
                    buffer_.commit(boost::asio::buffer_copy(
                        buffer_.prepare(n), boost::asio::buffer(data, n)));
                });
        }

        void
        init(beast::error_code&) noexcept
        {
        }

        boost::optional<std::pair<const_buffers_type, bool>>
        get(beast::error_code& ec)
        {
            return {{buffer_.data(), false}};
        }

        void
        finish(beast::error_code&)
        {
        }
    };
};

} // sdchain

#endif
