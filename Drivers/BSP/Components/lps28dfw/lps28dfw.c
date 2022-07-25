/**
  ******************************************************************************
  * @file    lps28dfw.c
  * @author  MEMS Software Solutions Team
  * @brief   LPS28DFW driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lps28dfw.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup LPS28DFW LPS28DFW
  * @{
  */

/** @defgroup LPS28DFW_Exported_Variables LPS28DFW Exported Variables
  * @{
  */

LPS28DFW_CommonDrv_t LPS28DFW_COMMON_Driver =
{
  LPS28DFW_Init,
  LPS28DFW_DeInit,
  LPS28DFW_ReadID,
  LPS28DFW_GetCapabilities,
};

LPS28DFW_PRESS_Drv_t LPS28DFW_PRESS_Driver =
{
  LPS28DFW_PRESS_Enable,
  LPS28DFW_PRESS_Disable,
  LPS28DFW_PRESS_GetOutputDataRate,
  LPS28DFW_PRESS_SetOutputDataRate,
  LPS28DFW_PRESS_GetPressure,
};

LPS28DFW_TEMP_Drv_t LPS28DFW_TEMP_Driver =
{
  LPS28DFW_TEMP_Enable,
  LPS28DFW_TEMP_Disable,
  LPS28DFW_TEMP_GetOutputDataRate,
  LPS28DFW_TEMP_SetOutputDataRate,
  LPS28DFW_TEMP_GetTemperature,
};

/**
  * @}
  */

/** @defgroup LPS28DFW_Private_Function_Prototypes LPS28DFW Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t LPS28DFW_GetOutputDataRate(LPS28DFW_Object_t *pObj, float *Odr);
static int32_t LPS28DFW_SetOutputDataRate_When_Enabled(LPS28DFW_Object_t *pObj, float Odr);
static int32_t LPS28DFW_SetOutputDataRate_When_Disabled(LPS28DFW_Object_t *pObj, float Odr);
static int32_t LPS28DFW_Initialize(LPS28DFW_Object_t *pObj);

/**
  * @}
  */

/** @defgroup LPS28DFW_Exported_Functions LPS28DFW Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_RegisterBusIO(LPS28DFW_Object_t *pObj, LPS28DFW_IO_t *pIO)
{
  int32_t ret = LPS28DFW_OK;

  if (pObj == NULL)
  {
    ret = LPS28DFW_ERROR;
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
    pObj->Ctx.handle    = pObj;

    if (pObj->IO.Init == NULL)
    {
      ret = LPS28DFW_ERROR;
    }

    if (pObj->IO.Init() != LPS28DFW_OK)
    {
      ret = LPS28DFW_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Initialize the LPS28DFW sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_Init(LPS28DFW_Object_t *pObj)
{
  if (pObj->is_initialized == 0U)
  {
    if (LPS28DFW_Initialize(pObj) != LPS28DFW_OK)
    {
      return LPS28DFW_ERROR;
    }
  }

  pObj->is_initialized = 1U;

  return LPS28DFW_OK;
}

/**
  * @brief  Deinitialize the LPS28DFW sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_DeInit(LPS28DFW_Object_t *pObj)
{
  if (pObj->is_initialized == 1U)
  {
    if (LPS28DFW_PRESS_Disable(pObj) != LPS28DFW_OK)
    {
      return LPS28DFW_ERROR;
    }

    if (LPS28DFW_TEMP_Disable(pObj) != LPS28DFW_OK)
    {
      return LPS28DFW_ERROR;
    }
  }

  pObj->is_initialized = 0;

  return LPS28DFW_OK;
}

/**
  * @brief  Get WHO_AM_I value
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_ReadID(LPS28DFW_Object_t *pObj, uint8_t *Id)
{
  lps28dfw_id_t val;
  if (lps28dfw_id_get(&(pObj->Ctx), &val) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  *Id = val.whoami;

  return LPS28DFW_OK;
}

/**
  * @brief  Get LPS28DFW sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to LPS28DFW sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_GetCapabilities(LPS28DFW_Object_t *pObj, LPS28DFW_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Humidity    = 0;
  Capabilities->Pressure    = 1;
  Capabilities->Temperature = 1;
  Capabilities->LowPower    = 0;
  Capabilities->HumMaxOdr   = 0.0f;
  Capabilities->TempMaxOdr  = 200.0f;
  Capabilities->PressMaxOdr = 200.0f;
  return LPS28DFW_OK;
}

/**
  * @brief  Get the LPS28DFW initialization status
  * @param  pObj the device pObj
  * @param  Status 1 if initialized, 0 otherwise
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_Get_Init_Status(LPS28DFW_Object_t *pObj, uint8_t *Status)
{
  if (pObj == NULL)
  {
    return LPS28DFW_ERROR;
  }

  *Status = pObj->is_initialized;

  return LPS28DFW_OK;
}

/**
  * @brief  Enable the LPS28DFW pressure sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_PRESS_Enable(LPS28DFW_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->press_is_enabled == 1U)
  {
    return LPS28DFW_OK;
  }

  /* Output data rate selection. */
  if (lps28dfw_mode_set(&(pObj->Ctx), &pObj->last_odr) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  pObj->press_is_enabled = 1;

  return LPS28DFW_OK;
}

