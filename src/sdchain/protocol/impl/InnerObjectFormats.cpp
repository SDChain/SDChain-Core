//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/protocol/InnerObjectFormats.h>

namespace sdchain {

InnerObjectFormats::InnerObjectFormats ()
{
    add (sfSignerEntry.getJsonName ().c_str (), sfSignerEntry.getCode ())
        << SOElement (sfAccount,              SOE_REQUIRED)
        << SOElement (sfSignerWeight,         SOE_REQUIRED)
        ;

    add (sfSigner.getJsonName ().c_str (), sfSigner.getCode ())
        << SOElement (sfAccount,              SOE_REQUIRED)
        << SOElement (sfSigningPubKey,        SOE_REQUIRED)
        << SOElement (sfTxnSignature,         SOE_REQUIRED)
        ;
}

void InnerObjectFormats::addCommonFields (Item& item)
{
}

InnerObjectFormats const&
InnerObjectFormats::getInstance ()
{
    static InnerObjectFormats instance;
    return instance;
}

SOTemplate const*
InnerObjectFormats::findSOTemplateBySField (SField const& sField) const
{
    SOTemplate const* ret = nullptr;
    auto itemPtr = findByType (sField.getCode ());
    if (itemPtr)
        ret = &(itemPtr->elements);

    return ret;
}

} // sdchain
