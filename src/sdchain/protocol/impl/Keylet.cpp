//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/protocol/Keylet.h>
#include <sdchain/protocol/STLedgerEntry.h>

namespace sdchain {

bool
Keylet::check (SLE const& sle) const
{
    if (type == ltANY)
        return true;
    if (type == ltINVALID)
        return false;
    if (type == ltCHILD)
    {
        assert(sle.getType() != ltDIR_NODE);
        return sle.getType() != ltDIR_NODE;
    }
    assert(sle.getType() == type);
    return sle.getType() == type;
}

} // sdchain
