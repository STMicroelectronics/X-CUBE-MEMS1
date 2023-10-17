/**
  ******************************************************************************
  * @file    lps22df.c
  * @author  MEMS Software Solutions Team
  * @brief   LPS22DF driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lps22df.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup LPS22DF LPS22DF
  * @{
  */

/** @defgroup LPS22DF_Exported_Variables LPS22DF Exported Variables
  * @{
  */

LPS22DF_CommonDrv_t LPS22DF_COMMON_Driver =
{
  LPS22DF_Init,
  LPS22DF_DeInit,
  LPS22DF_ReadID,
  LPS22DF_GetCapabilities,
};

LPS22DF_PRESS_Drv_t LPS22DF_PRESS_Driver =
{
  LPS22DF_PRESS_Enable,
  LPS22DF_PRESS_Disable,
  LPS22DF_PRESS_GetOutputDataRate,
  LPS22DF_PRESS_SetOutputDataRate,
  LPS22DF_PRESS_GetPressure,
};

LPS22DF_TEMP_Drv_t LPS22DF_TEMP_Driver =
{
  LPS22DF_TEMP_Enable,
  LPS22DF_TEMP_Disable,
  LPS22DF_TEMP_GetOutputDataRate,
  LPS22DF_TEMP_SetOutputDataRate,
  LPS22DF_TEMP_GetTemperature,
};

/**
  * @}
  */

/** @defgroup LPS22DF_Private_Function_Prototypes LPS22DF Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t LPS22DF_GetOutputDataRate(LPS22DF_Object_t *pObj, float *Odr);
static int32_t LPS22DF_SetOutputDataRate_When_Enabled(LPS22DF_Object_t *pObj, float Odr);
static int32_t LPS22DF_SetOutputDataRate_When_Disabled(LPS22DF_Object_t *pObj, float Odr);
static int32_t LPS22DF_Initialize(LPS22DF_Object_t *pObj);

/**
  * @}
  */

/** @defgroup LPS22DF_Exported_Functions LPS22DF Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_RegisterBusIO(LPS22DF_Object_t *pObj, LPS22DF_IO_t *pIO)
{
  int32_t ret = LPS22DF_OK;

  if (pObj == NULL)
  {
    ret = LPS22DF_ERROR;
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
      ret = LPS22DF_ERROR;
    }
    else if (pObj->IO.Init() != LPS22DF_OK)
    {
      ret = LPS22DF_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == LPS22DF_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x20;

          if (LPS22DF_Write_Reg(pObj, LPS22DF_IF_CTRL, data) != LPS22DF_OK)
          {
            ret = LPS22DF_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  Initialize the LPS22DF sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_Init(LPS22DF_Object_t *pObj)
{
  if (pObj->is_initialized == 0U)
  {
    if (LPS22DF_Initialize(pObj) != LPS22DF_OK)
    {
      return LPS22DF_ERROR;
    }
  }

  pObj->is_initialized = 1U;

  return LPS22DF_OK;
}

/**
  * @brief  Deinitialize the LPS22DF sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_DeInit(LPS22DF_Object_t *pObj)
{
  if (pObj->is_initialized == 1U)
  {
    if (LPS22DF_PRESS_Disable(pObj) != LPS22DF_OK)
    {
      return LPS22DF_ERROR;
    }

    if (LPS22DF_TEMP_Disable(pObj) != LPS22DF_OK)
    {
      return LPS22DF_ERROR;
    }
  }

  pObj->is_initialized = 0;

  return LPS22DF_OK;
}

/**
  * @brief  Get WHO_AM_I value
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_ReadID(LPS22DF_Object_t *pObj, uint8_t *Id)
{
  lps22df_id_t val;
  if (lps22df_id_get(&(pObj->Ctx), &val) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  *Id = val.whoami;

  return LPS22DF_OK;
}

/**
  * @brief  Get LPS22DF sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to LPS22DF sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_GetCapabilities(LPS22DF_Object_t *pObj, LPS22DF_Capabilities_t *Capabilities)
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
  return LPS22DF_OK;
}

/**
  * @brief  Get the LPS22DF initialization status
  * @param  pObj the device pObj
  * @param  Status 1 if initialized, 0 otherwise
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_Get_Init_Status(LPS22DF_Object_t *pObj, uint8_t *Status)
{
  if (pObj == NULL)
  {
    return LPS22DF_ERROR;
  }

  *Status = pObj->is_initialized;

  return LPS22DF_OK;
}

/**
  * @brief  Enable the LPS22DF pressure sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_PRESS_Enable(LPS22DF_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->press_is_enabled == 1U)
  {
    return LPS22DF_OK;
  }

  /* Output data rate selection. */
  if (lps22df_mode_set(&(pObj->Ctx), &pObj->last_odr) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  pObj->press_is_enabled = 1;

  return LPS22DF_OK;
}

