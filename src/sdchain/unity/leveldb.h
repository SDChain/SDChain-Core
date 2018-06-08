//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012, 2013 SDChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef SDCHAIN_UNITY_LEVELDB_H_INCLUDED
#define SDCHAIN_UNITY_LEVELDB_H_INCLUDED

#ifndef SDCHAIN_LEVELDB_AVAILABLE
# define SDCHAIN_LEVELDB_AVAILABLE 1
#endif

#if SDCHAIN_LEVELDB_AVAILABLE

#define SNAPPY
#include "leveldb/cache.h"
#include "leveldb/filter_policy.h"
#include "leveldb/db.h"
#include "leveldb/write_batch.h"

#endif

#endif
