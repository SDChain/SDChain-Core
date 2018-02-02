//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#include <sdchain/beast/utility/Debug.h>
#include <sdchain/beast/core/SystemStats.h>

namespace beast {

namespace Debug {

//------------------------------------------------------------------------------

#if BEAST_MSVC && defined (_DEBUG)

#if BEAST_CHECK_MEMORY_LEAKS
struct DebugFlagsInitialiser
{
    DebugFlagsInitialiser()
    {
        // Activate leak checks on exit in the MSVC Debug CRT (C Runtime)
        //
        _CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    }
};

static DebugFlagsInitialiser debugFlagsInitialiser;
#endif

void setAlwaysCheckHeap (bool bAlwaysCheck)
{
    int flags = _CrtSetDbgFlag (_CRTDBG_REPORT_FLAG);

    if (bAlwaysCheck) flags |= _CRTDBG_CHECK_ALWAYS_DF; // on
    else flags &= ~_CRTDBG_CHECK_ALWAYS_DF; // off

    _CrtSetDbgFlag (flags);
}

void setHeapDelayedFree (bool bDelayedFree)
{
    int flags = _CrtSetDbgFlag (_CRTDBG_REPORT_FLAG);

    if (bDelayedFree) flags |= _CRTDBG_DELAY_FREE_MEM_DF; // on
    else flags &= ~_CRTDBG_DELAY_FREE_MEM_DF; // off

    _CrtSetDbgFlag (flags);
}

void setHeapReportLeaks (bool bReportLeaks)
{
    int flags = _CrtSetDbgFlag (_CRTDBG_REPORT_FLAG);

    if (bReportLeaks) flags |= _CRTDBG_LEAK_CHECK_DF; // on
    else flags &= ~_CRTDBG_LEAK_CHECK_DF; // off

    _CrtSetDbgFlag (flags);
}

void reportLeaks ()
{
    _CrtDumpMemoryLeaks ();
}

void checkHeap ()
{
    _CrtCheckMemory ();
}

//------------------------------------------------------------------------------

#else

void setAlwaysCheckHeap (bool)
{
}

void setHeapDelayedFree (bool)
{
}

void setHeapReportLeaks (bool)
{
}

void reportLeaks ()
{
}

void checkHeap ()
{
}

#endif

}

}
