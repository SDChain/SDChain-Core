//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#ifndef BEAST_CONFIG_CONFIGCHECK_H_INCLUDED
#define BEAST_CONFIG_CONFIGCHECK_H_INCLUDED

//
// Apply sensible defaults for the configuration settings
//

#ifndef BEAST_FORCE_DEBUG
#define BEAST_FORCE_DEBUG 0
#endif

#ifndef BEAST_LOG_ASSERTIONS
# if BEAST_ANDROID
#  define BEAST_LOG_ASSERTIONS 1
# else
#  define BEAST_LOG_ASSERTIONS 0
# endif
#endif

#if BEAST_DEBUG && ! defined (BEAST_CHECK_MEMORY_LEAKS)
#define BEAST_CHECK_MEMORY_LEAKS 1
#endif

#ifndef BEAST_DISABLE_CONTRACT_CHECKS
#define BEAST_DISABLE_CONTRACT_CHECKS 0
#endif

//------------------------------------------------------------------------------

#ifndef BEAST_DONT_AUTOLINK_TO_WIN32_LIBRARIES
#define BEAST_DONT_AUTOLINK_TO_WIN32_LIBRARIES 0
#endif

#ifndef BEAST_INCLUDE_ZLIB_CODE
#define BEAST_INCLUDE_ZLIB_CODE 1
#endif

#ifndef BEAST_ZLIB_INCLUDE_PATH
#define BEAST_ZLIB_INCLUDE_PATH <zlib.h>
#endif

#ifndef BEAST_SQLITE_FORCE_NDEBUG
#define BEAST_SQLITE_FORCE_NDEBUG 0
#endif

#ifndef BEAST_STRING_UTF_TYPE
#define BEAST_STRING_UTF_TYPE 8
#endif

#endif
