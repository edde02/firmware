/**
 * @file       I2c.h
 * @author     Pere Tuset-Peiro (peretuset@openmote.com)
 * @version    v0.1
 * @date       May, 2015
 * @brief
 *
 * @copyright  Copyright 2015, OpenMote Technologies, S.L.
 *             This file is licensed under the GNU General Public License v2.
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>

#include "Callback.h"
#include "Semaphore.h"

class Gpio;
struct I2cConfig;

class I2c
{

friend class InterruptHandler;

public:
    I2c(Gpio& scl, Gpio& sda, I2cConfig& config);
    void enable(uint32_t baudrate = 100000);
    void sleep(void);
    void wakeup(void);
    void lock(void);
    void unlock(void);
    void unlockFromInterrupt(void);
    bool readByte(uint8_t address, uint8_t* buffer);
    bool readByte(uint8_t address, uint8_t* buffer, uint8_t size);
    bool writeByte(uint8_t address, uint8_t byte);
    bool writeByte(uint8_t address, uint8_t* buffer, uint8_t size);
protected:
    void interruptHandler(void);
private:
    Gpio& scl_;
    Gpio& sda_;

    I2cConfig& config_;

    SemaphoreBinary semaphore_;
};

#endif /* I2C_H_ */
