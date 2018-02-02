//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_JSON_JSONPROPERTYSTREAM_H_INCLUDED
#define SDCHAIN_JSON_JSONPROPERTYSTREAM_H_INCLUDED

#include <sdchain/json/json_value.h>
#include <sdchain/beast/utility/PropertyStream.h>

namespace sdchain {

/** A PropertyStream::Sink which produces a Json::Value of type objectValue. */
class JsonPropertyStream : public beast::PropertyStream
{
public:
    Json::Value m_top;
    std::vector <Json::Value*> m_stack;

public:
    JsonPropertyStream ();
    Json::Value const& top() const;

protected:

    void map_begin ();
    void map_begin (std::string const& key);
    void map_end ();
    void add (std::string const& key, short value);
    void add (std::string const& key, unsigned short value);
    void add (std::string const& key, int value);
    void add (std::string const& key, unsigned int value);
    void add (std::string const& key, long value);
    void add (std::string const& key, float v);
    void add (std::string const& key, double v);
    void add (std::string const& key, std::string const& v);
    void array_begin ();
    void array_begin (std::string const& key);
    void array_end ();

    void add (short value);
    void add (unsigned short value);
    void add (int value);
    void add (unsigned int value);
    void add (long value);
    void add (float v);
    void add (double v);
    void add (std::string const& v);
};

}

#endif
