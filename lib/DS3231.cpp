#include "DS3231.h"
#include "Rtc.h"
#include "TempSensor.h"
#include "I2CDevice.h"
#include "tm.h" // struct tm
#include <string.h> // memset()

unsigned char bcd_to_int(unsigned char);
unsigned char int_to_bcd(unsigned char);



DS3231::DS3231(unsigned int deviceId) : m_i2c(deviceId)
{
}

DS3231::~DS3231()
{
}


int DS3231::getTime(struct tm* tm) const
{
    int rc = -1;

    if (this->isPresent()) {
        int bytesReceived;
        int bytesRequested;
        unsigned char buf[8];

        memset(&buf[0], 0, sizeof buf);

        bytesRequested = 7;
        bytesReceived = m_i2c.read(0, &buf[0], bytesRequested);
        if (bytesReceived >= bytesRequested) {
            rc = 0;
            if (tm != 0) {
                tm->tm_sec = bcd_to_int(buf[0]);
                tm->tm_min = bcd_to_int(buf[1]);
                tm->tm_hour = bcd_to_int(buf[2] & 0x3f);
                tm->tm_wday = bcd_to_int(buf[3] - 1);
                tm->tm_mday = bcd_to_int(buf[4]);
                tm->tm_mon = bcd_to_int((buf[5] & 0x7f) - 1);
                // years since 1900
                tm->tm_year = bcd_to_int(buf[6]);
                if (buf[5] & 0x080) {
                    tm->tm_year += 100;
                }
            }
        }
    }

    return rc;
}


int DS3231::setTime(const struct tm* tm)
{
    int rc;
    unsigned int reg;
    unsigned char val;

    rc = -1; // not-success
    if (tm != 0 && this->isPresent()) {
        //seconds
        reg = 0;
        val = int_to_bcd(tm->tm_sec) & 0x7f;
        m_i2c.write(reg, val);

        //minutes
        reg = 1;
        val = int_to_bcd(tm->tm_min) & 0x7f;
        m_i2c.write(reg, val);

        //hours
        reg = 2;
        val = int_to_bcd(tm->tm_hour) & 0x3f; // forces 24hr mode.
        m_i2c.write(reg, val);

        // day of week
        reg = 3;
        val = (int_to_bcd(tm->tm_wday) + 1) & 0x07;
        m_i2c.write(reg, val);

        // day of month
        reg = 4;
        val = int_to_bcd(tm->tm_mday) & 0x3f;
        m_i2c.write(reg, val);

        //month
        reg = 5;
        val = (int_to_bcd(tm->tm_mon) + 1) & 0x1f;
        // if the year is after 1999
        if (tm->tm_year > 99) {
            val |= (1 << 7); // set the century bit
        }
        m_i2c.write(reg, val);

        // year
        reg = 6;
        val = int_to_bcd(tm->tm_year % 100) & 0x0ff;
        m_i2c.write(reg, val);

        rc = 0;
    }

    return rc;
}


float DS3231::getTemp()
{
    float temp;
    unsigned char buf[4];

    temp = -999.9;
    memset(buf, 0, sizeof buf);
    if (m_i2c.isPresent()) {
        m_i2c.read(0x11, &buf[0], 2);
        temp = (buf[0]&0x7f) + 0.25 * int((buf[1]>>6)&0x03);
    }
    return temp;
}

bool DS3231::isPresent(void) const
{
    return m_i2c.isPresent();
}

unsigned char bcd_to_int(unsigned char bcd)
{
    unsigned char val;
    val = 10 * ((bcd>>4) & 0x0f) + (bcd & 0x0f);
    return val;
}

unsigned char int_to_bcd(unsigned char val)
{
    unsigned char bcd;
    bcd = (((val/10) & 0x0f) <<4) | ((val%10) & 0x0f);
    return bcd;
}
