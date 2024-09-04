/**
  ******************************************************************************
  * @file    iks4a1_env_sensors.h
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
#ifndef IKS4A1_ENV_SENSORS_H
#define IKS4A1_ENV_SENSORS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "iks4a1_conf.h"
#include "env_sensor.h"

#ifndef USE_IKS4A1_ENV_SENSOR_LPS22DF_0
#define USE_IKS4A1_ENV_SENSOR_LPS22DF_0         1U
#endif

#ifndef USE_IKS4A1_ENV_SENSOR_SHT40AD1B_0
#define USE_IKS4A1_ENV_SENSOR_SHT40AD1B_0       1U
#endif

#ifndef USE_IKS4A1_ENV_SENSOR_STTS22H_0
#define USE_IKS4A1_ENV_SENSOR_STTS22H_0         1U
#endif

#ifndef USE_IKS4A1_ENV_SENSOR_HTS221_0
#define USE_IKS4A1_ENV_SENSOR_HTS221_0          0U
#endif

#ifndef USE_IKS4A1_ENV_SENSOR_LPS22HH_0
#define USE_IKS4A1_ENV_SENSOR_LPS22HH_0         0U
#endif

#ifndef USE_IKS4A1_ENV_SENSOR_STTS751_0
#define USE_IKS4A1_ENV_SENSOR_STTS751_0         0U
#endif

#ifndef USE_IKS4A1_ENV_SENSOR_LPS33HW_0
#define USE_IKS4A1_ENV_SENSOR_LPS33HW_0         0U
#endif

#ifndef USE_IKS4A1_ENV_SENSOR_LPS33K_0
#define USE_IKS4A1_ENV_SENSOR_LPS33K_0          0U
#endif

#ifndef USE_IKS4A1_ENV_SENSOR_LPS22CH_0
#define USE_IKS4A1_ENV_SENSOR_LPS22CH_0         0U
#endif

#ifndef USE_IKS4A1_ENV_SENSOR_LPS27HHTW_0
#define USE_IKS4A1_ENV_SENSOR_LPS27HHTW_0       0U
#endif

#ifndef USE_IKS4A1_ENV_SENSOR_ILPS22QS_0
#define USE_IKS4A1_ENV_SENSOR_ILPS22QS_0        0U
#endif

#ifndef USE_IKS4A1_ENV_SENSOR_ILPS28QSW_0
#define USE_IKS4A1_ENV_SENSOR_ILPS28QSW_0       0U
#endif

#ifndef USE_IKS4A1_ENV_SENSOR_LPS28DFW_0
#define USE_IKS4A1_ENV_SENSOR_LPS28DFW_0        0U
#endif

#ifndef USE_IKS4A1_ENV_SENSOR_STHS34PF80_0
#define USE_IKS4A1_ENV_SENSOR_STHS34PF80_0      0U
#endif

#if (USE_IKS4A1_ENV_SENSOR_HTS221_0 == 1)
#include "hts221.h"
#endif

#if (USE_IKS4A1_ENV_SENSOR_LPS22HH_0 == 1)
#include "lps22hh.h"
#endif

#if (USE_IKS4A1_ENV_SENSOR_STTS751_0 == 1)
#include "stts751.h"
#endif

#if (USE_IKS4A1_ENV_SENSOR_LPS33HW_0 == 1)
#include "lps33hw.h"
#endif

#if (USE_IKS4A1_ENV_SENSOR_STTS22H_0 == 1)
#include "stts22h.h"
#endif

#if (USE_IKS4A1_ENV_SENSOR_LPS33K_0 == 1)
#include "lps33k.h"
#endif

#if (USE_IKS4A1_ENV_SENSOR_LPS22CH_0 == 1)
#include "lps22ch.h"
#endif

#if (USE_IKS4A1_ENV_SENSOR_LPS27HHTW_0 == 1)
#include "lps27hhtw.h"
#endif

#if (USE_IKS4A1_ENV_SENSOR_LPS22DF_0 == 1)
#include "lps22df.h"
#endif

#if (USE_IKS4A1_ENV_SENSOR_ILPS22QS_0 == 1)
#include "ilps22qs.h"
#endif

#if (USE_IKS4A1_ENV_SENSOR_ILPS28QSW_0 == 1)
#include "ilps28qsw.h"
#endif

#if (USE_IKS4A1_ENV_SENSOR_LPS28DFW_0 == 1)
#include "lps28dfw.h"
#endif

#if (USE_IKS4A1_ENV_SENSOR_SHT40AD1B_0 == 1)
#include "sht40ad1b.h"
#endif

#if (USE_IKS4A1_ENV_SENSOR_STHS34PF80_0 == 1)
#include "sths34pf80.h"
#endif

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup IKS4A1 IKS4A1
  * @{
  */

