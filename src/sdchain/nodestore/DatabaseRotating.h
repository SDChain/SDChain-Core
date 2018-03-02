//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_NODESTORE_DATABASEROTATING_H_INCLUDED
#define SDCHAIN_NODESTORE_DATABASEROTATING_H_INCLUDED

#include <sdchain/nodestore/Database.h>

namespace sdchain {
namespace NodeStore {

/* This class has two key-value store Backend objects for persisting SHAMap
 * records. This facilitates online deletion of data. New backends are
 * rotated in. Old ones are rotated out and deleted.
 */

class DatabaseRotating
{
public:
    virtual ~DatabaseRotating() = default;

    virtual TaggedCache <uint256, NodeObject>& getPositiveCache() = 0;

    virtual std::mutex& peekMutex() const = 0;

    virtual std::shared_ptr <Backend> const& getWritableBackend() const = 0;

    virtual std::shared_ptr <Backend> const& getArchiveBackend () const = 0;

    virtual std::shared_ptr <Backend> rotateBackends (
            std::shared_ptr <Backend> const& newBackend) = 0;

    /** Ensure that node is in writableBackend */
    virtual std::shared_ptr<NodeObject> fetchNode (uint256 const& hash) = 0;
};

}
}

#endif
