/**
  ******************************************************************************
  * @file    lps33hw.c
  * @author  MEMS Software Solutions Team
  * @brief   LPS33HW driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lps33hw.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup LPS33HW LPS33HW
  * @{
  */

/** @defgroup LPS33HW_Exported_Variables LPS33HW Exported Variables
  * @{
  */

LPS33HW_CommonDrv_t LPS33HW_COMMON_Driver =
{
  LPS33HW_Init,
  LPS33HW_DeInit,
  LPS33HW_ReadID,
  LPS33HW_GetCapabilities,
};

LPS33HW_PRESS_Drv_t LPS33HW_PRESS_Driver =
{
  LPS33HW_PRESS_Enable,
  LPS33HW_PRESS_Disable,
  LPS33HW_PRESS_GetOutputDataRate,
  LPS33HW_PRESS_SetOutputDataRate,
  LPS33HW_PRESS_GetPressure,
};

LPS33HW_TEMP_Drv_t LPS33HW_TEMP_Driver =
{
  LPS33HW_TEMP_Enable,
  LPS33HW_TEMP_Disable,
  LPS33HW_TEMP_GetOutputDataRate,
  LPS33HW_TEMP_SetOutputDataRate,
  LPS33HW_TEMP_GetTemperature,
};

/**
  * @}
  */

/** @defgroup LPS33HW_Private_Function_Prototypes LPS33HW Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t LPS33HW_GetOutputDataRate(LPS33HW_Object_t *pObj, float *Odr);
static int32_t LPS33HW_SetOutputDataRate_When_Enabled(LPS33HW_Object_t *pObj, float Odr);
static int32_t LPS33HW_SetOutputDataRate_When_Disabled(LPS33HW_Object_t *pObj, float Odr);
static int32_t LPS33HW_Initialize(LPS33HW_Object_t *pObj);

/**
  * @}
  */

/** @defgroup LPS33HW_Exported_Functions LPS33HW Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_RegisterBusIO(LPS33HW_Object_t *pObj, LPS33HW_IO_t *pIO)
{
  int32_t ret = LPS33HW_OK;

  if (pObj == NULL)
  {
    ret = LPS33HW_ERROR;
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
    pObj->Ctx.handle   = pObj;

    if (pObj->IO.Init == NULL)
    {
      ret = LPS33HW_ERROR;
    }
    else if (pObj->IO.Init() != LPS33HW_OK)
    {
      ret = LPS33HW_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == LPS33HW_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x01;

          if (LPS33HW_Write_Reg(pObj, LPS33HW_CTRL_REG1, data) != LPS33HW_OK)
          {
            ret = LPS33HW_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  Initialize the LPS33HW sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_Init(LPS33HW_Object_t *pObj)
{
  if (pObj->is_initialized == 0U)
  {
    if (LPS33HW_Initialize(pObj) != LPS33HW_OK)
    {
      return LPS33HW_ERROR;
    }
  }

  pObj->is_initialized = 1U;

  return LPS33HW_OK;
}

/**
  * @brief  Deinitialize the LPS33HW sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_DeInit(LPS33HW_Object_t *pObj)
{
  if (pObj->is_initialized == 1U)
  {
    if (LPS33HW_PRESS_Disable(pObj) != LPS33HW_OK)
    {
      return LPS33HW_ERROR;
    }

    if (LPS33HW_TEMP_Disable(pObj) != LPS33HW_OK)
    {
      return LPS33HW_ERROR;
    }
  }

  pObj->is_initialized = 0;

  return LPS33HW_OK;
}

/**
  * @brief  Get WHO_AM_I value
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_ReadID(LPS33HW_Object_t *pObj, uint8_t *Id)
{
  if (lps33hw_device_id_get(&(pObj->Ctx), Id) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  return LPS33HW_OK;
}

/**
  * @brief  Get LPS33HW sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to LPS33HW sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_GetCapabilities(LPS33HW_Object_t *pObj, LPS33HW_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Humidity    = 0;
  Capabilities->Pressure    = 1;
  Capabilities->Temperature = 1;
  Capabilities->LowPower    = 1;
  Capabilities->HumMaxOdr   = 0.0f;
  Capabilities->TempMaxOdr  = 75.0f;
  Capabilities->PressMaxOdr = 75.0f;
  return LPS33HW_OK;
}

/**
  * @brief  Get the LPS33HW initialization status
  * @param  pObj the device pObj
  * @param  Status 1 if initialized, 0 otherwise
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_Get_Init_Status(LPS33HW_Object_t *pObj, uint8_t *Status)
{
  if (pObj == NULL)
  {
    return LPS33HW_ERROR;
  }

  *Status = pObj->is_initialized;

  return LPS33HW_OK;
}

/**
  * @brief  Enable the LPS33HW pressure sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_PRESS_Enable(LPS33HW_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->press_is_enabled == 1U)
  {
    return LPS33HW_OK;
  }

  /* Output data rate selection. */
  if (lps33hw_data_rate_set(&(pObj->Ctx), pObj->last_odr) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  pObj->press_is_enabled = 1;

  return LPS33HW_OK;
}

