#include "logger.h"
#include <string>

#ifdef UNITTEST
#   include "/opt/include/Catch/include/catch.hpp"

using namespace integra_utils;
using namespace std;

// --------------------  Unit tests  -------------------------------------

    TEST_CASE("CLogger verbose", "[logger verbose]")
    {
        REQUIRE(! CLogger::instance().verbose());
        CLogger::instance().setVerbose(true);
        REQUIRE(CLogger::instance().verbose());
        CLogger::instance().setVerbose(false);
        REQUIRE(! CLogger::instance().verbose());
        CLogger::instance().setVerbose(true);
        REQUIRE(CLogger::instance().verbose());
        CLogger::instance().setVerbose(false);
        REQUIRE(! CLogger::instance().verbose());
    }

    TEST_CASE("CLogger module", "[logger module]")
    {
        REQUIRE(CLogger::sModule == "");
        CLogger::setModule("Test");
        REQUIRE(CLogger::sModule == "Test");
        CLogger::setModule("1234");
        REQUIRE(CLogger::sModule == "1234");
        CLogger::setModule("linkstate");
        REQUIRE(CLogger::sModule == "linkstate");
    }

#endif



