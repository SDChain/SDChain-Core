//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/protocol/STBase.h>
#include <boost/checked_delete.hpp>
#include <cassert>
#include <memory>

namespace sdchain {

STBase::STBase()
    : fName(&sfGeneric)
{
}

STBase::STBase (SField const& n)
    : fName(&n)
{
    assert(fName);
}

STBase&
STBase::operator= (const STBase& t)
{
    if ((t.fName != fName) && fName->isUseful() && t.fName->isUseful())
    {
        // VFALCO We shouldn't be logging at this low a level
        /*
        WriteLog ((t.getSType () == STI_AMOUNT) ? lsTRACE : lsWARNING, STBase) // This is common for amounts
                << "Caution: " << t.fName->getName () << " not replacing " << fName->getName ();
        */
    }
    if (!fName->isUseful())
        fName = t.fName;
    return *this;
}

bool
STBase::operator== (const STBase& t) const
{
    return (getSType () == t.getSType ()) && isEquivalent (t);
}

bool
STBase::operator!= (const STBase& t) const
{
    return (getSType () != t.getSType ()) || !isEquivalent (t);
}

SerializedTypeID
STBase::getSType() const
{
    return STI_NOTPRESENT;
}

std::string
STBase::getFullText() const
{
    std::string ret;

    if (getSType () != STI_NOTPRESENT)
    {
        if (fName->hasName ())
        {
            ret = fName->fieldName;
            ret += " = ";
        }

        ret += getText ();
    }

    return ret;
}

std::string
STBase::getText() const
{
    return std::string();
}

Json::Value
STBase::getJson (int /*options*/) const
{
    return getText();
}

void
STBase::add (Serializer& s) const
{
    // Should never be called
    assert(false);
}

bool
STBase::isEquivalent (const STBase& t) const
{
    assert (getSType () == STI_NOTPRESENT);
    if (t.getSType () == STI_NOTPRESENT)
        return true;
    // VFALCO We shouldn't be logging at this low a level
    //WriteLog (lsDEBUG, STBase) << "notEquiv " << getFullText() << " not STI_NOTPRESENT";
    return false;
}

bool
STBase::isDefault() const
{
    return true;
}

void
STBase::setFName (SField const& n)
{
    fName = &n;
    assert (fName);
}

SField const&
STBase::getFName() const
{
    return *fName;
}

void
STBase::addFieldID (Serializer& s) const
{
    assert (fName->isBinary ());
    s.addFieldID (fName->fieldType, fName->fieldValue);
}

//------------------------------------------------------------------------------

std::ostream&
operator<< (std::ostream& out, const STBase& t)
{
    return out << t.getFullText ();
}

} // sdchain