/**
  * @brief  Disable the LPS28DFW pressure sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_PRESS_Disable(LPS28DFW_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->press_is_enabled == 0U)
  {
    return LPS28DFW_OK;
  }

  /* Check if the LPS28DFW temperature sensor is still enable. */
  /* If yes, skip the disable function, if not call disable function */
  if (pObj->temp_is_enabled == 0U)
  {
    lps28dfw_md_t val;

    /* Get current output data rate. */
    if (lps28dfw_mode_get(&(pObj->Ctx), &val) != LPS28DFW_OK)
    {
      return LPS28DFW_ERROR;
    }

    memcpy(&pObj->last_odr, &val, sizeof(lps28dfw_md_t));

    val.odr = LPS28DFW_ONE_SHOT;

    /* Output data rate selection - power down. */
    if (lps28dfw_mode_set(&(pObj->Ctx), &val) != LPS28DFW_OK)
    {
      return LPS28DFW_ERROR;
    }
  }

  pObj->press_is_enabled = 0;

  return LPS28DFW_OK;
}

/**
  * @brief  Get the LPS28DFW pressure sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_PRESS_GetOutputDataRate(LPS28DFW_Object_t *pObj, float *Odr)
{
  return LPS28DFW_GetOutputDataRate(pObj, Odr);
}

/**
  * @brief  Set the LPS28DFW pressure sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_PRESS_SetOutputDataRate(LPS28DFW_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->press_is_enabled == 1U)
  {
    return LPS28DFW_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return LPS28DFW_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the LPS28DFW pressure value
  * @param  pObj the device pObj
  * @param  Value pointer where the pressure value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_PRESS_GetPressure(LPS28DFW_Object_t *pObj, float *Value)
{
  lps28dfw_md_t md;
  lps28dfw_data_t data;

  if (lps28dfw_mode_get(&(pObj->Ctx), &md) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  if (md.fs != LPS28DFW_1260hPa)  /* NOTE: Currently only 1260 hPa full scale supported */
  {
    md.fs = LPS28DFW_1260hPa;

    if (lps28dfw_mode_set(&(pObj->Ctx), &md) != LPS28DFW_OK)
    {
      return LPS28DFW_ERROR;
    }
  }

  if (lps28dfw_data_get(&(pObj->Ctx), &md, &data) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  *Value = data.pressure.hpa;

  return LPS28DFW_OK;
}

/**
  * @brief  Get the LPS28DFW pressure data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_PRESS_Get_DRDY_Status(LPS28DFW_Object_t *pObj, uint8_t *Status)
{
  lps28dfw_status_t reg;

  if (lps28dfw_read_reg(&(pObj->Ctx), LPS28DFW_STATUS, (uint8_t *) &reg, 1) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  *Status = reg.p_da;

  return LPS28DFW_OK;
}

/**
  * @brief  Enable the LPS28DFW temperature sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_TEMP_Enable(LPS28DFW_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->temp_is_enabled == 1U)
  {
    return LPS28DFW_OK;
  }

  /* Output data rate selection. */
  if (lps28dfw_mode_set(&(pObj->Ctx), &pObj->last_odr) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  pObj->temp_is_enabled = 1;

  return LPS28DFW_OK;
}

