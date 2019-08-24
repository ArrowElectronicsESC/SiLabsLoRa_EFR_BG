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
#ifndef MBED_DIGITALOUT_H
#define MBED_DIGITALOUT_H

#include "em_gpio.h"

namespace mbed {
/** \addtogroup drivers */

/** A digital output, used for setting the state of a pin
 *
 * @note Synchronization level: Interrupt safe
 *
 * Example:
 * @code
 * // Toggle a LED
 * #include "mbed.h"
 *
 * DigitalOut led(LED1);
 *
 * int main() {
 *     while(1) {
 *         led = !led;
 *         wait(0.2);
 *     }
 * }
 * @endcode
 * @ingroup drivers
 */
class DigitalOut {

public:

    /** Create a DigitalOut connected to the specified pin
     *
     *  @param pin DigitalOut pin to connect to
     *  @param value the initial pin value
     */
    DigitalOut(GPIO_Port_TypeDef port, unsigned int pin, GPIO_Mode_TypeDef mode, int value)
    {
        // No lock needed in the constructor
    	_port = port;
    	_pin = pin;
    	_mode = mode;
    	_out = value;

    	_mode = gpioModePushPull;
        if (value) {
            GPIO_PinOutSet((GPIO_Port_TypeDef)(port & 0xF), pin & 0xF); // Pin number encoded in first four bits of obj->pin
        } else {
            GPIO_PinOutClear((GPIO_Port_TypeDef)(port & 0xF), pin & 0xF);
        }

    	GPIO_PinModeSet(_port, _pin, _mode, GPIO_PinOutGet(_port, _pin & 0xF));
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
     *    an integer representing the output setting of the pin,
     *    0 for logical 0, 1 for logical 1
     */
    unsigned int read()
    {
        // Thread safe / atomic HAL call
        return GPIO_PinInGet(_port, _pin);
    }

    /** A shorthand for write()
     * \sa DigitalOut::write()
     * @code
     *      DigitalIn  button(BUTTON1);
     *      DigitalOut led(LED1);
     *      led = button;   // Equivalent to led.write(button.read())
     * @endcode
     */
    DigitalOut &operator= (int value)
    {
        // Underlying write is thread safe
        write(value);
        return *this;
    }

    /** A shorthand for write() using the assignment operator which copies the
     * state from the DigitalOut argument.
     * \sa DigitalOut::write()
     */
    DigitalOut &operator= (DigitalOut &rhs)
    {
        write(rhs.read());
        return *this;
    }

    /** A shorthand for read()
     * \sa DigitalOut::read()
     * @code
     *      DigitalIn  button(BUTTON1);
     *      DigitalOut led(LED1);
     *      led = button;   // Equivalent to led.write(button.read())
     * @endcode
     */
    operator int()
    {
        // Underlying call is thread safe
        return read();
    }
    GPIO_Port_TypeDef _port;
    unsigned int _pin;

protected:
#if !defined(DOXYGEN_ONLY)
    GPIO_Mode_TypeDef _mode;
    unsigned int _out;
#endif //!defined(DOXYGEN_ONLY)
};

} // namespace mbed

#endif
