//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2014, Tom Ritchford <tom@swirly.com>



*/
//==============================================================================

#ifndef BEAST_UTILITY_ZERO_H_INCLUDED
#define BEAST_UTILITY_ZERO_H_INCLUDED

#include <sdchain/beast/core/CompilerConfig.h>

// VS2013 SP1 fails with decltype return
#define BEAST_NO_ZERO_AUTO_RETURN 1

namespace beast {

/** Zero allows classes to offer efficient comparisons to zero.

    Zero is a struct to allow classes to efficiently compare with zero without
    requiring an rvalue construction.

    It's often the case that we have classes which combine a number and a unit.
    In such cases, comparisons like t > 0 or t != 0 make sense, but comparisons
    like t > 1 or t != 1 do not.

    The class Zero allows such comparisons to be easily made.

    The comparing class T either needs to have a method called signum() which
    returns a positive number, 0, or a negative; or there needs to be a signum
    function which resolves in the namespace which takes an instance of T and
    returns a positive, zero or negative number.
*/

struct Zero
{
};

namespace {
static constexpr Zero zero{};
}

/** Default implementation of signum calls the method on the class. */
template <typename T>
#if BEAST_NO_ZERO_AUTO_RETURN
int signum(T const& t)
#else
auto signum(T const& t) -> decltype(t.signum())
#endif
{
    return t.signum();
}

namespace detail {
namespace zero_helper {

// For argument dependent lookup to function properly, calls to signum must
// be made from a namespace that does not include overloads of the function..
template <class T>
#if BEAST_NO_ZERO_AUTO_RETURN
int call_signum (T const& t)
#else
auto call_signum(T const& t) -> decltype(t.signum())
#endif
{
    return signum(t);
}

} // zero_helper
} // detail

// Handle operators where T is on the left side using signum.

template <typename T>
bool operator==(T const& t, Zero)
{
    return detail::zero_helper::call_signum(t) == 0;
}

template <typename T>
bool operator!=(T const& t, Zero)
{
    return detail::zero_helper::call_signum(t) != 0;
}

template <typename T>
bool operator<(T const& t, Zero)
{
    return detail::zero_helper::call_signum(t) < 0;
}

template <typename T>
bool operator>(T const& t, Zero)
{
    return detail::zero_helper::call_signum(t) > 0;
}

template <typename T>
bool operator>=(T const& t, Zero)
{
    return detail::zero_helper::call_signum(t) >= 0;
}

template <typename T>
bool operator<=(T const& t, Zero)
{
    return detail::zero_helper::call_signum(t) <= 0;
}

// Handle operators where T is on the right side by
// reversing the operation, so that T is on the left side.

template <typename T>
bool operator==(Zero, T const& t)
{
    return t == zero;
}

template <typename T>
bool operator!=(Zero, T const& t)
{
    return t != zero;
}

template <typename T>
bool operator<(Zero, T const& t)
{
    return t > zero;
}

template <typename T>
bool operator>(Zero, T const& t)
{
    return t < zero;
}

template <typename T>
bool operator>=(Zero, T const& t)
{
    return t <= zero;
}

template <typename T>
bool operator<=(Zero, T const& t)
{
    return t >= zero;
}

} // beast

#endif
