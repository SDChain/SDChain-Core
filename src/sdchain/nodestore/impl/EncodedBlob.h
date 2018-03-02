//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_NODESTORE_ENCODEDBLOB_H_INCLUDED
#define SDCHAIN_NODESTORE_ENCODEDBLOB_H_INCLUDED

#include <sdchain/basics/Buffer.h>
#include <sdchain/nodestore/NodeObject.h>
#include <cstddef>

namespace sdchain {
namespace NodeStore {

/** Utility for producing flattened node objects.
    @note This defines the database format of a NodeObject!
*/
// VFALCO TODO Make allocator aware and use short_alloc
struct EncodedBlob
{
public:
    void prepare (std::shared_ptr<NodeObject> const& object);

    void const* getKey () const noexcept
    {
        return m_key;
    }

    std::size_t getSize () const noexcept
    {
        return m_data.size();
    }

    void const* getData () const noexcept
    {
        return reinterpret_cast<void const*>(m_data.data());
    }

private:
    void const* m_key;
    Buffer m_data;
};

}
}

#endif
