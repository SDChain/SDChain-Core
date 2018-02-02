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
    OutputDebugStringA ((text + "\n").c_str ());
}

//==============================================================================
std::string getComputerName()
{
    char text [MAX_COMPUTERNAME_LENGTH + 2] = { 0 };
    DWORD len = MAX_COMPUTERNAME_LENGTH + 1;
    if (!GetComputerNameA (text, &len))
        text[0] = 0;
    return text;
}

} // beast
