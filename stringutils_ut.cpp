#include "stringutils.h"
#include <string>

#ifdef UNITTEST
#   include "../Catch/include/catch.hpp"

using namespace integra_utils;
using namespace std;

// --------------------  Unit tests  -------------------------------------

    TEST_CASE("CStringUtils::trim", "[trim]")
    {
        string leading("   leading whitespace");
        string trailing("trailing whitespace    ");
        string centre("   centre   ");
        string tab("\tTab");
        string lf("Line Feed\n");
        string cr("Carriage Return\r");

        REQUIRE(trim(leading)  == "leading whitespace");
        REQUIRE(trim(trailing) == "trailing whitespace");
        REQUIRE(trim(centre)   == "centre");
        REQUIRE(trim(tab)      == "Tab");
        REQUIRE(trim(lf)       == "Line Feed");
        REQUIRE(trim(cr)       == "Carriage Return");
    }

    TEST_CASE("CStringUtils::split", "[split]")
    {
        string spl("One,Two,Three,Four,,Six");

        TStringList list = split(spl, ',');

        REQUIRE(list.size() == 6);
        REQUIRE(list.at(0) == "One");
        REQUIRE(list.at(1) == "Two");
        REQUIRE(list.at(2) == "Three");
        REQUIRE(list.at(3) == "Four");
        REQUIRE(list.at(4) == "");
        REQUIRE(list.at(5) == "Six");
    }

    TEST_CASE("CStringUtils::escapeChars", "[escape]")
    {
        string chars("{ \"json\": \"test\" }");

        REQUIRE(escapeChars('"',chars) == "{ \\\"json\\\": \\\"test\\\" }");
    }

    //    TEST_CASE("Byte to integer conversion", "[byte to int]")
    //    {
    //        CIDirectModem dev;

    //        char bytes[] = { (char)0xff, (char)0xff, (char)0xff, (char)0x06 };

    //        REQUIRE(dev.bytesToInt32(&bytes[0]) == -250);
    //    }

    //    TEST_CASE("Byte to float conversion", "[byte to float]")
    //    {
    //        CIDirectModem dev;

    //        char bytes[] = { (char)0x41, (char)0xd4, (char)0x00, (char)0x00 };

    //        REQUIRE((int)(dev.bytesToFloat(&bytes[0])*10) == 265);
    //    }

#endif



