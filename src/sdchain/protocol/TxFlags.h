//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_TXFLAGS_H_INCLUDED
#define SDCHAIN_PROTOCOL_TXFLAGS_H_INCLUDED

#include <cstdint>

namespace sdchain {

//
// Transaction flags.
//

/** Transaction flags.

    These flags modify the behavior of an operation.

    @note Changing these will create a hard fork
    @ingroup protocol
*/
class TxFlag
{
public:
    static std::uint32_t const requireDestTag = 0x00010000;
};
// VFALCO TODO Move all flags into this container after some study.

// Universal Transaction flags:
const std::uint32_t tfFullyCanonicalSig    = 0x80000000;
const std::uint32_t tfUniversal            = tfFullyCanonicalSig;
const std::uint32_t tfUniversalMask        = ~ tfUniversal;

// AccountSet flags:
// VFALCO TODO Javadoc comment every one of these constants
//const std::uint32_t TxFlag::requireDestTag       = 0x00010000;
const std::uint32_t tfOptionalDestTag      = 0x00020000;
const std::uint32_t tfRequireAuth          = 0x00040000;
const std::uint32_t tfOptionalAuth         = 0x00080000;
const std::uint32_t tfDisallowSDA          = 0x00100000;
const std::uint32_t tfAllowSDA             = 0x00200000;
const std::uint32_t tfAccountSetMask       = ~ (tfUniversal | TxFlag::requireDestTag | tfOptionalDestTag
                                             | tfRequireAuth | tfOptionalAuth
                                             | tfDisallowSDA | tfAllowSDA);

// AccountSet SetFlag/ClearFlag values
const std::uint32_t asfRequireDest         = 1;
const std::uint32_t asfRequireAuth         = 2;
const std::uint32_t asfDisallowSDA         = 3;
const std::uint32_t asfDisableMaster       = 4;
const std::uint32_t asfAccountTxnID        = 5;
const std::uint32_t asfNoFreeze            = 6;
const std::uint32_t asfGlobalFreeze        = 7;
const std::uint32_t asfDefaultSDChain       = 8;

// OfferCreate flags:
const std::uint32_t tfPassive              = 0x00010000;
const std::uint32_t tfImmediateOrCancel    = 0x00020000;
const std::uint32_t tfFillOrKill           = 0x00040000;
const std::uint32_t tfSell                 = 0x00080000;
const std::uint32_t tfOfferCreateMask      = ~ (tfUniversal | tfPassive | tfImmediateOrCancel | tfFillOrKill | tfSell);

// Payment flags:
const std::uint32_t tfNoSDChainDirect       = 0x00010000;
const std::uint32_t tfPartialPayment       = 0x00020000;
const std::uint32_t tfLimitQuality         = 0x00040000;
const std::uint32_t tfPaymentMask          = ~ (tfUniversal | tfPartialPayment | tfLimitQuality | tfNoSDChainDirect);

// TrustSet flags:
const std::uint32_t tfSetfAuth             = 0x00010000;
const std::uint32_t tfSetNoSDChain          = 0x00020000;
const std::uint32_t tfClearNoSDChain        = 0x00040000;
const std::uint32_t tfSetFreeze            = 0x00100000;
const std::uint32_t tfClearFreeze          = 0x00200000;
const std::uint32_t tfTrustSetMask         = ~ (tfUniversal | tfSetfAuth | tfSetNoSDChain | tfClearNoSDChain
                                             | tfSetFreeze | tfClearFreeze);

// EnableAmendment flags:
const std::uint32_t tfGotMajority          = 0x00010000;
const std::uint32_t tfLostMajority         = 0x00020000;

// PaymentChannel flags:
const std::uint32_t tfRenew                = 0x00010000;
const std::uint32_t tfClose                = 0x00020000;

} // sdchain

#endif
