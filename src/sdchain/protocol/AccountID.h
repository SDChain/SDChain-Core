//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2014 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_ACCOUNTID_H_INCLUDED
#define SDCHAIN_PROTOCOL_ACCOUNTID_H_INCLUDED

#include <sdchain/protocol/tokens.h>
// VFALCO Uncomment when the header issues are resolved
//#include <sdchain/protocol/PublicKey.h>
#include <sdchain/basics/base_uint.h>
#include <sdchain/basics/UnorderedContainers.h>
#include <sdchain/json/json_value.h>
#include <boost/optional.hpp>
#include <cstddef>
#include <mutex>
#include <string>

namespace sdchain {

namespace detail {

class AccountIDTag { };

} // detail

/** A 160-bit unsigned that uniquely identifies an account. */
using AccountID = base_uint<
    160, detail::AccountIDTag>;

/** Convert AccountID to base58 checked string */
std::string
toBase58 (AccountID const& v);

/** Parse AccountID from checked, base58 string.
    @return boost::none if a parse error occurs
*/
template<>
boost::optional<AccountID>
parseBase58 (std::string const& s);

// Parses AccountID using Bitcoin's alphabet
// This is to catch user error. Likely not needed
// DEPRECATED
boost::optional<AccountID>
deprecatedParseBitcoinAccountID (std::string const& s);

// Compatibility with legacy code
bool
deprecatedParseBase58 (AccountID& account,
    Json::Value const& jv);

/** Parse AccountID from hexadecimal string

    If the string is not exactly 40
    hex digits, boost::none is returned.

    @return boost::none if a parse error occurs
*/
template<>
boost::optional<AccountID>
parseHex (std::string const& s);

/** Parse AccountID from hex or checked base58 string.

    @return boost::none if a parse error occurs
*/
template<>
boost::optional<AccountID>
parseHexOrBase58 (std::string const& s);

/** Compute AccountID from public key.

    The account ID is computed as the 160-bit hash of the
    public key data. This excludes the version byte and
    guard bytes included in the base58 representation.

*/
// VFALCO In PublicKey.h for now
//AccountID
//calcAccountID (PublicKey const& pk);

/** A special account that's used as the "issuer" for SDA. */
AccountID const&
sdaAccount();

/** A placeholder for empty accounts. */
AccountID const&
noAccount();

/** Convert hex or base58 string to AccountID.

    @return `true` if the parsing was successful.
*/
// DEPRECATED
bool
to_issuer (AccountID&, std::string const&);

// DEPRECATED Should be checking the currency or native flag
inline
bool
isSDA(AccountID const& c)
{
    return c == zero;
}

// DEPRECATED
inline
std::string
to_string (AccountID const& account)
{
    return toBase58(account);
}

// DEPRECATED
inline std::ostream& operator<< (std::ostream& os, AccountID const& x)
{
    os << to_string (x);
    return os;
}

//------------------------------------------------------------------------------

/** Caches the base58 representations of AccountIDs

    This operation occurs with sufficient frequency to
    justify having a cache. In the future, sdchaind should
    require clients to receive "binary" results, where
    AccountIDs are hex-encoded.
*/
class AccountIDCache
{
private:
    std::mutex mutable mutex_;
    std::size_t capacity_;
    hash_map<AccountID,
        std::string> mutable m0_;
    hash_map<AccountID,
        std::string> mutable m1_;

public:
    AccountIDCache(AccountIDCache const&) = delete;
    AccountIDCache& operator= (AccountIDCache const&) = delete;

    explicit
    AccountIDCache (std::size_t capacity);

    /** Return sdchain::toBase58 for the AccountID

        Thread Safety:
            Safe to call from any thread concurrently

        @note This function intentionally returns a
              copy for correctness.
    */
    std::string
    toBase58 (AccountID const&) const;
};

} // sdchain

//------------------------------------------------------------------------------

namespace std {

// DEPRECATED
// VFALCO Use beast::uhash or a hardened container
template <>
struct hash <sdchain::AccountID> : sdchain::AccountID::hasher
{
};

} // std

#endif
