/**
 ******************************************************************************
 * @file    iks01a3_hybrid_sensors_ex.h
 * @author  MEMS Software Solutions Team
 * @brief   This file provides a set of extended functions needed to manage the
 *          hybrid sensors
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
