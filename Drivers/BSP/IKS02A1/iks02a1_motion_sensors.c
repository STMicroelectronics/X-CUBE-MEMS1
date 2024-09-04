/**
  ******************************************************************************
  * @file    iks02a1_motion_sensors.c
  * @author  MEMS Software Solutions Team
  * @brief   This file provides a set of functions needed to manage the motion sensors
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
#include "iks02a1_motion_sensors.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup IKS02A1 IKS02A1
  * @{
  */

/** @defgroup IKS02A1_MOTION_SENSOR IKS02A1 MOTION SENSOR
  * @{
  */

/** @defgroup IKS02A1_MOTION_SENSOR_Exported_Variables IKS02A1 MOTION SENSOR Exported Variables
  * @{
  */
extern void
*MotionCompObj[IKS02A1_MOTION_INSTANCES_NBR]; /* This "redundant" line is here to fulfil MISRA C-2012 rule 8.4 */
void *MotionCompObj[IKS02A1_MOTION_INSTANCES_NBR];

/**
  * @}
  */

/** @defgroup IKS02A1_MOTION_SENSOR_Private_Variables IKS02A1 MOTION SENSOR Private Variables
  * @{
  */

/* We define a jump table in order to get the correct index from the desired function. */
/* This table should have a size equal to the maximum value of a function plus 1.      */
/* But due to MISRA it has to be increased to 7 + 1. */
static uint32_t FunctionIndex[] = {0, 0, 1, 1, 2, 2, 2, 2, 3};
static MOTION_SENSOR_FuncDrv_t *MotionFuncDrv[IKS02A1_MOTION_INSTANCES_NBR][IKS02A1_MOTION_FUNCTIONS_NBR];
static MOTION_SENSOR_CommonDrv_t *MotionDrv[IKS02A1_MOTION_INSTANCES_NBR];
static IKS02A1_MOTION_SENSOR_Ctx_t MotionCtx[IKS02A1_MOTION_INSTANCES_NBR];

/**
  * @}
  */

/** @defgroup IKS02A1_MOTION_SENSOR_Private_Function_Prototypes IKS02A1 MOTION SENSOR Private Function Prototypes
  * @{
  */

#if (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
static int32_t ISM330DHCX_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS02A1_MOTION_SENSOR_IIS2DLPC_0 == 1)
static int32_t IIS2DLPC_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS02A1_MOTION_SENSOR_IIS2MDC_0 == 1)
static int32_t IIS2MDC_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS02A1_MOTION_SENSOR_IIS2ICLX_0 == 1)
static int32_t IIS2ICLX_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS02A1_MOTION_SENSOR_ASM330LHHX_0 == 1)
static int32_t ASM330LHHX_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS02A1_MOTION_SENSOR_ISM330BX_0 == 1)
static int32_t ISM330BX_0_Probe(uint32_t Functions);
#endif

/**
  * @}
  */

/** @defgroup IKS02A1_MOTION_SENSOR_Exported_Functions IKS02A1 MOTION SENSOR Exported Functions
  * @{
  */

