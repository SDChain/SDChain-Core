//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>

    Portions of this file are from JUCE.
    Copyright (c) 2013 - Raw Material Software Ltd.
    Please visit http://www.juce.com



*/
//==============================================================================

#include <sdchain/beast/core/Time.h>

namespace beast
{

std::int64_t currentTimeMillis()
{
  #if BEAST_WINDOWS
    struct _timeb t;
   #ifdef _INC_TIME_INL
    _ftime_s (&t);
   #else
    _ftime (&t);
   #endif
    return ((std::int64_t) t.time) * 1000 + t.millitm;
  #else
    struct timeval tv;
    gettimeofday (&tv, nullptr);
    return ((std::int64_t) tv.tv_sec) * 1000 + tv.tv_usec / 1000;
  #endif
}

} // beast
