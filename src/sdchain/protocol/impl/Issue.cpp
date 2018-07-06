//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <sdchain/protocol/Issue.h>

namespace sdchain {

bool
isConsistent (Issue const& ac)
{
    return isSDA (ac.currency) == isSDA (ac.account);
}

std::string
to_string (Issue const& ac)
{
    if (isSDA (ac.account))
        return to_string (ac.currency);

    return to_string(ac.account) + "/" + to_string(ac.currency);
}

std::ostream&
operator<< (std::ostream& os, Issue const& x)
{
    os << to_string (x);
    return os;
}

/** Ordered comparison.
    The assets are ordered first by currency and then by account,
    if the currency is not SDA.
*/
int
compare (Issue const& lhs, Issue const& rhs)
{
    int diff = compare (lhs.currency, rhs.currency);
    if (diff != 0)
        return diff;
    if (isSDA (lhs.currency))
        return 0;
    return compare (lhs.account, rhs.account);
}

/** Equality comparison. */
/** @{ */
bool
operator== (Issue const& lhs, Issue const& rhs)
{
    return compare (lhs, rhs) == 0;
}

bool
operator!= (Issue const& lhs, Issue const& rhs)
{
    return ! (lhs == rhs);
}
/** @} */

/** Strict weak ordering. */
/** @{ */
bool
operator< (Issue const& lhs, Issue const& rhs)
{
    return compare (lhs, rhs) < 0;
}

bool
operator> (Issue const& lhs, Issue const& rhs)
{
    return rhs < lhs;
}

bool
operator>= (Issue const& lhs, Issue const& rhs)
{
    return ! (lhs < rhs);
}

bool
operator<= (Issue const& lhs, Issue const& rhs)
{
    return ! (rhs < lhs);
}

} // sdchain
