//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/basics/contract.h>
#include <sdchain/basics/impl/CheckLibraryVersionsImpl.h>
#include <sdchain/beast/core/SemanticVersion.h>
#include <boost/version.hpp>
#include <openssl/opensslv.h>
#include <sstream>
#include <vector>

namespace sdchain {
namespace version {

std::string
boostVersion(VersionNumber boostVersion)
{
    std::stringstream ss;
    ss << (boostVersion / 100000) << "."
       << (boostVersion / 100 % 1000) << "."
       << (boostVersion % 100);
    return ss.str();
}

std::string
openSSLVersion(VersionNumber openSSLVersion)
{
    std::stringstream ss;
    ss << (openSSLVersion / 0x10000000L) << "."
       << (openSSLVersion / 0x100000 % 0x100) << "."
       << (openSSLVersion / 0x1000 % 0x100);
    auto patchNo = openSSLVersion % 0x10;
    if (patchNo)
        ss << '-' << char('a' + patchNo - 1);
    return ss.str();
}

void checkVersion(std::string name, std::string required, std::string actual)
{
    beast::SemanticVersion r, a;
    if (! r.parse(required))
    {
        Throw<std::runtime_error> (
            "Didn't understand required version of " + name + ": " + required);
    }

    if (! a.parse(actual))
    {
        Throw<std::runtime_error> (
            "Didn't understand actual version of " + name + ": " + required);
    }

    if (a < r)
    {
        Throw<std::runtime_error> (
            "Your " + name + " library is out of date.\n" + "Your version: " +
                actual + "\nRequired version: " + required + "\n");
    }
}

void checkBoost(std::string version)
{
    const char* boostMinimal = "1.57.0";
    checkVersion("Boost", boostMinimal, version);
}

void checkOpenSSL(std::string version)
{
    // The minimal version depends on whether we're linking
    // against 1.0.1 or later versions:
    beast::SemanticVersion v;

    char const* openSSLMinimal101 = "1.0.1-g";
    char const* openSSLMinimal102 = "1.0.2-j";

    if (v.parse (version) &&
            v.majorVersion == 1 &&
                v.minorVersion == 0 &&
                    v.patchVersion == 1)
    {
        // Use of the 1.0.1 series should be dropped as soon
        // as possible since as of January 2, 2017 it is no
        // longer supported. Unfortunately, a number of
        // platforms officially supported by SDChain still
        // use the 1.0.1 branch.
        //
        // Additionally, requiring 1.0.1u (the latest) is
        // similarly not possible, since those officially
        // supported platforms use older releases and
        // backport important fixes.
        checkVersion ("OpenSSL", openSSLMinimal101, version);
        return;
    }

    checkVersion ("OpenSSL", openSSLMinimal102, version);
}

void checkLibraryVersions()
{
    checkBoost(boostVersion(BOOST_VERSION));
    checkOpenSSL(openSSLVersion(OPENSSL_VERSION_NUMBER));
}

}  // namespace version
}  // namespace sdchain
