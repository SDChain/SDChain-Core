//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>



*/
//==============================================================================

#include <sdchain/beast/unit_test.h>

#include <sdchain/beast/clock/basic_seconds_clock.h>

namespace beast {

class basic_seconds_clock_test : public unit_test::suite
{
public:
    void
    run()
    {
        basic_seconds_clock <
            std::chrono::steady_clock>::now ();
        pass ();
    }
};

BEAST_DEFINE_TESTSUITE(basic_seconds_clock,chrono,beast);

}
