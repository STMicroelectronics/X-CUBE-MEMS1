/**
 ******************************************************************************
 * @file    iks01a3_hybrid_sensors.c
 * @author  MEMS Software Solutions Team
 * @brief   This file provides a set of functions needed to manage the hybrid
 *          sensors
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
#include "iks01a3_hybrid_sensors.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup IKS01A3 IKS01A3
 * @{
 */

/** @defgroup IKS01A3_HYBRID_SENSORS IKS01A3 HYBRID SENSORS
 * @{
 */

/** @defgroup IKS01A3_HYBRID_SENSORS_Exported_Variables IKS01A3 HYBRID SENSORS Exported Variables
 * @{
 */

void *HybridCompObj[IKS01A3_HYBRID_INSTANCES_NBR];

/**
 * @}
 */

/** @defgroup IKS01A3_HYBRID_SENSORS_Private_Variables IKS01A3 HYBRID SENSORS Private Variables
 * @{
 */

static HYBRID_ENV_SENSOR_FuncDrv_t        *HybridEnvFuncDrv[IKS01A3_HYBRID_INSTANCES_NBR][IKS01A3_HYBRID_ENV_FUNCTIONS_NBR];
static HYBRID_MOTION_SENSOR_FuncDrv_t     *HybridMotionFuncDrv[IKS01A3_HYBRID_INSTANCES_NBR][IKS01A3_HYBRID_MOTION_FUNCTIONS_NBR];
static HYBRID_SENSOR_CommonDrv_t          *HybridDrv[IKS01A3_HYBRID_INSTANCES_NBR];
static IKS01A3_HYBRID_ENV_SENSOR_Ctx_t     HybridEnvCtx[IKS01A3_HYBRID_INSTANCES_NBR];
static IKS01A3_HYBRID_MOTION_SENSOR_Ctx_t  HybridMotionCtx[IKS01A3_HYBRID_INSTANCES_NBR];

/**
 * @}
 */

/** @defgroup IKS01A3_HYBRID_SENSORS_Private_Function_Prototypes IKS01A3 HYBRID SENSORS Private Function Prototypes
 * @{
 */

#if (USE_IKS01A3_HYBRID_SENSOR_LIS2DTW12_0 == 1)
static int32_t LIS2DTW12_0_Probe(uint32_t Functions);
#endif

/**
 * @}
 */

/** @defgroup IKS01A3_HYBRID_SENSORS_Exported_Functions IKS01A3 HYBRID SENSORS Exported Functions
 * @{
 */

/**
 * @brief  Initializes the hybrid sensor
 * @param  Instance Hybrid sensor instance
 * @param  Functions Hybrid sensor functions. Must be combination of motion function:
 *         - HYBRID_ACCELERO
 *         - HYBRID_GYRO
 *         - HYBRID_MAGNETO
 *
 *         and environmental function:
 *         - HYBRID_HUMIDITY
 *         - HYBRID_PRESSURE
 *         - HYBRID_TEMPERATURE
 *
 * @retval BSP status
 */
