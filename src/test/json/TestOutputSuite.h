//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_RPC_TESTOUTPUTSUITE_H_INCLUDED
#define SDCHAIN_RPC_TESTOUTPUTSUITE_H_INCLUDED

#include <sdchain/json/Output.h>
#include <sdchain/json/Writer.h>
#include <test/jtx/TestSuite.h>

namespace sdchain {
namespace test {

class TestOutputSuite : public TestSuite
{
protected:
    std::string output_;
    std::unique_ptr <Json::Writer> writer_;

    void setup (std::string const& testName)
    {
        testcase (testName);
        output_.clear ();
        writer_ = std::make_unique <Json::Writer> (
            Json::stringOutput (output_));
    }

    // Test the result and report values.
    void expectResult (std::string const& expected,
                       std::string const& message = "")
    {
        writer_.reset ();

        expectEquals (output_, expected, message);
    }
};

} // test
} // sdchain

#endif
