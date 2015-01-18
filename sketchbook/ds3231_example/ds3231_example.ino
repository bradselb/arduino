#include <string.h>
#include <ctype.h>
#include <Wire.h> 

#include "tm.h"
#include "I2Ceeprom.h"
#include "DS3231.h"

extern int strtotm(char* s, struct tm* tm);

I2Ceeprom eeprom(0x57); // an Atmel AT24C32 with A2A1A0 = 0x07 
DS3231 rtc(0x68); // DS3231

static const int led = 13;


void setup()
{
    Serial.begin(57600);

#ifdef VERBOSE
    if (eeprom.isPresent()) {
        Serial.print("eeprom is present at device id: 0x");
        Serial.println(eeprom.getDeviceId(), HEX);
    } else {
        Serial.print("eeprom is not present at device id: 0x");
        Serial.println(eeprom.getDeviceId(), HEX);
    }

    if (rtc.isPresent()) {
        Serial.print("rtc is present.\n");
    } else {
        Serial.print("rtc is not present.\n");
    }
#endif

}


void loop()
{
    int byteCount;
    char* arg;
    const char* delims = " ,;";
    int bufsize;
    char buf[256];
    char* bufp;
    struct tm tm;

    bufsize = sizeof buf;
    memset(buf, 0, bufsize);

    byteCount = Serial.readBytesUntil('\n', buf, bufsize-1);


    if (byteCount > 0) {
        memset(&tm, 0, sizeof tm);

        bufp = &buf[0];
        while (0 != (arg = strtok(bufp, delims))) {
            bufp = 0;

            if ((0 == strncmp(arg, "get", 3)) || (0 == strncmp(arg, "show", 3))) {
                // show what?
                arg = strtok(0, delims);
                if (!arg) {
                    continue;
                }
                if (0 == strncmp(arg, "time", 3)) {
                    // show the time
                    if (rtc.isPresent()) {
                        rtc.getTime(&tm);
                    }
                    memset(buf, 0, sizeof buf);
                    sprintf(buf, "%4d-%02d-%02d %02d:%02d:%02d UTC  ",
                        tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
                    Serial.println(buf);

                } else if (0 == strncmp(arg, "temp", 3)) {
                    if (rtc.isPresent()) {
                        Serial.print(rtc.getTemp());
                    }
                    Serial.println(" degrees C");
                }

            // TODO: fix this. contemplate seting other things too --> use strtok().
            } else if (0 == strncmp(arg, "set time", 8)) {
                // set the time.
                int n;
                char* p = &buf[9];
                Serial.println(p);
                n = strtotm(p, &tm);
                Serial.println(1900+tm.tm_year); Serial.println(1+tm.tm_mon); Serial.println(tm.tm_mday);
                if (n >= 6) {
                    rtc.setTime(&tm);
                }
            } else {
                // not understood.
                Serial.print("OK\n");
            }
        }
    }
}


