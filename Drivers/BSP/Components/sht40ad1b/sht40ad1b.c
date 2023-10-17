/**
  ******************************************************************************
  * @file    sht40ad1b.c
  * @author  MEMS Software Solutions Team
  * @brief   SHT40AD1B driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014-2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sht40ad1b.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup SHT40AD1B SHT40AD1B
  * @{
  */

/** @defgroup SHT40AD1B_Exported_Variables SHT40AD1B Exported Variables
  * @{
  */

SHT40AD1B_CommonDrv_t SHT40AD1B_COMMON_Driver =
{
  SHT40AD1B_Init,
  SHT40AD1B_DeInit,
  SHT40AD1B_ReadID,
  SHT40AD1B_GetCapabilities,
};

SHT40AD1B_HUM_Drv_t SHT40AD1B_HUM_Driver =
{
  SHT40AD1B_HUM_Enable,
  SHT40AD1B_HUM_Disable,
  SHT40AD1B_HUM_GetOutputDataRate,
  SHT40AD1B_HUM_SetOutputDataRate,
  SHT40AD1B_HUM_GetHumidity,
};

SHT40AD1B_TEMP_Drv_t SHT40AD1B_TEMP_Driver =
{
  SHT40AD1B_TEMP_Enable,
  SHT40AD1B_TEMP_Disable,
  SHT40AD1B_TEMP_GetOutputDataRate,
  SHT40AD1B_TEMP_SetOutputDataRate,
  SHT40AD1B_TEMP_GetTemperature,
};

/**
  * @}
  */

/** @defgroup SHT40AD1B_Private_Function_Prototypes SHT40AD1B Private Function Prototypes
  * @{
  */

static int32_t GetData(SHT40AD1B_Object_t *pObj);
static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);

/**
  * @}
  */

/** @defgroup SHT40AD1B_Exported_Functions SHT40AD1B Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SHT40AD1B_RegisterBusIO(SHT40AD1B_Object_t *pObj, SHT40AD1B_IO_t *pIO)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = SHT40AD1B_ERROR;
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
      ret = SHT40AD1B_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Initialize the SHT40AD1B sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SHT40AD1B_Init(SHT40AD1B_Object_t *pObj)
{
  /* This device doesn't support ODR, works more like one-shot measurement */
  pObj->hum_odr  = 0.0f;
  pObj->temp_odr = 0.0f;

  pObj->is_initialized = 1;

  return SHT40AD1B_OK;
}

/**
  * @brief  Deinitialize the SHT40AD1B sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SHT40AD1B_DeInit(SHT40AD1B_Object_t *pObj)
{
  pObj->is_initialized = 0;

  return SHT40AD1B_OK;
}

/**
  * @brief  Get WHO_AM_I value
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SHT40AD1B_ReadID(SHT40AD1B_Object_t *pObj, uint8_t *Id)
{
  return sht40ad1b_device_id_get(&(pObj->Ctx), Id);
}

/**
  * @brief  Get SHT40AD1B sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to SHT40AD1B sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SHT40AD1B_GetCapabilities(SHT40AD1B_Object_t *pObj, SHT40AD1B_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Humidity    = 1;
  Capabilities->Pressure    = 0;
  Capabilities->Temperature = 1;
  Capabilities->Gas         = 0;
  Capabilities->LowPower    = 0;
  Capabilities->HumMaxOdr   = 0.0f;
  Capabilities->TempMaxOdr  = 0.0f;
  Capabilities->PressMaxOdr = 0.0f;
  Capabilities->GasMaxOdr   = 0.0f;
  return SHT40AD1B_OK;
}

/**
  * @brief  Get the SHT40AD1B initialization status
  * @param  pObj the device pObj
  * @param  Status 1 if initialized, 0 otherwise
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SHT40AD1B_Get_Init_Status(SHT40AD1B_Object_t *pObj, uint8_t *Status)
{
  if (pObj == NULL)
  {
    return SHT40AD1B_ERROR;
  }

  *Status = pObj->is_initialized;

  return SHT40AD1B_OK;
}

/**
  * @brief  Enable the SHT40AD1B humidity sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SHT40AD1B_HUM_Enable(SHT40AD1B_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->hum_is_enabled == 1U)
  {
    return SHT40AD1B_OK;
  }

  pObj->hum_is_enabled = 1;

  return SHT40AD1B_OK;
}

/**
  * @brief  Disable the SHT40AD1B humidity sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SHT40AD1B_HUM_Disable(SHT40AD1B_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->hum_is_enabled == 0U)
  {
    return SHT40AD1B_OK;
  }

  pObj->hum_is_enabled = 0;

  return SHT40AD1B_OK;
}

/**
  * @brief  Get the SHT40AD1B humidity sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SHT40AD1B_HUM_GetOutputDataRate(SHT40AD1B_Object_t *pObj, float_t *Odr)
{
  *Odr = pObj->hum_odr;

  return SHT40AD1B_OK;
}

/**
  * @brief  Set the SHT40AD1B humidity sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SHT40AD1B_HUM_SetOutputDataRate(SHT40AD1B_Object_t *pObj, float_t Odr)
{
  /* This device doesn't support ODR, works more like one-shot measurement */
  return SHT40AD1B_OK;
}