/**
  * @brief  Initializes the motion sensors
  * @param  Instance Motion sensor instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO for instances 0, 3, 4
  *         - MOTION_ACCELERO for instance 1
  *         - MOTION_MAGNETO for instance 2
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_Init(uint32_t Instance, uint32_t Functions)
{
  int32_t ret = BSP_ERROR_NONE;
  uint32_t function = MOTION_GYRO;
  uint32_t i;
  uint32_t component_functions = 0;
  IKS02A1_MOTION_SENSOR_Capabilities_t cap;

  switch (Instance)
  {
#if (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS02A1_ISM330DHCX_0:
      if (ISM330DHCX_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS02A1_MOTION_SENSOR_IIS2DLPC_0 == 1)
    case IKS02A1_IIS2DLPC_0:
      if (IIS2DLPC_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS02A1_MOTION_SENSOR_IIS2MDC_0 == 1)
    case IKS02A1_IIS2MDC_0:
      if (IIS2MDC_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS02A1_MOTION_SENSOR_IIS2ICLX_0 == 1)
    case IKS02A1_IIS2ICLX_0:
      if (IIS2ICLX_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS02A1_MOTION_SENSOR_ASM330LHHX_0 == 1)
    case IKS02A1_ASM330LHHX_0:
      if (ASM330LHHX_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS02A1_MOTION_SENSOR_ISM330BX_0 == 1)
    case IKS02A1_ISM330BX_0:
      if (ISM330BX_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  if (ret != BSP_ERROR_NONE)
  {
    return ret;
  }

  for (i = 0; i < IKS02A1_MOTION_FUNCTIONS_NBR; i++)
  {
    if (((Functions & function) == function) && ((component_functions & function) == function))
    {
      if (MotionFuncDrv[Instance][FunctionIndex[function]]->Enable(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_COMPONENT_FAILURE;
      }
    }
    function = function << 1;
  }

  return ret;
}

/**
  * @brief  Deinitialize Motion sensor
  * @param  Instance Motion sensor instance
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_DeInit(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= IKS02A1_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (MotionDrv[Instance]->DeInit(MotionCompObj[Instance]) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
  * @brief  Get motion sensor instance capabilities
  * @param  Instance Motion sensor instance
  * @param  Capabilities pointer to motion sensor capabilities
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_GetCapabilities(uint32_t Instance, IKS02A1_MOTION_SENSOR_Capabilities_t *Capabilities)
{
  int32_t ret;

  if (Instance >= IKS02A1_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], Capabilities) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
  * @brief  Get WHOAMI value
  * @param  Instance Motion sensor instance
  * @param  Id WHOAMI value
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_ReadID(uint32_t Instance, uint8_t *Id)
{
  int32_t ret;

  if (Instance >= IKS02A1_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (MotionDrv[Instance]->ReadID(MotionCompObj[Instance], Id) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
  * @brief  Enable Motion sensor
  * @param  Instance Motion sensor instance
  * @param  Function Motion sensor function. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
  *         - MOTION_ACCELERO for instance 1
  *         - MOTION_MAGNETO for instance 2
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_Enable(uint32_t Instance, uint32_t Function)
{
  int32_t ret;

  if (Instance >= IKS02A1_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->Enable(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
  }

  return ret;
}

/**
  * @brief  Disable Motion sensor
  * @param  Instance Motion sensor instance
  * @param  Function Motion sensor function. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
  *         - MOTION_ACCELERO for instance 1
  *         - MOTION_MAGNETO for instance 2
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_Disable(uint32_t Instance, uint32_t Function)
{
  int32_t ret;

  if (Instance >= IKS02A1_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->Disable(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
  }

  return ret;
}

/**
  * @brief  Get motion sensor axes data
  * @param  Instance Motion sensor instance
  * @param  Function Motion sensor function. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
  *         - MOTION_ACCELERO for instance 1
  *         - MOTION_MAGNETO for instance 2
  * @param  Axes pointer to axes data structure
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_GetAxes(uint32_t Instance, uint32_t Function, IKS02A1_MOTION_SENSOR_Axes_t *Axes)
{
  int32_t ret;

  if (Instance >= IKS02A1_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->GetAxes(MotionCompObj[Instance], Axes) != BSP_ERROR_NONE)
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
  }

  return ret;
}

/**
  * @brief  Get motion sensor axes raw data
  * @param  Instance Motion sensor instance
  * @param  Function Motion sensor function. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
  *         - MOTION_ACCELERO for instance 1
  *         - MOTION_MAGNETO for instance 2
  * @param  Axes pointer to axes raw data structure
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_GetAxesRaw(uint32_t Instance, uint32_t Function, IKS02A1_MOTION_SENSOR_AxesRaw_t *Axes)
{
  int32_t ret;

  if (Instance >= IKS02A1_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->GetAxesRaw(MotionCompObj[Instance], Axes) != BSP_ERROR_NONE)
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
  }

  return ret;
}

/**
  * @brief  Get motion sensor sensitivity
  * @param  Instance Motion sensor instance
  * @param  Function Motion sensor function. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
  *         - MOTION_ACCELERO for instance 1
  *         - MOTION_MAGNETO for instance 2
  * @param  Sensitivity pointer to sensitivity read value
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_GetSensitivity(uint32_t Instance, uint32_t Function, float_t *Sensitivity)
{
  int32_t ret;

  if (Instance >= IKS02A1_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->GetSensitivity(MotionCompObj[Instance],
                                                                           Sensitivity) != BSP_ERROR_NONE)
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
  }

  return ret;
}

/**
  * @brief  Get motion sensor Output Data Rate
  * @param  Instance Motion sensor instance
  * @param  Function Motion sensor function. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
  *         - MOTION_ACCELERO for instance 1
  *         - MOTION_MAGNETO for instance 2
  * @param  Odr pointer to Output Data Rate read value
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float_t *Odr)
{
  int32_t ret;

  if (Instance >= IKS02A1_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->GetOutputDataRate(MotionCompObj[Instance], Odr) != BSP_ERROR_NONE)
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
  }

  return ret;
}

/**
  * @brief  Get motion sensor Full Scale
  * @param  Instance Motion sensor instance
  * @param  Function Motion sensor function. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
  *         - MOTION_ACCELERO for instance 1
  *         - MOTION_MAGNETO for instance 2
  * @param  Fullscale pointer to Fullscale read value
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_GetFullScale(uint32_t Instance, uint32_t Function, int32_t *Fullscale)
{
  int32_t ret;

  if (Instance >= IKS02A1_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->GetFullScale(MotionCompObj[Instance],
                                                                         Fullscale) != BSP_ERROR_NONE)
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
  }

  return ret;
}

/**
  * @brief  Set motion sensor Output Data Rate
  * @param  Instance Motion sensor instance
  * @param  Function Motion sensor function. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
  *         - MOTION_ACCELERO for instance 1
  *         - MOTION_MAGNETO for instance 2
  * @param  Odr Output Data Rate value to be set
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float_t Odr)
{
  int32_t ret;

  if (Instance >= IKS02A1_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->SetOutputDataRate(MotionCompObj[Instance], Odr) != BSP_ERROR_NONE)
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
  }

  return ret;
}

/**
  * @brief  Set motion sensor Full Scale
  * @param  Instance Motion sensor instance
  * @param  Function Motion sensor function. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
  *         - MOTION_ACCELERO for instance 1
  *         - MOTION_MAGNETO for instance 2
  * @param  Fullscale Fullscale value to be set
  * @retval BSP status
  */
