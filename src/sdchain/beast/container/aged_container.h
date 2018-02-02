//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#ifndef BEAST_CONTAINER_AGED_CONTAINER_H_INCLUDED
#define BEAST_CONTAINER_AGED_CONTAINER_H_INCLUDED

#include <type_traits>

namespace beast {

template <class T>
struct is_aged_container
    : std::false_type
{
};

}

#endif
