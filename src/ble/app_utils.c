/***************************************************************************//**
 * @file app_utils.c
 * @brief Definitions of common variables and functions
 *******************************************************************************/

#include <src/ble/app_utils.h>

/**************************************************************************//**
 * Common variable definitions
 *****************************************************************************/
const uint8_t TRANSMISSION_ON = 1;
const uint8_t TRANSMISSION_OFF = 0;

const char NOTIFY_ENABLED_STRING[] = {"NOTIFY: Yes\n"};
const char NOTIFY_DISABLED_STRING[] = {"NOTIFY: No\n"};
const char INDICATE_ENABLED_STRING[] = {"INDICATE: Yes\n"};
const char INDICATE_DISABLED_STRING[] = {"INDICATE: No\n"};
const char ROLE_SLAVE_STRING[] = {"ROLE: Slave\n"};
const char ROLE_MASTER_STRING[] = {"ROLE: Master\n"};
const char ROLE_ADVERT_STRING[] = {"ROLE: Advert\n"};
const char ROLE_SCANNER_STRING[] = {"ROLE: Scanner\n"};

uint8_t boot_to_dfu = 0;                         // Flag for indicating DFU Reset must be performed
State_t state = ADV_SCAN;
uint8_t connection = 0xFF;                       // Connection handle initialized to known unreachable handle.
uint16_t mtuSize = 0;                            // Variable to hold the MTU size once a new connection is formed
uint16_t pduSize = 0;                            // Variable to hold PDU size once a new connection is formed
uint16_t interval = 0;                           // Variable to hold connection interval

uint8_t notificationsData[DATA_SIZE] = {0};
uint8_t indicationsData[DATA_SIZE] = {0};
uint16_t maxDataSizeIndications = DATA_SIZE;
uint16_t maxDataSizeNotifications = DATA_SIZE;   // Variable to calculate maximum data size for optimal throughput
uint32_t throughput = 0;
uint32_t bitsSent = 0;
uint32_t timeElapsed = 0;
uint32_t operationCount = 0;

uint8_t phyInUse = PHY_1M;
uint8_t phyToUse = 0;

bool roleIsSlave = true;
bool waitingForConfirmation = 0;                         // Flag to check if waiting for any pending confirmations
volatile bool buttonOneReleased = true;                  // Flag to check if button has been released for indications.
bool notificationsSubscribed = false;
bool indicationsSubscribed = false;
bool advStopped = false;                                 // Check if advertising has been stopped once the connection is formed.
#ifdef SEND_FIXED_TRANSFER_TIME
bool fixedTimeExpired = false;
#endif

/* Display strings */
char throughputString[] = "TH:           \n";       // Char array to print the throughput
char mtuSizeString[] = "MTU:    ";                  // Char array to print MTU size on the display
char connIntervalString[] = "INTRV:      ";         // Char array to print connection interval on the display
char pduSizeString[] = "PDU:    ";                  // Char array to print PDU size on the display
char phyString[] = "PHY:          ";	              // Char array to print PHY in use on the display

char maxDataSizeString[] = "DATA SIZE:    ";
char statusConnectedString[] = {"RSSI:     \n"};
char operationCountString[] = "CNT:          \n";
char statusDisconnectedString[] = {"STATUS: Discon\n"};

char *notifyString = (char *)NOTIFY_DISABLED_STRING;
char *indicateString = (char *)INDICATE_DISABLED_STRING;
char *statusString = (char *)statusDisconnectedString;
char *roleString;

int16_t txPowerResp = TX_POWER;
char txPowerString[] = "TX:        "; // 8 spaces, display TX power in dBm

#if defined(SEND_FIXED_TRANSFER_COUNT) && defined(SEND_FIXED_TRANSFER_TIME)
#error "These are mutually exclusive options, you either do a fixed amount of transfers of transfer over a fixed amount of time."
#endif

/**************************************************************************//**
 * Common function definitions
 *****************************************************************************/

/**
 * @reset_variables
 * Reset necessary flags and state to known states
 */
void reset_variables(void) {
  connection = 0xFF;
  throughput = 0;
  bitsSent = 0;
  mtuSize = 0;
  pduSize = 0;
  interval = 0;
  operationCount = 0;
  maxDataSizeNotifications = 0;
  maxDataSizeIndications = 0;
  state = ADV_SCAN;
  memset(notificationsData, 0, DATA_SIZE);
  memset(indicationsData, 0, DATA_SIZE);
  notificationsSubscribed = false;
  indicationsSubscribed = false;
  advStopped = false;
}

