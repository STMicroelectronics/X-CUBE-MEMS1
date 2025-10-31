/**
  ******************************************************************************
  * @file    iks5a1_motion_sensors_ex.c
  * @author  MEMS Software Solutions Team
  * @brief   This file provides a set of extended functions needed to manage the
  *          motion sensors
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
#include "iks5a1_motion_sensors_ex.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup IKS5A1 IKS5A1
  * @{
  */

/** @defgroup IKS5A1_MOTION_SENSOR_EX IKS5A1 MOTION SENSOR EX
  * @{
  */

/** @defgroup IKS5A1_MOTION_SENSOR_EX_Imported_Variables IKS5A1 MOTION SENSOR EX Imported Variables
  * @{
  */

extern void *MotionCompObj[IKS5A1_MOTION_INSTANCES_NBR];

/**
  * @}
  */

/** @defgroup IKS5A1_MOTION_SENSOR_EX_Exported_Functions IKS5A1 MOTION SENSOR EX Exported Functions
  * @{
  */

/**
  * @brief  Get the register value
  * @param  Instance the device instance
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval BSP status
  */
int32_t IKS5A1_MOTION_SENSOR_Read_Register(uint32_t Instance, uint8_t Reg, uint8_t *Data)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS5A1_MOTION_SENSOR_ISM6HG256X_0 == 1)
    case IKS5A1_ISM6HG256X_0:
      if (ISM6HG256X_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS5A1_MOTION_SENSOR_ISM330IS_0 == 1)
    case IKS5A1_ISM330IS_0:
      if (ISM330IS_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS5A1_MOTION_SENSOR_IIS2DULPX_0 == 1)
    case IKS5A1_IIS2DULPX_0:
      if (IIS2DULPX_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS5A1_MOTION_SENSOR_IIS2MDC_0 == 1)
    case IKS5A1_IIS2MDC_0:
      if (IIS2MDC_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS5A1_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS5A1_AIS2IH_0:
      if (AIS2IH_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS5A1_MOTION_SENSOR_ASM330LHHX_0 == 1)
    case IKS5A1_ASM330LHHX_0:
      if (ASM330LHHX_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS5A1_MOTION_SENSOR_IIS2DLPC_0 == 1)
    case IKS5A1_IIS2DLPC_0:
      if (IIS2DLPC_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS5A1_MOTION_SENSOR_IIS2ICLX_0 == 1)
    case IKS5A1_IIS2ICLX_0:
      if (IIS2ICLX_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS5A1_MOTION_SENSOR_ISM330BX_0 == 1)
    case IKS5A1_ISM330BX_0:
      if (ISM330BX_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS5A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS5A1_ISM330DHCX_0:
      if (ISM330DHCX_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
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
int32_t IKS5A1_MOTION_SENSOR_Write_Register(uint32_t Instance, uint8_t Reg, uint8_t Data)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS5A1_MOTION_SENSOR_ISM6HG256X_0 == 1)
    case IKS5A1_ISM6HG256X_0:
      if (ISM6HG256X_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS5A1_MOTION_SENSOR_ISM330IS_0 == 1)
    case IKS5A1_ISM330IS_0:
      if (ISM330IS_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS5A1_MOTION_SENSOR_IIS2DULPX_0 == 1)
    case IKS5A1_IIS2DULPX_0:
      if (IIS2DULPX_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS5A1_MOTION_SENSOR_IIS2MDC_0 == 1)
    case IKS5A1_IIS2MDC_0:
      if (IIS2MDC_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS5A1_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS5A1_AIS2IH_0:
      if (AIS2IH_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS5A1_MOTION_SENSOR_ASM330LHHX_0 == 1)
    case IKS5A1_ASM330LHHX_0:
      if (ASM330LHHX_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS5A1_MOTION_SENSOR_IIS2DLPC_0 == 1)
    case IKS5A1_IIS2DLPC_0:
      if (IIS2DLPC_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS5A1_MOTION_SENSOR_IIS2ICLX_0 == 1)
    case IKS5A1_IIS2ICLX_0:
      if (IIS2ICLX_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS5A1_MOTION_SENSOR_ISM330BX_0 == 1)
    case IKS5A1_ISM330BX_0:
      if (ISM330BX_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS5A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS5A1_ISM330DHCX_0:
      if (ISM330DHCX_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
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
  * @brief  Enable motion sensor high-g accelerometer
  * @param  Instance Motion sensor instance
  * @param  Function Motion sensor function. Could be:
  *         - MOTION_ACCELERO
  * @retval BSP status
  */
int32_t IKS5A1_MOTION_SENSOR_ACC_HG_Enable(uint32_t Instance, uint32_t Function)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS5A1_MOTION_SENSOR_ISM6HG256X_0 == 1)
    case IKS5A1_ISM6HG256X_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (ISM6HG256X_ACC_HG_Enable(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
  * @brief  Disable motion sensor high-g accelerometer
  * @param  Instance Motion sensor instance
  * @param  Function Motion sensor function. Could be:
  *         - MOTION_ACCELERO
  * @retval BSP status
  */
int32_t IKS5A1_MOTION_SENSOR_ACC_HG_Disable(uint32_t Instance, uint32_t Function)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS5A1_MOTION_SENSOR_ISM6HG256X_0 == 1)
    case IKS5A1_ISM6HG256X_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (ISM6HG256X_ACC_HG_Disable(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
  * @brief  Get motion sensor high-g accelerometer sensitivity
  * @param  Instance Motion sensor instance
  * @param  Function Motion sensor function. Could be:
  *         - MOTION_ACCELERO
  * @param  Sensitivity pointer
  * @retval BSP status
  */
int32_t IKS5A1_MOTION_SENSOR_ACC_HG_GetSensitivity(uint32_t Instance, uint32_t Function, float_t *Sensitivity)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS5A1_MOTION_SENSOR_ISM6HG256X_0 == 1)
    case IKS5A1_ISM6HG256X_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (ISM6HG256X_ACC_HG_GetSensitivity(MotionCompObj[Instance], Sensitivity) != BSP_ERROR_NONE)
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
  * @brief  Get motion sensor high-g accelerometer output data rate
  * @param  Instance Motion sensor instance
  * @param  Function Motion sensor function. Could be:
  *         - MOTION_ACCELERO
  * @param  Odr pointer where the output data rate is written
  * @retval BSP status
  */
int32_t IKS5A1_MOTION_SENSOR_ACC_HG_GetOutputDataRate(uint32_t Instance, uint32_t Function, float_t *Odr)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS5A1_MOTION_SENSOR_ISM6HG256X_0 == 1)
    case IKS5A1_ISM6HG256X_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (ISM6HG256X_ACC_HG_GetOutputDataRate(MotionCompObj[Instance], Odr) != BSP_ERROR_NONE)
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
  * @brief  Set motion sensor high-g accelerometer output data rate
  * @param  Instance Motion sensor instance
  * @param  Function Motion sensor function. Could be:
  *         - MOTION_ACCELERO
  * @param  Odr the output data rate value to be set
  * @retval BSP status
  */
int32_t IKS5A1_MOTION_SENSOR_ACC_HG_SetOutputDataRate(uint32_t Instance, uint32_t Function, float_t Odr)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS5A1_MOTION_SENSOR_ISM6HG256X_0 == 1)
    case IKS5A1_ISM6HG256X_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (ISM6HG256X_ACC_HG_SetOutputDataRate(MotionCompObj[Instance], Odr) != BSP_ERROR_NONE)
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
  * @brief  Get motion sensor high-g accelerometer full scale
  * @param  Instance Motion sensor instance
  * @param  Function Motion sensor function. Could be:
  *         - MOTION_ACCELERO
  * @param  FullScale pointer where the full scale is written
  * @retval BSP status
  */
int32_t IKS5A1_MOTION_SENSOR_ACC_HG_GetFullScale(uint32_t Instance, uint32_t Function, int32_t *FullScale)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS5A1_MOTION_SENSOR_ISM6HG256X_0 == 1)
    case IKS5A1_ISM6HG256X_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (ISM6HG256X_ACC_HG_GetFullScale(MotionCompObj[Instance], FullScale) != BSP_ERROR_NONE)
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
  * @brief  Set motion sensor high-g accelerometer full scale
  * @param  Instance Motion sensor instance
  * @param  Function Motion sensor function. Could be:
  *         - MOTION_ACCELERO
  * @param  FullScale the functional full scale to be set
  * @retval BSP status
  */
int32_t IKS5A1_MOTION_SENSOR_ACC_HG_SetFullScale(uint32_t Instance, uint32_t Function, int32_t FullScale)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS5A1_MOTION_SENSOR_ISM6HG256X_0 == 1)
    case IKS5A1_ISM6HG256X_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (ISM6HG256X_ACC_HG_SetFullScale(MotionCompObj[Instance], FullScale) != BSP_ERROR_NONE)
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
  * @brief  Get motion sensor high-g accelerometer raw axes data
  * @param  Instance Motion sensor instance
  * @param  Function Motion sensor function. Could be:
  *         - MOTION_ACCELERO
  * @param  AxesRaw pointer to raw axes data structure
  * @retval BSP status
  */
int32_t IKS5A1_MOTION_SENSOR_ACC_HG_GetAxesRaw(uint32_t Instance, uint32_t Function, IKS5A1_MOTION_SENSOR_AxesRaw_t *AxesRaw)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS5A1_MOTION_SENSOR_ISM6HG256X_0 == 1)
    case IKS5A1_ISM6HG256X_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (ISM6HG256X_ACC_HG_GetAxesRaw(MotionCompObj[Instance], (ISM6HG256X_AxesRaw_t *)AxesRaw) != BSP_ERROR_NONE)
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
  * @brief  Get motion sensor high-g accelerometer axes data
  * @param  Instance Motion sensor instance
  * @param  Function Motion sensor function. Could be:
  *         - MOTION_ACCELERO
  * @param  Axes pointer to axes data structure
  * @retval BSP status
  */
int32_t IKS5A1_MOTION_SENSOR_ACC_HG_GetAxes(uint32_t Instance, uint32_t Function, IKS5A1_MOTION_SENSOR_Axes_t *Axes)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS5A1_MOTION_SENSOR_ISM6HG256X_0 == 1)
    case IKS5A1_ISM6HG256X_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (ISM6HG256X_ACC_HG_GetAxes(MotionCompObj[Instance], (ISM6HG256X_Axes_t *)Axes) != BSP_ERROR_NONE)
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
  * @brief  Get the status of data ready bit
  * @param  Instance the device instance
  * @param  Function Motion sensor function. Could be:
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @param  Status the pointer to the status
  * @retval BSP status
  */
