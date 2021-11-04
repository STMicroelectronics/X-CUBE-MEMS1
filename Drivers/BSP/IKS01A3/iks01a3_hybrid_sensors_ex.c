/**
 ******************************************************************************
 * @file    iks01a3_hybrid_sensors_ex.c
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

/* Includes ------------------------------------------------------------------*/
#include "iks01a3_hybrid_sensors_ex.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup IKS01A3 IKS01A3
 * @{
 */

/** @defgroup IKS01A3_HYBRID_SENSOR_EX IKS01A3 HYBRID SENSOR EX
 * @{
 */

/** @defgroup IKS01A3_HYBRID_SENSOR_EX_Imported_Variables IKS01A3 HYBRID SENSOR EX Imported Variables
 * @{
 */

extern void *HybridCompObj[];

/**
 * @}
 */

/** @defgroup IKS01A3_HYBRID_SENSOR_EX_Exported_Functions IKS01A3 HYBRID SENSOR EX Exported Functions
 * @{
 */

/**
 * @brief  Get the status of data ready bit
 * @param  Instance the device instance
 * @param  Function Hybrid sensor function
 * @param  Status the pointer to the status
 * @retval BSP status
 */
int32_t IKS01A3_HYBRID_SENSOR_Get_DRDY_Status(uint32_t Instance, uint32_t Function, uint8_t *Status)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_HYBRID_SENSOR_LIS2DTW12_0 == 1)
    case IKS01A3_LIS2DTW12_0:
      if (((Function & HYBRID_ACCELERO) == HYBRID_ACCELERO) || ((Function & HYBRID_TEMPERATURE) == HYBRID_TEMPERATURE))
      {
        if (LIS2DTW12_ACC_Get_DRDY_Status(HybridCompObj[Instance], Status) != BSP_ERROR_NONE)
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
int32_t IKS01A3_HYBRID_SENSOR_Read_Register(uint32_t Instance, uint8_t Reg, uint8_t *Data)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_HYBRID_SENSOR_LIS2DTW12_0 == 1)
    case IKS01A3_LIS2DTW12_0:
      if (LIS2DTW12_Read_Reg(HybridCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
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
 * @brief  Set the register value
 * @param  Instance the device instance
 * @param  Reg address to be read
 * @param  Data value to be written
 * @retval BSP status
 */
int32_t IKS01A3_HYBRID_SENSOR_Write_Register(uint32_t Instance, uint8_t Reg, uint8_t Data)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_HYBRID_SENSOR_LIS2DTW12_0 == 1)
    case IKS01A3_LIS2DTW12_0:
      if (LIS2DTW12_Write_Reg(HybridCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