/**
  * @brief  Disable the LPS33HW pressure sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_PRESS_Disable(LPS33HW_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->press_is_enabled == 0U)
  {
    return LPS33HW_OK;
  }

  /* Check if the LPS33HW temperature sensor is still enable. */
  /* If yes, skip the disable function, if not call disable function */
  if (pObj->temp_is_enabled == 0U)
  {
    /* Get current output data rate. */
    if (lps33hw_data_rate_get(&(pObj->Ctx), &pObj->last_odr) != LPS33HW_OK)
    {
      return LPS33HW_ERROR;
    }

    /* Output data rate selection - power down. */
    if (lps33hw_data_rate_set(&(pObj->Ctx), LPS33HW_POWER_DOWN) != LPS33HW_OK)
    {
      return LPS33HW_ERROR;
    }
  }

  pObj->press_is_enabled = 0;

  return LPS33HW_OK;
}

/**
  * @brief  Get the LPS33HW pressure sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_PRESS_GetOutputDataRate(LPS33HW_Object_t *pObj, float *Odr)
{
  return LPS33HW_GetOutputDataRate(pObj, Odr);
}

/**
  * @brief  Set the LPS33HW pressure sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_PRESS_SetOutputDataRate(LPS33HW_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->press_is_enabled == 1U)
  {
    return LPS33HW_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return LPS33HW_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the LPS33HW pressure value
  * @param  pObj the device pObj
  * @param  Value pointer where the pressure value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_PRESS_GetPressure(LPS33HW_Object_t *pObj, float *Value)
{
  lps33hw_axis1bit32_t data_raw_pressure;

  (void)memset(data_raw_pressure.u8bit, 0x00, sizeof(int32_t));
  if (lps33hw_pressure_raw_get(&(pObj->Ctx), (uint32_t *)&data_raw_pressure.i32bit) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  *Value = lps33hw_from_lsb_to_hpa(data_raw_pressure.i32bit);

  return LPS33HW_OK;
}

/**
  * @brief  Get the LPS33HW pressure data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_PRESS_Get_DRDY_Status(LPS33HW_Object_t *pObj, uint8_t *Status)
{
  if (lps33hw_press_data_ready_get(&(pObj->Ctx), Status) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  return LPS33HW_OK;
}

/**
  * @brief  Enable the LPS33HW temperature sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_TEMP_Enable(LPS33HW_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->temp_is_enabled == 1U)
  {
    return LPS33HW_OK;
  }

  /* Output data rate selection. */
  if (lps33hw_data_rate_set(&(pObj->Ctx), pObj->last_odr) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  pObj->temp_is_enabled = 1;

  return LPS33HW_OK;
}

/**
  * @brief  Disable the LPS33HW temperature sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_TEMP_Disable(LPS33HW_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->temp_is_enabled == 0U)
  {
    return LPS33HW_OK;
  }

  /* Check if the LPS33HW pressure sensor is still enable. */
  /* If yes, skip the disable function, if not call disable function */
  if (pObj->press_is_enabled == 0U)
  {
    /* Get current output data rate. */
    if (lps33hw_data_rate_get(&(pObj->Ctx), &pObj->last_odr) != LPS33HW_OK)
    {
      return LPS33HW_ERROR;
    }

    /* Output data rate selection - power down. */
    if (lps33hw_data_rate_set(&(pObj->Ctx), LPS33HW_POWER_DOWN) != LPS33HW_OK)
    {
      return LPS33HW_ERROR;
    }
  }

  pObj->temp_is_enabled = 0;

  return LPS33HW_OK;
}

