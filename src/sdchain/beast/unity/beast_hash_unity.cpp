//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#include <sdchain/beast/hash/xxhasher.h>
#include <sdchain/beast/hash/impl/spookyv2.cpp>

#if ! BEAST_NO_XXHASH
#include <sdchain/beast/hash/impl/xxhash.c>
#endif
#include <sdchain/beast/hash/impl/siphash.cpp>
