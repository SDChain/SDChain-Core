//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/rpc/Status.h>

namespace sdchain {
namespace RPC {

constexpr Status::Code Status::OK;

std::string Status::codeString () const
{
    if (!*this)
        return "";

    if (type_ == Type::none)
        return std::to_string (code_);

    if (type_ == Status::Type::TER)
    {
        std::string s1, s2;

        auto success = transResultInfo (toTER (), s1, s2);
        assert (success);
        (void) success;

        return s1 + ": " + s2;
    }

    if (type_ == Status::Type::error_code_i)
    {
        auto info = get_error_info (toErrorCode ());
        return info.token +  ": " + info.message;
    }

    assert (false);
    return "";
}

void Status::fillJson (Json::Value& value)
{
    if (!*this)
        return;

    auto& error = value[jss::error];
    error[jss::code] = code_;
    error[jss::message] = codeString ();

    // Are there any more messages?
    if (!messages_.empty ())
    {
        auto& messages = error[jss::data];
        for (auto& i: messages_)
            messages.append (i);
    }
}

std::string Status::message() const {
    std::string result;
    for (auto& m: messages_)
    {
        if (!result.empty())
            result += '/';
        result += m;
    }

    return result;
}

std::string Status::toString() const {
    if (*this)
        return codeString() + ":" + message();
    return "";
}

} // namespace RPC
} // sdchain
