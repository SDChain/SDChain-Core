//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/misc/HashRouter.h>

namespace sdchain {

auto
HashRouter::emplace (uint256 const& key)
    -> std::pair<Entry&, bool>
{
    auto iter = suppressionMap_.find (key);

    if (iter != suppressionMap_.end ())
    {
        suppressionMap_.touch(iter);
        return std::make_pair(
            std::ref(iter->second), false);
    }

    // See if any supressions need to be expired
    expire(suppressionMap_, holdTime_);

    return std::make_pair(std::ref(
        suppressionMap_.emplace (
            key, Entry ()).first->second),
                true);
}

void HashRouter::addSuppression (uint256 const& key)
{
    std::lock_guard <std::mutex> lock (mutex_);

    emplace (key);
}

bool HashRouter::addSuppressionPeer (uint256 const& key, PeerShortID peer)
{
    std::lock_guard <std::mutex> lock (mutex_);

    auto result = emplace(key);
    result.first.addPeer(peer);
    return result.second;
}

bool HashRouter::addSuppressionPeer (uint256 const& key, PeerShortID peer, int& flags)
{
    std::lock_guard <std::mutex> lock (mutex_);

    auto result = emplace(key);
    auto& s = result.first;
    s.addPeer (peer);
    flags = s.getFlags ();
    return result.second;
}

int HashRouter::getFlags (uint256 const& key)
{
    std::lock_guard <std::mutex> lock (mutex_);

    return emplace(key).first.getFlags ();
}

bool HashRouter::setFlags (uint256 const& key, int flags)
{
    assert (flags != 0);

    std::lock_guard <std::mutex> lock (mutex_);

    auto& s = emplace(key).first;

    if ((s.getFlags () & flags) == flags)
        return false;

    s.setFlags (flags);
    return true;
}

auto
HashRouter::shouldRelay (uint256 const& key)
    -> boost::optional<std::set<PeerShortID>>
{
    std::lock_guard <std::mutex> lock (mutex_);

    auto& s = emplace(key).first;

    if (!s.shouldRelay(suppressionMap_.clock().now(), holdTime_))
        return boost::none;

    return s.releasePeerSet();
}

bool
HashRouter::shouldRecover(uint256 const& key)
{
    std::lock_guard <std::mutex> lock(mutex_);

    auto& s = emplace(key).first;

    return s.shouldRecover(recoverLimit_);
}

} // sdchain
