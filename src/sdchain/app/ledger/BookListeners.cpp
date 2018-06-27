//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/ledger/OrderBookDB.h>
#include <sdchain/app/misc/NetworkOPs.h>
#include <sdchain/json/to_string.h>

namespace sdchain {

void
BookListeners::addSubscriber(InfoSub::ref sub)
{
    std::lock_guard<std::recursive_mutex> sl(mLock);
    mListeners[sub->getSeq()] = sub;
}

void
BookListeners::removeSubscriber(std::uint64_t seq)
{
    std::lock_guard<std::recursive_mutex> sl(mLock);
    mListeners.erase(seq);
}

void
BookListeners::publish(
    Json::Value const& jvObj,
    hash_set<std::uint64_t>& havePublished)
{
    std::lock_guard<std::recursive_mutex> sl(mLock);
    auto it = mListeners.cbegin();

    while (it != mListeners.cend())
    {
        InfoSub::pointer p = it->second.lock();

        if (p)
        {
            // Only publish jvObj if this is the first occurence
            if(havePublished.emplace(p->getSeq()).second)
            {
                p->send(jvObj, true);
            }
            ++it;
        }
        else
            it = mListeners.erase(it);
    }
}

}  // namespace sdchain
