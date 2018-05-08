//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_RESOURCE_CHARGE_H_INCLUDED
#define SDCHAIN_RESOURCE_CHARGE_H_INCLUDED

#include <ios>
#include <string>

namespace sdchain {
namespace Resource {

/** A consumption charge. */
class Charge
{
public:
    /** The type used to hold a consumption charge. */
    using value_type = int;

    // A default constructed Charge has no way to get a label.  Delete
    Charge () = delete;

    /** Create a charge with the specified cost and name. */
    Charge (value_type cost, std::string const& label = std::string());

    /** Return the human readable label associated with the charge. */
    std::string const& label() const;

    /** Return the cost of the charge in Resource::Manager units. */
    value_type cost () const;

    /** Converts this charge into a human readable string. */
    std::string to_string () const;

    bool operator== (Charge const&) const;
    bool operator!= (Charge const&) const;

private:
    value_type m_cost;
    std::string m_label;
};

std::ostream& operator<< (std::ostream& os, Charge const& v);

}
}

#endif
