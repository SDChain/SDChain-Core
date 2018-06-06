//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/tx/impl/SetRegularKey.h>
#include <sdchain/basics/Log.h>
#include <sdchain/protocol/TxFlags.h>
#include <sdchain/protocol/types.h>

namespace sdchain {

std::uint64_t
SetRegularKey::calculateBaseFee (
    PreclaimContext const& ctx)
{
    auto const id = ctx.tx.getAccountID(sfAccount);
    auto const spk = ctx.tx.getSigningPubKey();

    if (publicKeyType (makeSlice (spk)))
    {
        if (calcAccountID(PublicKey (makeSlice(spk))) == id)
        {
            auto const sle = ctx.view.read(keylet::account(id));

            if (sle && (! (sle->getFlags () & lsfPasswordSpent)))
            {
                // flag is armed and they signed with the right account
                return 0;
            }
        }
    }

    return Transactor::calculateBaseFee (ctx);
}

TER
SetRegularKey::preflight (PreflightContext const& ctx)
{
    auto const ret = preflight1 (ctx);
    if (!isTesSuccess (ret))
        return ret;

    std::uint32_t const uTxFlags = ctx.tx.getFlags ();

    if (uTxFlags & tfUniversalMask)
    {
        JLOG(ctx.j.trace()) <<
            "Malformed transaction: Invalid flags set.";

        return temINVALID_FLAG;
    }

    return preflight2(ctx);
}

TER
SetRegularKey::doApply ()
{
    auto const sle = view().peek(
        keylet::account(account_));

    if (mFeeDue == zero)
        sle->setFlag (lsfPasswordSpent);

    if (ctx_.tx.isFieldPresent (sfRegularKey))
    {
        sle->setAccountID (sfRegularKey,
            ctx_.tx.getAccountID (sfRegularKey));
    }
    else
    {
        if (sle->isFlag (lsfDisableMaster) &&
            !view().peek (keylet::signers (account_)))
            // Account has disabled master key and no multi-signer signer list.
            return tecNO_ALTERNATIVE_KEY;

        sle->makeFieldAbsent (sfRegularKey);
    }

    return tesSUCCESS;
}

}
