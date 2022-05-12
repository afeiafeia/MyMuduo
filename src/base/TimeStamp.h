#ifndef TIME_STAMP_H
#define TIME_STAMP_H
#include <stdint.h>
namespace afa
{
    class TimeStamp
    {
    private:
        int64_t m_time;//unix时间戳
        static const int64_t time_unit = 1000;

    public:
        TimeStamp();
        TimeStamp(int64_t time);

        TimeStamp operator+(const TimeStamp &rhs);

        bool operator<(const TimeStamp &rhs);
        bool operator>(const TimeStamp &rhs);

        static int64_t Now();

        int64_t GetTime();

        void SetNewTime(int64_t new_time);

        

    };
}

#endif