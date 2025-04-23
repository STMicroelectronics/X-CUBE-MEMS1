/**
  ******************************************************************************
  * @file    iks02a1_motion_sensors_ex.c
  * @author  MEMS Software Solutions Team
  * @brief   This file provides a set of extended functions needed to manage the motion sensors
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
#include "iks02a1_motion_sensors_ex.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup IKS02A1 IKS02A1
  * @{
  */

/** @defgroup IKS02A1_MOTION_SENSOR_EX IKS02A1 MOTION SENSOR EX
  * @{
  */

/** @defgroup IKS02A1_MOTION_SENSOR_EX_Imported_Variables IKS02A1 MOTION SENSOR EX Imported Variables
  * @{
  */

extern void *MotionCompObj[IKS02A1_MOTION_INSTANCES_NBR];

/**
  * @}
  */

/** @defgroup IKS02A1_MOTION_SENSOR_EX_Exported_Functions IKS02A1 MOTION SENSOR EX Exported Functions
  * @{
  */

/**
  * @brief  Get the status of data ready bit (available only for ISM330DHCX, IIS2DLPC, IIS2MDC, ASM330LHHX, ISM330BX, IIS2DULPX sensors)
  * @param  Instance the device instance
  * @param  Function Motion sensor function. Could be:
  *         - MOTION_ACCELERO or MOTION_GYRO for instance IKS02A1_ISM330DHCX_0, IKS02A1_ASM330LHHX_0
  *         - MOTION_ACCELERO for instance IKS02A1_IIS2DLPC_0
  *         - MOTION_MAGNETO for instance IKS02A1_IIS2MDC_0
  * @param  Status the pointer to the status
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_Get_DRDY_Status(uint32_t Instance, uint32_t Function, uint8_t *Status)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS02A1_ISM330DHCX_0:
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

#if (USE_IKS02A1_MOTION_SENSOR_IIS2DLPC_0 == 1)
    case IKS02A1_IIS2DLPC_0:
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

#if (USE_IKS02A1_MOTION_SENSOR_IIS2MDC_0 == 1)
    case IKS02A1_IIS2MDC_0:
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

#if (USE_IKS02A1_MOTION_SENSOR_IIS2ICLX_0 == 1)
    case IKS02A1_IIS2ICLX_0:
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

#if (USE_IKS02A1_MOTION_SENSOR_ASM330LHHX_0 == 1)
    case IKS02A1_ASM330LHHX_0:
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

#if (USE_IKS02A1_MOTION_SENSOR_ISM330BX_0 == 1)
    case IKS02A1_ISM330BX_0:
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

#if (USE_IKS02A1_MOTION_SENSOR_IIS2DULPX_0 == 1)
    case IKS02A1_IIS2DULPX_0:
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

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Get the register value (available only for ISM330DHCX, IIS2DLPC, IIS2MDC, ASM330LHHX, ISM330BX, IIS2DULPX sensors)
  * @param  Instance the device instance
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_Read_Register(uint32_t Instance, uint8_t Reg, uint8_t *Data)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS02A1_ISM330DHCX_0:
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

#if (USE_IKS02A1_MOTION_SENSOR_IIS2DLPC_0 == 1)
    case IKS02A1_IIS2DLPC_0:
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

#if (USE_IKS02A1_MOTION_SENSOR_IIS2MDC_0 == 1)
    case IKS02A1_IIS2MDC_0:
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

#if (USE_IKS02A1_MOTION_SENSOR_IIS2ICLX_0 == 1)
    case IKS02A1_IIS2ICLX_0:
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

#if (USE_IKS02A1_MOTION_SENSOR_ASM330LHHX_0 == 1)
    case IKS02A1_ASM330LHHX_0:
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

#if (USE_IKS02A1_MOTION_SENSOR_ISM330BX_0 == 1)
    case IKS02A1_ISM330BX_0:
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

#if (USE_IKS02A1_MOTION_SENSOR_IIS2DULPX_0 == 1)
    case IKS02A1_IIS2DULPX_0:
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

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Set the register value (available only for ISM330DHCX, IIS2DLPC, IIS2MDC, ASM330LHHX, ISM330BX, IIS2DULPX sensors)
  * @param  Instance the device instance
  * @param  Reg address to be read
  * @param  Data value to be written
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_Write_Register(uint32_t Instance, uint8_t Reg, uint8_t Data)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS02A1_ISM330DHCX_0:
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

#if (USE_IKS02A1_MOTION_SENSOR_IIS2DLPC_0 == 1)
    case IKS02A1_IIS2DLPC_0:
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

#if (USE_IKS02A1_MOTION_SENSOR_IIS2MDC_0 == 1)
    case IKS02A1_IIS2MDC_0:
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

#if (USE_IKS02A1_MOTION_SENSOR_IIS2ICLX_0 == 1)
    case IKS02A1_IIS2ICLX_0:
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

#if (USE_IKS02A1_MOTION_SENSOR_ASM330LHHX_0 == 1)
    case IKS02A1_ASM330LHHX_0:
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

#if (USE_IKS02A1_MOTION_SENSOR_ISM330BX_0 == 1)
    case IKS02A1_ISM330BX_0:
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

#if (USE_IKS02A1_MOTION_SENSOR_IIS2DULPX_0 == 1)
    case IKS02A1_IIS2DULPX_0:
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

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Get number of unread FIFO samples (available only for ISM330DHCX sensor)
  * @param  Instance the device instance
  * @param  NumSamples number of unread FIFO samples
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_FIFO_Get_Num_Samples(uint32_t Instance, uint16_t *NumSamples)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS02A1_ISM330DHCX_0:
      if (ISM330DHCX_FIFO_Get_Num_Samples(MotionCompObj[Instance], NumSamples) != BSP_ERROR_NONE)
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
  * @brief  Set FIFO BDR value (available only for ISM330DHCX sensor)
  * @param  Instance the device instance
  * @param  Function Motion sensor function. Could be:
  *         - MOTION_GYRO or MOTION_ACCELERO for instance IKS02A1_ISM330DHCX_0
  * @param  Bdr FIFO BDR value
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_FIFO_Set_BDR(uint32_t Instance, uint32_t Function, float_t Bdr)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS02A1_ISM330DHCX_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (ISM330DHCX_FIFO_ACC_Set_BDR(MotionCompObj[Instance], Bdr) != BSP_ERROR_NONE)
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
        if (ISM330DHCX_FIFO_GYRO_Set_BDR(MotionCompObj[Instance], Bdr) != BSP_ERROR_NONE)
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
        ret = BSP_ERROR_WRONG_PARAM;
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
  * @brief  Set FIFO full interrupt on INT1 pin (available only for ISM330DHCX sensor)
  * @param  Instance the device instance
  * @param  Status FIFO full interrupt on INT1 pin
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_FIFO_Set_INT1_FIFO_Full(uint32_t Instance, uint8_t Status)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS02A1_ISM330DHCX_0:
      if (ISM330DHCX_FIFO_Set_INT1_FIFO_Full(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
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
  * @brief  Set FIFO watermark level (available only for ISM330DHCX sensor)
  * @param  Instance the device instance
  * @param  Watermark FIFO watermark level
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_FIFO_Set_Watermark_Level(uint32_t Instance, uint16_t Watermark)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS02A1_ISM330DHCX_0:
      if (ISM330DHCX_FIFO_Set_Watermark_Level(MotionCompObj[Instance], Watermark) != BSP_ERROR_NONE)
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
  * @brief  Set FIFO stop on watermark (available only for ISM330DHCX sensor)
  * @param  Instance the device instance
  * @param  Status FIFO stop on watermark status
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_FIFO_Set_Stop_On_Fth(uint32_t Instance, uint8_t Status)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS02A1_ISM330DHCX_0:
      if (ISM330DHCX_FIFO_Set_Stop_On_Fth(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
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
  * @brief  Set FIFO mode (available only for ISM330DHCX sensor)
  * @param  Instance the device instance
  * @param  Mode FIFO mode
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_FIFO_Set_Mode(uint32_t Instance, uint8_t Mode)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS02A1_ISM330DHCX_0:
      if (ISM330DHCX_FIFO_Set_Mode(MotionCompObj[Instance], Mode) != BSP_ERROR_NONE)
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
  * @brief  Get FIFO tag (available only for ISM330DHCX sensor)
  * @param  Instance the device instance
  * @param  Tag FIFO tag
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_FIFO_Get_Tag(uint32_t Instance, uint8_t *Tag)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS02A1_ISM330DHCX_0:
      if (ISM330DHCX_FIFO_Get_Tag(MotionCompObj[Instance], Tag) != BSP_ERROR_NONE)
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
  * @brief  Get FIFO axes data (available only for ISM330DHCX sensor)
  * @param  Instance the device instance
  * @param  Function Motion sensor function. Could be:
  *         - MOTION_GYRO or MOTION_ACCELERO for instance IKS02A1_ISM330DHCX_0
  * @param  Data FIFO axes data
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_FIFO_Get_Axes(uint32_t Instance, uint32_t Function, IKS02A1_MOTION_SENSOR_Axes_t *Data)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS02A1_ISM330DHCX_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (ISM330DHCX_FIFO_ACC_Get_Axes(MotionCompObj[Instance], (ISM330DHCX_Axes_t *)Data) != BSP_ERROR_NONE)
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
        if (ISM330DHCX_FIFO_GYRO_Get_Axes(MotionCompObj[Instance], (ISM330DHCX_Axes_t *)Data) != BSP_ERROR_NONE)
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
        ret = BSP_ERROR_WRONG_PARAM;
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
  * @brief  Set DRDY mode (available only for ISM330DHCX sensor)
  * @param  Instance the device instance
  * @param  Mode DRDY mode
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_DRDY_Set_Mode(uint32_t Instance, uint8_t Mode)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS02A1_ISM330DHCX_0:
      if (ISM330DHCX_DRDY_Set_Mode(MotionCompObj[Instance], Mode) != BSP_ERROR_NONE)
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
  * @brief  Enable DRDY on selected interrupt pin (available only for ISM330DHCX sensor)
  * @param  Instance the device instance
  * @param  Function Motion sensor function. Could be:
  *         - MOTION_ACCELERO or MOTION_GYRO for instance IKS02A1_ISM330DHCX_0
  * @param  IntPin the interrupt pin to be used
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_DRDY_Enable_Interrupt(uint32_t Instance, uint32_t Function,
                                                    IKS02A1_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS02A1_ISM330DHCX_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (IntPin == IKS02A1_MOTION_SENSOR_INT1_PIN)
        {
          if (ISM330DHCX_ACC_Enable_DRDY_On_INT1(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (IntPin == IKS02A1_MOTION_SENSOR_INT2_PIN)
        {
          if (ISM330DHCX_GYRO_Enable_DRDY_On_INT2(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
  * @brief  Disable DRDY on selected interrupt pin (available only for ISM330DHCX sensor)
  * @param  Instance the device instance
  * @param  Function Motion sensor function. Could be:
  *         - MOTION_ACCELERO or MOTION_GYRO for instance IKS02A1_ISM330DHCX_0
  * @param  IntPin the interrupt pin to be used
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_DRDY_Disable_Interrupt(uint32_t Instance, uint32_t Function,
                                                     IKS02A1_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS02A1_ISM330DHCX_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (IntPin == IKS02A1_MOTION_SENSOR_INT1_PIN)
        {
          if (ISM330DHCX_ACC_Disable_DRDY_On_INT1(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (IntPin == IKS02A1_MOTION_SENSOR_INT2_PIN)
        {
          if (ISM330DHCX_GYRO_Disable_DRDY_On_INT2(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
