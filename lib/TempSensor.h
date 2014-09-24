#if !defined TEMPSENSOR_H
#define TEMPSENSOR_H

// a pure abstract base class to define the interface to an
// integrated circuit temperture sensor

class TempSensor
{
  public:
    virtual ~TempSensor() {}

    virtual float getTemp() = 0;

};

#endif // !defined TEMPSENSOR_H
