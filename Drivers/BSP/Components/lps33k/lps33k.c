/**
  ******************************************************************************
  * @file    lps33k.c
  * @author  MEMS Software Solutions Team
  * @brief   LPS33K driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lps33k.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup LPS33K LPS33K
  * @{
  */

/** @defgroup LPS33K_Exported_Variables LPS33K Exported Variables
  * @{
  */

LPS33K_CommonDrv_t LPS33K_COMMON_Driver =
{
  LPS33K_Init,
  LPS33K_DeInit,
  LPS33K_ReadID,
  LPS33K_GetCapabilities,
};

LPS33K_PRESS_Drv_t LPS33K_PRESS_Driver =
{
  LPS33K_PRESS_Enable,
  LPS33K_PRESS_Disable,
  LPS33K_PRESS_GetOutputDataRate,
  LPS33K_PRESS_SetOutputDataRate,
  LPS33K_PRESS_GetPressure,
};

LPS33K_TEMP_Drv_t LPS33K_TEMP_Driver =
{
  LPS33K_TEMP_Enable,
  LPS33K_TEMP_Disable,
  LPS33K_TEMP_GetOutputDataRate,
  LPS33K_TEMP_SetOutputDataRate,
  LPS33K_TEMP_GetTemperature,
};

/**
  * @}
  */

/** @defgroup LPS33K_Private_Function_Prototypes LPS33K Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t LPS33K_GetOutputDataRate(LPS33K_Object_t *pObj, float *Odr);
static int32_t LPS33K_SetOutputDataRate_When_Enabled(LPS33K_Object_t *pObj, float Odr);
static int32_t LPS33K_SetOutputDataRate_When_Disabled(LPS33K_Object_t *pObj, float Odr);
static int32_t LPS33K_Initialize(LPS33K_Object_t *pObj);

/**
  * @}
  */

/** @defgroup LPS33K_Exported_Functions LPS33K Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33K_RegisterBusIO(LPS33K_Object_t *pObj, LPS33K_IO_t *pIO)
{
  int32_t ret = LPS33K_OK;

  if (pObj == NULL)
  {
    ret = LPS33K_ERROR;
  }
  else
  {
    pObj->IO.Init      = pIO->Init;
    pObj->IO.DeInit    = pIO->DeInit;
    pObj->IO.BusType   = pIO->BusType;
    pObj->IO.Address   = pIO->Address;
    pObj->IO.WriteReg  = pIO->WriteReg;
    pObj->IO.ReadReg   = pIO->ReadReg;
    pObj->IO.GetTick   = pIO->GetTick;

    pObj->Ctx.read_reg  = ReadRegWrap;
    pObj->Ctx.write_reg = WriteRegWrap;
    pObj->Ctx.mdelay    = pIO->Delay;
    pObj->Ctx.handle   = pObj;

    if (pObj->IO.Init == NULL)
    {
      ret = LPS33K_ERROR;
    }
    else if (pObj->IO.Init() != LPS33K_OK)
    {
      ret = LPS33K_ERROR;
    }
    else
    {
      ret = LPS33K_OK;
    }
  }

  return ret;
}

/**
  * @brief  Initialize the LPS33K sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33K_Init(LPS33K_Object_t *pObj)
{
  if (pObj->is_initialized == 0U)
  {
    if (LPS33K_Initialize(pObj) != LPS33K_OK)
    {
      return LPS33K_ERROR;
    }
  }

  pObj->is_initialized = 1U;

  return LPS33K_OK;
}

/**
  * @brief  Deinitialize the LPS33K sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33K_DeInit(LPS33K_Object_t *pObj)
{
  if (pObj->is_initialized == 1U)
  {
    if (LPS33K_PRESS_Disable(pObj) != LPS33K_OK)
    {
      return LPS33K_ERROR;
    }

    if (LPS33K_TEMP_Disable(pObj) != LPS33K_OK)
    {
      return LPS33K_ERROR;
    }
  }

  pObj->is_initialized = 0;

  return LPS33K_OK;
}

/**
  * @brief  Get WHO_AM_I value
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33K_ReadID(LPS33K_Object_t *pObj, uint8_t *Id)
{
  if (lps33k_device_id_get(&(pObj->Ctx), Id) != LPS33K_OK)
  {
    return LPS33K_ERROR;
  }

  return LPS33K_OK;
}

/**
  * @brief  Get LPS33K sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to LPS33K sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33K_GetCapabilities(LPS33K_Object_t *pObj, LPS33K_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Humidity    = 0;
  Capabilities->Pressure    = 1;
  Capabilities->Temperature = 1;
  Capabilities->Gas         = 0;
  Capabilities->LowPower    = 1;
  Capabilities->HumMaxOdr   = 0.0f;
  Capabilities->TempMaxOdr  = 75.0f;
  Capabilities->PressMaxOdr = 75.0f;
  Capabilities->GasMaxOdr   = 0.0f;
  return LPS33K_OK;
}

/**
  * @brief  Get the LPS33K initialization status
  * @param  pObj the device pObj
  * @param  Status 1 if initialized, 0 otherwise
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33K_Get_Init_Status(LPS33K_Object_t *pObj, uint8_t *Status)
{
  if (pObj == NULL)
  {
    return LPS33K_ERROR;
  }

  *Status = pObj->is_initialized;

  return LPS33K_OK;
}

/**
  * @brief  Enable the LPS33K pressure sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33K_PRESS_Enable(LPS33K_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->press_is_enabled == 1U)
  {
    return LPS33K_OK;
  }

  /* Output data rate selection. */
  if (lps33k_data_rate_set(&(pObj->Ctx), pObj->last_odr) != LPS33K_OK)
  {
    return LPS33K_ERROR;
  }

  pObj->press_is_enabled = 1;

  return LPS33K_OK;
}

