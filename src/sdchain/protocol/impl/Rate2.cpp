//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2015 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/protocol/Quality.h>
#include <sdchain/protocol/Rate.h>

namespace sdchain {

Rate const parityRate (QUALITY_ONE);

namespace detail {

STAmount as_amount (Rate const& rate)
{
    return { noIssue(), rate.value, -9, false };
}

}

STAmount
multiply (
    STAmount const& amount,
    Rate const& rate)
{
    assert (rate.value != 0);

    if (rate == parityRate)
        return amount;

    return multiply (
        amount,
        detail::as_amount(rate),
        amount.issue());
}

STAmount
multiplyRound (
    STAmount const& amount,
    Rate const& rate,
    bool roundUp)
{
    assert (rate.value != 0);

    if (rate == parityRate)
        return amount;

    return mulRound (
        amount,
        detail::as_amount(rate),
        amount.issue(),
        roundUp);
}

STAmount
multiplyRound (
    STAmount const& amount,
    Rate const& rate,
    Issue const& issue,
    bool roundUp)
{
    assert (rate.value != 0);

    if (rate == parityRate)
    {
        return amount;
    }

    return mulRound (
        amount,
        detail::as_amount(rate),
        issue,
        roundUp);
}

STAmount
divide (
    STAmount const& amount,
    Rate const& rate)
{
    assert (rate.value != 0);

    if (rate == parityRate)
        return amount;

    return divide (
        amount,
        detail::as_amount(rate),
        amount.issue());
}

STAmount
divideRound (
    STAmount const& amount,
    Rate const& rate,
    bool roundUp)
{
    assert (rate.value != 0);

    if (rate == parityRate)
        return amount;

    return divRound (
        amount,
        detail::as_amount(rate),
        amount.issue(),
        roundUp);
}

STAmount
divideRound (
    STAmount const& amount,
    Rate const& rate,
    Issue const& issue,
    bool roundUp)
{
    assert (rate.value != 0);

    if (rate == parityRate)
        return amount;

    return divRound (
        amount,
        detail::as_amount(rate),
        issue,
        roundUp);
}

}
