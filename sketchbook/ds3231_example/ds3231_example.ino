#include <Wire.h>
#include <string.h>
#include <ctype.h>

#include "tm.h"
#include "I2Ceeprom.h"
#include "DS3231.h"

extern int strtotm(char* s, struct tm* tm);

void blink(int pin, int period, int duty_cycle=50, int n=1);

I2Ceeprom eeprom(0x57); // an Atmel AT24C32 with A2A1A0 = 0x07 
DS3231 rtc(0x68); // DS3231

static const int led = 13;
static const int relay1 = 2;
static const int relay2 = 3;


void setup()
{
    pinMode(led, OUTPUT);
    pinMode(relay1, OUTPUT);
    pinMode(relay2, OUTPUT);

    Wire.begin();
    Serial.begin(57600);


#if 0
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

    //blink(led, 1000, 25, 5);
}


void loop()
{
    int c;
    int bytesRead;
    int bufsize;
    char buf[128];
    struct tm tm;

    bufsize = sizeof buf;
    memset(buf, 0, bufsize);

    bytesRead = 0;
    c = 0;
    while (bytesRead < bufsize && c != '\n') {
        if (Serial.available() > 0) {
            c = Serial.read();
            if (c != -1) {
                buf[bytesRead] = (c & 0x0ff);
                ++bytesRead;
                //Serial.print((const char)c);
            }
            if ('\n' == c || '\r' == c) {
                break;
            }
        }
    }

    if (bytesRead > 0) {
        memset(&tm, 0, sizeof tm);

        if (0 == strncmp(buf, "toggle relay ", 13)) {
            if (0 == strncmp(buf+13, "1", 1)) {
                blink(relay1, 200, 80, 1);
                Serial.println("relay 1 on-off");
            } else if (0 == strncmp(buf+13, "2", 1)) {
                blink(relay2, 200, 80, 1);
                Serial.println("relay 2 on-off");
            }
        } else if (0 == strncmp(buf, "set relay ", 10)) {
            if (0 == strncmp(buf+10, "1 on", 4)) {
                digitalWrite(relay1, HIGH);
                Serial.println("relay 1 on");
            } else if (0 == strncmp(buf+10, "1 off", 5)) {
                digitalWrite(relay1, LOW);
                Serial.println("relay 1 off");
            } else if (0 == strncmp(buf+10, "2 on", 4)) {
                digitalWrite(relay2, HIGH);
                Serial.println("relay 2 on");
            } else if (0 == strncmp(buf+10, "2 off", 5)) {
                digitalWrite(relay2, LOW);
                Serial.println("relay 2 off");
            }
        }

        if (rtc.isPresent()) {
            if (0 == strncmp(buf, "show t", 6)) {
                // show the time and temp. 
                rtc.getTime(&tm);
                memset(buf, 0, sizeof buf);
                sprintf(buf, "%4d-%02d-%02d %02d:%02d:%02d UTC  ",
                    tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
                Serial.println(buf);
                Serial.print(rtc.getTemp()); Serial.println(" degrees C");

            } else if (0 == strncmp(buf, "set time", 8)) {
                // set the time.
                int n;
                char* p = &buf[9];
                Serial.println(p);
                n = strtotm(p, &tm);
                Serial.println(1900+tm.tm_year); Serial.println(1+tm.tm_mon); Serial.println(tm.tm_mday);
                if (n >= 6) {
                    rtc.setTime(&tm);
                }
            }
        }
    }
}

// blink the LED connected to digital output pin
// period is in milliseconds
// duty cycle is percent
// n is the number of cycles. 
void blink(int pin, int period, int duty_cycle, int n)
{
  int time_on = period * float(duty_cycle) * 0.01;
  int time_off = period - time_on;

  for (int i=0; i<n; ++i) {
    digitalWrite(pin, HIGH);
    delay(time_on);
    digitalWrite(pin, LOW);
    delay(time_off);
  }
}
