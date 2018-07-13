//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PATH_IMPL_AMOUNTSPEC_H_INCLUDED
#define SDCHAIN_PATH_IMPL_AMOUNTSPEC_H_INCLUDED

#include <sdchain/protocol/IOUAmount.h>
#include <sdchain/protocol/SDAAmount.h>
#include <sdchain/protocol/STAmount.h>

namespace sdchain {

struct AmountSpec
{
    bool native;
    union
    {
        SDAAmount sda;
        IOUAmount iou;
    };
    boost::optional<AccountID> issuer;
    boost::optional<Currency> currency;

    friend
    std::ostream&
    operator << (
        std::ostream& stream,
        AmountSpec const& amt)
    {
        if (amt.native)
            stream << to_string (amt.sda);
        else
            stream << to_string (amt.iou);
        if (amt.currency)
            stream << "/(" << *amt.currency << ")";
        if (amt.issuer)
            stream << "/" << *amt.issuer << "";
        return stream;
    }
};

struct EitherAmount
{
#ifndef NDEBUG
    bool native = false;
#endif

    union
    {
        IOUAmount iou;
        SDAAmount sda;
    };

    EitherAmount () = default;

    explicit
    EitherAmount (IOUAmount const& a)
            :iou(a)
    {
    }

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
    // ignore warning about half of iou amount being uninitialized
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
    explicit
    EitherAmount (SDAAmount const& a)
            :sda(a)
    {
#ifndef NDEBUG
        native = true;
#endif
    }
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif

    explicit
    EitherAmount (AmountSpec const& a)
    {
#ifndef NDEBUG
        native = a.native;
#endif
        if (a.native)
            sda = a.sda;
        else
            iou = a.iou;
    }
};

template <class T>
T&
get (EitherAmount& amt)
{
    static_assert(sizeof(T) == -1, "Must used specialized function");
    return T(0);
}

template <>
inline
IOUAmount&
get<IOUAmount> (EitherAmount& amt)
{
    assert (!amt.native);
    return amt.iou;
}

template <>
inline
SDAAmount&
get<SDAAmount> (EitherAmount& amt)
{
    assert (amt.native);
    return amt.sda;
}

template <class T>
T const&
get (EitherAmount const& amt)
{
    static_assert(sizeof(T) == -1, "Must used specialized function");
    return T(0);
}

template <>
inline
IOUAmount const&
get<IOUAmount> (EitherAmount const& amt)
{
    assert (!amt.native);
    return amt.iou;
}

template <>
inline
SDAAmount const&
get<SDAAmount> (EitherAmount const& amt)
{
    assert (amt.native);
    return amt.sda;
}

inline
AmountSpec
toAmountSpec (STAmount const& amt)
{
    assert (amt.mantissa () < std::numeric_limits<std::int64_t>::max ());
    bool const isNeg = amt.negative ();
    std::int64_t const sMant =
        isNeg ? - std::int64_t (amt.mantissa ()) : amt.mantissa ();
    AmountSpec result;

    result.native = isSDA (amt);
    if (result.native)
    {
        result.sda = SDAAmount (sMant);
    }
    else
    {
        result.iou = IOUAmount (sMant, amt.exponent ());
        result.issuer = amt.issue().account;
        result.currency = amt.issue().currency;
    }

    return result;
}

inline
EitherAmount
toEitherAmount (STAmount const& amt)
{
    if (isSDA (amt))
        return EitherAmount{amt.sda()};
    return EitherAmount{amt.iou()};
}

inline
AmountSpec
toAmountSpec (
    EitherAmount const& ea,
    boost::optional<Currency> const& c)
{
    AmountSpec r;
    r.native = (!c || isSDA (*c));
    r.currency = c;
    assert (ea.native == r.native);
    if (r.native)
    {
        r.sda = ea.sda;
    }
    else
    {
        r.iou = ea.iou;
    }
    return r;
}

}

#endif
