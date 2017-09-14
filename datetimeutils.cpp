#include "datetimeutils.h"

#include <stdlib.h>
#include <string.h>
#include <iostream>

namespace integra_utils
{
    CDateTime::CDateTime()
    {
        mTimeSpec.tv_nsec = 0;
        mTimeSpec.tv_sec  = 0;
    }

    void CDateTime::copy(const CDateTime &_dt)
    {
        mTimeSpec.tv_sec  = _dt.mTimeSpec.tv_sec;
        mTimeSpec.tv_nsec = _dt.mTimeSpec.tv_nsec;
    }

    void CDateTime::setDateTime(int _day,  int _month, int _year, int _hour, int _min, int _sec, int _msec)
    {
        struct tm timeinfo;

        timeinfo.tm_sec  = _sec;
        timeinfo.tm_min	 = _min;
        timeinfo.tm_hour = _hour;
        timeinfo.tm_mday = _day;          // day of the month (1-31)
        timeinfo.tm_mon	 = _month - 1;    // months since January (0-11)
        timeinfo.tm_year = _year  - 1900; // years since 1900
        timeinfo.tm_isdst = -1;

        mTimeSpec.tv_sec  = mktime(&timeinfo);
        mTimeSpec.tv_nsec = _msec * 1000000;
    }

    void CDateTime::setTimespec(timespec &_ts)
    {
        mTimeSpec.tv_nsec = _ts.tv_nsec;
        mTimeSpec.tv_sec  = _ts.tv_sec;
    }

    string CDateTime::toString() const
    {
        // e.g 2017-01-31T13:46:57.000Z

        string str;
        char   buff[25] = {0};
        struct tm t;

        if (localtime_r(&(mTimeSpec.tv_sec), &t) != NULL)
        {
            int ret = strftime(buff, 25, "%FT%T", &t);
            if (ret > 0)
                snprintf(&buff[strlen(buff)], 5, ".%03ld", mTimeSpec.tv_nsec);

            str = buff;
            str += "Z";
        }

        return str;
    }

    /*static*/ CDateTime CDateTime::currentDateTime()
    {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);

        CDateTime dt;
        dt.setTimespec(ts);

        return dt;
    }
}
