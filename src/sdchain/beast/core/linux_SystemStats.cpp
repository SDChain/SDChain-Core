//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>

    Portions of this file are from JUCE.
    Copyright (c) 2013 - Raw Material Software Ltd.
    Please visit http://www.juce.com



*/
//==============================================================================

namespace beast
{

void outputDebugString (std::string const& text)
{
    std::cerr << text << std::endl;
}

//==============================================================================
std::string getComputerName()
{
    char name [256] = { 0 };
    if (gethostname (name, sizeof (name) - 1) == 0)
        return name;

    return std::string{};
}

} // beast
