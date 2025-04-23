/**
  ******************************************************************************
  * @file    iks01a3_env_sensors.c
  * @author  MEMS Software Solutions Team
  * @brief   This file provides a set of functions needed to manage the environmental sensors
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
#include "iks01a3_env_sensors.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup IKS01A3 IKS01A3
  * @{
  */

/** @defgroup IKS01A3_ENV_SENSORS IKS01A3 ENV SENSORS
  * @{
  */

/** @defgroup IKS01A3_ENV_SENSORS_Exported_Variables IKS01A3 ENV SENSORS Exported Variables
  * @{
  */

extern void *EnvCompObj[IKS01A3_ENV_INSTANCES_NBR]; /* This "redundant" line is here to fulfill MISRA C-2012 rule 8.4 */
void *EnvCompObj[IKS01A3_ENV_INSTANCES_NBR];

/**
  * @}
  */

/** @defgroup IKS01A3_ENV_SENSORS_Private_Variables IKS01A3 ENV SENSORS Private Variables
  * @{
  */

/* We define a jump table in order to get the correct index from the desired function. */
/* This table should have a size equal to the maximum value of a function plus 1.      */
/* But due to MISRA it has to be increased to 15 + 1. */
static uint32_t FunctionIndex[] = {0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3};
static ENV_SENSOR_FuncDrv_t     *EnvFuncDrv[IKS01A3_ENV_INSTANCES_NBR][IKS01A3_ENV_FUNCTIONS_NBR];
static ENV_SENSOR_CommonDrv_t   *EnvDrv[IKS01A3_ENV_INSTANCES_NBR];
static IKS01A3_ENV_SENSOR_Ctx_t  EnvCtx[IKS01A3_ENV_INSTANCES_NBR];

/**
  * @}
  */

/** @defgroup IKS01A3_ENV_SENSORS_Private_Function_Prototypes IKS01A3 ENV SENSORS Private Function Prototypes
  * @{
  */

#if (USE_IKS01A3_ENV_SENSOR_HTS221_0 == 1)
static int32_t HTS221_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_ENV_SENSOR_SHT40AD1B_0 == 1)
static int32_t SHT40AD1B_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_ENV_SENSOR_SGP40_0 == 1)
static int32_t SGP40_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_ENV_SENSOR_LPS22HH_0 == 1)
static int32_t LPS22HH_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_ENV_SENSOR_STTS751_0 == 1)
static int32_t STTS751_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_ENV_SENSOR_LPS33HW_0 == 1)
static int32_t LPS33HW_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_ENV_SENSOR_LPS33K_0 == 1)
static int32_t LPS33K_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_ENV_SENSOR_STTS22H_0 == 1)
static int32_t STTS22H_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_ENV_SENSOR_LPS22CH_0 == 1)
static int32_t LPS22CH_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_ENV_SENSOR_LPS27HHTW_0 == 1)
static int32_t LPS27HHTW_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_ENV_SENSOR_LPS22DF_0 == 1)
static int32_t LPS22DF_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_ENV_SENSOR_ILPS22QS_0 == 1)
static int32_t ILPS22QS_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_ENV_SENSOR_ILPS28QSW_0 == 1)
static int32_t ILPS28QSW_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_ENV_SENSOR_LPS28DFW_0 == 1)
static int32_t LPS28DFW_0_Probe(uint32_t Functions);
#endif

/**
  * @}
  */

/** @defgroup IKS01A3_ENV_SENSORS_Exported_Functions IKS01A3 ENV SENSOR Exported Functions
  * @{
  */

/**
  * @brief  Initializes the environmental sensor
  * @param  Instance environmental sensor instance to be used
  * @param  Functions Environmental sensor functions. Could be :
  *         - ENV_TEMPERATURE and/or ENV_HUMIDITY for instance 0
  *         - ENV_TEMPERATURE and/or ENV_PRESSURE for instance 1
  *         - ENV_TEMPERATURE for instance 2
  * @retval BSP status
  */
