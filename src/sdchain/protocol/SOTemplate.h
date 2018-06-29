//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_SOTEMPLATE_H_INCLUDED
#define SDCHAIN_PROTOCOL_SOTEMPLATE_H_INCLUDED

#include <sdchain/protocol/SField.h>
#include <boost/range.hpp>
#include <memory>

namespace sdchain {

/** Flags for elements in a SOTemplate. */
// VFALCO NOTE these don't look like bit-flags...
enum SOE_Flags
{
    SOE_INVALID  = -1,
    SOE_REQUIRED = 0,   // required
    SOE_OPTIONAL = 1,   // optional, may be present with default value
    SOE_DEFAULT  = 2,   // optional, if present, must not have default value
};

//------------------------------------------------------------------------------

/** An element in a SOTemplate. */
class SOElement
{
public:
    SField const&     e_field;
    SOE_Flags const   flags;

    SOElement (SField const& fieldName, SOE_Flags flags)
        : e_field (fieldName)
        , flags (flags)
    {
    }
};

//------------------------------------------------------------------------------

/** Defines the fields and their attributes within a STObject.
    Each subclass of SerializedObject will provide its own template
    describing the available fields and their metadata attributes.
*/
class SOTemplate
{
public:
    using list_type = std::vector <std::unique_ptr <SOElement const>>;
    using iterator_range = boost::iterator_range<list_type::const_iterator>;

    /** Create an empty template.
        After creating the template, call @ref push_back with the
        desired fields.
        @see push_back
    */
    SOTemplate () = default;

    SOTemplate(SOTemplate&& other)
        : mTypes(std::move(other.mTypes))
        , mIndex(std::move(other.mIndex))
    {
    }

    /* Provide for the enumeration of fields */
    iterator_range all () const
    {
        return boost::make_iterator_range(mTypes);
    }

    /** The number of entries in this template */
    std::size_t size () const
    {
        return mTypes.size ();
    }

    /** Add an element to the template. */
    void push_back (SOElement const& r);

    /** Retrieve the position of a named field. */
    int getIndex (SField const&) const;

    SOE_Flags
    style(SField const& sf) const
    {
        return mTypes[mIndex[sf.getNum()]]->flags;
    }

private:
    list_type mTypes;

    std::vector <int> mIndex;       // field num -> index
};

} // sdchain

#endif
