//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#ifndef BEAST_UTILITY_DEBUG_H_INCLUDED
#define BEAST_UTILITY_DEBUG_H_INCLUDED

#include <string>

namespace beast {

// Auxiliary outines for debugging

namespace Debug
{

//
// These control the MSVC C Runtime Debug heap.
//
// The calls currently do nothing on other platforms.
//

/** Calls checkHeap() at every allocation and deallocation.
*/
extern void setAlwaysCheckHeap (bool bAlwaysCheck);

/** Keep freed memory blocks in the heap's linked list, assign them the
    _FREE_BLOCK type, and fill them with the byte value 0xDD.
*/
extern void setHeapDelayedFree (bool bDelayedFree);

/** Perform automatic leak checking at program exit through a call to
    dumpMemoryLeaks() and generate an error report if the application
    failed to free all the memory it allocated.
*/
extern void setHeapReportLeaks (bool bReportLeaks);

/** Report all memory blocks which have not been freed.
*/
extern void reportLeaks ();

/** Confirms the integrity of the memory blocks allocated in the
    debug heap (debug version only.
*/
extern void checkHeap ();

}

}

#endif
