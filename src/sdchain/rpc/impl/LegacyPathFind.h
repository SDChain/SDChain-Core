//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_RPC_LEGACYPATHFIND_H_INCLUDED
#define SDCHAIN_RPC_LEGACYPATHFIND_H_INCLUDED

#include <atomic>

namespace sdchain {

class Application;

namespace RPC {

class LegacyPathFind
{
public:
    LegacyPathFind (bool isAdmin, Application& app);
    ~LegacyPathFind ();

    bool isOk () const
    {
        return m_isOk;
    }

private:
    static std::atomic <int> inProgress;

    bool m_isOk;
};

} // RPC
} // sdchain

#endif
