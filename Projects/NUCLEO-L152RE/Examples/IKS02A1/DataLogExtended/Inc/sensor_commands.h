/**
  ******************************************************************************
  * @file    sensor_commands.h
  * @author  MEMS Software Solutions Team
  * @brief   Header for sensor_commands.h
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

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef SENSOR_COMMANDS_H
#define SENSOR_COMMANDS_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "serial_protocol.h"

/* Exported defines ----------------------------------------------------------*/
#define SC_ACCELEROMETER                                                0x01
#define SC_GYROSCOPE                                                    0x02
#define SC_MAGNETOMETER                                                 0x03
#define SC_TEMPERATURE                                                  0x04
#define SC_HUMIDITY                                                     0x05
#define SC_PRESSURE                                                     0x06
#define SC_UV                                                           0x07

#define SC_GET_SENSOR_NAME                                              0x01
#define SC_READ_REGISTER                                                0x02
#define SC_WRITE_REGISTER                                               0x03
#define SC_GET_FULL_SCALE_LIST                                          0x04
#define SC_SET_FULL_SCALE                                               0x05
#define SC_GET_ODR_LIST                                                 0x06
#define SC_SET_ODR                                                      0x07
#define SC_GET_FULL_SCALE                                               0x1E
#define SC_GET_ODR                                                      0x1F
#define SC_GET_SENSOR_LIST                                              0x14
#define SC_SET_SENSOR_INDEX                                             0x15
#define SC_WRITE_MULTI_REGISTER                                         0x16
#define SC_READ_MULTI_REGISTER                                          0x17
#define SC_READ_MODIFY_WRITE_REGISTER                                   0x18

/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
extern uint32_t AccInstance;
extern uint32_t GyrInstance;
extern uint32_t MagInstance;
extern uint32_t HumInstance;
extern uint32_t TmpInstance;
extern uint32_t PrsInstance;

/* Private functions ---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int32_t Handle_Sensor_command(Msg_t *Msg);

#endif /* SENSOR_COMMANDS_H */