int32_t IKS01A3_ENV_SENSOR_Init(uint32_t Instance, uint32_t Functions)
{
  int32_t ret = BSP_ERROR_NONE;
  uint32_t function = ENV_TEMPERATURE;
  uint32_t i;
  uint32_t component_functions = 0;
  IKS01A3_ENV_SENSOR_Capabilities_t cap;

  switch (Instance)
  {
#if (USE_IKS01A3_ENV_SENSOR_HTS221_0 == 1)
    case IKS01A3_HTS221_0:
      if (HTS221_0_Probe(Functions) != BSP_ERROR_NONE)
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

#if (USE_IKS01A3_ENV_SENSOR_SHT40AD1B_0 == 1)
    case IKS01A3_SHT40AD1B_0:
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

#if (USE_IKS01A3_ENV_SENSOR_SGP40_0 == 1)
    case IKS01A3_SGP40_0:
      if (SGP40_0_Probe(Functions) != BSP_ERROR_NONE)
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

#if (USE_IKS01A3_ENV_SENSOR_LPS22HH_0 == 1)
    case IKS01A3_LPS22HH_0:
      if (LPS22HH_0_Probe(Functions) != BSP_ERROR_NONE)
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

#if (USE_IKS01A3_ENV_SENSOR_STTS751_0 == 1)
    case IKS01A3_STTS751_0:
      if (STTS751_0_Probe(Functions) != BSP_ERROR_NONE)
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

#if (USE_IKS01A3_ENV_SENSOR_LPS33HW_0 == 1)
    case IKS01A3_LPS33HW_0:
      if (LPS33HW_0_Probe(Functions) != BSP_ERROR_NONE)
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

#if (USE_IKS01A3_ENV_SENSOR_STTS22H_0 == 1)
    case IKS01A3_STTS22H_0:
      if (STTS22H_0_Probe(Functions) != BSP_ERROR_NONE)
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

#if (USE_IKS01A3_ENV_SENSOR_LPS33K_0 == 1)
    case IKS01A3_LPS33K_0:
      if (LPS33K_0_Probe(Functions) != BSP_ERROR_NONE)
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

#if (USE_IKS01A3_ENV_SENSOR_LPS22CH_0 == 1)
    case IKS01A3_LPS22CH_0:
      if (LPS22CH_0_Probe(Functions) != BSP_ERROR_NONE)
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

#if (USE_IKS01A3_ENV_SENSOR_LPS27HHTW_0 == 1)
    case IKS01A3_LPS27HHTW_0:
      if (LPS27HHTW_0_Probe(Functions) != BSP_ERROR_NONE)
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

#if (USE_IKS01A3_ENV_SENSOR_LPS22DF_0 == 1)
    case IKS01A3_LPS22DF_0:
      if (LPS22DF_0_Probe(Functions) != BSP_ERROR_NONE)
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

#if (USE_IKS01A3_ENV_SENSOR_ILPS22QS_0 == 1)
    case IKS01A3_ILPS22QS_0:
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

#if (USE_IKS01A3_ENV_SENSOR_ILPS28QSW_0 == 1)
    case IKS01A3_ILPS28QSW_0:
      if (ILPS28QSW_0_Probe(Functions) != BSP_ERROR_NONE)
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

#if (USE_IKS01A3_ENV_SENSOR_LPS28DFW_0 == 1)
    case IKS01A3_LPS28DFW_0:
      if (LPS28DFW_0_Probe(Functions) != BSP_ERROR_NONE)
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

  for (i = 0; i < IKS01A3_ENV_FUNCTIONS_NBR; i++)
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
int32_t IKS01A3_ENV_SENSOR_DeInit(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= IKS01A3_ENV_INSTANCES_NBR)
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
int32_t IKS01A3_ENV_SENSOR_GetCapabilities(uint32_t Instance, IKS01A3_ENV_SENSOR_Capabilities_t *Capabilities)
{
  int32_t ret;

  if (Instance >= IKS01A3_ENV_INSTANCES_NBR)
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
int32_t IKS01A3_ENV_SENSOR_ReadID(uint32_t Instance, uint8_t *Id)
{
  int32_t ret;

  if (Instance >= IKS01A3_ENV_INSTANCES_NBR)
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
  * @param  Function Environmental sensor function
  * @retval BSP status
  */
int32_t IKS01A3_ENV_SENSOR_Enable(uint32_t Instance, uint32_t Function)
{
  int32_t ret;

  if (Instance >= IKS01A3_ENV_INSTANCES_NBR)
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
  * @param  Function Environmental sensor function
  * @retval BSP status
  */
int32_t IKS01A3_ENV_SENSOR_Disable(uint32_t Instance, uint32_t Function)
{
  int32_t ret;

  if (Instance >= IKS01A3_ENV_INSTANCES_NBR)
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
  * @param  Function Environmental sensor function
  * @param  Odr pointer to Output Data Rate read value
  * @retval BSP status
  */
int32_t IKS01A3_ENV_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float_t *Odr)
{
  int32_t ret;

  if (Instance >= IKS01A3_ENV_INSTANCES_NBR)
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
  * @param  Function Environmental sensor function
  * @param  Odr Output Data Rate value to be set
  * @retval BSP status
  */
int32_t IKS01A3_ENV_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float_t Odr)
{
  int32_t ret;

  if (Instance >= IKS01A3_ENV_INSTANCES_NBR)
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
  * @param  Function Environmental sensor function
  * @param  Value pointer to environmental sensor value
  * @retval BSP status
  */
int32_t IKS01A3_ENV_SENSOR_GetValue(uint32_t Instance, uint32_t Function, float_t *Value)
{
  int32_t ret;

  if (Instance >= IKS01A3_ENV_INSTANCES_NBR)
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

/** @defgroup IKS01A3_ENV_SENSORS_Private_Functions IKS01A3 ENV SENSORS Private Functions
  * @{
  */

#if (USE_IKS01A3_ENV_SENSOR_HTS221_0 == 1)
/**
  * @brief  Register Bus IOs for instance 0 if component ID is OK
  * @param  Functions Environmental sensor functions. Could be :
  *         - ENV_TEMPERATURE and/or ENV_HUMIDITY
  * @retval BSP status
  */
static int32_t HTS221_0_Probe(uint32_t Functions)
{
  HTS221_IO_t            io_ctx;
  uint8_t                id;
  int32_t                ret = BSP_ERROR_NONE;
  static HTS221_Object_t hts221_obj_0;
  HTS221_Capabilities_t  cap;

  /* Configure the environmental sensor driver */
  io_ctx.BusType     = HTS221_I2C_BUS; /* I2C */
  io_ctx.Address     = HTS221_I2C_ADDRESS;
  io_ctx.Init        = IKS01A3_I2C_INIT;
  io_ctx.DeInit      = IKS01A3_I2C_DEINIT;
  io_ctx.ReadReg     = IKS01A3_I2C_READ_REG;
  io_ctx.WriteReg    = IKS01A3_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS01A3_GET_TICK;

  if (HTS221_RegisterBusIO(&hts221_obj_0, &io_ctx) != HTS221_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (HTS221_ReadID(&hts221_obj_0, &id) != HTS221_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != HTS221_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)HTS221_GetCapabilities(&hts221_obj_0, &cap);
    EnvCtx[IKS01A3_HTS221_0].Functions = ((uint32_t)cap.Temperature)
                                       | ((uint32_t)cap.Pressure << 1)
                                       | ((uint32_t)cap.Humidity << 2)
                                       | ((uint32_t)cap.Gas      << 3);

    EnvCompObj[IKS01A3_HTS221_0] = &hts221_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    EnvDrv[IKS01A3_HTS221_0] = (ENV_SENSOR_CommonDrv_t *)(void *)&HTS221_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_TEMPERATURE) == ENV_TEMPERATURE) && (cap.Temperature == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      EnvFuncDrv[IKS01A3_HTS221_0][FunctionIndex[ENV_TEMPERATURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&HTS221_TEMP_Driver;

      if (EnvDrv[IKS01A3_HTS221_0]->Init(EnvCompObj[IKS01A3_HTS221_0]) != HTS221_OK)
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
      EnvFuncDrv[IKS01A3_HTS221_0][FunctionIndex[ENV_HUMIDITY]] = (ENV_SENSOR_FuncDrv_t *)(void *)&HTS221_HUM_Driver;

      if (EnvDrv[IKS01A3_HTS221_0]->Init(EnvCompObj[IKS01A3_HTS221_0]) != HTS221_OK)
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

#if (USE_IKS01A3_ENV_SENSOR_SHT40AD1B_0 == 1)
/**
  * @brief  Register Bus IOs for instance 0 if component ID is OK
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

  /* Configure the environmental sensor driver */
  io_ctx.BusType  = SHT40AD1B_I2C_BUS; /* I2C */
  io_ctx.Address  = SHT40AD1B_I2C_ADDRESS;
  io_ctx.Init     = IKS01A3_I2C_INIT;
  io_ctx.DeInit   = IKS01A3_I2C_DEINIT;
  io_ctx.Read     = IKS01A3_I2C_Read;
  io_ctx.Write    = IKS01A3_I2C_Write;
  io_ctx.GetTick  = IKS01A3_GET_TICK;
  io_ctx.Delay    = IKS01A3_DELAY;

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
    EnvCtx[IKS01A3_SHT40AD1B_0].Functions = ((uint32_t)cap.Temperature)
                                          | ((uint32_t)cap.Pressure << 1)
                                          | ((uint32_t)cap.Humidity << 2)
                                          | ((uint32_t)cap.Gas      << 3);

    EnvCompObj[IKS01A3_SHT40AD1B_0] = &sht40ad1b_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    EnvDrv[IKS01A3_SHT40AD1B_0] = (ENV_SENSOR_CommonDrv_t *)(void *)&SHT40AD1B_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_TEMPERATURE) == ENV_TEMPERATURE) && (cap.Temperature == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      EnvFuncDrv[IKS01A3_SHT40AD1B_0][FunctionIndex[ENV_TEMPERATURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&SHT40AD1B_TEMP_Driver;

      if (EnvDrv[IKS01A3_SHT40AD1B_0]->Init(EnvCompObj[IKS01A3_SHT40AD1B_0]) != SHT40AD1B_OK)
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
      EnvFuncDrv[IKS01A3_SHT40AD1B_0][FunctionIndex[ENV_HUMIDITY]] = (ENV_SENSOR_FuncDrv_t *)(void *)&SHT40AD1B_HUM_Driver;

      if (EnvDrv[IKS01A3_SHT40AD1B_0]->Init(EnvCompObj[IKS01A3_SHT40AD1B_0]) != SHT40AD1B_OK)
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

#if (USE_IKS01A3_ENV_SENSOR_SGP40_0 == 1)
/**
  * @brief  Register Bus IOs for instance 0 if component ID is OK
  * @param  Functions Environmental sensor functions. Could be :
  *         - ENV_GAS
  * @retval BSP status
  */
static int32_t SGP40_0_Probe(uint32_t Functions)
{
  SGP40_IO_t            io_ctx;
  uint8_t               id;
  int32_t               ret = BSP_ERROR_NONE;
  static SGP40_Object_t sgp40_obj_0;
  SGP40_Capabilities_t  cap;

  /* Configure the environmental sensor driver */
  io_ctx.BusType  = SGP40_I2C_BUS; /* I2C */
  io_ctx.Address  = SGP40_I2C_ADDRESS;
  io_ctx.Init     = IKS01A3_I2C_INIT;
  io_ctx.DeInit   = IKS01A3_I2C_DEINIT;
  io_ctx.Read     = IKS01A3_I2C_Read;
  io_ctx.Write    = IKS01A3_I2C_Write;
  io_ctx.GetTick  = IKS01A3_GET_TICK;
  io_ctx.Delay    = IKS01A3_DELAY;

  if (SGP40_RegisterBusIO(&sgp40_obj_0, &io_ctx) != SGP40_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (SGP40_ReadID(&sgp40_obj_0, &id) != SGP40_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != SGP40_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)SGP40_GetCapabilities(&sgp40_obj_0, &cap);
    EnvCtx[IKS01A3_SGP40_0].Functions = ((uint32_t)cap.Temperature)
                                      | ((uint32_t)cap.Pressure << 1)
                                      | ((uint32_t)cap.Humidity << 2)
                                      | ((uint32_t)cap.Gas      << 3);

    EnvCompObj[IKS01A3_SGP40_0] = &sgp40_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    EnvDrv[IKS01A3_SGP40_0] = (ENV_SENSOR_CommonDrv_t *)(void *)&SGP40_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_TEMPERATURE) == ENV_TEMPERATURE))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_HUMIDITY) == ENV_HUMIDITY))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_PRESSURE) == ENV_PRESSURE))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_GAS) == ENV_GAS) && (cap.Gas == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      EnvFuncDrv[IKS01A3_SGP40_0][FunctionIndex[ENV_GAS]] = (ENV_SENSOR_FuncDrv_t *)(void *)&SGP40_GAS_Driver;

      if (EnvDrv[IKS01A3_SGP40_0]->Init(EnvCompObj[IKS01A3_SGP40_0]) != SGP40_OK)
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

#if (USE_IKS01A3_ENV_SENSOR_LPS22HH_0 == 1)
/**
  * @brief  Register Bus IOs for instance 1 if component ID is OK
  * @param  Functions Environmental sensor functions. Could be :
  *         - ENV_TEMPERATURE and/or ENV_PRESSURE
  * @retval BSP status
  */
static int32_t LPS22HH_0_Probe(uint32_t Functions)
{
  LPS22HH_IO_t            io_ctx;
  uint8_t                 id;
  int32_t                 ret = BSP_ERROR_NONE;
  static LPS22HH_Object_t lps22hh_obj_0;
  LPS22HH_Capabilities_t  cap;

  /* Configure the pressure driver */
  io_ctx.BusType     = LPS22HH_I2C_BUS; /* I2C */
  io_ctx.Address     = LPS22HH_I2C_ADD_H;
  io_ctx.Init        = IKS01A3_I2C_INIT;
  io_ctx.DeInit      = IKS01A3_I2C_DEINIT;
  io_ctx.ReadReg     = IKS01A3_I2C_READ_REG;
  io_ctx.WriteReg    = IKS01A3_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS01A3_GET_TICK;

  if (LPS22HH_RegisterBusIO(&lps22hh_obj_0, &io_ctx) != LPS22HH_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LPS22HH_ReadID(&lps22hh_obj_0, &id) != LPS22HH_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LPS22HH_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LPS22HH_GetCapabilities(&lps22hh_obj_0, &cap);

    EnvCtx[IKS01A3_LPS22HH_0].Functions = ((uint32_t)cap.Temperature)
                                        | ((uint32_t)cap.Pressure << 1)
                                        | ((uint32_t)cap.Humidity << 2)
                                        | ((uint32_t)cap.Gas      << 3);

    EnvCompObj[IKS01A3_LPS22HH_0] = &lps22hh_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    EnvDrv[IKS01A3_LPS22HH_0] = (ENV_SENSOR_CommonDrv_t *)(void *)&LPS22HH_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_TEMPERATURE) == ENV_TEMPERATURE) && (cap.Temperature == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      EnvFuncDrv[IKS01A3_LPS22HH_0][FunctionIndex[ENV_TEMPERATURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&LPS22HH_TEMP_Driver;

      if (EnvDrv[IKS01A3_LPS22HH_0]->Init(EnvCompObj[IKS01A3_LPS22HH_0]) != LPS22HH_OK)
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
      EnvFuncDrv[IKS01A3_LPS22HH_0][FunctionIndex[ENV_PRESSURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&LPS22HH_PRESS_Driver;

      if (EnvDrv[IKS01A3_LPS22HH_0]->Init(EnvCompObj[IKS01A3_LPS22HH_0]) != LPS22HH_OK)
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

#if (USE_IKS01A3_ENV_SENSOR_STTS751_0 == 1)
/**
  * @brief  Register Bus IOs for instance 2 if component ID is OK
  * @param  Functions Environmental sensor functions. Could be :
  *         - ENV_TEMPERATURE
  * @retval BSP status
  */
static int32_t STTS751_0_Probe(uint32_t Functions)
{
  STTS751_IO_t            io_ctx;
  uint8_t                 id;
  int32_t                 ret = BSP_ERROR_NONE;
  static STTS751_Object_t stts751_obj_0;
  STTS751_Capabilities_t  cap;

  /* Configure the pressure driver */
  io_ctx.BusType     = STTS751_I2C_BUS; /* I2C */
  io_ctx.Address     = STTS751_1xxxx_ADD_7K5;
  io_ctx.Init        = IKS01A3_I2C_INIT;
  io_ctx.DeInit      = IKS01A3_I2C_DEINIT;
  io_ctx.ReadReg     = IKS01A3_I2C_READ_REG;
  io_ctx.WriteReg    = IKS01A3_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS01A3_GET_TICK;

  if (STTS751_RegisterBusIO(&stts751_obj_0, &io_ctx) != STTS751_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (STTS751_ReadID(&stts751_obj_0, &id) != STTS751_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != STTS751_ID_MAN)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)STTS751_GetCapabilities(&stts751_obj_0, &cap);

    EnvCtx[IKS01A3_STTS751_0].Functions = ((uint32_t)cap.Temperature)
                                        | ((uint32_t)cap.Pressure << 1)
                                        | ((uint32_t)cap.Humidity << 2)
                                        | ((uint32_t)cap.Gas      << 3);

    EnvCompObj[IKS01A3_STTS751_0] = &stts751_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    EnvDrv[IKS01A3_STTS751_0] = (ENV_SENSOR_CommonDrv_t *)(void *)&STTS751_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_TEMPERATURE) == ENV_TEMPERATURE) && (cap.Temperature == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      EnvFuncDrv[IKS01A3_STTS751_0][FunctionIndex[ENV_TEMPERATURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&STTS751_TEMP_Driver;

      if (EnvDrv[IKS01A3_STTS751_0]->Init(EnvCompObj[IKS01A3_STTS751_0]) != STTS751_OK)
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

#if (USE_IKS01A3_ENV_SENSOR_LPS33HW_0 == 1)
/**
  * @brief  Register Bus IOs for instance 1 if component ID is OK
  * @param  Functions Environmental sensor functions. Could be :
  *         - ENV_TEMPERATURE and/or ENV_PRESSURE
  * @retval BSP status
  */
static int32_t LPS33HW_0_Probe(uint32_t Functions)
{
  LPS33HW_IO_t            io_ctx;
  uint8_t                 id;
  int32_t                 ret = BSP_ERROR_NONE;
  static LPS33HW_Object_t lps33hw_obj_0;
  LPS33HW_Capabilities_t  cap;

  /* Configure the pressure driver */
  io_ctx.BusType     = LPS33HW_I2C_BUS; /* I2C */
  io_ctx.Address     = LPS33HW_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_INIT;
  io_ctx.DeInit      = IKS01A3_I2C_DEINIT;
  io_ctx.ReadReg     = IKS01A3_I2C_READ_REG;
  io_ctx.WriteReg    = IKS01A3_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS01A3_GET_TICK;

  if (LPS33HW_RegisterBusIO(&lps33hw_obj_0, &io_ctx) != LPS33HW_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LPS33HW_ReadID(&lps33hw_obj_0, &id) != LPS33HW_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LPS33HW_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LPS33HW_GetCapabilities(&lps33hw_obj_0, &cap);

    EnvCtx[IKS01A3_LPS33HW_0].Functions = ((uint32_t)cap.Temperature)
                                        | ((uint32_t)cap.Pressure << 1)
                                        | ((uint32_t)cap.Humidity << 2)
                                        | ((uint32_t)cap.Gas      << 3);

    EnvCompObj[IKS01A3_LPS33HW_0] = &lps33hw_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    EnvDrv[IKS01A3_LPS33HW_0] = (ENV_SENSOR_CommonDrv_t *)(void *)&LPS33HW_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_TEMPERATURE) == ENV_TEMPERATURE) && (cap.Temperature == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      EnvFuncDrv[IKS01A3_LPS33HW_0][FunctionIndex[ENV_TEMPERATURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&LPS33HW_TEMP_Driver;

      if (EnvDrv[IKS01A3_LPS33HW_0]->Init(EnvCompObj[IKS01A3_LPS33HW_0]) != LPS33HW_OK)
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
      EnvFuncDrv[IKS01A3_LPS33HW_0][FunctionIndex[ENV_PRESSURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&LPS33HW_PRESS_Driver;

      if (EnvDrv[IKS01A3_LPS33HW_0]->Init(EnvCompObj[IKS01A3_LPS33HW_0]) != LPS33HW_OK)
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

#if (USE_IKS01A3_ENV_SENSOR_STTS22H_0 == 1)
/**
  * @brief  Register Bus IOs for instance 2 if component ID is OK
  * @param  Functions Environmental sensor functions. Could be :
  *         - ENV_TEMPERATURE
  * @retval BSP status
  */
static int32_t STTS22H_0_Probe(uint32_t Functions)
{
  STTS22H_IO_t            io_ctx;
  uint8_t                 id;
  int32_t                 ret = BSP_ERROR_NONE;
  static STTS22H_Object_t stts22h_obj_0;
  STTS22H_Capabilities_t  cap;

  /* Configure the pressure driver */
  io_ctx.BusType     = STTS22H_I2C_BUS; /* I2C */
  io_ctx.Address     = STTS22H_I2C_ADD_H;
  io_ctx.Init        = IKS01A3_I2C_INIT;
  io_ctx.DeInit      = IKS01A3_I2C_DEINIT;
  io_ctx.ReadReg     = IKS01A3_I2C_READ_REG;
  io_ctx.WriteReg    = IKS01A3_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS01A3_GET_TICK;

  if (STTS22H_RegisterBusIO(&stts22h_obj_0, &io_ctx) != STTS22H_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (STTS22H_ReadID(&stts22h_obj_0, &id) != STTS22H_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != STTS22H_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)STTS22H_GetCapabilities(&stts22h_obj_0, &cap);

    EnvCtx[IKS01A3_STTS22H_0].Functions = ((uint32_t)cap.Temperature)
                                        | ((uint32_t)cap.Pressure << 1)
                                        | ((uint32_t)cap.Humidity << 2)
                                        | ((uint32_t)cap.Gas      << 3);

    EnvCompObj[IKS01A3_STTS22H_0] = &stts22h_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    EnvDrv[IKS01A3_STTS22H_0] = (ENV_SENSOR_CommonDrv_t *)(void *)&STTS22H_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_TEMPERATURE) == ENV_TEMPERATURE) && (cap.Temperature == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      EnvFuncDrv[IKS01A3_STTS22H_0][FunctionIndex[ENV_TEMPERATURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&STTS22H_TEMP_Driver;

      if (EnvDrv[IKS01A3_STTS22H_0]->Init(EnvCompObj[IKS01A3_STTS22H_0]) != STTS22H_OK)
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

#if (USE_IKS01A3_ENV_SENSOR_LPS33K_0 == 1)
/**
  * @brief  Register Bus IOs for instance 1 if component ID is OK
  * @param  Functions Environmental sensor functions. Could be :
  *         - ENV_TEMPERATURE and/or ENV_PRESSURE
  * @retval BSP status
  */
static int32_t LPS33K_0_Probe(uint32_t Functions)
{
  LPS33K_IO_t             io_ctx;
  uint8_t                 id;
  int32_t                 ret = BSP_ERROR_NONE;
  static LPS33K_Object_t  lps33k_obj_0;
  LPS33K_Capabilities_t   cap;

  /* Configure the pressure driver */
  io_ctx.BusType     = LPS33K_I2C_BUS; /* I2C */
  io_ctx.Address     = LPS33K_I2C_ADD;
  io_ctx.Init        = IKS01A3_I2C_INIT;
  io_ctx.DeInit      = IKS01A3_I2C_DEINIT;
  io_ctx.ReadReg     = IKS01A3_I2C_READ_REG;
  io_ctx.WriteReg    = IKS01A3_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS01A3_GET_TICK;

  if (LPS33K_RegisterBusIO(&lps33k_obj_0, &io_ctx) != LPS33K_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LPS33K_ReadID(&lps33k_obj_0, &id) != LPS33K_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LPS33K_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LPS33K_GetCapabilities(&lps33k_obj_0, &cap);

    EnvCtx[IKS01A3_LPS33K_0].Functions = ((uint32_t)cap.Temperature)
                                       | ((uint32_t)cap.Pressure << 1)
                                       | ((uint32_t)cap.Humidity << 2)
                                       | ((uint32_t)cap.Gas      << 3);

    EnvCompObj[IKS01A3_LPS33K_0] = &lps33k_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    EnvDrv[IKS01A3_LPS33K_0] = (ENV_SENSOR_CommonDrv_t *)(void *)&LPS33K_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_TEMPERATURE) == ENV_TEMPERATURE) && (cap.Temperature == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      EnvFuncDrv[IKS01A3_LPS33K_0][FunctionIndex[ENV_TEMPERATURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&LPS33K_TEMP_Driver;

      if (EnvDrv[IKS01A3_LPS33K_0]->Init(EnvCompObj[IKS01A3_LPS33K_0]) != LPS33K_OK)
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
      EnvFuncDrv[IKS01A3_LPS33K_0][FunctionIndex[ENV_PRESSURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&LPS33K_PRESS_Driver;

      if (EnvDrv[IKS01A3_LPS33K_0]->Init(EnvCompObj[IKS01A3_LPS33K_0]) != LPS33K_OK)
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

#if (USE_IKS01A3_ENV_SENSOR_LPS22CH_0 == 1)
/**
  * @brief  Register Bus IOs for instance 1 if component ID is OK
  * @param  Functions Environmental sensor functions. Could be :
  *         - ENV_TEMPERATURE and/or ENV_PRESSURE
  * @retval BSP status
  */
static int32_t LPS22CH_0_Probe(uint32_t Functions)
{
  LPS22CH_IO_t            io_ctx;
  uint8_t                 id;
  int32_t                 ret = BSP_ERROR_NONE;
  static LPS22CH_Object_t lps22ch_obj_0;
  LPS22CH_Capabilities_t  cap;

  /* Configure the pressure driver */
  io_ctx.BusType     = LPS22CH_I2C_BUS; /* I2C */
  io_ctx.Address     = LPS22CH_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_INIT;
  io_ctx.DeInit      = IKS01A3_I2C_DEINIT;
  io_ctx.ReadReg     = IKS01A3_I2C_READ_REG;
  io_ctx.WriteReg    = IKS01A3_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS01A3_GET_TICK;

  if (LPS22CH_RegisterBusIO(&lps22ch_obj_0, &io_ctx) != LPS22CH_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LPS22CH_ReadID(&lps22ch_obj_0, &id) != LPS22CH_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LPS22CH_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LPS22CH_GetCapabilities(&lps22ch_obj_0, &cap);
    EnvCtx[IKS01A3_LPS22CH_0].Functions = ((uint32_t)cap.Temperature)
                                        | ((uint32_t)cap.Pressure << 1)
                                        | ((uint32_t)cap.Humidity << 2)
                                        | ((uint32_t)cap.Gas      << 3);

    EnvCompObj[IKS01A3_LPS22CH_0] = &lps22ch_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    EnvDrv[IKS01A3_LPS22CH_0] = (ENV_SENSOR_CommonDrv_t *)(void *)&LPS22CH_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_TEMPERATURE) == ENV_TEMPERATURE) && (cap.Temperature == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      EnvFuncDrv[IKS01A3_LPS22CH_0][FunctionIndex[ENV_TEMPERATURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&LPS22CH_TEMP_Driver;

      if (EnvDrv[IKS01A3_LPS22CH_0]->Init(EnvCompObj[IKS01A3_LPS22CH_0]) != LPS22CH_OK)
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
      EnvFuncDrv[IKS01A3_LPS22CH_0][FunctionIndex[ENV_PRESSURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&LPS22CH_PRESS_Driver;

      if (EnvDrv[IKS01A3_LPS22CH_0]->Init(EnvCompObj[IKS01A3_LPS22CH_0]) != LPS22CH_OK)
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

#if (USE_IKS01A3_ENV_SENSOR_LPS27HHTW_0 == 1)
/**
  * @brief  Register Bus IOs for instance 1 if component ID is OK
  * @param  Functions Environmental sensor functions. Could be :
  *         - ENV_TEMPERATURE and/or ENV_PRESSURE
  * @retval BSP status
  */
static int32_t LPS27HHTW_0_Probe(uint32_t Functions)
{
  LPS27HHTW_IO_t            io_ctx;
  uint8_t                   id;
  int32_t                   ret = BSP_ERROR_NONE;
  static LPS27HHTW_Object_t lps27hhtw_obj_0;
  LPS27HHTW_Capabilities_t  cap;

  /* Configure the pressure driver */
  io_ctx.BusType     = LPS27HHTW_I2C_BUS; /* I2C */
  io_ctx.Address     = LPS27HHTW_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_INIT;
  io_ctx.DeInit      = IKS01A3_I2C_DEINIT;
  io_ctx.ReadReg     = IKS01A3_I2C_READ_REG;
  io_ctx.WriteReg    = IKS01A3_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS01A3_GET_TICK;

  if (LPS27HHTW_RegisterBusIO(&lps27hhtw_obj_0, &io_ctx) != LPS27HHTW_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LPS27HHTW_ReadID(&lps27hhtw_obj_0, &id) != LPS27HHTW_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LPS27HHTW_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LPS27HHTW_GetCapabilities(&lps27hhtw_obj_0, &cap);
    EnvCtx[IKS01A3_LPS27HHTW_0].Functions = ((uint32_t)cap.Temperature)
                                          | ((uint32_t)cap.Pressure << 1)
                                          | ((uint32_t)cap.Humidity << 2)
                                          | ((uint32_t)cap.Gas      << 3);

    EnvCompObj[IKS01A3_LPS27HHTW_0] = &lps27hhtw_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    EnvDrv[IKS01A3_LPS27HHTW_0] = (ENV_SENSOR_CommonDrv_t *)(void *)&LPS27HHTW_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_TEMPERATURE) == ENV_TEMPERATURE) && (cap.Temperature == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      EnvFuncDrv[IKS01A3_LPS27HHTW_0][FunctionIndex[ENV_TEMPERATURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&LPS27HHTW_TEMP_Driver;

      if (EnvDrv[IKS01A3_LPS27HHTW_0]->Init(EnvCompObj[IKS01A3_LPS27HHTW_0]) != LPS27HHTW_OK)
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
      EnvFuncDrv[IKS01A3_LPS27HHTW_0][FunctionIndex[ENV_PRESSURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&LPS27HHTW_PRESS_Driver;

      if (EnvDrv[IKS01A3_LPS27HHTW_0]->Init(EnvCompObj[IKS01A3_LPS27HHTW_0]) != LPS27HHTW_OK)
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

#if (USE_IKS01A3_ENV_SENSOR_LPS22DF_0 == 1)
/**
  * @brief  Register Bus IOs for instance 1 if component ID is OK
  * @param  Functions Environmental sensor functions. Could be :
  *         - ENV_TEMPERATURE and/or ENV_PRESSURE
  * @retval BSP status
  */
static int32_t LPS22DF_0_Probe(uint32_t Functions)
{
  LPS22DF_IO_t            io_ctx;
  uint8_t                 id;
  int32_t                 ret = BSP_ERROR_NONE;
  static LPS22DF_Object_t lps22df_obj_0;
  LPS22DF_Capabilities_t  cap;

  /* Configure the pressure driver */
  io_ctx.BusType     = LPS22DF_I2C_BUS; /* I2C */
  io_ctx.Address     = LPS22DF_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_INIT;
  io_ctx.DeInit      = IKS01A3_I2C_DEINIT;
  io_ctx.ReadReg     = IKS01A3_I2C_READ_REG;
  io_ctx.WriteReg    = IKS01A3_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS01A3_GET_TICK;

  if (LPS22DF_RegisterBusIO(&lps22df_obj_0, &io_ctx) != LPS22DF_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LPS22DF_ReadID(&lps22df_obj_0, &id) != LPS22DF_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LPS22DF_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LPS22DF_GetCapabilities(&lps22df_obj_0, &cap);
    EnvCtx[IKS01A3_LPS22DF_0].Functions = ((uint32_t)cap.Temperature)
                                        | ((uint32_t)cap.Pressure << 1)
                                        | ((uint32_t)cap.Humidity << 2)
                                        | ((uint32_t)cap.Gas      << 3);

    EnvCompObj[IKS01A3_LPS22DF_0] = &lps22df_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    EnvDrv[IKS01A3_LPS22DF_0] = (ENV_SENSOR_CommonDrv_t *)(void *)&LPS22DF_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_TEMPERATURE) == ENV_TEMPERATURE) && (cap.Temperature == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      EnvFuncDrv[IKS01A3_LPS22DF_0][FunctionIndex[ENV_TEMPERATURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&LPS22DF_TEMP_Driver;

      if (EnvDrv[IKS01A3_LPS22DF_0]->Init(EnvCompObj[IKS01A3_LPS22DF_0]) != LPS22DF_OK)
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
      EnvFuncDrv[IKS01A3_LPS22DF_0][FunctionIndex[ENV_PRESSURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&LPS22DF_PRESS_Driver;

      if (EnvDrv[IKS01A3_LPS22DF_0]->Init(EnvCompObj[IKS01A3_LPS22DF_0]) != LPS22DF_OK)
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

#if (USE_IKS01A3_ENV_SENSOR_ILPS22QS_0 == 1)
/**
  * @brief  Register Bus IOs for instance 1 if component ID is OK
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

  /* Configure the pressure driver */
  io_ctx.BusType     = ILPS22QS_I2C_BUS; /* I2C */
  io_ctx.Address     = ILPS22QS_I2C_ADD;
  io_ctx.Init        = IKS01A3_I2C_INIT;
  io_ctx.DeInit      = IKS01A3_I2C_DEINIT;
  io_ctx.ReadReg     = IKS01A3_I2C_READ_REG;
  io_ctx.WriteReg    = IKS01A3_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS01A3_GET_TICK;

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
    EnvCtx[IKS01A3_ILPS22QS_0].Functions = ((uint32_t)cap.Temperature)
                                         | ((uint32_t)cap.Pressure << 1)
                                         | ((uint32_t)cap.Humidity << 2)
                                         | ((uint32_t)cap.Gas      << 3);

    EnvCompObj[IKS01A3_ILPS22QS_0] = &ilps22qs_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    EnvDrv[IKS01A3_ILPS22QS_0] = (ENV_SENSOR_CommonDrv_t *)(void *)&ILPS22QS_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_TEMPERATURE) == ENV_TEMPERATURE) && (cap.Temperature == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      EnvFuncDrv[IKS01A3_ILPS22QS_0][FunctionIndex[ENV_TEMPERATURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&ILPS22QS_TEMP_Driver;

      if (EnvDrv[IKS01A3_ILPS22QS_0]->Init(EnvCompObj[IKS01A3_ILPS22QS_0]) != ILPS22QS_OK)
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
      EnvFuncDrv[IKS01A3_ILPS22QS_0][FunctionIndex[ENV_PRESSURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&ILPS22QS_PRESS_Driver;

      if (EnvDrv[IKS01A3_ILPS22QS_0]->Init(EnvCompObj[IKS01A3_ILPS22QS_0]) != ILPS22QS_OK)
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

#if (USE_IKS01A3_ENV_SENSOR_ILPS28QSW_0 == 1)
/**
  * @brief  Register Bus IOs for instance 1 if component ID is OK
  * @param  Functions Environmental sensor functions. Could be :
  *         - ENV_TEMPERATURE and/or ENV_PRESSURE
  * @retval BSP status
  */
static int32_t ILPS28QSW_0_Probe(uint32_t Functions)
{
  ILPS28QSW_IO_t            io_ctx;
  uint8_t                   id;
  int32_t                   ret = BSP_ERROR_NONE;
  static ILPS28QSW_Object_t ilps28qsv_obj_0;
  ILPS28QSW_Capabilities_t  cap;

  /* Configure the pressure driver */
  io_ctx.BusType     = ILPS28QSW_I2C_BUS; /* I2C */
  io_ctx.Address     = ILPS28QSW_I2C_ADD;
  io_ctx.Init        = IKS01A3_I2C_INIT;
  io_ctx.DeInit      = IKS01A3_I2C_DEINIT;
  io_ctx.ReadReg     = IKS01A3_I2C_READ_REG;
  io_ctx.WriteReg    = IKS01A3_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS01A3_GET_TICK;

  if (ILPS28QSW_RegisterBusIO(&ilps28qsv_obj_0, &io_ctx) != ILPS28QSW_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (ILPS28QSW_ReadID(&ilps28qsv_obj_0, &id) != ILPS28QSW_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != ILPS28QSW_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)ILPS28QSW_GetCapabilities(&ilps28qsv_obj_0, &cap);
    EnvCtx[IKS01A3_ILPS28QSW_0].Functions = ((uint32_t)cap.Temperature)
                                          | ((uint32_t)cap.Pressure << 1)
                                          | ((uint32_t)cap.Humidity << 2)
                                          | ((uint32_t)cap.Gas      << 3);

    EnvCompObj[IKS01A3_ILPS28QSW_0] = &ilps28qsv_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    EnvDrv[IKS01A3_ILPS28QSW_0] = (ENV_SENSOR_CommonDrv_t *)(void *)&ILPS28QSW_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_TEMPERATURE) == ENV_TEMPERATURE) && (cap.Temperature == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      EnvFuncDrv[IKS01A3_ILPS28QSW_0][FunctionIndex[ENV_TEMPERATURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&ILPS28QSW_TEMP_Driver;

      if (EnvDrv[IKS01A3_ILPS28QSW_0]->Init(EnvCompObj[IKS01A3_ILPS28QSW_0]) != ILPS28QSW_OK)
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
      EnvFuncDrv[IKS01A3_ILPS28QSW_0][FunctionIndex[ENV_PRESSURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&ILPS28QSW_PRESS_Driver;

      if (EnvDrv[IKS01A3_ILPS28QSW_0]->Init(EnvCompObj[IKS01A3_ILPS28QSW_0]) != ILPS28QSW_OK)
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

#if (USE_IKS01A3_ENV_SENSOR_LPS28DFW_0 == 1)
/**
  * @brief  Register Bus IOs for instance 1 if component ID is OK
  * @param  Functions Environmental sensor functions. Could be :
  *         - ENV_TEMPERATURE and/or ENV_PRESSURE
  * @retval BSP status
  */
static int32_t LPS28DFW_0_Probe(uint32_t Functions)
{
  LPS28DFW_IO_t            io_ctx;
  uint8_t                  id;
  int32_t                  ret = BSP_ERROR_NONE;
  static LPS28DFW_Object_t lps28dfw_obj_0;
  LPS28DFW_Capabilities_t  cap;

  /* Configure the pressure driver */
  io_ctx.BusType     = LPS28DFW_I2C_BUS; /* I2C */
  io_ctx.Address     = LPS28DFW_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_INIT;
  io_ctx.DeInit      = IKS01A3_I2C_DEINIT;
  io_ctx.ReadReg     = IKS01A3_I2C_READ_REG;
  io_ctx.WriteReg    = IKS01A3_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS01A3_GET_TICK;

  if (LPS28DFW_RegisterBusIO(&lps28dfw_obj_0, &io_ctx) != LPS28DFW_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LPS28DFW_ReadID(&lps28dfw_obj_0, &id) != LPS28DFW_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LPS28DFW_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LPS28DFW_GetCapabilities(&lps28dfw_obj_0, &cap);
    EnvCtx[IKS01A3_LPS28DFW_0].Functions = ((uint32_t)cap.Temperature)
                                         | ((uint32_t)cap.Pressure << 1)
                                         | ((uint32_t)cap.Humidity << 2)
                                         | ((uint32_t)cap.Gas      << 3);

    EnvCompObj[IKS01A3_LPS28DFW_0] = &lps28dfw_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    EnvDrv[IKS01A3_LPS28DFW_0] = (ENV_SENSOR_CommonDrv_t *)(void *)&LPS28DFW_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & ENV_TEMPERATURE) == ENV_TEMPERATURE) && (cap.Temperature == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      EnvFuncDrv[IKS01A3_LPS28DFW_0][FunctionIndex[ENV_TEMPERATURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&LPS28DFW_TEMP_Driver;

      if (EnvDrv[IKS01A3_LPS28DFW_0]->Init(EnvCompObj[IKS01A3_LPS28DFW_0]) != LPS28DFW_OK)
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
      EnvFuncDrv[IKS01A3_LPS28DFW_0][FunctionIndex[ENV_PRESSURE]] = (ENV_SENSOR_FuncDrv_t *)(void *)&LPS28DFW_PRESS_Driver;

      if (EnvDrv[IKS01A3_LPS28DFW_0]->Init(EnvCompObj[IKS01A3_LPS28DFW_0]) != LPS28DFW_OK)
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