/**
  * @brief  Disable the LPS28DFW temperature sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_TEMP_Disable(LPS28DFW_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->temp_is_enabled == 0U)
  {
    return LPS28DFW_OK;
  }

  /* Check if the LPS28DFW pressure sensor is still enable. */
  /* If yes, skip the disable function, if not call disable function */
  if (pObj->press_is_enabled == 0U)
  {
    lps28dfw_md_t val;

    /* Get current output data rate. */
    if (lps28dfw_mode_get(&(pObj->Ctx), &val) != LPS28DFW_OK)
    {
      return LPS28DFW_ERROR;
    }

    memcpy(&pObj->last_odr, &val, sizeof(lps28dfw_md_t));

    val.odr = LPS28DFW_ONE_SHOT;

    /* Output data rate selection - power down. */
    if (lps28dfw_mode_set(&(pObj->Ctx), &val) != LPS28DFW_OK)
    {
      return LPS28DFW_ERROR;
    }
  }

  pObj->temp_is_enabled = 0;

  return LPS28DFW_OK;
}

/**
  * @brief  Get the LPS28DFW temperature sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_TEMP_GetOutputDataRate(LPS28DFW_Object_t *pObj, float *Odr)
{
  return LPS28DFW_GetOutputDataRate(pObj, Odr);
}

/**
  * @brief  Set the LPS28DFW temperature sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_TEMP_SetOutputDataRate(LPS28DFW_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->temp_is_enabled == 1U)
  {
    return LPS28DFW_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return LPS28DFW_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the LPS28DFW temperature value
  * @param  pObj the device pObj
  * @param  Value pointer where the temperature value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_TEMP_GetTemperature(LPS28DFW_Object_t *pObj, float *Value)
{
  lps28dfw_md_t md;
  lps28dfw_data_t data;

  if (lps28dfw_mode_get(&(pObj->Ctx), &md) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  if (md.fs != LPS28DFW_1260hPa)  /* NOTE: Currently only 1260 hPa full scale supported */
  {
    md.fs = LPS28DFW_1260hPa;

    if (lps28dfw_mode_set(&(pObj->Ctx), &md) != LPS28DFW_OK)
    {
      return LPS28DFW_ERROR;
    }
  }

  if (lps28dfw_data_get(&(pObj->Ctx), &md, &data) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  *Value = data.heat.deg_c;

  return LPS28DFW_OK;
}

/**
  * @brief  Get the LPS28DFW temperature data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_TEMP_Get_DRDY_Status(LPS28DFW_Object_t *pObj, uint8_t *Status)
{
  lps28dfw_status_t reg;

  if (lps28dfw_read_reg(&(pObj->Ctx), LPS28DFW_STATUS, (uint8_t *) &reg, 1) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  *Status = reg.t_da;

  return LPS28DFW_OK;
}

/**
  * @brief  Get the LPS28DFW register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_Read_Reg(LPS28DFW_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (lps28dfw_read_reg(&(pObj->Ctx), Reg, Data, 1) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  return LPS28DFW_OK;
}

/**
  * @brief  Set the LPS28DFW register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_Write_Reg(LPS28DFW_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (lps28dfw_write_reg(&(pObj->Ctx), Reg, &Data, 1) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  return LPS28DFW_OK;
}

/**
  * @}
  */

/** @defgroup LPS28DFW_Private_Functions LPS28DFW Private Functions
  * @{
  */

