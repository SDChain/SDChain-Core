//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012, 2013 SDChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef SDCHAIN_NODESTORE_VISITCALLBACK_H_INCLUDED
#define SDCHAIN_NODESTORE_VISITCALLBACK_H_INCLUDED

namespace sdchain {
namespace NodeStore {

/** Callback for iterating through objects.

    @see visitAll
*/
// VFALCO DEPRECATED Use std::function instead
struct VisitCallback
{
    virtual void visitObject (NodeObject::Ptr const& object) = 0;
};

}
}

#endif
