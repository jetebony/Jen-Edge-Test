#include "datetimeutils.h"
#include <string>

#ifdef UNITTEST
#   include "/opt/include/Catch/include/catch.hpp"

using namespace integra_utils;
using namespace std;

// --------------------  Unit tests  -------------------------------------

TEST_CASE("CDateTimeUtils::toString", "[toString]")
{
    CDateTime dt;

    dt.setDateTime(01, 02, 2017, 13, 36, 23, 123);

    REQUIRE(dt.toString() == "2017-02-01T13:36:23.123Z");
}

//TEST_CASE("CDateTimeUtils::currentDateTime", "[now]")
//{
//    REQUIRE(CDateTime::currentDateTime().toString() == "2017-02-01T13:36:23.123Z");
//}


#endif



