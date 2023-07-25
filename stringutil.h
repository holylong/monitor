#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_

#include <QString>

namespace feiker{
    class StringUtil{
        public:
        static bool StringFuzzySearch(const QString& s, const QString& ch){
            size_t it = s.indexOf(ch);
            if(it != -1)
                return true;

            return false;
        }

        static QString FormatToTime(qint64 ms, qint64 ss, qint64 mm, qint64 hh, qint64 dd){
            qint64 day = ms / dd;
            qint64 hour = (ms-day*dd)/hh;
            qint64 minute = (ms - day*dd)-hour;

            return "";
        }
    };
}

#endif //_STRING_UTIL_H_
