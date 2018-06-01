//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_INNER_OBJECT_FORMATS_H_INCLUDED
#define SDCHAIN_PROTOCOL_INNER_OBJECT_FORMATS_H_INCLUDED

#include <sdchain/protocol/KnownFormats.h>

namespace sdchain {

/** Manages the list of known inner object formats.
*/
class InnerObjectFormats : public KnownFormats <int>
{
private:
    void addCommonFields (Item& item);

public:
    /** Create the object.
        This will load the object will all the known inner object formats.
    */
    InnerObjectFormats ();

    static InnerObjectFormats const& getInstance ();

    SOTemplate const* findSOTemplateBySField (SField const& sField) const;
};

} // sdchain

#endif