/**
 * @brief set_display_defaults
 * Set display strings according to device role defaults
 */
void set_display_defaults(void) {
  // Clear display strings on disconnect
  if (roleIsSlave) {
      roleString = (char *)ROLE_ADVERT_STRING;
  } else {
      roleString = (char *)ROLE_SCANNER_STRING;
  }

  notifyString = (char *)NOTIFY_DISABLED_STRING;
  indicateString = (char *)INDICATE_DISABLED_STRING;
  statusString = (char *)statusDisconnectedString;

  sprintf(txPowerString + 4, ((txPowerResp / 10) == 0) ? "%01d dBm" : "%+0d dBm", txPowerResp / 10); // 0 dBm without sign
  sprintf(mtuSizeString + 5, "%s", "   ");			        // 3 spaces
  sprintf(throughputString + 4, "%s", "       ");		    // 7 spaces
  sprintf(pduSizeString + 5, "%s", "   ");			        // 3 spaces
  sprintf(connIntervalString + 7, "%s", "    ");		    // 4 spaces
  sprintf(maxDataSizeString + 11, "%s", "   ");		      // 3 spaces
  sprintf(phyString + 5, "%s", "        ");					    // 8 spaces
  sprintf(operationCountString + 5, "%s", "         "); // 9 spaces
}

/**
 * @brief update_displayed_phy
 * Changes displayed string to reflect used PHY
 * @param currentPhy - Currently used PHY (le_gap_phy_type)
 */
void update_displayed_phy(uint8_t currentPhy) {
  phyToUse = 0;
  phyInUse = currentPhy;

  switch(phyInUse) {
    case PHY_1M:
      sprintf(phyString + 5, "%s", "1M");
      break;

    case PHY_2M:
      sprintf(phyString + 5, "%s", "2M");
      break;

    case PHY_S8:
      sprintf(phyString + 5, "%s", "CODED S8");
      break;

    default:
      break;
  } 
}

/**
 * @brief setup_adv_scan
 * Start advertising or scanning depending on the role.
 */
void setup_adv_scan(void) {
  if (roleIsSlave) {
	  //1M PHY Advertising
	  gecko_cmd_le_gap_set_advertise_timing(0, 160, 160, 0, 0);
	  gecko_cmd_le_gap_set_advertise_channel_map(0, 7);
	  gecko_cmd_le_gap_set_advertise_phy(0, le_gap_phy_1m, le_gap_phy_1m);
	  gecko_cmd_le_gap_start_advertising(0, le_gap_general_discoverable, le_gap_connectable_scannable);

	  advStopped = false;
  } else {

      gecko_cmd_le_gap_set_discovery_type(5, 0);
      gecko_cmd_le_gap_set_discovery_timing(5, 16, 16);
      gecko_cmd_le_gap_start_discovery(le_gap_phy_1m, le_gap_discover_observation);
  }
}

/**
 * @brief handle_button_change
 * Callback function to handle buttons press and release actions
 * @param pin -  push button mask
 */
void handle_button_change(uint8_t pin) {

  if(pin == BSP_BUTTON0_PIN) {

    if(!GPIO_PinInGet(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN)) {
      // PB0 pressed down
      if(roleIsSlave) {
        gecko_external_signal(NOTIFICATIONS_START);
      }
    } else {
      // PB0 released
      if(roleIsSlave) {
        gecko_external_signal(NOTIFICATIONS_END);
      }
    }
  } else if(pin == BSP_BUTTON1_PIN) {

    if (!GPIO_PinInGet(BSP_BUTTON1_PORT, BSP_BUTTON1_PIN)) {
      // PB1 pressed
      if (roleIsSlave) {
        gecko_external_signal(INDICATIONS_START);
        buttonOneReleased = false;
      } else {
        // Role: Master
        if (state == ADV_SCAN) {
          // In master role, PB1 toggles scanning PHY between 1M and LE Coded
          gecko_external_signal(SCAN_PHY_CHANGE);
        } else {
          // When connected, PB1 toggles PHY between 1M, 2M and LE Coded
          gecko_external_signal(PHY_CHANGE);
        }
      }
    } else {
      // PB1 released
      if (roleIsSlave) {
        gecko_external_signal(INDICATIONS_END);
        buttonOneReleased = true;
      }
    }
  }
}

/**
 * @brief refresh_display
 * Routine to refresh the info on the display based on the Bluetooth connection status
 */
