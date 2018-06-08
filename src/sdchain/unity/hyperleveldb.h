//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012, 2013 SDChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef SDCHAIN_UNITY_HYPERLEVELDB_H_INCLUDED
#define SDCHAIN_UNITY_HYPERLEVELDB_H_INCLUDED

#include <beast/Config.h>

#if ! BEAST_WIN32

#define SDCHAIN_HYPERLEVELDB_AVAILABLE 1

#include <hyperleveldb/hyperleveldb/cache.h>
#include <hyperleveldb/hyperleveldb/filter_policy.h>
#include <hyperleveldb/hyperleveldb/db.h>
#include <hyperleveldb/hyperleveldb/write_batch.h>

#else

#define SDCHAIN_HYPERLEVELDB_AVAILABLE 0

#endif

#endif
