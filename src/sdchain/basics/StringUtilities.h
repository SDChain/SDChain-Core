//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_BASICS_STRINGUTILITIES_H_INCLUDED
#define SDCHAIN_BASICS_STRINGUTILITIES_H_INCLUDED

#include <sdchain/basics/ByteOrder.h>
#include <sdchain/basics/Blob.h>
#include <sdchain/basics/strHex.h>
#include <boost/format.hpp>
#include <sstream>
#include <string>

namespace sdchain {

// NIKB TODO Remove the need for all these overloads. Move them out of here.
inline const std::string strHex (std::string const& strSrc)
{
    return strHex (strSrc.begin (), strSrc.size ());
}

inline std::string strHex (Blob const& vucData)
{
    return strHex (vucData.begin (), vucData.size ());
}

inline std::string strHex (const std::uint64_t uiHost)
{
    uint64_t    uBig    = htobe64 (uiHost);

    return strHex ((unsigned char*) &uBig, sizeof (uBig));
}

inline static std::string sqlEscape (std::string const& strSrc)
{
    static boost::format f ("X'%s'");
    return str (boost::format (f) % strHex (strSrc));
}

inline static std::string sqlEscape (Blob const& vecSrc)
{
    size_t size = vecSrc.size ();

    if (size == 0)
        return "X''";

    std::string j (size * 2 + 3, 0);

    unsigned char* oPtr = reinterpret_cast<unsigned char*> (&*j.begin ());
    const unsigned char* iPtr = &vecSrc[0];

    *oPtr++ = 'X';
    *oPtr++ = '\'';

    for (int i = size; i != 0; --i)
    {
        unsigned char c = *iPtr++;
        *oPtr++ = charHex (c >> 4);
        *oPtr++ = charHex (c & 15);
    }

    *oPtr++ = '\'';
    return j;
}

uint64_t uintFromHex (std::string const& strSrc);

std::pair<Blob, bool> strUnHex (std::string const& strSrc);

struct parsedURL
{
    std::string scheme;
    std::string domain;
    boost::optional<std::uint16_t> port;
    std::string path;
};

bool parseUrl (parsedURL& pUrl, std::string const& strUrl);

std::string trim_whitespace (std::string str);

} // sdchain

#endif
