/**
 * @file       GpioIn.cpp
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
#include "InterruptHandler.h"

#include "cc2538_include.h"
#include "platform_types.h"

/*================================ define ===================================*/

/*================================ typedef ==================================*/

/*=============================== variables =================================*/

/*=============================== prototypes ================================*/

/*================================= public ==================================*/

GpioIn::GpioIn(GpioConfig& config):
    Gpio(config)
{
    // Set the pin as input
    GPIOPinTypeGPIOInput(config_.port, config_.pin);

    // Set the edge of the interrupt
    GPIOIntTypeSet(config_.port, config_.pin, config_.edge);
}

bool GpioIn::read(void)
{
    uint32_t state;
    state = GPIOPinRead(config_.port, config_.pin);
    return (bool)state;
}

void GpioIn::setCallback(Callback* callback)
{
    // Save the pointer to the callback function
    callback_ = callback;

    // Get a reference to the interruptHandler object
    InterruptHandler& interruptHandler = InterruptHandler::getInstance();

    // Register to the interruptHandler by passing a pointer to the object
    interruptHandler.setInterruptHandler(this);
}

void GpioIn::clearCallback(void)
{
    // Clear the pointer to the callback function
    callback_ = nullptr;
}

void GpioIn::enableInterrupts(void)
{
    // Clear the interrupt
    GPIOPinIntClear(config_.port, config_.pin);

    // Enable the interrupt
    GPIOPinIntEnable(config_.port, config_.pin);
}

void GpioIn::disableInterrupts(void)
{
    // Disable the interrupt
    GPIOPinIntDisable(config_.port, config_.pin);
}

void GpioIn::interruptHandler(void)
{
    // Call the interrupt handler if it is NOT null
    if (callback_ != nullptr) {
        callback_->execute();
    }
}

/*=============================== protected =================================*/

/*================================ private ==================================*/