void refresh_display(void) {
  GRAPHICS_Clear();

  GRAPHICS_AppendString(roleString);
  sprintf(txPowerString + 4, ((txPowerResp / 10) == 0) ? "%01d dBm" : "%+0d dBm", txPowerResp / 10); // 0 dBm without sign
  GRAPHICS_AppendString(txPowerString);
  GRAPHICS_AppendString(statusString);
  GRAPHICS_AppendString(phyString);
  GRAPHICS_AppendString(connIntervalString);
  GRAPHICS_AppendString(pduSizeString);
  GRAPHICS_AppendString(mtuSizeString);
  GRAPHICS_AppendString(maxDataSizeString);

  if (roleIsSlave) {
    GRAPHICS_AppendString(notifyString);
    GRAPHICS_AppendString(indicateString);
  }

  sprintf(throughputString + 4, "%07lu", throughput);
  throughputString[11] = ' ';
  throughputString[12] = 'b';
  throughputString[13] = 'p';
  throughputString[14] = 's';
  GRAPHICS_AppendString(throughputString);
  sprintf(operationCountString + 5, "%09lu", operationCount);
  GRAPHICS_AppendString(operationCountString);

  GRAPHICS_Update();
}

/**
 * @brief calculate_notification_size
 * Calculate optimal notification size given current PDU and MTU sizes. 
 */
void calculate_notification_size(void) {
  if (DATA_TRANSFER_SIZE_NOTIFICATIONS == 0 || DATA_TRANSFER_SIZE_NOTIFICATIONS > (mtuSize - NOTIFICATION_GATT_HEADER)) {
    if ((pduSize != 0) && (mtuSize != 0)) {
      // Optimally split over multiple over-the-air packets.
      if (pduSize <= mtuSize) {
        maxDataSizeNotifications = (pduSize - (L2CAP_HEADER + NOTIFICATION_GATT_HEADER))
                                    + ((mtuSize - NOTIFICATION_GATT_HEADER - pduSize + (L2CAP_HEADER + NOTIFICATION_GATT_HEADER)) / pduSize * pduSize);
      } else {
        // Single over-the-air packet, but accommodate room for headers.
        if ((pduSize - mtuSize) <= L2CAP_HEADER) {
          maxDataSizeNotifications = pduSize - (L2CAP_HEADER + NOTIFICATION_GATT_HEADER); // LL PDU size - (L2CAP+GATT Headers)
        } else {
          // Room for the whole MTU, so data payload is MTU - Header of operation.
          maxDataSizeNotifications = mtuSize - NOTIFICATION_GATT_HEADER; // MTU - GATT Header
        }
      }
    }
  } else {
    maxDataSizeNotifications = DATA_TRANSFER_SIZE_NOTIFICATIONS;
  }
}

/**
 * @brief calculate_indication_size
 * Calculate indication size given current MTU size.
 */
void calculate_indication_size(void) {
  // MTU - 3B for indication GATT operation header.
  if (DATA_TRANSFER_SIZE_INDICATIONS == 0 || DATA_TRANSFER_SIZE_INDICATIONS > (mtuSize - INDICATION_GATT_HEADER)) {
    maxDataSizeIndications = mtuSize - INDICATION_GATT_HEADER; // If larger than max, use max for operation.
  } else {
    maxDataSizeIndications = DATA_TRANSFER_SIZE_INDICATIONS; // If smaller, use given.
  }
}

/**
 * @brief generate_notifications_data
 * Function to generate circular data (0-255) in the data payload
 */
void generate_notifications_data(void) {

  notificationsData[0] = notificationsData[maxDataSizeNotifications - 1] + 1;

  for (int i = 1; i < maxDataSizeNotifications; i++) {
    notificationsData[i] = notificationsData[i - 1] + 1;
  }
}

/**
 * @brief generate_indications_data
 * Function to generate circular data (0-255) in the data payload
 */
void generate_indications_data(void) {

  indicationsData[0] = indicationsData[maxDataSizeIndications - 1] + 1;

  for (int i = 1; i < maxDataSizeIndications; i++) {
    indicationsData[i] = indicationsData[i - 1] + 1;
  }
}

/**
 * @brief start_data_transmission
 * Sets up counter variables and writes 1 to transmission_on to indicate start
 */
void start_data_transmission(void) {
  // Slave tells master to turn off display refresh, resets counters and starts timing a new measurement.
  bitsSent = 0;
  throughput = 0;
  timeElapsed = RTCC_CounterGet();

  // Turn OFF Display refresh on master side
  while(gecko_cmd_gatt_write_characteristic_value_without_response(connection, gattdb_transmission_on, 1, &TRANSMISSION_ON)->result != 0);
  // Stop display refresh
  while(gecko_cmd_hardware_set_soft_timer(0, SOFT_TIMER_DISPLAY_REFRESH_HANDLE, 0)->result != 0);
}

