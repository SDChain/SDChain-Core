//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2015 SDChain Alliance.



*/
//==============================================================================

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated"
#endif

#include <BeastConfig.h>

// Core soci
#include <core/common.cpp>
#include <core/connection-parameters.cpp>
#include <core/connection-pool.cpp>
#include <core/error.cpp>
#include <core/into-type.cpp>
#include <core/once-temp-type.cpp>
#include <core/prepare-temp-type.cpp>
#include <core/procedure.cpp>
#include <core/ref-counted-prepare-info.cpp>
#include <core/ref-counted-statement.cpp>
#include <core/row.cpp>
#include <core/rowid.cpp>
#include <core/session.cpp>
#include <core/soci-simple.cpp>
#include <core/statement.cpp>
#include <core/transaction.cpp>
#include <core/use-type.cpp>
#include <core/values.cpp>

#include <backends/sqlite3/error.cpp>
#include <backends/sqlite3/factory.cpp>
#include <backends/sqlite3/row-id.cpp>
#include <backends/sqlite3/session.cpp>
#include <backends/sqlite3/standard-into-type.cpp>
#include <backends/sqlite3/standard-use-type.cpp>
#include <backends/sqlite3/statement.cpp>
#include <backends/sqlite3/vector-into-type.cpp>
#include <backends/sqlite3/vector-use-type.cpp>

#include <core/blob.cpp>
#include <backends/sqlite3/blob.cpp>

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
