//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2015 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/basics/mulDiv.h>
#include <sdchain/basics/contract.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <limits>
#include <utility>

namespace sdchain
{

std::pair<bool, std::uint64_t>
mulDiv(std::uint64_t value, std::uint64_t mul, std::uint64_t div)
{
    using namespace boost::multiprecision;

    uint128_t result;
    result = multiply(result, value, mul);

    result /= div;

    auto const limit = std::numeric_limits<std::uint64_t>::max();

    if (result > limit)
        return { false, limit };

    return { true, static_cast<std::uint64_t>(result) };
}

} // sdchain
