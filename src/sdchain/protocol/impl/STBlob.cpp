//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/protocol/STBlob.h>
#include <sdchain/basics/StringUtilities.h>

namespace sdchain {

STBlob::STBlob (SerialIter& st, SField const& name)
    : STBase (name)
    , value_ (st.getVLBuffer ())
{
}

std::string
STBlob::getText () const
{
    return strHex (value_.data (), value_.size ());
}

bool
STBlob::isEquivalent (const STBase& t) const
{
    const STBlob* v = dynamic_cast<const STBlob*> (&t);
    return v && (value_ == v->value_);
}

} // sdchain
