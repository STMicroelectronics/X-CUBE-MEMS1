/**
  ******************************************************************************
  * @file    iks02a1_env_sensors.h
  * @author  MEMS Software Solutions Team
  * @brief   This file provides a set of functions needed to manage the environmental sensors
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IKS02A1_ENV_SENSORS_H
#define IKS02A1_ENV_SENSORS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "iks02a1_conf.h"
#include "env_sensor.h"

#ifndef USE_IKS02A1_ENV_SENSOR_SHT40AD1B_0
#define USE_IKS02A1_ENV_SENSOR_SHT40AD1B_0       0
#endif

#if (USE_IKS02A1_ENV_SENSOR_SHT40AD1B_0 == 1)
#include "sht40ad1b.h"
#endif

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup IKS02A1 IKS02A1
  * @{
  */

/** @addtogroup IKS02A1_ENV_SENSORS IKS02A1 ENV SENSORS
  * @{
  */

/** @defgroup IKS02A1_ENV_SENSORS_Exported_Types IKS02A1 ENV SENSORS Exported Types
  * @{
  */

/* Environmental Sensor instance Info */
typedef struct
{
  uint8_t Temperature;
  uint8_t Pressure;
  uint8_t Humidity;
  uint8_t Gas;
  uint8_t LowPower;
  float   HumMaxOdr;
  float   TempMaxOdr;
  float   PressMaxOdr;
  float   GasMaxOdr;
} IKS02A1_ENV_SENSOR_Capabilities_t;

typedef struct
{
  uint32_t Functions;
} IKS02A1_ENV_SENSOR_Ctx_t;

/**
  * @}
  */

/** @defgroup IKS02A1_ENV_SENSOR_Exported_Constants IKS02A1 ENV SENSOR Exported Constants
  * @{
  */

#if (USE_IKS02A1_ENV_SENSOR_SHT40AD1B_0 == 1)
#define IKS02A1_SHT40AD1B_0 0
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
#ifndef ENV_GAS
#define ENV_GAS              8U
#endif

#define IKS02A1_ENV_FUNCTIONS_NBR    4U
#define IKS02A1_ENV_INSTANCES_NBR    (USE_IKS02A1_ENV_SENSOR_SHT40AD1B_0)

#if (IKS02A1_ENV_INSTANCES_NBR == 0)
#error "No environmental sensor instance has been selected"
#endif

/**
  * @}
  */

/** @addtogroup IKS02A1_ENV_SENSORS_Exported_Functions IKS02A1 ENV SENSOR Exported Functions
  * @{
  */

int32_t IKS02A1_ENV_SENSOR_Init(uint32_t Instance, uint32_t Functions);
int32_t IKS02A1_ENV_SENSOR_DeInit(uint32_t Instance);
int32_t IKS02A1_ENV_SENSOR_GetCapabilities(uint32_t Instance, IKS02A1_ENV_SENSOR_Capabilities_t *Capabilities);
int32_t IKS02A1_ENV_SENSOR_ReadID(uint32_t Instance, uint8_t *Id);
int32_t IKS02A1_ENV_SENSOR_Enable(uint32_t Instance, uint32_t Function);
int32_t IKS02A1_ENV_SENSOR_Disable(uint32_t Instance, uint32_t Function);
int32_t IKS02A1_ENV_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float *Odr);
int32_t IKS02A1_ENV_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float Odr);
int32_t IKS02A1_ENV_SENSOR_GetValue(uint32_t Instance, uint32_t Function, float *Value);

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

#endif /* IKS02A1_ENV_SENSORS_H */
