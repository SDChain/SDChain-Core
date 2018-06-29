//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_STTX_H_INCLUDED
#define SDCHAIN_PROTOCOL_STTX_H_INCLUDED

#include <sdchain/protocol/PublicKey.h>
#include <sdchain/protocol/SecretKey.h>
#include <sdchain/protocol/STObject.h>
#include <sdchain/protocol/TxFormats.h>
#include <boost/container/flat_set.hpp>
#include <boost/logic/tribool.hpp>
#include <functional>

namespace sdchain {

// VFALCO TODO replace these macros with language constants
#define TXN_SQL_NEW         'N'
#define TXN_SQL_CONFLICT    'C'
#define TXN_SQL_HELD        'H'
#define TXN_SQL_VALIDATED   'V'
#define TXN_SQL_INCLUDED    'I'
#define TXN_SQL_UNKNOWN     'U'

class STTx final
    : public STObject
    , public CountedObject <STTx>
{
public:
    static char const* getCountedObjectName () { return "STTx"; }

    static std::size_t const minMultiSigners = 1;
    static std::size_t const maxMultiSigners = 8;

public:
    STTx () = delete;
    STTx& operator= (STTx const& other) = delete;

    STTx (STTx const& other) = default;

    explicit STTx (SerialIter& sit);
    explicit STTx (SerialIter&& sit) : STTx(sit) {}

    explicit STTx (STObject&& object);

    /** Constructs a transaction.

        The returned transaction will have the specified type and
        any fields that the callback function adds to the object
        that's passed in.
    */
    STTx (
        TxType type,
        std::function<void(STObject&)> assembler);

    STBase*
    copy (std::size_t n, void* buf) const override
    {
        return emplace(n, buf, *this);
    }

    STBase*
    move (std::size_t n, void* buf) override
    {
        return emplace(n, buf, std::move(*this));
    }

    // STObject functions.
    SerializedTypeID getSType () const override
    {
        return STI_TRANSACTION;
    }
    std::string getFullText () const override;

    // Outer transaction functions / signature functions.
    Blob getSignature () const;

    uint256 getSigningHash () const;

    TxType getTxnType () const
    {
        return tx_type_;
    }

    Blob getSigningPubKey () const
    {
        return getFieldVL (sfSigningPubKey);
    }

    std::uint32_t getSequence () const
    {
        return getFieldU32 (sfSequence);
    }
    void setSequence (std::uint32_t seq)
    {
        return setFieldU32 (sfSequence, seq);
    }

    boost::container::flat_set<AccountID>
    getMentionedAccounts() const;

    uint256 getTransactionID () const
    {
        return tid_;
    }

    Json::Value getJson (int options) const override;
    Json::Value getJson (int options, bool binary) const;

    void sign (
        PublicKey const& publicKey,
        SecretKey const& secretKey);

    /** Check the signature.
        @return `true` if valid signature. If invalid, the error message string.
    */
    std::pair<bool, std::string>
    checkSign(bool allowMultiSign) const;

    // SQL Functions with metadata.
    static
    std::string const&
    getMetaSQLInsertReplaceHeader ();

    std::string getMetaSQL (
        std::uint32_t inLedger, std::string const& escapedMetaData) const;

    std::string getMetaSQL (
        Serializer rawTxn,
        std::uint32_t inLedger,
        char status,
        std::string const& escapedMetaData) const;

private:
    std::pair<bool, std::string> checkSingleSign () const;
    std::pair<bool, std::string> checkMultiSign () const;

    uint256 tid_;
    TxType tx_type_;
};

bool passesLocalChecks (STObject const& st, std::string&);

/** Sterilize a transaction.

    The transaction is serialized and then deserialized,
    ensuring that all equivalent transactions are in canonical
    form. This also ensures that program metadata such as
    the transaction's digest, are all computed.
*/
std::shared_ptr<STTx const>
sterilize (STTx const& stx);

/** Check whether a transaction is a pseudo-transaction */
bool isPseudoTx(STObject const& tx);

} // sdchain

#endif
