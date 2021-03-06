/**
 * @file       Spi.cpp
 * @author     Pere Tuset-Peiro (peretuset@openmote.com)
 * @version    v0.1
 * @date       May, 2015
 * @brief
 *
 * @copyright  Copyright 2015, OpenMote Technologies, S.L.
 *             This file is licensed under the GNU General Public License v2.
 */

/*================================ include ==================================*/

#include "Gpio.h"
#include "Spi.h"
#include "InterruptHandler.h"

#include "cc2538_include.h"
#include "platform_types.h"

/*================================ define ===================================*/

/*================================ typedef ==================================*/

/*=============================== variables =================================*/

/*=============================== prototypes ================================*/

/*================================= public ==================================*/

Spi::Spi(Gpio& miso, Gpio& mosi, Gpio& clk, GpioOut& ncs, SpiConfig& config):
        miso_(miso), mosi_(mosi), clk_(clk), ncs_(ncs), config_(config)
{
}

void Spi::enable(uint32_t baudrate)
{
    GpioConfig& miso = miso_.getGpioConfig();
    GpioConfig& mosi = mosi_.getGpioConfig();
    GpioConfig& clk  = clk_.getGpioConfig();
    // GpioConfig& ncs  = ncs_.getGpioConfig();

    // Store baudrate in configuration
    if (baudrate != 0) {
        config_.baudrate = baudrate;
    }
    
    // Enable peripheral except in deep sleep modes (e.g. LPM1, LPM2, LPM3)
    SysCtrlPeripheralEnable(config_.peripheral);
    SysCtrlPeripheralSleepEnable(config_.peripheral);
    SysCtrlPeripheralDeepSleepDisable(config_.peripheral);

    // Reset peripheral previous to configuring it
    SSIDisable(config_.base);

    // Set IO clock as SPI0 clock source
    SSIClockSourceSet(config_.base, config_.clock);

    // Configure the MISO, MOSI, CLK and nCS pins as peripheral
    IOCPinConfigPeriphInput(miso.port, miso.pin, miso.ioc);
    IOCPinConfigPeriphOutput(mosi.port, mosi.pin, mosi.ioc);
    IOCPinConfigPeriphOutput(clk.port, clk.pin, clk.ioc);
    // IOCPinConfigPeriphOutput(ncs.port, ncs.pin, ncs.ioc);

    // Configure MISO, MOSI, CLK and nCS GPIOs
    GPIOPinTypeSSI(miso.port, miso.pin);
    GPIOPinTypeSSI(mosi.port, mosi.pin);
    GPIOPinTypeSSI(clk.port, clk.pin);
    // GPIOPinTypeSSI(ncs.port, ncs.pin);

    // Configure the SPI0 clock
    SSIConfigSetExpClk(config_.base, SysCtrlIOClockGet(), config_.protocol, \
                       config_.mode, config_.baudrate, config_.datawidth);

    // Enable the SPI0 module
    SSIEnable(config_.base);
}

void Spi::sleep(void)
{
    GpioConfig& miso = miso_.getGpioConfig();
    GpioConfig& mosi = mosi_.getGpioConfig();
    GpioConfig& clk  = clk_.getGpioConfig();
    // GpioConfig& ncs  = ncs_.getGpioConfig();

    SSIDisable(config_.base);

    // Configure the MISO, MOSI, CLK and nCS pins as output
    GPIOPinTypeGPIOOutput(miso.port, miso.pin);
    GPIOPinTypeGPIOOutput(mosi.port, mosi.pin);
    GPIOPinTypeGPIOOutput(clk.port, clk.pin);
    // GPIOPinTypeGPIOOutput(ncs.port, ncs.pin);

    //
    GPIOPinWrite(miso.port, miso.pin, 0);
    GPIOPinWrite(mosi.port, mosi.pin, 0);
    GPIOPinWrite(clk.port, clk.pin, 0);
    // GPIOPinWrite(ncs.port, ncs.pin, 0);
}