int32_t IKS02A1_MOTION_SENSOR_SetFullScale(uint32_t Instance, uint32_t Function, int32_t Fullscale)
{
  int32_t ret;

  if (Instance >= IKS02A1_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->SetFullScale(MotionCompObj[Instance],
                                                                         Fullscale) != BSP_ERROR_NONE)
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
  }

  return ret;
}

/**
  * @}
  */

/** @defgroup IKS02A1_MOTION_SENSOR_Private_Functions IKS02A1 MOTION SENSOR Private Functions
  * @{
  */

#if (USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0  == 1)
/**
  * @brief  Register Bus IOs for instance 0 if component ID is OK
  * @retval BSP status
  */
static int32_t ISM330DHCX_0_Probe(uint32_t Functions)
{
  ISM330DHCX_IO_t            io_ctx;
  uint8_t                    id;
  static ISM330DHCX_Object_t ism330dhcx_obj_0;
  ISM330DHCX_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = ISM330DHCX_I2C_BUS; /* I2C */
  io_ctx.Address     = ISM330DHCX_I2C_ADD_H;
  io_ctx.Init        = IKS02A1_I2C_INIT;
  io_ctx.DeInit      = IKS02A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS02A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS02A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS02A1_GET_TICK;
  io_ctx.Delay       = IKS02A1_DELAY;

  if (ISM330DHCX_RegisterBusIO(&ism330dhcx_obj_0, &io_ctx) != ISM330DHCX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (ISM330DHCX_Set_Mem_Bank(&ism330dhcx_obj_0, ISM330DHCX_USER_BANK) != ISM330DHCX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (ISM330DHCX_ReadID(&ism330dhcx_obj_0, &id) != ISM330DHCX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != ISM330DHCX_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)ISM330DHCX_GetCapabilities(&ism330dhcx_obj_0, &cap);
    MotionCtx[IKS02A1_ISM330DHCX_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS02A1_ISM330DHCX_0] = &ism330dhcx_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS02A1_ISM330DHCX_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&ISM330DHCX_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS02A1_ISM330DHCX_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(void *)&ISM330DHCX_GYRO_Driver;

      if (MotionDrv[IKS02A1_ISM330DHCX_0]->Init(MotionCompObj[IKS02A1_ISM330DHCX_0]) != ISM330DHCX_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS02A1_ISM330DHCX_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                              void *)&ISM330DHCX_ACC_Driver;

      if (MotionDrv[IKS02A1_ISM330DHCX_0]->Init(MotionCompObj[IKS02A1_ISM330DHCX_0]) != ISM330DHCX_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS02A1_MOTION_SENSOR_IIS2DLPC_0  == 1)
/**
  * @brief  Register Bus IOs for instance 0 if component ID is OK
  * @retval BSP status
  */
static int32_t IIS2DLPC_0_Probe(uint32_t Functions)
{
  IIS2DLPC_IO_t            io_ctx;
  uint8_t                  id;
  static IIS2DLPC_Object_t iis2dlpc_obj_0;
  IIS2DLPC_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = IIS2DLPC_I2C_BUS; /* I2C */
  io_ctx.Address     = IIS2DLPC_I2C_ADD_H;
  io_ctx.Init        = IKS02A1_I2C_INIT;
  io_ctx.DeInit      = IKS02A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS02A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS02A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS02A1_GET_TICK;
  io_ctx.Delay       = IKS02A1_DELAY;

  if (IIS2DLPC_RegisterBusIO(&iis2dlpc_obj_0, &io_ctx) != IIS2DLPC_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (IIS2DLPC_ReadID(&iis2dlpc_obj_0, &id) != IIS2DLPC_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != IIS2DLPC_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)IIS2DLPC_GetCapabilities(&iis2dlpc_obj_0, &cap);
    MotionCtx[IKS02A1_IIS2DLPC_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS02A1_IIS2DLPC_0] = &iis2dlpc_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS02A1_IIS2DLPC_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&IIS2DLPC_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS02A1_IIS2DLPC_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                            void *)&IIS2DLPC_ACC_Driver;

      if (MotionDrv[IKS02A1_IIS2DLPC_0]->Init(MotionCompObj[IKS02A1_IIS2DLPC_0]) != IIS2DLPC_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS02A1_MOTION_SENSOR_IIS2MDC_0  == 1)
/**
  * @brief  Register Bus IOs for instance 0 if component ID is OK
  * @retval BSP status
  */
static int32_t IIS2MDC_0_Probe(uint32_t Functions)
{
  IIS2MDC_IO_t            io_ctx;
  uint8_t                 id;
  static IIS2MDC_Object_t iis2mdc_obj_0;
  IIS2MDC_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = IIS2MDC_I2C_BUS; /* I2C */
  io_ctx.Address     = IIS2MDC_I2C_ADD;
  io_ctx.Init        = IKS02A1_I2C_INIT;
  io_ctx.DeInit      = IKS02A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS02A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS02A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS02A1_GET_TICK;
  io_ctx.Delay       = IKS02A1_DELAY;

  if (IIS2MDC_RegisterBusIO(&iis2mdc_obj_0, &io_ctx) != IIS2MDC_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (IIS2MDC_ReadID(&iis2mdc_obj_0, &id) != IIS2MDC_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != IIS2MDC_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)IIS2MDC_GetCapabilities(&iis2mdc_obj_0, &cap);
    MotionCtx[IKS02A1_IIS2MDC_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS02A1_IIS2MDC_0] = &iis2mdc_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS02A1_IIS2MDC_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&IIS2MDC_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS02A1_IIS2MDC_0][FunctionIndex[MOTION_MAGNETO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                          void *)&IIS2MDC_MAG_Driver;

      if (MotionDrv[IKS02A1_IIS2MDC_0]->Init(MotionCompObj[IKS02A1_IIS2MDC_0]) != IIS2MDC_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
  }
  return ret;
}
#endif

#if (USE_IKS02A1_MOTION_SENSOR_IIS2ICLX_0  == 1)
/**
  * @brief  Register Bus IOs for instance 0 if component ID is OK
  * @retval BSP status
  */
static int32_t IIS2ICLX_0_Probe(uint32_t Functions)
{
  IIS2ICLX_IO_t            io_ctx;
  uint8_t                  id;
  static IIS2ICLX_Object_t iis2iclx_obj_0;
  IIS2ICLX_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = IIS2ICLX_I2C_BUS; /* I2C */
  io_ctx.Address     = IIS2ICLX_I2C_ADD_L;
  io_ctx.Init        = IKS02A1_I2C_INIT;
  io_ctx.DeInit      = IKS02A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS02A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS02A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS02A1_GET_TICK;
  io_ctx.Delay       = IKS02A1_DELAY;

  if (IIS2ICLX_RegisterBusIO(&iis2iclx_obj_0, &io_ctx) != IIS2ICLX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (IIS2ICLX_Set_Mem_Bank(&iis2iclx_obj_0, IIS2ICLX_USER_BANK) != IIS2ICLX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (IIS2ICLX_ReadID(&iis2iclx_obj_0, &id) != IIS2ICLX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != IIS2ICLX_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)IIS2ICLX_GetCapabilities(&iis2iclx_obj_0, &cap);
    MotionCtx[IKS02A1_IIS2ICLX_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS02A1_IIS2ICLX_0] = &iis2iclx_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS02A1_IIS2ICLX_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&IIS2ICLX_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS02A1_IIS2ICLX_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                            void *)&IIS2ICLX_ACC_Driver;

      if (MotionDrv[IKS02A1_IIS2ICLX_0]->Init(MotionCompObj[IKS02A1_IIS2ICLX_0]) != IIS2ICLX_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS02A1_MOTION_SENSOR_ASM330LHHX_0  == 1)
/**
  * @brief  Register Bus IOs for instance 0 if component ID is OK
  * @retval BSP status
  */
static int32_t ASM330LHHX_0_Probe(uint32_t Functions)
{
  ASM330LHHX_IO_t            io_ctx;
  uint8_t                    id;
  static ASM330LHHX_Object_t asm330lhhx_obj_0;
  ASM330LHHX_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = ASM330LHHX_I2C_BUS; /* I2C */
  io_ctx.Address     = ASM330LHHX_I2C_ADD_L;
  io_ctx.Init        = IKS02A1_I2C_INIT;
  io_ctx.DeInit      = IKS02A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS02A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS02A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS02A1_GET_TICK;
  io_ctx.Delay       = IKS02A1_DELAY;

  if (ASM330LHHX_RegisterBusIO(&asm330lhhx_obj_0, &io_ctx) != ASM330LHHX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (ASM330LHHX_Set_Mem_Bank(&asm330lhhx_obj_0, ASM330LHHX_USER_BANK) != ASM330LHHX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (ASM330LHHX_ReadID(&asm330lhhx_obj_0, &id) != ASM330LHHX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != ASM330LHHX_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)ASM330LHHX_GetCapabilities(&asm330lhhx_obj_0, &cap);
    MotionCtx[IKS02A1_ASM330LHHX_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS02A1_ASM330LHHX_0] = &asm330lhhx_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS02A1_ASM330LHHX_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&ASM330LHHX_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS02A1_ASM330LHHX_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(void *)&ASM330LHHX_GYRO_Driver;

      if (MotionDrv[IKS02A1_ASM330LHHX_0]->Init(MotionCompObj[IKS02A1_ASM330LHHX_0]) != ASM330LHHX_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS02A1_ASM330LHHX_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                            void *)&ASM330LHHX_ACC_Driver;

      if (MotionDrv[IKS02A1_ASM330LHHX_0]->Init(MotionCompObj[IKS02A1_ASM330LHHX_0]) != ASM330LHHX_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS02A1_MOTION_SENSOR_ISM330BX_0  == 1)
/**
  * @brief  Register Bus IOs for instance 0 if component ID is OK
  * @retval BSP status
  */
static int32_t ISM330BX_0_Probe(uint32_t Functions)
{
  ISM330BX_IO_t            io_ctx;
  uint8_t                  id;
  static ISM330BX_Object_t ism330bx_obj_0;
  ISM330BX_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = ISM330BX_I2C_BUS; /* I2C */
  io_ctx.Address     = ISM330BX_I2C_ADD_L;
  io_ctx.Init        = IKS02A1_I2C_INIT;
  io_ctx.DeInit      = IKS02A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS02A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS02A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS02A1_GET_TICK;
  io_ctx.Delay       = IKS02A1_DELAY;

  if (ISM330BX_RegisterBusIO(&ism330bx_obj_0, &io_ctx) != ISM330BX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (ISM330BX_Set_Mem_Bank(&ism330bx_obj_0, ISM330BX_MAIN_MEM_BANK) != ISM330BX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (ISM330BX_ReadID(&ism330bx_obj_0, &id) != ISM330BX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != ISM330BX_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)ISM330BX_GetCapabilities(&ism330bx_obj_0, &cap);
    MotionCtx[IKS02A1_ISM330BX_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS02A1_ISM330BX_0] = &ism330bx_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS02A1_ISM330BX_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&ISM330BX_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS02A1_ISM330BX_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(void *)&ISM330BX_GYRO_Driver;

      if (MotionDrv[IKS02A1_ISM330BX_0]->Init(MotionCompObj[IKS02A1_ISM330BX_0]) != ISM330BX_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS02A1_ISM330BX_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(void *)&ISM330BX_ACC_Driver;

      if (MotionDrv[IKS02A1_ISM330BX_0]->Init(MotionCompObj[IKS02A1_ISM330BX_0]) != ISM330BX_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

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
