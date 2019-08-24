/**
 * @file
 * @brief app_utils.h
 * Common declarations for both master and slave roles.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef APP_UTILS_H
#define APP_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif

#ifndef MAX_ADVERTISERS
#define MAX_ADVERTISERS 4
#endif

#include "bg_types.h"
#include "rtos_gecko.h"
#include "gatt_db.h"
#include "em_rtcc.h"
#include "graphics.h"
#include "gpiointerrupt.h"
#include <stdio.h>

/* Device initialization header */
#include "hal-config.h"

/**************************************************************************//**
 * Constants and state type
 *****************************************************************************/
// Software timer handles
#define SOFT_TIMER_DISPLAY_REFRESH_HANDLE       0
#define SOFT_TIMER_FIXED_TRANSFER_TIME_HANDLE 	1

#define DATA_SIZE                           255		// Size of the arrays for sending and receiving data
#define DATA_TRANSFER_SIZE_INDICATIONS      0       // If == 0 or > MTU-3 then it will send MTU-3 bytes of data, otherwise it will use this value
#define DATA_TRANSFER_SIZE_NOTIFICATIONS    0       // If == 0 or > MTU-3 then it will calculate the data amount to send for maximum over-the-air packet usage, otherwise it will use this value
#define INDICATION_GATT_HEADER              3       // GATT operation header byte count
#define NOTIFICATION_GATT_HEADER            3       // GATT operation header byte count
#define L2CAP_HEADER                        4       // Header byte count
#define HW_TICKS_PER_SECOND      (uint16_t)(32768)  // Hardware clock ticks that equal one second
#define TX_POWER 100

#define PHY_1M (0x01)
#define PHY_2M (0x02)
#define PHY_S8 (0x04)

#define SCAN_INTERVAL               16			   // 16 * 0.625 = 10ms
#define SCAN_WINDOW                 16			   // 16 * 0.625 = 10ms
#define ACTIVE_SCANNING             1			   // 1 = active scanning (sends scan requests), 0 = passive scanning (doesn't send scan requests)

#define NOTIFICATIONS_START (uint32)(1 << 0)   // Bit flag to external signal command
#define NOTIFICATIONS_END   (uint32)(1 << 1)   // Bit flag to external signal command
#define INDICATIONS_START   (uint32)(1 << 2)   // Bit flag to external signal command
#define INDICATIONS_END     (uint32)(1 << 3)   // Bit flag to external signal command
#define PHY_CHANGE          (uint32)(1 << 4)   // Bit flag to external signal command
#define SCAN_PHY_CHANGE     (uint32)(1 << 5)   // Bit flag for scan PHY change 1M<->LE Coded

/* COMPILE TIME OPTIONS FOR FIXED MODES BETWEEN TWO KITS. UNCOMMENT ONLY ONE. */
//#define SEND_FIXED_TRANSFER_COUNT				10000 						          // Uncomment this if you want to send a fixed amount of indications/notifications on each button press
#define SEND_FIXED_TRANSFER_TIME				((HW_TICKS_PER_SECOND)*5)     // Uncomment this if you want to send indications/notifications for a fixed amount of time

// Main state enum
typedef enum {
    ADV_SCAN,
    CONNECTED,
    SUBSCRIBED_NOTIFICATIONS,
    SUBSCRIBED_INDICATIONS,
    SUBSCRIBED,
    RECEIVE,
    NOTIFY,
    INDICATE
} State_t;

/**************************************************************************//**
 * Common variable declarations
 *****************************************************************************/
extern uint8_t boot_to_dfu;
extern State_t state;
extern uint8_t connection;                          // Connection handle initialized to known unreachable handle.
extern uint16_t mtuSize ;                           // Variable to hold the MTU size once a new connection is formed
extern uint16_t pduSize;                            // Variable to hold PDU size once a new connection is formed
extern uint16_t interval;                           // Variable to hold connection interval

extern uint8_t notificationsData[DATA_SIZE];
extern uint8_t indicationsData[DATA_SIZE];
extern uint16_t maxDataSizeIndications;
extern uint16_t maxDataSizeNotifications;           // Variable to calculate maximum data size for optimal throughput
extern uint32_t throughput;
extern uint32_t bitsSent;
extern uint32_t timeElapsed;
extern uint32_t operationCount;

extern uint8_t phyInUse;
extern uint8_t phyToUse;

extern bool roleIsSlave;
extern bool waitingForConfirmation;                      // Flag to check if waiting for any pending confirmations
extern volatile bool buttonOneReleased;                  // Flag to check if button has been released for indications.
extern bool notificationsSubscribed;
extern bool indicationsSubscribed;
extern bool advStopped;                                  // Check if advertising has been stopped once the connection is formed.
#ifdef SEND_FIXED_TRANSFER_TIME
extern bool fixedTimeExpired;
#endif

// Display strings
extern char throughputString[];           // Char array to print the throughput
extern char mtuSizeString[];              // Char array to print MTU size on the display
extern char connIntervalString[];         // Char array to print connection interval on the display
extern char pduSizeString[];              // Char array to print PDU size on the display
extern char phyString[];	              // Char array to print PHY in use on the display

extern char maxDataSizeString[];
extern char statusConnectedString[];
extern char operationCountString[];
extern char statusDisconnectedString[];

extern char *notifyString;
extern char *indicateString;
extern char *statusString;
extern char *roleString;
extern int16_t txPowerResp;
extern char txPowerString[12];

// Transmission start and stop signal constants.
const uint8_t TRANSMISSION_ON;
const uint8_t TRANSMISSION_OFF;

const char NOTIFY_ENABLED_STRING[13];
const char NOTIFY_DISABLED_STRING[12];
const char INDICATE_ENABLED_STRING[15];
const char INDICATE_DISABLED_STRING[14];
const char ROLE_SLAVE_STRING[13];
const char ROLE_MASTER_STRING[14];
const char ROLE_ADVERT_STRING[14];
const char ROLE_SCANNER_STRING[15];

/**************************************************************************//**
 * Common function declarations
 *****************************************************************************/
void reset_variables(void);
void setup_adv_scan(void);
void handle_button_change(uint8_t pin);
void refresh_display(void);
void set_display_defaults(void);
void update_displayed_phy(uint8_t currentPhy);

void calculate_notification_size(void);
void calculate_indication_size(void);
void generate_notifications_data(void);
void generate_indications_data(void);
void start_data_transmission(void);
void end_data_transmission(void);

void handle_universal_events(struct gecko_cmd_packet *evt);
void slave_main(void);
void master_main(void);

#ifdef __cplusplus
}
#endif

#endif
