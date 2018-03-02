//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/basics/Slice.h>
#include <sdchain/basics/strHex.h>
#include <algorithm>

namespace sdchain {

int charUnHex (unsigned char c)
{
    struct HexTab
    {
        int hex[256];

        HexTab ()
        {
            std::fill (std::begin (hex), std::end (hex), -1);
            for (int i = 0; i < 10; ++i)
                hex ['0'+i] = i;
            for (int i = 0; i < 6; ++i)
            {
                hex ['A'+i] = 10 + i;
                hex ['a'+i] = 10 + i;
            }
        }
        int operator[] (unsigned char c) const
        {
            return hex[c];
        }
    };

    static HexTab xtab;

    return xtab[c];
}

std::string
strHex(Slice const& slice)
{
    return strHex(slice.data(), slice.size());
}

}