/** @addtogroup IKS4A1_ENV_SENSORS IKS4A1 ENV SENSORS
  * @{
  */

/** @defgroup IKS4A1_ENV_SENSORS_Exported_Types IKS4A1 ENV SENSORS Exported Types
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
} IKS4A1_ENV_SENSOR_Capabilities_t;

typedef struct
{
  uint32_t Functions;
} IKS4A1_ENV_SENSOR_Ctx_t;

/**
  * @}
  */

/** @defgroup IKS4A1_ENV_SENSOR_Exported_Constants IKS4A1 ENV SENSOR Exported Constants
  * @{
  */

#if (USE_IKS4A1_ENV_SENSOR_HTS221_0 == 1)
#define IKS4A1_HTS221_0 0
#endif

#if (USE_IKS4A1_ENV_SENSOR_LPS22HH_0 == 1)
#define IKS4A1_LPS22HH_0 (USE_IKS4A1_ENV_SENSOR_HTS221_0)
#endif

#if (USE_IKS4A1_ENV_SENSOR_STTS751_0 == 1)
#define IKS4A1_STTS751_0 (USE_IKS4A1_ENV_SENSOR_HTS221_0 + \
                          USE_IKS4A1_ENV_SENSOR_LPS22HH_0)
#endif

#if (USE_IKS4A1_ENV_SENSOR_LPS33HW_0 == 1)
#define IKS4A1_LPS33HW_0 (USE_IKS4A1_ENV_SENSOR_HTS221_0 + \
                          USE_IKS4A1_ENV_SENSOR_LPS22HH_0 + \
                          USE_IKS4A1_ENV_SENSOR_STTS751_0)
#endif

#if (USE_IKS4A1_ENV_SENSOR_STTS22H_0 == 1)
#define IKS4A1_STTS22H_0 (USE_IKS4A1_ENV_SENSOR_HTS221_0 + \
                          USE_IKS4A1_ENV_SENSOR_LPS22HH_0 + \
                          USE_IKS4A1_ENV_SENSOR_STTS751_0 + \
                          USE_IKS4A1_ENV_SENSOR_LPS33HW_0)
#endif

#if (USE_IKS4A1_ENV_SENSOR_LPS33K_0 == 1)
#define IKS4A1_LPS33K_0 (USE_IKS4A1_ENV_SENSOR_HTS221_0 + \
                         USE_IKS4A1_ENV_SENSOR_LPS22HH_0 + \
                         USE_IKS4A1_ENV_SENSOR_STTS751_0 + \
                         USE_IKS4A1_ENV_SENSOR_LPS33HW_0 + \
                         USE_IKS4A1_ENV_SENSOR_STTS22H_0)
#endif

#if (USE_IKS4A1_ENV_SENSOR_LPS22CH_0 == 1)
#define IKS4A1_LPS22CH_0 (USE_IKS4A1_ENV_SENSOR_HTS221_0 + \
                          USE_IKS4A1_ENV_SENSOR_LPS22HH_0 + \
                          USE_IKS4A1_ENV_SENSOR_STTS751_0 + \
                          USE_IKS4A1_ENV_SENSOR_LPS33HW_0 + \
                          USE_IKS4A1_ENV_SENSOR_STTS22H_0 + \
                          USE_IKS4A1_ENV_SENSOR_LPS33K_0)
#endif

#if (USE_IKS4A1_ENV_SENSOR_LPS27HHTW_0 == 1)
#define IKS4A1_LPS27HHTW_0 (USE_IKS4A1_ENV_SENSOR_HTS221_0 + \
                            USE_IKS4A1_ENV_SENSOR_LPS22HH_0 + \
                            USE_IKS4A1_ENV_SENSOR_STTS751_0 + \
                            USE_IKS4A1_ENV_SENSOR_LPS33HW_0 + \
                            USE_IKS4A1_ENV_SENSOR_STTS22H_0 + \
                            USE_IKS4A1_ENV_SENSOR_LPS33K_0 + \
                            USE_IKS4A1_ENV_SENSOR_LPS22CH_0)
#endif

