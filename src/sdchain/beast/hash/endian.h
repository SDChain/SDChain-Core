//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2014, Howard Hinnant <howard.hinnant@gmail.com>,
        Vinnie Falco <vinnie.falco@gmail.com



*/
//==============================================================================

#ifndef BEAST_HASH_ENDIAN_H_INCLUDED
#define BEAST_HASH_ENDIAN_H_INCLUDED

namespace beast {

// endian provides answers to the following questions:
// 1.  Is this system big or little endian?
// 2.  Is the "desired endian" of some class or function the same as the
//     native endian?
enum class endian
{
#ifdef _MSC_VER
    big    = 1,
    little = 0,
    native = little
#else
    native = __BYTE_ORDER__,
    little = __ORDER_LITTLE_ENDIAN__,
    big    = __ORDER_BIG_ENDIAN__
#endif
};

#ifndef __INTELLISENSE__
static_assert(endian::native == endian::little ||
              endian::native == endian::big,
              "endian::native shall be one of endian::little or endian::big");

static_assert(endian::big != endian::little,
              "endian::big and endian::little shall have different values");
#endif

} // beast

#endif
