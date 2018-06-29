//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_SDAAMOUNT_H_INCLUDED
#define SDCHAIN_PROTOCOL_SDAAMOUNT_H_INCLUDED

#include <sdchain/basics/contract.h>
#include <sdchain/protocol/SystemParameters.h>
#include <sdchain/beast/utility/Zero.h>
#include <boost/operators.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <cstdint>
#include <string>
#include <type_traits>

using beast::zero;

namespace sdchain {

class SDAAmount
    : private boost::totally_ordered <SDAAmount>
    , private boost::additive <SDAAmount>
{
private:
    std::int64_t drops_;

public:
    SDAAmount () = default;
    SDAAmount (SDAAmount const& other) = default;
    SDAAmount& operator= (SDAAmount const& other) = default;

    SDAAmount (beast::Zero)
        : drops_ (0)
    {
    }

    SDAAmount&
    operator= (beast::Zero)
    {
        drops_ = 0;
        return *this;
    }

    template <class Integer,
        class = typename std::enable_if_t <
            std::is_integral<Integer>::value>>
    SDAAmount (Integer drops)
        : drops_ (static_cast<std::int64_t> (drops))
    {
    }

    template <class Integer,
        class = typename std::enable_if_t <
            std::is_integral<Integer>::value>>
    SDAAmount&
    operator= (Integer drops)
    {
        drops_ = static_cast<std::int64_t> (drops);
        return *this;
    }

    SDAAmount&
    operator+= (SDAAmount const& other)
    {
        drops_ += other.drops_;
        return *this;
    }

    SDAAmount&
    operator-= (SDAAmount const& other)
    {
        drops_ -= other.drops_;
        return *this;
    }

    SDAAmount
    operator- () const
    {
        return { -drops_ };
    }

    bool
    operator==(SDAAmount const& other) const
    {
        return drops_ == other.drops_;
    }

    bool
    operator<(SDAAmount const& other) const
    {
        return drops_ < other.drops_;
    }

    /** Returns true if the amount is not zero */
    explicit
    operator bool() const noexcept
    {
        return drops_ != 0;
    }

    /** Return the sign of the amount */
    int
    signum() const noexcept
    {
        return (drops_ < 0) ? -1 : (drops_ ? 1 : 0);
    }

    /** Returns the number of drops */
    std::int64_t
    drops () const
    {
        return drops_;
    }
};

inline
std::string
to_string (SDAAmount const& amount)
{
    return std::to_string (amount.drops ());
}

inline
SDAAmount
mulRatio (
    SDAAmount const& amt,
    std::uint32_t num,
    std::uint32_t den,
    bool roundUp)
{
    using namespace boost::multiprecision;

    if (!den)
        Throw<std::runtime_error> ("division by zero");

    int128_t const amt128 (amt.drops ());
    auto const neg = amt.drops () < 0;
    auto const m = amt128 * num;
    auto r = m / den;
    if (m % den)
    {
        if (!neg && roundUp)
            r += 1;
        if (neg && !roundUp)
            r -= 1;
    }
    if (r > std::numeric_limits<std::int64_t>::max ())
        Throw<std::overflow_error> ("SDA mulRatio overflow");
    return SDAAmount (r.convert_to<std::int64_t> ());
}

/** Returns true if the amount does not exceed the initial SDA in existence. */
inline
bool isLegalAmount (SDAAmount const& amount)
{
    return amount.drops () <= SYSTEM_CURRENCY_START;
}

}

#endif
