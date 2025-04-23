/**
  ******************************************************************************
  * @file    iks02a1_env_sensors_ex.c
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

/* Includes ------------------------------------------------------------------*/
#include "iks02a1_env_sensors_ex.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup IKS02A1 IKS02A1
  * @{
  */

/** @defgroup IKS02A1_ENV_SENSOR_EX IKS02A1 ENV SENSOR EX
  * @{
  */

/** @defgroup IKS02A1_ENV_SENSOR_EX_Imported_Variables IKS02A1 ENV SENSOR EX Imported Variables
  * @{
  */

extern void *EnvCompObj[IKS02A1_ENV_INSTANCES_NBR];

/**
  * @}
  */

/** @defgroup IKS02A1_ENV_SENSOR_EX_Exported_Functions IKS02A1 ENV SENSOR EX Exported Functions
  * @{
  */

/**
  * @brief  Get the status of data ready bit (available only for SHT40AD1B sensor)
  * @param  Instance the device instance
  * @param  Function Environmental sensor function. Could be:
  *         - ENV_HUMIDITY or ENV_TEMPERATURE for instance IKS02A1_HTS221_0
  *         - ENV_PRESSURE or ENV_TEMPERATURE for instance IKS02A1_LPS22HH_0
  *         - ENV_TEMPERATURE for instance IKS02A1_STTS751_0
  * @param  Status the pointer to the status
  * @retval BSP status
  */
int32_t IKS02A1_ENV_SENSOR_Get_DRDY_Status(uint32_t Instance, uint32_t Function, uint8_t *Status)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS02A1_ENV_SENSOR_SHT40AD1B_0 == 1)
    case IKS02A1_SHT40AD1B_0:
      if ((Function & ENV_HUMIDITY) == ENV_HUMIDITY)
      {
        if (SHT40AD1B_HUM_Get_DRDY_Status(EnvCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & ENV_TEMPERATURE) == ENV_TEMPERATURE)
      {
        if (SHT40AD1B_TEMP_Get_DRDY_Status(EnvCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Get the register value
  * @param  Instance the device instance
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval BSP status
  */
int32_t IKS02A1_ENV_SENSOR_Read_Register(uint32_t Instance, uint8_t Reg, uint8_t *Data)
{
  (void)Reg;
  (void)Data;
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS02A1_ENV_SENSOR_SHT40AD1B_0 == 1)
    case IKS02A1_SHT40AD1B_0:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Set the register value
  * @param  Instance the device instance
  * @param  Reg address to be read
  * @param  Data value to be written
  * @retval BSP status
  */
int32_t IKS02A1_ENV_SENSOR_Write_Register(uint32_t Instance, uint8_t Reg, uint8_t Data)
{
  (void)Reg;
  (void)Data;
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS02A1_ENV_SENSOR_SHT40AD1B_0 == 1)
    case IKS02A1_SHT40AD1B_0:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

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
