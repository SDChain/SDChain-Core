//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/protocol/SOTemplate.h>

namespace sdchain {

void SOTemplate::push_back (SOElement const& r)
{
    // Ensure there is the enough space in the index mapping
    // table for all possible fields.
    //
    if (mIndex.empty ())
    {
        // Unmapped indices will be set to -1
        //
        mIndex.resize (SField::getNumFields () + 1, -1);
    }

    // Make sure the field's index is in range
    //
    assert (r.e_field.getNum () < mIndex.size ());

    // Make sure that this field hasn't already been assigned
    //
    assert (getIndex (r.e_field) == -1);

    // Add the field to the index mapping table
    //
    mIndex [r.e_field.getNum ()] = mTypes.size ();

    // Append the new element.
    //
    mTypes.push_back (std::make_unique<SOElement const> (r));
}

int SOTemplate::getIndex (SField const& f) const
{
    // The mapping table should be large enough for any possible field
    //
    assert (f.getNum () < mIndex.size ());

    return mIndex[f.getNum ()];
}

} // sdchain
