//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2016 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_MISC_DETAIL_WORK_H_INCLUDED
#define SDCHAIN_APP_MISC_DETAIL_WORK_H_INCLUDED

#include <beast/http/message.hpp>
#include <beast/http/string_body.hpp>

namespace sdchain {

namespace detail {

using response_type =
    beast::http::response<beast::http::string_body>;

class Work
{
public:
    virtual ~Work() = default;

    virtual void run() = 0;

    virtual void cancel() = 0;
};

} // detail

} // sdchain

#endif
