//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_RESOURCE_ENTRY_H_INCLUDED
#define SDCHAIN_RESOURCE_ENTRY_H_INCLUDED

#include <sdchain/basics/DecayingSample.h>
#include <sdchain/resource/impl/Key.h>
#include <sdchain/resource/impl/Tuning.h>
#include <sdchain/beast/clock/abstract_clock.h>
#include <sdchain/beast/core/List.h>
#include <cassert>

namespace sdchain {
namespace Resource {

using clock_type = beast::abstract_clock <std::chrono::steady_clock>;

// An entry in the table
// VFALCO DEPRECATED using boost::intrusive list
struct Entry
    : public beast::List <Entry>::Node
{
    Entry () = delete;

    /**
       @param now Construction time of Entry.
    */
    explicit Entry(clock_type::time_point const now)
        : refcount (0)
        , local_balance (now)
        , remote_balance (0)
        , lastWarningTime ()
        , whenExpires ()
    {
    }

    std::string to_string() const
    {
        switch (key->kind)
        {
        case kindInbound:   return key->address.to_string();
        case kindOutbound:  return key->address.to_string();
        case kindUnlimited: return std::string ("\"") + key->name + "\"";
        default:
            assert(false);
        }

        return "(undefined)";
    }

    /**
     * Returns `true` if this connection should have no
     * resource limits applied--it is still possible for certain RPC commands
     * to be forbidden, but that depends on Role.
     */
    bool isUnlimited () const
    {
        return key->kind == kindUnlimited;
    }

    // Balance including remote contributions
    int balance (clock_type::time_point const now)
    {
        return local_balance.value (now) + remote_balance;
    }

    // Add a charge and return normalized balance
    // including contributions from imports.
    int add (int charge, clock_type::time_point const now)
    {
        return local_balance.add (charge, now) + remote_balance;
    }

    // Back pointer to the map key (bit of a hack here)
    Key const* key;

    // Number of Consumer references
    int refcount;

    // Exponentially decaying balance of resource consumption
    DecayingSample <decayWindowSeconds, clock_type> local_balance;

    // Normalized balance contribution from imports
    int remote_balance;

    // Time of the last warning
    clock_type::time_point lastWarningTime;

    // For inactive entries, time after which this entry will be erased
    clock_type::time_point whenExpires;
};

inline std::ostream& operator<< (std::ostream& os, Entry const& v)
{
    os << v.to_string();
    return os;
}

}
}

#endif
