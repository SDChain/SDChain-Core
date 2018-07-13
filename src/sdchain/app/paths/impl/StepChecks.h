//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2015 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_PATHS_IMPL_STEP_CHECKS_H_INCLUDED
#define SDCHAIN_APP_PATHS_IMPL_STEP_CHECKS_H_INCLUDED

#include <sdchain/basics/Log.h>
#include <sdchain/beast/utility/Journal.h>
#include <sdchain/ledger/ReadView.h>
#include <sdchain/protocol/AccountID.h>
#include <sdchain/protocol/UintTypes.h>

namespace sdchain {

inline
TER
checkFreeze (
    ReadView const& view,
    AccountID const& src,
    AccountID const& dst,
    Currency const& currency)
{
    assert (src != dst);

    // check freeze
    if (auto sle = view.read (keylet::account (dst)))
    {
        if (sle->isFlag (lsfGlobalFreeze))
        {
            return terNO_LINE;
        }
    }

    if (auto sle = view.read (keylet::line (src, dst, currency)))
    {
        if (sle->isFlag ((dst > src) ? lsfHighFreeze : lsfLowFreeze))
        {
            return terNO_LINE;
        }
    }

    return tesSUCCESS;
}

inline
TER
checkNoSDChain (
    ReadView const& view,
    AccountID const& prev,
    AccountID const& cur,
    // This is the account whose constraints we are checking
    AccountID const& next,
    Currency const& currency,
    beast::Journal j)
{
    // fetch the sdchain lines into and out of this node
    auto sleIn = view.read (keylet::line (prev, cur, currency));
    auto sleOut = view.read (keylet::line (cur, next, currency));

    if (!sleIn || !sleOut)
        return terNO_LINE;

    if ((*sleIn)[sfFlags] &
            ((cur > prev) ? lsfHighNoSDChain : lsfLowNoSDChain) &&
        (*sleOut)[sfFlags] &
            ((cur > next) ? lsfHighNoSDChain : lsfLowNoSDChain))
    {
        JLOG (j.info()) << "Path violates noSDChain constraint between " << prev
                      << ", " << cur << " and " << next;

        return terNO_SDCHAIN;
    }
    return tesSUCCESS;
}

}

#endif
