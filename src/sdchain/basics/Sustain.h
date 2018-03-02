//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_BASICS_SUSTAIN_H_INCLUDED
#define SDCHAIN_BASICS_SUSTAIN_H_INCLUDED

#include <string>

namespace sdchain {

// "Sustain" is a system for a buddy process that monitors the main process
// and relaunches it on a fault.
bool HaveSustain ();
std::string StopSustain ();
std::string DoSustain ();

} // sdchain

#endif