/**
  * @brief  Get the LPS33HW temperature sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_TEMP_GetOutputDataRate(LPS33HW_Object_t *pObj, float *Odr)
{
  return LPS33HW_GetOutputDataRate(pObj, Odr);
}

/**
  * @brief  Set the LPS33HW temperature sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_TEMP_SetOutputDataRate(LPS33HW_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->temp_is_enabled == 1U)
  {
    return LPS33HW_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return LPS33HW_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the LPS33HW temperature value
  * @param  pObj the device pObj
  * @param  Value pointer where the temperature value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_TEMP_GetTemperature(LPS33HW_Object_t *pObj, float *Value)
{
  lps33hw_axis1bit16_t data_raw_temperature;

  (void)memset(data_raw_temperature.u8bit, 0x00, sizeof(int16_t));
  if (lps33hw_temperature_raw_get(&(pObj->Ctx), &data_raw_temperature.i16bit) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  *Value = lps33hw_from_lsb_to_degc(data_raw_temperature.i16bit);

  return LPS33HW_OK;
}

/**
  * @brief  Get the LPS33HW temperature data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_TEMP_Get_DRDY_Status(LPS33HW_Object_t *pObj, uint8_t *Status)
{
  if (lps33hw_temp_data_ready_get(&(pObj->Ctx), Status) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  return LPS33HW_OK;
}

/**
  * @brief  Get the LPS33HW FIFO data level
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_FIFO_Get_Data(LPS33HW_Object_t *pObj, float *Press, float *Temp)
{
  if (LPS33HW_PRESS_GetPressure(pObj, Press) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  if (LPS33HW_TEMP_GetTemperature(pObj, Temp) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  return LPS33HW_OK;
}

/**
  * @brief  Get the LPS33HW FIFO threshold
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_FIFO_Get_FTh_Status(LPS33HW_Object_t *pObj, uint8_t *Status)
{
  if (lps33hw_fifo_fth_flag_get(&(pObj->Ctx), Status) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  return LPS33HW_OK;
}

/**
  * @brief  Get the LPS33HW FIFO full status
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_FIFO_Get_Full_Status(LPS33HW_Object_t *pObj, uint8_t *Status)
{
  if (lps33hw_fifo_data_level_get(&(pObj->Ctx), Status) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  if (*Status == LPS33HW_FIFO_FULL)
  {
    *Status = (uint8_t) 1;
  }

  return LPS33HW_OK;
}

/**
  * @brief  Get the LPS33HW FIFO data level
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_FIFO_Get_Level(LPS33HW_Object_t *pObj, uint8_t *Status)
{
  if (lps33hw_fifo_data_level_get(&(pObj->Ctx), Status) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  return LPS33HW_OK;
}

/**
  * @brief  Get the LPS33HW FIFO OVR status
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_FIFO_Get_Ovr_Status(LPS33HW_Object_t *pObj, uint8_t *Status)
{
  if (lps33hw_fifo_ovr_flag_get(&(pObj->Ctx), Status) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  return LPS33HW_OK;
}

/**
  * @brief  Reset the FIFO interrupt
  * @param  pObj the device pObj
  * @param  interrupt The FIFO interrupt to be reset; values: 0 = FTH; 1 = FULL; 2 = OVR
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_FIFO_Reset_Interrupt(LPS33HW_Object_t *pObj, uint8_t interrupt)
{
  switch (interrupt)
  {
    case 0:
      if (lps33hw_fifo_threshold_on_int_set(&(pObj->Ctx), PROPERTY_DISABLE) != LPS33HW_OK)
      {
        return LPS33HW_ERROR;
      }
      break;
    case 1:
      if (lps33hw_fifo_full_on_int_set(&(pObj->Ctx), PROPERTY_DISABLE) != LPS33HW_OK)
      {
        return LPS33HW_ERROR;
      }
      break;
    case 2:
      if (lps33hw_fifo_ovr_on_int_set(&(pObj->Ctx), PROPERTY_DISABLE) != LPS33HW_OK)
      {
        return LPS33HW_ERROR;
      }
      break;
    default:
      return LPS33HW_ERROR;
  }

  return LPS33HW_OK;
}

/**
  * @brief  Set the FIFO interrupt
  * @param  pObj the device pObj
  * @param  interrupt The FIFO interrupt to be reset; values: 0 = FTH; 1 = FULL; 2 = OVR
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_FIFO_Set_Interrupt(LPS33HW_Object_t *pObj, uint8_t interrupt)
{
  switch (interrupt)
  {
    case 0:
      if (lps33hw_fifo_threshold_on_int_set(&(pObj->Ctx), PROPERTY_ENABLE) != LPS33HW_OK)
      {
        return LPS33HW_ERROR;
      }
      break;
    case 1:
      if (lps33hw_fifo_full_on_int_set(&(pObj->Ctx), PROPERTY_ENABLE) != LPS33HW_OK)
      {
        return LPS33HW_ERROR;
      }
      break;
    case 2:
      if (lps33hw_fifo_ovr_on_int_set(&(pObj->Ctx), PROPERTY_ENABLE) != LPS33HW_OK)
      {
        return LPS33HW_ERROR;
      }
      break;
    default:
      return LPS33HW_ERROR;
  }

  return LPS33HW_OK;
}

/**
  * @brief  Set the FIFO mode
  * @param  pObj the device pObj
  * @param  Mode the FIFO mode to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_FIFO_Set_Mode(LPS33HW_Object_t *pObj, uint8_t Mode)
{
  /* Verify that the passed parameter contains one of the valid values */
  switch ((lps33hw_f_mode_t)Mode)
  {
    case LPS33HW_BYPASS_MODE:
    case LPS33HW_FIFO_MODE:
    case LPS33HW_STREAM_MODE:
    case LPS33HW_STREAM_TO_FIFO_MODE:
    case LPS33HW_BYPASS_TO_STREAM_MODE:
    case LPS33HW_DYNAMIC_STREAM_MODE:
    case LPS33HW_BYPASS_TO_FIFO_MODE:
      break;
    default:
      return LPS33HW_ERROR;
  }

  if (lps33hw_fifo_mode_set(&(pObj->Ctx), (lps33hw_f_mode_t)Mode) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  return LPS33HW_OK;
}

/**
  * @brief  Set the LPS33HW FIFO data level
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_FIFO_Set_Watermark_Level(LPS33HW_Object_t *pObj, uint8_t Watermark)
{
  if (lps33hw_fifo_watermark_set(&(pObj->Ctx), Watermark) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  return LPS33HW_OK;
}

/**
  * @brief  Set the LPS33HW stop on watermark function
  * @param  pObj the device pObj
  * @param  Stop the state of stop on watermark function
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_FIFO_Stop_On_Watermark(LPS33HW_Object_t *pObj, uint8_t Stop)
{
  if (lps33hw_stop_on_fifo_threshold_set(&(pObj->Ctx), Stop) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  return LPS33HW_OK;
}

/**
  * @brief  Enable the FIFO usage
  * @param  pObj the device pObj
  * @param  Status the FIFO disabled/enabled state to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_FIFO_Usage(LPS33HW_Object_t *pObj, uint8_t Status)
{
  if (lps33hw_fifo_set(&(pObj->Ctx), Status) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  return LPS33HW_OK;
}

/**
  * @brief  Get the LPS33HW register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_Read_Reg(LPS33HW_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (lps33hw_read_reg(&(pObj->Ctx), Reg, Data, 1) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  return LPS33HW_OK;
}

/**
  * @brief  Set the LPS33HW register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_Write_Reg(LPS33HW_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (lps33hw_write_reg(&(pObj->Ctx), Reg, &Data, 1) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  return LPS33HW_OK;
}

/**
  * @}
  */

/** @defgroup LPS33HW_Private_Functions LPS33HW Private Functions
  * @{
  */

