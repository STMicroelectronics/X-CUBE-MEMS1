/**
  ******************************************************************************
  * @file    sgp40.c
  * @author  MEMS Software Solutions Team
  * @brief   SGP40 driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sgp40.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup SGP40 SGP40
  * @{
  */

/** @defgroup SGP40_Exported_Variables SGP40 Exported Variables
  * @{
  */

SGP40_CommonDrv_t SGP40_COMMON_Driver =
{
  SGP40_Init,
  SGP40_DeInit,
  SGP40_ReadID,
  SGP40_GetCapabilities,
};

SGP40_GAS_Drv_t SGP40_GAS_Driver =
{
  SGP40_GAS_Enable,
  SGP40_GAS_Disable,
  SGP40_GAS_GetOutputDataRate,
  SGP40_GAS_SetOutputDataRate,
  SGP40_GAS_GetValue,
};

/**
  * @}
  */

/** @defgroup SGP40_Private_Function_Prototypes SGP40 Private Function Prototypes
  * @{
  */

static int32_t GetData(SGP40_Object_t *pObj, float_t *ht_data);
static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);

/**
  * @}
  */

/** @defgroup SGP40_Exported_Functions SGP40 Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SGP40_RegisterBusIO(SGP40_Object_t *pObj, SGP40_IO_t *pIO)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = SGP40_ERROR;
  }
  else
  {
    pObj->IO.Init    = pIO->Init;
    pObj->IO.DeInit  = pIO->DeInit;
    pObj->IO.BusType = pIO->BusType;
    pObj->IO.Address = pIO->Address;
    pObj->IO.Write   = pIO->Write;
    pObj->IO.Read    = pIO->Read;
    pObj->IO.GetTick = pIO->GetTick;

    pObj->Ctx.read_reg  = ReadRegWrap;
    pObj->Ctx.write_reg = WriteRegWrap;
    pObj->Ctx.mdelay    = pIO->Delay;
    pObj->Ctx.handle    = pObj;

    if (pObj->IO.Init != NULL)
    {
      ret = pObj->IO.Init();
    }
    else
    {
      ret = SGP40_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Initialize the SGP40 sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SGP40_Init(SGP40_Object_t *pObj)
{
  /* This device doesn't support ODR, works more like one-shot measurement */
  pObj->gas_odr = 0.0f;

  /* Humidity and temperature values - used for compensation purposes.
     NOTE: Humidity / Temperature compensation is disabled when both humidity
           and temperature are at default values */
  pObj->hum_value  = 50.0f;  /* Default humidity value: 50 [%] */
  pObj->temp_value = 25.0f;  /* Default temperature value: 25 [degC] */

  pObj->is_initialized = 1;

  return SGP40_OK;
}

/**
  * @brief  Deinitialize the SGP40 sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SGP40_DeInit(SGP40_Object_t *pObj)
{
  pObj->is_initialized = 0;

  return SGP40_OK;
}

/**
  * @brief  Get WHO_AM_I value
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SGP40_ReadID(SGP40_Object_t *pObj, uint8_t *Id)
{
  return sgp40_device_id_get(&(pObj->Ctx), Id);
}

/**
  * @brief  Get SGP40 sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to SGP40 sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SGP40_GetCapabilities(SGP40_Object_t *pObj, SGP40_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Humidity    = 0;
  Capabilities->Pressure    = 0;
  Capabilities->Temperature = 0;
  Capabilities->Gas         = 1;
  Capabilities->LowPower    = 0;
  Capabilities->HumMaxOdr   = 0.0f;
  Capabilities->TempMaxOdr  = 0.0f;
  Capabilities->PressMaxOdr = 0.0f;
  Capabilities->GasMaxOdr   = 0.0f;
  return SGP40_OK;
}

/**
  * @brief  Get the SGP40 initialization status
  * @param  pObj the device pObj
  * @param  Status 1 if initialized, 0 otherwise
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SGP40_Get_Init_Status(SGP40_Object_t *pObj, uint8_t *Status)
{
  if (pObj == NULL)
  {
    return SGP40_ERROR;
  }

  *Status = pObj->is_initialized;

  return SGP40_OK;
}

/**
  * @brief  Enable the SGP40 sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SGP40_GAS_Enable(SGP40_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->gas_is_enabled == 1U)
  {
    return SGP40_OK;
  }

  pObj->gas_is_enabled = 1;

  return SGP40_OK;
}

/**
  * @brief  Disable the SGP40 sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SGP40_GAS_Disable(SGP40_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->gas_is_enabled == 0U)
  {
    return SGP40_OK;
  }

  pObj->gas_is_enabled = 0;

  return SGP40_OK;
}

/**
  * @brief  Get the SGP40 sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SGP40_GAS_GetOutputDataRate(SGP40_Object_t *pObj, float_t *Odr)
{
  /* NOTE: This device doesn't support ODR, works more like one-shot measurement */

  *Odr = pObj->gas_odr;

  return SGP40_OK;
}

/**
  * @brief  Set the SGP40 sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SGP40_GAS_SetOutputDataRate(SGP40_Object_t *pObj, float_t Odr)
{
  /* NOTE: This device doesn't support ODR, works more like one-shot measurement */

  pObj->gas_odr = Odr;

  return SGP40_OK;
}

/**
  * @brief  Get the SGP40 gas value (no humidity compensation)
  * @param  pObj the device pObj
  * @param  Value pointer where the gas value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SGP40_GAS_GetValue(SGP40_Object_t *pObj, float_t *Value)
{
  float_t ht_data[2];

  ht_data[0] = pObj->hum_value;
  ht_data[1] = pObj->temp_value;

  if (GetData(pObj, ht_data) != SGP40_OK)
  {
    return SGP40_ERROR;
  }

  *Value = pObj->gas_value;

  return SGP40_OK;
}

/**
  * @brief  Get the SGP40 new data status
  * @param  pObj the device pObj
  * @param  Status the new data status
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SGP40_GAS_Get_DRDY_Status(SGP40_Object_t *pObj, uint8_t *Status)
{
  (void)pObj;

  *Status = 1;

  return SGP40_OK;
}

/**
  * @brief  Set the SGP40 humidity value
  * @param  pObj the device pObj
  * @param  Value value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SGP40_GAS_Set_Humidity(SGP40_Object_t *pObj, float_t Value)
{
  pObj->hum_value = Value;

  return SGP40_OK;
}

/**
  * @brief  Set the SGP40 temperature value
  * @param  pObj the device pObj
  * @param  Value value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SGP40_GAS_Set_Temperature(SGP40_Object_t *pObj, float_t Value)
{
  pObj->temp_value = Value;

  return SGP40_OK;
}

/**
  * @}
  */

/** @defgroup SGP40_Private_Functions SGP40 Private Functions
  * @{
  */

/**
  * @brief  Get the SGP40 new data status
  * @param  pObj the device pObj
  * @param  ht_value  Humidity and Temperature value
  *                   or NULL if no humidity compensation required
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t GetData(SGP40_Object_t *pObj, float_t *ht_data)
{
  uint16_t data; /* VOC index */

  if (sgp40_data_get(&(pObj->Ctx), &data, ht_data) != 0)
  {
    return SGP40_ERROR;
  }

  pObj->gas_value = (float_t)data;

  return SGP40_OK;
}

/**
  * @brief  Wrap Read register component function to Bus IO function
  * @param  Handle the device handler
  * @param  Reg the register address
  * @param  pData the stored data pointer
  * @param  Length the length
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length)
{
  SGP40_Object_t *pObj = (SGP40_Object_t *)Handle;
  (void)Reg;

  if (pObj->IO.BusType == (uint32_t)SGP40_I2C_BUS) /* I2C */
  {
    return pObj->IO.Read(pObj->IO.Address, pData, Length);
  }
  else
  {
    return SGP40_ERROR;
  }
}

/**
  * @brief  Wrap Write register component function to Bus IO function
  * @param  Handle the device handler
  * @param  Reg the register address
  * @param  pData the stored data pointer
  * @param  Length the length
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length)
{
  SGP40_Object_t *pObj = (SGP40_Object_t *)Handle;
  (void)Reg;

  if (pObj->IO.BusType == (uint32_t)SGP40_I2C_BUS) /* I2C */
  {
    return pObj->IO.Write(pObj->IO.Address, pData, Length);
  }
  else
  {
    return SGP40_ERROR;
  }
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