#if (USE_IKS4A1_ENV_SENSOR_LPS22DF_0 == 1)
#define IKS4A1_LPS22DF_0 (USE_IKS4A1_ENV_SENSOR_HTS221_0 + \
                          USE_IKS4A1_ENV_SENSOR_LPS22HH_0 + \
                          USE_IKS4A1_ENV_SENSOR_STTS751_0 + \
                          USE_IKS4A1_ENV_SENSOR_LPS33HW_0 + \
                          USE_IKS4A1_ENV_SENSOR_STTS22H_0 + \
                          USE_IKS4A1_ENV_SENSOR_LPS33K_0 + \
                          USE_IKS4A1_ENV_SENSOR_LPS22CH_0 + \
                          USE_IKS4A1_ENV_SENSOR_LPS27HHTW_0)
#endif

#if (USE_IKS4A1_ENV_SENSOR_ILPS22QS_0 == 1)
#define IKS4A1_ILPS22QS_0 (USE_IKS4A1_ENV_SENSOR_HTS221_0 + \
                           USE_IKS4A1_ENV_SENSOR_LPS22HH_0 + \
                           USE_IKS4A1_ENV_SENSOR_STTS751_0 + \
                           USE_IKS4A1_ENV_SENSOR_LPS33HW_0 + \
                           USE_IKS4A1_ENV_SENSOR_STTS22H_0 + \
                           USE_IKS4A1_ENV_SENSOR_LPS33K_0 + \
                           USE_IKS4A1_ENV_SENSOR_LPS22CH_0 + \
                           USE_IKS4A1_ENV_SENSOR_LPS27HHTW_0 + \
                           USE_IKS4A1_ENV_SENSOR_LPS22DF_0)
#endif

#if (USE_IKS4A1_ENV_SENSOR_ILPS28QSW_0 == 1)
#define IKS4A1_ILPS28QSW_0 (USE_IKS4A1_ENV_SENSOR_HTS221_0 + \
                            USE_IKS4A1_ENV_SENSOR_LPS22HH_0 + \
                            USE_IKS4A1_ENV_SENSOR_STTS751_0 + \
                            USE_IKS4A1_ENV_SENSOR_LPS33HW_0 + \
                            USE_IKS4A1_ENV_SENSOR_STTS22H_0 + \
                            USE_IKS4A1_ENV_SENSOR_LPS33K_0 + \
                            USE_IKS4A1_ENV_SENSOR_LPS22CH_0 + \
                            USE_IKS4A1_ENV_SENSOR_LPS27HHTW_0 + \
                            USE_IKS4A1_ENV_SENSOR_LPS22DF_0 + \
                            USE_IKS4A1_ENV_SENSOR_ILPS22QS_0)
#endif

#if (USE_IKS4A1_ENV_SENSOR_LPS28DFW_0 == 1)
#define IKS4A1_LPS28DFW_0 (USE_IKS4A1_ENV_SENSOR_HTS221_0 + \
                           USE_IKS4A1_ENV_SENSOR_LPS22HH_0 + \
                           USE_IKS4A1_ENV_SENSOR_STTS751_0 + \
                           USE_IKS4A1_ENV_SENSOR_LPS33HW_0 + \
                           USE_IKS4A1_ENV_SENSOR_STTS22H_0 + \
                           USE_IKS4A1_ENV_SENSOR_LPS33K_0 + \
                           USE_IKS4A1_ENV_SENSOR_LPS22CH_0 + \
                           USE_IKS4A1_ENV_SENSOR_LPS27HHTW_0 + \
                           USE_IKS4A1_ENV_SENSOR_LPS22DF_0 + \
                           USE_IKS4A1_ENV_SENSOR_ILPS22QS_0 + \
                           USE_IKS4A1_ENV_SENSOR_ILPS28QSW_0)
#endif

#if (USE_IKS4A1_ENV_SENSOR_SHT40AD1B_0 == 1)
#define IKS4A1_SHT40AD1B_0 (USE_IKS4A1_ENV_SENSOR_HTS221_0 + \
                            USE_IKS4A1_ENV_SENSOR_LPS22HH_0 + \
                            USE_IKS4A1_ENV_SENSOR_STTS751_0 + \
                            USE_IKS4A1_ENV_SENSOR_LPS33HW_0 + \
                            USE_IKS4A1_ENV_SENSOR_STTS22H_0 + \
                            USE_IKS4A1_ENV_SENSOR_LPS33K_0 + \
                            USE_IKS4A1_ENV_SENSOR_LPS22CH_0 + \
                            USE_IKS4A1_ENV_SENSOR_LPS27HHTW_0 + \
                            USE_IKS4A1_ENV_SENSOR_LPS22DF_0 + \
                            USE_IKS4A1_ENV_SENSOR_ILPS22QS_0 + \
                            USE_IKS4A1_ENV_SENSOR_ILPS28QSW_0 + \
                            USE_IKS4A1_ENV_SENSOR_LPS28DFW_0)
