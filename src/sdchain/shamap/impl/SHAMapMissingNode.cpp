//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/shamap/SHAMapMissingNode.h>
#include <ostream>

namespace sdchain {

std::ostream&
operator<< (std::ostream& out, const SHAMapMissingNode& mn)
{
    switch (mn.mType)
    {
    case SHAMapType::TRANSACTION:
        out << "Missing/TXN(";
        break;

    case SHAMapType::STATE:
        out << "Missing/STA(";
        break;

    case SHAMapType::FREE:
    default:
        out << "Missing/(";
        break;
    };

    if (mn.mNodeHash == zero)
        out << "id : " << mn.mNodeID;
    else
        out << "hash : " << mn.mNodeHash;
    out << ")";
    return out;
}

} // sdchain
