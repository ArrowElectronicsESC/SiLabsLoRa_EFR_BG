/**************************************************************************//**
 * @file
 * @brief EFM32 Segment LCD Display driver, header file
 * @version 4.4.0
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/
/**************************************************************************/
#ifndef RETARGETSWO_H
#define RETARGETSWO_H

/***************************************************************************/
/***************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

int RETARGET_WriteChar(char c);
int RETARGET_ReadChar(void);
void RETARGET_SwoInit(void);

#define setupSWOForPrint RETARGET_SwoInit

#ifdef __cplusplus
}
#endif

#endif
