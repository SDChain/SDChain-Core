//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2014 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_UINTTYPES_H_INCLUDED
#define SDCHAIN_PROTOCOL_UINTTYPES_H_INCLUDED

#include <sdchain/basics/UnorderedContainers.h>
#include <sdchain/basics/base_uint.h>
#include <sdchain/protocol/AccountID.h>

namespace sdchain {
namespace detail {

class CurrencyTag {};
class DirectoryTag {};
class NodeIDTag {};

} // detail

/** Directory is an index into the directory of offer books.
    The last 64 bits of this are the quality. */
using Directory = base_uint<256, detail::DirectoryTag>;

/** Currency is a hash representing a specific currency. */
using Currency = base_uint<160, detail::CurrencyTag>;

/** NodeID is a 160-bit hash representing one node. */
using NodeID = base_uint<160, detail::NodeIDTag>;

/** SDA currency. */
Currency const& sdaCurrency();

/** A placeholder for empty currencies. */
Currency const& noCurrency();

/** We deliberately disallow the currency that looks like "SDA" because too
    many people were using it instead of the correct SDA currency. */
Currency const& badCurrency();

inline bool isSDA(Currency const& c)
{
    return c == zero;
}

/** Returns "", "SDA", or three letter ISO code. */
std::string to_string(Currency const& c);

/** Tries to convert a string to a Currency, returns true on success. */
bool to_currency(Currency&, std::string const&);

/** Tries to convert a string to a Currency, returns noCurrency() on failure. */
Currency to_currency(std::string const&);

inline std::ostream& operator<< (std::ostream& os, Currency const& x)
{
    os << to_string (x);
    return os;
}

} // sdchain

namespace std {

template <>
struct hash <sdchain::Currency> : sdchain::Currency::hasher
{
};

template <>
struct hash <sdchain::NodeID> : sdchain::NodeID::hasher
{
};

template <>
struct hash <sdchain::Directory> : sdchain::Directory::hasher
{
};

} // std

#endif