/**
 * @brief end_data_transmission
 * Does a few things after data transmissions ended. Calculate transmission time,
 * enable display refresh in master side.
 */
void end_data_transmission(void) {
  timeElapsed = RTCC_CounterGet() - timeElapsed;
  // Turn ON Display on master side - stack is probably still busy pushing the last few notifications out so we need to check output
  while(gecko_cmd_gatt_write_characteristic_value_without_response(connection, gattdb_transmission_on, 1, &TRANSMISSION_OFF)->result != 0);
  // Resume display refresh - stack is probably still busy pushing the last few notifications out so we need to check output
  while(gecko_cmd_hardware_set_soft_timer(HW_TICKS_PER_SECOND, SOFT_TIMER_DISPLAY_REFRESH_HANDLE, 0)->result != 0);
  // Calculate throughput
  throughput = (uint32_t) ((float) bitsSent / (float) ((float) timeElapsed / (float) HW_TICKS_PER_SECOND ));
  // Write result to local GATT to be looked up on e.g. smart phone.
  while(gecko_cmd_gatt_server_write_attribute_value(gattdb_throughput_result, 0, sizeof(throughput), (uint8_t *) (&throughput))->result != 0);
  // Send result to subscribed NCP host or SoC master. Check for wrong state error, which means the client isn't subscribed to indications on the result.
  if (gecko_cmd_gatt_server_send_characteristic_notification(connection, gattdb_throughput_result, sizeof(throughput), (uint8_t *) (&throughput))->result != bg_err_wrong_state) {
    gecko_cmd_gatt_server_send_characteristic_notification(connection, gattdb_throughput_result, sizeof(throughput), (uint8_t *) (&throughput));
  }

}

/**
 * @brief handle_universal_events
 * This handles events that are 'universal' in the sense that 
 * they need to be handled regardless of program state
 * @param evt - The same stack event processed by main event loop
 */
