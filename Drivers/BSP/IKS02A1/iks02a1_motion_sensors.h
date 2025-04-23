/**
  ******************************************************************************
  * @file    iks02a1_motion_sensors.h
  * @author  MEMS Software Solutions Team
  * @brief   This file provides a set of functions needed to manage the motion sensors
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IKS02A1_MOTION_SENSOR_H
#define IKS02A1_MOTION_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "iks02a1_conf.h"
#include "motion_sensor.h"

#ifndef USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0
#define USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0       1
#endif

#ifndef USE_IKS02A1_MOTION_SENSOR_IIS2DLPC_0
#define USE_IKS02A1_MOTION_SENSOR_IIS2DLPC_0         1
#endif

#ifndef USE_IKS02A1_MOTION_SENSOR_IIS2MDC_0
#define USE_IKS02A1_MOTION_SENSOR_IIS2MDC_0          1
#endif

#ifndef USE_IKS02A1_MOTION_SENSOR_IIS2ICLX_0
#define USE_IKS02A1_MOTION_SENSOR_IIS2ICLX_0         0
#endif

#ifndef USE_IKS02A1_MOTION_SENSOR_ASM330LHHX_0
#define USE_IKS02A1_MOTION_SENSOR_ASM330LHHX_0       0
#endif

#ifndef USE_IKS02A1_MOTION_SENSOR_ISM330BX_0
#define USE_IKS02A1_MOTION_SENSOR_ISM330BX_0         0
#endif

#ifndef USE_IKS02A1_MOTION_SENSOR_IIS2DULPX_0
#define USE_IKS02A1_MOTION_SENSOR_IIS2DULPX_0        0
#endif

#if (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
#include "ism330dhcx.h"
#endif

#if (USE_IKS02A1_MOTION_SENSOR_IIS2DLPC_0 == 1)
#include "iis2dlpc.h"
#endif

#if (USE_IKS02A1_MOTION_SENSOR_IIS2MDC_0 == 1)
#include "iis2mdc.h"
#endif

#if (USE_IKS02A1_MOTION_SENSOR_IIS2ICLX_0 == 1)
#include "iis2iclx.h"
#endif

#if (USE_IKS02A1_MOTION_SENSOR_ASM330LHHX_0 == 1)
#include "asm330lhhx.h"
#endif

#if (USE_IKS02A1_MOTION_SENSOR_ISM330BX_0 == 1)
#include "ism330bx.h"
#endif

#if (USE_IKS02A1_MOTION_SENSOR_IIS2DULPX_0 == 1)
#include "iis2dulpx.h"
#endif

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup IKS02A1 IKS02A1
  * @{
  */

/** @addtogroup IKS02A1_MOTION_SENSOR IKS02A1 MOTION SENSOR
  * @{
  */

/** @defgroup IKS02A1_MOTION_SENSOR_Exported_Types IKS02A1 MOTION SENSOR Exported Types
  * @{
  */

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} IKS02A1_MOTION_SENSOR_Axes_t;

typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} IKS02A1_MOTION_SENSOR_AxesRaw_t;

/* Motion Sensor instance Info */
typedef struct
{
  uint8_t  Acc;
  uint8_t  Gyro;
  uint8_t  Magneto;
  uint8_t  LowPower;
  uint32_t GyroMaxFS;
  uint32_t AccMaxFS;
  uint32_t MagMaxFS;
  float    GyroMaxOdr;
  float    AccMaxOdr;
  float    MagMaxOdr;
} IKS02A1_MOTION_SENSOR_Capabilities_t;

typedef struct
{
  uint32_t Functions;
} IKS02A1_MOTION_SENSOR_Ctx_t;

/**
  * @}
  */

/** @defgroup IKS02A1_MOTION_SENSOR_Exported_Constants IKS02A1 MOTION SENSOR Exported Constants
  * @{
  */

#if (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
#define IKS02A1_ISM330DHCX_0 0
#endif

#if (USE_IKS02A1_MOTION_SENSOR_IIS2DLPC_0 == 1)
#define IKS02A1_IIS2DLPC_0 (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0)
#endif

#if (USE_IKS02A1_MOTION_SENSOR_IIS2MDC_0 == 1)
#define IKS02A1_IIS2MDC_0 (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 + \
                           USE_IKS02A1_MOTION_SENSOR_IIS2DLPC_0)
#endif

#if (USE_IKS02A1_MOTION_SENSOR_IIS2ICLX_0 == 1)
#define IKS02A1_IIS2ICLX_0  (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 + \
                             USE_IKS02A1_MOTION_SENSOR_IIS2DLPC_0 + \
                             USE_IKS02A1_MOTION_SENSOR_IIS2MDC_0)
