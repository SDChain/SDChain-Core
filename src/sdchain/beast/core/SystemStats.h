//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>

    Portions of this file are from JUCE.
    Copyright (c) 2013 - Raw Material Software Ltd.
    Please visit http://www.juce.com



*/
//==============================================================================

#ifndef BEAST_MODULE_CORE_SYSTEM_SYSTEMSTATS_H_INCLUDED
#define BEAST_MODULE_CORE_SYSTEM_SYSTEMSTATS_H_INCLUDED

#include <string>
#include <vector>

namespace beast
{

//==============================================================================
/** Returns the host-name of the computer. */
std::string getComputerName();

//==============================================================================
/** Returns a backtrace of the current call-stack.
    The usefulness of the result will depend on the level of debug symbols
    that are available in the executable.
*/
std::vector <std::string>
getStackBacktrace();

} // beast

#endif   // BEAST_SYSTEMSTATS_H_INCLUDED
