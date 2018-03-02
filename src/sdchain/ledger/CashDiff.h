//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2016 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_LEDGER_CASHDIFF_H_INCLUDED
#define SDCHAIN_LEDGER_CASHDIFF_H_INCLUDED

#include <sdchain/protocol/STAmount.h>
#include <memory>                       // std::unique_ptr

namespace sdchain {

class ReadView;

namespace detail {

class ApplyStateTable;

}

// Used by CashDiff to specify filters applied while processing differences.
// Entries are bit flags that can be ANDed and ORed.
enum class CashFilter : std::uint8_t
{
    none = 0x0,
    treatZeroOfferAsDeletion = 0x1
};
inline CashFilter operator| (CashFilter lhs, CashFilter rhs)
{
    using ul_t = std::underlying_type<CashFilter>::type;
    return static_cast<CashFilter>(
        static_cast<ul_t>(lhs) | static_cast<ul_t>(rhs));
}
inline CashFilter operator& (CashFilter lhs, CashFilter rhs)
{
    using ul_t = std::underlying_type<CashFilter>::type;
    return static_cast<CashFilter>(
        static_cast<ul_t>(lhs) & static_cast<ul_t>(rhs));
}

//------------------------------------------------------------------------------

// A class to identify differences between two ApplyStateTable instances
// for debugging.
class CashDiff
{
public:
    CashDiff() = delete;
    CashDiff (CashDiff const&) = delete;
    CashDiff (CashDiff&& other);
    CashDiff& operator= (CashDiff const&) = delete;
    ~CashDiff();

    CashDiff (ReadView const& view,
        CashFilter lhsFilter, detail::ApplyStateTable const& lhs,
        CashFilter rhsFilter, detail::ApplyStateTable const& rhs);

    // Returns the number of cases where lhs and rhs had the same entries
    // (but not necessarily the same amounts)
    std::size_t commonCount () const;

    // Returns the number of entries that were present in rhs but not in lhs.
    std::size_t rhsOnlyCount () const;

    // Returns the number of entries that were present in lhs but not in rhs.
    std::size_t lhsOnlyCount () const;

    // Returns true is there are any differences to report.
    bool hasDiff() const;

    // Remove dust-sized differences.  Returns true is dust was removed.
    bool rmDust();

    // Remove offer deletion differences from a given side.  Returns true
    // if any deleted offers were removed from the differences.
    bool rmLhsDeletedOffers();
    bool rmRhsDeletedOffers();

    struct OfferAmounts
    {
        static std::size_t constexpr count_ = 2;
        static std::size_t constexpr count() { return count_; }
        STAmount amounts[count_];
        STAmount const& takerPays() const { return amounts[0]; }
        STAmount const& takerGets() const { return amounts[1]; }
        STAmount const& operator[] (std::size_t i) const
        {
            assert (i < count());
            return amounts[i];
        }
        friend bool operator< (OfferAmounts const& lhs, OfferAmounts const& rhs)
        {
            if (lhs[0] < rhs[0])
                return true;
            if (lhs[0] > rhs[0])
                return false;
            return lhs[1] < rhs[1];
        }
    };

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

// Return true if the difference between two STAmounts is "small".
//
// If v1 and v2 have different issues, then their difference is never dust.
// If v1 < v2, smallness is computed as v1 / (v2 - v1).
// The e10 argument says at least how big that ratio must be.  Default is 10^6.
// If both v1 and v2 are SDA, any difference of 2 or smaller is considered dust.
bool diffIsDust (STAmount const& v1, STAmount const& v2, std::uint8_t e10 = 6);

} // sdchain

#endif
