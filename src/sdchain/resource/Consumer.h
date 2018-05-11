//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_RESOURCE_CONSUMER_H_INCLUDED
#define SDCHAIN_RESOURCE_CONSUMER_H_INCLUDED

#include <sdchain/resource/Charge.h>
#include <sdchain/resource/Disposition.h>

namespace sdchain {
namespace Resource {

struct Entry;
class Logic;

/** An endpoint that consumes resources. */
class Consumer
{
private:
    friend class Logic;
    Consumer (Logic& logic, Entry& entry);

public:
    Consumer ();
    ~Consumer ();
    Consumer (Consumer const& other);
    Consumer& operator= (Consumer const& other);

    /** Return a human readable string uniquely identifying this consumer. */
    std::string to_string () const;

    /** Returns `true` if this is a privileged endpoint. */
    bool isUnlimited () const;

    /** Raise the Consumer's privilege level to a Named endpoint.
        The reference to the original endpoint descriptor is released.
    */
    void elevate (std::string const& name);

    /** Returns the current disposition of this consumer.
        This should be checked upon creation to determine if the consumer
        should be disconnected immediately.
    */
    Disposition disposition() const;

    /** Apply a load charge to the consumer. */
    Disposition charge (Charge const& fee);

    /** Returns `true` if the consumer should be warned.
        This consumes the warning.
    */
    bool warn();

    /** Returns `true` if the consumer should be disconnected. */
    bool disconnect();

    /** Returns the credit balance representing consumption. */
    int balance();

    // Private: Retrieve the entry associated with the consumer
    Entry& entry();

private:
    Logic* m_logic;
    Entry* m_entry;
};

std::ostream& operator<< (std::ostream& os, Consumer const& v);

}
}

#endif
