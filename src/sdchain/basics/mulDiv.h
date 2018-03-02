//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2015 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_BASICS_MULDIV_H_INCLUDED
#define SDCHAIN_BASICS_MULDIV_H_INCLUDED

#include <cstdint>
#include <utility>

namespace sdchain
{

/** Return value*mul/div accurately.
    Computes the result of the multiplication and division in
    a single step, avoiding overflow and retaining precision.
    Throws:
        None
    Returns:
        `std::pair`:
            `first` is `false` if the calculation overflows,
                `true` if the calculation is safe.
            `second` is the result of the calculation if
                `first` is `false`, max value of `uint64_t`
                if `true`.
*/
std::pair<bool, std::uint64_t>
mulDiv(std::uint64_t value, std::uint64_t mul, std::uint64_t div);

} // sdchain

#endif
