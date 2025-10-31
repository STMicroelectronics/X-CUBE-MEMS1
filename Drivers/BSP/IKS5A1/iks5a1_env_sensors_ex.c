/**
  ******************************************************************************
  * @file    iks5a1_env_sensors_ex.c
  * @author  MEMS Software Solutions Team
  * @brief   This file provides a set of extended functions needed to manage the
  *          environmental sensors
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

/* Includes ------------------------------------------------------------------*/
#include "iks5a1_env_sensors_ex.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup IKS5A1 IKS5A1
  * @{
  */

/** @defgroup IKS5A1_ENV_SENSOR_EX IKS5A1 ENV SENSOR EX
  * @{
  */

/** @defgroup IKS5A1_ENV_SENSOR_EX_Imported_Variables IKS5A1 ENV SENSOR EX Imported Variables
  * @{
  */

extern void *EnvCompObj[IKS5A1_ENV_INSTANCES_NBR];

/**
  * @}
  */

/** @defgroup IKS5A1_ENV_SENSOR_EX_Exported_Functions IKS5A1 ENV SENSOR EX Exported Functions
  * @{
  */

/**
  * @brief  Get the register value
  * @param  Instance the device instance
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval BSP status
  */
int32_t IKS5A1_ENV_SENSOR_Read_Register(uint32_t Instance, uint8_t Reg, uint8_t *Data)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS5A1_ENV_SENSOR_ILPS22QS_0 == 1)
    case IKS5A1_ILPS22QS_0:
      if (ILPS22QS_Read_Reg(EnvCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS5A1_ENV_SENSOR_SHT40AD1B_0 == 1)
    case IKS5A1_SHT40AD1B_0:
      /* Note: This special device doesn't contain registers */
      *Data = 0;
      ret = BSP_ERROR_COMPONENT_FAILURE;
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
int32_t IKS5A1_ENV_SENSOR_Write_Register(uint32_t Instance, uint8_t Reg, uint8_t Data)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS5A1_ENV_SENSOR_ILPS22QS_0 == 1)
    case IKS5A1_ILPS22QS_0:
      if (ILPS22QS_Write_Reg(EnvCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS5A1_ENV_SENSOR_SHT40AD1B_0 == 1)
    case IKS5A1_SHT40AD1B_0:
      /* Note: This special device doesn't contain registers */
      ret = BSP_ERROR_COMPONENT_FAILURE;
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Get the status of data ready bit
  * @param  Instance the device instance
  * @param  Function Environmental sensor function. Could be:
  *         - ENV_HUMIDITY
  *         - ENV_PRESSURE
  *         - ENV_TEMPERATURE
  * @param  Status the pointer to the status
  * @retval BSP status
  */
int32_t IKS5A1_ENV_SENSOR_Get_DRDY_Status(uint32_t Instance, uint32_t Function, uint8_t *Status)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS5A1_ENV_SENSOR_ILPS22QS_0 == 1)
    case IKS5A1_ILPS22QS_0:
      if ((Function & ENV_PRESSURE) == ENV_PRESSURE)
      {
        if (ILPS22QS_PRESS_Get_DRDY_Status(EnvCompObj[Instance], Status) != BSP_ERROR_NONE)
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
        if (ILPS22QS_TEMP_Get_DRDY_Status(EnvCompObj[Instance], Status) != BSP_ERROR_NONE)
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

#if (USE_IKS5A1_ENV_SENSOR_SHT40AD1B_0 == 1)
    case IKS5A1_SHT40AD1B_0:
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
  * @brief  Set environmental sensor one shot mode
  * @param  Instance environmental sensor instance to be used
  * @retval BSP status
  */
int32_t IKS5A1_ENV_SENSOR_Set_One_Shot(uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS5A1_ENV_SENSOR_ILPS22QS_0 == 1)
    case IKS5A1_ILPS22QS_0:
      if (ILPS22QS_Set_One_Shot(EnvCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
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
  * @brief  Get environmental sensor one shot status
  * @param  Instance environmental sensor instance to be used
  * @param  Status pointer to the one shot status (1 means measurements available, 0 means measurements not available yet)
  * @retval BSP status
  */
int32_t IKS5A1_ENV_SENSOR_Get_One_Shot_Status(uint32_t Instance, uint8_t *Status)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS5A1_ENV_SENSOR_ILPS22QS_0 == 1)
    case IKS5A1_ILPS22QS_0:
      if (ILPS22QS_Get_One_Shot_Status(EnvCompObj[Instance], Status) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
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
