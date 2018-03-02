//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_LEDGER_APPLYSTATETABLE_H_INCLUDED
#define SDCHAIN_LEDGER_APPLYSTATETABLE_H_INCLUDED

#include <sdchain/ledger/OpenView.h>
#include <sdchain/ledger/RawView.h>
#include <sdchain/ledger/ReadView.h>
#include <sdchain/ledger/TxMeta.h>
#include <sdchain/protocol/TER.h>
#include <sdchain/protocol/SDAAmount.h>
#include <sdchain/beast/utility/Journal.h>
#include <memory>

namespace sdchain {
namespace detail {

// Helper class that buffers modifications
class ApplyStateTable
{
public:
    using key_type = ReadView::key_type;

private:
    enum class Action
    {
        cache,
        erase,
        insert,
        modify,
    };

    using items_t = std::map<key_type,
        std::pair<Action, std::shared_ptr<SLE>>>;

    items_t items_;
    SDAAmount dropsDestroyed_ = 0;

public:
    ApplyStateTable() = default;
    ApplyStateTable (ApplyStateTable&&) = default;

    ApplyStateTable (ApplyStateTable const&) = delete;
    ApplyStateTable& operator= (ApplyStateTable&&) = delete;
    ApplyStateTable& operator= (ApplyStateTable const&) = delete;

    void
    apply (RawView& to) const;

    void
    apply (OpenView& to, STTx const& tx,
        TER ter, boost::optional<
            STAmount> const& deliver,
                beast::Journal j);

    bool
    exists (ReadView const& base,
        Keylet const& k) const;

    boost::optional<key_type>
    succ (ReadView const& base,
        key_type const& key, boost::optional<
            key_type> const& last) const;

    std::shared_ptr<SLE const>
    read (ReadView const& base,
        Keylet const& k) const;

    std::shared_ptr<SLE>
    peek (ReadView const& base,
        Keylet const& k);

    std::size_t
    size () const;

    void
    visit (ReadView const& base,
        std::function <void (
            uint256 const& key,
            bool isDelete,
            std::shared_ptr <SLE const> const& before,
            std::shared_ptr <SLE const> const& after)> const& func) const;

    void
    erase (ReadView const& base,
        std::shared_ptr<SLE> const& sle);

    void
    rawErase (ReadView const& base,
        std::shared_ptr<SLE> const& sle);

    void
    insert(ReadView const& base,
        std::shared_ptr<SLE> const& sle);

    void
    update(ReadView const& base,
        std::shared_ptr<SLE> const& sle);

    void
    replace(ReadView const& base,
        std::shared_ptr<SLE> const& sle);

    void
    destroySDA (SDAAmount const& fee);

    // For debugging
    SDAAmount const& dropsDestroyed () const
    {
        return dropsDestroyed_;
    }

private:
    using Mods = hash_map<key_type,
        std::shared_ptr<SLE>>;

    static
    void
    threadItem (TxMeta& meta,
        std::shared_ptr<SLE> const& to);

    std::shared_ptr<SLE>
    getForMod (ReadView const& base,
        key_type const& key, Mods& mods,
            beast::Journal j);

    void
    threadTx (ReadView const& base, TxMeta& meta,
        AccountID const& to, Mods& mods,
            beast::Journal j);

    void
    threadOwners (ReadView const& base,
        TxMeta& meta, std::shared_ptr<
            SLE const> const& sle, Mods& mods,
                beast::Journal j);
};

} // detail
} // sdchain

#endif
