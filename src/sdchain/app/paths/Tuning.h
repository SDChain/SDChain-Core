//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_PATHS_TUNING_H_INCLUDED
#define SDCHAIN_APP_PATHS_TUNING_H_INCLUDED

namespace sdchain {

int const CALC_NODE_DELIVER_MAX_LOOPS = 100;
int const CALC_NODE_DELIVER_MAX_LOOPS_MQ = 2000;
int const NODE_ADVANCE_MAX_LOOPS = 100;
int const PAYMENT_MAX_LOOPS = 1000;
int const PATHFINDER_HIGH_PRIORITY = 100000;
int const PATHFINDER_MAX_PATHS = 50;
int const PATHFINDER_MAX_COMPLETE_PATHS = 1000;
int const PATHFINDER_MAX_PATHS_FROM_SOURCE = 10;

} // sdchain

#endif
