//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2017 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_TX_INVARIANTCHECK_H_INCLUDED
#define SDCHAIN_APP_TX_INVARIANTCHECK_H_INCLUDED

#include <sdchain/basics/base_uint.h>
#include <sdchain/protocol/STLedgerEntry.h>
#include <sdchain/protocol/STTx.h>
#include <sdchain/protocol/TER.h>
#include <sdchain/beast/utility/Journal.h>
#include <tuple>
#include <cstdint>

namespace sdchain {

#if GENERATING_DOCS
/**
 * @brief Prototype for invariant check implementations.
 *
 * __THIS CLASS DOES NOT EXIST__ - or rather it exists in documentation only to
 * communicate the interface required of any invariant checker. Any invariant
 * check implementation should implement the public methods documented here.
 *
 */
class InvariantChecker_PROTOTYPE
{
public:

    /**
     * @brief called for each ledger entry in the current transaction.
     *
     * @param index the key (identifier) for the ledger entry
     * @param isDelete true if the SLE is being deleted
     * @param before ledger entry before modification by the transaction
     * @param after ledger entry after modification by the transaction
     */
    void
    visitEntry(
        uint256 const& index,
        bool isDelete,
        std::shared_ptr<SLE const> const& before,
        std::shared_ptr<SLE const> const& after);

    /**
     * @brief called after all ledger entries have been visited to determine
     * the final status of the check
     *
     * @param tx the transaction being applied
     * @param tec the current TER result of the transaction
     * @param j journal for logging
     *
     * @return true if check passes, false if it fails
     */
    bool
    finalize(
        STTx const& tx,
        TER tec,
        beast::Journal const& j);
};
#endif

/**
 * @brief Invariant: A transaction must not create SDA and should only destroy
 * SDA, up to the transaction fee.
 *
 * For this check, we start with a signed 64-bit integer set to zero. As we go
 * through the ledger entries, look only at account roots, escrow payments,
 * and payment channels.  Remove from the total any previous SDA values and add
 * to the total any new SDA values. The net balance of a payment channel is
 * computed from two fields (amount and balance) and deletions are ignored
 * for paychan and escrow because the amount fields have not been adjusted for
 * those in the case of deletion.
 *
 * The final total must be less than or equal to zero and greater than or equal
 * to the negative of the tx fee.
 *
 */
class SDANotCreated
{
    std::int64_t drops_ = 0;

public:

    void
    visitEntry(
        uint256 const&,
        bool,
        std::shared_ptr<SLE const> const&,
        std::shared_ptr<SLE const> const&);

    bool
    finalize(STTx const&, TER, beast::Journal const&);
};

/**
 * @brief Invariant: we cannot remove an account ledger entry
 *
 * an account root should never be the target of a delete
 */
class AccountRootsNotDeleted
{
    bool accountDeleted_ = false;

public:

    void
    visitEntry(
        uint256 const&,
        bool,
        std::shared_ptr<SLE const> const&,
        std::shared_ptr<SLE const> const&);

    bool
    finalize(STTx const&, TER, beast::Journal const&);
};

/**
 * @brief Invariant: An account SDA balance must be in SDA and take a value
                     between 0 and SYSTEM_CURRENCY_START drops, inclusive.
 */
class SDABalanceChecks
{
    bool bad_ = false;

public:
    void
    visitEntry(
        uint256 const&,
        bool,
        std::shared_ptr<SLE const> const&,
        std::shared_ptr<SLE const> const&);

    bool
    finalize(STTx const&, TER, beast::Journal const&);
};

/**
 * @brief Invariant: corresponding modified ledger entries should match in type
 *                   and added entries should be a valid type.
 */
class LedgerEntryTypesMatch
{
    bool typeMismatch_ = false;
    bool invalidTypeAdded_ = false;

public:

    void
    visitEntry(
        uint256 const&,
        bool,
        std::shared_ptr<SLE const> const&,
        std::shared_ptr<SLE const> const&);

    bool
    finalize(STTx const&, TER, beast::Journal const&);
};

/**
 * @brief Invariant: Trust lines using SDA are not allowed.
 */
class NoSDATrustLines
{
    bool sdaTrustLine_ = false;

public:

    void
    visitEntry(
        uint256 const&,
        bool,
        std::shared_ptr<SLE const> const&,
        std::shared_ptr<SLE const> const&);

    bool
    finalize(STTx const&, TER, beast::Journal const&);
};

/**
 * @brief Invariant: offers should be for non-negative amounts and must not
 *                   be SDA to SDA.
 */
class NoBadOffers
{
    bool bad_ = false;

public:

    void
    visitEntry(
        uint256 const&,
        bool,
        std::shared_ptr<SLE const> const&,
        std::shared_ptr<SLE const> const&);

    bool
    finalize(STTx const&, TER, beast::Journal const&);
    
};

/**
 * @brief Invariant: an escrow entry must take a value between 0 and
 *                   SYSTEM_CURRENCY_START drops exclusive.
 */
class NoZeroEscrow
{
    bool bad_ = false;

public:

    void
    visitEntry(
        uint256 const&,
        bool,
        std::shared_ptr<SLE const> const&,
        std::shared_ptr<SLE const> const&);

    bool
    finalize(STTx const&, TER, beast::Journal const&);
    
};

// additional invariant checks can be declared above and then added to this
// tuple
using InvariantChecks = std::tuple<
    AccountRootsNotDeleted,
    LedgerEntryTypesMatch,
    SDABalanceChecks,
    SDANotCreated,
    NoSDATrustLines,
    NoBadOffers,
    NoZeroEscrow
>;

/**
 * @brief get a tuple of all invariant checks
 *
 * @return std::tuple of instances that implement the required invariant check
 * methods
 *
 * @see sdchain::InvariantChecker_PROTOTYPE
 */
inline
InvariantChecks
getInvariantChecks()
{
    return InvariantChecks{};
}

} //sdchain

#endif
