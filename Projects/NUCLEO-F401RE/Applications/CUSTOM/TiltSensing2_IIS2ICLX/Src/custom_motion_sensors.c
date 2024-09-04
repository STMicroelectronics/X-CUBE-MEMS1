/**
  ******************************************************************************
  * @file    custom_motion_sensors.c
  * @author  MEMS Software Solutions Team
  * @brief   This file provides BSP Motion Sensors interface for custom boards
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "custom_motion_sensors.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup CUSTOM CUSTOM
  * @{
  */

/** @defgroup CUSTOM_MOTION_SENSOR CUSTOM MOTION SENSOR
  * @{
  */

/** @defgroup CUSTOM_MOTION_SENSOR_Exported_Variables CUSTOM MOTION SENSOR Exported Variables
  * @{
  */

extern void
*MotionCompObj[CUSTOM_MOTION_INSTANCES_NBR]; /* This "redundant" line is here to fulfil MISRA C-2012 rule 8.4 */
void *MotionCompObj[CUSTOM_MOTION_INSTANCES_NBR];

/**
  * @}
  */

/** @defgroup CUSTOM_MOTION_SENSOR_Private_Variables CUSTOM MOTION SENSOR Private Variables
  * @{
  */

/* We define a jump table in order to get the correct index from the desired function. */
/* This table should have a size equal to the maximum value of a function plus 1.      */
/* But due to MISRA it has to be increased to 7 + 1. */
static uint32_t FunctionIndex[] = {0, 0, 1, 1, 2, 2, 2, 2};
static MOTION_SENSOR_FuncDrv_t *MotionFuncDrv[CUSTOM_MOTION_INSTANCES_NBR][CUSTOM_MOTION_FUNCTIONS_NBR];
static MOTION_SENSOR_CommonDrv_t *MotionDrv[CUSTOM_MOTION_INSTANCES_NBR];
static CUSTOM_MOTION_SENSOR_Ctx_t MotionCtx[CUSTOM_MOTION_INSTANCES_NBR];

/**
  * @}
  */

/** @defgroup CUSTOM_MOTION_SENSOR_Private_Function_Prototypes CUSTOM MOTION SENSOR Private Function Prototypes
  * @{
  */

#if (USE_CUSTOM_MOTION_SENSOR_IIS2ICLX_0 == 1)
static int32_t IIS2ICLX_0_Probe(uint32_t Functions);
#endif

/**
  * @}
  */

/** @defgroup CUSTOM_MOTION_SENSOR_Exported_Functions CUSTOM MOTION SENSOR Exported Functions
  * @{
  */

/**
  * @brief  Initializes the motion sensors
  * @param  Instance Motion sensor instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @retval BSP status
  */