/**
  * @brief  Get the SHT40AD1B humidity value
  * @param  pObj the device pObj
  * @param  Value pointer where the humidity value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SHT40AD1B_HUM_GetHumidity(SHT40AD1B_Object_t *pObj, float_t *Value)
{
  if (GetData(pObj) != SHT40AD1B_OK)
  {
    return SHT40AD1B_ERROR;
  }

  *Value = pObj->hum_value;

  return SHT40AD1B_OK;
}

/**
  * @brief  Get the SHT40AD1B humidity new data status
  * @param  pObj the device pObj
  * @param  Status the new data status
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SHT40AD1B_HUM_Get_DRDY_Status(SHT40AD1B_Object_t *pObj, uint8_t *Status)
{
  *Status = 1;

  return SHT40AD1B_OK;
}

/**
  * @brief  Enable the SHT40AD1B temperature sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SHT40AD1B_TEMP_Enable(SHT40AD1B_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->temp_is_enabled == 1U)
  {
    return SHT40AD1B_OK;
  }

  pObj->temp_is_enabled = 1;

  return SHT40AD1B_OK;
}

/**
  * @brief  Disable the SHT40AD1B temperature sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SHT40AD1B_TEMP_Disable(SHT40AD1B_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->temp_is_enabled == 0U)
  {
    return SHT40AD1B_OK;
  }

  pObj->temp_is_enabled = 0;

  return SHT40AD1B_OK;
}

/**
  * @brief  Get the SHT40AD1B temperature sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SHT40AD1B_TEMP_GetOutputDataRate(SHT40AD1B_Object_t *pObj, float_t *Odr)
{
  *Odr = pObj->temp_odr;

  return SHT40AD1B_OK;
}

/**
  * @brief  Set the SHT40AD1B temperature sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SHT40AD1B_TEMP_SetOutputDataRate(SHT40AD1B_Object_t *pObj, float_t Odr)
{
  /* This device doesn't support ODR, works more like one-shot measurement */
  return SHT40AD1B_OK;
}

/**
  * @brief  Get the SHT40AD1B temperature value
  * @param  pObj the device pObj
  * @param  Value pointer where the temperature value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SHT40AD1B_TEMP_GetTemperature(SHT40AD1B_Object_t *pObj, float_t *Value)
{
  if (GetData(pObj) != SHT40AD1B_OK)
  {
    return SHT40AD1B_ERROR;
  }

  *Value = pObj->temp_value;

  return SHT40AD1B_OK;
}

/**
  * @brief  Get the SHT40AD1B temperature new data status
  * @param  pObj the device pObj
  * @param  Status the new data status
  * @retval 0 in case of success, an error code otherwise
  */
int32_t SHT40AD1B_TEMP_Get_DRDY_Status(SHT40AD1B_Object_t *pObj, uint8_t *Status)
{
  *Status = 1;

  return SHT40AD1B_OK;
}

/**
  * @}
  */

/** @defgroup SHT40AD1B_Private_Functions SHT40AD1B Private Functions
  * @{
  */

/**
  * @brief  Get the SHT40AD1B humidity new data status
  * @param  pObj the device pObj
  * @param  Status the new data status
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t GetData(SHT40AD1B_Object_t *pObj)
{
  float_t data[2];  /* humidity, temperature */

  if (sht40ad1b_data_get(&(pObj->Ctx), data) != 0)
  {
    return SHT40AD1B_ERROR;
  }

  pObj->hum_value = data[0];
  pObj->temp_value = data[1];

  return SHT40AD1B_OK;
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
  SHT40AD1B_Object_t *pObj = (SHT40AD1B_Object_t *)Handle;
  (void)Reg;

  if (pObj->IO.BusType == (uint32_t)SHT40AD1B_I2C_BUS) /* I2C */
  {
    return pObj->IO.Read(pObj->IO.Address, pData, Length);
  }
  else
  {
    return SHT40AD1B_ERROR;
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
  SHT40AD1B_Object_t *pObj = (SHT40AD1B_Object_t *)Handle;
  (void)Reg;

  if (pObj->IO.BusType == (uint32_t)SHT40AD1B_I2C_BUS) /* I2C */
  {
    return pObj->IO.Write(pObj->IO.Address, pData, Length);
  }
  else
  {
    return SHT40AD1B_ERROR;
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
