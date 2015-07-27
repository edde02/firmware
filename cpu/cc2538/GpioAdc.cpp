/**
 * @file       GpioAdc.cpp
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

/*================================ define ===================================*/

/*================================ typedef ==================================*/

/*=============================== variables =================================*/

/*=============================== prototypes ================================*/

/*================================= public ==================================*/

GpioAdc::GpioAdc(const Gpio_TypeDef& gpio):
    Gpio(gpio)
{
}

void GpioAdc::init(uint32_t resolution, uint32_t reference)
{
    // Configure ADC with resolution and reference
    SOCADCSingleConfigure(resolution, reference);
}

uint32_t GpioAdc::read(void)
{
    uint32_t value;

    // Trigger single conversion on internal temp sensor
    SOCADCSingleStart(gpio_.adc);

    // Wait until conversion is completed
    while(!SOCADCEndOfCOnversionGet())
    {
    }

    // Get the ADC value and shift it according to resolution
    value = SOCADCDataGet() >> SOCADC_12_BIT_RSHIFT;

    return value;
}

/*=============================== protected =================================*/

/*================================ private ==================================*/