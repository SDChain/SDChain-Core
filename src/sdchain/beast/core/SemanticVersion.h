//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#ifndef BEAST_MODULE_CORE_DIAGNOSTIC_SEMANTICVERSION_H_INCLUDED
#define BEAST_MODULE_CORE_DIAGNOSTIC_SEMANTICVERSION_H_INCLUDED

#include <vector>
#include <string>

namespace beast {

/** A Semantic Version number.

    Identifies the build of a particular version of software using
    the Semantic Versioning Specification described here:

    http://semver.org/
*/
class SemanticVersion
{
public:
    using identifier_list = std::vector<std::string>;

    int majorVersion;
    int minorVersion;
    int patchVersion;

    identifier_list preReleaseIdentifiers;
    identifier_list metaData;

    SemanticVersion ();

    SemanticVersion (std::string const& version);

    /** Parse a semantic version string.
        The parsing is as strict as possible.
        @return `true` if the string was parsed.
    */
    bool parse (std::string const& input);

    /** Produce a string from semantic version components. */
    std::string print () const;

    inline bool isRelease () const noexcept
    {
        return preReleaseIdentifiers.empty();
    }
    inline bool isPreRelease () const noexcept
    {
        return !isRelease ();
    }
};

/** Compare two SemanticVersions against each other.
    The comparison follows the rules as per the specification.
*/
int compare (SemanticVersion const& lhs, SemanticVersion const& rhs);

inline bool
operator== (SemanticVersion const& lhs, SemanticVersion const& rhs)
{
    return compare (lhs, rhs) == 0;
}

inline bool
operator!= (SemanticVersion const& lhs, SemanticVersion const& rhs)
{
    return compare (lhs, rhs) != 0;
}

inline bool
operator>= (SemanticVersion const& lhs, SemanticVersion const& rhs)
{
    return compare (lhs, rhs) >= 0;
}

inline bool
operator<= (SemanticVersion const& lhs, SemanticVersion const& rhs)
{
    return compare (lhs, rhs) <= 0;
}

inline bool
operator>  (SemanticVersion const& lhs, SemanticVersion const& rhs)
{
    return compare (lhs, rhs) >  0;
}

inline bool
operator<  (SemanticVersion const& lhs, SemanticVersion const& rhs)
{
    return compare (lhs, rhs) <  0;
}

} // beast

#endif
