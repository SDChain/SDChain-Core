//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_SYSTEMPARAMETERS_H_INCLUDED
#define SDCHAIN_PROTOCOL_SYSTEMPARAMETERS_H_INCLUDED

#include <cstdint>
#include <string>

namespace sdchain {

// Various protocol and system specific constant globals.

/* The name of the system. */
static inline
std::string const&
systemName ()
{
    static std::string const name = "sdchain";
    return name;
}

/** Configure the native currency. */
static
std::uint64_t const
SYSTEM_CURRENCY_GIFT = 1000;

static
std::uint64_t const
SYSTEM_CURRENCY_USERS = 100000000;

/** Number of drops per 1 SDA */
static
std::uint64_t const
SYSTEM_CURRENCY_PARTS = 1000000;

/** Number of drops in the genesis account. */
static
std::uint64_t const
SYSTEM_CURRENCY_START = SYSTEM_CURRENCY_GIFT * SYSTEM_CURRENCY_USERS * SYSTEM_CURRENCY_PARTS;

/* The currency code for the native currency. */
static inline
std::string const&
systemCurrencyCode ()
{
    static std::string const code = "SDA";
    return code;
}

} // sdchain

#endif
