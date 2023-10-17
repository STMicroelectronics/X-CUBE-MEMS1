/**
  ******************************************************************************
  * @file    iks4a1_hybrid_sensors.h
  * @author  MEMS Software Solutions Team
  * @brief   This file provides a set of functions needed to manage the hybrid sensors
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
#ifndef IKS4A1_HYBRID_SENSORS_H
#define IKS4A1_HYBRID_SENSORS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "iks4a1_conf.h"
#include "hybrid_sensor.h"

#ifndef USE_IKS4A1_HYBRID_SENSOR_LIS2DTW12_0
#define USE_IKS4A1_HYBRID_SENSOR_LIS2DTW12_0  0
#endif

#if (USE_IKS4A1_HYBRID_SENSOR_LIS2DTW12_0 == 1)
#include "lis2dtw12.h"
#endif

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup IKS4A1 IKS4A1
  * @{
  */

/** @addtogroup IKS4A1_HYBRID_SENSORS IKS4A1 HYBRID SENSORS
  * @{
  */

/** @defgroup IKS4A1_HYBRID_SENSORS_Exported_Types IKS4A1 HYBRID SENSORS Exported Types
  * @{
  */

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} IKS4A1_HYBRID_MOTION_SENSOR_Axes_t;

typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} IKS4A1_HYBRID_MOTION_SENSOR_AxesRaw_t;

/* Hybrid Sensor instance Info */
typedef struct
{
  uint8_t  Acc;
  uint8_t  Gyro;
  uint8_t  Magneto;
  uint8_t  Temperature;
  uint8_t  Pressure;
  uint8_t  Humidity;
  uint8_t  Gas;
  uint8_t  LowPower;
  uint32_t GyroMaxFS;
  uint32_t AccMaxFS;
  uint32_t MagMaxFS;
  float_t  GyroMaxOdr;
  float_t  AccMaxOdr;
  float_t  MagMaxOdr;
  float_t  HumMaxOdr;
  float_t  TempMaxOdr;
  float_t  PressMaxOdr;
  float    GasMaxOdr;
} IKS4A1_HYBRID_SENSOR_Capabilities_t;

typedef struct
{
  uint32_t Functions;
} IKS4A1_HYBRID_MOTION_SENSOR_Ctx_t;

typedef struct
{
  uint32_t Functions;
} IKS4A1_HYBRID_ENV_SENSOR_Ctx_t;

/**
  * @}
  */

/** @defgroup IKS4A1_HYBRID_SENSOR_Exported_Constants IKS4A1 HYBRID SENSOR Exported Constants
  * @{
  */

#if (USE_IKS4A1_HYBRID_SENSOR_LIS2DTW12_0 == 1)
#define IKS4A1_LIS2DTW12_0  0
#endif

#define IKS4A1_HYBRID_INSTANCES_NBR  (USE_IKS4A1_HYBRID_SENSOR_LIS2DTW12_0)

#if (IKS4A1_HYBRID_INSTANCES_NBR == 0)
#error "No hybrid sensor instance has been selected"
#endif

#ifndef HYBRID_GYRO
#define HYBRID_GYRO         (uint32_t)0x01U
#endif
#ifndef HYBRID_ACCELERO
#define HYBRID_ACCELERO     (uint32_t)0x02U
#endif
#ifndef HYBRID_MAGNETO
#define HYBRID_MAGNETO      (uint32_t)0x04U
#endif
#ifndef HYBRID_MOTION_FUNC
#define HYBRID_MOTION_FUNC  (HYBRID_GYRO | HYBRID_ACCELERO | HYBRID_MAGNETO)
#endif

#define HYBRID_MOTION_FUNC_ID(f) \
  (((f) & HYBRID_GYRO) == (f))            ? 0 \
  :    (((f) & HYBRID_ACCELERO) == (f))   ? 1 \
  : /* (((f) & HYBRID_MAGNETO) == (f)) */   2

