//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/basics/contract.h>
#include <sdchain/basics/Log.h>
#include <cstdlib>
#include <exception>
#include <iostream>

namespace sdchain {

namespace detail {

[[noreturn]]
void
accessViolation() noexcept
{
    // dereference memory location zero
    int volatile* j = 0;
    (void)*j;
    std::abort ();
}

} // detail

void
LogThrow (std::string const& title)
{
    JLOG(debugLog().warn()) << title;
}

[[noreturn]]
void
LogicError (std::string const& s) noexcept
{
    JLOG(debugLog().fatal()) << s;
    std::cerr << "Logic error: " << s << std::endl;
    detail::accessViolation();
}

} // sdchain
