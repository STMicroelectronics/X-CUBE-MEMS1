/**
  ******************************************************************************
  * @file    main.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains definitions for the main.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014-2021 STMicroelectronics.
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

#if (defined (USE_STM32F4XX_NUCLEO))
#include "nucleo_f401re_bus.h"
#include "nucleo_f401re_errno.h"

#elif (defined (USE_STM32L0XX_NUCLEO))
#include "nucleo_l073rz_bus.h"
#include "nucleo_l073rz_errno.h"

#elif (defined (USE_STM32L1XX_NUCLEO))
#include "nucleo_l152re_bus.h"
#include "nucleo_l152re_errno.h"

#elif (defined (USE_STM32L4XX_NUCLEO))
#include "nucleo_l476rg_bus.h"
#include "nucleo_l476rg_errno.h"

#else
#error Not supported platform
#endif

#if (defined (USE_IKS02A1))
#include "iks02a1_motion_sensors.h"
#include "iks02a1_motion_sensors_ex.h"
#endif

#include "sensor_def.h"
#include "serial_protocol.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint8_t hp_filter;
  uint8_t switch_HP_to_DC_null;
} gui_settings_t;

/* Exported constants --------------------------------------------------------*/
#define NUM_SENSORS  1

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern gui_settings_t GuiSettings;
extern float OdrMeasured;

/* Exported defines ----------------------------------------------------------*/
/* Enable sensor masks */
#define PRESSURE_SENSOR                         0x00000001U
#define TEMPERATURE_SENSOR                      0x00000002U
#define HUMIDITY_SENSOR                         0x00000004U
#define UV_SENSOR                               0x00000008U /* for future use */
#define ACCELEROMETER_SENSOR                    0x00000010U
#define GYROSCOPE_SENSOR                        0x00000020U
#define MAGNETIC_SENSOR                         0x00000040U

/* Exported functions --------------------------------------------------------*/
void Error_Handler(void);
void RTC_DateRegulate(uint8_t y, uint8_t m, uint8_t d, uint8_t dw);
void RTC_TimeRegulate(uint8_t hh, uint8_t mm, uint8_t ss);

uint8_t Init_Demo(void);
uint8_t Restart_FIFO(void);
uint8_t Get_HP_Filter(void);
uint8_t Set_HP_Filter(uint8_t value);
void Get_Msg(Msg_t *Msg, ACTIVE_AXIS_t AxisActive);
uint8_t *Get_Sensor_List(void);
uint8_t Set_ODR(uint8_t value);
uint8_t Set_Full_Scale(uint8_t value);
uint16_t Get_Samples(void);
uint8_t Set_Samples(uint8_t value);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */
