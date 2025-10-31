/**
  ******************************************************************************
  * @file    iks5a1_env_sensors_ex.h
  * @author  MEMS Software Solutions Team
  * @brief   This file provides a set of extended functions needed to manage the environmental sensors
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IKS5A1_ENV_SENSOR_EX_H
#define IKS5A1_ENV_SENSOR_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "iks5a1_env_sensors.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup IKS5A1 IKS5A1
  * @{
  */

/** @addtogroup IKS5A1_ENV_SENSOR_EX IKS5A1_ENV_SENSOR_EX
  * @{
  */

/** @addtogroup IKS5A1_ENV_SENSOR_EX_Exported_Functions IKS5A1_ENV_SENSOR_EX Exported Functions
  * @{
  */

int32_t IKS5A1_ENV_SENSOR_Read_Register(uint32_t Instance, uint8_t Reg, uint8_t *Data);
int32_t IKS5A1_ENV_SENSOR_Write_Register(uint32_t Instance, uint8_t Reg, uint8_t Data);
int32_t IKS5A1_ENV_SENSOR_Get_DRDY_Status(uint32_t Instance, uint32_t Function, uint8_t *Status);
int32_t IKS5A1_ENV_SENSOR_Set_One_Shot(uint32_t Instance);
int32_t IKS5A1_ENV_SENSOR_Get_One_Shot_Status(uint32_t Instance, uint8_t *Status);

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

#endif /* IKS5A1_ENV_SENSOR_EX_H */
