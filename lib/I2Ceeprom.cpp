
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
  int byteCount; // how many we've read so far.
  byteCount = 0;

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

#if 0
#define min(a,b) ((a)<(b)?(a):(b))
// --------------------------------------------------------------------------
uint8_t I2Ceeprom::write(unsigned int addr, const unsigned char* buf, unsigned int length)
// WriteMem(int iAddr, const char *pBuf, int iCnt)
{
  uint8_t iBytes;
  uint8_t iRC=0;

// Writes are restricted to a single 32 byte page.  Therefore. if a write spans a page
// boundry we must split the write.

  while (iCnt > 0) {
    iBytes = min(iCnt, BUFFER_LENGTH-2);
    int iCurPage = addr & ~((int)0x1f);
    if (addr+iBytes > iCurPage+32) { // Number of bytes is too large
      iBytes = (iCurPage+32) - addr;
    }

    Wire.beginTransmission(m_deviceId);
    Wire.send(highByte(addr));   // Address MSB
    Wire.send(lowByte(addr)); // Address LSB
    Wire.send(pBuf, iBytes);
    Wire.endTransmission();
    iRC  += (int)iBytes;
    iCnt -= (int)iBytes;
    addr += (int)iBytes;
    pBuf += (int)iBytes;
    delay(50);  // Give the EEPROM time to write its data
  }

  return iRC;
}
#endif