int32_t IKS5A1_MOTION_SENSOR_Get_DRDY_Status(uint32_t Instance, uint32_t Function, uint8_t *Status)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS5A1_MOTION_SENSOR_ISM6HG256X_0 == 1)
    case IKS5A1_ISM6HG256X_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        uint8_t status_acc = 0;
        uint8_t status_acc_hg = 0;

        ret = BSP_ERROR_NONE;

        if (ISM6HG256X_ACC_Get_DRDY_Status(MotionCompObj[Instance], &status_acc) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }

        if (ISM6HG256X_ACC_HG_Get_DRDY_Status(MotionCompObj[Instance], &status_acc_hg) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }

        if (ret == BSP_ERROR_NONE)
        {
          *Status = (status_acc_hg << 1) + status_acc;
        }
        else
        {
          *Status = 0;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (ISM6HG256X_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
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

#if (USE_IKS5A1_MOTION_SENSOR_ISM330IS_0 == 1)
    case IKS5A1_ISM330IS_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (ISM330IS_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (ISM330IS_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
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

#if (USE_IKS5A1_MOTION_SENSOR_IIS2DULPX_0 == 1)
    case IKS5A1_IIS2DULPX_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (IIS2DULPX_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
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

#if (USE_IKS5A1_MOTION_SENSOR_IIS2MDC_0 == 1)
    case IKS5A1_IIS2MDC_0:
      if ((Function & MOTION_MAGNETO) == MOTION_MAGNETO)
      {
        if (IIS2MDC_MAG_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
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

#if (USE_IKS5A1_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS5A1_AIS2IH_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (AIS2IH_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
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

#if (USE_IKS5A1_MOTION_SENSOR_ASM330LHHX_0 == 1)
    case IKS5A1_ASM330LHHX_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (ASM330LHHX_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (ASM330LHHX_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
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

#if (USE_IKS5A1_MOTION_SENSOR_IIS2DLPC_0 == 1)
    case IKS5A1_IIS2DLPC_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (IIS2DLPC_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
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

#if (USE_IKS5A1_MOTION_SENSOR_IIS2ICLX_0 == 1)
    case IKS5A1_IIS2ICLX_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (IIS2ICLX_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
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

#if (USE_IKS5A1_MOTION_SENSOR_ISM330BX_0 == 1)
    case IKS5A1_ISM330BX_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (ISM330BX_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (ISM330BX_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
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

#if (USE_IKS5A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS5A1_ISM330DHCX_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (ISM330DHCX_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (ISM330DHCX_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
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