/**
  * @brief  Disable the LPS33K pressure sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33K_PRESS_Disable(LPS33K_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->press_is_enabled == 0U)
  {
    return LPS33K_OK;
  }

  /* Check if the LPS33K temperature sensor is still enable. */
  /* If yes, skip the disable function, if not call disable function */
  if (pObj->temp_is_enabled == 0U)
  {
    /* Get current output data rate. */
    if (lps33k_data_rate_get(&(pObj->Ctx), &pObj->last_odr) != LPS33K_OK)
    {
      return LPS33K_ERROR;
    }

    /* Output data rate selection - power down. */
    if (lps33k_data_rate_set(&(pObj->Ctx), LPS33K_POWER_DOWN) != LPS33K_OK)
    {
      return LPS33K_ERROR;
    }
  }

  pObj->press_is_enabled = 0;

  return LPS33K_OK;
}

/**
  * @brief  Get the LPS33K pressure sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33K_PRESS_GetOutputDataRate(LPS33K_Object_t *pObj, float *Odr)
{
  return LPS33K_GetOutputDataRate(pObj, Odr);
}

/**
  * @brief  Set the LPS33K pressure sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33K_PRESS_SetOutputDataRate(LPS33K_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->press_is_enabled == 1U)
  {
    return LPS33K_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return LPS33K_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the LPS33K pressure value
  * @param  pObj the device pObj
  * @param  Value pointer where the pressure value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33K_PRESS_GetPressure(LPS33K_Object_t *pObj, float *Value)
{
  lps33k_axis1bit32_t data_raw_pressure;

  (void)memset(data_raw_pressure.u8bit, 0x00, sizeof(int32_t));
  if (lps33k_pressure_raw_get(&(pObj->Ctx), (uint32_t *)&data_raw_pressure.i32bit) != LPS33K_OK)
  {
    return LPS33K_ERROR;
  }

  *Value = lps33k_from_lsb_to_hpa(data_raw_pressure.i32bit);

  return LPS33K_OK;
}

/**
  * @brief  Get the LPS33K pressure data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33K_PRESS_Get_DRDY_Status(LPS33K_Object_t *pObj, uint8_t *Status)
{
  if (lps33k_press_data_ready_get(&(pObj->Ctx), Status) != LPS33K_OK)
  {
    return LPS33K_ERROR;
  }

  return LPS33K_OK;
}

/**
  * @brief  Enable the LPS33K temperature sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33K_TEMP_Enable(LPS33K_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->temp_is_enabled == 1U)
  {
    return LPS33K_OK;
  }

  /* Output data rate selection. */
  if (lps33k_data_rate_set(&(pObj->Ctx), pObj->last_odr) != LPS33K_OK)
  {
    return LPS33K_ERROR;
  }

  pObj->temp_is_enabled = 1;

  return LPS33K_OK;
}

