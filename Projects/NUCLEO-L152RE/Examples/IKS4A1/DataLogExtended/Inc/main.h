/**
  ******************************************************************************
  * @file    main.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains definitions for the main.c file.
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
#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "cube_hal.h"
#include "nucleo_l152re_bus.h"
#include "nucleo_l152re_errno.h"
#include "iks4a1_env_sensors.h"
#include "iks4a1_env_sensors_ex.h"
#include "iks4a1_motion_sensors.h"
#include "iks4a1_motion_sensors_ex.h"
#include "iks4a1_hybrid_sensors.h"
#include "iks4a1_hybrid_sensors_ex.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported defines ----------------------------------------------------------*/

/* Enable sensor masks */
#define PRESSURE_SENSOR_ENABLED          0x00000001U
#define TEMPERATURE_SENSOR_ENABLED       0x00000002U
#define HUMIDITY_SENSOR_ENABLED          0x00000004U
#define UV_SENSOR_ENABLED                0x00000008U
#define ACCELEROMETER_SENSOR_ENABLED     0x00000010U
#define GYROSCOPE_SENSOR_ENABLED         0x00000020U
#define MAGNETIC_SENSOR_ENABLED          0x00000040U
#define INTERRUPTS_ENABLED               0x00000100U
#define FSM_ENABLED                      0x00000200U
#define MLC_ENABLED                      0x00000400U
#define QVAR_ENABLED                     0x00001000U
#define ACCELEROMETER_HG_SENSOR_ENABLED  0x00002000U

/* Sync sensor masks */
#define PRESSURE_SENSOR_SYNC          0x00000001U
#define TEMPERATURE_SENSOR_SYNC       0x00000002U
#define HUMIDITY_SENSOR_SYNC          0x00000004U
#define ACCELEROMETER_SENSOR_SYNC     0x00000008U
#define GYROSCOPE_SENSOR_SYNC         0x00000010U
#define MAGNETIC_SENSOR_SYNC          0x00000020U
#define INTERRUPTS_SYNC               0x00000040U
#define MLC_SYNC                      0x00000080U
#define FSM_SYNC                      0x00000100U
#define ISPU_SYNC                     0x00000200U
#define QVAR_SYNC                     0x00000400U
#define ACCELEROMETER_HG_SENSOR_SYNC  0x00000800U

extern int32_t UseLSI;
extern uint32_t SensorsEnabled;
extern uint32_t StartTime;
extern uint8_t UpdateInterrupt;

/* Exported functions --------------------------------------------------------*/
void Error_Handler(void);
void RTC_TimeRegulate(uint8_t hh, uint8_t mm, uint8_t ss);
uint32_t DWT_GetTickUS(void);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */
