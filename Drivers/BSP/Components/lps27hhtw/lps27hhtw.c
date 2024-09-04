/**
  ******************************************************************************
  * @file    lps27hhtw.c
  * @author  MEMS Software Solutions Team
  * @brief   LPS27HHTW driver file
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
#include "lps27hhtw.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup LPS27HHTW LPS27HHTW
  * @{
  */

/** @defgroup LPS27HHTW_Exported_Variables LPS27HHTW Exported Variables
  * @{
  */

LPS27HHTW_CommonDrv_t LPS27HHTW_COMMON_Driver =
{
  LPS27HHTW_Init,
  LPS27HHTW_DeInit,
  LPS27HHTW_ReadID,
  LPS27HHTW_GetCapabilities,
};

LPS27HHTW_PRESS_Drv_t LPS27HHTW_PRESS_Driver =
{
  LPS27HHTW_PRESS_Enable,
  LPS27HHTW_PRESS_Disable,
  LPS27HHTW_PRESS_GetOutputDataRate,
  LPS27HHTW_PRESS_SetOutputDataRate,
  LPS27HHTW_PRESS_GetPressure,
};

LPS27HHTW_TEMP_Drv_t LPS27HHTW_TEMP_Driver =
{
  LPS27HHTW_TEMP_Enable,
  LPS27HHTW_TEMP_Disable,
  LPS27HHTW_TEMP_GetOutputDataRate,
  LPS27HHTW_TEMP_SetOutputDataRate,
  LPS27HHTW_TEMP_GetTemperature,
};

/**
  * @}
  */

/** @defgroup LPS27HHTW_Private_Function_Prototypes LPS27HHTW Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t LPS27HHTW_GetOutputDataRate(LPS27HHTW_Object_t *pObj, float *Odr);
static int32_t LPS27HHTW_SetOutputDataRate_When_Enabled(LPS27HHTW_Object_t *pObj, float Odr);
static int32_t LPS27HHTW_SetOutputDataRate_When_Disabled(LPS27HHTW_Object_t *pObj, float Odr);
static int32_t LPS27HHTW_Initialize(LPS27HHTW_Object_t *pObj);

/**
  * @}
  */

/** @defgroup LPS27HHTW_Exported_Functions LPS27HHTW Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_RegisterBusIO(LPS27HHTW_Object_t *pObj, LPS27HHTW_IO_t *pIO)
{
  int32_t ret = LPS27HHTW_OK;

  if (pObj == NULL)
  {
    ret = LPS27HHTW_ERROR;
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
    pObj->Ctx.handle    = pObj;

    if (pObj->IO.Init == NULL)
    {
      ret = LPS27HHTW_ERROR;
    }
    else if (pObj->IO.Init() != LPS27HHTW_OK)
    {
      ret = LPS27HHTW_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == LPS27HHTW_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x01;

          if (LPS27HHTW_Write_Reg(pObj, LPS27HHTW_CTRL_REG1, data) != LPS27HHTW_OK)
          {
            ret = LPS27HHTW_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  Initialize the LPS27HHTW sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_Init(LPS27HHTW_Object_t *pObj)
{
  if (pObj->is_initialized == 0U)
  {
    if (LPS27HHTW_Initialize(pObj) != LPS27HHTW_OK)
    {
      return LPS27HHTW_ERROR;
    }
  }

  pObj->is_initialized = 1U;

  return LPS27HHTW_OK;
}

/**
  * @brief  Deinitialize the LPS27HHTW sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_DeInit(LPS27HHTW_Object_t *pObj)
{
  if (pObj->is_initialized == 1U)
  {
    if (LPS27HHTW_PRESS_Disable(pObj) != LPS27HHTW_OK)
    {
      return LPS27HHTW_ERROR;
    }

    if (LPS27HHTW_TEMP_Disable(pObj) != LPS27HHTW_OK)
    {
      return LPS27HHTW_ERROR;
    }
  }

  pObj->is_initialized = 0;

  return LPS27HHTW_OK;
}

/**
  * @brief  Get WHO_AM_I value
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_ReadID(LPS27HHTW_Object_t *pObj, uint8_t *Id)
{
  if (lps27hhtw_device_id_get(&(pObj->Ctx), Id) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  return LPS27HHTW_OK;
}

/**
  * @brief  Get LPS27HHTW sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to LPS27HHTW sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_GetCapabilities(LPS27HHTW_Object_t *pObj, LPS27HHTW_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Humidity    = 0;
  Capabilities->Pressure    = 1;
  Capabilities->Temperature = 1;
  Capabilities->Gas         = 0;
  Capabilities->LowPower    = 0;
  Capabilities->HumMaxOdr   = 0.0f;
  Capabilities->TempMaxOdr  = 200.0f;
  Capabilities->PressMaxOdr = 200.0f;
  Capabilities->GasMaxOdr   = 0.0f;
  return LPS27HHTW_OK;
}

/**
  * @brief  Get the LPS27HHTW initialization status
  * @param  pObj the device pObj
  * @param  Status 1 if initialized, 0 otherwise
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_Get_Init_Status(LPS27HHTW_Object_t *pObj, uint8_t *Status)
{
  if (pObj == NULL)
  {
    return LPS27HHTW_ERROR;
  }

  *Status = pObj->is_initialized;

  return LPS27HHTW_OK;
}

/**
  * @brief  Enable the LPS27HHTW pressure sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_PRESS_Enable(LPS27HHTW_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->press_is_enabled == 1U)
  {
    return LPS27HHTW_OK;
  }

  /* Output data rate selection. */
  if (lps27hhtw_data_rate_set(&(pObj->Ctx), pObj->last_odr) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  pObj->press_is_enabled = 1;

  return LPS27HHTW_OK;
}

