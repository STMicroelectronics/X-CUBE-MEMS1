/**
  ******************************************************************************
  * @file    DemoSerial.h
  * @author  MEMS Software Solutions Team
  * @brief   Header for DemoSerial.c
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014-2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DEMO_SERIAL_H
#define DEMO_SERIAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "cube_hal.h"
#include "serial_protocol.h"
#include "Serial_CMD.h"

/* Private defines -----------------------------------------------------------*/
#define SENDER_UART  0x01
#define SENDER_USB   0x02
#define SENDER_SPI   0x03

#define DEV_ADDR                        50U
#define I2C_DATA_MAX_LENGTH_BYTES       16
#define STREAMING_MSG_LENGTH            68
#define MIN(A,B) ((A)<(B)?(A):(B))

/* Exported variables --------------------------------------------------------*/
extern volatile uint8_t DataLoggerActive;
extern UART_HandleTypeDef UartHandle;

/* Exported functions ------------------------------------------------------- */
int32_t HandleMSG(Msg_t *Msg);

void SEND_BOARD_RESTARTED_MSG(Msg_t *Msg);
void SEND_INIT_ERR_MSG(Msg_t *Msg);

/* Private functions -------------------------------------------------------- */
void BUILD_REPLY_HEADER(Msg_t *Msg);
void INIT_STREAMING_MSG(Msg_t *Msg);
void BUILD_NACK_HEADER(Msg_t *Msg);
void INIT_STREAMING_HEADER(Msg_t *Msg);

#ifdef __cplusplus
}
#endif

#endif /* DEMO_SERIAL_H */