/**
  * @brief  Get output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LPS28DFW_GetOutputDataRate(LPS28DFW_Object_t *pObj, float *Odr)
{
  int32_t ret = LPS28DFW_OK;
  lps28dfw_md_t val;

  if (lps28dfw_mode_get(&(pObj->Ctx), &val) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  switch (val.odr)
  {
    case LPS28DFW_ONE_SHOT:
      *Odr = 0.0f;
      break;

    case LPS28DFW_1Hz:
      *Odr = 1.0f;
      break;

    case LPS28DFW_4Hz:
      *Odr = 4.0f;
      break;

    case LPS28DFW_10Hz:
      *Odr = 10.0f;
      break;

    case LPS28DFW_25Hz:
      *Odr = 25.0f;
      break;

    case LPS28DFW_50Hz:
      *Odr = 50.0f;
      break;

    case LPS28DFW_75Hz:
      *Odr = 75.0f;
      break;

    case LPS28DFW_100Hz:
      *Odr = 100.0f;
      break;

    case LPS28DFW_200Hz:
      *Odr = 200.0f;
      break;

    default:
      ret = LPS28DFW_ERROR;
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
static int32_t LPS28DFW_SetOutputDataRate_When_Enabled(LPS28DFW_Object_t *pObj, float Odr)
{
  lps28dfw_md_t new_val;

  if (lps28dfw_mode_get(&(pObj->Ctx), &new_val) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  new_val.odr = (Odr <=   1.0f) ? LPS28DFW_1Hz
                : (Odr <=   4.0f) ? LPS28DFW_4Hz
                : (Odr <=  10.0f) ? LPS28DFW_10Hz
                : (Odr <=  25.0f) ? LPS28DFW_25Hz
                : (Odr <=  50.0f) ? LPS28DFW_50Hz
                : (Odr <=  75.0f) ? LPS28DFW_75Hz
                : (Odr <= 100.0f) ? LPS28DFW_100Hz
                :                   LPS28DFW_200Hz;

  if (lps28dfw_mode_set(&(pObj->Ctx), &new_val) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  if (lps28dfw_mode_get(&(pObj->Ctx), &pObj->last_odr) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  return LPS28DFW_OK;
}

/**
  * @brief  Set output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LPS28DFW_SetOutputDataRate_When_Disabled(LPS28DFW_Object_t *pObj, float Odr)
{
  pObj->last_odr.odr = (Odr <=   1.0f) ? LPS28DFW_1Hz
                       : (Odr <=   4.0f) ? LPS28DFW_4Hz
                       : (Odr <=  10.0f) ? LPS28DFW_10Hz
                       : (Odr <=  25.0f) ? LPS28DFW_25Hz
                       : (Odr <=  50.0f) ? LPS28DFW_50Hz
                       : (Odr <=  75.0f) ? LPS28DFW_75Hz
                       : (Odr <= 100.0f) ? LPS28DFW_100Hz
                       :                   LPS28DFW_200Hz;

  return LPS28DFW_OK;
}

/**
  * @brief  Initialize the LPS28DFW sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LPS28DFW_Initialize(LPS28DFW_Object_t *pObj)
{
  lps28dfw_md_t md;
  lps28dfw_bus_mode_t bus_mode;

  /* Set bdu and if_inc recommended for driver usage */
  if (lps28dfw_init_set(&(pObj->Ctx), LPS28DFW_DRV_RDY) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  /* Select bus interface */
  bus_mode.interface = LPS28DFW_SEL_BY_HW;

  bus_mode.filter = LPS28DFW_AUTO;
  if (lps28dfw_bus_mode_set(&(pObj->Ctx), &bus_mode) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  /* Set Output Data Rate in Power Down */
  md.odr = LPS28DFW_ONE_SHOT;
  md.avg = LPS28DFW_4_AVG;
  md.lpf = LPS28DFW_LPF_ODR_DIV_4;

  /* Power down the device */
  if (lps28dfw_mode_set(&(pObj->Ctx), &md) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  if (lps28dfw_mode_get(&(pObj->Ctx), &pObj->last_odr) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  pObj->last_odr.odr = LPS28DFW_25Hz;

  return LPS28DFW_OK;
}

/**
  * @brief  Set the LPS28DFW One Shot Mode
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_Set_One_Shot(LPS28DFW_Object_t *pObj)
{
  lps28dfw_md_t md;

  if (lps28dfw_mode_get(&(pObj->Ctx), &md) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  /* Start One Shot Measurement */
  if (lps28dfw_trigger_sw(&(pObj->Ctx), &md) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  return LPS28DFW_OK;
}

/**
  * @brief  Get the LPS28DFW One Shot Status
  * @param  pObj the device pObj
  * @param  Status pointer to the one shot status (1 means measurements available, 0 means measurements not available yet)
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_Get_One_Shot_Status(LPS28DFW_Object_t *pObj, uint8_t *Status)
{
  uint8_t p_da;
  uint8_t t_da;

  /* Get DataReady for pressure */
  if (LPS28DFW_PRESS_Get_DRDY_Status(pObj, &p_da) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  /* Get DataReady for temperature */
  if (LPS28DFW_TEMP_Get_DRDY_Status(pObj, &t_da) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  if (p_da && t_da)
  {
    *Status = 1;
  }
  else
  {
    *Status = 0;
  }

  return LPS28DFW_OK;
}

/**
  * @brief  Enable LPS28DFW DRDY interrupt mode
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_Enable_DRDY_Interrupt(LPS28DFW_Object_t *pObj)
{
  lps28dfw_ctrl_reg4_t ctrl_reg4;

  /* Enable Interrupt DRDY on INT1 */
  if (lps28dfw_read_reg(&(pObj->Ctx), LPS28DFW_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }
  ctrl_reg4.drdy = 1;
  if (lps28dfw_write_reg(&(pObj->Ctx), LPS28DFW_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  return LPS28DFW_OK;
}

/**
  * @brief  Set the LPS28DFW averaging selection
  * @param  pObj the device pObj
  * @param  avg averaging selection to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_Set_AVG(LPS28DFW_Object_t *pObj, uint8_t avg)
{
  lps28dfw_md_t md;

  if (avg > 7)
  {
    return LPS28DFW_ERROR;
  }

  if (lps28dfw_mode_get(&(pObj->Ctx), &md) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  switch (avg)
  {
    case 0:
    default:
      md.avg = LPS28DFW_4_AVG;
      break;
    case 1:
      md.avg = LPS28DFW_8_AVG;
      break;
    case 2:
      md.avg = LPS28DFW_16_AVG;
      break;
    case 3:
      md.avg = LPS28DFW_32_AVG;
      break;
    case 4:
      md.avg = LPS28DFW_64_AVG;
      break;
    case 5:
      md.avg = LPS28DFW_128_AVG;
      break;
    case 6:
      md.avg = LPS28DFW_256_AVG;
      break;
    case 7:
      md.avg = LPS28DFW_512_AVG;
      break;
  }

  if (lps28dfw_mode_set(&(pObj->Ctx), &md) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  return LPS28DFW_OK;
}

/**
  * @brief  Set the LPS28DFW low pass filter
  * @param  pObj the device pObj
  * @param  lpf low pass filter mode to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS28DFW_Set_LPF(LPS28DFW_Object_t *pObj, uint8_t lpf)
{
  lps28dfw_md_t md;

  if (lpf != 0 && lpf != 1 && lpf != 3)
  {
    return LPS28DFW_ERROR;
  }

  if (lps28dfw_mode_get(&(pObj->Ctx), &md) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  switch (lpf)
  {
    case 0:
    default:
      md.lpf = LPS28DFW_LPF_DISABLE;
      break;
    case 1:
      md.lpf = LPS28DFW_LPF_ODR_DIV_4;
      break;
    case 3:
      md.lpf = LPS28DFW_LPF_ODR_DIV_9;
      break;
  }

  if (lps28dfw_mode_set(&(pObj->Ctx), &md) != LPS28DFW_OK)
  {
    return LPS28DFW_ERROR;
  }

  return LPS28DFW_OK;
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
  LPS28DFW_Object_t *pObj = (LPS28DFW_Object_t *)Handle;

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
  LPS28DFW_Object_t *pObj = (LPS28DFW_Object_t *)Handle;

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
