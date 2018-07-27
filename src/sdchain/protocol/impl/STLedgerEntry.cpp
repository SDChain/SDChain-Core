//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/basics/contract.h>
#include <sdchain/basics/Log.h>
#include <sdchain/json/to_string.h>
#include <sdchain/protocol/Indexes.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/protocol/STLedgerEntry.h>
#include <boost/format.hpp>

namespace sdchain {

STLedgerEntry::STLedgerEntry (Keylet const& k)
    :  STObject(sfLedgerEntry)
    , key_ (k.key)
    , type_ (k.type)
{
    auto const format =
        LedgerFormats::getInstance().findByType (type_);

    if (format == nullptr)
        Throw<std::runtime_error> ("invalid ledger entry type");

    set (format->elements);

    setFieldU16 (sfLedgerEntryType,
        static_cast <std::uint16_t> (type_));
}

STLedgerEntry::STLedgerEntry (
        SerialIter& sit,
        uint256 const& index)
    : STObject (sfLedgerEntry)
    , key_ (index)
{
    set (sit);
    setSLEType ();
}

STLedgerEntry::STLedgerEntry (
        STObject const& object,
        uint256 const& index)
    : STObject (object)
    , key_ (index)
{
    setSLEType ();
}

void STLedgerEntry::setSLEType ()
{
    auto format = LedgerFormats::getInstance().findByType (
        static_cast <LedgerEntryType> (
            getFieldU16 (sfLedgerEntryType)));

    if (format == nullptr)
        Throw<std::runtime_error> ("invalid ledger entry type");

    type_ = format->getType ();

    if (!setType (format->elements))
    {
        if (auto j = debugLog().error())
        {
            j << "Ledger entry not valid for type " << format->getName ();
            j << "Object: " << getJson (0);
        }

        Throw<std::runtime_error> ("ledger entry not valid for type");
    }
}

std::string STLedgerEntry::getFullText () const
{
    auto const format =
        LedgerFormats::getInstance().findByType (type_);

    if (format == nullptr)
        Throw<std::runtime_error> ("invalid ledger entry type");

    std::string ret = "\"";
    ret += to_string (key_);
    ret += "\" = { ";
    ret += format->getName ();
    ret += ", ";
    ret += STObject::getFullText ();
    ret += "}";
    return ret;
}

std::string STLedgerEntry::getText () const
{
    return str (boost::format ("{ %s, %s }")
                % to_string (key_)
                % STObject::getText ());
}

Json::Value STLedgerEntry::getJson (int options) const
{
    Json::Value ret (STObject::getJson (options));

    ret[jss::index] = to_string (key_);

    return ret;
}

bool STLedgerEntry::isThreadedType () const
{
    return getFieldIndex (sfPreviousTxnID) != -1;
}

bool STLedgerEntry::thread (
    uint256 const& txID,
    std::uint32_t ledgerSeq,
    uint256& prevTxID,
    std::uint32_t& prevLedgerID)
{
    uint256 oldPrevTxID = getFieldH256 (sfPreviousTxnID);

    JLOG (debugLog().info())
        << "Thread Tx:" << txID << " prev:" << oldPrevTxID;

    if (oldPrevTxID == txID)
    {
        // this transaction is already threaded
        assert (getFieldU32 (sfPreviousTxnLgrSeq) == ledgerSeq);
        return false;
    }

    prevTxID = oldPrevTxID;
    prevLedgerID = getFieldU32 (sfPreviousTxnLgrSeq);
    setFieldH256 (sfPreviousTxnID, txID);
    setFieldU32 (sfPreviousTxnLgrSeq, ledgerSeq);
    return true;
}

} // sdchain
