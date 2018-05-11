//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <sdchain/protocol/Book.h>

namespace sdchain {

bool
isConsistent (Book const& book)
{
    return isConsistent(book.in) && isConsistent (book.out)
            && book.in != book.out;
}

std::string
to_string (Book const& book)
{
    return to_string(book.in) + "->" + to_string(book.out);
}

std::ostream&
operator<< (std::ostream& os, Book const& x)
{
    os << to_string (x);
    return os;
}

Book
reversed (Book const& book)
{
    return Book (book.out, book.in);
}

/** Ordered comparison. */
int
compare (Book const& lhs, Book const& rhs)
{
    int const diff (compare (lhs.in, rhs.in));
    if (diff != 0)
        return diff;
    return compare (lhs.out, rhs.out);
}

/** Equality comparison. */
/** @{ */
bool
operator== (Book const& lhs, Book const& rhs)
{
    return (lhs.in == rhs.in) &&
           (lhs.out == rhs.out);
}

bool
operator!= (Book const& lhs, Book const& rhs)
{
    return (lhs.in != rhs.in) ||
           (lhs.out != rhs.out);
}
/** @} */

/** Strict weak ordering. */
/** @{ */
bool
operator< (Book const& lhs,Book const& rhs)
{
    int const diff (compare (lhs.in, rhs.in));
    if (diff != 0)
        return diff < 0;
    return lhs.out < rhs.out;
}

bool
operator> (Book const& lhs, Book const& rhs)
{
    return rhs < lhs;
}

bool
operator>= (Book const& lhs, Book const& rhs)
{
    return ! (lhs < rhs);
}

bool
operator<= (Book const& lhs, Book const& rhs)
{
    return ! (rhs < lhs);
}

} // sdchain