int32_t IKS01A3_HYBRID_SENSOR_Init(uint32_t Instance, uint32_t Functions)
{
  int32_t ret = BSP_ERROR_NONE;
  uint32_t i;
  uint32_t function = 0;
  uint32_t component_functions = 0;
  IKS01A3_HYBRID_SENSOR_Capabilities_t cap;

  switch (Instance)
  {
#if (USE_IKS01A3_HYBRID_SENSOR_LIS2DTW12_0 == 1)
    case IKS01A3_LIS2DTW12_0:
      if (LIS2DTW12_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (HybridDrv[Instance]->GetCapabilities(HybridCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      component_functions = ((cap.Acc         == 1U) ? HYBRID_ACCELERO    : 0)
                          | ((cap.Gyro        == 1U) ? HYBRID_GYRO        : 0)
                          | ((cap.Magneto     == 1U) ? HYBRID_MAGNETO     : 0)
                          | ((cap.Temperature == 1U) ? HYBRID_TEMPERATURE : 0)
                          | ((cap.Humidity    == 1U) ? HYBRID_HUMIDITY    : 0)
                          | ((cap.Pressure    == 1U) ? HYBRID_PRESSURE    : 0);
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

  for (i = 0; i < IKS01A3_HYBRID_ENV_FUNCTIONS_NBR; i++)
  {
    function = HYBRID_TEMPERATURE << i;
    if ((Functions & component_functions & function) == function)
    {
      if (HybridEnvFuncDrv[Instance][i]->Enable(HybridCompObj[Instance]) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_COMPONENT_FAILURE;
      }
    }
  }

  for (i = 0; i < IKS01A3_HYBRID_MOTION_FUNCTIONS_NBR; i++)
  {
    function = HYBRID_GYRO << i;
    if ((Functions & component_functions & function) == function)
    {
      if (HybridMotionFuncDrv[Instance][i]->Enable(HybridCompObj[Instance]) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_COMPONENT_FAILURE;
      }
    }
  }

  return ret;
}


/**
 * @brief  Deinitialize hybrid sensor
 * @param  Instance Hybrid sensor instance
 * @retval BSP status
 */
int32_t IKS01A3_HYBRID_SENSOR_DeInit(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= IKS01A3_HYBRID_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (HybridDrv[Instance]->DeInit(HybridCompObj[Instance]) != BSP_ERROR_NONE)
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
 * @brief  Get hybrid sensor instance capabilities
 * @param  Instance Hybrid sensor instance
 * @param  Capabilities pointer to hybrid sensor capabilities
 * @retval BSP status
 */
int32_t IKS01A3_HYBRID_SENSOR_GetCapabilities(uint32_t Instance, IKS01A3_HYBRID_SENSOR_Capabilities_t *Capabilities)
{
  int32_t ret;

  if (Instance >= IKS01A3_HYBRID_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (HybridDrv[Instance]->GetCapabilities(HybridCompObj[Instance], Capabilities) != BSP_ERROR_NONE)
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
 * @param  Instance Hybrid sensor instance
 * @param  Id WHOAMI value
 * @retval BSP status
 */
int32_t IKS01A3_HYBRID_SENSOR_ReadID(uint32_t Instance, uint8_t *Id)
{
  int32_t ret;

  if (Instance >= IKS01A3_HYBRID_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (HybridDrv[Instance]->ReadID(HybridCompObj[Instance], Id) != BSP_ERROR_NONE)
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
 * @brief  Enable Hybrid sensor - motion part
 * @param  Instance Hybrid sensor instance
 * @param  Functions Hybrid sensor motion functions. Could be :
 *         - HYBRID_ACCELERO
 *         - HYBRID_GYRO
 *         - HYBRID_MAGNETO
 * @retval BSP status
 */
int32_t IKS01A3_HYBRID_MOTION_SENSOR_Enable(uint32_t Instance, uint32_t Function)
{
  if (Instance >= IKS01A3_HYBRID_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if ((HybridMotionCtx[Instance].Functions & HYBRID_MOTION_FUNC & Function) != Function)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (HybridMotionFuncDrv[Instance][HYBRID_MOTION_FUNC_ID(Function)]->Enable(HybridCompObj[Instance]) != BSP_ERROR_NONE)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}


/**
 * @brief  Disable Hybrid sensor - motion part
 * @param  Instance Hybrid sensor instance
 * @param  Functions Hybrid sensor motion functions. Could be :
 *         - HYBRID_ACCELERO
 *         - HYBRID_GYRO
 *         - HYBRID_MAGNETO
 * @retval BSP status
 */
int32_t IKS01A3_HYBRID_MOTION_SENSOR_Disable(uint32_t Instance, uint32_t Function)
{
  if (Instance >= IKS01A3_HYBRID_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if ((HybridMotionCtx[Instance].Functions & HYBRID_MOTION_FUNC & Function) != Function)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (HybridMotionFuncDrv[Instance][HYBRID_MOTION_FUNC_ID(Function)]->Disable(HybridCompObj[Instance]) != BSP_ERROR_NONE)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}


/**
 * @brief  Get hybrid sensor axes data - motion part only
 * @param  Instance Hybrid sensor instance
 * @param  Functions Hybrid sensor motion functions. Could be :
 *         - HYBRID_ACCELERO
 *         - HYBRID_GYRO
 *         - HYBRID_MAGNETO
 * @param  Axes pointer to axes data structure
 * @retval BSP status
 */
int32_t IKS01A3_HYBRID_MOTION_SENSOR_GetAxes(uint32_t Instance, uint32_t Function, IKS01A3_HYBRID_MOTION_SENSOR_Axes_t *Axes)
{
  if (Instance >= IKS01A3_HYBRID_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if ((HybridMotionCtx[Instance].Functions & HYBRID_MOTION_FUNC & Function) != Function)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (HybridMotionFuncDrv[Instance][HYBRID_MOTION_FUNC_ID(Function)]->GetAxes(HybridCompObj[Instance], Axes) != BSP_ERROR_NONE)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}


/**
 * @brief  Get hybrid sensor axes raw data - motion part only
 * @param  Instance Hybrid sensor instance
 * @param  Functions Hybrid sensor motion functions. Could be :
 *         - HYBRID_ACCELERO
 *         - HYBRID_GYRO
 *         - HYBRID_MAGNETO
 * @param  Axes pointer to axes raw data structure
 * @retval BSP status
 */
int32_t IKS01A3_HYBRID_MOTION_SENSOR_GetAxesRaw(uint32_t Instance, uint32_t Function, IKS01A3_HYBRID_MOTION_SENSOR_AxesRaw_t *Axes)
{
  if (Instance >= IKS01A3_HYBRID_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if ((HybridMotionCtx[Instance].Functions & HYBRID_MOTION_FUNC & Function) != Function)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (HybridMotionFuncDrv[Instance][HYBRID_MOTION_FUNC_ID(Function)]->GetAxesRaw(HybridCompObj[Instance], Axes) != BSP_ERROR_NONE)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}


/**
 * @brief  Get hybrid sensor sensitivity - motion part only
 * @param  Instance Hybrid sensor instance
 * @param  Functions Hybrid sensor motion functions. Could be :
 *         - HYBRID_ACCELERO
 *         - HYBRID_GYRO
 *         - HYBRID_MAGNETO
 * @param  Sensitivity pointer to sensitivity read value
 * @retval BSP status
 */
int32_t IKS01A3_HYBRID_MOTION_SENSOR_GetSensitivity(uint32_t Instance, uint32_t Function, float *Sensitivity)
{
  if (Instance >= IKS01A3_HYBRID_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if ((HybridMotionCtx[Instance].Functions & HYBRID_MOTION_FUNC & Function) != Function)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (HybridMotionFuncDrv[Instance][HYBRID_MOTION_FUNC_ID(Function)]->GetSensitivity(HybridCompObj[Instance], Sensitivity) != BSP_ERROR_NONE)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}


/**
 * @brief  Get hybrid sensor Output Data Rate - motion part
 * @param  Instance Hybrid sensor instance
 * @param  Functions Hybrid sensor motion functions. Could be :
 *         - HYBRID_ACCELERO
 *         - HYBRID_GYRO
 *         - HYBRID_MAGNETO
 * @param  Odr pointer to Output Data Rate read value
 * @retval BSP status
 */
int32_t IKS01A3_HYBRID_MOTION_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float *Odr)
{
  if (Instance >= IKS01A3_HYBRID_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if ((HybridMotionCtx[Instance].Functions & HYBRID_MOTION_FUNC & Function) != Function)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (HybridMotionFuncDrv[Instance][HYBRID_MOTION_FUNC_ID(Function)]->GetOutputDataRate(HybridCompObj[Instance], Odr) != BSP_ERROR_NONE)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}


/**
 * @brief  Set hybrid sensor Output Data Rate - motion part
 * @param  Instance Hybrid sensor instance
 * @param  Functions Hybrid sensor motion functions. Could be :
 *         - HYBRID_ACCELERO
 *         - HYBRID_GYRO
 *         - HYBRID_MAGNETO
 * @param  Odr Output Data Rate value to be set
 * @retval BSP status
 */
int32_t IKS01A3_HYBRID_MOTION_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float Odr)
{
  if (Instance >= IKS01A3_HYBRID_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if ((HybridMotionCtx[Instance].Functions & HYBRID_MOTION_FUNC & Function) != Function)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (HybridMotionFuncDrv[Instance][HYBRID_MOTION_FUNC_ID(Function)]->SetOutputDataRate(HybridCompObj[Instance], Odr) != BSP_ERROR_NONE)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}


/**
 * @brief  Get hybrid sensor Full Scale - motion part only
 * @param  Instance Hybrid sensor instance
 * @param  Functions Hybrid sensor motion functions. Could be :
 *         - HYBRID_ACCELERO
 *         - HYBRID_GYRO
 *         - HYBRID_MAGNETO
 * @param  Fullscale pointer to Fullscale read value
 * @retval BSP status
 */
int32_t IKS01A3_HYBRID_MOTION_SENSOR_GetFullScale(uint32_t Instance, uint32_t Function, int32_t *Fullscale)
{
  if (Instance >= IKS01A3_HYBRID_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if ((HybridMotionCtx[Instance].Functions & HYBRID_MOTION_FUNC & Function) != Function)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (HybridMotionFuncDrv[Instance][HYBRID_MOTION_FUNC_ID(Function)]->GetFullScale(HybridCompObj[Instance], Fullscale) != BSP_ERROR_NONE)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}


/**
 * @brief  Set hybrid sensor Full Scale - motion part only
 * @param  Instance Hybrid sensor instance
 * @param  Functions Hybrid sensor motion functions. Could be :
 *         - HYBRID_ACCELERO
 *         - HYBRID_GYRO
 *         - HYBRID_MAGNETO
 * @param  Fullscale Fullscale value to be set
 * @retval BSP status
 */
int32_t IKS01A3_HYBRID_MOTION_SENSOR_SetFullScale(uint32_t Instance, uint32_t Function, int32_t Fullscale)
{
  if (Instance >= IKS01A3_HYBRID_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if ((HybridMotionCtx[Instance].Functions & HYBRID_MOTION_FUNC & Function) != Function)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (HybridMotionFuncDrv[Instance][HYBRID_MOTION_FUNC_ID(Function)]->SetFullScale(HybridCompObj[Instance], Fullscale) != BSP_ERROR_NONE)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}


/**
 * @brief  Enable Hybrid sensor - environmental part
 * @param  Instance Hybrid sensor instance
 * @param  Functions Hybrid sensor environmental functions. Could be :
 *         - HYBRID_HUMIDITY
 *         - HYBRID_PRESSURE
 *         - HYBRID_TEMPERATURE
 * @retval BSP status
 */
int32_t IKS01A3_HYBRID_ENV_SENSOR_Enable(uint32_t Instance, uint32_t Function)
{
  if (Instance >= IKS01A3_HYBRID_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if ((HybridEnvCtx[Instance].Functions & HYBRID_ENV_FUNC & Function) != Function)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (HybridEnvFuncDrv[Instance][HYBRID_ENV_FUNC_ID(Function)]->Enable(HybridCompObj[Instance]) != BSP_ERROR_NONE)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}


/**
 * @brief  Disable Hybrid sensor - environmental part
 * @param  Instance Hybrid sensor instance
 * @param  Functions Hybrid sensor environmental functions. Could be :
 *         - HYBRID_HUMIDITY
 *         - HYBRID_PRESSURE
 *         - HYBRID_TEMPERATURE
 * @retval BSP status
 */
int32_t IKS01A3_HYBRID_ENV_SENSOR_Disable(uint32_t Instance, uint32_t Function)
{
  if (Instance >= IKS01A3_HYBRID_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if ((HybridEnvCtx[Instance].Functions & HYBRID_ENV_FUNC & Function) != Function)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (HybridEnvFuncDrv[Instance][HYBRID_ENV_FUNC_ID(Function)]->Disable(HybridCompObj[Instance]) != BSP_ERROR_NONE)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}


/**
 * @brief  Get hybrid sensor Output Data Rate - environmental part
 * @param  Instance Hybrid sensor instance
 * @param  Functions Hybrid sensor environmental functions. Could be :
 *         - HYBRID_HUMIDITY
 *         - HYBRID_PRESSURE
 *         - HYBRID_TEMPERATURE
 * @param  Odr pointer to Output Data Rate read value
 * @retval BSP status
 */
int32_t IKS01A3_HYBRID_ENV_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float *Odr)
{
  if (Instance >= IKS01A3_HYBRID_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if ((HybridEnvCtx[Instance].Functions & HYBRID_ENV_FUNC & Function) != Function)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (HybridEnvFuncDrv[Instance][HYBRID_ENV_FUNC_ID(Function)]->GetOutputDataRate(HybridCompObj[Instance], Odr) != BSP_ERROR_NONE)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}


/**
 * @brief  Set hybrid sensor Output Data Rate - environmental part
 * @param  Instance Hybrid sensor instance
 * @param  Functions Hybrid sensor environmental functions. Could be :
 *         - HYBRID_HUMIDITY
 *         - HYBRID_PRESSURE
 *         - HYBRID_TEMPERATURE
 * @param  Odr Output Data Rate value to be set
 * @retval BSP status
 */
int32_t IKS01A3_HYBRID_ENV_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float Odr)
{
  if (Instance >= IKS01A3_HYBRID_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if ((HybridEnvCtx[Instance].Functions & HYBRID_ENV_FUNC & Function) != Function)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (HybridEnvFuncDrv[Instance][HYBRID_ENV_FUNC_ID(Function)]->SetOutputDataRate(HybridCompObj[Instance], Odr) != BSP_ERROR_NONE)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}


/**
 * @brief  Get hybrid sensor value - environmental part only
 * @param  Instance hybrid sensor instance
 * @param  Functions Hybrid sensor environmental functions. Could be :
 *         - HYBRID_HUMIDITY
 *         - HYBRID_PRESSURE
 *         - HYBRID_TEMPERATURE
 * @param  Value pointer to hybrid sensor value
 * @retval BSP status
 */
int32_t IKS01A3_HYBRID_ENV_SENSOR_GetValue(uint32_t Instance, uint32_t Function, float *Value)
{
  if (Instance >= IKS01A3_HYBRID_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if ((HybridEnvCtx[Instance].Functions & HYBRID_ENV_FUNC & Function) != Function)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (HybridEnvFuncDrv[Instance][HYBRID_ENV_FUNC_ID(Function)]->GetValue(HybridCompObj[Instance], Value) != BSP_ERROR_NONE)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}


/**
 * @}
 */

/** @defgroup IKS01A3_HYBRID_SENSORS_Private_Functions IKS01A3 HYBRID SENSORS Private Functions
 * @{
 */

#if (USE_IKS01A3_HYBRID_SENSOR_LIS2DTW12_0  == 1)
/**
 * @brief  Register Bus IOs for instance if component ID is OK
 * @retval BSP status
 */
static int32_t LIS2DTW12_0_Probe(uint32_t Functions)
{
  LIS2DTW12_IO_t            io_ctx;
  uint8_t                   id;
  static LIS2DTW12_Object_t lis2dtw12_obj_0;
  LIS2DTW12_Capabilities_t  cap;

  /* Configure the hybrid sensor driver */
  io_ctx.BusType  = LIS2DTW12_I2C_BUS; /* I2C */
  io_ctx.Address  = LIS2DTW12_I2C_ADD_L;
  io_ctx.Init     = IKS01A3_I2C_Init;
  io_ctx.DeInit   = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg  = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick  = IKS01A3_GetTick;

  if (LIS2DTW12_RegisterBusIO(&lis2dtw12_obj_0, &io_ctx) != LIS2DTW12_OK)
  {
    return BSP_ERROR_UNKNOWN_COMPONENT;
  }

  if (LIS2DTW12_ReadID(&lis2dtw12_obj_0, &id) != LIS2DTW12_OK)
  {
    return BSP_ERROR_UNKNOWN_COMPONENT;
  }

  if (id != LIS2DTW12_ID)
  {
    return BSP_ERROR_UNKNOWN_COMPONENT;
  }

  if (LIS2DTW12_GetCapabilities(&lis2dtw12_obj_0, &cap) != LIS2DTW12_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  HybridMotionCtx[IKS01A3_LIS2DTW12_0].Functions = ((cap.Acc     == 1U) ? HYBRID_ACCELERO : 0)
                                                 | ((cap.Gyro    == 1U) ? HYBRID_GYRO     : 0)
                                                 | ((cap.Magneto == 1U) ? HYBRID_MAGNETO  : 0);

  HybridEnvCtx[IKS01A3_LIS2DTW12_0].Functions = ((cap.Temperature == 1U) ? HYBRID_TEMPERATURE : 0)
                                              | ((cap.Humidity    == 1U) ? HYBRID_HUMIDITY    : 0)
                                              | ((cap.Pressure    == 1U) ? HYBRID_PRESSURE    : 0);

  HybridCompObj[IKS01A3_LIS2DTW12_0] = &lis2dtw12_obj_0;

  /* The second cast (void *) is added to bypass Misra R11.3 rule */
  HybridDrv[IKS01A3_LIS2DTW12_0] = (HYBRID_SENSOR_CommonDrv_t *)(void *)&LIS2DTW12_COMMON_Driver;

  if ((Functions & HybridMotionCtx[IKS01A3_LIS2DTW12_0].Functions & HYBRID_ACCELERO) == HYBRID_ACCELERO)
  {
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    HybridMotionFuncDrv[IKS01A3_LIS2DTW12_0][HYBRID_MOTION_FUNC_ID(HYBRID_ACCELERO)] = (HYBRID_MOTION_SENSOR_FuncDrv_t *)(void *)&LIS2DTW12_ACC_Driver;

    if (HybridDrv[IKS01A3_LIS2DTW12_0]->Init(HybridCompObj[IKS01A3_LIS2DTW12_0]) != LIS2DTW12_OK)
    {
      return BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  if ((Functions & HybridMotionCtx[IKS01A3_LIS2DTW12_0].Functions & HYBRID_GYRO) == HYBRID_GYRO)
  {
    /* Function not supported by the component */
    return BSP_ERROR_WRONG_PARAM;
  }

  if ((Functions & HybridMotionCtx[IKS01A3_LIS2DTW12_0].Functions & HYBRID_MAGNETO) == HYBRID_MAGNETO)
  {
    /* Function not supported by the component */
    return BSP_ERROR_WRONG_PARAM;
  }

  if ((Functions & HybridEnvCtx[IKS01A3_LIS2DTW12_0].Functions & HYBRID_TEMPERATURE) == HYBRID_TEMPERATURE)
  {
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    HybridEnvFuncDrv[IKS01A3_LIS2DTW12_0][HYBRID_ENV_FUNC_ID(HYBRID_TEMPERATURE)] = (HYBRID_ENV_SENSOR_FuncDrv_t *)(void *)&LIS2DTW12_TEMP_Driver;

    if (HybridDrv[IKS01A3_LIS2DTW12_0]->Init(HybridCompObj[IKS01A3_LIS2DTW12_0]) != LIS2DTW12_OK)
    {
      return BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  if ((Functions & HybridEnvCtx[IKS01A3_LIS2DTW12_0].Functions & HYBRID_HUMIDITY) == HYBRID_HUMIDITY)
  {
    /* Function not supported by the component */
    return BSP_ERROR_WRONG_PARAM;
  }

  if ((Functions & HybridEnvCtx[IKS01A3_LIS2DTW12_0].Functions & HYBRID_PRESSURE) == HYBRID_PRESSURE)
  {
    /* Function not supported by the component */
    return BSP_ERROR_WRONG_PARAM;
  }

  return BSP_ERROR_NONE;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
