/**
  ******************************************************************************
  * @file    demo_serial.h
  * @author  MEMS Software Solutions Team
  * @brief   Header for demo_serial.c
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014-2021 STMicroelectronics.
  * All rights reserved
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

#include "serial_cmd.h"
#include "cube_hal.h"
#include "serial_protocol.h"

#define SENDER_UART                     0x01
#define SENDER_USB                      0x02
#define SENDER_SPI                      0x03

#define DEV_ADDR                        50U
#define I2C_DATA_MAX_LENGTH_BYTES       16

#define STREAMING_MSG_LENGTH            81  // TODO: Is it used properly?

#define NOT_HYBRID_SENSOR               0x00U
#define HYBRID_SENSOR                   0x01U

extern volatile uint8_t DataLoggerActive;

int32_t HandleMSG(Msg_t *Msg);
void BUILD_REPLY_HEADER(Msg_t *Msg);
void BUILD_NACK_HEADER(Msg_t *Msg);
void INIT_STREAMING_MSG(Msg_t *Msg);
void INIT_STREAMING_HEADER(Msg_t *Msg);

#endif /* DEMO_SERIAL_H */