/**
  * @brief  Disable the LPS27HHTW pressure sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_PRESS_Disable(LPS27HHTW_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->press_is_enabled == 0U)
  {
    return LPS27HHTW_OK;
  }

  /* Check if the LPS27HHTW temperature sensor is still enable. */
  /* If yes, skip the disable function, if not call disable function */
  if (pObj->temp_is_enabled == 0U)
  {
    /* Get current output data rate. */
    if (lps27hhtw_data_rate_get(&(pObj->Ctx), &pObj->last_odr) != LPS27HHTW_OK)
    {
      return LPS27HHTW_ERROR;
    }

    /* Output data rate selection - power down. */
    if (lps27hhtw_data_rate_set(&(pObj->Ctx), LPS27HHTW_POWER_DOWN) != LPS27HHTW_OK)
    {
      return LPS27HHTW_ERROR;
    }
  }

  pObj->press_is_enabled = 0;

  return LPS27HHTW_OK;
}

/**
  * @brief  Get the LPS27HHTW pressure sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_PRESS_GetOutputDataRate(LPS27HHTW_Object_t *pObj, float *Odr)
{
  return LPS27HHTW_GetOutputDataRate(pObj, Odr);
}

/**
  * @brief  Set the LPS27HHTW pressure sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_PRESS_SetOutputDataRate(LPS27HHTW_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->press_is_enabled == 1U)
  {
    return LPS27HHTW_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return LPS27HHTW_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the LPS27HHTW pressure value
  * @param  pObj the device pObj
  * @param  Value pointer where the pressure value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_PRESS_GetPressure(LPS27HHTW_Object_t *pObj, float *Value)
{
  lps27hhtw_axis1bit32_t data_raw_pressure;

  (void)memset(data_raw_pressure.u8bit, 0x00, sizeof(int32_t));
  if (lps27hhtw_pressure_raw_get(&(pObj->Ctx), (uint32_t *)&data_raw_pressure.i32bit) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  *Value = lps27hhtw_from_lsb_to_hpa(data_raw_pressure.i32bit);

  return LPS27HHTW_OK;
}

/**
  * @brief  Get the LPS27HHTW pressure data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_PRESS_Get_DRDY_Status(LPS27HHTW_Object_t *pObj, uint8_t *Status)
{
  if (lps27hhtw_press_flag_data_ready_get(&(pObj->Ctx), Status) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  return LPS27HHTW_OK;
}

/**
  * @brief  Enable the LPS27HHTW temperature sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_TEMP_Enable(LPS27HHTW_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->temp_is_enabled == 1U)
  {
    return LPS27HHTW_OK;
  }

  /* Output data rate selection. */
  if (lps27hhtw_data_rate_set(&(pObj->Ctx), pObj->last_odr) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  pObj->temp_is_enabled = 1;

  return LPS27HHTW_OK;
}

