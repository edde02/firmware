/**
 * @file       Uart.h
 * @author     Pere Tuset-Peiro (peretuset@openmote.com)
 * @version    v0.1
 * @date       May, 2015
 * @brief
 *
 * @copyright  Copyright 2015, OpenMote Technologies, S.L.
 *             This file is licensed under the GNU General Public License v2.
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

#include "Callback.h"
#include "Semaphore.h"

class Gpio;
struct UartConfig;

class Uart
{

friend class InterruptHandler;

public:
    Uart(Gpio& rx, Gpio& tx, UartConfig& config);
    void enable(uint32_t baudrate = 0);
    void sleep(void);
    void wakeup(void);
    void setRxCallback(Callback* callback);
    void setTxCallback(Callback* callback);
    void enableInterrupts(void);
    void disableInterrupts(void);
    void rxLock(void);
    void txLock(void);
    void rxUnlock(void);
    void txUnlock(void);
    void rxUnlockFromInterrupt(void);
    void txUnlockFromInterrupt(void);
    uint8_t readByte(void);
    uint32_t readByte(uint8_t* buffer, uint32_t length);
    void writeByte(uint8_t byte);
    uint32_t writeByte(uint8_t* buffer, uint32_t length);
protected:
    UartConfig& getConfig(void);
    void interruptHandler(void);
private:
    void interruptHandlerRx(void);
    void interruptHandlerTx(void);
private:
    Gpio& rx_;
    Gpio& tx_;
    UartConfig& config_;

    SemaphoreBinary rxSemaphore_;
    SemaphoreBinary txSemaphore_;

    Callback* rx_callback_;
    Callback* tx_callback_;
};

#endif /* UART_H_ */
