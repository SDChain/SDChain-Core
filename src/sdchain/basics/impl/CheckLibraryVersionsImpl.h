//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_BASICS_CHECKLIBRARYVERSIONSIMPL_H_INCLUDED
#define SDCHAIN_BASICS_CHECKLIBRARYVERSIONSIMPL_H_INCLUDED

#include <sdchain/basics/CheckLibraryVersions.h>

namespace sdchain {
namespace version {

/** Both Boost and OpenSSL have integral version numbers. */
using VersionNumber = unsigned long long;

std::string
boostVersion(VersionNumber boostVersion);

std::string
openSSLVersion(VersionNumber openSSLVersion);

void checkVersion(
    std::string name,
    std::string required,
    std::string actual);

void checkBoost(std::string version);
void checkOpenSSL(std::string version);

}  // namespace version
}  // namespace sdchain

#endif