void Spi::wakeup(void)
{
    enable();
}

void Spi::setRxCallback(Callback* callback)
{
    rx_callback_ = callback;
}

void Spi::setTxCallback(Callback* callback)
{
    tx_callback_ = callback;
}

void Spi::enableInterrupts(void)
{
    // Register the interrupt handler
    InterruptHandler::getInstance().setInterruptHandler(this);

    // Enable the SPI interrupt
    SSIIntEnable(config_.base, (SSI_TXFF | SSI_RXFF | SSI_RXTO | SSI_RXOR));

    // Enable the SPI interrupt
    IntEnable(config_.interrupt);
}

void Spi::disableInterrupts(void)
{
    // Disable the SPI interrupt
    SSIIntDisable(config_.base, (SSI_TXFF | SSI_RXFF | SSI_RXTO | SSI_RXOR));

    // Disable the SPI interrupt
    IntDisable(config_.interrupt);
}

void Spi::select(void)
{
    if (config_.protocol == SSI_FRF_MOTO_MODE_0 ||
        config_.protocol == SSI_FRF_MOTO_MODE_1)
    {
        ncs_.off();
    }
    else
    {
        ncs_.on();
    }
}

void Spi::deselect(void)
{
    if (config_.protocol == SSI_FRF_MOTO_MODE_0 ||
        config_.protocol == SSI_FRF_MOTO_MODE_1)
    {
        ncs_.on();
    }
    else
    {
        ncs_.off();
    }
}

uint8_t Spi::readByte(void)
{
    uint32_t byte;

    // Push a byte
    SSIDataPut(config_.base, 0x00);

    // Wait until it is complete
    while(SSIBusy(config_.base))
        ;

    // Read a byte
    SSIDataGet(config_.base, &byte);

    return (uint8_t)(byte & 0xFF);
}

uint32_t Spi::readByte(uint8_t* buffer, uint32_t length)
{
    uint32_t data;

    for (uint32_t i =  0; i < length; i++)
    {
        // Push a byte
        SSIDataPut(config_.base, 0x00);

        // Wait until it is complete
        while(SSIBusy(config_.base))
            ;

        // Read a byte
        SSIDataGet(config_.base, &data);

        *buffer++ = (uint8_t) data;
    }
    return 0;
}

void Spi::writeByte(uint8_t byte)
{
    uint32_t data;

    // Push a byte
    SSIDataPut(config_.base, byte);

    // Wait until it is complete
    while(SSIBusy(config_.base))
        ;

    // Read a byte
    SSIDataGet(config_.base, &data);
}

uint32_t Spi::writeByte(uint8_t* buffer, uint32_t length)
{
    uint32_t data;

    for (uint32_t i = 0; i < length; i++)
    {
        // Push a byte
        SSIDataPut(config_.base, *buffer++);

        // Wait until it is complete
        while(SSIBusy(config_.base))
            ;

        // Read a byte
        SSIDataGet(config_.base, &data);
    }

    return 0;
}

/*=============================== protected =================================*/

SpiConfig& Spi::getConfig(void)
{
    return config_;
}

void Spi::interruptHandler(void)
{
    uint32_t status;

    // Read interrupt source
    status = SSIIntStatus(config_.base, true);

    // Clear SPI interrupt in the NVIC
    IntPendClear(config_.interrupt);

    // Process TX interrupt
    if (status & SSI_TXFF) {
        interruptHandlerTx();
    }

    // Process RX interrupt
    if (status & SSI_RXFF) {
        interruptHandlerRx();
    }
}

/*================================ private ==================================*/

void Spi::interruptHandlerRx(void)
{
    if (tx_callback_ != nullptr)
    {
        tx_callback_->execute();
    }
}

void Spi::interruptHandlerTx(void)
{
    if (rx_callback_ != nullptr)
    {
        rx_callback_->execute();
    }
}
