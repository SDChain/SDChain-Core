//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_JSON_TO_STRING_H_INCLUDED
#define SDCHAIN_JSON_TO_STRING_H_INCLUDED

#include <string>
#include <ostream>

namespace Json {

class Value;

/** Writes a Json::Value to an std::string. */
std::string to_string (Value const&);

/** Writes a Json::Value to an std::string. */
std::string pretty (Value const&);

/** Output using the StyledStreamWriter. @see Json::operator>>(). */
std::ostream& operator<< (std::ostream&, const Value& root);

} // Json

#endif // JSON_TO_STRING_H_INCLUDED
