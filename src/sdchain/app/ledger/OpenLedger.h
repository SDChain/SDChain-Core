//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_LEDGER_OPENLEDGER_H_INCLUDED
#define SDCHAIN_APP_LEDGER_OPENLEDGER_H_INCLUDED

#include <sdchain/app/ledger/Ledger.h>
#include <sdchain/ledger/CachedSLEs.h>
#include <sdchain/ledger/OpenView.h>
#include <sdchain/app/misc/CanonicalTXSet.h>
#include <sdchain/basics/Log.h>
#include <sdchain/basics/UnorderedContainers.h>
#include <sdchain/core/Config.h>
#include <sdchain/beast/utility/Journal.h>
#include <cassert>
#include <mutex>

namespace sdchain {

// How many total extra passes we make
// We must ensure we make at least one non-retriable pass
#define LEDGER_TOTAL_PASSES 3

// How many extra retry passes we
// make if the previous retry pass made changes
#define LEDGER_RETRY_PASSES 1

using OrderedTxs = CanonicalTXSet;

//------------------------------------------------------------------------------

/** Represents the open ledger. */
class OpenLedger
{
private:
    beast::Journal j_;
    CachedSLEs& cache_;
    std::mutex mutable modify_mutex_;
    std::mutex mutable current_mutex_;
    std::shared_ptr<OpenView const> current_;

public:
    /** Signature for modification functions.

        The modification function is called during
        apply and modify with an OpenView to accumulate
        changes and the Journal to use for logging.

        A return value of `true` informs OpenLedger
        that changes were made. Always returning
        `true` won't cause harm, but it may be
        sub-optimal.
    */
    using modify_type = std::function<
        bool(OpenView&, beast::Journal)>;

    OpenLedger() = delete;
    OpenLedger (OpenLedger const&) = delete;
    OpenLedger& operator= (OpenLedger const&) = delete;

    /** Create a new open ledger object.

        @param ledger A closed ledger
    */
    explicit
    OpenLedger(std::shared_ptr<
        Ledger const> const& ledger,
            CachedSLEs& cache,
                beast::Journal journal);

    /** Returns `true` if there are no transactions.

        The behavior of ledger closing can be different
        depending on whether or not transactions exist
        in the open ledger.

        @note The value returned is only meaningful for
              that specific instant in time. An open,
              empty ledger can become non empty from
              subsequent modifications. Caller is
              responsible for synchronizing the meaning of
              the return value.
    */
    bool
    empty() const;

    /** Returns a view to the current open ledger.

        Thread safety:
            Can be called concurrently from any thread.

        Effects:
            The caller is given ownership of a
            non-modifiable snapshot of the open ledger
            at the time of the call.
    */
    std::shared_ptr<OpenView const>
    current() const;

    /** Modify the open ledger

        Thread safety:
            Can be called concurrently from any thread.

        If `f` returns `true`, the changes made in the
        OpenView will be published to the open ledger.

        @return `true` if the open view was changed
    */
    bool
    modify (modify_type const& f);

    /** Accept a new ledger.

        Thread safety:
            Can be called concurrently from any thread.

        Effects:

            A new open view based on the accepted ledger
            is created, and the list of retriable
            transactions is optionally applied first
            depending on the value of `retriesFirst`.

            The transactions in the current open view
            are applied to the new open view.

            The list of local transactions are applied
            to the new open view.

            The optional modify function f is called
            to perform further modifications to the
            open view, atomically. Changes made in
            the modify function are not visible to
            callers until accept() returns.

            Any failed, retriable transactions are left
            in `retries` for the caller.

            The current view is atomically set to the
            new open view.

        @param rules The rules for the open ledger
        @param ledger A new closed ledger
    */
    void
    accept (Application& app, Rules const& rules,
        std::shared_ptr<Ledger const> const& ledger,
            OrderedTxs const& locals, bool retriesFirst,
                OrderedTxs& retries, ApplyFlags flags,
                    std::string const& suffix = "",
                        modify_type const& f = {});

private:
    /** Algorithm for applying transactions.

        This has the retry logic and ordering semantics
        used for consensus and building the open ledger.
    */
    template <class FwdRange>
    static
    void
    apply (Application& app, OpenView& view,
        ReadView const& check, FwdRange const& txs,
            OrderedTxs& retries, ApplyFlags flags,
                std::map<uint256, bool>& shouldRecover,
                    beast::Journal j);

    enum Result
    {
        success,
        failure,
        retry
    };

    std::shared_ptr<OpenView>
    create (Rules const& rules,
        std::shared_ptr<Ledger const> const& ledger);

    static
    Result
    apply_one (Application& app, OpenView& view,
        std::shared_ptr< STTx const> const& tx,
            bool retry, ApplyFlags flags,
                bool shouldRecover, beast::Journal j);
};

//------------------------------------------------------------------------------

template <class FwdRange>
void
OpenLedger::apply (Application& app, OpenView& view,
    ReadView const& check, FwdRange const& txs,
        OrderedTxs& retries, ApplyFlags flags,
            std::map<uint256, bool>& shouldRecover,
                beast::Journal j)
{
    for (auto iter = txs.begin();
        iter != txs.end(); ++iter)
    {
        try
        {
            // Dereferencing the iterator can
            // throw since it may be transformed.
            auto const tx = *iter;
            auto const txId = tx->getTransactionID();
            if (check.txExists(txId))
                continue;
            auto const result = apply_one(app, view,
                tx, true, flags, shouldRecover[txId], j);
            if (result == Result::retry)
                retries.insert(tx);
        }
        catch(std::exception const&)
        {
            JLOG(j.error()) <<
                "Caught exception";
        }
    }
    bool retry = true;
    for (int pass = 0;
        pass < LEDGER_TOTAL_PASSES;
            ++pass)
    {
        int changes = 0;
        auto iter = retries.begin();
        while (iter != retries.end())
        {
            switch (apply_one(app, view,
                iter->second, retry, flags,
                    shouldRecover[iter->second->getTransactionID()], j))
            {
            case Result::success:
                ++changes;
            case Result::failure:
                iter = retries.erase (iter);
                break;
            case Result::retry:
                ++iter;
            }
        }
        // A non-retry pass made no changes
        if (! changes && ! retry)
            return;
        // Stop retriable passes
        if (! changes || (pass >= LEDGER_RETRY_PASSES))
            retry = false;
    }

    // If there are any transactions left, we must have
    // tried them in at least one final pass
    assert (retries.empty() || ! retry);
}

//------------------------------------------------------------------------------

// For debug logging

std::string
debugTxstr (std::shared_ptr<STTx const> const& tx);

std::string
debugTostr (OrderedTxs const& set);

std::string
debugTostr (SHAMap const& set);

std::string
debugTostr (std::shared_ptr<ReadView const> const& view);

} // sdchain

#endif
