//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_NODESTORE_FACTORY_H_INCLUDED
#define SDCHAIN_NODESTORE_FACTORY_H_INCLUDED

#include <sdchain/nodestore/Backend.h>
#include <sdchain/nodestore/Scheduler.h>
#include <sdchain/beast/utility/Journal.h>

namespace sdchain {
namespace NodeStore {

/** Base class for backend factories. */
class Factory
{
public:
    virtual
    ~Factory() = default;

    /** Retrieve the name of this factory. */
    virtual
    std::string
    getName() const = 0;

    /** Create an instance of this factory's backend.

        @param keyBytes The fixed number of bytes per key.
        @param keyValues A set of key/value configuration pairs.
        @param scheduler The scheduler to use for running tasks.
        @return A pointer to the Backend object.
    */
    virtual
    std::unique_ptr <Backend>
    createInstance (size_t keyBytes, Section const& parameters,
        Scheduler& scheduler, beast::Journal journal) = 0;
};

}
}

#endif
