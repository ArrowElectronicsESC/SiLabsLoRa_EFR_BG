/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#ifndef MBED_SPI_H
#define MBED_SPI_H


#include "em_gpio.h"
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_leuart.h"
#include "em_ldma.h"
#include "bspconfig.h"
#include "spidrv.h"


namespace mbed {
/** \addtogroup drivers
 * @endcode
 * @ingroup drivers
 */
class SPI {

public:

    /** Create a SPI master connected to the specified pins.
     *
     *  @note This constructor passes the SSEL pin selection to the target HAL.
     *  Not all targets support SSEL, so this cannot be relied on in portable code.
     *  Portable code should use the alternative constructor that uses GPIO
     *  for SSEL.
     *
     *  @note You can specify mosi or miso as NC if not used.
     *
     *  @param mosi SPI Master Out, Slave In pin.
     *  @param miso SPI Master In, Slave Out pin.
     *  @param sclk SPI Clock pin.
     *  @param ssel SPI Chip Select pin.
     */
    SPI(unsigned int mosi, unsigned int miso, unsigned int sclk);

    /** Create a SPI master connected to the specified pins.
     *
     *  @note This constructor manipulates the SSEL pin as a GPIO output
     *  using a DigitalOut object. This should work on any target, and permits
     *  the use of select() and deselect() methods to keep the pin asserted
     *  between transfers.
     *
     *  @note You can specify mosi or miso as NC if not used.
     *
     *  @param mosi SPI Master Out, Slave In pin.
     *  @param miso SPI Master In, Slave Out pin.
     *  @param sclk SPI Clock pin.
     *  @param ssel SPI Chip Select pin.
     */
    //SPI(PinName mosi, PinName miso, PinName sclk, PinName ssel, use_gpio_ssel_t);

    virtual ~SPI();

    /** Configure the data transmission format.
     *
     *  @param bits Number of bits per SPI frame (4 - 16).
     *  @param mode Clock polarity and phase mode (0 - 3).
     *
     * @code
     * mode | POL PHA
     * -----+--------
     *   0  |  0   0
     *   1  |  0   1
     *   2  |  1   0
     *   3  |  1   1
     * @endcode
     */
    //void format(int bits, int mode = 0);

    /** Set the SPI bus clock frequency.
     *
     *  @param hz Clock frequency in Hz (default = 1MHz).
     */
    //void frequency(int hz = 1000000);

    /** Write to the SPI Slave and return the response.
     *
     *  @param value Data to be sent to the SPI slave.
     *
     *  @return Response from the SPI slave.
     */
    virtual int write(int value);

    /** Write to the SPI Slave and obtain the response.
     *
     *  The total number of bytes sent and received will be the maximum of
     *  tx_length and rx_length. The bytes written will be padded with the
     *  value 0xff.
     *
     *  @param tx_buffer Pointer to the byte-array of data to write to the device.
     *  @param tx_length Number of bytes to write, may be zero.
     *  @param rx_buffer Pointer to the byte-array of data to read from the device.
     *  @param rx_length Number of bytes to read, may be zero.
     *  @return
     *      The number of bytes written and read from the device. This is
     *      maximum of tx_length and rx_length.
     */
    //virtual int write(const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length);

    /** Acquire exclusive access to this SPI bus.
     */
    //virtual void lock(void);

    /** Release exclusive access to this SPI bus.
     */
    //virtual void unlock(void);

    /** Assert the Slave Select line, acquiring exclusive access to this SPI bus.
     *
     * If use_gpio_ssel was not passed to the constructor, this only acquires
     * exclusive access; it cannot assert the Slave Select line.
     */
    //void select(void);

    /** Deassert the Slave Select line, releasing exclusive access to this SPI bus.
     */
    //void deselect(void);

    /** Set default write data.
      * SPI requires the master to send some data during a read operation.
      * Different devices may require different default byte values.
      * For example: A SD Card requires default bytes to be 0xFF.
      *
      * @param data Default character to be transmitted during a read operation.
      */
    //void set_default_write_value(char data);


    // Configuration.

private:
    void _do_construct();
    SPIDRV_HandleData_t handleDataMaster;
    SPIDRV_Handle_t handleMaster = &handleDataMaster;
    unsigned int  _mosi;
    unsigned int  _miso;
    unsigned int  _sclk;
    uint8_t _tvalue;
    uint8_t _rvalue;
};

} // namespace mbed



#endif // MBED_SPI_H
