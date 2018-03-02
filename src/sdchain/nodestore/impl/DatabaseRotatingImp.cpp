//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/nodestore/impl/DatabaseRotatingImp.h>

namespace sdchain {
namespace NodeStore {

// Make sure to call it already locked!
std::shared_ptr <Backend> DatabaseRotatingImp::rotateBackends (
        std::shared_ptr <Backend> const& newBackend)
{
    std::shared_ptr <Backend> oldBackend = archiveBackend_;
    archiveBackend_ = writableBackend_;
    writableBackend_ = newBackend;

    return oldBackend;
}

std::shared_ptr<NodeObject> DatabaseRotatingImp::fetchFrom (uint256 const& hash)
{
    Backends b = getBackends();
    std::shared_ptr<NodeObject> object = fetchInternal (*b.writableBackend, hash);
    if (!object)
    {
        object = fetchInternal (*b.archiveBackend, hash);
        if (object)
        {
            getWritableBackend()->store (object);
            m_negCache.erase (hash);
        }
    }

    return object;
}
}

}
