#ifndef CMOSQUITTO_H
#define CMOSQUITTO_H

#include "def.h"

#include <string>
#include <functional>
#include <map>
#include <mosquittopp.h>

#include "stringutils.h"

using namespace std;

namespace integra_utils
{
    class LIBRARY_EXPORT CMossie : protected mosqpp::mosquittopp
    {
        CMossie(const string &_id, bool _clean = true);
        CMossie(const CMossie &);

        CMossie &operator=(const CMossie &m);

    public:

        static CMossie &instance();
        static void     destroy();

        static string processName();

    public:

        ~CMossie();

        bool connect();
        bool disconnect();

        bool isConnected()  const { return mIsConnected; }

        bool setWill(const string &_will = string(), const string &_payload = string(), int _qos = sQos, bool _retained = sRetained);
        bool clearWill();


        bool subscribe(const   string &_topic, int _qos = sQos);
        bool unsubscribe(const string &_topic);

        bool send(const string &_topic, const string &_subTopic = string(), const string &_str = string(), int _qos = sQos, bool _retained = sRetained);

        bool clearRetained(const string &_topic, const string &_subTopic = string());

        const string &errorStr() const { return mErrorStr; }

        void setNotificationCallback(std::function<void (const string&, const string&, int)> _cb)  { mNotificationCallback = _cb; }
        void setConnectCallback(std::function<void     (bool)> _cb)                                { mConnectCallback      = _cb; }
        void setDisConnectCallback(std::function<void  (bool)> _cb)                                { mDisconnectCallback   = _cb; }
        void setSubscribeCallback(std::function<void   (void)> _cb)                                { mSubscribeCallback    = _cb; }
        void setUnsubscribeCallback(std::function<void (void)> _cb)                                { mUnsubscribeCallback  = _cb; }
        void setPublisheCallback(std::function<void    (void)> _cb)                                { mPublishCallback      = _cb; }

        void on_connect(int    _rc);
        void on_disconnect(int _rc);
        void on_message(const struct mosquitto_message *message);
        void on_subscribe(int, int, const int *);
        void on_unsubscribe(int);
        void on_publish(int);
        void on_error();
        void on_log(int _level, const char *_pStr);

    private:

        bool checkResult(int _res);

    public:


        static void setQos(int                 _qos)      { sQos          = _qos;      }
        static void setRetained(bool           _retained) { sRetained     = _retained; }
        static void setCleanSession(bool       _clean)    { sCleanSession = _clean;    }
        static void setID(const        string &_id)       { sID           = _id;       }
        static void setHostAddr(const  string &_addr)     { sHost         = _addr;     }
        static void setHostPort(int            _port)     { sPort         = _port;     }

        static int           qos()          { return sQos;          }
        static bool          retained()     { return sRetained;     }
        static bool          cleanSession() { return sCleanSession; }
        static const string &id()           { return sID;           }
        static const string &hostAddr()     { return sHost;         }
        static int           hostPort()     { return sPort;         }

    private:

        static CMossie   *pInstance;
        static string     sID;
        static string     sHost;
        static int        sPort;
        static int        sQos;
        static bool       sRetained;
        static bool       sCleanSession;

    private:

        string mErrorStr;

        bool   mIsConnected;

        std::function<void(const string&, const string&, int)> mNotificationCallback;
        std::function<void(bool)>                              mConnectCallback;
        std::function<void(bool)>                              mDisconnectCallback;
        std::function<void()>                                  mSubscribeCallback;
        std::function<void()>                                  mUnsubscribeCallback;
        std::function<void()>                                  mPublishCallback;
    };
}

#endif // CMOSQUITTO_H
