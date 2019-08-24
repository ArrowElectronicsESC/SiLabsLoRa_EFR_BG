/***************************************************************************//**
 * @file app_slave.c
 * @brief Slave mode functions:
 * slave_main: main event loop
 * check_subscription_status:  Client Characteristic Configuration checking
 ******************************************************************************/

#include <src/ble/app_utils.h>
#include  <kernel/include/os.h>

static void check_subscription_status(struct gecko_cmd_packet *evt);
static bool indicationTransmissionOngoing = false; // Tracks whether transmission is ongoing when triggered by other means besides buttons

/***************************************************************************************************
 * @brief Slave mode main loop
 **************************************************************************************************/
void slave_main(void) {

 RTOS_ERR  err;

  while (1) {
    /* Event pointer for handling events */
    struct gecko_cmd_packet *evt;

    evt = gecko_peek_event();
    													/* Delay Start Task execution for                    */
    OSTimeDly(1,                                        /* 500 OS Ticks                                      */
    		  OS_OPT_TIME_DLY,                          /* from now.                                         */
			  &err);

    /* Main state loop */
    switch (state) {
      case ADV_SCAN:

        switch (BGLIB_MSG_ID(evt->header) ) {

          case gecko_evt_system_boot_id:
          case gecko_evt_le_connection_closed_id:
        	reset_variables();
            indicationTransmissionOngoing = false;
            gecko_cmd_gatt_set_max_mtu(250);
            txPowerResp = gecko_cmd_system_set_tx_power(TX_POWER)->set_power; // 0.1 dBm count, stack may return something around the setpoint
            refresh_display();
            setup_adv_scan();
            gecko_cmd_hardware_set_soft_timer(HW_TICKS_PER_SECOND, SOFT_TIMER_DISPLAY_REFRESH_HANDLE, 0);
        	break;

          case gecko_evt_le_connection_opened_id:
            connection = evt->data.evt_le_connection_opened.connection;
            sprintf(phyString + 5, "%s", (phyInUse == PHY_S8) ? "CODED S8" : "1M");
            roleString = (char *)ROLE_SLAVE_STRING;
            state = CONNECTED;
            break;

          default:
            break;
        }
        break;

      case CONNECTED:

        switch (BGLIB_MSG_ID(evt->header) ) {
          case gecko_evt_le_connection_phy_status_id:
            update_displayed_phy(evt->data.evt_le_connection_phy_status.phy);
            break;

          // Slave/Server receives
          case gecko_evt_gatt_server_characteristic_status_id:
            check_subscription_status(evt);
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
        break;

      case SUBSCRIBED_NOTIFICATIONS:
        if (!advStopped) {
          gecko_cmd_le_gap_stop_advertising(0); // Stop advertising both sets. (Multiple connections are allowed, so it will continue to advertise the unconnected set)
          gecko_cmd_le_gap_stop_advertising(1);
          advStopped = true;
        }

        switch (BGLIB_MSG_ID(evt->header) ) {

          case gecko_evt_gatt_server_attribute_value_id:
            if (evt->data.evt_gatt_server_attribute_value.attribute == gattdb_transmission_on) {
              // Display ON/OFF state changes
              if (evt->data.evt_gatt_server_attribute_value.value.data[0] == TRANSMISSION_ON) {
                start_data_transmission();
                state = NOTIFY;
                generate_notifications_data();
              }
            }
            break;

          // Slave/Server receives
          case gecko_evt_gatt_server_characteristic_status_id:
            check_subscription_status(evt);

            if (notificationsSubscribed && indicationsSubscribed) {state = SUBSCRIBED;}
            break;

          case gecko_evt_le_connection_phy_status_id:
            update_displayed_phy(evt->data.evt_le_connection_phy_status.phy);
            break;

          default:
            break;
        }
        break;

      case SUBSCRIBED_INDICATIONS:
        if (!advStopped) {
          gecko_cmd_le_gap_stop_advertising(0); // Stop advertising both sets. (Multiple connections are allowed, so it will continue to advertise the unconnected set)
          gecko_cmd_le_gap_stop_advertising(1);
          advStopped = true;
        }

        switch (BGLIB_MSG_ID(evt->header) ) {

          case gecko_evt_gatt_server_attribute_value_id:
            if (evt->data.evt_gatt_server_attribute_value.attribute == gattdb_transmission_on) {
              // Display ON/OFF state changes
              if (evt->data.evt_gatt_server_attribute_value.value.data[0] == TRANSMISSION_ON) {
                start_data_transmission();
                indicationTransmissionOngoing = true;
                state = INDICATE;
                generate_indications_data();
                while(gecko_cmd_gatt_server_send_characteristic_notification(connection, gattdb_throughput_indications, maxDataSizeIndications, indicationsData)->result != 0);
                waitingForConfirmation = 1;
              }
            }
            break;

          // Slave/Server receives
          case gecko_evt_gatt_server_characteristic_status_id:
            check_subscription_status(evt);

            if (notificationsSubscribed && indicationsSubscribed) { state = SUBSCRIBED;}
            break;

          case gecko_evt_le_connection_phy_status_id:
            update_displayed_phy(evt->data.evt_le_connection_phy_status.phy);
            break;
          default:
            break;
        }
        break;

      case SUBSCRIBED:

        if (!advStopped) {
          gecko_cmd_le_gap_stop_advertising(0); // Stop advertising both sets. (Multiple connections are allowed, so it will continue to advertise the unconnected set)
          gecko_cmd_le_gap_stop_advertising(1);
          advStopped = true;
        }

        switch (BGLIB_MSG_ID(evt->header) ) {
          case gecko_evt_le_connection_phy_status_id:
            update_displayed_phy(evt->data.evt_le_connection_phy_status.phy);
            break;

          // Slave/Server receives
          case gecko_evt_gatt_server_characteristic_status_id:
            check_subscription_status(evt);
            break;
          default:
            break;
        }
        break;

      case NOTIFY:
        // As slave, just send and move on.
        switch (BGLIB_MSG_ID(evt->header)) {
          case gecko_evt_gatt_server_attribute_value_id:
            if (evt->data.evt_gatt_server_attribute_value.attribute == gattdb_transmission_on) {
              if (evt->data.evt_gatt_server_attribute_value.value.data[0] == TRANSMISSION_OFF) {
                timeElapsed = RTCC_CounterGet() - timeElapsed;
                // Enable display refresh
                while (gecko_cmd_hardware_set_soft_timer(HW_TICKS_PER_SECOND, SOFT_TIMER_DISPLAY_REFRESH_HANDLE, 0)->result != 0);
                // Calculate throughput
                throughput = (uint32_t) ((float) bitsSent / (float) ((float) timeElapsed / (float) HW_TICKS_PER_SECOND ));
                // Write result to local GATT to be looked up on e.g. smart phone.
                while(gecko_cmd_gatt_server_write_attribute_value(gattdb_throughput_result, 0, sizeof(throughput), (uint8_t *) (&throughput))->result != 0);
                // Send result to subscribed NCP host or SoC master. Check for wrong state error, which means the client isn't subscribed to indications on the result.
                if (gecko_cmd_gatt_server_send_characteristic_notification(connection, gattdb_throughput_result, sizeof(throughput), (uint8_t *) (&throughput))->result != bg_err_wrong_state) {
                  gecko_cmd_gatt_server_send_characteristic_notification(connection, gattdb_throughput_result, sizeof(throughput), (uint8_t *) (&throughput));
                }

                if (notificationsSubscribed && indicationsSubscribed) {
                  state = SUBSCRIBED;
                } else {
                  state = SUBSCRIBED_NOTIFICATIONS;
                }
              }
            }
            break;

          default:
            break;
        }

        if(gecko_cmd_gatt_server_send_characteristic_notification(connection, gattdb_throughput_notifications, maxDataSizeNotifications, notificationsData)->result == 0) {
          bitsSent += (maxDataSizeNotifications * 8);
          operationCount++;
          generate_notifications_data();
#ifdef SEND_FIXED_TRANSFER_COUNT
          if (bitsSent >= (SEND_FIXED_TRANSFER_COUNT * 8)) {
            end_data_transmission();
            if (notificationsSubscribed && indicationsSubscribed) {
              state = SUBSCRIBED;
            } else {
              state = SUBSCRIBED_NOTIFICATIONS;
            }
          }
#endif
        }
        break;

      case INDICATE:
        switch(BGLIB_MSG_ID(evt->header)) {

          case gecko_evt_gatt_server_attribute_value_id:
            if (evt->data.evt_gatt_server_attribute_value.attribute == gattdb_transmission_on) {
              if (evt->data.evt_gatt_server_attribute_value.value.data[0] == TRANSMISSION_OFF) {
                indicationTransmissionOngoing = false;
                timeElapsed = RTCC_CounterGet() - timeElapsed;
                // Enable display refresh
                while(gecko_cmd_hardware_set_soft_timer(HW_TICKS_PER_SECOND, SOFT_TIMER_DISPLAY_REFRESH_HANDLE, 0)->result != 0);
                // Calculate throughput
                throughput = (uint32_t) ((float) bitsSent / (float) ((float) timeElapsed / (float) HW_TICKS_PER_SECOND ));
                // Write result to local GATT to be looked up on e.g. smart phone.
                while(gecko_cmd_gatt_server_write_attribute_value(gattdb_throughput_result, 0, sizeof(throughput), (uint8_t *) (&throughput))->result != 0);
                // Send result to subscribed NCP host or SoC master. Check for wrong state error, which means the client isn't subscribed to indications on the result.
                if (gecko_cmd_gatt_server_send_characteristic_notification(connection, gattdb_throughput_result, sizeof(throughput), (uint8_t *) (&throughput))->result != bg_err_wrong_state) {
                  gecko_cmd_gatt_server_send_characteristic_notification(connection, gattdb_throughput_result, sizeof(throughput), (uint8_t *) (&throughput));
                }

                if (notificationsSubscribed && indicationsSubscribed) {
                  state = SUBSCRIBED;
                } else {
                  state = SUBSCRIBED_INDICATIONS;
                }
              }
            }
            break;

          case gecko_evt_gatt_server_characteristic_status_id:
            if (evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_throughput_indications) {
              if (evt->data.evt_gatt_server_characteristic_status.status_flags == gatt_server_confirmation) {
                // Last indicate operation was acknowledged, send more data
                bitsSent += ((maxDataSizeIndications) * 8);
                operationCount++;
                waitingForConfirmation = 0; // When received confirmation, set flag to zero.
#ifdef SEND_FIXED_TRANSFER_COUNT
                if (bitsSent >= (SEND_FIXED_TRANSFER_COUNT * 8)) {
                  end_data_transmission();
                  if (notificationsSubscribed && indicationsSubscribed) {
                    state = SUBSCRIBED;
                  } else {
                    state = SUBSCRIBED_INDICATIONS;
                  }
                  break;
                } else {
                  generate_indications_data();
                  while(gecko_cmd_gatt_server_send_characteristic_notification(connection, gattdb_throughput_indications, maxDataSizeIndications, indicationsData)->result != 0);
                  waitingForConfirmation = 1;
                  break;
                }
#endif
#ifdef SEND_FIXED_TRANSFER_TIME
                // Check if timer event has been fired
                if (fixedTimeExpired) {
                  end_data_transmission();
                  if (notificationsSubscribed && indicationsSubscribed) {
                    state = SUBSCRIBED;
                  } else {
                    state = SUBSCRIBED_INDICATIONS;
                  }
                  break;
                } else {
                  generate_indications_data();
                  while(gecko_cmd_gatt_server_send_characteristic_notification(connection, gattdb_throughput_indications, maxDataSizeIndications, indicationsData)->result != 0);
                  waitingForConfirmation = 1;
                  break;
                }
#endif
              }

              if (indicationsSubscribed && (!buttonOneReleased || waitingForConfirmation || indicationTransmissionOngoing)) {
                generate_indications_data();
                while(gecko_cmd_gatt_server_send_characteristic_notification(connection, gattdb_throughput_indications, maxDataSizeIndications, indicationsData)->result != 0);
                waitingForConfirmation = 1;
              } else {
                end_data_transmission();
                indicationTransmissionOngoing = false;

                // Fall back to both or just indications subscribed
                if (notificationsSubscribed && indicationsSubscribed) {
                  state = SUBSCRIBED;
                } else if (indicationsSubscribed) {
                  state = SUBSCRIBED_INDICATIONS;
                } else {
                  state = CONNECTED;
                }
              }

            }
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

/**
 * @brief check_subscription_status
 * Check if GATT Client has changed the CCC, and enable notifications or indications accordingly.
 * @param evt - event struct from main loop passed in
 */
static void check_subscription_status(struct gecko_cmd_packet *evt) {
  // Check if Client Characteristic Configuration has been changed by client.
  if (evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_throughput_notifications) {
    if (evt->data.evt_gatt_server_characteristic_status.status_flags == gatt_server_client_config) {
      // Notifications on.
      if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == gatt_notification) {
          state = SUBSCRIBED_NOTIFICATIONS;
          notificationsSubscribed = true;
          notifyString = (char *)NOTIFY_ENABLED_STRING;
      }
      // Notifications off
      if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == gatt_disable) {
        notificationsSubscribed = false;
        notifyString = (char *)NOTIFY_DISABLED_STRING;

        if (indicationsSubscribed) {
          state = SUBSCRIBED_INDICATIONS;
        } else {
          state = CONNECTED;
        }
      }
    }
  }

  if (evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_throughput_indications) {
    if (evt->data.evt_gatt_server_characteristic_status.status_flags == gatt_server_client_config) {
      // Indications on.
      if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == gatt_indication) {
        state = SUBSCRIBED_INDICATIONS;
        indicationsSubscribed = true;
        indicateString = (char *)INDICATE_ENABLED_STRING;
      }
      // Indications off.
      if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == gatt_disable) {
        indicateString = (char *)INDICATE_DISABLED_STRING;
        indicationsSubscribed = false;

        if (notificationsSubscribed) {
          state = SUBSCRIBED_NOTIFICATIONS;
        } else {
          state = CONNECTED;
        }

      }
    }
  }
}