#define IKS4A1_HYBRID_MOTION_FUNCTIONS_NBR  3U

#ifndef HYBRID_TEMPERATURE
#define HYBRID_TEMPERATURE  (uint32_t)0x10U
#endif
#ifndef HYBRID_PRESSURE
#define HYBRID_PRESSURE     (uint32_t)0x20U
#endif
#ifndef HYBRID_HUMIDITY
#define HYBRID_HUMIDITY     (uint32_t)0x40U
#endif
#ifndef HYBRID_GAS
#define HYBRID_GAS          (uint32_t)0x80U
#endif
#ifndef HYBRID_ENV_FUNC
#define HYBRID_ENV_FUNC     (HYBRID_TEMPERATURE | HYBRID_PRESSURE | HYBRID_HUMIDITY | HYBRID_GAS)
#endif

#define HYBRID_ENV_FUNC_ID(f) \
  (((f) & HYBRID_TEMPERATURE) == (f))      ? 0 \
  :    (((f) & HYBRID_PRESSURE) == (f))    ? 1 \
  :    (((f) & HYBRID_HUMIDITY) == (f))    ? 2 \
  : /* (((f) & HYBRID_GAS) == (f)) */        3

#define IKS4A1_HYBRID_ENV_FUNCTIONS_NBR  4U

/**
  * @}
  */

/** @addtogroup IKS4A1_HYBRID_SENSORS_Exported_Functions IKS4A1 HYBRID SENSOR Exported Functions
  * @{
  */

int32_t IKS4A1_HYBRID_SENSOR_Init(uint32_t Instance, uint32_t Functions);
int32_t IKS4A1_HYBRID_SENSOR_DeInit(uint32_t Instance);
int32_t IKS4A1_HYBRID_SENSOR_GetCapabilities(uint32_t Instance, IKS4A1_HYBRID_SENSOR_Capabilities_t *Capabilities);
int32_t IKS4A1_HYBRID_SENSOR_ReadID(uint32_t Instance, uint8_t *Id);
int32_t IKS4A1_HYBRID_MOTION_SENSOR_Enable(uint32_t Instance, uint32_t Function);
int32_t IKS4A1_HYBRID_MOTION_SENSOR_Disable(uint32_t Instance, uint32_t Function);
int32_t IKS4A1_HYBRID_MOTION_SENSOR_GetAxes(uint32_t Instance, uint32_t Function,
                                            IKS4A1_HYBRID_MOTION_SENSOR_Axes_t *Axes);
int32_t IKS4A1_HYBRID_MOTION_SENSOR_GetAxesRaw(uint32_t Instance, uint32_t Function,
                                               IKS4A1_HYBRID_MOTION_SENSOR_AxesRaw_t *Axes);
int32_t IKS4A1_HYBRID_MOTION_SENSOR_GetSensitivity(uint32_t Instance, uint32_t Function, float_t *Sensitivity);
int32_t IKS4A1_HYBRID_MOTION_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float_t *Odr);
int32_t IKS4A1_HYBRID_MOTION_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float_t Odr);
int32_t IKS4A1_HYBRID_MOTION_SENSOR_GetFullScale(uint32_t Instance, uint32_t Function, int32_t *Fullscale);
int32_t IKS4A1_HYBRID_MOTION_SENSOR_SetFullScale(uint32_t Instance, uint32_t Function, int32_t Fullscale);
int32_t IKS4A1_HYBRID_ENV_SENSOR_Enable(uint32_t Instance, uint32_t Function);
int32_t IKS4A1_HYBRID_ENV_SENSOR_Disable(uint32_t Instance, uint32_t Function);
int32_t IKS4A1_HYBRID_ENV_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float_t *Odr);
int32_t IKS4A1_HYBRID_ENV_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float_t Odr);
int32_t IKS4A1_HYBRID_ENV_SENSOR_GetValue(uint32_t Instance, uint32_t Function, float_t *Value);

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

#endif /* IKS4A1_HYBRID_SENSORS_H */
