#if !defined I2CEEPROM_H
#define I2CEEPROM_H

class I2Ceeprom
{
  public:
    I2Ceeprom(unsigned int deviceId = 0x50);
    virtual ~I2Ceeprom();


    unsigned int getDeviceId() const {return m_deviceId;}
    void setDeviceId(unsigned int deviceId) {m_deviceId = deviceId;}

    bool isPresent(void);

    // returns number of bytes actually read (<= requestSize).
    int read(unsigned int addr, char* buf, int requestSize);

    // returns an error code.
    int write(unsigned int addr, unsigned char data);
    //int write(unsigned int addr, const unsigned char* buf, unsigned int byteCount);

  private:
    unsigned char m_deviceId;
};

#endif // !defined I2CEEPROM_H

