//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/beast/core/Config.h>

#ifdef _MSC_VER
#include <cstddef>
namespace snappy {
using ssize_t = std::ptrdiff_t;
}
#endif

#if BEAST_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wdeprecated"
#endif

#include <snappy/snappy/snappy.cc>
#include <snappy/snappy/snappy-sinksource.cc>
#include <snappy/snappy/snappy-stubs-internal.cc>

#if BEAST_CLANG
#pragma clang diagnostic pop
#endif
