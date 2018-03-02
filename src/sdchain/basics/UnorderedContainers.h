//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_BASICS_UNORDEREDCONTAINERS_H_INCLUDED
#define SDCHAIN_BASICS_UNORDEREDCONTAINERS_H_INCLUDED

#include <sdchain/basics/hardened_hash.h>
#include <sdchain/beast/hash/hash_append.h>
#include <sdchain/beast/hash/uhash.h>
#include <sdchain/beast/hash/xxhasher.h>
#include <unordered_map>
#include <unordered_set>

/**
* Use hash_* containers for keys that do not need a cryptographically secure
* hashing algorithm.
*
* Use hardened_hash_* containers for keys that do need a secure hashing algorithm.
*
* The cryptographic security of containers where a hash function is used as a
* template parameter depends entirely on that hash function and not at all on
* what container it is.
*/

namespace sdchain
{

// hash containers

template <class Key, class Value, class Hash = beast::uhash<>,
          class Pred = std::equal_to<Key>,
          class Allocator = std::allocator<std::pair<Key const, Value>>>
using hash_map = std::unordered_map <Key, Value, Hash, Pred, Allocator>;

template <class Key, class Value, class Hash = beast::uhash<>,
          class Pred = std::equal_to<Key>,
          class Allocator = std::allocator<std::pair<Key const, Value>>>
using hash_multimap = std::unordered_multimap <Key, Value, Hash, Pred, Allocator>;

template <class Value, class Hash = beast::uhash<>,
          class Pred = std::equal_to<Value>,
          class Allocator = std::allocator<Value>>
using hash_set = std::unordered_set <Value, Hash, Pred, Allocator>;

template <class Value, class Hash = beast::uhash<>,
          class Pred = std::equal_to<Value>,
          class Allocator = std::allocator<Value>>
using hash_multiset = std::unordered_multiset <Value, Hash, Pred, Allocator>;

// hardened_hash containers

using strong_hash = beast::xxhasher;

template <class Key, class Value, class Hash = hardened_hash<strong_hash>,
          class Pred = std::equal_to<Key>,
          class Allocator = std::allocator<std::pair<Key const, Value>>>
using hardened_hash_map = std::unordered_map <Key, Value, Hash, Pred, Allocator>;

template <class Key, class Value, class Hash = hardened_hash<strong_hash>,
          class Pred = std::equal_to<Key>,
          class Allocator = std::allocator<std::pair<Key const, Value>>>
using hardened_hash_multimap = std::unordered_multimap <Key, Value, Hash, Pred, Allocator>;

template <class Value, class Hash = hardened_hash<strong_hash>,
          class Pred = std::equal_to<Value>,
          class Allocator = std::allocator<Value>>
using hardened_hash_set = std::unordered_set <Value, Hash, Pred, Allocator>;

template <class Value, class Hash = hardened_hash<strong_hash>,
          class Pred = std::equal_to<Value>,
          class Allocator = std::allocator<Value>>
using hardened_hash_multiset = std::unordered_multiset <Value, Hash, Pred, Allocator>;

} // sdchain

#endif
