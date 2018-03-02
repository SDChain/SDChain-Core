//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_NET_SNTPCLOCK_H_INCLUDED
#define SDCHAIN_NET_SNTPCLOCK_H_INCLUDED

#include <sdchain/beast/clock/abstract_clock.h>
#include <sdchain/beast/utility/Journal.h>
#include <chrono>
#include <memory>
#include <string>
#include <vector>

namespace sdchain {

/** A clock based on system_clock and adjusted for SNTP. */
class SNTPClock
    : public beast::abstract_clock<
        std::chrono::system_clock>
{
public:
    virtual
    void
    run (std::vector <std::string> const& servers) = 0;

    virtual
    duration
    offset() const = 0;
};

extern
std::unique_ptr<SNTPClock>
make_SNTPClock (beast::Journal);

} // sdchain

#endif
