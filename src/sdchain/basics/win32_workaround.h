//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2015, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#ifndef BEAST_WINDOWS_H_INCLUDED
#define BEAST_WINDOWS_H_INCLUDED

#ifdef _MSC_VER
#pragma push_macro("NOMINMAX")
#pragma push_macro("UNICODE")
#pragma push_macro("STRICT")
# ifndef NOMINMAX
#  define NOMINMAX
# endif
# ifndef UNICODE
#  define UNICODE
# endif
# ifndef STRICT
#  define STRICT
# endif
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# include <Windows.h>
#pragma pop_macro("STRICT")
#pragma pop_macro("UNICODE")
#pragma pop_macro("NOMINMAX")

#endif
#endif
