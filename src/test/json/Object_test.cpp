//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/json/Object.h>
#include <test/json/TestOutputSuite.h>
#include <sdchain/beast/unit_test.h>

namespace Json {

class JsonObject_test : public sdchain::test::TestOutputSuite
{
    void setup (std::string const& testName)
    {
        testcase (testName);
        output_.clear ();
    }

    std::unique_ptr<WriterObject> writerObject_;

    Object& makeRoot()
    {
        writerObject_ = std::make_unique<WriterObject> (
            stringWriterObject (output_));
        return **writerObject_;
    }

    void expectResult (std::string const& expected)
    {
        writerObject_.reset();
        TestOutputSuite::expectResult (expected);
    }

public:
    void testTrivial ()
    {
        setup ("trivial");

        {
            auto& root = makeRoot();
            (void) root;
        }
        expectResult ("{}");
    }

    void testSimple ()
    {
        setup ("simple");
        {
            auto& root = makeRoot();
            root["hello"] = "world";
            root["skidoo"] = 23;
            root["awake"] = false;
            root["temperature"] = 98.6;
        }

        expectResult (
            "{\"hello\":\"world\","
            "\"skidoo\":23,"
            "\"awake\":false,"
            "\"temperature\":98.6}");
    }

    void testOneSub ()
    {
        setup ("oneSub");
        {
            auto& root = makeRoot();
            root.setArray ("ar");
        }
        expectResult ("{\"ar\":[]}");
    }

    void testSubs ()
    {
        setup ("subs");
        {
            auto& root = makeRoot();

            {
                // Add an array with three entries.
                auto array = root.setArray ("ar");
                array.append (23);
                array.append (false);
                array.append (23.5);
            }

            {
                // Add an object with one entry.
                auto obj = root.setObject ("obj");
                obj["hello"] = "world";
            }

            {
                // Add another object with two entries.
                Json::Value value;
                value["h"] = "w";
                value["f"] = false;
                root["obj2"] = value;
            }
        }

        // Json::Value has an unstable order...
        auto case1 = "{\"ar\":[23,false,23.5],"
                "\"obj\":{\"hello\":\"world\"},"
                "\"obj2\":{\"h\":\"w\",\"f\":false}}";
        auto case2 = "{\"ar\":[23,false,23.5],"
                "\"obj\":{\"hello\":\"world\"},"
                "\"obj2\":{\"f\":false,\"h\":\"w\"}}";
        writerObject_.reset();
        BEAST_EXPECT(output_ == case1 || output_ == case2);
    }

    void testSubsShort ()
    {
        setup ("subsShort");

        {
            auto& root = makeRoot();

            {
                // Add an array with three entries.
                auto array = root.setArray ("ar");
                array.append (23);
                array.append (false);
                array.append (23.5);
            }

            // Add an object with one entry.
            root.setObject ("obj")["hello"] = "world";

            {
                // Add another object with two entries.
                auto object = root.setObject ("obj2");
                object.set("h", "w");
                object.set("f", false);
            }
        }
        expectResult (
            "{\"ar\":[23,false,23.5],"
            "\"obj\":{\"hello\":\"world\"},"
            "\"obj2\":{\"h\":\"w\",\"f\":false}}");
    }

    void testFailureObject()
    {
        {
            setup ("object failure assign");
            auto& root = makeRoot();
            auto obj = root.setObject ("o1");
            expectException ([&]() { root["fail"] = "complete"; });
        }
        {
            setup ("object failure object");
            auto& root = makeRoot();
            auto obj = root.setObject ("o1");
            expectException ([&] () { root.setObject ("o2"); });
        }
        {
            setup ("object failure Array");
            auto& root = makeRoot();
            auto obj = root.setArray ("o1");
            expectException ([&] () { root.setArray ("o2"); });
        }
    }

    void testFailureArray()
    {
        {
            setup ("array failure append");
            auto& root = makeRoot();
            auto array = root.setArray ("array");
            auto subarray = array.appendArray ();
            auto fail = [&]() { array.append ("fail"); };
            expectException (fail);
        }
        {
            setup ("array failure appendArray");
            auto& root = makeRoot();
            auto array = root.setArray ("array");
            auto subarray = array.appendArray ();
            auto fail = [&]() { array.appendArray (); };
            expectException (fail);
        }
        {
            setup ("array failure appendObject");
            auto& root = makeRoot();
            auto array = root.setArray ("array");
            auto subarray = array.appendArray ();
            auto fail = [&]() { array.appendObject (); };
            expectException (fail);
        }
    }

    void testKeyFailure ()
    {
#ifdef DEBUG
        setup ("repeating keys");
        auto& root = makeRoot();
        root.set ("foo", "bar");
        root.set ("baz", 0);
        auto fail = [&]() { root.set ("foo", "bar"); };
        expectException (fail);
#endif
    }

    void run () override
    {
        testTrivial ();
        testSimple ();

        testOneSub ();
        testSubs ();
        testSubsShort ();

        testFailureObject ();
        testFailureArray ();
        testKeyFailure ();
    }
};

BEAST_DEFINE_TESTSUITE(JsonObject, sdchain_basics, sdchain);

} // Json
