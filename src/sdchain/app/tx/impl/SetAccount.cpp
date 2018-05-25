//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/tx/impl/SetAccount.h>
#include <sdchain/basics/Log.h>
#include <sdchain/core/Config.h>
#include <sdchain/protocol/Feature.h>
#include <sdchain/protocol/Indexes.h>
#include <sdchain/protocol/PublicKey.h>
#include <sdchain/protocol/Quality.h>
#include <sdchain/protocol/st.h>
#include <sdchain/ledger/View.h>

namespace sdchain {

bool
SetAccount::affectsSubsequentTransactionAuth(STTx const& tx)
{
    auto const uTxFlags = tx.getFlags();
    if(uTxFlags & (tfRequireAuth | tfOptionalAuth))
        return true;

    auto const uSetFlag = tx[~sfSetFlag];
    if(uSetFlag && (*uSetFlag == asfRequireAuth ||
        *uSetFlag == asfDisableMaster ||
            *uSetFlag == asfAccountTxnID))
                return true;

    auto const uClearFlag = tx[~sfClearFlag];
    return uClearFlag && (*uClearFlag == asfRequireAuth ||
        *uClearFlag == asfDisableMaster ||
            *uClearFlag == asfAccountTxnID);
}

TER
SetAccount::preflight (PreflightContext const& ctx)
{
    auto const ret = preflight1 (ctx);
    if (!isTesSuccess (ret))
        return ret;

    auto& tx = ctx.tx;
    auto& j = ctx.j;

    std::uint32_t const uTxFlags = tx.getFlags ();

    if (uTxFlags & tfAccountSetMask)
    {
        JLOG(j.trace()) << "Malformed transaction: Invalid flags set.";
        return temINVALID_FLAG;
    }

    std::uint32_t const uSetFlag = tx.getFieldU32 (sfSetFlag);
    std::uint32_t const uClearFlag = tx.getFieldU32 (sfClearFlag);

    if ((uSetFlag != 0) && (uSetFlag == uClearFlag))
    {
        JLOG(j.trace()) << "Malformed transaction: Set and clear same flag.";
        return temINVALID_FLAG;
    }

    //
    // RequireAuth
    //
    bool bSetRequireAuth   = (uTxFlags & tfRequireAuth) || (uSetFlag == asfRequireAuth);
    bool bClearRequireAuth = (uTxFlags & tfOptionalAuth) || (uClearFlag == asfRequireAuth);

    if (bSetRequireAuth && bClearRequireAuth)
    {
        JLOG(j.trace()) << "Malformed transaction: Contradictory flags set.";
        return temINVALID_FLAG;
    }

    //
    // RequireDestTag
    //
    bool bSetRequireDest   = (uTxFlags & TxFlag::requireDestTag) || (uSetFlag == asfRequireDest);
    bool bClearRequireDest = (uTxFlags & tfOptionalDestTag) || (uClearFlag == asfRequireDest);

    if (bSetRequireDest && bClearRequireDest)
    {
        JLOG(j.trace()) << "Malformed transaction: Contradictory flags set.";
        return temINVALID_FLAG;
    }

    //
    // DisallowSDA
    //
    bool bSetDisallowSDA   = (uTxFlags & tfDisallowSDA) || (uSetFlag == asfDisallowSDA);
    bool bClearDisallowSDA = (uTxFlags & tfAllowSDA) || (uClearFlag == asfDisallowSDA);

    if (bSetDisallowSDA && bClearDisallowSDA)
    {
        JLOG(j.trace()) << "Malformed transaction: Contradictory flags set.";
        return temINVALID_FLAG;
    }

    // TransferRate
    if (tx.isFieldPresent (sfTransferRate))
    {
        std::uint32_t uRate = tx.getFieldU32 (sfTransferRate);

        if (uRate && (uRate < QUALITY_ONE))
        {
            JLOG(j.trace()) << "Malformed transaction: Transfer rate too small.";
            return temBAD_TRANSFER_RATE;
        }

        if (ctx.rules.enabled(fix1201) && (uRate > 2 * QUALITY_ONE))
        {
            JLOG(j.trace()) << "Malformed transaction: Transfer rate too large.";
            return temBAD_TRANSFER_RATE;
        }
    }

    // TickSize
    if (tx.isFieldPresent (sfTickSize))
    {
        if (!ctx.rules.enabled(featureTickSize))
            return temDISABLED;

        auto uTickSize = tx[sfTickSize];
        if (uTickSize &&
            ((uTickSize < Quality::minTickSize) ||
            (uTickSize > Quality::maxTickSize)))
        {
            JLOG(j.trace()) << "Malformed transaction: Bad tick size.";
            return temBAD_TICK_SIZE;
        }
    }

    if (auto const mk = tx[~sfMessageKey])
    {
        if (mk->size() && ! publicKeyType ({mk->data(), mk->size()}))
        {
            JLOG(j.trace()) << "Invalid message key specified.";
            return telBAD_PUBLIC_KEY;
        }
    }

    auto const domain = tx[~sfDomain];
    if (domain && domain->size() > DOMAIN_BYTES_MAX)
    {
        JLOG(j.trace()) << "domain too long";
        return telBAD_DOMAIN;
    }

    return preflight2(ctx);
}

TER
SetAccount::preclaim(PreclaimContext const& ctx)
{
    auto const id = ctx.tx[sfAccount];

    std::uint32_t const uTxFlags = ctx.tx.getFlags();

    auto const sle = ctx.view.read(
        keylet::account(id));

    std::uint32_t const uFlagsIn = sle->getFieldU32(sfFlags);

    std::uint32_t const uSetFlag = ctx.tx.getFieldU32(sfSetFlag);

    // legacy AccountSet flags
    bool bSetRequireAuth = (uTxFlags & tfRequireAuth) || (uSetFlag == asfRequireAuth);

    //
    // RequireAuth
    //
    if (bSetRequireAuth && !(uFlagsIn & lsfRequireAuth))
    {
        if (!dirIsEmpty(ctx.view,
            keylet::ownerDir(id)))
        {
            JLOG(ctx.j.trace()) << "Retry: Owner directory not empty.";
            return (ctx.flags & tapRETRY) ? terOWNERS : tecOWNERS;
        }
    }

    return tesSUCCESS;
}

TER
SetAccount::doApply ()
{
    std::uint32_t const uTxFlags = ctx_.tx.getFlags ();

    auto const sle = view().peek(
        keylet::account(account_));

    std::uint32_t const uFlagsIn = sle->getFieldU32 (sfFlags);
    std::uint32_t uFlagsOut = uFlagsIn;

    std::uint32_t const uSetFlag = ctx_.tx.getFieldU32 (sfSetFlag);
    std::uint32_t const uClearFlag = ctx_.tx.getFieldU32 (sfClearFlag);

    // legacy AccountSet flags
    bool bSetRequireDest   = (uTxFlags & TxFlag::requireDestTag) || (uSetFlag == asfRequireDest);
    bool bClearRequireDest = (uTxFlags & tfOptionalDestTag) || (uClearFlag == asfRequireDest);
    bool bSetRequireAuth   = (uTxFlags & tfRequireAuth) || (uSetFlag == asfRequireAuth);
    bool bClearRequireAuth = (uTxFlags & tfOptionalAuth) || (uClearFlag == asfRequireAuth);
    bool bSetDisallowSDA   = (uTxFlags & tfDisallowSDA) || (uSetFlag == asfDisallowSDA);
    bool bClearDisallowSDA = (uTxFlags & tfAllowSDA) || (uClearFlag == asfDisallowSDA);

    bool sigWithMaster = false;

    {
        auto const spk = ctx_.tx.getSigningPubKey();

        if (publicKeyType (makeSlice (spk)))
        {
            PublicKey const signingPubKey (makeSlice (spk));

            if (calcAccountID(signingPubKey) == account_)
                sigWithMaster = true;
        }
    }

    //
    // RequireAuth
    //
    if (bSetRequireAuth && !(uFlagsIn & lsfRequireAuth))
    {
        JLOG(j_.trace()) << "Set RequireAuth.";
        uFlagsOut |= lsfRequireAuth;
    }

    if (bClearRequireAuth && (uFlagsIn & lsfRequireAuth))
    {
        JLOG(j_.trace()) << "Clear RequireAuth.";
        uFlagsOut &= ~lsfRequireAuth;
    }

    //
    // RequireDestTag
    //
    if (bSetRequireDest && !(uFlagsIn & lsfRequireDestTag))
    {
        JLOG(j_.trace()) << "Set lsfRequireDestTag.";
        uFlagsOut |= lsfRequireDestTag;
    }

    if (bClearRequireDest && (uFlagsIn & lsfRequireDestTag))
    {
        JLOG(j_.trace()) << "Clear lsfRequireDestTag.";
        uFlagsOut &= ~lsfRequireDestTag;
    }

    //
    // DisallowSDA
    //
    if (bSetDisallowSDA && !(uFlagsIn & lsfDisallowSDA))
    {
        JLOG(j_.trace()) << "Set lsfDisallowSDA.";
        uFlagsOut |= lsfDisallowSDA;
    }

    if (bClearDisallowSDA && (uFlagsIn & lsfDisallowSDA))
    {
        JLOG(j_.trace()) << "Clear lsfDisallowSDA.";
        uFlagsOut &= ~lsfDisallowSDA;
    }

    //
    // DisableMaster
    //
    if ((uSetFlag == asfDisableMaster) && !(uFlagsIn & lsfDisableMaster))
    {
        if (!sigWithMaster)
        {
            JLOG(j_.trace()) << "Must use master key to disable master key.";
            return tecNEED_MASTER_KEY;
        }

        if ((!sle->isFieldPresent (sfRegularKey)) &&
            (!view().peek (keylet::signers (account_))))
        {
            // Account has no regular key or multi-signer signer list.

            // Prevent transaction changes until we're ready.
            if (view().rules().enabled(featureMultiSign))
                return tecNO_ALTERNATIVE_KEY;

            return tecNO_REGULAR_KEY;
        }

        JLOG(j_.trace()) << "Set lsfDisableMaster.";
        uFlagsOut |= lsfDisableMaster;
    }

    if ((uClearFlag == asfDisableMaster) && (uFlagsIn & lsfDisableMaster))
    {
        JLOG(j_.trace()) << "Clear lsfDisableMaster.";
        uFlagsOut &= ~lsfDisableMaster;
    }

    //
    // DefaultSDChain
    //
    if (uSetFlag == asfDefaultSDChain)
    {
        uFlagsOut   |= lsfDefaultSDChain;
    }
    else if (uClearFlag == asfDefaultSDChain)
    {
        uFlagsOut   &= ~lsfDefaultSDChain;
    }

    //
    // NoFreeze
    //
    if (uSetFlag == asfNoFreeze)
    {
        if (!sigWithMaster && !(uFlagsIn & lsfDisableMaster))
        {
            JLOG(j_.trace()) << "Can't use regular key to set NoFreeze.";
            return tecNEED_MASTER_KEY;
        }

        JLOG(j_.trace()) << "Set NoFreeze flag";
        uFlagsOut |= lsfNoFreeze;
    }

    // Anyone may set global freeze
    if (uSetFlag == asfGlobalFreeze)
    {
        JLOG(j_.trace()) << "Set GlobalFreeze flag";
        uFlagsOut |= lsfGlobalFreeze;
    }

    // If you have set NoFreeze, you may not clear GlobalFreeze
    // This prevents those who have set NoFreeze from using
    // GlobalFreeze strategically.
    if ((uSetFlag != asfGlobalFreeze) && (uClearFlag == asfGlobalFreeze) &&
        ((uFlagsOut & lsfNoFreeze) == 0))
    {
        JLOG(j_.trace()) << "Clear GlobalFreeze flag";
        uFlagsOut &= ~lsfGlobalFreeze;
    }

    //
    // Track transaction IDs signed by this account in its root
    //
    if ((uSetFlag == asfAccountTxnID) && !sle->isFieldPresent (sfAccountTxnID))
    {
        JLOG(j_.trace()) << "Set AccountTxnID";
        sle->makeFieldPresent (sfAccountTxnID);
        }

    if ((uClearFlag == asfAccountTxnID) && sle->isFieldPresent (sfAccountTxnID))
    {
        JLOG(j_.trace()) << "Clear AccountTxnID";
        sle->makeFieldAbsent (sfAccountTxnID);
    }

    //
    // EmailHash
    //
    if (ctx_.tx.isFieldPresent (sfEmailHash))
    {
        uint128 const uHash = ctx_.tx.getFieldH128 (sfEmailHash);

        if (!uHash)
        {
            JLOG(j_.trace()) << "unset email hash";
            sle->makeFieldAbsent (sfEmailHash);
        }
        else
        {
            JLOG(j_.trace()) << "set email hash";
            sle->setFieldH128 (sfEmailHash, uHash);
        }
    }

    //
    // WalletLocator
    //
    if (ctx_.tx.isFieldPresent (sfWalletLocator))
    {
        uint256 const uHash = ctx_.tx.getFieldH256 (sfWalletLocator);

        if (!uHash)
        {
            JLOG(j_.trace()) << "unset wallet locator";
            sle->makeFieldAbsent (sfWalletLocator);
        }
        else
        {
            JLOG(j_.trace()) << "set wallet locator";
            sle->setFieldH256 (sfWalletLocator, uHash);
        }
    }

    //
    // MessageKey
    //
    if (ctx_.tx.isFieldPresent (sfMessageKey))
    {
        Blob const messageKey = ctx_.tx.getFieldVL (sfMessageKey);

        if (messageKey.empty ())
        {
            JLOG(j_.debug()) << "set message key";
            sle->makeFieldAbsent (sfMessageKey);
        }
        else
        {
            JLOG(j_.debug()) << "set message key";
            sle->setFieldVL (sfMessageKey, messageKey);
        }
    }

    //
    // Domain
    //
    if (ctx_.tx.isFieldPresent (sfDomain))
    {
        Blob const domain = ctx_.tx.getFieldVL (sfDomain);

        if (domain.empty ())
        {
            JLOG(j_.trace()) << "unset domain";
            sle->makeFieldAbsent (sfDomain);
        }
        else
        {
            JLOG(j_.trace()) << "set domain";
            sle->setFieldVL (sfDomain, domain);
        }
    }

    //
    // TransferRate
    //
    if (ctx_.tx.isFieldPresent (sfTransferRate))
    {
        std::uint32_t uRate = ctx_.tx.getFieldU32 (sfTransferRate);

        if (uRate == 0 || uRate == QUALITY_ONE)
        {
            JLOG(j_.trace()) << "unset transfer rate";
            sle->makeFieldAbsent (sfTransferRate);
        }
        else
        {
            JLOG(j_.trace()) << "set transfer rate";
            sle->setFieldU32 (sfTransferRate, uRate);
        }
    }

    //
    // TickSize
    //
    if (ctx_.tx.isFieldPresent (sfTickSize))
    {
        auto uTickSize = ctx_.tx[sfTickSize];
        if ((uTickSize == 0) || (uTickSize == Quality::maxTickSize))
        {
            JLOG(j_.trace()) << "unset tick size";
            sle->makeFieldAbsent (sfTickSize);
        }
        else
        {
            JLOG(j_.trace()) << "set tick size";
            sle->setFieldU8 (sfTickSize, uTickSize);
        }
    }

    if (uFlagsIn != uFlagsOut)
        sle->setFieldU32 (sfFlags, uFlagsOut);

    return tesSUCCESS;
}

}