/**
  * @brief  Disable the LPS33K temperature sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33K_TEMP_Disable(LPS33K_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->temp_is_enabled == 0U)
  {
    return LPS33K_OK;
  }

  /* Check if the LPS33K pressure sensor is still enable. */
  /* If yes, skip the disable function, if not call disable function */
  if (pObj->press_is_enabled == 0U)
  {
    /* Get current output data rate. */
    if (lps33k_data_rate_get(&(pObj->Ctx), &pObj->last_odr) != LPS33K_OK)
    {
      return LPS33K_ERROR;
    }

    /* Output data rate selection - power down. */
    if (lps33k_data_rate_set(&(pObj->Ctx), LPS33K_POWER_DOWN) != LPS33K_OK)
    {
      return LPS33K_ERROR;
    }
  }

  pObj->temp_is_enabled = 0;

  return LPS33K_OK;
}

/**
  * @brief  Get the LPS33K temperature sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33K_TEMP_GetOutputDataRate(LPS33K_Object_t *pObj, float *Odr)
{
  return LPS33K_GetOutputDataRate(pObj, Odr);
}

/**
  * @brief  Set the LPS33K temperature sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33K_TEMP_SetOutputDataRate(LPS33K_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->temp_is_enabled == 1U)
  {
    return LPS33K_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return LPS33K_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the LPS33K temperature value
  * @param  pObj the device pObj
  * @param  Value pointer where the temperature value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33K_TEMP_GetTemperature(LPS33K_Object_t *pObj, float *Value)
{
  lps33k_axis1bit16_t data_raw_temperature;

  (void)memset(data_raw_temperature.u8bit, 0x00, sizeof(int16_t));
  if (lps33k_temperature_raw_get(&(pObj->Ctx), &data_raw_temperature.i16bit) != LPS33K_OK)
  {
    return LPS33K_ERROR;
  }

  *Value = lps33k_from_lsb_to_degc(data_raw_temperature.i16bit);

  return LPS33K_OK;
}

/**
  * @brief  Get the LPS33K temperature data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33K_TEMP_Get_DRDY_Status(LPS33K_Object_t *pObj, uint8_t *Status)
{
  if (lps33k_temp_data_ready_get(&(pObj->Ctx), Status) != LPS33K_OK)
  {
    return LPS33K_ERROR;
  }

  return LPS33K_OK;
}

/**
  * @brief  Get the LPS33K register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33K_Read_Reg(LPS33K_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (lps33k_read_reg(&(pObj->Ctx), Reg, Data, 1) != LPS33K_OK)
  {
    return LPS33K_ERROR;
  }

  return LPS33K_OK;
}

/**
  * @brief  Set the LPS33K register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33K_Write_Reg(LPS33K_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (lps33k_write_reg(&(pObj->Ctx), Reg, &Data, 1) != LPS33K_OK)
  {
    return LPS33K_ERROR;
  }

  return LPS33K_OK;
}

/**
  * @}
  */

/** @defgroup LPS33K_Private_Functions LPS33K Private Functions
  * @{
  */

