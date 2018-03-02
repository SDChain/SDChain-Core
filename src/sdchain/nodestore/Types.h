//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_NODESTORE_TYPES_H_INCLUDED
#define SDCHAIN_NODESTORE_TYPES_H_INCLUDED

#include <sdchain/nodestore/NodeObject.h>
#include <sdchain/basics/BasicConfig.h>
#include <vector>

namespace sdchain {
namespace NodeStore {

enum
{
    // This is only used to pre-allocate the array for
    // batch objects and does not affect the amount written.
    //
    batchWritePreallocationSize = 128
};

/** Return codes from Backend operations. */
enum Status
{
    ok,
    notFound,
    dataCorrupt,
    unknown,

    customCode = 100
};

/** A batch of NodeObjects to write at once. */
using Batch = std::vector <std::shared_ptr<NodeObject>>;
}
}

#endif
