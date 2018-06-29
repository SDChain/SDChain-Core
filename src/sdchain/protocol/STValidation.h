//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_STVALIDATION_H_INCLUDED
#define SDCHAIN_PROTOCOL_STVALIDATION_H_INCLUDED

#include <sdchain/protocol/PublicKey.h>
#include <sdchain/protocol/SecretKey.h>
#include <sdchain/protocol/STObject.h>
#include <cstdint>
#include <memory>

namespace sdchain {

// Validation flags
const std::uint32_t vfFullyCanonicalSig    = 0x80000000; // signature is fully canonical

class STValidation final
    : public STObject
    , public CountedObject <STValidation>
{
public:
    static char const* getCountedObjectName () { return "STValidation"; }

    using pointer = std::shared_ptr<STValidation>;
    using ref     = const std::shared_ptr<STValidation>&;

    enum
    {
        kFullFlag = 0x1
    };

    // These throw if the object is not valid
    STValidation (SerialIter & sit, bool checkSignature = true);

    // Does not sign the validation
    STValidation (
        uint256 const& ledgerHash,
        NetClock::time_point signTime,
        PublicKey const& raPub,
        bool isFull);

    STBase*
    copy (std::size_t n, void* buf) const override
    {
        return emplace(n, buf, *this);
    }

    STBase*
    move (std::size_t n, void* buf) override
    {
        return emplace(n, buf, std::move(*this));
    }

    uint256         getLedgerHash ()     const;
    NetClock::time_point getSignTime ()  const;
    NetClock::time_point getSeenTime ()  const;
    std::uint32_t   getFlags ()          const;
    PublicKey       getSignerPublic ()   const;
    NodeID          getNodeID ()         const
    {
        return mNodeID;
    }
    bool            isValid ()           const;
    bool            isFull ()            const;
    bool            isTrusted ()         const
    {
        return mTrusted;
    }
    uint256         getSigningHash ()    const;
    bool            isValid (uint256 const& ) const;

    void            setTrusted ()
    {
        mTrusted = true;
    }
    void            setSeen (NetClock::time_point s)
    {
        mSeen = s;
    }
    Blob    getSerialized ()             const;
    Blob    getSignature ()              const;

    // Signs the validation and returns the signing hash
    uint256 sign (SecretKey const& secretKey);

private:
    static SOTemplate const& getFormat ();

    void setNode ();

    NodeID mNodeID;
    bool mTrusted = false;
    NetClock::time_point mSeen = {};
};

} // sdchain

#endif
