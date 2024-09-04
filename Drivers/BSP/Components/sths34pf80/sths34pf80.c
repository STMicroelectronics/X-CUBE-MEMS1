/**
  ******************************************************************************
  * @file    sths34pf80.c
  * @author  MEMS Software Solutions Team
  * @brief   STHS34PF80 driver file
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
#include "sths34pf80.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup STHS34PF80 STHS34PF80
  * @{
  */

/** @defgroup STHS34PF80_Exported_Variables STHS34PF80 Exported Variables
  * @{
  */

STHS34PF80_CommonDrv_t STHS34PF80_COMMON_Driver =
{
  STHS34PF80_Init,
  STHS34PF80_DeInit,
  STHS34PF80_ReadID,
  STHS34PF80_GetCapabilities,
};

STHS34PF80_TEMP_Drv_t STHS34PF80_TEMP_Driver =
{
  STHS34PF80_TEMP_Enable,
  STHS34PF80_TEMP_Disable,
  STHS34PF80_TEMP_GetOutputDataRate,
  STHS34PF80_TEMP_SetOutputDataRate,
  STHS34PF80_TEMP_GetTemperature,
};

/**
  * @}
  */

/** @defgroup STHS34PF80_Private_Function_Prototypes STHS34PF80 Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);

/**
  * @}
  */

/** @defgroup STHS34PF80_Exported_Functions STHS34PF80 Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_RegisterBusIO(STHS34PF80_Object_t *pObj, STHS34PF80_IO_t *pIO)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = STHS34PF80_ERROR;
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

    if (pObj->IO.Init != NULL)
    {
      ret = pObj->IO.Init();
    }
    else
    {
      ret = STHS34PF80_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Initialize the STHS34PF80 sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_Init(STHS34PF80_Object_t *pObj)
{
  /* Set main memory bank */
  if (STHS34PF80_Set_Mem_Bank(pObj, (uint8_t)STHS34PF80_MAIN_MEM_BANK) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  if (pObj->is_initialized == 0U)
  {
    /* Set default ODR */
    pObj->temp_odr = 1.0f;

    /* Enable BDU */
    if (sths34pf80_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != STHS34PF80_OK)
    {
      return STHS34PF80_ERROR;
    }

    /* Put the component in standby mode. */
    if (sths34pf80_tmos_odr_set(&(pObj->Ctx), STHS34PF80_TMOS_ODR_OFF) != STHS34PF80_OK)
    {
      return STHS34PF80_ERROR;
    }
  }

  pObj->is_initialized = 1;

  return STHS34PF80_OK;
}

/**
  * @brief  Deinitialize the STHS34PF80 sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_DeInit(STHS34PF80_Object_t *pObj)
{
  if (pObj->is_initialized == 1U)
  {
    /* Put the component in standby mode */
    if (STHS34PF80_TEMP_Disable(pObj) != STHS34PF80_OK)
    {
      return STHS34PF80_ERROR;
    }
  }

  pObj->is_initialized = 0;

  return STHS34PF80_OK;
}

/**
  * @brief  Get WHO_AM_I value
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_ReadID(STHS34PF80_Object_t *pObj, uint8_t *Id)
{
  uint8_t buf;

  if (sths34pf80_device_id_get(&(pObj->Ctx), &buf) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  *Id = buf;

  return STHS34PF80_OK;
}

/**
  * @brief  Get STHS34PF80 sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to STHS34PF80 sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_GetCapabilities(STHS34PF80_Object_t *pObj, STHS34PF80_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Humidity    = 0;
  Capabilities->Pressure    = 0;
  Capabilities->Temperature = 1;
  Capabilities->Gas         = 0;
  Capabilities->LowPower    = 0;
  Capabilities->HumMaxOdr   = 0.0f;
  Capabilities->TempMaxOdr  = 30.0f;
  Capabilities->PressMaxOdr = 0.0f;
  Capabilities->GasMaxOdr   = 0.0f;
  return STHS34PF80_OK;
}

/**
  * @brief  Get the STHS34PF80 initialization status
  * @param  pObj the device pObj
  * @param  Status 1 if initialized, 0 otherwise
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_Get_Init_Status(STHS34PF80_Object_t *pObj, uint8_t *Status)
{
  if (pObj == NULL)
  {
    return STHS34PF80_ERROR;
  }

  *Status = pObj->is_initialized;

  return STHS34PF80_OK;
}

/**
  * @brief  Enable the STHS34PF80 temperature sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_TEMP_Enable(STHS34PF80_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->temp_is_enabled == 1U)
  {
    return STHS34PF80_OK;
  }

  /* Power on the component and set the odr. */
  if (STHS34PF80_TEMP_SetOutputDataRate(pObj, pObj->temp_odr) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  pObj->temp_is_enabled = 1;

  return STHS34PF80_OK;
}