#endif

#if (USE_IKS4A1_ENV_SENSOR_STHS34PF80_0 == 1)
#define IKS4A1_STHS34PF80_0 (USE_IKS4A1_ENV_SENSOR_HTS221_0 + \
                             USE_IKS4A1_ENV_SENSOR_LPS22HH_0 + \
                             USE_IKS4A1_ENV_SENSOR_STTS751_0 + \
                             USE_IKS4A1_ENV_SENSOR_LPS33HW_0 + \
                             USE_IKS4A1_ENV_SENSOR_STTS22H_0 + \
                             USE_IKS4A1_ENV_SENSOR_LPS33K_0 + \
                             USE_IKS4A1_ENV_SENSOR_LPS22CH_0 + \
                             USE_IKS4A1_ENV_SENSOR_LPS27HHTW_0 + \
                             USE_IKS4A1_ENV_SENSOR_LPS22DF_0 + \
                             USE_IKS4A1_ENV_SENSOR_ILPS22QS_0 + \
                             USE_IKS4A1_ENV_SENSOR_ILPS28QSW_0 + \
                             USE_IKS4A1_ENV_SENSOR_LPS28DFW_0 + \
                             USE_IKS4A1_ENV_SENSOR_SHT40AD1B_0)
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

#define IKS4A1_ENV_FUNCTIONS_NBR    4U
#define IKS4A1_ENV_INSTANCES_NBR    (USE_IKS4A1_ENV_SENSOR_HTS221_0 + \
                                     USE_IKS4A1_ENV_SENSOR_LPS22HH_0 + \
                                     USE_IKS4A1_ENV_SENSOR_STTS751_0 + \
                                     USE_IKS4A1_ENV_SENSOR_LPS33HW_0 + \
                                     USE_IKS4A1_ENV_SENSOR_STTS22H_0 + \
                                     USE_IKS4A1_ENV_SENSOR_LPS33K_0 + \
                                     USE_IKS4A1_ENV_SENSOR_LPS22CH_0 + \
                                     USE_IKS4A1_ENV_SENSOR_LPS27HHTW_0 + \
                                     USE_IKS4A1_ENV_SENSOR_LPS22DF_0 + \
                                     USE_IKS4A1_ENV_SENSOR_ILPS22QS_0 + \
                                     USE_IKS4A1_ENV_SENSOR_ILPS28QSW_0 + \
                                     USE_IKS4A1_ENV_SENSOR_LPS28DFW_0 + \
                                     USE_IKS4A1_ENV_SENSOR_SHT40AD1B_0 + \
                                     USE_IKS4A1_ENV_SENSOR_STHS34PF80_0)

#if (IKS4A1_ENV_INSTANCES_NBR == 0)
#error "No environmental sensor instance has been selected"
#endif

/**
  * @}
  */

/** @addtogroup IKS4A1_ENV_SENSORS_Exported_Functions IKS4A1 ENV SENSOR Exported Functions
  * @{
  */

int32_t IKS4A1_ENV_SENSOR_Init(uint32_t Instance, uint32_t Functions);
int32_t IKS4A1_ENV_SENSOR_DeInit(uint32_t Instance);
int32_t IKS4A1_ENV_SENSOR_GetCapabilities(uint32_t Instance, IKS4A1_ENV_SENSOR_Capabilities_t *Capabilities);
int32_t IKS4A1_ENV_SENSOR_ReadID(uint32_t Instance, uint8_t *Id);
int32_t IKS4A1_ENV_SENSOR_Enable(uint32_t Instance, uint32_t Function);
int32_t IKS4A1_ENV_SENSOR_Disable(uint32_t Instance, uint32_t Function);
int32_t IKS4A1_ENV_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float_t *Odr);
int32_t IKS4A1_ENV_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float_t Odr);
int32_t IKS4A1_ENV_SENSOR_GetValue(uint32_t Instance, uint32_t Function, float_t *Value);

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

#endif /* IKS4A1_ENV_SENSORS_H */
