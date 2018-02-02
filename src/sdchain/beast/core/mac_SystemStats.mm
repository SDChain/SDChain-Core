//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>

    Portions of this file are from JUCE.
    Copyright (c) 2013 - Raw Material Software Ltd.
    Please visit http://www.juce.com



*/
//==============================================================================

namespace beast
{

ScopedAutoReleasePool::ScopedAutoReleasePool()
{
    pool = [[NSAutoreleasePool alloc] init];
}

ScopedAutoReleasePool::~ScopedAutoReleasePool()
{
    [((NSAutoreleasePool*) pool) release];
}

//==============================================================================
void outputDebugString (std::string const& text)
{
    // Would prefer to use std::cerr here, but avoiding it for
    // the moment, due to clang JIT linkage problems.
    fputs (text.c_str (), stderr);
    fputs ("\n", stderr);
    fflush (stderr);
}

//==============================================================================

#if BEAST_MAC
struct RLimitInitialiser
{
    RLimitInitialiser()
    {
        rlimit lim;
        getrlimit (RLIMIT_NOFILE, &lim);
        lim.rlim_cur = lim.rlim_max = RLIM_INFINITY;
        setrlimit (RLIMIT_NOFILE, &lim);
    }
};

static RLimitInitialiser rLimitInitialiser;
#endif

//==============================================================================
std::string getComputerName()
{
    // FIXME: Remove ".local" from the name
    char name [256] = { 0 };

    if (gethostname (name, sizeof (name) - 1) == 0)
        return name;

    return std::string{};
}

} // beast