/**
  * @brief  Disable the LPS27HHTW temperature sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_TEMP_Disable(LPS27HHTW_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->temp_is_enabled == 0U)
  {
    return LPS27HHTW_OK;
  }

  /* Check if the LPS27HHTW pressure sensor is still enable. */
  /* If yes, skip the disable function, if not call disable function */
  if (pObj->press_is_enabled == 0U)
  {
    /* Get current output data rate. */
    if (lps27hhtw_data_rate_get(&(pObj->Ctx), &pObj->last_odr) != LPS27HHTW_OK)
    {
      return LPS27HHTW_ERROR;
    }

    /* Output data rate selection - power down. */
    if (lps27hhtw_data_rate_set(&(pObj->Ctx), LPS27HHTW_POWER_DOWN) != LPS27HHTW_OK)
    {
      return LPS27HHTW_ERROR;
    }
  }

  pObj->temp_is_enabled = 0;

  return LPS27HHTW_OK;
}

/**
  * @brief  Get the LPS27HHTW temperature sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_TEMP_GetOutputDataRate(LPS27HHTW_Object_t *pObj, float *Odr)
{
  return LPS27HHTW_GetOutputDataRate(pObj, Odr);
}

/**
  * @brief  Set the LPS27HHTW temperature sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_TEMP_SetOutputDataRate(LPS27HHTW_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->temp_is_enabled == 1U)
  {
    return LPS27HHTW_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return LPS27HHTW_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the LPS27HHTW temperature value
  * @param  pObj the device pObj
  * @param  Value pointer where the temperature value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_TEMP_GetTemperature(LPS27HHTW_Object_t *pObj, float *Value)
{
  lps27hhtw_axis1bit16_t data_raw_temperature;

  (void)memset(data_raw_temperature.u8bit, 0x00, sizeof(int16_t));
  if (lps27hhtw_temperature_raw_get(&(pObj->Ctx), &data_raw_temperature.i16bit) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  *Value = lps27hhtw_from_lsb_to_celsius(data_raw_temperature.i16bit);

  return LPS27HHTW_OK;
}

/**
  * @brief  Get the LPS27HHTW temperature data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_TEMP_Get_DRDY_Status(LPS27HHTW_Object_t *pObj, uint8_t *Status)
{
  if (lps27hhtw_temp_flag_data_ready_get(&(pObj->Ctx), Status) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  return LPS27HHTW_OK;
}

/**
  * @brief  Get the LPS27HHTW register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_Read_Reg(LPS27HHTW_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (lps27hhtw_read_reg(&(pObj->Ctx), Reg, Data, 1) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  return LPS27HHTW_OK;
}

/**
  * @brief  Set the LPS27HHTW register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_Write_Reg(LPS27HHTW_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (lps27hhtw_write_reg(&(pObj->Ctx), Reg, &Data, 1) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  return LPS27HHTW_OK;
}

/**
  * @}
  */

/** @defgroup LPS27HHTW_Private_Functions LPS27HHTW Private Functions
  * @{
  */

