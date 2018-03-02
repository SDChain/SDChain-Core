//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2016 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_CONDITIONS_ERROR_H
#define SDCHAIN_CONDITIONS_ERROR_H

#include <system_error>
#include <string>

namespace sdchain {
namespace cryptoconditions {

enum class error
{
    generic = 1,
    unsupported_type,
    unsupported_subtype,
    unknown_type,
    unknown_subtype,
    fingerprint_size,
    incorrect_encoding,
    trailing_garbage,
    buffer_empty,
    buffer_overfull,
    buffer_underfull,
    malformed_encoding,
    short_preamble,
    unexpected_tag,
    long_tag,
    large_size,
    preimage_too_long
};

std::error_code
make_error_code(error ev);

} // cryptoconditions
} // sdchain

namespace std
{

template<>
struct is_error_code_enum<sdchain::cryptoconditions::error>
{
    static bool const value = true;
};

} // std

#endif
