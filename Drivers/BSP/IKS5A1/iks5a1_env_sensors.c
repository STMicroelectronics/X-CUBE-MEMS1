/**
  ******************************************************************************
  * @file    iks5a1_env_sensors.c
  * @author  MEMS Software Solutions Team
  * @brief   This file provides a set of functions needed to manage the
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
#include "iks5a1_env_sensors.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup IKS5A1 IKS5A1
  * @{
  */

/** @defgroup IKS5A1_ENV_SENSORS IKS5A1 ENV SENSORS
  * @{
  */

/** @defgroup IKS5A1_ENV_SENSORS_Exported_Variables IKS5A1 ENV SENSORS Exported Variables
  * @{
  */

extern void *EnvCompObj[IKS5A1_ENV_INSTANCES_NBR]; /* This "redundant" line is here
                                                      to fulfil MISRA C-2012 rule 8.4 */
void *EnvCompObj[IKS5A1_ENV_INSTANCES_NBR];

/**
  * @}
  */

/** @defgroup IKS5A1_ENV_SENSORS_Private_Variables IKS5A1 ENV SENSORS Private Variables
  * @{
  */

/* We define a jump table in order to get the correct index from the desired function. */
/* This table should have a size equal to the maximum value of a function plus 1.      */
/* But due to MISRA it has to be increased to 15 + 1. */
static uint32_t FunctionIndex[] = {0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3};
static ENV_SENSOR_FuncDrv_t *EnvFuncDrv[IKS5A1_ENV_INSTANCES_NBR][IKS5A1_ENV_FUNCTIONS_NBR];
static ENV_SENSOR_CommonDrv_t *EnvDrv[IKS5A1_ENV_INSTANCES_NBR];
static IKS5A1_ENV_SENSOR_Ctx_t EnvCtx[IKS5A1_ENV_INSTANCES_NBR];

/**
  * @}
  */

/** @defgroup IKS5A1_ENV_SENSORS_Private_Function_Prototypes IKS5A1 ENV SENSORS Private Function Prototypes
  * @{
  */

#if (USE_IKS5A1_ENV_SENSOR_ILPS22QS_0 == 1)
static int32_t ILPS22QS_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS5A1_ENV_SENSOR_SHT40AD1B_0 == 1)
static int32_t SHT40AD1B_0_Probe(uint32_t Functions);
#endif

/**
  * @}
  */

/** @defgroup IKS5A1_ENV_SENSORS_Exported_Functions IKS5A1 ENV SENSOR Exported Functions
  * @{
  */

/**
  * @brief  Initializes the environmental sensor
  * @param  Instance environmental sensor instance to be used
  * @param  Functions Environmental sensor functions. Could be :
  *         - ENV_TEMPERATURE
  *         - ENV_PRESSURE
  *         - ENV_HUMIDITY
  * @retval BSP status
  */
