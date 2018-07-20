//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PEERFINDER_SIM_MESSAGE_H_INCLUDED
#define SDCHAIN_PEERFINDER_SIM_MESSAGE_H_INCLUDED

namespace sdchain {
namespace PeerFinder {
namespace Sim {

class Message
{
public:
    explicit Message (Endpoints const& endpoints)
        : m_payload (endpoints)
        { }
    Endpoints const& payload () const
        { return m_payload; }
private:
    Endpoints m_payload;
};

}
}
}

#endif
