//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/tx/impl/ApplyContext.h>
#include <sdchain/app/tx/impl/InvariantCheck.h>
#include <sdchain/app/tx/impl/Transactor.h>
#include <sdchain/basics/Log.h>
#include <sdchain/json/to_string.h>
#include <sdchain/protocol/Indexes.h>
#include <sdchain/protocol/Feature.h>
#include <cassert>

namespace sdchain {

ApplyContext::ApplyContext(Application& app_,
    OpenView& base, STTx const& tx_, TER preclaimResult_,
        std::uint64_t baseFee_, ApplyFlags flags,
            beast::Journal journal_)
    : app(app_)
    , tx(tx_)
    , preclaimResult(preclaimResult_)
    , baseFee(baseFee_)
    , journal(journal_)
    , base_ (base)
    , flags_(flags)
{
    view_.emplace(&base_, flags_);
}

void
ApplyContext::discard()
{
    view_.emplace(&base_, flags_);
}

void
ApplyContext::apply(TER ter)
{
    view_->apply(base_, tx, ter, journal);
}

std::size_t
ApplyContext::size()
{
    return view_->size();
}

void
ApplyContext::visit (std::function <void (
    uint256 const&, bool,
    std::shared_ptr<SLE const> const&,
    std::shared_ptr<SLE const> const&)> const& func)
{
    view_->visit(base_, func);
}

template<std::size_t... Is>
TER
ApplyContext::checkInvariantsHelper(TER terResult, std::index_sequence<Is...>)
{
    if (view_->rules().enabled(featureEnforceInvariants))
    {
        auto checkers = getInvariantChecks();
        try
        {
            // call each check's per-entry method
            visit (
                [&checkers](
                    uint256 const& index,
                    bool isDelete,
                    std::shared_ptr <SLE const> const& before,
                    std::shared_ptr <SLE const> const& after)
                {
                    // Sean Parent for_each_argument trick
                    (void)std::array<int, sizeof...(Is)>{
                        {((std::get<Is>(checkers).
                            visitEntry(index, isDelete, before, after)), 0)...}
                    };
                });

            // Sean Parent for_each_argument trick (a fold expression with `&&`
            // would be really nice here when we move to C++-17)
            std::array<bool, sizeof...(Is)> finalizers {{
                std::get<Is>(checkers).finalize(tx, terResult, journal)...}};

            // call each check's finalizer to see that it passes
            if (! std::all_of( finalizers.cbegin(), finalizers.cend(),
                    [](auto const& b) { return b; }))
            {
                terResult = (terResult == tecINVARIANT_FAILED) ?
                    tefINVARIANT_FAILED :
                    tecINVARIANT_FAILED ;
                JLOG(journal.fatal()) <<
                    "Transaction has failed one or more invariants: " <<
                    to_string(tx.getJson (0));
            }
        }
        catch(std::exception const& ex)
        {
            terResult = (terResult == tecINVARIANT_FAILED) ?
                tefINVARIANT_FAILED :
                tecINVARIANT_FAILED ;
            JLOG(journal.fatal()) <<
                "Transaction caused an exception in an invariant" <<
                ", ex: " << ex.what() <<
                ", tx: " << to_string(tx.getJson (0));
        }
    }

    return terResult;
}

TER
ApplyContext::checkInvariants(TER terResult)
{
    return checkInvariantsHelper(
        terResult, std::make_index_sequence<std::tuple_size<InvariantChecks>::value>{});
}

} // sdchain
