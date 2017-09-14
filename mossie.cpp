#include "mossie.h"

#include <mosquitto.h>
#include <mosquittopp.h>
#include <errno.h>
#include <string.h>
#include <logger.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <iostream>

namespace integra_utils
{
    /*static*/ string CMossie::sID;
    /*static*/ string CMossie::sHost("127.0.0.1");
    /*static*/ int    CMossie::sPort(1883);
    /*static*/ int    CMossie::sQos(2);
    /*static*/ bool   CMossie::sRetained(false);
    /*static*/ bool   CMossie::sCleanSession(false);

    /*static*/ CMossie *CMossie::pInstance(nullptr);

    /*static*/ CMossie &CMossie::instance()
    {
        if (! pInstance)
        {
            if (sID.empty())
                sID = processName();

            pInstance = new CMossie(sID, sCleanSession);
        }
        return *pInstance;
    }

    /*static*/ void CMossie::destroy()
    {
        if (pInstance)
        {
            delete pInstance;
            pInstance = nullptr;
        }
    }


    /*static*/ string CMossie::processName()
    {
        ifstream file("/proc/self/comm");
        string str;
        string procName;
        while (getline(file, str))
            procName += str;

        size_t nameStart = procName.find('-') + 1;
        if (nameStart < procName.length())
            procName = procName.substr(nameStart);

        return procName;
    }

    CMossie::CMossie(const string &_id, bool _clean)
        : mosqpp::mosquittopp(_id.c_str(), _clean)
        , mIsConnected(false)
        , mNotificationCallback(nullptr)
        , mConnectCallback(nullptr)
        , mDisconnectCallback(nullptr)
        , mSubscribeCallback(nullptr)
        , mUnsubscribeCallback(nullptr)
        , mPublishCallback(nullptr)
    {
        mosqpp::lib_init();
    }

    CMossie::~CMossie()
    {
        mosqpp::mosquittopp::loop_stop(true);
        mosqpp::lib_cleanup();
    }

    bool CMossie::connect()
    {
        if (mIsConnected)
            return true;

        int keepalive = 20; // seconds
        int ok = mosqpp::mosquittopp::connect_async(sHost.c_str(), sPort, keepalive);

        if (ok == MOSQ_ERR_SUCCESS)
            mosqpp::mosquittopp::loop_start();

        return checkResult(ok);
    }

    bool CMossie::disconnect()
    {
        if (! mIsConnected)
            return true;

        mIsConnected = false;
        int ok = mosqpp::mosquittopp::disconnect();

        return checkResult(ok);
    }

    bool CMossie::setWill(const string &_will, const string &_payload, int _qos, bool _retained)
    {
        if (mIsConnected)
        {
            mErrorStr = "Too late - connected to MQTT";
            return false;
        }

        int ok;
        if ((! _will.empty()) && (! _payload.empty()))
            ok = mosqpp::mosquittopp::will_set(_will.c_str(), _payload.length(), _payload.c_str(), _qos, _retained);

        else if (! _will.empty())
            ok = mosqpp::mosquittopp::will_set(_will.c_str(), 0, NULL, _qos, _retained);

        else
            ok = mosqpp::mosquittopp::will_clear();

        return checkResult(ok);
    }

    bool CMossie::clearWill()
    {
        if (! mIsConnected)
        {
            mErrorStr = "Not connected to MQTT";
            return false;
        }

        int ok = mosqpp::mosquittopp::will_clear();
        return checkResult(ok);
    }

    bool CMossie::send(const string &_topic, const string &_subTopic, const string &_str, int _qos, bool _retained)
    {
        if (! mIsConnected)
        {
            mErrorStr = "Not connected to MQTT";
            return false;
        }


        string topic = _topic;
        if (! _subTopic.empty())
            topic += "/" + _subTopic;

        int ok = mosqpp::mosquittopp::publish(NULL, topic.c_str(), _str.length(), _str.c_str(), _qos, _retained);

        return checkResult(ok);
    }

    bool CMossie::clearRetained(const string &_topic, const string &_subTopic)
    {
        if (! mIsConnected)
        {
            mErrorStr = "Not connected to MQTT";
            return false;
        }

        string topic = _topic;
        if (! _subTopic.empty())
            topic += "/" + _subTopic;

        int ok = mosqpp::mosquittopp::publish(NULL, topic.c_str(), 0, NULL, 0, true);

        return checkResult(ok);
    }

    bool CMossie::subscribe(const string &_topic, int _qos)
    {
        if (! mIsConnected)
        {
            mErrorStr = "Not connected to MQTT";
            return false;
        }

        int ok = mosqpp::mosquittopp::subscribe(NULL, _topic.c_str(), _qos);

        return checkResult(ok);
    }

    bool CMossie::unsubscribe(const string &_topic)
    {
        if (! mIsConnected)
        {
            mErrorStr = "Not connected to MQTT";
            return false;
        }

        int ok = mosqpp::mosquittopp::unsubscribe(NULL, _topic.c_str());

        return checkResult(ok);
    }

    void CMossie::on_connect(int _rc)
    {
        mIsConnected = checkResult(_rc);

        CLogger::instance().writeSyslogEntry(CLogger::Level::Information, "Connected to Mosquitto");

        if (mConnectCallback)
            mConnectCallback(mIsConnected);
    }

    void CMossie::on_disconnect(int _rc)
    {
        if (! mIsConnected)
        {
            mIsConnected = ! checkResult(_rc);

            mosqpp::mosquittopp::loop_stop();
        }

        CLogger::instance().writeSyslogEntry(CLogger::Level::Information, "Disconnected from Mosquitto");
        if (mDisconnectCallback)
            mDisconnectCallback(mIsConnected);
    }

    void CMossie::on_message(const struct mosquitto_message *_pMessage)
    {
        if (_pMessage && mNotificationCallback)
        {
            if(_pMessage->payloadlen)
                mNotificationCallback(_pMessage->topic, (char*)_pMessage->payload, _pMessage->mid);

            else
                mNotificationCallback(_pMessage->topic, string(), _pMessage->mid);
        }
    }

    void CMossie::on_publish(int)
    {
        if(mPublishCallback)
            mPublishCallback();

        CLogger::instance().writeSyslogEntry(CLogger::Level::Debug, "Subscribed to Mosquitto");
    }

    void CMossie::on_subscribe(int, int, const int *)
    {
        if (mSubscribeCallback)
            mSubscribeCallback();

        CLogger::instance().writeSyslogEntry(CLogger::Level::Information, "Subscribed to Mosquitto");
    }

    void CMossie::on_unsubscribe(int)
    {
        if (mUnsubscribeCallback)
            mUnsubscribeCallback();

        CLogger::instance().writeSyslogEntry(CLogger::Level::Information, "Unsubscribed to Mosquitto");
    }

    void CMossie::on_error()
    {
        std::cout << "MQTT error" << std::endl;
        CLogger::instance().writeSyslogEntry(CLogger::Level::Information, "Mosquitto error");
    }

    void CMossie::on_log(int _level, const char *_pStr)
    {   }

    bool CMossie::checkResult(int _res)
    {
        switch (_res)
        {
        case MOSQ_ERR_SUCCESS:
            mErrorStr.clear();
            return true;
            break;
        case MOSQ_ERR_INVAL:
            mErrorStr = "Invalid parameters";
            break;
        case MOSQ_ERR_NOMEM:
            mErrorStr = "Out of memory";
            break;
        case MOSQ_ERR_NO_CONN:
            mErrorStr = "Not connected to broker";
            break;
        };

        return false;
    }
}
