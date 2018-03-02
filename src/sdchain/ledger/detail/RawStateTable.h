//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_LEDGER_RAWSTATETABLE_H_INCLUDED
#define SDCHAIN_LEDGER_RAWSTATETABLE_H_INCLUDED

#include <sdchain/ledger/RawView.h>
#include <sdchain/ledger/ReadView.h>
#include <sdchain/basics/qalloc.h>
#include <map>
#include <utility>

namespace sdchain {
namespace detail {

// Helper class that buffers raw modifications
class RawStateTable
{
public:
    using key_type = ReadView::key_type;

    RawStateTable() = default;
    RawStateTable (RawStateTable const&) = default;
    RawStateTable (RawStateTable&&) = default;

    RawStateTable& operator= (RawStateTable&&) = delete;
    RawStateTable& operator= (RawStateTable const&) = delete;

    void
    apply (RawView& to) const;

    bool
    exists (ReadView const& base,
        Keylet const& k) const;

    boost::optional<key_type>
    succ (ReadView const& base,
        key_type const& key, boost::optional<
            key_type> const& last) const;

    void
    erase (std::shared_ptr<SLE> const& sle);

    void
    insert (std::shared_ptr<SLE> const& sle);

    void
    replace (std::shared_ptr<SLE> const& sle);

    std::shared_ptr<SLE const>
    read (ReadView const& base,
        Keylet const& k) const;

    void
    destroySDA (SDAAmount const& fee);

    std::unique_ptr<ReadView::sles_type::iter_base>
    slesBegin (ReadView const& base) const;

    std::unique_ptr<ReadView::sles_type::iter_base>
    slesEnd (ReadView const& base) const;

    std::unique_ptr<ReadView::sles_type::iter_base>
    slesUpperBound (ReadView const& base, uint256 const& key) const;

private:
    enum class Action
    {
        erase,
        insert,
        replace,
    };

    class sles_iter_impl;

    using items_t = std::map<key_type,
        std::pair<Action, std::shared_ptr<SLE>>,
        std::less<key_type>, qalloc_type<std::pair<key_type const,
        std::pair<Action, std::shared_ptr<SLE>>>, false>>;

    items_t items_;
    SDAAmount dropsDestroyed_ = 0;
};

} // detail
} // sdchain

#endif
