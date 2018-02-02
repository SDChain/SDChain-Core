//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>

    Portions of this file are from JUCE.
    Copyright (c) 2013 - Raw Material Software Ltd.
    Please visit http://www.juce.com



*/
//==============================================================================

#if BEAST_INCLUDE_BEASTCONFIG
#include <BeastConfig.h>
#endif

//==============================================================================

#include <sdchain/beast/core/BasicNativeHeaders.h>

// TargetPlatform.h should not use anything from BeastConfig.h
#include <sdchain/beast/core/Config.h>

#if BEAST_MSVC
# pragma warning (disable: 4251) // (DLL build warning, must be disabled before pushing the warning state)
# pragma warning (push)
# pragma warning (disable: 4786) // (long class name warning)
# ifdef __INTEL_COMPILER
#  pragma warning (disable: 1125)
# endif
#endif

//------------------------------------------------------------------------------

// New header-only library modeled more closely according to boost
#include <sdchain/beast/core/ByteOrder.h>

#include <sdchain/beast/core/StandardIncludes.h>

// Order matters, since headers don't have their own #include lines.
// Add new includes to the bottom.

#include <sdchain/beast/core/LexicalCast.h>

#include <sdchain/beast/core/SystemStats.h>
#include <sdchain/beast/core/SemanticVersion.h>

#if BEAST_MSVC
#pragma warning (pop)
#endif

#include <locale>
#include <cctype>

#if ! BEAST_BSD
 #include <sys/timeb.h>
#endif

#if ! BEAST_ANDROID
 #include <cwctype>
#endif

#if BEAST_WINDOWS
 #include <ctime>
 #include <winsock2.h>
 #include <ws2tcpip.h>

 #if ! BEAST_MINGW
  #pragma warning ( push )
  #pragma warning ( disable: 4091 )
  #include <Dbghelp.h>
  #pragma warning ( pop )

  #if ! BEAST_DONT_AUTOLINK_TO_WIN32_LIBRARIES
   #pragma comment (lib, "DbgHelp.lib")
  #endif
 #endif

 #if BEAST_MINGW
  #include <ws2spi.h>
 #endif

#else
 #if BEAST_LINUX || BEAST_ANDROID
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <sys/errno.h>
  #include <unistd.h>
  #include <netinet/in.h>
 #endif

 #if BEAST_LINUX
  #include <langinfo.h>
 #endif

 #include <pwd.h>
 #include <fcntl.h>
 #include <netdb.h>
 #include <arpa/inet.h>
 #include <netinet/tcp.h>
 #include <sys/time.h>
 #include <net/if.h>
 #include <sys/ioctl.h>

 #if ! BEAST_ANDROID && ! BEAST_BSD
  #include <execinfo.h>
 #endif
#endif

#if BEAST_MAC || BEAST_IOS
 #include <xlocale.h>
 #include <mach/mach.h>
#endif

#if BEAST_ANDROID
 #include <android/log.h>
#endif

//------------------------------------------------------------------------------

// If the MSVC debug heap headers were included, disable
// the macros during the juce include since they conflict.
#ifdef _CRTDBG_MAP_ALLOC
#pragma push_macro("calloc")
#pragma push_macro("free")
#pragma push_macro("malloc")
#pragma push_macro("realloc")
#pragma push_macro("_recalloc")
#pragma push_macro("_aligned_free")
#pragma push_macro("_aligned_malloc")
#pragma push_macro("_aligned_offset_malloc")
#pragma push_macro("_aligned_realloc")
#pragma push_macro("_aligned_recalloc")
#pragma push_macro("_aligned_offset_realloc")
#pragma push_macro("_aligned_offset_recalloc")
#pragma push_macro("_aligned_msize")
#undef calloc
#undef free
#undef malloc
#undef realloc
#undef _recalloc
#undef _aligned_free
#undef _aligned_malloc
#undef _aligned_offset_malloc
#undef _aligned_realloc
#undef _aligned_recalloc
#undef _aligned_offset_realloc
#undef _aligned_offset_recalloc
#undef _aligned_msize
#endif

#include <sdchain/beast/core/Memory.h>

#if BEAST_MAC || BEAST_IOS
#include <sdchain/beast/core/osx_ObjCHelpers.h>
#endif

#if BEAST_ANDROID
#include "native/android_JNIHelpers.h"
#endif

#if BEAST_MAC || BEAST_IOS
#include <sdchain/beast/core/mac_SystemStats.mm>

#elif BEAST_WINDOWS
#include <sdchain/beast/core/win32_SystemStats.cpp>

#elif BEAST_LINUX
#include <sdchain/beast/core/linux_SystemStats.cpp>

#elif BEAST_BSD
#include <sdchain/beast/core/bsd_SystemStats.cpp>

#elif BEAST_ANDROID
#include "native/android_Files.cpp"
#include "native/android_Misc.cpp"
#include "native/android_SystemStats.cpp"
#include "native/android_Threads.cpp"

#endif

#include <sdchain/beast/core/CurrentThreadName.cpp>
#include <sdchain/beast/core/SemanticVersion.cpp>
#include <sdchain/beast/core/SystemStats.cpp>
#include <sdchain/beast/core/Time.cpp>
#include <sdchain/beast/core/WaitableEvent.cpp>

#ifdef _CRTDBG_MAP_ALLOC
#pragma pop_macro("calloc")
#pragma pop_macro("free")
#pragma pop_macro("malloc")
#pragma pop_macro("realloc")
#pragma pop_macro("_recalloc")
#pragma pop_macro("_aligned_free")
#pragma pop_macro("_aligned_malloc")
#pragma pop_macro("_aligned_offset_malloc")
#pragma pop_macro("_aligned_realloc")
#pragma pop_macro("_aligned_recalloc")
#pragma pop_macro("_aligned_offset_realloc")
#pragma pop_macro("_aligned_offset_recalloc")
#pragma pop_macro("_aligned_msize")
#endif
