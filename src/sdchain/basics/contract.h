//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2014 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_BASICS_CONTRACT_H_INCLUDED
#define SDCHAIN_BASICS_CONTRACT_H_INCLUDED

#include <sdchain/beast/type_name.h>
#include <exception>
#include <string>
#include <typeinfo>
#include <utility>

namespace sdchain {

/*  Programming By Contract

    This routines are used when checking
    preconditions, postconditions, and invariants.
*/

/** Generates and logs a call stack */
void
LogThrow (std::string const& title);

/** Rethrow the exception currently being handled.

    When called from within a catch block, it will pass
    control to the next matching exception handler, if any.
    Otherwise, std::terminate will be called.
*/
[[noreturn]]
inline
void
Rethrow ()
{
    LogThrow ("Re-throwing exception");
    throw;
}

template <class E, class... Args>
[[noreturn]]
inline
void
Throw (Args&&... args)
{
    static_assert (std::is_convertible<E*, std::exception*>::value,
        "Exception must derive from std::exception.");

    E e(std::forward<Args>(args)...);
    LogThrow (std::string("Throwing exception of type " +
                          beast::type_name<E>() +": ") + e.what());
    throw e;
}

/** Called when faulty logic causes a broken invariant. */
[[noreturn]]
void
LogicError (
    std::string const& how) noexcept;

} // sdchain

#endif
