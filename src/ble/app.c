/***************************************************************************//**
 * @file
 * @brief app.c
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

/* Bluetooth stack headers */
#include <src/ble/app.h>
#include <src/ble/app_utils.h>
#include "bg_types.h"
#include "rtos_gecko.h"
#include "gatt_db.h"
#include "gpiointerrupt.h"
#include "graphics.h"
#include "init_mcu.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_rtcc.h"
#include "rtos_bluetooth.h"

/* Main application */
void appMain(void *pconfig)
{
#if DISABLE_SLEEP > 0
  pconfig->sleep.flags = 0;
#endif

  gecko_configuration_t *pcurrconfig;

  pcurrconfig = (gecko_configuration_t *) pconfig;

  /* Initialize debug prints. Note: debug prints are off by default. See DEBUG_LEVEL in app.h */
  initLog();

  pcurrconfig->bluetooth.max_advertisers = 4; // Change to allow more than one connectable advertisement set

  // Adaptive frequency hopping enabled if transmit power set to over 10 dBm
#if (TX_POWER > 100)
  gecko_init_afh();
#endif

#if defined(_SILICON_LABS_32B_SERIES_1_CONFIG_3) //|| defined(_SILICON_LABS_32B_SERIES_2_CONFIG_1)
  // RTCC init
 void initMcu_RTCC(void) {
    /* Enable clock to LE modules */
    CMU_ClockEnable(cmuClock_CORELE, true);
    /* Enable clock for RTCC */
    CMU_ClockEnable(cmuClock_RTCC, true);
    RTCC_Init_TypeDef rtccInit = RTCC_INIT_DEFAULT;

    rtccInit.enable                = true;
    rtccInit.debugRun              = false;
    rtccInit.precntWrapOnCCV0      = false;
    rtccInit.cntWrapOnCCV1         = false;
    rtccInit.prescMode             = rtccCntTickPresc;
    rtccInit.presc                 = rtccCntPresc_1;
    rtccInit.enaOSCFailDetect      = false;
    rtccInit.cntMode               = rtccCntModeNormal;

    RTCC_Init(&rtccInit);
  }
 initMcu_RTCC();
#endif

  // Callback func assignment for PB0 and PB1
  GPIOINT_CallbackRegister(BSP_BUTTON0_PIN, handle_button_change);
  GPIOINT_CallbackRegister(BSP_BUTTON1_PIN, handle_button_change);

  // Set mode to master if PB0 pressed at boot time.
  if (GPIO_PinInGet(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN)) {
    roleIsSlave = true;
    roleString = (char *)ROLE_ADVERT_STRING;
  } else {
    roleIsSlave = false;
    roleString = (char *)ROLE_SCANNER_STRING;
  }

  // Initialize display
  GRAPHICS_Init();

  reset_variables();
  gecko_cmd_gatt_set_max_mtu(250);
  txPowerResp = gecko_cmd_system_set_tx_power(TX_POWER)->set_power; // 0.1 dBm count, stack may return something around the setpoint
  refresh_display();
  setup_adv_scan();
  gecko_cmd_hardware_set_soft_timer(HW_TICKS_PER_SECOND, SOFT_TIMER_DISPLAY_REFRESH_HANDLE, 0);

  while (DEF_TRUE) {
	  if (roleIsSlave) {

		  GRAPHICS_Clear();
		  GRAPHICS_AppendString("Throughput Test\nRole: Slave\n");
		  GRAPHICS_Update();

		  slave_main();
	  } else {

		  GRAPHICS_Clear();
		  GRAPHICS_AppendString("Throughput Test\nRole: Master\n");
		  GRAPHICS_Update();

		  master_main();
	  }
  }
}
