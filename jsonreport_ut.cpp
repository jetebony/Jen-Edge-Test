#include "jsonreport.h"

#ifdef UNITTEST
#   include "/opt/include/Catch/include/catch.hpp"

using namespace integra_utils;

// --------------------  Unit tests  -------------------------------------


    TEST_CASE("CJsonReport::CWarning::accessors", "[warning accessors]")
    {
        CJsonReport::CWarning warn;

        REQUIRE(warn.description() == "");
        REQUIRE(warn.message()     == "");
        REQUIRE(warn.level()       == -1);

        warn.setDescription("Description");
        warn.setMessage("Message");
        warn.setLevel(3);

        REQUIRE(warn.description() == "Description");
        REQUIRE(warn.message()     == "Message");
        REQUIRE(warn.level()       == 3);
    }

    TEST_CASE("CJsonReport::CWarning::copy", "[warning copy]")
    {
        CJsonReport::CWarning warn1;

        warn1.setDescription("Description");
        warn1.setMessage("Message");
        warn1.setLevel(3);

        CJsonReport::CWarning warn2;
        warn2.copy(warn1);

        REQUIRE(warn2.description() == "Description");
        REQUIRE(warn2.message()     == "Message");
        REQUIRE(warn2.level()       == 3);

        CJsonReport::CWarning warn3;
        warn3 = warn1;

        REQUIRE(warn3.description() == "Description");
        REQUIRE(warn3.message()     == "Message");
        REQUIRE(warn3.level()       == 3);

        CJsonReport::CWarning warn4(warn1);

        REQUIRE(warn4.description() == "Description");
        REQUIRE(warn4.message()     == "Message");
        REQUIRE(warn4.level()       == 3);
    }

    TEST_CASE("CJsonReport::CError::accessors", "[error accessors]")
    {
        CJsonReport::CError err;

        REQUIRE(err.description() == "");
        REQUIRE(err.message()     == "");

        err.setDescription("Description");
        err.setMessage("Message");

        REQUIRE(err.description() == "Description");
        REQUIRE(err.message()     == "Message");
    }

    TEST_CASE("CJsonReport::CError::copy", "[error copy]")
    {
        CJsonReport::CError err1;

        err1.setDescription("Description");
        err1.setMessage("Message");

        CJsonReport::CError err2;
        err2.copy(err1);

        REQUIRE(err2.description() == "Description");
        REQUIRE(err2.message()     == "Message");

        CJsonReport::CError err3;
        err3 = err1;

        REQUIRE(err3.description() == "Description");
        REQUIRE(err3.message()     == "Message");

        CJsonReport::CError err4(err1);

        REQUIRE(err4.description() == "Description");
        REQUIRE(err4.message()     == "Message");
    }

    TEST_CASE("CJsonReport::addData", "[addData]")
    {
        CJsonReport::TStringPairList list;
        CJsonReport::TStringPair     pair("one", "two");
        CJsonReport rpt;

        list.push_back(pair);
        rpt.addObject();
        rpt.addData("testInt",    5);
        rpt.addData("testDouble", 4.5);
        rpt.addData("testBool",   true);
        rpt.addData("testString", "this is a test");
        rpt.addData("testPair",   list);

        REQUIRE(rpt.toString() == "{ \"success\": true, \"errors\": [ ], \"warnings\": [ ], \"data\": [ { \"testInt\": 5, \"testBool\": true, \"testString\": true, \"testDouble\": 4.500000, \"testPair\": { \"one\": \"two\" } } ] }");
    }

    TEST_CASE("CJsonReport::addMultiData", "[addMultipleData]")
    {
        CJsonReport::TStringPairList list;
        CJsonReport::TStringPair     pair("one", "two");
        CJsonReport rpt;

        list.push_back(pair);
        rpt.addObject();
        rpt.addData("testInt",    5);
        rpt.addData("testDouble", 4.5);
        rpt.addData("testBool",   true);
        rpt.addData("testString", "this is a test");
        rpt.addData("testPair",   list);

        rpt.addObject();
        rpt.addData("testInt",    50);
        rpt.addData("testDouble", 40.5);
        rpt.addData("testBool",   false);
        rpt.addData("testString", "this is another test");
        rpt.addData("testPair",   list);

        REQUIRE(rpt.toString() == "{ \"success\": true, \"errors\": [ ], \"warnings\": [ ], "
                                  "\"data\": [ { \"testInt\": 5, \"testBool\": true, \"testString\": "
                                  "true, \"testDouble\": 4.500000, \"testPair\": { \"one\": "
                                  "\"two\" } }, { \"testInt\": 50, \"testBool\": false, \"testString\": "
                                  "true, \"testDouble\": 40.500000, \"testPair\": { \"one\": \"two\" } } ] }");
    }

    TEST_CASE("CJsonReport::multiDataCopy", "[copyMultipleData]")
    {
        CJsonReport::TStringPairList list;
        CJsonReport::TStringPair     pair("one", "two");
        CJsonReport rpt;

        list.push_back(pair);
        rpt.setSuccess(true);
        rpt.addObject();
        rpt.addData("testInt",    5);
        rpt.addData("testDouble", 4.5);
        rpt.addData("testBool",   true);
        rpt.addData("testString", "this is a test");
        rpt.addData("testPair",   list);

        rpt.addObject();
        rpt.addData("testInt",    50);
        rpt.addData("testDouble", 40.5);
        rpt.addData("testBool",   false);
        rpt.addData("testString", "this is another test");
        rpt.addData("testPair",   list);

        CJsonReport rpt2(rpt);
        CJsonReport rpt3 = rpt;
        CJsonReport rpt4;
        rpt4.copy(rpt);

        REQUIRE(rpt.toString() == "{ \"success\": true, \"errors\": [ ], \"warnings\": [ ], "
                                  "\"data\": [ { \"testInt\": 5, \"testBool\": true, \"testString\": "
                                  "true, \"testDouble\": 4.500000, \"testPair\": { \"one\": "
                                  "\"two\" } }, { \"testInt\": 50, \"testBool\": false, \"testString\": "
                                  "true, \"testDouble\": 40.500000, \"testPair\": { \"one\": \"two\" } } ] }");
        REQUIRE(rpt2.toString() == "{ \"success\": true, \"errors\": [ ], \"warnings\": [ ], "
                                  "\"data\": [ { \"testInt\": 5, \"testBool\": true, \"testString\": "
                                  "true, \"testDouble\": 4.500000, \"testPair\": { \"one\": "
                                  "\"two\" } }, { \"testInt\": 50, \"testBool\": false, \"testString\": "
                                  "true, \"testDouble\": 40.500000, \"testPair\": { \"one\": \"two\" } } ] }");
        REQUIRE(rpt3.toString() == "{ \"success\": true, \"errors\": [ ], \"warnings\": [ ], "
                                  "\"data\": [ { \"testInt\": 5, \"testBool\": true, \"testString\": "
                                  "true, \"testDouble\": 4.500000, \"testPair\": { \"one\": "
                                  "\"two\" } }, { \"testInt\": 50, \"testBool\": false, \"testString\": "
                                  "true, \"testDouble\": 40.500000, \"testPair\": { \"one\": \"two\" } } ] }");
        REQUIRE(rpt4.toString() == "{ \"success\": true, \"errors\": [ ], \"warnings\": [ ], "
                                  "\"data\": [ { \"testInt\": 5, \"testBool\": true, \"testString\": "
                                  "true, \"testDouble\": 4.500000, \"testPair\": { \"one\": "
                                  "\"two\" } }, { \"testInt\": 50, \"testBool\": false, \"testString\": "
                                  "true, \"testDouble\": 40.500000, \"testPair\": { \"one\": \"two\" } } ] }");
    }

    TEST_CASE("CJsonReport::clear", "[clear]")
    {
        CJsonReport rpt;

        rpt.addObject();
        rpt.addData("testInt",    5);
        rpt.addData("testDouble", 4.5);

        rpt.addObject();
        rpt.addData("testInt",    50);
        rpt.addData("testDouble", 40.5);

        REQUIRE(rpt.toString() == "{ \"success\": true, \"errors\": [ ], \"warnings\": [ ], "
                                  "\"data\": [ { \"testInt\": 5, \"testDouble\": 4.500000 }, "
                                  "{ \"testInt\": 50, \"testDouble\": 40.500000 } ] }");

        rpt.clear();
        REQUIRE(rpt.toString() == "{ \"success\": false, \"errors\": [ ], \"warnings\": [ ], "
                                  "\"data\": [ \"\" ] }");
    }

#endif