/**
  * @brief  Get output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LPS33K_GetOutputDataRate(LPS33K_Object_t *pObj, float *Odr)
{
  int32_t ret = LPS33K_OK;
  lps33k_odr_t odr_low_level;

  if (lps33k_data_rate_get(&(pObj->Ctx), &odr_low_level) != LPS33K_OK)
  {
    return LPS33K_ERROR;
  }

  switch (odr_low_level)
  {
    case LPS33K_POWER_DOWN:
      *Odr = 0.0f;
      break;

    case LPS33K_ODR_1_Hz:
      *Odr = 1.0f;
      break;

    case LPS33K_ODR_10_Hz:
      *Odr = 10.0f;
      break;

    case LPS33K_ODR_25_Hz:
      *Odr = 25.0f;
      break;

    case LPS33K_ODR_50_Hz:
      *Odr = 50.0f;
      break;

    case LPS33K_ODR_75_Hz:
      *Odr = 75.0f;
      break;

    default:
      ret = LPS33K_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LPS33K_SetOutputDataRate_When_Enabled(LPS33K_Object_t *pObj, float Odr)
{
  lps33k_odr_t new_odr;

  new_odr = (Odr <=  1.0f) ? LPS33K_ODR_1_Hz
            : (Odr <= 10.0f) ? LPS33K_ODR_10_Hz
            : (Odr <= 25.0f) ? LPS33K_ODR_25_Hz
            : (Odr <= 50.0f) ? LPS33K_ODR_50_Hz
            :                  LPS33K_ODR_75_Hz;

  if (lps33k_data_rate_set(&(pObj->Ctx), new_odr) != LPS33K_OK)
  {
    return LPS33K_ERROR;
  }

  if (lps33k_data_rate_get(&(pObj->Ctx), &pObj->last_odr) != LPS33K_OK)
  {
    return LPS33K_ERROR;
  }

  return LPS33K_OK;
}

/**
  * @brief  Set output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LPS33K_SetOutputDataRate_When_Disabled(LPS33K_Object_t *pObj, float Odr)
{
  pObj->last_odr = (Odr <=  1.0f) ? LPS33K_ODR_1_Hz
                   : (Odr <= 10.0f) ? LPS33K_ODR_10_Hz
                   : (Odr <= 25.0f) ? LPS33K_ODR_25_Hz
                   : (Odr <= 50.0f) ? LPS33K_ODR_50_Hz
                   :                  LPS33K_ODR_75_Hz;

  return LPS33K_OK;
}

/**
  * @brief  Initialize the LPS33K sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LPS33K_Initialize(LPS33K_Object_t *pObj)
{
  /* Power down the device */
  if (lps33k_data_rate_set(&(pObj->Ctx), LPS33K_POWER_DOWN) != LPS33K_OK)
  {
    return LPS33K_ERROR;
  }

  /* Set Power mode */
  if (lps33k_low_power_set(&(pObj->Ctx), PROPERTY_ENABLE) != LPS33K_OK)
  {
    return LPS33K_ERROR;
  }

  /* Disable low-pass filter on LPS33K pressure data */
  if (lps33k_low_pass_filter_mode_set(&(pObj->Ctx), LPS33K_LPF_ODR_DIV_2) != LPS33K_OK)
  {
    return LPS33K_ERROR;
  }

  /* Set block data update mode */
  if (lps33k_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != LPS33K_OK)
  {
    return LPS33K_ERROR;
  }

  /* Set autoincrement for multi-byte read/write */
  if (lps33k_auto_add_inc_set(&(pObj->Ctx), PROPERTY_ENABLE) != LPS33K_OK)
  {
    return LPS33K_ERROR;
  }

  pObj->last_odr = LPS33K_ODR_25_Hz;

  return LPS33K_OK;
}

/**
  * @brief  Set the LPS33K One Shot Mode
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33K_Set_One_Shot(LPS33K_Object_t *pObj)
{
  /* Set ODR */
  if (lps33k_data_rate_set(&(pObj->Ctx), LPS33K_POWER_DOWN) != LPS33K_OK)
  {
    return LPS33K_ERROR;
  }

  /* Start One Shot Measurement */
  if (lps33k_one_shoot_trigger_set(&(pObj->Ctx), PROPERTY_ENABLE) != LPS33K_OK)
  {
    return LPS33K_ERROR;
  }

  return LPS33K_OK;
}

/**
  * @brief  Get the LPS33K One Shot Status
  * @param  pObj the device pObj
  * @param  Status pointer to the one shot status (1 means measurements available, 0 means measurements not available yet)
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33K_Get_One_Shot_Status(LPS33K_Object_t *pObj, uint8_t *Status)
{
  uint8_t p_da;
  uint8_t t_da;

  /* Get DataReady for pressure */
  if (lps33k_press_data_ready_get(&(pObj->Ctx), &p_da) != LPS33K_OK)
  {
    return LPS33K_ERROR;
  }

  /* Get DataReady for temperature */
  if (lps33k_temp_data_ready_get(&(pObj->Ctx), &t_da) != LPS33K_OK)
  {
    return LPS33K_ERROR;
  }

  if (p_da && t_da)
  {
    *Status = 1;
  }
  else
  {
    *Status = 0;
  }

  return LPS33K_OK;
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
  LPS33K_Object_t *pObj = (LPS33K_Object_t *)Handle;

  return pObj->IO.ReadReg(pObj->IO.Address, Reg, pData, Length);
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
  LPS33K_Object_t *pObj = (LPS33K_Object_t *)Handle;

  return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
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
