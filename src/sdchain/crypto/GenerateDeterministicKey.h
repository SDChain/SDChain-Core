//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2011 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file license.txt or http://www.opensource.org/licenses/mit-license.php.

#ifndef SDCHAIN_CRYPTO_GENERATEDETERMINISTICKEY_H_INCLUDED
#define SDCHAIN_CRYPTO_GENERATEDETERMINISTICKEY_H_INCLUDED

#include <sdchain/basics/base_uint.h>

namespace sdchain {

Blob
generateRootDeterministicPublicKey (
    uint128 const& seed);

uint256
generateRootDeterministicPrivateKey (
    uint128 const& seed);

Blob
generatePublicDeterministicKey (
    Blob const& generator,
    int n);

uint256
generatePrivateDeterministicKey (
    Blob const& family,
    uint128 const& seed,
    int n);

} // sdchain

#endif
