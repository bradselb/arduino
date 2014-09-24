#if !defined DS3231_H
#define DS3231_H

#include "Rtc.h"
#include "TempSensor.h"
#include "I2CDevice.h"

// represents the (Maxim) Dallas Semiconductor DS3231 Real Time Clock IC
// which has a temp sensor (for temperture compensation) in addition to the rtc. 
class DS3231 : public Rtc, public TempSensor
{
  public: 
    DS3231(unsigned int deviceId = 0x68); 
    ~DS3231();

    // the RTC interface:
    int getTime(struct tm*) const;
    int setTime(const struct tm*);

    // the TempSensor interface
    float getTemp();

    bool isPresent(void) const;

  private:
    I2CDevice m_i2c;

};


#endif // !defined DS3231_H
