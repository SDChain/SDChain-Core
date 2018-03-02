//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_BASICS_MAKE_LOCK_H_INCLUDED
#define SDCHAIN_BASICS_MAKE_LOCK_H_INCLUDED

#include <mutex>
#include <utility>

namespace sdchain {

template <class Mutex, class ...Args>
inline
std::unique_lock<Mutex>
make_lock(Mutex& mutex, Args&&... args)
{
    return std::unique_lock<Mutex>(mutex, std::forward<Args>(args)...);
}

} // sdchain

#endif
