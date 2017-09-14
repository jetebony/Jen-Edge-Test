#include "logger.h"
#include "mossie.h"
#include "datetimeutils.h"
#include <json-c/json.h>
#include <syslog.h>

namespace integra_utils
{

    /*static*/ CLogger *CLogger::pInstance = nullptr;

    /*static*/ const string CLogger::sAuditPrefix("audit");
    /*static*/ const string CLogger::sDebugPrefix("debug");
    /*static*/ string       CLogger::sModule;

    CLogger::CLogger()
        : mVerbose(false)
    {   }

    CLogger::~CLogger()
    {   }

    /*static*/ CLogger &CLogger::instance()
    {
        if (! pInstance)
            pInstance = new CLogger;

        return *pInstance;
    }

    void CLogger::writeLogEntry(Level         _level,
                                const string &_category,
                                const string &_action,
                                const string &_subTopic,
                                const string &_desc,
                                bool          _notify,
                                int           _oid)
    {
        if ((_level != Level::Debug) || (mVerbose))
        {
            json_object *json     = json_object_new_object();
            json_object *category = json_object_new_string(_category.c_str());
            json_object *module   = json_object_new_string(sModule.c_str());
            json_object *action   = json_object_new_string(_action.c_str());
            json_object *desc     = json_object_new_string(_desc.c_str());
            json_object *when     = json_object_new_string(CDateTime::currentDateTime().toString().c_str());
            json_object *level    = json_object_new_int(levelToInt(_level));
            json_object *notify   = json_object_new_boolean(_notify);

            if (_oid >= 0)
            {
                json_object *oid = json_object_new_int(_oid);
                json_object_object_add(json, "oid", oid);
            }

            json_object_object_add(json, "module",      module);
            json_object_object_add(json, "category",    category);
            json_object_object_add(json, "action",      action);
            json_object_object_add(json, "description", desc);
            json_object_object_add(json, "level",       level);
            json_object_object_add(json, "notify",      notify);
            json_object_object_add(json, "occured_at",  when);

            string str = json_object_to_json_string(json);
            json_object_put(json);

            if (_level == Level::Debug)
                CMossie::instance().send(sDebugPrefix + "/" + sModule, _subTopic, str);

            else
                CMossie::instance().send(sAuditPrefix + "/" + sModule, _subTopic, str);
        }
    }

    void CLogger::writeSyslogEntry(Level _level, const string &_desc)
    {
        if (_level == Level::Debug && mVerbose)
            syslog(LOG_DAEMON | LOG_DEBUG, "%s: %s\n", sModule.c_str(), _desc.c_str());
        else if (_level == Level::Critical)
            syslog(LOG_DAEMON | LOG_EMERG, "%s: %s\n", sModule.c_str(),  _desc.c_str());
        else if (_level == Level::Error)
            syslog(LOG_DAEMON | LOG_ERR, "%s: %s\n", sModule.c_str(),  _desc.c_str());
        else if (_level == Level::Warning)
            syslog(LOG_DAEMON | LOG_WARNING, "%s: %s\n", sModule.c_str(),  _desc.c_str());
        else if (_level == Level::Information)
            syslog(LOG_DAEMON | LOG_INFO, "%s: %s\n", sModule.c_str(),  _desc.c_str());
    }

    int CLogger::levelToInt(Level _level) const
    {
        switch (_level)
        {
        case Level::Critical:
            return 3;

        case Level::Error:
            return 2;

        case Level::Warning:
            return 1;

        case Level::Information:
            return 0;

        default:
            return 4;
        }
    }

    void CLogger::setVerbose(bool _verbose)
    {
        mVerbose = _verbose;

        if (mVerbose)
            writeLogEntry(Level::Information, "debug", "activated", string(), "Debug enabled");

        else
            writeLogEntry(Level::Information, "debug", "deactivated", string(),"Debug disabled");
    }

}
