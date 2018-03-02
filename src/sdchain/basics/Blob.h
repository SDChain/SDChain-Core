//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_BASICS_BLOB_H_INCLUDED
#define SDCHAIN_BASICS_BLOB_H_INCLUDED

#include <vector>

namespace sdchain {

/** Storage for linear binary data.
    Blocks of binary data appear often in various idioms and structures.
*/
using Blob = std::vector <unsigned char>;

}

#endif
