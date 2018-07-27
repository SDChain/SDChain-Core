//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_STVAR_H_INCLUDED
#define SDCHAIN_PROTOCOL_STVAR_H_INCLUDED

#include <sdchain/protocol/Serializer.h>
#include <sdchain/protocol/SField.h>
#include <sdchain/protocol/STBase.h>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <typeinfo>

namespace sdchain {
namespace detail {

struct defaultObject_t { };
struct nonPresentObject_t { };

extern defaultObject_t defaultObject;
extern nonPresentObject_t nonPresentObject;

// "variant" that can hold any type of serialized object
// and includes a small-object allocation optimization.
class STVar
{
private:
    // The largest "small object" we can accomodate
    static std::size_t constexpr max_size = 72;

    std::aligned_storage<max_size>::type d_;
    STBase* p_ = nullptr;

public:
    ~STVar();
    STVar (STVar const& other);
    STVar (STVar&& other);
    STVar& operator= (STVar const& rhs);
    STVar& operator= (STVar&& rhs);

    STVar (STBase&& t)
    {
        p_ = t.move(max_size, &d_);
    }

    STVar (STBase const& t)
    {
        p_ = t.copy(max_size, &d_);
    }

    STVar (defaultObject_t, SField const& name);
    STVar (nonPresentObject_t, SField const& name);
    STVar (SerialIter& sit, SField const& name);

    STBase& get() { return *p_; }
    STBase& operator*() { return get(); }
    STBase* operator->() { return &get(); }
    STBase const& get() const { return *p_; }
    STBase const& operator*() const { return get(); }
    STBase const* operator->() const { return &get(); }

    template <class T, class... Args>
    friend
    STVar
    make_stvar(Args&&... args);

private:
    STVar() = default;

    STVar (SerializedTypeID id, SField const& name);

    void destroy();

    template <class T, class... Args>
    void
    construct(Args&&... args)
    {
        if(sizeof(T) > max_size)
            p_ = new T(std::forward<Args>(args)...);
        else
            p_ = new(&d_) T(std::forward<Args>(args)...);
    }

    bool
    on_heap() const
    {
        return static_cast<void const*>(p_) !=
            static_cast<void const*>(&d_);
    }
};

template <class T, class... Args>
inline
STVar
make_stvar(Args&&... args)
{
    STVar st;
    st.construct<T>(std::forward<Args>(args)...);
    return st;
}

inline
bool
operator== (STVar const& lhs, STVar const& rhs)
{
    return lhs.get().isEquivalent(rhs.get());
}

inline
bool
operator!= (STVar const& lhs, STVar const& rhs)
{
    return ! (lhs == rhs);
}

} // detail
} // sdchain

#endif
