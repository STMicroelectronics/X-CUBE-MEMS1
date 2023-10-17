/**
  ******************************************************************************
  * @file    iks4a1_hybrid_sensors_ex.h
  * @author  MEMS Software Solutions Team
  * @brief   This file provides a set of extended functions needed to manage the
  *          hybrid sensors
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
#ifndef IKS4A1_HYBRID_SENSOR_EX_H
#define IKS4A1_HYBRID_SENSOR_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "iks4a1_hybrid_sensors.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup IKS4A1 IKS4A1
  * @{
  */

/** @addtogroup IKS4A1_HYBRID_SENSOR_EX IKS4A1 HYBRID SENSOR EX
  * @{
  */

/** @defgroup IKS4A1_HYBRID_SENSOR_EX_Exported_Types IKS4A1 HYBRID SENSOR EX Exported Types
  * @{
  */

typedef enum
{
  IKS4A1_HYBRID_SENSOR_INT1_PIN = 0,
  IKS4A1_HYBRID_SENSOR_INT2_PIN
} IKS4A1_HYBRID_SENSOR_IntPin_t;

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
} IKS4A1_HYBRID_SENSOR_Event_Status_t;

/**
  * @}
  */

/** @addtogroup IKS4A1_HYBRID_SENSOR_EX_Exported_Functions IKS4A1 HYBRID SENSOR EX Exported Functions
  * @{
  */

int32_t IKS4A1_HYBRID_SENSOR_Read_Register(uint32_t Instance, uint8_t Reg, uint8_t *Data);
int32_t IKS4A1_HYBRID_SENSOR_Write_Register(uint32_t Instance, uint8_t Reg, uint8_t Data);
int32_t IKS4A1_HYBRID_SENSOR_Get_DRDY_Status(uint32_t Instance, uint32_t Function, uint8_t *Status);

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

#endif /* IKS4A1_HYBRID_SENSOR_EX_H */
