//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_BASICS_CHECKLIBRARYVERSIONS_H_INCLUDED
#define SDCHAIN_BASICS_CHECKLIBRARYVERSIONS_H_INCLUDED

#include <string>

namespace sdchain {
namespace version {

/** Check all library versions against SDChain's version requirements.

    Throws std::runtime_error if one or more libraries are out-of-date and do
    not meet the version requirements.
 */
void checkLibraryVersions();

}  // namespace version
}  // namespace sdchain

#endif
