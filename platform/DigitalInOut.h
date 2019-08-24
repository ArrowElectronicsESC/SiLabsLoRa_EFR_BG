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
#ifndef MBED_DIGITALINOUT_H
#define MBED_DIGITALINOUT_H

#include "em_gpio.h"

namespace mbed {
/** \addtogroup drivers */

/** A digital input/output, used for setting or reading a bi-directional pin
 *
 * @note Synchronization level: Interrupt safe
 * @ingroup drivers
 */
class DigitalInOut {

public:
    /** Create a DigitalInOut connected to the specified pin
     *
     *  @param pin DigitalInOut pin to connect to
     *  @param direction the initial direction of the pin
     *  @param mode the initial mode of the pin
     *  @param value the initial value of the pin if is an output
     */
    DigitalInOut(GPIO_Port_TypeDef port, unsigned int pin, GPIO_Mode_TypeDef mode, int value)
    {
        // No lock needed in the constructor
    	_port = port;
    	_pin = pin;
    	_mode = mode;
    	_out = value;

    	mode = gpioModeInputPull;
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

    	GPIO_PinModeSet(_port, _pin, _mode, 1);
    }


    /** Set the output, specified as 0 or 1 (int)
     *
     *  @param value An integer specifying the pin output value,
     *      0 for logical 0, 1 (or any other non-zero value) for logical 1
     */
    void write(int value)
    {
        // Thread safe / atomic HAL call
    	if (value) {
    		GPIO_PinOutSet(_port, _pin);
    	}
    	else{
    		GPIO_PinOutClear(_port, _pin);
    	}

    }

    /** Return the output setting, represented as 0 or 1 (int)
     *
     *  @returns
     *    an integer representing the output setting of the pin if it is an output,
     *    or read the input if set as an input
     */
    int read()
    {
        // Thread safe / atomic HAL call
    	return GPIO_PinInGet(_port, _pin);
    }

    /** Set as an output
     */
    void output()
    {
    	_mode = gpioModePushPull;
    	GPIO_PinModeSet(_port, _pin, _mode, GPIO_PinOutGet(_port, _pin & 0xF));
    }

    /** Set as an input
     */
    void input()
    {
    	_mode = gpioModeInputPull;
    	_out = 0;

    	//_mode = gpioModeInput;
        //Handle DOUT setting
        if((_mode & 0x10) != 0) {
            //Set DOUT
#ifdef _GPIO_P_DOUTSET_MASK
            GPIO->P[_port].DOUTSET = _pin;
#else
            GPIO->P[_port].DOUT |= _pin;
#endif
        } else {
            //Clear DOUT
#ifdef _GPIO_P_DOUTCLR_MASK
            GPIO->P[_port].DOUTCLR = _pin;
#else
            GPIO->P[_port].DOUT &= ~_pin;
#endif
        }

    	GPIO_PinModeSet(_port, _pin, _mode, 1);
    }


    /** A shorthand for write()
     * \sa DigitalInOut::write()
     * @code
     *      DigitalInOut  inout(PIN);
     *      DigitalIn     button(BUTTON1);
     *      inout.output();
     *
     *      inout = button;     // Equivalent to inout.write(button.read())
     * @endcode
     */
    DigitalInOut &operator= (int value)
    {
        // Underlying write is thread safe
        write(value);
        return *this;
    }

    /**A shorthand for write() using the assignment operator which copies the
     * state from the DigitalInOut argument.
     * \sa DigitalInOut::write()
     */
    DigitalInOut &operator= (DigitalInOut &rhs)
    {
        write(rhs.read());
        return *this;
    }

    /** A shorthand for read()
     * \sa DigitalInOut::read()
     * @code
     *      DigitalInOut inout(PIN);
     *      DigitalOut led(LED1);
     *
     *      inout.input();
     *      led = inout;   // Equivalent to led.write(inout.read())
     * @endcode
     */
    operator int()
    {
        // Underlying call is thread safe
        return read();
    }
protected:
#if !defined(DOXYGEN_ONLY)
    GPIO_Port_TypeDef _port;
    unsigned int _pin;
    GPIO_Mode_TypeDef _mode;
    unsigned int _out;
#endif //!defined(DOXYGEN_ONLY)
};

} // namespace mbed

#endif
