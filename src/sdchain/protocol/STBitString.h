//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_STBITSTRING_H_INCLUDED
#define SDCHAIN_PROTOCOL_STBITSTRING_H_INCLUDED

#include <sdchain/protocol/STBase.h>

namespace sdchain {

template <std::size_t Bits>
class STBitString final
    : public STBase
{
public:
    using value_type = base_uint<Bits>;

    STBitString () = default;

    STBitString (SField const& n)
        : STBase (n)
    { }

    STBitString (const value_type& v)
        : value_ (v)
    { }

    STBitString (SField const& n, const value_type& v)
        : STBase (n), value_ (v)
    { }

    STBitString (SField const& n, const char* v)
        : STBase (n)
    {
        value_.SetHex (v);
    }

    STBitString (SField const& n, std::string const& v)
        : STBase (n)
    {
        value_.SetHex (v);
    }

    STBitString (SerialIter& sit, SField const& name)
        : STBitString(name, sit.getBitString<Bits>())
    {
    }

    STBase*
    copy (std::size_t n, void* buf) const override
    {
        return emplace(n, buf, *this);
    }

    STBase*
    move (std::size_t n, void* buf) override
    {
        return emplace(n, buf, std::move(*this));
    }

    SerializedTypeID
    getSType () const override;

    std::string
    getText () const override
    {
        return to_string (value_);
    }

    bool
    isEquivalent (const STBase& t) const override
    {
        const STBitString* v = dynamic_cast<const STBitString*> (&t);
        return v && (value_ == v->value_);
    }

    void
    add (Serializer& s) const override
    {
        assert (fName->isBinary ());
        assert (fName->fieldType == getSType());
        s.addBitString<Bits> (value_);
    }

    template <typename Tag>
    void setValue (base_uint<Bits, Tag> const& v)
    {
        value_.copyFrom(v);
    }

    value_type const&
    value() const
    {
        return value_;
    }

    operator value_type () const
    {
        return value_;
    }

    bool
    isDefault () const override
    {
        return value_ == zero;
    }

private:
    value_type value_;
};

using STHash128 = STBitString<128>;
using STHash160 = STBitString<160>;
using STHash256 = STBitString<256>;

template <>
inline
SerializedTypeID
STHash128::getSType () const
{
    return STI_HASH128;
}

template <>
inline
SerializedTypeID
STHash160::getSType () const
{
    return STI_HASH160;
}

template <>
inline
SerializedTypeID
STHash256::getSType () const
{
    return STI_HASH256;
}

} // sdchain

#endif
