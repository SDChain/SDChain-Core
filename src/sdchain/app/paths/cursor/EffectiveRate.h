//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_PATHS_CURSOR_EFFECTIVERATE_H_INCLUDED
#define SDCHAIN_APP_PATHS_CURSOR_EFFECTIVERATE_H_INCLUDED

#include <sdchain/protocol/AccountID.h>
#include <sdchain/protocol/Issue.h>
#include <sdchain/protocol/Rate.h>
#include <boost/optional.hpp>

namespace sdchain {
namespace path {

Rate
effectiveRate(
    Issue const& issue,
    AccountID const& account1,
    AccountID const& account2,
    boost::optional<Rate> const& rate);

} // path
} // sdchain

#endif