void handle_universal_events(struct gecko_cmd_packet *evt) {
  // Handle universal events
  switch (BGLIB_MSG_ID(evt->header)) {
    case gecko_evt_system_external_signal_id:
      switch (evt->data.evt_system_external_signal.extsignals) {

        case NOTIFICATIONS_START:
          // PB0 pressed down as slave.
          if ( (state == SUBSCRIBED) || (state == SUBSCRIBED_NOTIFICATIONS)) {
            state = NOTIFY;
            generate_notifications_data();
#if defined(SEND_FIXED_TRANSFER_TIME)
            gecko_cmd_hardware_set_soft_timer(SEND_FIXED_TRANSFER_TIME, SOFT_TIMER_FIXED_TRANSFER_TIME_HANDLE, 1);
            fixedTimeExpired = false;
#endif
            start_data_transmission();
            break;
          }
          break;

        case NOTIFICATIONS_END:
          // PB0 released as slave.
          if (state == NOTIFY) {
#if !defined(SEND_FIXED_TRANSFER_COUNT) && !defined(SEND_FIXED_TRANSFER_TIME)
            end_data_transmission();
            if (notificationsSubscribed && indicationsSubscribed) {
              state = SUBSCRIBED;
            } else if (notificationsSubscribed) {
              state = SUBSCRIBED_NOTIFICATIONS;
            } else if (indicationsSubscribed) {
              state = SUBSCRIBED_INDICATIONS;
            } else {
              state = CONNECTED;
            }

#endif
            break;
          }
          break;

        case INDICATIONS_START:
          // PB1 pressed down as slave.
          if ( (state == SUBSCRIBED) || (state == SUBSCRIBED_INDICATIONS) ) {
            state = INDICATE;
            start_data_transmission();
            generate_indications_data();
#if defined(SEND_FIXED_TRANSFER_TIME)
            gecko_cmd_hardware_set_soft_timer(SEND_FIXED_TRANSFER_TIME, SOFT_TIMER_FIXED_TRANSFER_TIME_HANDLE, 1);
            fixedTimeExpired = false;
#endif
            while(gecko_cmd_gatt_server_send_characteristic_notification(connection, gattdb_throughput_indications, maxDataSizeIndications, indicationsData)->result != 0);
          }

          break;

        case INDICATIONS_END:
          break;

        case PHY_CHANGE:
          // PB1 pressed down as master.
          switch (phyInUse) {
            case PHY_1M:
#if defined(_SILICON_LABS_32B_SERIES_1_CONFIG_2) || defined(_SILICON_LABS_32B_SERIES_1_CONFIG_3) || defined(_SILICON_LABS_32B_SERIES_2_CONFIG_1)
              // We're on 1M PHY, go to 2M PHY - only supported by xG12 and xG13
              phyToUse = PHY_2M;
#endif
              break;

            case PHY_2M:
#if defined(_SILICON_LABS_32B_SERIES_1_CONFIG_3) || defined(_SILICON_LABS_32B_SERIES_2_CONFIG_1)
              // We're on 2M PHY, go to 125kbit Coded PHY (S=8) - only supported by xG13
              phyToUse = PHY_S8;
#else
              // We're on 2MPHY but with xG12, go back to 1M PHY
              phyToUse = PHY_1M;
#endif
              break;

            case PHY_S8:
              // We're on S8 PHY, go back to 1M PHY
              phyToUse = PHY_1M;
              break;

            default:
              break;
          }
          break;

        case SCAN_PHY_CHANGE:
					if (state == ADV_SCAN) {
						gecko_cmd_le_gap_end_procedure();
						switch (phyInUse) {
							case PHY_1M:
#if defined(_SILICON_LABS_32B_SERIES_1_CONFIG_3) || defined(_SILICON_LABS_32B_SERIES_2_CONFIG_1)
								gecko_cmd_le_gap_start_discovery(le_gap_phy_coded, le_gap_discover_observation);
								phyInUse = PHY_S8;
								sprintf(phyString + 5, "%s", "CODED S8");
#else
								gecko_cmd_le_gap_start_discovery(le_gap_phy_1m, le_gap_discover_observation);
#endif
								break;

							case PHY_2M:
								gecko_cmd_le_gap_start_discovery(le_gap_phy_1m, le_gap_discover_observation);
								phyInUse = PHY_1M;
								sprintf(phyString + 5, "%s", "1M");
								break;

							case PHY_S8:
								// We're on S8 PHY, go to 1M PHY
								gecko_cmd_le_gap_start_discovery(le_gap_phy_1m, le_gap_discover_observation);
								phyInUse = PHY_1M;
								sprintf(phyString + 5, "%s", "1M");
								break;

							default:
								break;
						}
					}
					break;

        default:
          break;
      } // switch-extsignals

      break;

    case gecko_evt_hardware_soft_timer_id:
      switch (evt->data.evt_hardware_soft_timer.handle) {
          case SOFT_TIMER_DISPLAY_REFRESH_HANDLE:
              gecko_cmd_le_connection_get_rssi(connection);
              refresh_display();
              break;
#ifdef SEND_FIXED_TRANSFER_TIME
          case SOFT_TIMER_FIXED_TRANSFER_TIME_HANDLE:
            fixedTimeExpired = true;
            if ((state == INDICATE) && waitingForConfirmation) {
              break;
            }

            end_data_transmission();
            state = SUBSCRIBED;
            break;
#endif
          default:
              break;
      }
      break;

    case gecko_evt_le_connection_parameters_id:
      pduSize = evt->data.evt_le_connection_parameters.txsize;
      interval = evt->data.evt_le_connection_parameters.interval;
      sprintf(pduSizeString + 5, "%03u", pduSize);
      sprintf(connIntervalString + 7, "%04u", (unsigned int) ((float) interval * 1.25));
      calculate_notification_size();
      sprintf(maxDataSizeString + 11, "%03u", maxDataSizeNotifications);
      statusString = (char *)statusConnectedString;
      break;

    case gecko_evt_gatt_mtu_exchanged_id:
      mtuSize = evt->data.evt_gatt_mtu_exchanged.mtu;
      sprintf(mtuSizeString + 5, "%03u", mtuSize);
      calculate_indication_size();
      calculate_notification_size();
      sprintf(maxDataSizeString + 11, "%03u", maxDataSizeNotifications);
      break;

    case gecko_evt_le_connection_rssi_id:
      sprintf(statusConnectedString + 6, "%03d", evt->data.evt_le_connection_rssi.rssi);
      break;

    case gecko_evt_le_connection_closed_id:
      // Set key variables to defaults and state to ADV_SCAN.
      reset_variables(); 
      set_display_defaults();
      // Check if need to boot to dfu mode
      if (boot_to_dfu) {
        gecko_cmd_system_reset(2);
      } else {
        // After disconnect, save the last used PHY to be the next to change into.
        phyToUse = phyInUse; 
        setup_adv_scan();
      }
      break;

    default:
      break;
  } // switch-universal events
}
