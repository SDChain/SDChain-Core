//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>

    Portions of this file are from JUCE.
    Copyright (c) 2013 - Raw Material Software Ltd.
    Please visit http://www.juce.com



*/
//==============================================================================

#ifndef BEAST_MEMORY_H_INCLUDED
#define BEAST_MEMORY_H_INCLUDED

#include <cstring>

#include <sdchain/beast/core/Config.h>

namespace beast {

//==============================================================================
/** Fills a block of memory with zeros. */
inline void zeromem (void* memory, size_t numBytes) noexcept
    { memset (memory, 0, numBytes); }

/** Overwrites a structure or object with zeros. */
template <typename Type>
void zerostruct (Type& structure) noexcept
    { memset (&structure, 0, sizeof (structure)); }

//==============================================================================
#if BEAST_MAC || BEAST_IOS || DOXYGEN

 /** A handy C++ wrapper that creates and deletes an NSAutoreleasePool object using RAII.
     You should use the BEAST_AUTORELEASEPOOL macro to create a local auto-release pool on the stack.
 */
 class ScopedAutoReleasePool
 {
 public:
     ScopedAutoReleasePool();
     ~ScopedAutoReleasePool();


    ScopedAutoReleasePool(ScopedAutoReleasePool const&) = delete;
    ScopedAutoReleasePool& operator= (ScopedAutoReleasePool const&) = delete;

 private:
     void* pool;
 };

 /** A macro that can be used to easily declare a local ScopedAutoReleasePool
     object for RAII-based obj-C autoreleasing.
     Because this may use the \@autoreleasepool syntax, you must follow the macro with
     a set of braces to mark the scope of the pool.
 */
#if (BEAST_COMPILER_SUPPORTS_ARC && defined (__OBJC__)) || DOXYGEN
 #define BEAST_AUTORELEASEPOOL  @autoreleasepool
#else
 #define BEAST_AUTORELEASEPOOL  const beast::ScopedAutoReleasePool BEAST_JOIN_MACRO (autoReleasePool_, __LINE__);
#endif

#else
 #define BEAST_AUTORELEASEPOOL
#endif

}

#endif

