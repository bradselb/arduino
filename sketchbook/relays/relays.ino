#include <Wire.h>
#include <string.h>
#include <ctype.h>


extern int strtotm(char* s, struct tm* tm);

void blink(int pin, int period, int time_on, int n=1);


static const int led = 13;
static const int output[] = {13, 2, 3, 4, 5};
static const int output_count = sizeof output / sizeof output[0];


void setup()
{
    pinMode(led, OUTPUT);
    for (int i = 0; i < output_count; ++i) {
        pinMode(output[i], OUTPUT);
    }

    Wire.begin();
    Serial.begin(57600);
}


void loop()
{
    int bytesRead;
    char* arg;
    const char* delims = " ,;";
    int bufsize;
    char buf[256];
    char* p = &buf[0];

    bufsize = sizeof buf;
    memset(buf, 0, bufsize);

    bytesRead = Serial.readBytesUntil('\n', buf, bufsize-1);

    if (bytesRead > 0) {
        Serial.println(buf);

        while (0 != (arg = strtok(p, delims))) {
            p = 0;
            
            // what command?
            if (0 == strncmp(arg, "output", 3) || 0 == strncmp(arg, "relay", 3)) {
                int nr = 0;

                // which relay?
                arg = strtok(0, delims);
                if (!arg) continue;
                nr = atoi(arg);

                if (nr < 0 || nr >= output_count) {
                    Serial.println("illegal value.");
                }

                // what pattern?
                arg = strtok(0, delims);
                if (!arg) {
                    // show the state of this relay.
                    int state = digitalRead(output[nr]);
                    Serial.println(state);
                    continue;
                }

                if (0) {
                } else if(0 == strncmp(arg, "on", 2)) {
                    digitalWrite(output[nr],HIGH);
                } else if(0 == strncmp(arg, "off", 3)) {
                    digitalWrite(output[nr],LOW);
                } else if (0 == strncmp(arg, "blink", 5)) {
                    int count = 1;
                    arg = strtok(0, delims);
                    if (arg) {
                        count = atoi(arg);
                    }
                    blink(output[nr], 1000, 250, count);
                }

            } else if (0==strncmp(arg, "wait", 4)) {
                int milliseconds = 0;

                // how long
                arg = strtok(0, delims);
                if (!arg) return ;
                milliseconds = atoi(arg);
                delay(milliseconds);

            }
        } // while arg
    }
}

// blink the digital output pin
// period is in milliseconds
// duty cycle is percent
// n is the number of cycles. 
void blink(int pin, int period, int time_on, int n)
{
  int time_off = period - time_on;

  for (int i=0; i<n; ++i) {
    digitalWrite(pin, HIGH);
    delay(time_on);
    digitalWrite(pin, LOW);
    delay(time_off);
  }
}
