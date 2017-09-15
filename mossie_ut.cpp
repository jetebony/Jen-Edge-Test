#include "mossie.h"
#include <string>
#include <unistd.h>
#include <iostream>
#include <functional>

#ifdef UNITTEST
#   include "/opt/include/Catch/include/catch.hpp"

using namespace integra_utils;
using namespace std;
using namespace std::placeholders;

// --------------------  Unit tests  -------------------------------------

    string testMsg   = string();
    string testTopic = string();

    void mosquittoNotification(const string &_topic, const string &_msg, int)
    {
        testTopic = _topic;
        testMsg   = _msg;
    }

    TEST_CASE("CMossie processName", "[mosquitto processName]")
    {
        REQUIRE(CMossie::processName() == "utilstest");
    }

    TEST_CASE("CMossie host details", "[mosquitto host]")
    {
        REQUIRE(CMossie::hostAddr() == "127.0.0.1");
        REQUIRE(CMossie::hostPort() == 1883);
        REQUIRE(CMossie::qos()      == 2);
        REQUIRE(CMossie::id()       == "");
        REQUIRE(! CMossie::retained());

        CMossie::setHostAddr("192.168.0.1");
        CMossie::setHostPort(8080);
        CMossie::setQos(1);
        CMossie::setRetained(true);
        CMossie::instance();

        REQUIRE(CMossie::hostAddr() == "192.168.0.1");
        REQUIRE(CMossie::hostPort() == 8080);
        REQUIRE(CMossie::qos()      == 1);
        REQUIRE(CMossie::id()       == "utilstest");
        REQUIRE(CMossie::retained());

        CMossie::setHostAddr("10.10.10.10");
        CMossie::setHostPort(1234);
        CMossie::setQos(0);
        CMossie::setRetained(false);
        CMossie::setID("unittester");

        REQUIRE(CMossie::hostAddr() == "10.10.10.10");
        REQUIRE(CMossie::hostPort() == 1234);
        REQUIRE(CMossie::qos()      == 0);
        REQUIRE(CMossie::id()       == "unittester");
        REQUIRE(! CMossie::retained());
    }

    TEST_CASE("CMossie connection", "[mosquitto connect / disconnect")
    {
        CMossie::setHostAddr("127.0.0.1");
        CMossie::setHostPort(1883);

        for (int c = 0; c < 10; ++c)
        {
            REQUIRE(CMossie::instance().connect());
            int timeout = 10;
            while ((timeout > 0) && (! CMossie::instance().isConnected()))
            {
                usleep(50000);
                --timeout;
            }

            REQUIRE(CMossie::instance().isConnected());

            usleep(1000);

            REQUIRE(CMossie::instance().disconnect());
            timeout = 10;
            while ((timeout > 0) && (CMossie::instance().isConnected()))
            {
                usleep(50000);
                --timeout;
            }
            REQUIRE(! CMossie::instance().isConnected());

            usleep(1000);
        }
    }

    TEST_CASE("CMossie subscription", "[mosquitto subscribe / unsubscribe")
    {
        CMossie::setHostAddr("127.0.0.1");
        CMossie::setHostPort(1883);

        REQUIRE(! CMossie::instance().subscribe("testing/fail"));
        REQUIRE(CMossie::instance().errorStr() == "Not connected to MQTT");
        REQUIRE(CMossie::instance().connect());

        int timeout = 10;
        while ((timeout > 0) && (! CMossie::instance().isConnected()))
        {
            usleep(500000);
            --timeout;
        }

        REQUIRE(CMossie::instance().isConnected());

        for (int c = 0; c < 105; ++c)
        {
            REQUIRE(CMossie::instance().subscribe("testing_2"));
            usleep(100000);

            REQUIRE(CMossie::instance().unsubscribe("testing_2"));
            usleep(100000);
        }

        for (int c = 0; c < 10; ++c)
        {
            REQUIRE(CMossie::instance().subscribe("testing_1", 1));
            usleep(100000);

            REQUIRE(CMossie::instance().unsubscribe("testing_1"));
            usleep(100000);
        }

        for (int c = 0; c < 10; ++c)
        {
            REQUIRE(CMossie::instance().subscribe("testing_0", 0));
            usleep(100000);

            REQUIRE(CMossie::instance().unsubscribe("testing_0"));
            usleep(100000);
        }

        REQUIRE(CMossie::instance().unsubscribe("testing_not_subscribed"));

        REQUIRE(CMossie::instance().disconnect());
    }

    TEST_CASE("CMossie send", "[mosquitto send")
    {
        sleep(1);
        CMossie::setHostAddr("127.0.0.1");
        CMossie::setHostPort(1883);

        REQUIRE(CMossie::instance().connect());
        int timeout = 10;
        while ((timeout > 0) && (! CMossie::instance().isConnected()))
        {
            usleep(500000);
            --timeout;
        }

        REQUIRE(CMossie::instance().isConnected());
        CMossie::instance().setNotificationCallback(std::bind(mosquittoNotification, _1, _2, _3));

        REQUIRE(CMossie::instance().subscribe("unittest/info"));

        for (int c = 0; c < 500; ++c)
        {
            int timeout = 30;
            string msg("Testing-");
            msg += to_string(c);

            REQUIRE(CMossie::instance().send("unittest", "info", msg));

            usleep(5000);
            REQUIRE(testMsg   == msg);
            REQUIRE(testTopic == "unittest/info");
        }

        REQUIRE(CMossie::instance().disconnect());
    }

    TEST_CASE("CMossie wills", "[mosquitto wills]")
    {
        sleep(1);
        CMossie::setHostAddr("127.0.0.1");
        CMossie::setHostPort(1883);

        REQUIRE(! CMossie::instance().isConnected());
        REQUIRE(CMossie::instance().setWill("unittest/will", "This is a will"));

        REQUIRE(CMossie::instance().connect());
        int timeout = 10;
        while ((timeout > 0) && (! CMossie::instance().isConnected()))
        {
            usleep(500000);
            --timeout;
        }

        REQUIRE(CMossie::instance().isConnected());

        CMossie::destroy();
    }

    TEST_CASE("CMossie retained msgs", "[mosquitto retained]")
    {
        sleep(1);
        testMsg.clear();
        testTopic.clear();

        CMossie::setHostAddr("127.0.0.1");
        CMossie::setHostPort(1883);

        REQUIRE(CMossie::instance().connect());
        int timeout = 10;
        while ((timeout > 0) && (! CMossie::instance().isConnected()))
        {
            usleep(500000);
            --timeout;
        }
        REQUIRE(CMossie::instance().isConnected());

        CMossie::instance().setNotificationCallback(std::bind(mosquittoNotification, _1, _2, _3));

        string msg("Retained-Message");
        REQUIRE(CMossie::instance().send("unittest", "retained", msg, 2, true));

        REQUIRE(CMossie::instance().subscribe("unittest/retained"));

        sleep(1); // give time for the message to be sent
        REQUIRE(testMsg   == msg);
        REQUIRE(testTopic == "unittest/retained");

        testMsg.clear();
        testTopic.clear();

        REQUIRE(CMossie::instance().unsubscribe("unittest/retained"));
        REQUIRE(CMossie::instance().clearRetained("unittest", "retained"));

        REQUIRE(CMossie::instance().subscribe("unittest/retained"));

        usleep(5000);
        REQUIRE(testMsg.empty());
        REQUIRE(testTopic.empty());

        REQUIRE(CMossie::instance().disconnect());
    }

    TEST_CASE("CMossie clean session", "[mosquitto clean session]")
    {
        sleep(1);
        testMsg.clear();
        testTopic.clear();

        CMossie::destroy();
        CMossie::setHostAddr("127.0.0.1");
        CMossie::setHostPort(1883);
        CMossie::setCleanSession(false);

        REQUIRE(CMossie::instance().connect());
        int timeout = 10;
        while ((timeout > 0) && (! CMossie::instance().isConnected()))
        {
            usleep(500000);
            --timeout;
        }
        REQUIRE(CMossie::instance().isConnected());

        REQUIRE(CMossie::instance().subscribe("unittest/clean"));

        usleep(100000);

        CMossie::instance().disconnect();
        CMossie::destroy();

        CMossie::instance().setNotificationCallback(std::bind(mosquittoNotification, _1, _2, _3));
        REQUIRE(CMossie::instance().connect());
        timeout = 10;
        while ((timeout > 0) && (! CMossie::instance().isConnected()))
        {
            usleep(500000);
            --timeout;
        }
        REQUIRE(CMossie::instance().isConnected());

        string msg("CleanSession-Message");
        REQUIRE(CMossie::instance().send("unittest", "clean", msg));


        sleep(1); // give time for the message to be sent
        REQUIRE(testMsg   == msg);
        REQUIRE(testTopic == "unittest/clean");

        REQUIRE(CMossie::instance().disconnect());
    }

#endif



