//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_TX_IMPL_SIGNER_ENTRIES_H_INCLUDED
#define SDCHAIN_TX_IMPL_SIGNER_ENTRIES_H_INCLUDED

#include <sdchain/protocol/STTx.h>      // STTx::maxMultiSigners
#include <sdchain/protocol/UintTypes.h> // AccountID
#include <sdchain/protocol/TER.h>       // temMALFORMED
#include <sdchain/beast/utility/Journal.h>     // beast::Journal

namespace sdchain {

// Forward declarations
class STObject;

// Support for SignerEntries that is needed by a few Transactors
class SignerEntries
{
public:
    struct SignerEntry
    {
        AccountID account;
        std::uint16_t weight;

        SignerEntry (AccountID const& inAccount, std::uint16_t inWeight)
        : account (inAccount)
        , weight (inWeight)
        { }

        // For sorting to look for duplicate accounts
        friend bool operator< (SignerEntry const& lhs, SignerEntry const& rhs)
        {
            return lhs.account < rhs.account;
        }

        friend bool operator== (SignerEntry const& lhs, SignerEntry const& rhs)
        {
            return lhs.account == rhs.account;
        }
    };

    // Deserialize a SignerEntries array from the network or from the ledger.
    static
    std::pair<std::vector<SignerEntry>, TER>
    deserialize (
        STObject const& obj,
        beast::Journal journal,
        std::string const& annotation);
};

} // sdchain

#endif // SDCHAIN_TX_IMPL_SIGNER_ENTRIES_H_INCLUDED
