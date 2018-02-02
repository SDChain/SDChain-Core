//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>

    Portions of this file are from JUCE.
    Copyright (c) 2013 - Raw Material Software Ltd.
    Please visit http://www.juce.com



*/
//==============================================================================

#ifndef BEAST_MODULE_CORE_SYSTEM_STANDARDINCLUDES_H_INCLUDED
#define BEAST_MODULE_CORE_SYSTEM_STANDARDINCLUDES_H_INCLUDED

// Include some common OS headers..
#if BEAST_MSVC
#pragma warning (push)
#pragma warning (disable: 4514 4245 4100)
#endif

#include <algorithm>
#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <exception>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <new>
#include <numeric>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <locale.h>
#include <math.h>
#include <memory.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// undef symbols that are sometimes set by misguided 3rd-party headers..
#undef check
#undef TYPE_BOOL
#undef max
#undef min

#endif
