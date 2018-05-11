//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_AMOUNTCONVERSION_H_INCLUDED
#define SDCHAIN_PROTOCOL_AMOUNTCONVERSION_H_INCLUDED

#include <sdchain/protocol/IOUAmount.h>
#include <sdchain/protocol/SDAAmount.h>
#include <sdchain/protocol/STAmount.h>

namespace sdchain {

inline
STAmount
toSTAmount (IOUAmount const& iou, Issue const& iss)
{
    bool const isNeg = iou.signum() < 0;
    std::uint64_t const umant = isNeg ? - iou.mantissa () : iou.mantissa ();
    return STAmount (iss, umant, iou.exponent (), /*native*/ false, isNeg,
                     STAmount::unchecked ());
}

inline
STAmount
toSTAmount (IOUAmount const& iou)
{
    return toSTAmount (iou, noIssue ());
}

inline
STAmount
toSTAmount (SDAAmount const& sda)
{
    bool const isNeg = sda.signum() < 0;
    std::uint64_t const umant = isNeg ? - sda.drops () : sda.drops ();
    return STAmount (umant, isNeg);
}

inline
STAmount
toSTAmount (SDAAmount const& sda, Issue const& iss)
{
    assert (isSDA(iss.account) && isSDA(iss.currency));
    return toSTAmount (sda);
}

template <class T>
T
toAmount (STAmount const& amt)
{
    static_assert(sizeof(T) == -1, "Must use specialized function");
    return T(0);
}

template <>
inline
STAmount
toAmount<STAmount> (STAmount const& amt)
{
    return amt;
}

template <>
inline
IOUAmount
toAmount<IOUAmount> (STAmount const& amt)
{
    assert (amt.mantissa () < std::numeric_limits<std::int64_t>::max ());
    bool const isNeg = amt.negative ();
    std::int64_t const sMant =
            isNeg ? - std::int64_t (amt.mantissa ()) : amt.mantissa ();

    assert (! isSDA (amt));
    return IOUAmount (sMant, amt.exponent ());
}

template <>
inline
SDAAmount
toAmount<SDAAmount> (STAmount const& amt)
{
    assert (amt.mantissa () < std::numeric_limits<std::int64_t>::max ());
    bool const isNeg = amt.negative ();
    std::int64_t const sMant =
            isNeg ? - std::int64_t (amt.mantissa ()) : amt.mantissa ();

    assert (isSDA (amt));
    return SDAAmount (sMant);
}


}

#endif
