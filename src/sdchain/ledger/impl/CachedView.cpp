//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/ledger/CachedView.h>
#include <sdchain/basics/contract.h>
#include <sdchain/protocol/Serializer.h>

namespace sdchain {
namespace detail {

bool
CachedViewImpl::exists (Keylet const& k) const
{
    return read(k) != nullptr;
}

std::shared_ptr<SLE const>
CachedViewImpl::read (Keylet const& k) const
{
    {
        std::lock_guard<
            std::mutex> lock(mutex_);
        auto const iter = map_.find(k.key);
        if (iter != map_.end())
        {
            if (! k.check(*iter->second))
                return nullptr;
            return iter->second;
        }
    }
    auto const digest =
        base_.digest(k.key);
    if (! digest)
        return nullptr;
    auto sle = cache_.fetch(*digest,
        [&]() { return base_.read(k); });
    std::lock_guard<
        std::mutex> lock(mutex_);
    auto const iter =
        map_.find(k.key);
    if (iter == map_.end())
    {
        map_.emplace(k.key, sle);
        return sle;
    }
    if (! k.check(*iter->second))
        LogicError("CachedView::read: wrong type");
    return iter->second;

}

} // detail
} // sdchain
