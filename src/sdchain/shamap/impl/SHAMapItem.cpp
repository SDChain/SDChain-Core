//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/protocol/Serializer.h>
#include <sdchain/shamap/SHAMapItem.h>

namespace sdchain {

class SHAMap;

SHAMapItem::SHAMapItem (uint256 const& tag, Blob const& data)
    : tag_(tag)
    , data_(data)
{
}

SHAMapItem::SHAMapItem (uint256 const& tag, const Serializer& data)
    : tag_ (tag)
    , data_(data.peekData())
{
}

SHAMapItem::SHAMapItem (uint256 const& tag, Serializer&& data)
    : tag_ (tag)
    , data_(std::move(data.modData()))
{
}

} // sdchain
