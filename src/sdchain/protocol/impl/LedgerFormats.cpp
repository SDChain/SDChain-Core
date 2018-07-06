//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/protocol/LedgerFormats.h>
#include <sdchain/protocol/ErrorCodes.h>
#include <sdchain/protocol/JsonFields.h>
#include <algorithm>
#include <array>
#include <utility>

namespace sdchain {

LedgerFormats::LedgerFormats ()
{
    add ("AccountRoot", ltACCOUNT_ROOT)
            << SOElement (sfAccount,             SOE_REQUIRED)
            << SOElement (sfSequence,            SOE_REQUIRED)
            << SOElement (sfBalance,             SOE_REQUIRED)
            << SOElement (sfOwnerCount,          SOE_REQUIRED)
            << SOElement (sfPreviousTxnID,       SOE_REQUIRED)
            << SOElement (sfPreviousTxnLgrSeq,   SOE_REQUIRED)
            << SOElement (sfAccountTxnID,        SOE_OPTIONAL)
            << SOElement (sfRegularKey,          SOE_OPTIONAL)
            << SOElement (sfEmailHash,           SOE_OPTIONAL)
            << SOElement (sfWalletLocator,       SOE_OPTIONAL)
            << SOElement (sfWalletSize,          SOE_OPTIONAL)
            << SOElement (sfMessageKey,          SOE_OPTIONAL)
            << SOElement (sfTransferRate,        SOE_OPTIONAL)
            << SOElement (sfDomain,              SOE_OPTIONAL)
            << SOElement (sfTickSize,            SOE_OPTIONAL)
            ;

    add ("DirectoryNode", ltDIR_NODE)
            << SOElement (sfOwner,               SOE_OPTIONAL)  // for owner directories
            << SOElement (sfTakerPaysCurrency,   SOE_OPTIONAL)  // for order book directories
            << SOElement (sfTakerPaysIssuer,     SOE_OPTIONAL)  // for order book directories
            << SOElement (sfTakerGetsCurrency,   SOE_OPTIONAL)  // for order book directories
            << SOElement (sfTakerGetsIssuer,     SOE_OPTIONAL)  // for order book directories
            << SOElement (sfExchangeRate,        SOE_OPTIONAL)  // for order book directories
            << SOElement (sfIndexes,             SOE_REQUIRED)
            << SOElement (sfRootIndex,           SOE_REQUIRED)
            << SOElement (sfIndexNext,           SOE_OPTIONAL)
            << SOElement (sfIndexPrevious,       SOE_OPTIONAL)
            ;

    add ("Offer", ltOFFER)
            << SOElement (sfAccount,             SOE_REQUIRED)
            << SOElement (sfSequence,            SOE_REQUIRED)
            << SOElement (sfTakerPays,           SOE_REQUIRED)
            << SOElement (sfTakerGets,           SOE_REQUIRED)
            << SOElement (sfBookDirectory,       SOE_REQUIRED)
            << SOElement (sfBookNode,            SOE_REQUIRED)
            << SOElement (sfOwnerNode,           SOE_REQUIRED)
            << SOElement (sfPreviousTxnID,       SOE_REQUIRED)
            << SOElement (sfPreviousTxnLgrSeq,   SOE_REQUIRED)
            << SOElement (sfExpiration,          SOE_OPTIONAL)
            ;

    add ("SDChainState", ltSDCHAIN_STATE)
            << SOElement (sfBalance,             SOE_REQUIRED)
            << SOElement (sfLowLimit,            SOE_REQUIRED)
            << SOElement (sfHighLimit,           SOE_REQUIRED)
            << SOElement (sfPreviousTxnID,       SOE_REQUIRED)
            << SOElement (sfPreviousTxnLgrSeq,   SOE_REQUIRED)
            << SOElement (sfLowNode,             SOE_OPTIONAL)
            << SOElement (sfLowQualityIn,        SOE_OPTIONAL)
            << SOElement (sfLowQualityOut,       SOE_OPTIONAL)
            << SOElement (sfHighNode,            SOE_OPTIONAL)
            << SOElement (sfHighQualityIn,       SOE_OPTIONAL)
            << SOElement (sfHighQualityOut,      SOE_OPTIONAL)
            ;

    add ("Escrow", ltESCROW) <<
        SOElement (sfAccount,           SOE_REQUIRED) <<
        SOElement (sfDestination,       SOE_REQUIRED) <<
        SOElement (sfAmount,            SOE_REQUIRED) <<
        SOElement (sfCondition,         SOE_OPTIONAL) <<
        SOElement (sfCancelAfter,       SOE_OPTIONAL) <<
        SOElement (sfFinishAfter,       SOE_OPTIONAL) <<
        SOElement (sfSourceTag,         SOE_OPTIONAL) <<
        SOElement (sfDestinationTag,    SOE_OPTIONAL) <<
        SOElement (sfOwnerNode,         SOE_REQUIRED) <<
        SOElement (sfPreviousTxnID,     SOE_REQUIRED) <<
        SOElement (sfPreviousTxnLgrSeq, SOE_REQUIRED) <<
        SOElement (sfDestinationNode,   SOE_OPTIONAL);

    add ("LedgerHashes", ltLEDGER_HASHES)
            << SOElement (sfFirstLedgerSequence, SOE_OPTIONAL) // Remove if we do a ledger restart
            << SOElement (sfLastLedgerSequence,  SOE_OPTIONAL)
            << SOElement (sfHashes,              SOE_REQUIRED)
            ;

    add ("Amendments", ltAMENDMENTS)
            << SOElement (sfAmendments,          SOE_OPTIONAL) // Enabled
            << SOElement (sfMajorities,          SOE_OPTIONAL)
            ;

    add ("FeeSettings", ltFEE_SETTINGS)
            << SOElement (sfBaseFee,             SOE_REQUIRED)
            << SOElement (sfReferenceFeeUnits,   SOE_REQUIRED)
            << SOElement (sfReserveBase,         SOE_REQUIRED)
            << SOElement (sfReserveIncrement,    SOE_REQUIRED)
            ;

    add ("Ticket", ltTICKET)
            << SOElement (sfAccount,             SOE_REQUIRED)
            << SOElement (sfSequence,            SOE_REQUIRED)
            << SOElement (sfOwnerNode,           SOE_REQUIRED)
            << SOElement (sfTarget,              SOE_OPTIONAL)
            << SOElement (sfExpiration,          SOE_OPTIONAL)
            ;

    // All fields are SOE_REQUIRED because there is always a
    // SignerEntries.  If there are no SignerEntries the node is deleted.
    add ("SignerList", ltSIGNER_LIST)
            << SOElement (sfOwnerNode,           SOE_REQUIRED)
            << SOElement (sfSignerQuorum,        SOE_REQUIRED)
            << SOElement (sfSignerEntries,       SOE_REQUIRED)
            << SOElement (sfSignerListID,        SOE_REQUIRED)
            << SOElement (sfPreviousTxnID,       SOE_REQUIRED)
            << SOElement (sfPreviousTxnLgrSeq,   SOE_REQUIRED)
            ;

    add ("PayChannel", ltPAYCHAN)
            << SOElement (sfAccount,           SOE_REQUIRED)
            << SOElement (sfDestination,       SOE_REQUIRED)
            << SOElement (sfAmount,            SOE_REQUIRED)
            << SOElement (sfBalance,           SOE_REQUIRED)
            << SOElement (sfPublicKey,         SOE_REQUIRED)
            << SOElement (sfSettleDelay,       SOE_REQUIRED)
            << SOElement (sfExpiration,        SOE_OPTIONAL)
            << SOElement (sfCancelAfter,       SOE_OPTIONAL)
            << SOElement (sfSourceTag,         SOE_OPTIONAL)
            << SOElement (sfDestinationTag,    SOE_OPTIONAL)
            << SOElement (sfOwnerNode,         SOE_REQUIRED)
            << SOElement (sfPreviousTxnID,     SOE_REQUIRED)
            << SOElement (sfPreviousTxnLgrSeq, SOE_REQUIRED)
            ;
}

void LedgerFormats::addCommonFields (Item& item)
{
    item
        << SOElement(sfLedgerIndex,             SOE_OPTIONAL)
        << SOElement(sfLedgerEntryType,         SOE_REQUIRED)
        << SOElement(sfFlags,                   SOE_REQUIRED)
        ;
}

LedgerFormats const&
LedgerFormats::getInstance ()
{
    static LedgerFormats instance;
    return instance;
}

} // sdchain
