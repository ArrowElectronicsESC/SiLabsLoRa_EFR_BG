/**************************************************************************//**
 * @file
 * @brief helper functions for configuring SWO
 * @version 4.4.0
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/
/**************************************************************************/
#include "em_cmu.h"
#include "bsp_trace.h"

int RETARGET_WriteChar(char c)
{
  return ITM_SendChar(c);
}

int RETARGET_ReadChar(void)
{
  return 0;
}

void RETARGET_SwoInit(void)
{
  BSP_TraceSwoSetup();
}
