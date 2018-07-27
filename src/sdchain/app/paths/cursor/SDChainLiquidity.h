//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_PATHS_CURSOR_SDCHAINLIQUIDITY_H_INCLUDED
#define SDCHAIN_APP_PATHS_CURSOR_SDCHAINLIQUIDITY_H_INCLUDED

#include <sdchain/app/paths/cursor/PathCursor.h>
#include <sdchain/app/paths/SDChainCalc.h>
#include <sdchain/app/paths/Tuning.h>
#include <sdchain/ledger/View.h>
#include <sdchain/protocol/Rate.h>

namespace sdchain {
namespace path {

void sdchainLiquidity (
    SDChainCalc&,
    Rate const& qualityIn,
    Rate const& qualityOut,
    STAmount const& saPrvReq,
    STAmount const& saCurReq,
    STAmount& saPrvAct,
    STAmount& saCurAct,
    std::uint64_t& uRateMax);

Rate
quality_in (
    ReadView const& view,
    AccountID const& uToAccountID,
    AccountID const& uFromAccountID,
    Currency const& currency);

Rate
quality_out (
    ReadView const& view,
    AccountID const& uToAccountID,
    AccountID const& uFromAccountID,
    Currency const& currency);

} // path
} // sdchain

#endif
