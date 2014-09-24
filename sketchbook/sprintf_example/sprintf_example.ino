#include <string.h>
#include <Wire.h>

unsigned int bufsize = 32;
char* buf;


void setup()
{
  buf = (char*)malloc(bufsize);
  Wire.begin();
  Serial.begin(57600);
}



void loop()
{
  Wire.requestFrom(0xA7, 1);
  while (Wire.available()) {
    uint8_t data = Wire.read();
    n = sprintf(buf, "0x%02x\n", data);
    Serial.write((uint8_t*)buf, n);
  }

  delay(1000);
}
