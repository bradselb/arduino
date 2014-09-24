#if !defined RTC_H

// a pure abstract base class to define the interface to a
// Real Time Clock IC.

struct tm;  // include <time.h>

class Rtc
{
  public: 
    virtual ~Rtc() {}

    virtual int getTime(struct tm*) const = 0;
    virtual int setTime(const struct tm*) = 0;

    //virtual int getAlarm(struct tm*) const = 0;
    //virtual int setAlarm(const struct tm*) = 0;
};

#define RTC_H
#endif // !defined RTC_H
