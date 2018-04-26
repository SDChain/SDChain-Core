//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/protocol/STAccount.h>

namespace sdchain {

STAccount::STAccount ()
    : STBase ()
    , value_ (beast::zero)
    , default_ (true)
{
}

STAccount::STAccount (SField const& n)
    : STBase (n)
    , value_ (beast::zero)
    , default_ (true)
{
}

STAccount::STAccount (SField const& n, Buffer&& v)
    : STAccount (n)
{
    if (v.empty())
        return;  // Zero is a valid size for a defaulted STAccount.

    // Is it safe to throw from this constructor?  Today (November 2015)
    // the only place that calls this constructor is
    //    STVar::STVar (SerialIter&, SField const&)
    // which throws.  If STVar can throw in its constructor, then so can
    // STAccount.
    if (v.size() != uint160::bytes)
        Throw<std::runtime_error> ("Invalid STAccount size");

    default_ = false;
    memcpy (value_.begin(), v.data (), uint160::bytes);
}

STAccount::STAccount (SerialIter& sit, SField const& name)
    : STAccount(name, sit.getVLBuffer())
{
}

STAccount::STAccount (SField const& n, AccountID const& v)
    : STBase (n)
    , default_ (false)
{
    value_.copyFrom (v);
}

std::string STAccount::getText () const
{
    if (isDefault())
        return "";
    return toBase58 (value());
}

} // sdchain
