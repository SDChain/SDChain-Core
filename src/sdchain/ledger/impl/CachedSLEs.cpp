//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/ledger/CachedSLEs.h>
#include <vector>

namespace sdchain {

void
CachedSLEs::expire()
{
    std::vector<
        std::shared_ptr<void const>> trash;
    {
        auto const expireTime =
            map_.clock().now() - timeToLive_;
        std::lock_guard<
            std::mutex> lock(mutex_);
        for (auto iter = map_.chronological.begin();
            iter != map_.chronological.end(); ++iter)
        {
            if (iter.when() > expireTime)
                break;
            if (iter->second.unique())
            {
                trash.emplace_back(
                    std::move(iter->second));
                iter = map_.erase(iter);
            }
        }
    }
}

double
CachedSLEs::rate() const
{
    std::lock_guard<
        std::mutex> lock(mutex_);
    auto const tot = hit_ + miss_;
    if (tot == 0)
        return 0;
    return double(hit_) / tot;
}

} // sdchain
