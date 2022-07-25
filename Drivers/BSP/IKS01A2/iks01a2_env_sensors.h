/**
  ******************************************************************************
  * @file    iks01a2_env_sensors.h
  * @author  MEMS Software Solutions Team
  * @brief   This file provides a set of functions needed to manage the environmental sensors
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016-2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IKS01A2_ENV_SENSORS_H
#define IKS01A2_ENV_SENSORS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "iks01a2_conf.h"
#include "env_sensor.h"

#ifndef USE_IKS01A2_ENV_SENSOR_HTS221_0
#define USE_IKS01A2_ENV_SENSOR_HTS221_0          1
#endif

#ifndef USE_IKS01A2_ENV_SENSOR_LPS22HB_0
#define USE_IKS01A2_ENV_SENSOR_LPS22HB_0         1
#endif

#ifndef USE_IKS01A2_ENV_SENSOR_LPS33HW_0
#define USE_IKS01A2_ENV_SENSOR_LPS33HW_0         0
#endif

#ifndef USE_IKS01A2_ENV_SENSOR_LPS22HH_0
#define USE_IKS01A2_ENV_SENSOR_LPS22HH_0         0
#endif

#ifndef USE_IKS01A2_ENV_SENSOR_STTS22H_0
#define USE_IKS01A2_ENV_SENSOR_STTS22H_0         0
#endif

#if (USE_IKS01A2_ENV_SENSOR_LPS22HB_0 == 1)
#include "lps22hb.h"
#endif

#if (USE_IKS01A2_ENV_SENSOR_HTS221_0 == 1)
#include "hts221.h"
#endif

#if (USE_IKS01A2_ENV_SENSOR_LPS33HW_0 == 1)
#include "lps33hw.h"
#endif

#if (USE_IKS01A2_ENV_SENSOR_LPS22HH_0 == 1)
#include "lps22hh.h"
#endif

#if (USE_IKS01A2_ENV_SENSOR_STTS22H_0 == 1)
#include "stts22h.h"
#endif

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup IKS01A2 IKS01A2
  * @{
  */

/** @addtogroup IKS01A2_ENV_SENSORS IKS01A2 ENV SENSORS
  * @{
  */

/** @defgroup IKS01A2_ENV_SENSORS_Exported_Types IKS01A2 ENV SENSORS Exported Types
  * @{
  */

/* Environmental Sensor instance Info */
typedef struct
{
  uint8_t Temperature;
  uint8_t Pressure;
  uint8_t Humidity;
  uint8_t LowPower;
  float   HumMaxOdr;
  float   TempMaxOdr;
  float   PressMaxOdr;
} IKS01A2_ENV_SENSOR_Capabilities_t;

typedef struct
{
  uint32_t Functions;
} IKS01A2_ENV_SENSOR_Ctx_t;

/**
  * @}
  */

/** @defgroup IKS01A2_ENV_SENSOR_Exported_Constants IKS01A2 ENV SENSOR Exported Constants
  * @{
  */

#if (USE_IKS01A2_ENV_SENSOR_HTS221_0 == 1)
#define IKS01A2_HTS221_0 0
#endif

#if (USE_IKS01A2_ENV_SENSOR_LPS22HB_0 == 1)
#define IKS01A2_LPS22HB_0 (USE_IKS01A2_ENV_SENSOR_HTS221_0)
#endif

#if (USE_IKS01A2_ENV_SENSOR_LPS33HW_0 == 1)
#define IKS01A2_LPS33HW_0 (USE_IKS01A2_ENV_SENSOR_HTS221_0 + \
                           USE_IKS01A2_ENV_SENSOR_LPS22HB_0)
#endif

#if (USE_IKS01A2_ENV_SENSOR_LPS22HH_0 == 1)
#define IKS01A2_LPS22HH_0 (USE_IKS01A2_ENV_SENSOR_HTS221_0 + \
                           USE_IKS01A2_ENV_SENSOR_LPS22HB_0 + \
                           USE_IKS01A2_ENV_SENSOR_LPS33HW_0)
#endif

#if (USE_IKS01A2_ENV_SENSOR_STTS22H_0 == 1)
#define IKS01A2_STTS22H_0 (USE_IKS01A2_ENV_SENSOR_HTS221_0 + \
                           USE_IKS01A2_ENV_SENSOR_LPS22HB_0 + \
                           USE_IKS01A2_ENV_SENSOR_LPS33HW_0 + \
                           USE_IKS01A2_ENV_SENSOR_LPS22HH_0)
#endif

#ifndef ENV_TEMPERATURE
#define ENV_TEMPERATURE      1U
#endif
#ifndef ENV_PRESSURE
#define ENV_PRESSURE         2U
#endif
#ifndef ENV_HUMIDITY
#define ENV_HUMIDITY         4U
#endif

#define IKS01A2_ENV_FUNCTIONS_NBR    3U
#define IKS01A2_ENV_INSTANCES_NBR    (USE_IKS01A2_ENV_SENSOR_HTS221_0 + \
                                      USE_IKS01A2_ENV_SENSOR_LPS22HB_0 + \
                                      USE_IKS01A2_ENV_SENSOR_LPS33HW_0 + \
                                      USE_IKS01A2_ENV_SENSOR_LPS22HH_0 + \
                                      USE_IKS01A2_ENV_SENSOR_STTS22H_0)

#if (IKS01A2_ENV_INSTANCES_NBR == 0)
#error "No environmental sensor instance has been selected"
#endif

/**
  * @}
  */

/** @addtogroup IKS01A2_ENV_SENSORS_Exported_Functions IKS01A2 ENV SENSOR Exported Functions
  * @{
  */

int32_t IKS01A2_ENV_SENSOR_Init(uint32_t Instance, uint32_t Functions);
int32_t IKS01A2_ENV_SENSOR_DeInit(uint32_t Instance);
int32_t IKS01A2_ENV_SENSOR_GetCapabilities(uint32_t Instance, IKS01A2_ENV_SENSOR_Capabilities_t *Capabilities);
int32_t IKS01A2_ENV_SENSOR_ReadID(uint32_t Instance, uint8_t *Id);
int32_t IKS01A2_ENV_SENSOR_Enable(uint32_t Instance, uint32_t Function);
int32_t IKS01A2_ENV_SENSOR_Disable(uint32_t Instance, uint32_t Function);
int32_t IKS01A2_ENV_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float *Odr);
int32_t IKS01A2_ENV_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float Odr);
int32_t IKS01A2_ENV_SENSOR_GetValue(uint32_t Instance, uint32_t Function, float *Value);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* IKS01A2_ENV_SENSORS_H */
