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
#include "SPI.h"
#include <stdio.h>
#include "em_usart.h"

namespace mbed {

SPI::SPI(unsigned int  mosi, unsigned int  miso, unsigned int sclk)
{
    _do_construct();
}


void SPI::_do_construct()
{
    SPIDRV_Init_t initDataMaster = SPIDRV_MASTER_USART2;

    // Initialize the USART0 a SPI Master
      SPIDRV_Init( handleMaster, &initDataMaster );
}

SPI::~SPI()
{
	SPIDRV_DeInit(handleMaster);
}


int SPI::write(int value)
{
	_tvalue = (uint8_t) value;

	// SPI_Write
    if (handleMaster->initData.frameLength <= 8) {
        USART_Tx(handleMaster->initData.port, (uint8_t) _tvalue);
    } else if (handleMaster->initData.frameLength == 9) {
        USART_TxExt(handleMaster->initData.port, (uint16_t) _tvalue & 0x1FF);
    } else {
        USART_TxDouble(handleMaster->initData.port, (uint16_t) _tvalue);
    }

    /* Wait for transmission of last byte */
    while (!(handleMaster->initData.port->STATUS & USART_STATUS_TXC)) {
    }

    // SPI_Read
    if (handleMaster->initData.frameLength <= 8) {
        return (int) handleMaster->initData.port->RXDATA;
    } else if (handleMaster->initData.frameLength == 9) {
        return (int) handleMaster->initData.port->RXDATAX & 0x1FF;
    } else {
        return (int) handleMaster->initData.port->RXDOUBLE;
    }

}


} // namespace mbed


