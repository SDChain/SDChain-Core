//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_RESOURCE_FEES_H_INCLUDED
#define SDCHAIN_RESOURCE_FEES_H_INCLUDED

#include <sdchain/resource/Charge.h>

namespace sdchain {
namespace Resource {

/** Schedule of fees charged for imposing load on the server. */
/** @{ */
extern Charge const feeInvalidRequest;        // A request that we can immediately tell is invalid
extern Charge const feeRequestNoReply;        // A request that we cannot satisfy
extern Charge const feeInvalidSignature;      // An object whose signature we had to check and it failed
extern Charge const feeUnwantedData;          // Data we have no use for
extern Charge const feeBadData;               // Data we have to verify before rejecting

// RPC loads
extern Charge const feeInvalidRPC;            // An RPC request that we can immediately tell is invalid.
extern Charge const feeReferenceRPC;          // A default "reference" unspecified load
extern Charge const feeExceptionRPC;          // An RPC load that causes an exception
extern Charge const feeLightRPC;              // A normal RPC command
extern Charge const feeLowBurdenRPC;          // A slightly burdensome RPC load
extern Charge const feeMediumBurdenRPC;       // A somewhat burdensome RPC load
extern Charge const feeHighBurdenRPC;         // A very burdensome RPC load
extern Charge const feePathFindUpdate;        // An update to an existing PF request

// Peer loads
extern Charge const feeLightPeer;             // Requires no reply
extern Charge const feeLowBurdenPeer;         // Quick/cheap, slight reply
extern Charge const feeMediumBurdenPeer;      // Requires some work
extern Charge const feeHighBurdenPeer;        // Extensive work

// Good things
extern Charge const feeNewTrustedNote;        // A new transaction/validation/proposal we trust
extern Charge const feeNewValidTx;            // A new, valid transaction
extern Charge const feeSatisfiedRequest;      // Data we requested

// Administrative
extern Charge const feeWarning;               // The cost of receiving a warning
extern Charge const feeDrop;                  // The cost of being dropped for excess load
/** @} */

}
}

#endif
