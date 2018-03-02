//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012, 2015 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_LEDGER_DIR_H_INCLUDED
#define SDCHAIN_LEDGER_DIR_H_INCLUDED

#include <sdchain/ledger/ReadView.h>
#include <sdchain/protocol/Indexes.h>

namespace sdchain {

class Dir
{
private:
    ReadView const* view_ = nullptr;
    Keylet root_;
    std::shared_ptr<SLE const> sle_;
    STVector256 const* indexes_ = nullptr;

public:
    class const_iterator;
    using value_type = std::shared_ptr<SLE const>;

    Dir(ReadView const&, Keylet const&);

    const_iterator
    begin() const;

    const_iterator
    end() const;
};

class Dir::const_iterator
{
public:
    using value_type =
        Dir::value_type;
    using pointer =
        value_type const*;
    using reference =
        value_type const&;
    using difference_type =
        std::ptrdiff_t;
    using iterator_category =
        std::forward_iterator_tag;

    const_iterator() = default;

    bool
    operator==(const_iterator const& other) const;

    bool
    operator!=(const_iterator const& other) const
    {
        return ! (*this == other);
    }

    reference
    operator*() const;

    pointer
    operator->() const
    {
        return &**this;
    }

    const_iterator&
    operator++();

    const_iterator
    operator++(int);

    Keylet const&
    page() const
    {
        return page_;
    }

    uint256
    index() const
    {
        return index_;
    }

private:
    friend class Dir;

    const_iterator(ReadView const& view,
        Keylet const& root, Keylet const& page)
    : view_(&view)
    , root_(root)
    , page_(page)
    {
    }

    ReadView const* view_ = nullptr;
    Keylet root_;
    Keylet page_;
    uint256 index_;
    boost::optional<value_type> mutable cache_;
    std::shared_ptr<SLE const> sle_;
    STVector256 const* indexes_ = nullptr;
    std::vector<uint256>::const_iterator it_;
};

} // sdchain

#endif
