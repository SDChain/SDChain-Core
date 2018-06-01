//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_INDEXES_H_INCLUDED
#define SDCHAIN_PROTOCOL_INDEXES_H_INCLUDED

#include <sdchain/protocol/Keylet.h>
#include <sdchain/protocol/LedgerFormats.h>
#include <sdchain/protocol/Protocol.h>
#include <sdchain/protocol/PublicKey.h>
#include <sdchain/protocol/Serializer.h>
#include <sdchain/protocol/UintTypes.h>
#include <sdchain/basics/base_uint.h>
#include <sdchain/protocol/Book.h>
#include <cstdint>

namespace sdchain {

// get the index of the node that holds the last 256 ledgers
uint256
getLedgerHashIndex ();

// Get the index of the node that holds the set of 256 ledgers that includes
// this ledger's hash (or the first ledger after it if it's not a multiple
// of 256).
uint256
getLedgerHashIndex (std::uint32_t desiredLedgerIndex);

// get the index of the node that holds the enabled amendments
uint256
getLedgerAmendmentIndex ();

// get the index of the node that holds the fee schedule
uint256
getLedgerFeeIndex ();

uint256
getAccountRootIndex (AccountID const& account);

uint256
getGeneratorIndex (AccountID const& uGeneratorID);

uint256
getBookBase (Book const& book);

uint256
getOfferIndex (AccountID const& account, std::uint32_t uSequence);

uint256
getOwnerDirIndex (AccountID const& account);

uint256
getDirNodeIndex (uint256 const& uDirRoot, const std::uint64_t uNodeIndex);

uint256
getQualityIndex (uint256 const& uBase, const std::uint64_t uNodeDir = 0);

uint256
getQualityNext (uint256 const& uBase);

// VFALCO This name could be better
std::uint64_t
getQuality (uint256 const& uBase);

uint256
getTicketIndex (AccountID const& account, std::uint32_t uSequence);

uint256
getSDChainStateIndex (AccountID const& a, AccountID const& b, Currency const& currency);

uint256
getSDChainStateIndex (AccountID const& a, Issue const& issue);

uint256
getSignerListIndex (AccountID const& account);

//------------------------------------------------------------------------------

/* VFALCO TODO
    For each of these operators that take just the uin256 and
    only attach the LedgerEntryType, we can comment out that
    operator to see what breaks, and those call sites are
    candidates for having the Keylet either passed in a a
    parameter, or having a data member that stores the keylet.
*/

/** Keylet computation funclets. */
namespace keylet {

/** AccountID root */
struct account_t
{
    Keylet operator()(AccountID const& id) const;
};
static account_t const account {};

/** The amendment table */
struct amendments_t
{
    Keylet operator()() const;
};
static amendments_t const amendments {};

/** Any item that can be in an owner dir. */
Keylet child (uint256 const& key);

/** Skip list */
struct skip_t
{
    Keylet operator()() const;

    Keylet operator()(LedgerIndex ledger) const;
};
static skip_t const skip {};

/** The ledger fees */
struct fees_t
{
    // VFALCO This could maybe be constexpr
    Keylet operator()() const;
};
static fees_t const fees {};

/** The beginning of an order book */
struct book_t
{
    Keylet operator()(Book const& b) const;
};
static book_t const book {};

/** A trust line */
struct line_t
{
    Keylet operator()(AccountID const& id0,
        AccountID const& id1, Currency const& currency) const;

    Keylet operator()(AccountID const& id,
        Issue const& issue) const;

    Keylet operator()(uint256 const& key) const
    {
        return { ltSDCHAIN_STATE, key };
    }
};
static line_t const line {};

/** An offer from an account */
struct offer_t
{
    Keylet operator()(AccountID const& id,
        std::uint32_t seq) const;

    Keylet operator()(uint256 const& key) const
    {
        return { ltOFFER, key };
    }
};
static offer_t const offer {};

/** The initial directory page for a specific quality */
struct quality_t
{
    Keylet operator()(Keylet const& k,
        std::uint64_t q) const;
};
static quality_t const quality {};

/** The directry for the next lower quality */
struct next_t
{
    Keylet operator()(Keylet const& k) const;
};
static next_t const next {};

/** A ticket belonging to an account */
struct ticket_t
{
    Keylet operator()(AccountID const& id,
        std::uint32_t seq) const;

    Keylet operator()(uint256 const& key) const
    {
        return { ltTICKET, key };
    }
};
static ticket_t const ticket {};

/** A SignerList */
struct signers_t
{
    Keylet operator()(AccountID const& id) const;

    Keylet operator()(uint256 const& key) const
    {
        return { ltSIGNER_LIST, key };
    }
};
static signers_t const signers {};

//------------------------------------------------------------------------------

/** Any ledger entry */
Keylet unchecked(uint256 const& key);

/** The root page of an account's directory */
Keylet ownerDir (AccountID const& id);

/** A page in a directory */
/** @{ */
Keylet page (uint256 const& root, std::uint64_t index);
Keylet page (Keylet const& root, std::uint64_t index);
/** @} */

// DEPRECATED
inline
Keylet page (uint256 const& key)
{
    return { ltDIR_NODE, key };
}

/** An escrow entry */
Keylet
escrow (AccountID const& source, std::uint32_t seq);

/** A PaymentChannel */
Keylet
payChan (AccountID const& source, AccountID const& dst, std::uint32_t seq);

} // keylet

}

#endif