int32_t CUSTOM_MOTION_SENSOR_Init(uint32_t Instance, uint32_t Functions)
{
  int32_t ret = BSP_ERROR_NONE;
  uint32_t function = MOTION_GYRO;
  uint32_t i;
  uint32_t component_functions = 0;
  CUSTOM_MOTION_SENSOR_Capabilities_t cap;

  switch (Instance)
  {
#if (USE_CUSTOM_MOTION_SENSOR_IIS2ICLX_0 == 1)
    case CUSTOM_IIS2ICLX_0:
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
    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  if (ret != BSP_ERROR_NONE)
  {
    return ret;
  }

  for (i = 0; i < CUSTOM_MOTION_FUNCTIONS_NBR; i++)
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
int32_t CUSTOM_MOTION_SENSOR_DeInit(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= CUSTOM_MOTION_INSTANCES_NBR)
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
int32_t CUSTOM_MOTION_SENSOR_GetCapabilities(uint32_t Instance, CUSTOM_MOTION_SENSOR_Capabilities_t *Capabilities)
{
  int32_t ret;

  if (Instance >= CUSTOM_MOTION_INSTANCES_NBR)
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
int32_t CUSTOM_MOTION_SENSOR_ReadID(uint32_t Instance, uint8_t *Id)
{
  int32_t ret;

  if (Instance >= CUSTOM_MOTION_INSTANCES_NBR)
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
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @retval BSP status
  */
int32_t CUSTOM_MOTION_SENSOR_Enable(uint32_t Instance, uint32_t Function)
{
  int32_t ret;

  if (Instance >= CUSTOM_MOTION_INSTANCES_NBR)
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
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @retval BSP status
  */
int32_t CUSTOM_MOTION_SENSOR_Disable(uint32_t Instance, uint32_t Function)
{
  int32_t ret;

  if (Instance >= CUSTOM_MOTION_INSTANCES_NBR)
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
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @param  Axes pointer to axes data structure
  * @retval BSP status
  */
int32_t CUSTOM_MOTION_SENSOR_GetAxes(uint32_t Instance, uint32_t Function, CUSTOM_MOTION_SENSOR_Axes_t *Axes)
{
  int32_t ret;

  if (Instance >= CUSTOM_MOTION_INSTANCES_NBR)
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
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @param  Axes pointer to axes raw data structure
  * @retval BSP status
  */
int32_t CUSTOM_MOTION_SENSOR_GetAxesRaw(uint32_t Instance, uint32_t Function, CUSTOM_MOTION_SENSOR_AxesRaw_t *Axes)
{
  int32_t ret;

  if (Instance >= CUSTOM_MOTION_INSTANCES_NBR)
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
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @param  Sensitivity pointer to sensitivity read value
  * @retval BSP status
  */
int32_t CUSTOM_MOTION_SENSOR_GetSensitivity(uint32_t Instance, uint32_t Function, float_t *Sensitivity)
{
  int32_t ret;

  if (Instance >= CUSTOM_MOTION_INSTANCES_NBR)
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
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @param  Odr pointer to Output Data Rate read value
  * @retval BSP status
  */
int32_t CUSTOM_MOTION_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float_t *Odr)
{
  int32_t ret;

  if (Instance >= CUSTOM_MOTION_INSTANCES_NBR)
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
int32_t CUSTOM_MOTION_SENSOR_GetFullScale(uint32_t Instance, uint32_t Function, int32_t *Fullscale)
{
  int32_t ret;

  if (Instance >= CUSTOM_MOTION_INSTANCES_NBR)
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
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @param  Odr Output Data Rate value to be set
  * @retval BSP status
  */
int32_t CUSTOM_MOTION_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float_t Odr)
{
  int32_t ret;

  if (Instance >= CUSTOM_MOTION_INSTANCES_NBR)
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
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @param  Fullscale Fullscale value to be set
  * @retval BSP status
  */
int32_t CUSTOM_MOTION_SENSOR_SetFullScale(uint32_t Instance, uint32_t Function, int32_t Fullscale)
{
  int32_t ret;

  if (Instance >= CUSTOM_MOTION_INSTANCES_NBR)
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

/** @defgroup CUSTOM_MOTION_SENSOR_Private_Functions CUSTOM MOTION SENSOR Private Functions
  * @{
  */

#if (USE_CUSTOM_MOTION_SENSOR_IIS2ICLX_0 == 1)
/**
  * @brief  Register Bus IOs for IIS2ICLX instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t IIS2ICLX_0_Probe(uint32_t Functions)
{
  IIS2ICLX_IO_t            io_ctx;
  uint8_t                  id;
  static IIS2ICLX_Object_t iis2iclx_obj_0;
  IIS2ICLX_Capabilities_t  cap;
  int32_t                  ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = IIS2ICLX_I2C_BUS; /* I2C */
  io_ctx.Address     = IIS2ICLX_I2C_ADD_L; /* SA0 = GND */
  io_ctx.Init        = CUSTOM_IIS2ICLX_0_I2C_INIT;
  io_ctx.DeInit      = CUSTOM_IIS2ICLX_0_I2C_DEINIT;
  io_ctx.ReadReg     = CUSTOM_IIS2ICLX_0_I2C_READ_REG;
  io_ctx.WriteReg    = CUSTOM_IIS2ICLX_0_I2C_WRITE_REG;
  io_ctx.GetTick     = BSP_GetTick;
  io_ctx.Delay       = HAL_Delay;

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
  else if (id != (uint8_t)IIS2ICLX_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)IIS2ICLX_GetCapabilities(&iis2iclx_obj_0, &cap);
    MotionCtx[CUSTOM_IIS2ICLX_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[CUSTOM_IIS2ICLX_0] = &iis2iclx_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[CUSTOM_IIS2ICLX_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&IIS2ICLX_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[CUSTOM_IIS2ICLX_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                          void *)&IIS2ICLX_ACC_Driver;

      if (MotionDrv[CUSTOM_IIS2ICLX_0]->Init(MotionCompObj[CUSTOM_IIS2ICLX_0]) != IIS2ICLX_OK)
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
