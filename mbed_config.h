/*
 * mbed SDK
 * Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* MBED_CONF_LORA_DEVICE_SELECT - 0 for EU868 and 1 for US915 				*/
/* MBED_CONF_LORA_DEVICE_SELECT - 0 for SX1261 and 1 for SX1262 			*/
/* FSB_MASK for US915 FSB BAND 1 - {0x00FF, 0x0000, 0x0000, 0x0000, 0x0001} */

#ifndef __MBED_CONFIG_DATA__
#define __MBED_CONFIG_DATA__

// Configuration parameters
#define MBED_CONF_APP_LORA_ANT_SWITCH                                         12                                                                                                 // set by application[EFR32BG12]
#define MBED_CONF_APP_LORA_BUSY                                               8                                                                                                 // set by application[EFR32BG12]
#define MBED_CONF_APP_LORA_CRYSTAL_SELECT                                     11                                                                                                 // set by application[EFR32BG12]
#define MBED_CONF_APP_LORA_CS                                                 9                                                                                                 // set by application[EFR32BG12]
#define MBED_CONF_APP_LORA_DEVICE_SELECT                                      7                                                                                                 // set by application[EFR32BG12]
#define MBED_CONF_APP_LORA_DIO0                                               D3                                                                                                 // set by application[EFR32BG12]
#define MBED_CONF_APP_LORA_DIO1                                               9                                                                                                 // set by application[EFR32BG12]
#define MBED_CONF_APP_LORA_DIO2                                               NC                                                                                                 // set by application[EFR32BG12]
#define MBED_CONF_APP_LORA_DIO3                                               NC                                                                                                 // set by application[EFR32BG12]
#define MBED_CONF_APP_LORA_DIO4                                               NC                                                                                                 // set by application[EFR32BG12]
#define MBED_CONF_APP_LORA_DIO5                                               NC                                                                                                 // set by application[EFR32BG12]
#define MBED_CONF_APP_LORA_FREQ_SELECT                                        6                                                                                                 // set by application[EFR32BG12]
#define MBED_CONF_APP_LORA_PWR_AMP_CTL                                        NC                                                                                                 // set by application[EFR32BG12]
#define MBED_CONF_APP_LORA_RADIO                                              SX126X                                                                                             // set by application[EFR32BG12]
#define MBED_CONF_APP_LORA_RESET                                              9                                                                                                 // set by application[EFR32BG12]
#define MBED_CONF_APP_LORA_RF_SWITCH_CTL1                                     NC                                                                                                 // set by application[EFR32BG12]
#define MBED_CONF_APP_LORA_RF_SWITCH_CTL2                                     NC                                                                                                 // set by application[EFR32BG12]
#define MBED_CONF_APP_LORA_RXCTL                                              NC                                                                                                 // set by application[EFR32BG12]
#define MBED_CONF_APP_LORA_SPI_MISO                                           7                                                                                                // set by application[EFR32BG12]
#define MBED_CONF_APP_LORA_SPI_MOSI                                           6                                                                                                // set by application[EFR32BG12]
#define MBED_CONF_APP_LORA_SPI_SCLK                                           8                                                                                                // set by application[EFR32BG12]
#define MBED_CONF_APP_LORA_TCXO                                               NC                                                                                                 // set by application[EFR32BG12]
#define MBED_CONF_APP_LORA_TXCTL                                              NC                                                                                                 // set by application[EFR32BG12]
#define MBED_CONF_LORA_ADR_ON                                                 1                                                                                                  // set by library:lora
#define MBED_CONF_LORA_APPLICATION_EUI                                        { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x10 }                                                 // set by application[*]
#define MBED_CONF_LORA_APPLICATION_KEY                                        { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x10, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x10 } // set by application[*]
#define MBED_CONF_LORA_APPSKEY                                                { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x10, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x10 }   // set by library:lora
#define MBED_CONF_LORA_APP_PORT                                               15                                                                                                 // set by library:lora
#define MBED_CONF_LORA_AUTOMATIC_UPLINK_MESSAGE                               1                                                                                                  // set by library:lora
#define MBED_CONF_LORA_DEVICE_ADDRESS                                         0x00000010                                                                                         // set by library:lora
#define MBED_CONF_LORA_DEVICE_EUI                                             { 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x68 }                                                 // set by application[*]
#define MBED_CONF_LORA_DEVICE_SELECT                                          1
#define MBED_CONF_LORA_DOWNLINK_PREAMBLE_LENGTH                               5                                                                                                  // set by library:lora
#define MBED_CONF_LORA_DUTY_CYCLE_ON                                          1                                                                                                  // set by application[*]
#define MBED_CONF_LORA_DUTY_CYCLE_ON_JOIN                                     1                                                                                                  // set by library:lora
#define MBED_CONF_LORA_FREQ_SELECT                                            0
#define MBED_CONF_LORA_FSB_MASK                                               {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00FF}                                                           // set by library:lora
#define MBED_CONF_LORA_FSB_MASK_CHINA                                         {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}                                                   // set by library:lora
#define MBED_CONF_LORA_LBT_ON                                                 0                                                                                                  // set by library:lora
#define MBED_CONF_LORA_MAX_SYS_RX_ERROR                                       100                                                                                                  // set by library:lora
#define MBED_CONF_LORA_NB_TRIALS                                              12                                                                                                 // set by library:lora
#define MBED_CONF_LORA_NWKSKEY                                                { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x10, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x10 }   // set by library:lora
#define MBED_CONF_LORA_OVER_THE_AIR_ACTIVATION                                1                                                                                              // set by application[*]
#define MBED_CONF_LORA_PHY                                                    EU868                                                                                              // set by application[*]
#define MBED_CONF_LORA_PUBLIC_NETWORK                                         0                                                                                                  // set by application[*]
#define MBED_CONF_LORA_TX_MAX_SIZE                                            255                                                                                                 // set by library:lora
#define MBED_CONF_LORA_UPLINK_PREAMBLE_LENGTH                                 8                                                                                                  // set by library:lora
#define MBED_CONF_LORA_WAKEUP_TIME                                            5                                                                                                  // set by library:lora
#define MBED_CONF_SX126X_LORA_DRIVER_BOOST_RX                                 0                                                                                                  // set by library:SX126X-lora-driver
#define MBED_CONF_SX126X_LORA_DRIVER_BUFFER_SIZE                              255                                                                                                // set by library:SX126X-lora-driver
#define MBED_CONF_SX126X_LORA_DRIVER_REGULATOR_MODE                           1                                                                                                  // set by library:SX126X-lora-driver
#define MBED_CONF_SX126X_LORA_DRIVER_SLEEP_MODE                               1                                                                                                  // set by library:SX126X-lora-driver
#define MBED_CONF_SX126X_LORA_DRIVER_SPI_FREQUENCY                            16000000                                                                                           // set by library:SX126X-lora-driver
#define MBED_CONF_SX126X_LORA_DRIVER_STANDBY_MODE                             0                                                                                                  // set by library:SX126X-lora-driver
// Macros
#define MBEDTLS_CIPHER_MODE_CTR                                                                                                                                                  // defined by library:SecureStore
#define MBEDTLS_CMAC_C                                                                                                                                                           // defined by library:SecureStore
#define MBEDTLS_USER_CONFIG_FILE                                              "mbedtls_lora_config.h"                                                                            // defined by application

#endif