/**
  * @brief  Disable the LPS22DF pressure sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_PRESS_Disable(LPS22DF_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->press_is_enabled == 0U)
  {
    return LPS22DF_OK;
  }

  /* Check if the LPS22DF temperature sensor is still enable. */
  /* If yes, skip the disable function, if not call disable function */
  if (pObj->temp_is_enabled == 0U)
  {
    lps22df_md_t val;

    /* Get current output data rate. */
    if (lps22df_mode_get(&(pObj->Ctx), &val) != LPS22DF_OK)
    {
      return LPS22DF_ERROR;
    }

    memcpy(&pObj->last_odr, &val, sizeof(lps22df_md_t));

    val.odr = LPS22DF_ONE_SHOT;

    /* Output data rate selection - power down. */
    if (lps22df_mode_set(&(pObj->Ctx), &val) != LPS22DF_OK)
    {
      return LPS22DF_ERROR;
    }
  }

  pObj->press_is_enabled = 0;

  return LPS22DF_OK;
}

/**
  * @brief  Get the LPS22DF pressure sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_PRESS_GetOutputDataRate(LPS22DF_Object_t *pObj, float *Odr)
{
  return LPS22DF_GetOutputDataRate(pObj, Odr);
}

/**
  * @brief  Set the LPS22DF pressure sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_PRESS_SetOutputDataRate(LPS22DF_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->press_is_enabled == 1U)
  {
    return LPS22DF_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return LPS22DF_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the LPS22DF pressure value
  * @param  pObj the device pObj
  * @param  Value pointer where the pressure value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_PRESS_GetPressure(LPS22DF_Object_t *pObj, float *Value)
{
  lps22df_data_t data;

  if (lps22df_data_get(&(pObj->Ctx), &data) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  *Value = data.pressure.hpa;

  return LPS22DF_OK;
}

/**
  * @brief  Get the LPS22DF pressure data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_PRESS_Get_DRDY_Status(LPS22DF_Object_t *pObj, uint8_t *Status)
{
  lps22df_status_t reg;

  if (lps22df_read_reg(&(pObj->Ctx), LPS22DF_STATUS, (uint8_t *) &reg, 1) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  *Status = reg.p_da;

  return LPS22DF_OK;
}

/**
  * @brief  Enable the LPS22DF temperature sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_TEMP_Enable(LPS22DF_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->temp_is_enabled == 1U)
  {
    return LPS22DF_OK;
  }

  /* Output data rate selection. */
  if (lps22df_mode_set(&(pObj->Ctx), &pObj->last_odr) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  pObj->temp_is_enabled = 1;

  return LPS22DF_OK;
}

/**
  * @brief  Disable the LPS22DF temperature sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_TEMP_Disable(LPS22DF_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->temp_is_enabled == 0U)
  {
    return LPS22DF_OK;
  }

  /* Check if the LPS22DF pressure sensor is still enable. */
  /* If yes, skip the disable function, if not call disable function */
  if (pObj->press_is_enabled == 0U)
  {
    lps22df_md_t val;

    /* Get current output data rate. */
    if (lps22df_mode_get(&(pObj->Ctx), &val) != LPS22DF_OK)
    {
      return LPS22DF_ERROR;
    }

    memcpy(&pObj->last_odr, &val, sizeof(lps22df_md_t));

    val.odr = LPS22DF_ONE_SHOT;

    /* Output data rate selection - power down. */
    if (lps22df_mode_set(&(pObj->Ctx), &val) != LPS22DF_OK)
    {
      return LPS22DF_ERROR;
    }
  }

  pObj->temp_is_enabled = 0;

  return LPS22DF_OK;
}

/**
  * @brief  Get the LPS22DF temperature sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_TEMP_GetOutputDataRate(LPS22DF_Object_t *pObj, float *Odr)
{
  return LPS22DF_GetOutputDataRate(pObj, Odr);
}

/**
  * @brief  Set the LPS22DF temperature sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_TEMP_SetOutputDataRate(LPS22DF_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->temp_is_enabled == 1U)
  {
    return LPS22DF_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return LPS22DF_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the LPS22DF temperature value
  * @param  pObj the device pObj
  * @param  Value pointer where the temperature value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_TEMP_GetTemperature(LPS22DF_Object_t *pObj, float *Value)
{
  lps22df_data_t data;

  if (lps22df_data_get(&(pObj->Ctx), &data) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  *Value = data.heat.deg_c;

  return LPS22DF_OK;
}

/**
  * @brief  Get the LPS22DF temperature data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_TEMP_Get_DRDY_Status(LPS22DF_Object_t *pObj, uint8_t *Status)
{
  lps22df_status_t reg;

  if (lps22df_read_reg(&(pObj->Ctx), LPS22DF_STATUS, (uint8_t *) &reg, 1) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  *Status = reg.t_da;

  return LPS22DF_OK;
}

/**
  * @brief  Get the LPS22DF register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_Read_Reg(LPS22DF_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (lps22df_read_reg(&(pObj->Ctx), Reg, Data, 1) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  return LPS22DF_OK;
}

/**
  * @brief  Set the LPS22DF register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_Write_Reg(LPS22DF_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (lps22df_write_reg(&(pObj->Ctx), Reg, &Data, 1) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  return LPS22DF_OK;
}

/**
  * @}
  */

