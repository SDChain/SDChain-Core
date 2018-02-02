//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2014, Howard Hinnant <howard.hinnant@gmail.com>,
        Vinnie Falco <vinnie.falco@gmail.com



*/
//==============================================================================

#ifndef BEAST_HASH_SIPHASH_H_INCLUDED
#define BEAST_HASH_SIPHASH_H_INCLUDED

#include <cstddef>
#include <cstdint>

namespace beast {

// See https://131002.net/siphash/
class siphash
{
private:
    std::uint64_t v0_ = 0x736f6d6570736575ULL;
    std::uint64_t v1_ = 0x646f72616e646f6dULL;
    std::uint64_t v2_ = 0x6c7967656e657261ULL;
    std::uint64_t v3_ = 0x7465646279746573ULL;
    unsigned char buf_[8];
    unsigned bufsize_ = 0;
    unsigned total_length_ = 0;

public:
    using result_type = std::size_t;

    static beast::endian const endian = beast::endian::native;

    siphash() = default;

    explicit
    siphash (std::uint64_t k0, std::uint64_t k1 = 0) noexcept;

    void
    operator() (void const* key, std::size_t len) noexcept;

    explicit
    operator std::size_t() noexcept;
};

} // beast

#endif
