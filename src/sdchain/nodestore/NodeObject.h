//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_NODESTORE_NODEOBJECT_H_INCLUDED
#define SDCHAIN_NODESTORE_NODEOBJECT_H_INCLUDED

#include <sdchain/basics/CountedObject.h>
#include <sdchain/protocol/Protocol.h>

// VFALCO NOTE Intentionally not in the NodeStore namespace

namespace sdchain {

/** The types of node objects. */
enum NodeObjectType
{
    hotUNKNOWN = 0,
    hotLEDGER = 1,
    //hotTRANSACTION = 2        // Not used
    hotACCOUNT_NODE = 3,
    hotTRANSACTION_NODE = 4
};

/** A simple object that the Ledger uses to store entries.
    NodeObjects are comprised of a type, a hash, a ledger index and a blob.
    They can be uniquely identified by the hash, which is a half-SHA512 of
    the blob. The blob is a variable length block of serialized data. The
    type identifies what the blob contains.

    @note No checking is performed to make sure the hash matches the data.
    @see SHAMap
*/
class NodeObject : public CountedObject <NodeObject>
{
public:
    static char const* getCountedObjectName () { return "NodeObject"; }

    enum
    {
        /** Size of the fixed keys, in bytes.

            We use a 256-bit hash for the keys.

            @see NodeObject
        */
        keyBytes = 32,
    };

private:
    // This hack is used to make the constructor effectively private
    // except for when we use it in the call to make_shared.
    // There's no portable way to make make_shared<> a friend work.
    struct PrivateAccess { };
public:
    // This constructor is private, use createObject instead.
    NodeObject (NodeObjectType type,
                Blob&& data,
                uint256 const& hash,
                PrivateAccess);

    /** Create an object from fields.

        The caller's variable is modified during this call. The
        underlying storage for the Blob is taken over by the NodeObject.

        @param type The type of object.
        @param ledgerIndex The ledger in which this object appears.
        @param data A buffer containing the payload. The caller's variable
                    is overwritten.
        @param hash The 256-bit hash of the payload data.
    */
    static
    std::shared_ptr<NodeObject>
    createObject (NodeObjectType type,
        Blob&& data, uint256 const& hash);

    /** Returns the type of this object. */
    NodeObjectType getType () const;

    /** Returns the hash of the data. */
    uint256 const& getHash () const;

    /** Returns the underlying data. */
    Blob const& getData () const;

private:
    NodeObjectType mType;
    uint256 mHash;
    Blob mData;
};

}

#endif