/** @defgroup LPS22DF_Private_Functions LPS22DF Private Functions
  * @{
  */

/**
  * @brief  Get output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LPS22DF_GetOutputDataRate(LPS22DF_Object_t *pObj, float *Odr)
{
  int32_t ret = LPS22DF_OK;
  lps22df_md_t val;

  if (lps22df_mode_get(&(pObj->Ctx), &val) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  switch (val.odr)
  {
    case LPS22DF_ONE_SHOT:
      *Odr = 0.0f;
      break;

    case LPS22DF_1Hz:
      *Odr = 1.0f;
      break;

    case LPS22DF_4Hz:
      *Odr = 4.0f;
      break;

    case LPS22DF_10Hz:
      *Odr = 10.0f;
      break;

    case LPS22DF_25Hz:
      *Odr = 25.0f;
      break;

    case LPS22DF_50Hz:
      *Odr = 50.0f;
      break;

    case LPS22DF_75Hz:
      *Odr = 75.0f;
      break;

    case LPS22DF_100Hz:
      *Odr = 100.0f;
      break;

    case LPS22DF_200Hz:
      *Odr = 200.0f;
      break;

    default:
      ret = LPS22DF_ERROR;
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
static int32_t LPS22DF_SetOutputDataRate_When_Enabled(LPS22DF_Object_t *pObj, float Odr)
{
  lps22df_md_t new_val;

  if (lps22df_mode_get(&(pObj->Ctx), &new_val) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  new_val.odr = (Odr <=   1.0f) ? LPS22DF_1Hz
                : (Odr <=   4.0f) ? LPS22DF_4Hz
                : (Odr <=  10.0f) ? LPS22DF_10Hz
                : (Odr <=  25.0f) ? LPS22DF_25Hz
                : (Odr <=  50.0f) ? LPS22DF_50Hz
                : (Odr <=  75.0f) ? LPS22DF_75Hz
                : (Odr <= 100.0f) ? LPS22DF_100Hz
                :                   LPS22DF_200Hz;

  if (lps22df_mode_set(&(pObj->Ctx), &new_val) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  if (lps22df_mode_get(&(pObj->Ctx), &pObj->last_odr) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  return LPS22DF_OK;
}

/**
  * @brief  Set output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LPS22DF_SetOutputDataRate_When_Disabled(LPS22DF_Object_t *pObj, float Odr)
{
  pObj->last_odr.odr = (Odr <=   1.0f) ? LPS22DF_1Hz
                       : (Odr <=   4.0f) ? LPS22DF_4Hz
                       : (Odr <=  10.0f) ? LPS22DF_10Hz
                       : (Odr <=  25.0f) ? LPS22DF_25Hz
                       : (Odr <=  50.0f) ? LPS22DF_50Hz
                       : (Odr <=  75.0f) ? LPS22DF_75Hz
                       : (Odr <= 100.0f) ? LPS22DF_100Hz
                       :                   LPS22DF_200Hz;

  return LPS22DF_OK;
}

/**
  * @brief  Initialize the LPS22DF sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LPS22DF_Initialize(LPS22DF_Object_t *pObj)
{
  lps22df_md_t md;
  lps22df_bus_mode_t bus_mode;

  /* Set bdu and if_inc recommended for driver usage */
  if (lps22df_init_set(&(pObj->Ctx), LPS22DF_DRV_RDY) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  /* Select bus interface */
  if (pObj->IO.BusType == LPS22DF_SPI_3WIRES_BUS) /* SPI 3-Wires */
  {
    bus_mode.interface = LPS22DF_SPI_3W;
  }
  else if (pObj->IO.BusType == LPS22DF_SPI_4WIRES_BUS) /* SPI 4-Wires */
  {
    bus_mode.interface = LPS22DF_SPI_4W;
  }
  else
  {
    bus_mode.interface = LPS22DF_SEL_BY_HW;
  }

  bus_mode.filter = LPS22DF_AUTO;
  if (lps22df_bus_mode_set(&(pObj->Ctx), &bus_mode) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  /* Set Output Data Rate in Power Down */
  md.odr = LPS22DF_ONE_SHOT;
  md.avg = LPS22DF_4_AVG;
  md.lpf = LPS22DF_LPF_ODR_DIV_4;

  /* Power down the device */
  if (lps22df_mode_set(&(pObj->Ctx), &md) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  if (lps22df_mode_get(&(pObj->Ctx), &pObj->last_odr) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  pObj->last_odr.odr = LPS22DF_25Hz;

  return LPS22DF_OK;
}

/**
  * @brief  Set the LPS22DF One Shot Mode
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_Set_One_Shot(LPS22DF_Object_t *pObj)
{
  lps22df_md_t md;

  if (lps22df_mode_get(&(pObj->Ctx), &md) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  /* Start One Shot Measurement */
  if (lps22df_trigger_sw(&(pObj->Ctx), &md) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  return LPS22DF_OK;
}

/**
  * @brief  Get the LPS22DF One Shot Status
  * @param  pObj the device pObj
  * @param  Status pointer to the one shot status (1 means measurements available, 0 means measurements not available yet)
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_Get_One_Shot_Status(LPS22DF_Object_t *pObj, uint8_t *Status)
{
  uint8_t p_da;
  uint8_t t_da;

  /* Get DataReady for pressure */
  if (LPS22DF_PRESS_Get_DRDY_Status(pObj, &p_da) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  /* Get DataReady for temperature */
  if (LPS22DF_TEMP_Get_DRDY_Status(pObj, &t_da) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  if (p_da && t_da)
  {
    *Status = 1;
  }
  else
  {
    *Status = 0;
  }

  return LPS22DF_OK;
}

/**
  * @brief  Enable LPS22DF DRDY interrupt mode
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_Enable_DRDY_Interrupt(LPS22DF_Object_t *pObj)
{
  lps22df_ctrl_reg4_t ctrl_reg4;

  /* Enable Interrupt DRDY on INT1 */
  if (lps22df_read_reg(&(pObj->Ctx), LPS22DF_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }
  ctrl_reg4.drdy = 1;
  if (lps22df_write_reg(&(pObj->Ctx), LPS22DF_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  return LPS22DF_OK;
}

/**
  * @brief  Set the LPS22DF averaging selection
  * @param  pObj the device pObj
  * @param  avg averaging selection to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_Set_AVG(LPS22DF_Object_t *pObj, uint8_t avg)
{
  lps22df_md_t md;

  if (avg > 7)
  {
    return LPS22DF_ERROR;
  }

  if (lps22df_mode_get(&(pObj->Ctx), &md) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  switch (avg)
  {
    case 0:
    default:
      md.avg = LPS22DF_4_AVG;
      break;
    case 1:
      md.avg = LPS22DF_8_AVG;
      break;
    case 2:
      md.avg = LPS22DF_16_AVG;
      break;
    case 3:
      md.avg = LPS22DF_32_AVG;
      break;
    case 4:
      md.avg = LPS22DF_64_AVG;
      break;
    case 5:
      md.avg = LPS22DF_128_AVG;
      break;
    case 6:
      md.avg = LPS22DF_256_AVG;
      break;
    case 7:
      md.avg = LPS22DF_512_AVG;
      break;
  }

  if (lps22df_mode_set(&(pObj->Ctx), &md) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  return LPS22DF_OK;
}

/**
  * @brief  Set the LPS22DF low pass filter
  * @param  pObj the device pObj
  * @param  lpf low pass filter mode to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LPS22DF_Set_LPF(LPS22DF_Object_t *pObj, uint8_t lpf)
{
  lps22df_md_t md;

  if (lpf != 0 && lpf != 1 && lpf != 3)
  {
    return LPS22DF_ERROR;
  }

  if (lps22df_mode_get(&(pObj->Ctx), &md) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  switch (lpf)
  {
    case 0:
    default:
      md.lpf = LPS22DF_LPF_DISABLE;
      break;
    case 1:
      md.lpf = LPS22DF_LPF_ODR_DIV_4;
      break;
    case 3:
      md.lpf = LPS22DF_LPF_ODR_DIV_9;
      break;
  }

  if (lps22df_mode_set(&(pObj->Ctx), &md) != LPS22DF_OK)
  {
    return LPS22DF_ERROR;
  }

  return LPS22DF_OK;
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
  LPS22DF_Object_t *pObj = (LPS22DF_Object_t *)Handle;

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
  LPS22DF_Object_t *pObj = (LPS22DF_Object_t *)Handle;

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
