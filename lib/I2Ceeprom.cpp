
#include <Arduino.h>
#include <Wire.h>

#include "I2Ceeprom.h"


// --------------------------------------------------------------------------
I2Ceeprom::I2Ceeprom(unsigned int deviceId) : m_deviceId(deviceId)
{
}


// --------------------------------------------------------------------------
I2Ceeprom::~I2Ceeprom()
{
}


// --------------------------------------------------------------------------
bool I2Ceeprom::isPresent(void)
{
  bool v;
  Wire.beginTransmission(m_deviceId);
  v = (Wire.endTransmission() == 0);
  return v;
}


// --------------------------------------------------------------------------
int I2Ceeprom::read(unsigned int addr, char* buf, int requestSize)
{
  int byteCount = 0; // how many we've read so far.

  while (byteCount < requestSize && requestSize > 0) {
    // send the deviceId and data address
    Wire.beginTransmission(m_deviceId);
    Wire.write(((addr + byteCount)>>8) & 0x0ff); // MSB
    Wire.write((addr + byteCount) & 0x0ff); // LSB
    Wire.endTransmission();

    Wire.requestFrom(m_deviceId, (uint8_t)requestSize); // can return an error?

    while (Wire.available() && byteCount < requestSize) {
      buf[byteCount] = Wire.read();
      ++byteCount;
    }
  }
  return byteCount;
}


// --------------------------------------------------------------------------
int I2Ceeprom::write(unsigned int addr, unsigned char data)
{
  int rc;
  uint8_t ec;

  Wire.beginTransmission(m_deviceId);
  Wire.write((addr >> 8) & 0x0ff); // MSB
  Wire.write(addr & 0x0ff); // LSB
  Wire.write(data);
  ec = Wire.endTransmission();

  rc = ec;

  return rc;
}

