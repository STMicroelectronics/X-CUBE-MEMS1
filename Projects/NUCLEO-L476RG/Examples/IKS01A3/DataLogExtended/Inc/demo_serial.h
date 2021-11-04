/**
 *******************************************************************************
 * @file    demo_serial.h
 * @author  MEMS Software Solutions Team
 * @brief   Header for demo_serial.c.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Software License Agreement
  * SLA0077, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0077
  *
  ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DEMO_SERIAL_H
#define DEMO_SERIAL_H

#include "cube_hal.h"
#include "serial_protocol.h"
#include "Serial_CMD.h"

#define SENDER_UART                     0x01
#define SENDER_USB                      0x02
#define SENDER_SPI                      0x03

#define DEV_ADDR                        50U
#define I2C_DATA_MAX_LENGTH_BYTES       16

#define STREAMING_MSG_LENGTH            81

#define NOT_HYBRID_SENSOR               0x00U
#define HYBRID_SENSOR                   0x01U

extern volatile uint8_t DataLoggerActive;

int HandleMSG(TMsg *Msg);
void BUILD_REPLY_HEADER(TMsg *Msg);
void BUILD_NACK_HEADER(TMsg *Msg);
void INIT_STREAMING_MSG(TMsg *Msg);
void INIT_STREAMING_HEADER(TMsg *Msg);

#endif /* DEMO_SERIAL_H */
