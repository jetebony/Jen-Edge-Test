#ifndef DATETIMEUTILS_H
#define DATETIMEUTILS_H

#include "def.h"
#include <string>
#include <time.h>

using namespace std;

namespace integra_utils
{
    /**
     * @brief The CDateTime class
     * Based on ISO 8601 date time format, with decimal seconds and no time zone information (UTC only)
     */
    class LIBRARY_EXPORT CDateTime
    {
    public:

        CDateTime();
        CDateTime(const CDateTime &_dt)  { copy(_dt); }

        void copy(const CDateTime &_dt);

        void setDateTime(int _day,  int _month, int _year, int _hour, int _min,   int _sec, int _msec = 0);

        string toString() const;

        CDateTime &operator=(const CDateTime &_dt) { copy(_dt); return *this; }

    public:

        static CDateTime currentDateTime();

    private:

        void setTimespec(struct timespec &_ts);

    private:

        struct timespec mTimeSpec;
    };
}

#endif // DATETIMEUTILS_H
