/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_DIGITALIN_H
#define MBED_DIGITALIN_H

#include "em_gpio.h"
namespace mbed {
/** \addtogroup drivers */

/** A digital input, used for reading the state of a pin
 *
 * @note Synchronization level: Interrupt safe
 *
 * Example:
 * @code
 * // Flash an LED while a DigitalIn is true
 *
 * #include "mbed.h"
 *
 * DigitalIn enable(p5);
 * DigitalOut led(LED1);
 *
 * int main() {
 *     while(1) {
 *         if(enable) {
 *             led = !led;
 *         }
 *         wait(0.25);
 *     }
 * }
 * @endcode
 * @ingroup drivers
 */
class DigitalIn {

public:
    /** Create a DigitalIn connected to the specified pin
     *
     *  @param pin DigitalIn pin to connect to
     *  @param mode the initial mode of the pin
     */

    DigitalIn(GPIO_Port_TypeDef port, unsigned int pin, GPIO_Mode_TypeDef mode, int value)
    {
        // No lock needed in the constructor
    	_port = port;
    	_pin = pin;
    	_mode = mode;
    	_out = value;

    	GPIO_Mode_TypeDef defaultmode;
    	defaultmode = gpioModeInput;
        //Handle DOUT setting
        if((defaultmode & 0x10) != 0) {
            //Set DOUT
#ifdef _GPIO_P_DOUTSET_MASK
            GPIO->P[port].DOUTSET = pin;
#else
            GPIO->P[port].DOUT |= pin;
#endif
        } else {
            //Clear DOUT
#ifdef _GPIO_P_DOUTCLR_MASK
            GPIO->P[port].DOUTCLR = pin;
#else
            GPIO->P[port].DOUT &= ~pin;
#endif
        }

    	GPIO_PinModeSet(_port, _pin, defaultmode,0);

    	//mode = gpioModeInput;
        //Handle DOUT setting
        if((mode & 0x10) != 0) {
            //Set DOUT
#ifdef _GPIO_P_DOUTSET_MASK
            GPIO->P[port].DOUTSET = pin;
#else
            GPIO->P[port].DOUT |= pin;
#endif
        } else {
            //Clear DOUT
#ifdef _GPIO_P_DOUTCLR_MASK
            GPIO->P[port].DOUTCLR = pin;
#else
            GPIO->P[port].DOUT &= ~pin;
#endif
        }
       int port_pin = GPIO_PinOutGet(_port, _pin & 0xF);

    	GPIO_PinModeSet(_port, _pin, mode, 0);
    }

    /** Read the input, represented as 0 or 1 (int)
     *
     *  @returns
     *    An integer representing the state of the input pin,
     *    0 for logical 0, 1 for logical 1
     */
    unsigned int read()
    {
        // Thread safe / atomic HAL call
        return GPIO_PinInGet(_port, _pin);
    }

    /** An operator shorthand for read()
     * \sa DigitalIn::read()
     * @code
     *      DigitalIn  button(BUTTON1);
     *      DigitalOut led(LED1);
     *      led = button;   // Equivalent to led.write(button.read())
     * @endcode
     */
    operator int()
    {
        // Underlying read is thread safe
        return read();
    }

    GPIO_Port_TypeDef _port;
    unsigned int _pin;
protected:
    GPIO_Mode_TypeDef _mode;
    unsigned int _out;

};

} // namespace mbed

#endif
