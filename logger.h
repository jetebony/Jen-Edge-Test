#ifndef CLOGGER_H
#define CLOGGER_H

#include "def.h"
#include <string>

using namespace std;

namespace integra_utils
{
    class LIBRARY_EXPORT CLogger
    {
        CLogger();
        ~CLogger();

    public:

        enum class Level
        {
            Information,
            Warning,
            Error,
            Critical,
            Debug
        };

        void setVerbose(bool _verbose);
        bool verbose() const { return mVerbose; }

        void writeLogEntry(Level         _level,
                           const string &_category,
                           const string &_action,
                           const string &_subTopic,
                           const string &_desc,
                           bool          _notify = false,
                           int           _id     = -1);

        void writeSyslogEntry(Level _level, const string &_desc);

    public:

        static CLogger &instance();

        static void setModule(const string &_mod) { sModule = _mod; }

    private:

        int levelToInt(Level _level) const;

#ifdef UNITTEST
    public:
#else
    private:
#endif

        static CLogger *pInstance;

        static const string sAuditPrefix;
        static const string sDebugPrefix;
        static       string sModule;

    private:

        bool         mVerbose;
    };
}

#endif // CLOGGER_H