/**
  * @brief  Disable the STHS34PF80 temperature sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_TEMP_Disable(STHS34PF80_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->temp_is_enabled == 0U)
  {
    return STHS34PF80_OK;
  }

  /* Save the current odr. */
  if (STHS34PF80_TEMP_GetOutputDataRate(pObj, &pObj->temp_odr) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  /* Put the component in standby mode. */
  if (sths34pf80_tmos_odr_set(&(pObj->Ctx), STHS34PF80_TMOS_ODR_OFF) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  pObj->temp_is_enabled = 0;

  return STHS34PF80_OK;
}

/**
  * @brief  Get the STHS34PF80 temperature sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_TEMP_GetOutputDataRate(STHS34PF80_Object_t *pObj, float_t *Odr)
{
  int32_t ret = STHS34PF80_OK;
  sths34pf80_tmos_odr_t odr_low_level;

  if (sths34pf80_tmos_odr_get(&(pObj->Ctx), &odr_low_level) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  switch (odr_low_level)
  {
    case STHS34PF80_TMOS_ODR_OFF:
      *Odr = 0.0f;
      break;

    case STHS34PF80_TMOS_ODR_AT_0Hz25:
      *Odr = 0.25f;
      break;

    case STHS34PF80_TMOS_ODR_AT_0Hz50:
      *Odr = 0.5f;
      break;

    case STHS34PF80_TMOS_ODR_AT_1Hz:
      *Odr = 1.0f;
      break;

    case STHS34PF80_TMOS_ODR_AT_2Hz:
      *Odr = 2.0f;
      break;

    case STHS34PF80_TMOS_ODR_AT_4Hz:
      *Odr = 4.0f;
      break;

    case STHS34PF80_TMOS_ODR_AT_8Hz:
      *Odr = 8.0f;
      break;

    case STHS34PF80_TMOS_ODR_AT_15Hz:
      *Odr = 15.0f;
      break;

    case STHS34PF80_TMOS_ODR_AT_30Hz:
      *Odr = 30.0f;
      break;

    default:
      ret = STHS34PF80_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the STHS34PF80 temperature sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_TEMP_SetOutputDataRate(STHS34PF80_Object_t *pObj, float_t Odr)
{
  sths34pf80_tmos_odr_t new_odr;

  new_odr = (Odr <= 0.25f) ? STHS34PF80_TMOS_ODR_AT_0Hz25
            : (Odr <= 0.5f) ? STHS34PF80_TMOS_ODR_AT_0Hz50
            : (Odr <= 1.0f) ? STHS34PF80_TMOS_ODR_AT_1Hz
            : (Odr <= 2.0f) ? STHS34PF80_TMOS_ODR_AT_2Hz
            : (Odr <= 4.0f) ? STHS34PF80_TMOS_ODR_AT_4Hz
            : (Odr <= 8.0f) ? STHS34PF80_TMOS_ODR_AT_8Hz
            : (Odr <= 15.0f) ? STHS34PF80_TMOS_ODR_AT_15Hz
            :                  STHS34PF80_TMOS_ODR_AT_30Hz;

  if (sths34pf80_tmos_odr_set(&(pObj->Ctx), new_odr) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  return STHS34PF80_OK;
}

/**
  * @brief  Get the STHS34PF80 temperature value
  * @param  pObj the device pObj
  * @param  Value pointer where the temperature value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_TEMP_GetTemperature(STHS34PF80_Object_t *pObj, float_t *Value)
{
  /* Get the ambient temperature */
  if (STHS34PF80_GetAmbientTemperature(pObj, Value) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  return STHS34PF80_OK;
}

/**
  * @brief  Get the STHS34PF80 temperature data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_TEMP_Get_DRDY_Status(STHS34PF80_Object_t *pObj, uint8_t *Status)
{
  sths34pf80_tmos_drdy_status_t val;

  if (sths34pf80_tmos_drdy_status_get(&(pObj->Ctx), &val) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  if (val.drdy != 0U)
  {
    *Status = 1;
  }
  else
  {
    *Status = 0;
  }

  return STHS34PF80_OK;
}

/**
  * @brief  Get the STHS34PF80 register value
  * @param  pObj the device pObj
  * @param  Reg address to be read
  * @param  Data pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_Read_Reg(STHS34PF80_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (sths34pf80_read_reg(&(pObj->Ctx), Reg, Data, 1) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  return STHS34PF80_OK;
}

/**
  * @brief  Set the STHS34PF80 register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_Write_Reg(STHS34PF80_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (sths34pf80_write_reg(&(pObj->Ctx), Reg, &Data, 1) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  return STHS34PF80_OK;
}

/**
  * @brief  Get the object raw temperature value in [LSB]
  * @param  pObj the device pObj
  * @param  Value pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_GetObjectTemperatureRaw(STHS34PF80_Object_t *pObj, int16_t *Value)
{
  /* Get the object temperature */
  if (sths34pf80_tobject_raw_get(&(pObj->Ctx), Value) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  return STHS34PF80_OK;
}

/**
  * @brief  Get the ambient raw temperature value in [LSB]
  * @param  pObj the device pObj
  * @param  Value pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_GetAmbientTemperatureRaw(STHS34PF80_Object_t *pObj, int16_t *Value)
{
  /* Get the ambient temperature */
  if (sths34pf80_tambient_raw_get(&(pObj->Ctx), Value) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  return STHS34PF80_OK;
}

/**
  * @brief  Get the object temperature value
  * @param  pObj the device pObj
  * @param  Value pointer where the temperature value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_GetObjectTemperature(STHS34PF80_Object_t *pObj, float_t *Value)
{
  int16_t raw_value;
  uint16_t sensitivity;
  int16_t int16_value;

  /* Get the object temperature */
  if (sths34pf80_tobject_raw_get(&(pObj->Ctx), &raw_value) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  /* Get the sensitivity */
  if (sths34pf80_tmos_sensitivity_get(&(pObj->Ctx), &sensitivity) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  if (sensitivity != 0U)
  {
    int16_value = raw_value / (int16_t)sensitivity;
    *Value = (float_t)int16_value;
  }
  else
  {
    return STHS34PF80_ERROR;
  }

  return STHS34PF80_OK;
}

/**
  * @brief  Get the ambient temperature value
  * @param  pObj the device pObj
  * @param  Value pointer where the temperature value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_GetAmbientTemperature(STHS34PF80_Object_t *pObj, float_t *Value)
{
  int16_t raw_value;
  int16_t int16_value;

  /* Get the ambient temperature */
  if (sths34pf80_tambient_raw_get(&(pObj->Ctx), &raw_value) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  int16_value = raw_value / 100;
  *Value = (float_t)int16_value;

  return STHS34PF80_OK;
}

/**
  * @brief  Get the presence value
  * @param  pObj the device pObj
  * @param  Value pointer where the presence value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_GetPresenceData(STHS34PF80_Object_t *pObj, int16_t *Value)
{
  /* Get the presence value */
  if (sths34pf80_tpresence_raw_get(&(pObj->Ctx), Value) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  return STHS34PF80_OK;
}

/**
  * @brief  Get the presence flag
  * @param  pObj the device pObj
  * @param  Value pointer where the presence flag is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_GetPresenceFlag(STHS34PF80_Object_t *pObj, uint8_t *Value)
{
  sths34pf80_tmos_func_status_t status;

  /* Get the presence flag */
  if (sths34pf80_tmos_func_status_get(&(pObj->Ctx), &status) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  *Value = status.pres_flag;

  return STHS34PF80_OK;
}

/**
  * @brief  Get the motion value
  * @param  pObj the device pObj
  * @param  Value pointer where the motion value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_GetMotionData(STHS34PF80_Object_t *pObj, int16_t *Value)
{
  /* Get the motion value */
  if (sths34pf80_tmotion_raw_get(&(pObj->Ctx), Value) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  return STHS34PF80_OK;
}

/**
  * @brief  Get the motion flag
  * @param  pObj the device pObj
  * @param  Value pointer where the motion flag is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_GetMotionFlag(STHS34PF80_Object_t *pObj, uint8_t *Value)
{
  sths34pf80_tmos_func_status_t status;

  /* Get the motion flag */
  if (sths34pf80_tmos_func_status_get(&(pObj->Ctx), &status) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  *Value = status.mot_flag;

  return STHS34PF80_OK;
}

/**
  * @brief  Get the AvgTmos value
  * @param  pObj the device pObj
  * @param  Value pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_GetAvgTmos(STHS34PF80_Object_t *pObj, uint16_t *Value)
{
  int32_t ret = STHS34PF80_OK;
  sths34pf80_avg_tobject_num_t raw_value;

  /* Get the raw value */
  if (sths34pf80_avg_tobject_num_get(&(pObj->Ctx), &raw_value) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  switch (raw_value)
  {
    case STHS34PF80_AVG_TMOS_2:
      *Value = 2;
      break;

    case STHS34PF80_AVG_TMOS_8:
      *Value = 8;
      break;

    case STHS34PF80_AVG_TMOS_32:
      *Value = 32;
      break;

    case STHS34PF80_AVG_TMOS_128:
      *Value = 128;
      break;

    case STHS34PF80_AVG_TMOS_256:
      *Value = 256;
      break;

    case STHS34PF80_AVG_TMOS_512:
      *Value = 512;
      break;

    case STHS34PF80_AVG_TMOS_1024:
      *Value = 1024;
      break;

    case STHS34PF80_AVG_TMOS_2048:
      *Value = 2048;
      break;

    default:
      ret = STHS34PF80_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the AvgTmos value
  * @param  pObj the device pObj
  * @param  Value the value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_SetAvgTmos(STHS34PF80_Object_t *pObj, uint16_t Value)
{
  int32_t ret = STHS34PF80_OK;
  sths34pf80_avg_tobject_num_t raw_value;

  raw_value = (Value <= 2U) ? STHS34PF80_AVG_TMOS_2
            : (Value <= 8U) ? STHS34PF80_AVG_TMOS_8
            : (Value <= 32U) ? STHS34PF80_AVG_TMOS_32
            : (Value <= 128U) ? STHS34PF80_AVG_TMOS_128
            : (Value <= 256U) ? STHS34PF80_AVG_TMOS_256
            : (Value <= 512U) ? STHS34PF80_AVG_TMOS_512
            : (Value <= 1024U) ? STHS34PF80_AVG_TMOS_1024
            :                    STHS34PF80_AVG_TMOS_2048;

  if (sths34pf80_avg_tobject_num_set(&(pObj->Ctx), raw_value) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  return ret;
}

/**
  * @brief  Get the AvgT value
  * @param  pObj the device pObj
  * @param  Value pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_GetAvgT(STHS34PF80_Object_t *pObj, uint8_t *Value)
{
  int32_t ret = STHS34PF80_OK;
  sths34pf80_avg_tambient_num_t raw_value;

  /* Get the raw value */
  if (sths34pf80_avg_tambient_num_get(&(pObj->Ctx), &raw_value) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  switch (raw_value)
  {
    case STHS34PF80_AVG_T_8:
      *Value = 8;
      break;

    case STHS34PF80_AVG_T_4:
      *Value = 4;
      break;

    case STHS34PF80_AVG_T_2:
      *Value = 2;
      break;

    case STHS34PF80_AVG_T_1:
      *Value = 1;
      break;

    default:
      ret = STHS34PF80_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the GainFactor value
  * @param  pObj the device pObj
  * @param  Value pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_GetGainFactor(STHS34PF80_Object_t *pObj, uint8_t *Value)
{
  int32_t ret = STHS34PF80_OK;
  sths34pf80_gain_mode_t raw_value;

  /* Get the raw value */
  if (sths34pf80_gain_mode_get(&(pObj->Ctx), &raw_value) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  switch (raw_value)
  {
    case STHS34PF80_GAIN_WIDE_MODE:
      *Value = 8;
      break;

    case STHS34PF80_GAIN_DEFAULT_MODE:
      *Value = 1;
      break;

    default:
      ret = STHS34PF80_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the SensData value
  * @param  pObj the device pObj
  * @param  Value pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_GetSensData(STHS34PF80_Object_t *pObj, uint8_t *Value)
{
  /* Get the raw value */
  if (sths34pf80_read_reg(&(pObj->Ctx), STHS34PF80_SENS_DATA, Value, 1) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  return STHS34PF80_OK;
}

/**
  * @brief  Get the sensitivity
  * @param  pObj the device pObj
  * @param  Value pointer where the sensitivity is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_GetSensitivity(STHS34PF80_Object_t *pObj, uint16_t *Value)
{
  if (sths34pf80_tmos_sensitivity_get(&(pObj->Ctx), Value) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }

  return STHS34PF80_OK;
}


/**
  * @}
  */

/**
  * @brief  Get the Ambient temperature shock
  * @param  pObj the device pObj
  * @param  Value pointer where the Ambient temperature shock is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_GetAmbTempShock(STHS34PF80_Object_t *pObj, int16_t *Value)
{
  if (sths34pf80_tamb_shock_raw_get(&(pObj->Ctx), Value) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }
  
  return STHS34PF80_OK;
}


/**
  * @brief  Get the object temperature compensated
  * @param  pObj the device pObj
  * @param  Value pointer where the object temperature compensated is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_GetObjectTempCompensated(STHS34PF80_Object_t *pObj, int16_t *Value)
{
  if (sths34pf80_tobj_comp_raw_get(&(pObj->Ctx), Value) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }
  
  return STHS34PF80_OK;
}


/**
  * @brief  Get the Presence temperature 
  * @param  pObj the device pObj
  * @param  Value pointer where the Presence temperature is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_GetPresenceTemperature(STHS34PF80_Object_t *pObj, int16_t *Value)
{
  if (sths34pf80_tpresence_raw_get(&(pObj->Ctx), Value) != STHS34PF80_OK)
  {
    return STHS34PF80_ERROR;
  }
  
  return STHS34PF80_OK;
}

/**
  * @brief  Set memory bank
  * @param  pObj the device pObj
  * @param  Val the value of memory bank in reg FUNC_CFG_ACCESS
  *         0 - STHS34PF80_MAIN_MEM_BANK, 1 - STHS34PF80_EMBED_FUNC_MEM_BANK
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STHS34PF80_Set_Mem_Bank(STHS34PF80_Object_t *pObj, uint8_t Val)
{
  int32_t ret = STHS34PF80_OK;
  sths34pf80_mem_bank_t reg;

  reg = (Val == 1U) ? STHS34PF80_EMBED_FUNC_MEM_BANK
        :               STHS34PF80_MAIN_MEM_BANK;

  if (sths34pf80_mem_bank_set(&(pObj->Ctx), reg) != STHS34PF80_OK)
  {
    ret = STHS34PF80_ERROR;
  }

  return ret;
}

/**
  * @}
  */

/** @defgroup STHS34PF80_Private_Functions STHS34PF80 Private Functions
  * @{
  */

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
  STHS34PF80_Object_t *pObj = (STHS34PF80_Object_t *)Handle;

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
  STHS34PF80_Object_t *pObj = (STHS34PF80_Object_t *)Handle;

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
