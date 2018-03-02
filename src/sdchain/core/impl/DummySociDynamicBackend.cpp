//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2015 SDChain Alliance.



*/
//==============================================================================

/*  Stub functions for soci dynamic backends.

    SDChain does not use dynamic backends, and inclduing soci's
    dynamic backends compilcates the build (it requires a generated
    header file and some macros to be defined.)
*/

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated"
#endif

#include <BeastConfig.h>
#include <sdchain/basics/contract.h>
#include <sdchain/core/SociDB.h>
#include <soci/sqlite3/soci-sqlite3.h>

// dummy soci-backend
namespace soci {
namespace dynamic_backends {
// used internally by session
backend_factory const& get (std::string const& name)
{
    sdchain::Throw<std::runtime_error> ("Not Supported");
    return std::ref(soci::sqlite3); // Silence compiler warning.
};

// provided for advanced user-level management
std::vector<std::string>& search_paths ()
{
    static std::vector<std::string> empty;
    return empty;
};
void register_backend (std::string const&, std::string const&){};
void register_backend (std::string const&, backend_factory const&){};
std::vector<std::string> list_all ()
{
    return {};
};
void unload (std::string const&){};
void unload_all (){};

}  // namespace dynamic_backends
}  // namespace soci

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
