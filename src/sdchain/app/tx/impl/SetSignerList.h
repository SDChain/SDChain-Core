//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2014 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_TX_SETSIGNERLIST_H_INCLUDED
#define SDCHAIN_TX_SETSIGNERLIST_H_INCLUDED

#include <sdchain/app/ledger/Ledger.h>
#include <sdchain/app/tx/impl/Transactor.h>
#include <sdchain/app/tx/impl/SignerEntries.h>
#include <sdchain/protocol/STObject.h>
#include <sdchain/protocol/STArray.h>
#include <sdchain/protocol/STTx.h>
#include <sdchain/protocol/Indexes.h>
#include <sdchain/basics/Log.h>
#include <algorithm>
#include <cstdint>
#include <vector>

namespace sdchain {

/**
See the README.md for an overview of the SetSignerList transaction that
this class implements.
*/
class SetSignerList : public Transactor
{
private:
    // Values determined during preCompute for use later.
    enum Operation {unknown, set, destroy};
    Operation do_ {unknown};
    std::uint32_t quorum_ {0};
    std::vector<SignerEntries::SignerEntry> signers_;

public:
    SetSignerList (ApplyContext& ctx)
        : Transactor(ctx)
    {
    }

    static
    bool
    affectsSubsequentTransactionAuth(STTx const& tx)
    {
        return true;
    }

    static
    TER
    preflight (PreflightContext const& ctx);

    TER doApply () override;
    void preCompute() override;

private:
    static
    std::tuple<TER, std::uint32_t,
        std::vector<SignerEntries::SignerEntry>,
            Operation>
    determineOperation(STTx const& tx,
        ApplyFlags flags, beast::Journal j);

    static
    TER validateQuorumAndSignerEntries (
        std::uint32_t quorum,
            std::vector<SignerEntries::SignerEntry> const& signers,
                AccountID const& account,
                    beast::Journal j);

    TER replaceSignerList ();
    TER destroySignerList ();

    TER removeSignersFromLedger (Keylet const& accountKeylet,
        Keylet const& ownerDirKeylet, Keylet const& signerListKeylet);
    void writeSignersToSLE (SLE::pointer const& ledgerEntry) const;

    static int ownerCountDelta (std::size_t entryCount);
};

} // sdchain

#endif
