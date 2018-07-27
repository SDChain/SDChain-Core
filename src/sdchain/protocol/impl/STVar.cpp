//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/basics/contract.h>
#include <sdchain/protocol/STAccount.h>
#include <sdchain/protocol/STAmount.h>
#include <sdchain/protocol/STArray.h>
#include <sdchain/protocol/STBase.h>
#include <sdchain/protocol/STBitString.h>
#include <sdchain/protocol/STBlob.h>
#include <sdchain/protocol/STInteger.h>
#include <sdchain/protocol/STObject.h>
#include <sdchain/protocol/STPathSet.h>
#include <sdchain/protocol/STVector256.h>
#include <sdchain/protocol/impl/STVar.h>

namespace sdchain {
namespace detail {

defaultObject_t defaultObject;
nonPresentObject_t nonPresentObject;

//------------------------------------------------------------------------------

STVar::~STVar()
{
    destroy();
}

STVar::STVar (STVar const& other)
{
    if (other.p_ != nullptr)
        p_ = other.p_->copy(max_size, &d_);
}

STVar::STVar (STVar&& other)
{
    if (other.on_heap())
    {
        p_ = other.p_;
        other.p_ = nullptr;
    }
    else
    {
        p_ = other.p_->move(max_size, &d_);
    }
}

STVar&
STVar::operator= (STVar const& rhs)
{
    if (&rhs != this)
    {
        destroy();
        if (rhs.p_)
            p_ = rhs.p_->copy(max_size, &d_);
        else
            p_ = nullptr;
    }

    return *this;
}

STVar&
STVar::operator= (STVar&& rhs)
{
    if (&rhs != this)
    {
        destroy();
        if (rhs.on_heap())
        {
            p_ = rhs.p_;
            rhs.p_ = nullptr;
        }
        else
        {
            p_ = rhs.p_->move(max_size, &d_);
        }
    }

    return *this;
}

STVar::STVar (defaultObject_t, SField const& name)
    : STVar(name.fieldType, name)
{
}

STVar::STVar (nonPresentObject_t, SField const& name)
    : STVar(STI_NOTPRESENT, name)
{
}

STVar::STVar (SerialIter& sit, SField const& name)
{
    switch (name.fieldType)
    {
    case STI_NOTPRESENT:    construct<STBase>(name); return;
    case STI_UINT8:         construct<STUInt8>(sit, name); return;
    case STI_UINT16:        construct<STUInt16>(sit, name); return;
    case STI_UINT32:        construct<STUInt32>(sit, name); return;
    case STI_UINT64:        construct<STUInt64>(sit, name); return;
    case STI_AMOUNT:        construct<STAmount>(sit, name); return;
    case STI_HASH128:       construct<STHash128>(sit, name); return;
    case STI_HASH160:       construct<STHash160>(sit, name); return;
    case STI_HASH256:       construct<STHash256>(sit, name); return;
    case STI_VECTOR256:     construct<STVector256>(sit, name); return;
    case STI_VL:            construct<STBlob>(sit, name); return;
    case STI_ACCOUNT:       construct<STAccount>(sit, name); return;
    case STI_PATHSET:       construct<STPathSet>(sit, name); return;
    case STI_OBJECT:        construct<STObject>(sit, name); return;
    case STI_ARRAY:         construct<STArray>(sit, name); return;
    default:
        Throw<std::runtime_error> ("Unknown object type");
    }
}

STVar::STVar (SerializedTypeID id, SField const& name)
{
    assert ((id == STI_NOTPRESENT) || (id == name.fieldType));
    switch (id)
    {
    case STI_NOTPRESENT:    construct<STBase>(name); return;
    case STI_UINT8:         construct<STUInt8>(name); return;
    case STI_UINT16:        construct<STUInt16>(name); return;
    case STI_UINT32:        construct<STUInt32>(name); return;
    case STI_UINT64:        construct<STUInt64>(name); return;
    case STI_AMOUNT:        construct<STAmount>(name); return;
    case STI_HASH128:       construct<STHash128>(name); return;
    case STI_HASH160:       construct<STHash160>(name); return;
    case STI_HASH256:       construct<STHash256>(name); return;
    case STI_VECTOR256:     construct<STVector256>(name); return;
    case STI_VL:            construct<STBlob>(name); return;
    case STI_ACCOUNT:       construct<STAccount>(name); return;
    case STI_PATHSET:       construct<STPathSet>(name); return;
    case STI_OBJECT:        construct<STObject>(name); return;
    case STI_ARRAY:         construct<STArray>(name); return;
    default:
        Throw<std::runtime_error> ("Unknown object type");
    }
}

void
STVar::destroy()
{
    if (on_heap())
        delete p_;
    else
        p_->~STBase();

    p_ = nullptr;
}

} // detail
} // sdchain
