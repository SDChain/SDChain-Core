//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_ISSUE_H_INCLUDED
#define SDCHAIN_PROTOCOL_ISSUE_H_INCLUDED

#include <cassert>
#include <functional>
#include <type_traits>

#include <sdchain/protocol/UintTypes.h>

namespace sdchain {

/** A currency issued by an account.
    @see Currency, AccountID, Issue, Book
*/
class Issue
{
public:
    Currency currency;
    AccountID account;

    Issue ()
    {
    }

    Issue (Currency const& c, AccountID const& a)
            : currency (c), account (a)
    {
    }
};

bool
isConsistent (Issue const& ac);

std::string
to_string (Issue const& ac);

std::ostream&
operator<< (std::ostream& os, Issue const& x);

template <class Hasher>
void
hash_append(Hasher& h, Issue const& r)
{
    using beast::hash_append;
    hash_append(h, r.currency, r.account);
}

/** Ordered comparison.
    The assets are ordered first by currency and then by account,
    if the currency is not SDA.
*/
int
compare (Issue const& lhs, Issue const& rhs);

/** Equality comparison. */
/** @{ */
bool
operator== (Issue const& lhs, Issue const& rhs);
bool
operator!= (Issue const& lhs, Issue const& rhs);
/** @} */

/** Strict weak ordering. */
/** @{ */
bool
operator< (Issue const& lhs, Issue const& rhs);
bool
operator> (Issue const& lhs, Issue const& rhs);
bool
operator>= (Issue const& lhs, Issue const& rhs);
bool
operator<= (Issue const& lhs, Issue const& rhs);
/** @} */

//------------------------------------------------------------------------------

/** Returns an asset specifier that represents SDA. */
inline Issue const& sdaIssue ()
{
    static Issue issue {sdaCurrency(), sdaAccount()};
    return issue;
}

/** Returns an asset specifier that represents no account and currency. */
inline Issue const& noIssue ()
{
    static Issue issue {noCurrency(), noAccount()};
    return issue;
}

}

#endif