int32_t IKS5A1_ENV_SENSOR_Init(uint32_t Instance, uint32_t Functions)
{
  int32_t ret = BSP_ERROR_NONE;
  uint32_t function = ENV_TEMPERATURE;
  uint32_t i;
  uint32_t component_functions = 0;
  IKS5A1_ENV_SENSOR_Capabilities_t cap;

  switch (Instance)
  {
#if (USE_IKS5A1_ENV_SENSOR_ILPS22QS_0 == 1)
    case IKS5A1_ILPS22QS_0:
      if (ILPS22QS_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (EnvDrv[Instance]->GetCapabilities(EnvCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Temperature == 1U)
      {
        component_functions |= ENV_TEMPERATURE;
      }
      if (cap.Humidity == 1U)
      {
        component_functions |= ENV_HUMIDITY;
      }
      if (cap.Pressure == 1U)
      {
        component_functions |= ENV_PRESSURE;
      }
      if (cap.Gas == 1U)
      {
        component_functions |= ENV_GAS;
      }
      break;
#endif

#if (USE_IKS5A1_ENV_SENSOR_SHT40AD1B_0 == 1)
    case IKS5A1_SHT40AD1B_0:
      if (SHT40AD1B_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (EnvDrv[Instance]->GetCapabilities(EnvCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Temperature == 1U)
      {
        component_functions |= ENV_TEMPERATURE;
      }
      if (cap.Humidity == 1U)
      {
        component_functions |= ENV_HUMIDITY;
      }
      if (cap.Pressure == 1U)
      {
        component_functions |= ENV_PRESSURE;
      }
      if (cap.Gas == 1U)
      {
        component_functions |= ENV_GAS;
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

  for (i = 0; i < IKS5A1_ENV_FUNCTIONS_NBR; i++)
  {
    if (((Functions & function) == function) && ((component_functions & function) == function))
    {
      if (EnvFuncDrv[Instance][FunctionIndex[function]]->Enable(EnvCompObj[Instance]) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_COMPONENT_FAILURE;
      }
    }
    function = function << 1;
  }

  return ret;
}

/**
  * @brief  Deinitialize environmental sensor
  * @param  Instance environmental sensor instance to be used
  * @retval BSP status
  */
int32_t IKS5A1_ENV_SENSOR_DeInit(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= IKS5A1_ENV_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (EnvDrv[Instance]->DeInit(EnvCompObj[Instance]) != BSP_ERROR_NONE)
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
  * @brief  Get environmental sensor instance capabilities
  * @param  Instance Environmental sensor instance
  * @param  Capabilities pointer to Environmental sensor capabilities
  * @retval BSP status
  */
int32_t IKS5A1_ENV_SENSOR_GetCapabilities(uint32_t Instance, IKS5A1_ENV_SENSOR_Capabilities_t *Capabilities)
{
  int32_t ret;

  if (Instance >= IKS5A1_ENV_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (EnvDrv[Instance]->GetCapabilities(EnvCompObj[Instance], Capabilities) != BSP_ERROR_NONE)
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
  * @param  Instance environmental sensor instance to be used
  * @param  Id WHOAMI value
  * @retval BSP status
  */
int32_t IKS5A1_ENV_SENSOR_ReadID(uint32_t Instance, uint8_t *Id)
{
  int32_t ret;

  if (Instance >= IKS5A1_ENV_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (EnvDrv[Instance]->ReadID(EnvCompObj[Instance], Id) != BSP_ERROR_NONE)
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
  * @brief  Enable environmental sensor
  * @param  Instance environmental sensor instance to be used
  * @param  Function Environmental sensor function. Could be :
  *         - ENV_TEMPERATURE
  *         - ENV_PRESSURE
  *         - ENV_HUMIDITY
  * @retval BSP status
  */
int32_t IKS5A1_ENV_SENSOR_Enable(uint32_t Instance, uint32_t Function)
{
  int32_t ret;

  if (Instance >= IKS5A1_ENV_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((EnvCtx[Instance].Functions & Function) == Function)
    {
      if (EnvFuncDrv[Instance][FunctionIndex[Function]]->Enable(EnvCompObj[Instance]) != BSP_ERROR_NONE)
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
  * @brief  Disable environmental sensor
  * @param  Instance environmental sensor instance to be used
  * @param  Function Environmental sensor function. Could be :
  *         - ENV_TEMPERATURE
  *         - ENV_PRESSURE
  *         - ENV_HUMIDITY
  * @retval BSP status
  */
int32_t IKS5A1_ENV_SENSOR_Disable(uint32_t Instance, uint32_t Function)
{
  int32_t ret;

  if (Instance >= IKS5A1_ENV_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((EnvCtx[Instance].Functions & Function) == Function)
    {
      if (EnvFuncDrv[Instance][FunctionIndex[Function]]->Disable(EnvCompObj[Instance]) != BSP_ERROR_NONE)
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
  * @brief  Get environmental sensor Output Data Rate
  * @param  Instance environmental sensor instance to be used
  * @param  Function Environmental sensor function. Could be :
  *         - ENV_TEMPERATURE
  *         - ENV_PRESSURE
  *         - ENV_HUMIDITY
  * @param  Odr pointer to Output Data Rate read value
  * @retval BSP status
  */
int32_t IKS5A1_ENV_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float_t *Odr)
{
  int32_t ret;

  if (Instance >= IKS5A1_ENV_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((EnvCtx[Instance].Functions & Function) == Function)
    {
      if (EnvFuncDrv[Instance][FunctionIndex[Function]]->GetOutputDataRate(EnvCompObj[Instance], Odr) != BSP_ERROR_NONE)
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
  * @brief  Set environmental sensor Output Data Rate
  * @param  Instance environmental sensor instance to be used
  * @param  Function Environmental sensor function. Could be :
  *         - ENV_TEMPERATURE
  *         - ENV_PRESSURE
  *         - ENV_HUMIDITY
  * @param  Odr Output Data Rate value to be set
  * @retval BSP status
  */
int32_t IKS5A1_ENV_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float_t Odr)
{
  int32_t ret;

  if (Instance >= IKS5A1_ENV_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((EnvCtx[Instance].Functions & Function) == Function)
    {
      if (EnvFuncDrv[Instance][FunctionIndex[Function]]->SetOutputDataRate(EnvCompObj[Instance], Odr) != BSP_ERROR_NONE)
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
  * @brief  Get environmental sensor value
  * @param  Instance environmental sensor instance to be used
  * @param  Function Environmental sensor function. Could be :
  *         - ENV_TEMPERATURE
  *         - ENV_PRESSURE
  *         - ENV_HUMIDITY
  * @param  Value pointer to environmental sensor value
  * @retval BSP status
  */
int32_t IKS5A1_ENV_SENSOR_GetValue(uint32_t Instance, uint32_t Function, float_t *Value)
{
  int32_t ret;

  if (Instance >= IKS5A1_ENV_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((EnvCtx[Instance].Functions & Function) == Function)
    {
      if (EnvFuncDrv[Instance][FunctionIndex[Function]]->GetValue(EnvCompObj[Instance], Value) != BSP_ERROR_NONE)
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

/** @defgroup IKS5A1_ENV_SENSORS_Private_Functions IKS5A1 ENV SENSORS Private Functions
  * @{
  */

#if (USE_IKS5A1_ENV_SENSOR_ILPS22QS_0 == 1)
/**
  * @brief  Register Bus IOs for ILPS22QS instance
  * @param  Functions Environmental sensor functions. Could be :
  *         - ENV_TEMPERATURE and/or ENV_PRESSURE
  * @retval BSP status
  */
static int32_t ILPS22QS_0_Probe(uint32_t Functions)
{
  ILPS22QS_IO_t            io_ctx;
  uint8_t                  id;
  int32_t                  ret = BSP_ERROR_NONE;
  static ILPS22QS_Object_t ilps22qs_obj_0;
  ILPS22QS_Capabilities_t  cap;

  /* Configure the driver */
  io_ctx.BusType     = ILPS22QS_I2C_BUS; /* I2C */
  io_ctx.Address     = ILPS22QS_I2C_ADD;
  io_ctx.Init        = IKS5A1_I2C_INIT;
  io_ctx.DeInit      = IKS5A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS5A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS5A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS5A1_GET_TICK;
  io_ctx.Delay       = IKS5A1_DELAY;

  if (ILPS22QS_RegisterBusIO(&ilps22qs_obj_0, &io_ctx) != ILPS22QS_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (ILPS22QS_ReadID(&ilps22qs_obj_0, &id) != ILPS22QS_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != ILPS22QS_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)ILPS22QS_GetCapabilities(&ilps22qs_obj_0, &cap);

    EnvCtx[IKS5A1_ILPS22QS_0].Functions = ((uint32_t)cap.Temperature)
                                        | ((uint32_t)cap.Pressure << 1)
                                        | ((uint32_t)cap.Humidity << 2)
                                        | ((uint32_t)cap.Gas      << 3);

    EnvCompObj[IKS5A1_ILPS22QS_0] = &ilps22qs_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    EnvDrv[IKS5A1_ILPS22QS_0] = (ENV_SENSOR_CommonDrv_t *)(void *)&ILPS22QS_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_TEMPERATURE) == ENV_TEMPERATURE) && (cap.Temperature == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      EnvFuncDrv[IKS5A1_ILPS22QS_0][FunctionIndex[ENV_TEMPERATURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&ILPS22QS_TEMP_Driver;

      if (EnvDrv[IKS5A1_ILPS22QS_0]->Init(EnvCompObj[IKS5A1_ILPS22QS_0]) != ILPS22QS_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_PRESSURE) == ENV_PRESSURE) && (cap.Pressure == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      EnvFuncDrv[IKS5A1_ILPS22QS_0][FunctionIndex[ENV_PRESSURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&ILPS22QS_PRESS_Driver;

      if (EnvDrv[IKS5A1_ILPS22QS_0]->Init(EnvCompObj[IKS5A1_ILPS22QS_0]) != ILPS22QS_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_HUMIDITY) == ENV_HUMIDITY))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_GAS) == ENV_GAS))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return ret;
}
#endif

#if (USE_IKS5A1_ENV_SENSOR_SHT40AD1B_0 == 1)
/**
  * @brief  Register Bus IOs for SHT40AD1B instance
  * @param  Functions Environmental sensor functions. Could be :
  *         - ENV_TEMPERATURE and/or ENV_HUMIDITY
  * @retval BSP status
  */
static int32_t SHT40AD1B_0_Probe(uint32_t Functions)
{
  SHT40AD1B_IO_t            io_ctx;
  uint8_t                   id;
  int32_t                   ret = BSP_ERROR_NONE;
  static SHT40AD1B_Object_t sht40ad1b_obj_0;
  SHT40AD1B_Capabilities_t  cap;

  /* Configure the driver */
  io_ctx.BusType     = SHT40AD1B_I2C_BUS; /* I2C */
  io_ctx.Address     = SHT40AD1B_I2C_ADDRESS;
  io_ctx.Init        = IKS5A1_I2C_INIT;
  io_ctx.DeInit      = IKS5A1_I2C_DEINIT;
  io_ctx.Read        = IKS5A1_I2C_READ;
  io_ctx.Write       = IKS5A1_I2C_WRITE;
  io_ctx.GetTick     = IKS5A1_GET_TICK;
  io_ctx.Delay       = IKS5A1_DELAY;

  if (SHT40AD1B_RegisterBusIO(&sht40ad1b_obj_0, &io_ctx) != SHT40AD1B_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (SHT40AD1B_ReadID(&sht40ad1b_obj_0, &id) != SHT40AD1B_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != SHT40AD1B_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)SHT40AD1B_GetCapabilities(&sht40ad1b_obj_0, &cap);

    EnvCtx[IKS5A1_SHT40AD1B_0].Functions = ((uint32_t)cap.Temperature)
                                         | ((uint32_t)cap.Pressure << 1)
                                         | ((uint32_t)cap.Humidity << 2)
                                         | ((uint32_t)cap.Gas      << 3);

    EnvCompObj[IKS5A1_SHT40AD1B_0] = &sht40ad1b_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    EnvDrv[IKS5A1_SHT40AD1B_0] = (ENV_SENSOR_CommonDrv_t *)(void *)&SHT40AD1B_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_TEMPERATURE) == ENV_TEMPERATURE) && (cap.Temperature == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      EnvFuncDrv[IKS5A1_SHT40AD1B_0][FunctionIndex[ENV_TEMPERATURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&SHT40AD1B_TEMP_Driver;

      if (EnvDrv[IKS5A1_SHT40AD1B_0]->Init(EnvCompObj[IKS5A1_SHT40AD1B_0]) != SHT40AD1B_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_HUMIDITY) == ENV_HUMIDITY) && (cap.Humidity == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      EnvFuncDrv[IKS5A1_SHT40AD1B_0][FunctionIndex[ENV_HUMIDITY]] = (ENV_SENSOR_FuncDrv_t *)(void *)&SHT40AD1B_HUM_Driver;

      if (EnvDrv[IKS5A1_SHT40AD1B_0]->Init(EnvCompObj[IKS5A1_SHT40AD1B_0]) != SHT40AD1B_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_PRESSURE) == ENV_PRESSURE))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_GAS) == ENV_GAS))
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
