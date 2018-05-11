//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PROTOCOL_BOOK_H_INCLUDED
#define SDCHAIN_PROTOCOL_BOOK_H_INCLUDED

#include <sdchain/protocol/Issue.h>
#include <boost/utility/base_from_member.hpp>

namespace sdchain {

/** Specifies an order book.
    The order book is a pair of Issues called in and out.
    @see Issue.
*/
class Book
{
public:
    Issue in;
    Issue out;

    Book ()
    {
    }

    Book (Issue const& in_, Issue const& out_)
        : in (in_)
        , out (out_)
    {
    }
};

bool
isConsistent (Book const& book);

std::string
to_string (Book const& book);

std::ostream&
operator<< (std::ostream& os, Book const& x);

template <class Hasher>
void
hash_append (Hasher& h, Book const& b)
{
    using beast::hash_append;
    hash_append(h, b.in, b.out);
}

Book
reversed (Book const& book);

/** Ordered comparison. */
int
compare (Book const& lhs, Book const& rhs);

/** Equality comparison. */
/** @{ */
bool
operator== (Book const& lhs, Book const& rhs);
bool
operator!= (Book const& lhs, Book const& rhs);
/** @} */

/** Strict weak ordering. */
/** @{ */
bool
operator< (Book const& lhs,Book const& rhs);
bool
operator> (Book const& lhs, Book const& rhs);
bool
operator>= (Book const& lhs, Book const& rhs);
bool
operator<= (Book const& lhs, Book const& rhs);
/** @} */

}

//------------------------------------------------------------------------------

namespace std {

template <>
struct hash <sdchain::Issue>
    : private boost::base_from_member <std::hash <sdchain::Currency>, 0>
    , private boost::base_from_member <std::hash <sdchain::AccountID>, 1>
{
private:
    using currency_hash_type = boost::base_from_member <
        std::hash <sdchain::Currency>, 0>;
    using issuer_hash_type = boost::base_from_member <
        std::hash <sdchain::AccountID>, 1>;

public:
    using value_type = std::size_t;
    using argument_type = sdchain::Issue;

    value_type operator() (argument_type const& value) const
    {
        value_type result (currency_hash_type::member (value.currency));
        if (!isSDA (value.currency))
            boost::hash_combine (result,
                issuer_hash_type::member (value.account));
        return result;
    }
};

//------------------------------------------------------------------------------

template <>
struct hash <sdchain::Book>
{
private:
    using hasher = std::hash <sdchain::Issue>;

    hasher m_hasher;

public:
    using value_type = std::size_t;
    using argument_type = sdchain::Book;

    value_type operator() (argument_type const& value) const
    {
        value_type result (m_hasher (value.in));
        boost::hash_combine (result, m_hasher (value.out));
        return result;
    }
};

}

//------------------------------------------------------------------------------

namespace boost {

template <>
struct hash <sdchain::Issue>
    : std::hash <sdchain::Issue>
{
    using Base = std::hash <sdchain::Issue>;
    // VFALCO NOTE broken in vs2012
    //using Base::Base; // inherit ctors
};

template <>
struct hash <sdchain::Book>
    : std::hash <sdchain::Book>
{
    using Base = std::hash <sdchain::Book>;
    // VFALCO NOTE broken in vs2012
    //using Base::Base; // inherit ctors
};

}

#endif