/**
  * @brief  Get output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LPS27HHTW_GetOutputDataRate(LPS27HHTW_Object_t *pObj, float *Odr)
{
  int32_t ret = LPS27HHTW_OK;
  lps27hhtw_odr_t odr_low_level;

  if (lps27hhtw_data_rate_get(&(pObj->Ctx), &odr_low_level) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  switch (odr_low_level)
  {
    case LPS27HHTW_POWER_DOWN:
    case LPS27HHTW_ONE_SHOOT:
      *Odr = 0.0f;
      break;

    case LPS27HHTW_1_Hz:
    case LPS27HHTW_1_Hz_LOW_NOISE:
      *Odr = 1.0f;
      break;

    case LPS27HHTW_10_Hz:
    case LPS27HHTW_10_Hz_LOW_NOISE:
      *Odr = 10.0f;
      break;

    case LPS27HHTW_25_Hz:
    case LPS27HHTW_25_Hz_LOW_NOISE:
      *Odr = 25.0f;
      break;

    case LPS27HHTW_50_Hz:
    case LPS27HHTW_50_Hz_LOW_NOISE:
      *Odr = 50.0f;
      break;

    case LPS27HHTW_75_Hz:
    case LPS27HHTW_75_Hz_LOW_NOISE:
      *Odr = 75.0f;
      break;

    case LPS27HHTW_100_Hz:
      *Odr = 100.0f;
      break;

    case LPS27HHTW_200_Hz:
      *Odr = 200.0f;
      break;

    default:
      ret = LPS27HHTW_ERROR;
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
static int32_t LPS27HHTW_SetOutputDataRate_When_Enabled(LPS27HHTW_Object_t *pObj, float Odr)
{
  lps27hhtw_odr_t new_odr;

  new_odr = (Odr <=   1.0f) ? LPS27HHTW_1_Hz
            : (Odr <=  10.0f) ? LPS27HHTW_10_Hz
            : (Odr <=  25.0f) ? LPS27HHTW_25_Hz
            : (Odr <=  50.0f) ? LPS27HHTW_50_Hz
            : (Odr <=  75.0f) ? LPS27HHTW_75_Hz
            : (Odr <= 100.0f) ? LPS27HHTW_100_Hz
            :                   LPS27HHTW_200_Hz;

  if (lps27hhtw_data_rate_set(&(pObj->Ctx), new_odr) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  if (lps27hhtw_data_rate_get(&(pObj->Ctx), &pObj->last_odr) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  return LPS27HHTW_OK;
}

/**
  * @brief  Set output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LPS27HHTW_SetOutputDataRate_When_Disabled(LPS27HHTW_Object_t *pObj, float Odr)
{
  pObj->last_odr = (Odr <=   1.0f) ? LPS27HHTW_1_Hz
                   : (Odr <=  10.0f) ? LPS27HHTW_10_Hz
                   : (Odr <=  25.0f) ? LPS27HHTW_25_Hz
                   : (Odr <=  50.0f) ? LPS27HHTW_50_Hz
                   : (Odr <=  75.0f) ? LPS27HHTW_75_Hz
                   : (Odr <= 100.0f) ? LPS27HHTW_100_Hz
                   :                   LPS27HHTW_200_Hz;

  return LPS27HHTW_OK;
}

/**
  * @brief  Initialize the LPS27HHTW sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LPS27HHTW_Initialize(LPS27HHTW_Object_t *pObj)
{
  if(pObj->IO.BusType != LPS27HHTW_I3C_BUS)
  {
    /* Disable MIPI I3C(SM) interface */
    if (lps27hhtw_i3c_interface_set(&(pObj->Ctx), LPS27HHTW_I3C_DISABLE) != LPS27HHTW_OK)
    {
      return LPS27HHTW_ERROR;
    }
  }

  /* Power down the device, set Low Noise Enable (bit 5), clear One Shot (bit 4) */
  if (lps27hhtw_data_rate_set(&(pObj->Ctx), (lps27hhtw_odr_t)(LPS27HHTW_POWER_DOWN | 0x10)) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  /* Disable low-pass filter on LPS27HHTW pressure data */
  if (lps27hhtw_lp_bandwidth_set(&(pObj->Ctx), LPS27HHTW_LPF_ODR_DIV_2) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  /* Set block data update mode */
  if (lps27hhtw_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  /* Set autoincrement for multi-byte read/write */
  if (lps27hhtw_auto_increment_set(&(pObj->Ctx), PROPERTY_ENABLE) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  pObj->last_odr = LPS27HHTW_25_Hz;

  return LPS27HHTW_OK;
}

/**
  * @brief  Get the LPS27HHTW FIFO data level
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_FIFO_Get_Data(LPS27HHTW_Object_t *pObj, float *Press, float *Temp)
{
  lps27hhtw_axis1bit32_t data_raw_pressure;
  lps27hhtw_axis1bit16_t data_raw_temperature;

  (void)memset(data_raw_pressure.u8bit, 0x00, sizeof(int32_t));
  if (lps27hhtw_fifo_pressure_raw_get(&(pObj->Ctx), (uint32_t *)&data_raw_pressure.i32bit) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  *Press = lps27hhtw_from_lsb_to_hpa(data_raw_pressure.i32bit);

  (void)memset(data_raw_temperature.u8bit, 0x00, sizeof(int16_t));
  if (lps27hhtw_fifo_temperature_raw_get(&(pObj->Ctx), &data_raw_temperature.i16bit) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  *Temp = lps27hhtw_from_lsb_to_celsius(data_raw_temperature.i16bit);

  return LPS27HHTW_OK;
}

/**
  * @brief  Get the LPS27HHTW FIFO threshold
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_FIFO_Get_FTh_Status(LPS27HHTW_Object_t *pObj, uint8_t *Status)
{
  if (lps27hhtw_fifo_wtm_flag_get(&(pObj->Ctx), Status) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  return LPS27HHTW_OK;
}

/**
  * @brief  Get the LPS27HHTW FIFO full status
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_FIFO_Get_Full_Status(LPS27HHTW_Object_t *pObj, uint8_t *Status)
{
  if (lps27hhtw_fifo_full_flag_get(&(pObj->Ctx), Status) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  return LPS27HHTW_OK;
}

/**
  * @brief  Get the LPS27HHTW FIFO OVR status
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_FIFO_Get_Ovr_Status(LPS27HHTW_Object_t *pObj, uint8_t *Status)
{
  if (lps27hhtw_fifo_ovr_flag_get(&(pObj->Ctx), Status) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  return LPS27HHTW_OK;
}

/**
  * @brief  Get the LPS27HHTW FIFO data level
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_FIFO_Get_Level(LPS27HHTW_Object_t *pObj, uint8_t *Status)
{
  if (lps27hhtw_fifo_data_level_get(&(pObj->Ctx), Status) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  return LPS27HHTW_OK;
}

/**
  * @brief  Reset the FIFO interrupt
  * @param  pObj the device pObj
  * @param  interrupt The FIFO interrupt to be reset; values: 0 = FTH; 1 = FULL; 2 = OVR
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_FIFO_Reset_Interrupt(LPS27HHTW_Object_t *pObj, uint8_t interrupt)
{
  switch (interrupt)
  {
    case 0:
      if (lps27hhtw_fifo_threshold_on_int_set(&(pObj->Ctx), PROPERTY_DISABLE) != LPS27HHTW_OK)
      {
        return LPS27HHTW_ERROR;
      }
      break;
    case 1:
      if (lps27hhtw_fifo_full_on_int_set(&(pObj->Ctx), PROPERTY_DISABLE) != LPS27HHTW_OK)
      {
        return LPS27HHTW_ERROR;
      }
      break;
    case 2:
      if (lps27hhtw_fifo_ovr_on_int_set(&(pObj->Ctx), PROPERTY_DISABLE) != LPS27HHTW_OK)
      {
        return LPS27HHTW_ERROR;
      }
      break;
    default:
      return LPS27HHTW_ERROR;
  }

  return LPS27HHTW_OK;
}

/**
  * @brief  Set the FIFO interrupt
  * @param  pObj the device pObj
  * @param  interrupt The FIFO interrupt to be reset; values: 0 = FTH; 1 = FULL; 2 = OVR
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_FIFO_Set_Interrupt(LPS27HHTW_Object_t *pObj, uint8_t interrupt)
{
  switch (interrupt)
  {
    case 0:
      if (lps27hhtw_fifo_threshold_on_int_set(&(pObj->Ctx), PROPERTY_ENABLE) != LPS27HHTW_OK)
      {
        return LPS27HHTW_ERROR;
      }
      break;
    case 1:
      if (lps27hhtw_fifo_full_on_int_set(&(pObj->Ctx), PROPERTY_ENABLE) != LPS27HHTW_OK)
      {
        return LPS27HHTW_ERROR;
      }
      break;
    case 2:
      if (lps27hhtw_fifo_ovr_on_int_set(&(pObj->Ctx), PROPERTY_ENABLE) != LPS27HHTW_OK)
      {
        return LPS27HHTW_ERROR;
      }
      break;
    default:
      return LPS27HHTW_ERROR;
  }

  return LPS27HHTW_OK;
}

/**
  * @brief  Set the FIFO mode
  * @param  pObj the device pObj
  * @param  Mode the FIFO mode to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_FIFO_Set_Mode(LPS27HHTW_Object_t *pObj, uint8_t Mode)
{
  /* Verify that the passed parameter contains one of the valid values */
  switch ((lps27hhtw_f_mode_t)Mode)
  {
    case LPS27HHTW_BYPASS_MODE:
    case LPS27HHTW_FIFO_MODE:
    case LPS27HHTW_STREAM_MODE:
    case LPS27HHTW_STREAM_TO_FIFO_MODE:
    case LPS27HHTW_BYPASS_TO_STREAM_MODE:
    case LPS27HHTW_BYPASS_TO_FIFO_MODE:
      break;
    default:
      return LPS27HHTW_ERROR;
  }

  if (lps27hhtw_fifo_mode_set(&(pObj->Ctx), (lps27hhtw_f_mode_t)Mode) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  return LPS27HHTW_OK;
}

/**
  * @brief  Set the LPS27HHTW FIFO data level
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_FIFO_Set_Watermark_Level(LPS27HHTW_Object_t *pObj, uint8_t Watermark)
{
  if (lps27hhtw_fifo_watermark_set(&(pObj->Ctx), Watermark) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  return LPS27HHTW_OK;
}

/**
  * @brief  Set the LPS27HHTW stop on watermark function
  * @param  pObj the device pObj
  * @param  Stop the state of stop on watermark function
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_FIFO_Stop_On_Watermark(LPS27HHTW_Object_t *pObj, uint8_t Stop)
{
  if (lps27hhtw_fifo_stop_on_wtm_set(&(pObj->Ctx), Stop) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  return LPS27HHTW_OK;
}

/**
  * @brief  Set the LPS27HHTW One Shot Mode
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_Set_One_Shot(LPS27HHTW_Object_t *pObj)
{
  /* Start One Shot Measurement */
  if (lps27hhtw_data_rate_set(&(pObj->Ctx), LPS27HHTW_ONE_SHOOT) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  return LPS27HHTW_OK;
}

/**
  * @brief  Get the LPS27HHTW One Shot Status
  * @param  pObj the device pObj
  * @param  Status pointer to the one shot status (1 means measurements available, 0 means measurements not available yet)
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_Get_One_Shot_Status(LPS27HHTW_Object_t *pObj, uint8_t *Status)
{
  uint8_t p_da;
  uint8_t t_da;

  /* Get DataReady for pressure */
  if (lps27hhtw_press_flag_data_ready_get(&(pObj->Ctx), &p_da) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  /* Get DataReady for temperature */
  if (lps27hhtw_temp_flag_data_ready_get(&(pObj->Ctx), &t_da) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  if (p_da && t_da)
  {
    *Status = 1;
  }
  else
  {
    *Status = 0;
  }

  return LPS27HHTW_OK;
}

/**
  * @brief  Enable LPS27HHTW DRDY interrupt mode
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_Enable_DRDY_Interrupt(LPS27HHTW_Object_t *pObj)
{
  lps27hhtw_ctrl_reg3_t ctrl1_reg3;

  /* Enable Interrupt DRDY on INT1 */
  if (lps27hhtw_pin_int_route_get(&(pObj->Ctx), &ctrl1_reg3) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }
  ctrl1_reg3.drdy = 1;
  if (lps27hhtw_pin_int_route_set(&(pObj->Ctx), &ctrl1_reg3) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  return LPS27HHTW_OK;
}

/**
  * @brief  Set the LPS27HHTW power mode
  * @param  pObj the device pObj
  * @param  powerMode mode to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_Set_Power_Mode(LPS27HHTW_Object_t *pObj, uint8_t powerMode)
{
  lps27hhtw_ctrl_reg2_t ctrl_reg2;

  if (lps27hhtw_read_reg(&(pObj->Ctx), LPS27HHTW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  if (powerMode == 1)
  {
    ctrl_reg2.low_noise_en = LPS27HHTW_LOW_NOISE_EN;
  }
  else
  {
    ctrl_reg2.low_noise_en = LPS27HHTW_LOW_NOISE_DIS;
  }

  if (lps27hhtw_write_reg(&(pObj->Ctx), LPS27HHTW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }

  return LPS27HHTW_OK;
}

/**
  * @brief  Set the LPS27HHTW filter mode
  * @param  pObj the device pObj
  * @param  filterMode to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS27HHTW_Set_Filter_Mode(LPS27HHTW_Object_t *pObj, uint8_t filterMode)
{
  if (lps27hhtw_lp_bandwidth_set(&(pObj->Ctx), (lps27hhtw_lpfp_cfg_t)filterMode) != LPS27HHTW_OK)
  {
    return LPS27HHTW_ERROR;
  }
  return LPS27HHTW_OK;
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
  LPS27HHTW_Object_t *pObj = (LPS27HHTW_Object_t *)Handle;

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
  LPS27HHTW_Object_t *pObj = (LPS27HHTW_Object_t *)Handle;

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