/**
  * @brief  Get output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LPS33HW_GetOutputDataRate(LPS33HW_Object_t *pObj, float *Odr)
{
  int32_t ret = LPS33HW_OK;
  lps33hw_odr_t odr_low_level;

  if (lps33hw_data_rate_get(&(pObj->Ctx), &odr_low_level) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  switch (odr_low_level)
  {
    case LPS33HW_POWER_DOWN:
      *Odr = 0.0f;
      break;

    case LPS33HW_ODR_1_Hz:
      *Odr = 1.0f;
      break;

    case LPS33HW_ODR_10_Hz:
      *Odr = 10.0f;
      break;

    case LPS33HW_ODR_25_Hz:
      *Odr = 25.0f;
      break;

    case LPS33HW_ODR_50_Hz:
      *Odr = 50.0f;
      break;

    case LPS33HW_ODR_75_Hz:
      *Odr = 75.0f;
      break;

    default:
      ret = LPS33HW_ERROR;
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
static int32_t LPS33HW_SetOutputDataRate_When_Enabled(LPS33HW_Object_t *pObj, float Odr)
{
  lps33hw_odr_t new_odr;

  new_odr = (Odr <=  1.0f) ? LPS33HW_ODR_1_Hz
            : (Odr <= 10.0f) ? LPS33HW_ODR_10_Hz
            : (Odr <= 25.0f) ? LPS33HW_ODR_25_Hz
            : (Odr <= 50.0f) ? LPS33HW_ODR_50_Hz
            :                  LPS33HW_ODR_75_Hz;

  if (lps33hw_data_rate_set(&(pObj->Ctx), new_odr) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  if (lps33hw_data_rate_get(&(pObj->Ctx), &pObj->last_odr) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  return LPS33HW_OK;
}

/**
  * @brief  Set output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LPS33HW_SetOutputDataRate_When_Disabled(LPS33HW_Object_t *pObj, float Odr)
{
  pObj->last_odr = (Odr <=  1.0f) ? LPS33HW_ODR_1_Hz
                   : (Odr <= 10.0f) ? LPS33HW_ODR_10_Hz
                   : (Odr <= 25.0f) ? LPS33HW_ODR_25_Hz
                   : (Odr <= 50.0f) ? LPS33HW_ODR_50_Hz
                   :                  LPS33HW_ODR_75_Hz;

  return LPS33HW_OK;
}

/**
  * @brief  Initialize the LPS33HW sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LPS33HW_Initialize(LPS33HW_Object_t *pObj)
{
  /* Power down the device */
  if (lps33hw_data_rate_set(&(pObj->Ctx), LPS33HW_POWER_DOWN) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  /* Set Power mode */
  if (lps33hw_low_power_set(&(pObj->Ctx), PROPERTY_ENABLE) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  /* Disable low-pass filter on LPS33HW pressure data */
  if (lps33hw_low_pass_filter_mode_set(&(pObj->Ctx), LPS33HW_LPF_ODR_DIV_2) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  /* Set block data update mode */
  if (lps33hw_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  /* Set autoincrement for multi-byte read/write */
  if (lps33hw_auto_add_inc_set(&(pObj->Ctx), PROPERTY_ENABLE) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  pObj->last_odr = LPS33HW_ODR_25_Hz;

  return LPS33HW_OK;
}

/**
  * @brief  Set the LPS33HW One Shot Mode
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_Set_One_Shot(LPS33HW_Object_t *pObj)
{
  /* Set ODR */
  if (lps33hw_data_rate_set(&(pObj->Ctx), LPS33HW_POWER_DOWN) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  /* Start One Shot Measurement */
  if (lps33hw_one_shoot_trigger_set(&(pObj->Ctx), PROPERTY_ENABLE) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  return LPS33HW_OK;
}

/**
  * @brief  Get the LPS33HW One Shot Status
  * @param  pObj the device pObj
  * @param  Status pointer to the one shot status (1 means measurements available, 0 means measurements not available yet)
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS33HW_Get_One_Shot_Status(LPS33HW_Object_t *pObj, uint8_t *Status)
{
  uint8_t p_da;
  uint8_t t_da;

  /* Get DataReady for pressure */
  if (lps33hw_press_data_ready_get(&(pObj->Ctx), &p_da) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  /* Get DataReady for temperature */
  if (lps33hw_temp_data_ready_get(&(pObj->Ctx), &t_da) != LPS33HW_OK)
  {
    return LPS33HW_ERROR;
  }

  if (p_da && t_da)
  {
    *Status = 1;
  }
  else
  {
    *Status = 0;
  }

  return LPS33HW_OK;
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
  LPS33HW_Object_t *pObj = (LPS33HW_Object_t *)Handle;

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
  LPS33HW_Object_t *pObj = (LPS33HW_Object_t *)Handle;

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
