//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/tx/impl/SignerEntries.h>
#include <sdchain/basics/Log.h>
#include <sdchain/protocol/STObject.h>
#include <sdchain/protocol/STArray.h>
#include <cstdint>

namespace sdchain {

std::pair<std::vector<SignerEntries::SignerEntry>, TER>
SignerEntries::deserialize (
    STObject const& obj, beast::Journal journal, std::string const& annotation)
{
    std::pair<std::vector<SignerEntry>, TER> s;

    if (!obj.isFieldPresent (sfSignerEntries))
    {
        JLOG(journal.trace()) <<
            "Malformed " << annotation << ": Need signer entry array.";
        s.second = temMALFORMED;
        return s;
    }

    auto& accountVec = s.first;
    accountVec.reserve (STTx::maxMultiSigners);

    STArray const& sEntries (obj.getFieldArray (sfSignerEntries));
    for (STObject const& sEntry : sEntries)
    {
        // Validate the SignerEntry.
        if (sEntry.getFName () != sfSignerEntry)
        {
            JLOG(journal.trace()) <<
                "Malformed " << annotation << ": Expected SignerEntry.";
            s.second = temMALFORMED;
            return s;
        }

        // Extract SignerEntry fields.
        AccountID const account = sEntry.getAccountID (sfAccount);
        std::uint16_t const weight = sEntry.getFieldU16 (sfSignerWeight);
        accountVec.emplace_back (account, weight);
    }

    s.second = tesSUCCESS;
    return s;
}

} // sdchain
