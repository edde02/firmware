/*
 * Copyright 2013 OpenMote Technologies, S.L.
 */

/**
 *
 * @file       Adxl346.h
 * @author     Pere Tuset-Peiro (peretuset@openmote.com)
 * @version    v0.1
 * @date       May, 2014
 * @brief
 * @ingroup
 *
 */

#ifndef ADXL346_H_
#define ADXL346_H_

#include <stdint.h>

#include "Sensor.h"
#include "Callback.h"

class I2cDriver;
class GpioIn;

class Adxl346: public Sensor
{
public:
    Adxl346(I2cDriver& i2c, GpioIn& gpio);
    bool enable(void);
    bool reset(void);
    bool wakeup(void);
    bool suspend(void);
    bool isPresent(void);
    bool selfTest(bool test);
    void setCallback(Callback* callback);
    void clearCallback(void);
    bool readSample(uint16_t* x, uint16_t* y, uint16_t* z);
    float convertAcceleration(int16_t acceleration);
private:
    I2cDriver& i2c_;
    GpioIn& gpio_;
};

#endif /* ADXL346_H_ */
