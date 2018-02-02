//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/json/json_writer.h>
#include <sdchain/json/to_string.h>

namespace Json
{

std::string to_string (Value const& value)
{
    return FastWriter ().write (value);
}

std::string pretty (Value const& value)
{
    return StyledWriter().write (value);
}

} // namespace Json
