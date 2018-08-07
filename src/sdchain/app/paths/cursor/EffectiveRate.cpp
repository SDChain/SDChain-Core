//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/paths/cursor/EffectiveRate.h>
#include <sdchain/basics/contract.h>

namespace sdchain {
namespace path {

Rate
effectiveRate(
    Issue const& issue,
    AccountID const& account1,
    AccountID const& account2,
    boost::optional<Rate> const& rate)
{
    // 1:1 transfer rate for SDA
    if (isSDA (issue))
        return parityRate;

    if (!rate)
        LogicError ("No transfer rate set for node.");

    // 1:1 transfer rate if either of the accounts is the issuer
    if (issue.account == account1 || issue.account == account2)
        return parityRate;

    return rate.get();
}

} // path
} // sdchain
