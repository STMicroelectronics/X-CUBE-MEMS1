/**
  ******************************************************************************
  * @file    iks02a1_env_sensors_ex.h
  * @author  MEMS Software Solutions Team
  * @brief   This file provides a set of extended functions needed to manage the environmental sensors
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
#ifndef IKS02A1_ENV_SENSOR_EX_H
#define IKS02A1_ENV_SENSOR_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "iks02a1_env_sensors.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup IKS02A1 IKS02A1
  * @{
  */

/** @addtogroup IKS02A1_ENV_SENSOR_EX IKS02A1_ENV_SENSOR_EX
  * @{
  */

/** @addtogroup IKS02A1_ENV_SENSOR_EX_Exported_Functions IKS02A1_ENV_SENSOR_EX Exported Functions
  * @{
  */

int32_t IKS02A1_ENV_SENSOR_Get_DRDY_Status(uint32_t Instance, uint32_t Function, uint8_t *Status);
int32_t IKS02A1_ENV_SENSOR_Read_Register(uint32_t Instance, uint8_t Reg, uint8_t *Data);
int32_t IKS02A1_ENV_SENSOR_Write_Register(uint32_t Instance, uint8_t Reg, uint8_t Data);

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

#endif /* IKS02A1_ENV_SENSOR_EX_H */
