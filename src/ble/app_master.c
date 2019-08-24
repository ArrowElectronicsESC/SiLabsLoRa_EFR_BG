/***************************************************************************//**
 * @file app_master.c
 * @brief Master mode functions:
 * master_main: main event loop
 * process_scan_response:  filter through AD data to identify slave device
 ******************************************************************************/

#include <src/ble/app_utils.h>
#include  <kernel/include/os.h>

/**************************************************************************//**
 * MASTER SIDE MACROS
 *****************************************************************************/
#define CONN_INTERVAL_1MPHY_MAX     40		    // 40 * 1.25ms = 50ms
#define CONN_INTERVAL_1MPHY_MIN     40		    // 40 * 1.25ms = 50ms
#define SLAVE_LATENCY_1MPHY         0			    // How many connection intervals can the slave skip if no data is to be sent
#define SUPERVISION_TIMEOUT_1MPHY   100       // 100 * 10ms = 1000ms
#define CONN_INTERVAL_2MPHY_MAX     20		    // 20 * 1.25ms = 25ms
#define CONN_INTERVAL_2MPHY_MIN     20		    // 20 * 1.25ms = 25ms
#define SLAVE_LATENCY_2MPHY         0			    // How many connection intervals can the slave skip if no data is to be sent
#define SUPERVISION_TIMEOUT_2MPHY   100       // 100 * 10ms = 1000ms
#define CONN_INTERVAL_125KPHY_MAX   160       // 160 * 1.25ms = 200ms
#define CONN_INTERVAL_125KPHY_MIN   160       // 160 * 1.25ms = 200ms
#define SLAVE_LATENCY_125KPHY       0			    // How many connection intervals can the slave skip if no data is to be sent
#define SUPERVISION_TIMEOUT_125KPHY 200       // 200 * 10ms = 2000ms

const char DEVICE_NAME_STRING[] = "Throughput Tester";    // Device name to match against scan results.

static int process_scan_response(struct gecko_msg_le_gap_scan_response_evt_t *pResp);

/***************************************************************************************************
 * @brief Master mode main loop
 **************************************************************************************************/
