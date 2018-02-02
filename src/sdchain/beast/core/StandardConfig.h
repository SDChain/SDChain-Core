//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>

    Portions of this file are from JUCE.
    Copyright (c) 2013 - Raw Material Software Ltd.
    Please visit http://www.juce.com



*/
//==============================================================================

#ifndef BEAST_CONFIG_STANDARDCONFIG_H_INCLUDED
#define BEAST_CONFIG_STANDARDCONFIG_H_INCLUDED

#ifndef BEAST_CONFIG_COMPILERCONFIG_H_INCLUDED
#error "CompilerConfig.h must be included first"
#endif

// Now we'll include some common OS headers..
#if BEAST_MSVC
#pragma warning (push)
#pragma warning (disable: 4514 4245 4100)
#endif

#if BEAST_USE_INTRINSICS
#include <intrin.h>
#endif

#if BEAST_MAC || BEAST_IOS
#include <libkern/OSAtomic.h>
#endif

#if BEAST_LINUX
#include <signal.h>
# if __INTEL_COMPILER
#  if __ia64__
#include <ia64intrin.h>
#  else
#include <ia32intrin.h>
#  endif
# endif
#endif

#if BEAST_MSVC && BEAST_DEBUG
#include <crtdbg.h>
#include <stdlib.h>
#include <malloc.h>
#endif

#if BEAST_MSVC
#pragma warning (pop)
#endif

#if BEAST_ANDROID
 #include <sys/atomics.h>
 #include <byteswap.h>
#endif

// undef symbols that are sometimes set by misguided 3rd-party headers..
#undef check
#undef TYPE_BOOL
#undef max
#undef min

#endif
