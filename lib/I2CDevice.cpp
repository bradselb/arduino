#include "I2CDevice.h"

#include <Arduino.h>
#include <Wire.h>


// this should work for any I2C device that uses a single byte address

// --------------------------------------------------------------------------
I2CDevice::I2CDevice(unsigned int deviceId) : m_deviceId(deviceId)
{
}


// --------------------------------------------------------------------------
I2CDevice::~I2CDevice()
{
}

// --------------------------------------------------------------------------
bool I2CDevice::isPresent(void) const
{
  bool v;
  Wire.beginTransmission(m_deviceId);
  v = (Wire.endTransmission() == 0);
  return v;
}


// --------------------------------------------------------------------------
int I2CDevice::read(unsigned int addr, unsigned char* buf, int requestSize) const
{
  int byteCount; // how many we've read so far.
  byteCount = 0;

  while (byteCount < requestSize && requestSize > 0) {
    // send the deviceId and data address
    Wire.beginTransmission(m_deviceId);
    Wire.write((addr + byteCount) & 0x0ff);
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
int I2CDevice::write(unsigned int addr, unsigned char data) const
{
  int rc;
  uint8_t ec;

  Wire.beginTransmission(m_deviceId);
  Wire.write(addr & 0x0ff);
  Wire.write(data);
  ec = Wire.endTransmission();
  delay(10);

  rc = ec;

  return rc;
}