void master_main(void) {

	RTOS_ERR  err;

//void master_main(struct gecko_cmd_packet *evt) {
  while (1) {
    /* Event pointer for handling events */
    struct gecko_cmd_packet *evt;

    evt = gecko_peek_event();

    /* Delay Start Task execution for                       */
    OSTimeDly( 1,                                        /*   500 OS Ticks                                      */
    		OS_OPT_TIME_DLY,                             /*   from now.                                          */
			&err);


    /* Main state loop */
    switch (state) {

      case ADV_SCAN:

        switch (BGLIB_MSG_ID(evt->header) ) {

          case gecko_evt_system_boot_id:
          case gecko_evt_le_connection_closed_id:

            reset_variables();
            gecko_cmd_gatt_set_max_mtu(250);
            txPowerResp = gecko_cmd_system_set_tx_power(TX_POWER)->set_power; // 0.1 dBm count, stack may return something around the setpoint
            refresh_display();
            gecko_cmd_hardware_set_soft_timer(HW_TICKS_PER_SECOND, SOFT_TIMER_DISPLAY_REFRESH_HANDLE, 0);

            setup_adv_scan();
            break;

          case gecko_evt_le_gap_scan_response_id:
            if (process_scan_response(&(evt->data.evt_le_gap_scan_response)) > 0) {
              gecko_cmd_le_gap_end_procedure(); // Stop scanning in the background.
              gecko_cmd_le_gap_connect(evt->data.evt_le_gap_scan_response.address,
                                      evt->data.evt_le_gap_scan_response.address_type,
                                      (phyInUse == le_gap_phy_2m) ? le_gap_phy_1m : phyInUse);
            }
            break;

          case gecko_evt_le_connection_opened_id:
            connection = evt->data.evt_le_connection_opened.connection;
            sprintf(phyString + 5, "%s", (phyInUse == PHY_S8) ? "CODED S8" : "1M");
            roleString = (char *)ROLE_MASTER_STRING;
            state = CONNECTED;
            break;

          default:
            break;
        }
        break;

      case CONNECTED:

        if (phyToUse) {
          gecko_cmd_le_connection_set_phy(connection, phyToUse);  // If change is needed, change PHY.
        }

        switch (BGLIB_MSG_ID(evt->header) ) {
          case gecko_evt_le_connection_phy_status_id:
            // Once a PHY change has been done, reset phyToUse and set conn parameters according to used PHY.
            // set_timing_parameters replaces connection_set_parameters in 2.12
            // Last 2 parameters are min and max length of connection event.
            phyToUse = 0;
            phyInUse = evt->data.evt_le_connection_phy_status.phy;

            switch (phyInUse) {
              case PHY_1M:
                sprintf(phyString + 5, "%s", "1M");
                gecko_cmd_le_connection_set_timing_parameters(connection,
                                                    CONN_INTERVAL_1MPHY_MIN,
                                                    CONN_INTERVAL_1MPHY_MAX,
                                                    SLAVE_LATENCY_1MPHY,
                                                    SUPERVISION_TIMEOUT_1MPHY,
                                                    0, 
                                                    0xFFFF);
                break;

              case PHY_2M:
                sprintf(phyString + 5, "%s", "2M");
                gecko_cmd_le_connection_set_timing_parameters(connection,
                                                    CONN_INTERVAL_2MPHY_MIN,
                                                    CONN_INTERVAL_2MPHY_MAX,
                                                    SLAVE_LATENCY_2MPHY,
                                                    SUPERVISION_TIMEOUT_2MPHY,
                                                    0, 
                                                    0xFFFF);
                break;

              case PHY_S8:
                sprintf(phyString + 5, "%s", "CODED S8");
                gecko_cmd_le_connection_set_timing_parameters(connection,
                                                    CONN_INTERVAL_125KPHY_MIN,
                                                    CONN_INTERVAL_125KPHY_MAX,
                                                    SLAVE_LATENCY_125KPHY,
                                                    SUPERVISION_TIMEOUT_125KPHY, 
                                                    0, 
                                                    0xFFFF);
                break;

              default:
                break;
            }
            break;

          /* Check if the user-type OTA Control Characteristic was written.
            * If ota_control was written, boot the device into Device Firmware Upgrade (DFU) mode. */
          case gecko_evt_gatt_server_user_write_request_id:
            if (evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_ota_control) {
              /* Set flag to enter to OTA mode */
              boot_to_dfu = 1;
              /* Send response to Write Request */
              gecko_cmd_gatt_server_send_user_write_response(evt->data.evt_gatt_server_user_write_request.connection,
                                                            gattdb_ota_control,
                                                            bg_err_success);
              /* Close connection to enter to DFU OTA mode */
              gecko_cmd_le_connection_close(evt->data.evt_gatt_server_user_write_request.connection);
            }
            break;
          default:
            break;
        }

        // Subscribe to Notifications and Indications data characteristics when PHY and interval have been set correctly.
        if (!phyToUse) {
          switch (phyInUse) {
            case PHY_1M:
              if (interval == CONN_INTERVAL_1MPHY_MIN) {
                gecko_cmd_gatt_set_characteristic_notification(connection, gattdb_throughput_notifications, gatt_notification);
                state = SUBSCRIBED_NOTIFICATIONS;
              }
              break;

            case PHY_2M:
              if (interval == CONN_INTERVAL_2MPHY_MIN) {
                gecko_cmd_gatt_set_characteristic_notification(connection, gattdb_throughput_notifications, gatt_notification);
                state = SUBSCRIBED_NOTIFICATIONS;
              }
              break;

            case PHY_S8:
              if (interval == CONN_INTERVAL_125KPHY_MIN) {
                gecko_cmd_gatt_set_characteristic_notification(connection, gattdb_throughput_notifications, gatt_notification);
                state = SUBSCRIBED_NOTIFICATIONS;
              }
              break;

            default:
              break;
          }
        }
        break;

      case SUBSCRIBED_NOTIFICATIONS:
        switch (BGLIB_MSG_ID(evt->header) ) {
          // Master/Client waits for completion of subscribing to notifications.
          case gecko_evt_gatt_procedure_completed_id:
            gecko_cmd_gatt_set_characteristic_notification(connection, gattdb_throughput_indications, gatt_indication);
            state = SUBSCRIBED_INDICATIONS;
            break;

          case gecko_evt_le_connection_phy_status_id:
            update_displayed_phy(evt->data.evt_le_connection_phy_status.phy);
            break;

          default:
            break;
        }
        break;

      case SUBSCRIBED_INDICATIONS:
        switch (BGLIB_MSG_ID(evt->header) ) {
          case gecko_evt_gatt_procedure_completed_id:
            state = SUBSCRIBED;
            break;

          case gecko_evt_le_connection_phy_status_id:
            update_displayed_phy(evt->data.evt_le_connection_phy_status.phy);
            break;

          default:
            break;
        }
        break;

      case SUBSCRIBED:
        if (phyToUse) {
          gecko_cmd_le_connection_set_phy(connection, phyToUse);  // If change is needed, change PHY.
        }

        switch (BGLIB_MSG_ID(evt->header) ) {
          // Turn off display on master
          case gecko_evt_gatt_server_attribute_value_id:
            // Write GATT to signal that transmission starts and display should be turned off.
            if (evt->data.evt_gatt_server_attribute_value.attribute == gattdb_transmission_on) {
              if (evt->data.evt_gatt_server_attribute_value.value.data[0] == TRANSMISSION_ON) {
                bitsSent = 0;
                throughput = 0;
                timeElapsed = RTCC_CounterGet();
                // Disable display refresh
                while(gecko_cmd_hardware_set_soft_timer(0, SOFT_TIMER_DISPLAY_REFRESH_HANDLE, 0)->result != 0);
                state = RECEIVE;
              }
            }
            break;

          case gecko_evt_le_connection_phy_status_id:
            phyToUse = 0;
            phyInUse = evt->data.evt_le_connection_phy_status.phy;

            switch (phyInUse) {
              case PHY_1M:
                sprintf(phyString + 5, "%s", "1M");
                gecko_cmd_le_connection_set_timing_parameters(connection,
                                                    CONN_INTERVAL_1MPHY_MIN,
                                                    CONN_INTERVAL_1MPHY_MAX,
                                                    SLAVE_LATENCY_1MPHY,
                                                    SUPERVISION_TIMEOUT_1MPHY, 
                                                    0, 
                                                    0xFFFF);
                break;
              case PHY_2M:
                sprintf(phyString + 5, "%s", "2M");
                gecko_cmd_le_connection_set_timing_parameters(connection,
                                                    CONN_INTERVAL_2MPHY_MIN,
                                                    CONN_INTERVAL_2MPHY_MAX,
                                                    SLAVE_LATENCY_2MPHY,
                                                    SUPERVISION_TIMEOUT_2MPHY, 
                                                    0, 
                                                    0xFFFF);
                break;
              case PHY_S8:
                sprintf(phyString + 5, "%s", "CODED S8");
                gecko_cmd_le_connection_set_timing_parameters(connection,
                                                    CONN_INTERVAL_125KPHY_MIN,
                                                    CONN_INTERVAL_125KPHY_MAX,
                                                    SLAVE_LATENCY_125KPHY,
                                                    SUPERVISION_TIMEOUT_125KPHY, 
                                                    0, 
                                                    0xFFFF);
                break;
              default:
                break;
            }
            break;

          default:
            break;
        }
        break;

      case RECEIVE:
        // Master exclusive state
        switch (BGLIB_MSG_ID(evt->header) ) {

          case gecko_evt_gatt_server_attribute_value_id:
            // Slave has written to master's GATT to signal that transmission is ending and display should be turned on.
            if (evt->data.evt_gatt_server_attribute_value.attribute == gattdb_transmission_on) {
              if (evt->data.evt_gatt_server_attribute_value.value.data[0] == TRANSMISSION_OFF) {
                timeElapsed = RTCC_CounterGet() - timeElapsed;
                // Enable display refresh
                while(gecko_cmd_hardware_set_soft_timer(HW_TICKS_PER_SECOND, SOFT_TIMER_DISPLAY_REFRESH_HANDLE, 0)->result != 0);
                // Calculate throughput
                throughput = (uint32_t) ((float) bitsSent / (float) ((float) timeElapsed / (float) HW_TICKS_PER_SECOND ));
                state = SUBSCRIBED;
              }
            }
            break;

          case gecko_evt_gatt_characteristic_value_id:
            /* Data received on master/client side */
            if (evt->data.evt_gatt_characteristic_value.characteristic == gattdb_throughput_indications) {
              if (evt->data.evt_gatt_characteristic_value.att_opcode == gatt_handle_value_indication) {
                gecko_cmd_gatt_send_characteristic_confirmation(evt->data.evt_gatt_characteristic_value.connection);
              }
            }
            bitsSent += (evt->data.evt_gatt_characteristic_value.value.len * 8);
            operationCount++;
            break;

          default:
            break;
        }
        break;
      default:
        break;
    }
    handle_universal_events(evt);
  }
}

/**************************************************************************//**
 * @brief process_scan_response
 * Processes advertisement packets looking for "Throughput Tester" device name
 *****************************************************************************/

/**
 * @brief process_scan_response
 * Processes advertisement packets looking for "Throughput Tester" device name.
 * More specific filtering logic can be added here.
 * @param pResp - scan_response structure passed in from scan_response event
 * @return adMatchFound - bool flag, 1 if correct name found
 */
static int process_scan_response(struct gecko_msg_le_gap_scan_response_evt_t *pResp) {
  int i = 0;
  int adMatchFound = 0;
  int adLen;
  int adType;

  while (i < (pResp->data.len - 1)) {
    adLen = pResp->data.data[i];
    adType = pResp->data.data[i + 1];

    // Type 0x09 = Complete Local Name, 0x08 Shortened Name
    if (adType == 0x09) {
      // Check if device name is Throughput Tester
      if (memcmp(pResp->data.data + i + 2, DEVICE_NAME_STRING, 17) == 0) {
        adMatchFound = 1;
        break;
      }
    }
    // Jump to next AD record
    i = i + adLen + 1;
  }

  return (adMatchFound);
}
