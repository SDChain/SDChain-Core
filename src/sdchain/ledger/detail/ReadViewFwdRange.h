//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_LEDGER_READVIEWFWDRANGE_H_INCLUDED
#define SDCHAIN_LEDGER_READVIEWFWDRANGE_H_INCLUDED

#include <cstddef>
#include <iterator>
#include <memory>
#include <boost/optional.hpp>

namespace sdchain {

class ReadView;

namespace detail {

// A type-erased ForwardIterator
//
template <class ValueType>
class ReadViewFwdIter
{
public:
    using base_type = ReadViewFwdIter;

    using value_type = ValueType;

    ReadViewFwdIter() = default;
    ReadViewFwdIter(ReadViewFwdIter const&) = default;
    ReadViewFwdIter& operator=(ReadViewFwdIter const&) = default;

    virtual
    ~ReadViewFwdIter() = default;

    virtual
    std::unique_ptr<ReadViewFwdIter>
    copy() const = 0;

    virtual
    bool
    equal (ReadViewFwdIter const& impl) const = 0;

    virtual
    void
    increment() = 0;

    virtual
    value_type
    dereference() const = 0;
};

// A range using type-erased ForwardIterator
//
template<class ValueType>
class ReadViewFwdRange
{
public:
    using iter_base =
        ReadViewFwdIter<ValueType>;

    class iterator
    {
    public:
        using value_type = ValueType;

        using pointer = value_type const*;

        using reference = value_type const&;

        using difference_type =
            std::ptrdiff_t;

        using iterator_category =
            std::forward_iterator_tag;

        iterator() = default;

        iterator (iterator const& other);
        iterator (iterator&& other);

        // Used by the implementation
        explicit
        iterator (ReadView const* view,
            std::unique_ptr<iter_base> impl);

        iterator&
        operator= (iterator const& other);

        iterator&
        operator= (iterator&& other);

        bool
        operator== (iterator const& other) const;

        bool
        operator!= (iterator const& other) const;

        // Can throw
        reference
        operator*() const;

        // Can throw
        pointer
        operator->() const;

        iterator&
        operator++();

        iterator
        operator++(int);

    private:
        ReadView const* view_ = nullptr;
        std::unique_ptr<iter_base> impl_;
        boost::optional<value_type> mutable cache_;
    };

    using const_iterator = iterator;

    using value_type = ValueType;

    ReadViewFwdRange() = delete;
    ReadViewFwdRange (ReadViewFwdRange const&) = default;
    ReadViewFwdRange& operator= (ReadViewFwdRange const&) = default;

    // VFALCO Otherwise causes errors on clang
//private:
//    friend class ReadView;

    explicit
    ReadViewFwdRange (ReadView const& view)
        : view_ (&view)
    {
    }

protected:
    ReadView const* view_;
    boost::optional<iterator> mutable end_;
};

} // detail
} // sdchain

#endif
