/**
 ******************************************************************************
 * @file    main.h
 * @author  MEMS Software Solutions Team
 * @brief   This file contains definitions for the main.c file.
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
#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "cube_hal.h"
#include "nucleo_l476rg_bus.h"
#include "nucleo_l476rg_errno.h"
#include "iks01a3_env_sensors.h"
#include "iks01a3_env_sensors_ex.h"
#include "iks01a3_motion_sensors.h"
#include "iks01a3_motion_sensors_ex.h"
#include "iks01a3_hybrid_sensors.h"
#include "iks01a3_hybrid_sensors_ex.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported defines ----------------------------------------------------------*/

/* Enable sensor masks */
#define PRESSURE_SENSOR_ENABLED                 0x00000001U
#define TEMPERATURE_SENSOR_ENABLED              0x00000002U
#define HUMIDITY_SENSOR_ENABLED                 0x00000004U
#define UV_SENSOR_ENABLED                       0x00000008U /* for future use */
#define ACCELEROMETER_SENSOR_ENABLED            0x00000010U
#define GYROSCOPE_SENSOR_ENABLED                0x00000020U
#define MAGNETIC_SENSOR_ENABLED                 0x00000040U
#define INTERRUPTS_ENABLED                      0x00000100U
#define FSM_ENABLED                             0x00000200U
#define MLC_ENABLED                             0x00000400U

extern int UseLSI;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
