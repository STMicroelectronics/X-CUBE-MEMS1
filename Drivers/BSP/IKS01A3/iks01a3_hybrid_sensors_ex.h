/**
  ******************************************************************************
  * @file    iks01a3_hybrid_sensors_ex.h
  * @author  MEMS Software Solutions Team
  * @brief   This file provides a set of extended functions needed to manage the
  *          hybrid sensors
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
#ifndef IKS01A3_HYBRID_SENSOR_EX_H
#define IKS01A3_HYBRID_SENSOR_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "iks01a3_hybrid_sensors.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup IKS01A3 IKS01A3
  * @{
  */

/** @addtogroup IKS01A3_HYBRID_SENSOR_EX IKS01A3 HYBRID SENSOR EX
  * @{
  */

/** @defgroup IKS01A3_HYBRID_SENSOR_EX_Exported_Types IKS01A3 HYBRID SENSOR EX Exported Types
  * @{
  */

typedef enum
{
  IKS01A3_HYBRID_SENSOR_INT1_PIN = 0,
  IKS01A3_HYBRID_SENSOR_INT2_PIN
} IKS01A3_HYBRID_SENSOR_IntPin_t;

typedef struct
{
  unsigned int FreeFallStatus : 1;
  unsigned int TapStatus : 1;
  unsigned int DoubleTapStatus : 1;
  unsigned int WakeUpStatus : 1;
  unsigned int StepStatus : 1;
  unsigned int TiltStatus : 1;
  unsigned int D6DOrientationStatus : 1;
  unsigned int SleepStatus : 1;
} IKS01A3_HYBRID_SENSOR_Event_Status_t;

/**
  * @}
  */

/** @addtogroup IKS01A3_HYBRID_SENSOR_EX_Exported_Functions IKS01A3 HYBRID SENSOR EX Exported Functions
  * @{
  */

int32_t IKS01A3_HYBRID_SENSOR_Get_DRDY_Status(uint32_t Instance, uint32_t Function, uint8_t *Status);
int32_t IKS01A3_HYBRID_SENSOR_Read_Register(uint32_t Instance, uint8_t Reg, uint8_t *Data);
int32_t IKS01A3_HYBRID_SENSOR_Write_Register(uint32_t Instance, uint8_t Reg, uint8_t Data);

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

#endif /* IKS01A3_HYBRID_SENSOR_EX_H */
