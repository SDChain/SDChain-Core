//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#if BEAST_INCLUDE_BEASTCONFIG
#include "../../BeastConfig.h"
#endif

#include <sdchain/beast/net/IPAddressV6.h>

namespace beast {
namespace IP {

//------------------------------------------------------------------------------

bool is_loopback (AddressV6 const&)
{
    // VFALCO TODO
    assert(false);
    return false;
}

bool is_unspecified (AddressV6 const&)
{
    // VFALCO TODO
    assert(false);
    return false;
}

bool is_multicast (AddressV6 const&)
{
    // VFALCO TODO
    assert(false);
    return false;
}

bool is_private (AddressV6 const&)
{
    // VFALCO TODO
    assert(false);
    return false;
}

bool is_public (AddressV6 const&)
{
    // VFALCO TODO
    assert(false);
    return false;
}

//------------------------------------------------------------------------------

std::string to_string (AddressV6 const&)
{
    // VFALCO TODO
    assert(false);
    return "";
}

std::istream& operator>> (std::istream& is, AddressV6&)
{
    // VFALCO TODO
    assert(false);
    return is;
}

}
}
