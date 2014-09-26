#if !defined I2CDEVICE_H
#define I2CDEVICE_H

class I2CDevice
{
  public:
    I2CDevice(unsigned int deviceId = 0);
    virtual ~I2CDevice();


    unsigned int getDeviceId() const {return m_deviceId;}
    void setDeviceId(unsigned int deviceId) {m_deviceId = deviceId;}

    bool isPresent(void) const;

    // returns number of bytes actually read (<= requestSize).
    int read(unsigned int addr, unsigned char* buf, int requestSize) const;

    // returns an error code.
    int write(unsigned int addr, unsigned char data) const;

  private:
    unsigned char m_deviceId;
};

#endif // !defined I2CDEVICE_H

