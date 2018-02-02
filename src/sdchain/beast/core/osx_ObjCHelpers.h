//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>

    Portions of this file are from JUCE.
    Copyright (c) 2013 - Raw Material Software Ltd.
    Please visit http://www.juce.com



*/
//==============================================================================

#ifndef BEAST_MODULE_CORE_NATIVE_OSX_OBJCHELPERS_H_INCLUDED
#define BEAST_MODULE_CORE_NATIVE_OSX_OBJCHELPERS_H_INCLUDED

#include <cassert>
#include <string>

namespace beast
{

/* This file contains a few helper functions that are used internally but which
   need to be kept away from the public headers because they use obj-C symbols.
*/
namespace
{
    static inline NSString* stringToNS (std::string const& s)
    {
#ifndef NDEBUG
        // The UTF-8 encoding function for ASCII characters
        // in the range [0-127] is the identity. We are more
        // strict and require only printable characters.
        for (auto const& c : s)
            assert (isprint(c));
#endif

        return [NSString stringWithUTF8String: s.c_str()];
    }
}

} // beast

#endif   // BEAST_OSX_OBJCHELPERS_H_INCLUDED