#endif

#if (USE_IKS02A1_MOTION_SENSOR_ASM330LHHX_0 == 1)
#define IKS02A1_ASM330LHHX_0  (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 + \
                               USE_IKS02A1_MOTION_SENSOR_IIS2DLPC_0 + \
                               USE_IKS02A1_MOTION_SENSOR_IIS2MDC_0 + \
                               USE_IKS02A1_MOTION_SENSOR_IIS2ICLX_0)
#endif

#if (USE_IKS02A1_MOTION_SENSOR_ISM330BX_0 == 1)
#define IKS02A1_ISM330BX_0  (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 + \
                             USE_IKS02A1_MOTION_SENSOR_IIS2DLPC_0 + \
                             USE_IKS02A1_MOTION_SENSOR_IIS2MDC_0 + \
                             USE_IKS02A1_MOTION_SENSOR_IIS2ICLX_0 + \
                             USE_IKS02A1_MOTION_SENSOR_ASM330LHHX_0)
#endif

#if (USE_IKS02A1_MOTION_SENSOR_IIS2DULPX_0 == 1)
#define IKS02A1_IIS2DULPX_0  (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 + \
                              USE_IKS02A1_MOTION_SENSOR_IIS2DLPC_0 + \
                              USE_IKS02A1_MOTION_SENSOR_IIS2MDC_0 + \
                              USE_IKS02A1_MOTION_SENSOR_IIS2ICLX_0 + \
                              USE_IKS02A1_MOTION_SENSOR_ASM330LHHX_0 + \
                              USE_IKS02A1_MOTION_SENSOR_ISM330BX_0)
#endif

#ifndef MOTION_GYRO
#define MOTION_GYRO             1U
#endif
#ifndef MOTION_ACCELERO
#define MOTION_ACCELERO         2U
#endif
#ifndef MOTION_MAGNETO
#define MOTION_MAGNETO          4U
#endif

#define IKS02A1_MOTION_FUNCTIONS_NBR    3U
#define IKS02A1_MOTION_INSTANCES_NBR    (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 + \
                                         USE_IKS02A1_MOTION_SENSOR_IIS2DLPC_0 + \
                                         USE_IKS02A1_MOTION_SENSOR_IIS2MDC_0 + \
                                         USE_IKS02A1_MOTION_SENSOR_IIS2ICLX_0 + \
                                         USE_IKS02A1_MOTION_SENSOR_ASM330LHHX_0 + \
                                         USE_IKS02A1_MOTION_SENSOR_ISM330BX_0 + \
                                         USE_IKS02A1_MOTION_SENSOR_IIS2DULPX_0)

#if (IKS02A1_MOTION_INSTANCES_NBR == 0)
#error "No motion sensor instance has been selected"
#endif

/**
  * @}
  */

/** @addtogroup IKS02A1_MOTION_SENSOR_Exported_Functions IKS02A1_MOTION_SENSOR Exported Functions
  * @{
  */

int32_t IKS02A1_MOTION_SENSOR_Init(uint32_t Instance, uint32_t Functions);
int32_t IKS02A1_MOTION_SENSOR_DeInit(uint32_t Instance);
int32_t IKS02A1_MOTION_SENSOR_GetCapabilities(uint32_t Instance, IKS02A1_MOTION_SENSOR_Capabilities_t *Capabilities);
int32_t IKS02A1_MOTION_SENSOR_ReadID(uint32_t Instance, uint8_t *Id);
int32_t IKS02A1_MOTION_SENSOR_Enable(uint32_t Instance, uint32_t Function);
int32_t IKS02A1_MOTION_SENSOR_Disable(uint32_t Instance, uint32_t Function);
int32_t IKS02A1_MOTION_SENSOR_GetAxes(uint32_t Instance, uint32_t Function, IKS02A1_MOTION_SENSOR_Axes_t *Axes);
int32_t IKS02A1_MOTION_SENSOR_GetAxesRaw(uint32_t Instance, uint32_t Function, IKS02A1_MOTION_SENSOR_AxesRaw_t *Axes);
int32_t IKS02A1_MOTION_SENSOR_GetSensitivity(uint32_t Instance, uint32_t Function, float *Sensitivity);
int32_t IKS02A1_MOTION_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float *Odr);
int32_t IKS02A1_MOTION_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float Odr);
int32_t IKS02A1_MOTION_SENSOR_GetFullScale(uint32_t Instance, uint32_t Function, int32_t *Fullscale);
int32_t IKS02A1_MOTION_SENSOR_SetFullScale(uint32_t Instance, uint32_t Function, int32_t Fullscale);

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

#endif /* IKS02A1_MOTION_SENSOR_H */
