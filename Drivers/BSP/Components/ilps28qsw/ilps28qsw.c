/**
  ******************************************************************************
  * @file    ilps28qsw.c
  * @author  MEMS Software Solutions Team
  * @brief   ILPS28QSW driver file
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
#include "ilps28qsw.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup ILPS28QSW ILPS28QSW
  * @{
  */

/** @defgroup ILPS28QSW_Exported_Variables ILPS28QSW Exported Variables
  * @{
  */

ILPS28QSW_CommonDrv_t ILPS28QSW_COMMON_Driver =
{
  ILPS28QSW_Init,
  ILPS28QSW_DeInit,
  ILPS28QSW_ReadID,
  ILPS28QSW_GetCapabilities,
};

ILPS28QSW_PRESS_Drv_t ILPS28QSW_PRESS_Driver =
{
  ILPS28QSW_PRESS_Enable,
  ILPS28QSW_PRESS_Disable,
  ILPS28QSW_PRESS_GetOutputDataRate,
  ILPS28QSW_PRESS_SetOutputDataRate,
  ILPS28QSW_PRESS_GetPressure,
};

ILPS28QSW_TEMP_Drv_t ILPS28QSW_TEMP_Driver =
{
  ILPS28QSW_TEMP_Enable,
  ILPS28QSW_TEMP_Disable,
  ILPS28QSW_TEMP_GetOutputDataRate,
  ILPS28QSW_TEMP_SetOutputDataRate,
  ILPS28QSW_TEMP_GetTemperature,
};

/**
  * @}
  */

/** @defgroup ILPS28QSW_Private_Function_Prototypes ILPS28QSW Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t ILPS28QSW_GetOutputDataRate(ILPS28QSW_Object_t *pObj, float *Odr);
static int32_t ILPS28QSW_SetOutputDataRate_When_Enabled(ILPS28QSW_Object_t *pObj, float Odr);
static int32_t ILPS28QSW_SetOutputDataRate_When_Disabled(ILPS28QSW_Object_t *pObj, float Odr);
static int32_t ILPS28QSW_Initialize(ILPS28QSW_Object_t *pObj);

/**
  * @}
  */

/** @defgroup ILPS28QSW_Exported_Functions ILPS28QSW Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_RegisterBusIO(ILPS28QSW_Object_t *pObj, ILPS28QSW_IO_t *pIO)
{
  int32_t ret = ILPS28QSW_OK;

  if (pObj == NULL)
  {
    ret = ILPS28QSW_ERROR;
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
      ret = ILPS28QSW_ERROR;
    }
    else if (pObj->IO.Init() != ILPS28QSW_OK)
    {
      ret = ILPS28QSW_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == ILPS28QSW_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x20;

          if (ILPS28QSW_Write_Reg(pObj, ILPS28QSW_IF_CTRL, data) != ILPS28QSW_OK)
          {
            ret = ILPS28QSW_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  Initialize the ILPS28QSW sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_Init(ILPS28QSW_Object_t *pObj)
{
  if (pObj->is_initialized == 0U)
  {
    if (ILPS28QSW_Initialize(pObj) != ILPS28QSW_OK)
    {
      return ILPS28QSW_ERROR;
    }
  }

  pObj->is_initialized = 1U;

  return ILPS28QSW_OK;
}

/**
  * @brief  Deinitialize the ILPS28QSW sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_DeInit(ILPS28QSW_Object_t *pObj)
{
  if (pObj->is_initialized == 1U)
  {
    if (ILPS28QSW_PRESS_Disable(pObj) != ILPS28QSW_OK)
    {
      return ILPS28QSW_ERROR;
    }

    if (ILPS28QSW_TEMP_Disable(pObj) != ILPS28QSW_OK)
    {
      return ILPS28QSW_ERROR;
    }
  }

  pObj->is_initialized = 0;

  return ILPS28QSW_OK;
}

/**
  * @brief  Get WHO_AM_I value
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_ReadID(ILPS28QSW_Object_t *pObj, uint8_t *Id)
{
  ilps28qsw_id_t val;
  if (ilps28qsw_id_get(&(pObj->Ctx), &val) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  *Id = val.whoami;

  return ILPS28QSW_OK;
}

/**
  * @brief  Get ILPS28QSW sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to ILPS28QSW sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_GetCapabilities(ILPS28QSW_Object_t *pObj, ILPS28QSW_Capabilities_t *Capabilities)
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
  return ILPS28QSW_OK;
}

/**
  * @brief  Get the ILPS28QSW initialization status
  * @param  pObj the device pObj
  * @param  Status 1 if initialized, 0 otherwise
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_Get_Init_Status(ILPS28QSW_Object_t *pObj, uint8_t *Status)
{
  if (pObj == NULL)
  {
    return ILPS28QSW_ERROR;
  }

  *Status = pObj->is_initialized;

  return ILPS28QSW_OK;
}

/**
  * @brief  Enable the ILPS28QSW pressure sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_PRESS_Enable(ILPS28QSW_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->press_is_enabled == 1U)
  {
    return ILPS28QSW_OK;
  }

  /* Output data rate selection. */
  if (ilps28qsw_mode_set(&(pObj->Ctx), &pObj->last_odr) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  pObj->press_is_enabled = 1;

  return ILPS28QSW_OK;
}

/**
  * @brief  Disable the ILPS28QSW pressure sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_PRESS_Disable(ILPS28QSW_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->press_is_enabled == 0U)
  {
    return ILPS28QSW_OK;
  }

  /* Check if the ILPS28QSW temperature sensor is still enable. */
  /* If yes, skip the disable function, if not call disable function */
  if (pObj->temp_is_enabled == 0U)
  {
    ilps28qsw_md_t val;

    /* Get current output data rate. */
    if (ilps28qsw_mode_get(&(pObj->Ctx), &val) != ILPS28QSW_OK)
    {
      return ILPS28QSW_ERROR;
    }

    memcpy(&pObj->last_odr, &val, sizeof(ilps28qsw_md_t));

    val.odr = ILPS28QSW_ONE_SHOT;

    /* Output data rate selection - power down. */
    if (ilps28qsw_mode_set(&(pObj->Ctx), &val) != ILPS28QSW_OK)
    {
      return ILPS28QSW_ERROR;
    }
  }

  pObj->press_is_enabled = 0;

  return ILPS28QSW_OK;
}

/**
  * @brief  Get the ILPS28QSW pressure sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_PRESS_GetOutputDataRate(ILPS28QSW_Object_t *pObj, float *Odr)
{
  return ILPS28QSW_GetOutputDataRate(pObj, Odr);
}

/**
  * @brief  Set the ILPS28QSW pressure sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_PRESS_SetOutputDataRate(ILPS28QSW_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->press_is_enabled == 1U)
  {
    return ILPS28QSW_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return ILPS28QSW_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the ILPS28QSW pressure value
  * @param  pObj the device pObj
  * @param  Value pointer where the pressure value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_PRESS_GetPressure(ILPS28QSW_Object_t *pObj, float *Value)
{
  ilps28qsw_data_t data;
  ilps28qsw_md_t md;

  if (ilps28qsw_mode_get(&(pObj->Ctx), &md) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  if (md.fs != ILPS28QSW_1260hPa)  /* NOTE: Currently only 1260 hPa full scale supported */
  {
    md.fs = ILPS28QSW_1260hPa;

    if (ilps28qsw_mode_set(&(pObj->Ctx), &md) != ILPS28QSW_OK)
    {
      return ILPS28QSW_ERROR;
    }
  }

  if (ilps28qsw_data_get(&(pObj->Ctx), &md, &data) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  *Value = data.pressure.hpa;

  return ILPS28QSW_OK;
}

/**
  * @brief  Get the ILPS28QSW pressure data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_PRESS_Get_DRDY_Status(ILPS28QSW_Object_t *pObj, uint8_t *Status)
{
  ilps28qsw_status_t reg;

  if (ilps28qsw_read_reg(&(pObj->Ctx), ILPS28QSW_STATUS, (uint8_t *) &reg, 1) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  *Status = reg.p_da;

  return ILPS28QSW_OK;
}

/**
  * @brief  Enable the ILPS28QSW temperature sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_TEMP_Enable(ILPS28QSW_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->temp_is_enabled == 1U)
  {
    return ILPS28QSW_OK;
  }

  /* Output data rate selection. */
  if (ilps28qsw_mode_set(&(pObj->Ctx), &pObj->last_odr) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  pObj->temp_is_enabled = 1;

  return ILPS28QSW_OK;
}

/**
  * @brief  Disable the ILPS28QSW temperature sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_TEMP_Disable(ILPS28QSW_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->temp_is_enabled == 0U)
  {
    return ILPS28QSW_OK;
  }

  /* Check if the ILPS28QSW pressure sensor is still enable. */
  /* If yes, skip the disable function, if not call disable function */
  if (pObj->press_is_enabled == 0U)
  {
    ilps28qsw_md_t val;

    /* Get current output data rate. */
    if (ilps28qsw_mode_get(&(pObj->Ctx), &val) != ILPS28QSW_OK)
    {
      return ILPS28QSW_ERROR;
    }

    memcpy(&pObj->last_odr, &val, sizeof(ilps28qsw_md_t));

    val.odr = ILPS28QSW_ONE_SHOT;

    /* Output data rate selection - power down. */
    if (ilps28qsw_mode_set(&(pObj->Ctx), &val) != ILPS28QSW_OK)
    {
      return ILPS28QSW_ERROR;
    }
  }

  pObj->temp_is_enabled = 0;

  return ILPS28QSW_OK;
}

/**
  * @brief  Get the ILPS28QSW temperature sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_TEMP_GetOutputDataRate(ILPS28QSW_Object_t *pObj, float *Odr)
{
  return ILPS28QSW_GetOutputDataRate(pObj, Odr);
}

/**
  * @brief  Set the ILPS28QSW temperature sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_TEMP_SetOutputDataRate(ILPS28QSW_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->temp_is_enabled == 1U)
  {
    return ILPS28QSW_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return ILPS28QSW_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the ILPS28QSW temperature value
  * @param  pObj the device pObj
  * @param  Value pointer where the temperature value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_TEMP_GetTemperature(ILPS28QSW_Object_t *pObj, float *Value)
{
  ilps28qsw_data_t data;
  ilps28qsw_md_t md;

  if (ilps28qsw_mode_get(&(pObj->Ctx), &md) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  if (md.fs != ILPS28QSW_1260hPa)  /* NOTE: Currently only 1260 hPa full scale supported */
  {
    md.fs = ILPS28QSW_1260hPa;

    if (ilps28qsw_mode_set(&(pObj->Ctx), &md) != ILPS28QSW_OK)
    {
      return ILPS28QSW_ERROR;
    }
  }

  if (ilps28qsw_data_get(&(pObj->Ctx), &md, &data) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  *Value = data.heat.deg_c;

  return ILPS28QSW_OK;
}

/**
  * @brief  Get the ILPS28QSW temperature data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_TEMP_Get_DRDY_Status(ILPS28QSW_Object_t *pObj, uint8_t *Status)
{
  ilps28qsw_status_t reg;

  if (ilps28qsw_read_reg(&(pObj->Ctx), ILPS28QSW_STATUS, (uint8_t *) &reg, 1) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  *Status = reg.t_da;

  return ILPS28QSW_OK;
}

/**
  * @brief  Get the ILPS28QSW register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_Read_Reg(ILPS28QSW_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (ilps28qsw_read_reg(&(pObj->Ctx), Reg, Data, 1) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  return ILPS28QSW_OK;
}

/**
  * @brief  Set the ILPS28QSW register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_Write_Reg(ILPS28QSW_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (ilps28qsw_write_reg(&(pObj->Ctx), Reg, &Data, 1) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  return ILPS28QSW_OK;
}

/**
  * @}
  */

/** @defgroup ILPS28QSW_Private_Functions ILPS28QSW Private Functions
  * @{
  */

/**
  * @brief  Get output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t ILPS28QSW_GetOutputDataRate(ILPS28QSW_Object_t *pObj, float *Odr)
{
  int32_t ret = ILPS28QSW_OK;
  ilps28qsw_md_t val;

  if (ilps28qsw_mode_get(&(pObj->Ctx), &val) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  switch (val.odr)
  {
    case ILPS28QSW_ONE_SHOT:
      *Odr = 0.0f;
      break;

    case ILPS28QSW_1Hz:
      *Odr = 1.0f;
      break;

    case ILPS28QSW_4Hz:
      *Odr = 4.0f;
      break;

    case ILPS28QSW_10Hz:
      *Odr = 10.0f;
      break;

    case ILPS28QSW_25Hz:
      *Odr = 25.0f;
      break;

    case ILPS28QSW_50Hz:
      *Odr = 50.0f;
      break;

    case ILPS28QSW_75Hz:
      *Odr = 75.0f;
      break;

    case ILPS28QSW_100Hz:
      *Odr = 100.0f;
      break;

    case ILPS28QSW_200Hz:
      *Odr = 200.0f;
      break;

    default:
      ret = ILPS28QSW_ERROR;
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
static int32_t ILPS28QSW_SetOutputDataRate_When_Enabled(ILPS28QSW_Object_t *pObj, float Odr)
{
  ilps28qsw_md_t new_val;

  if (ilps28qsw_mode_get(&(pObj->Ctx), &new_val) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  new_val.odr = (Odr <=   1.0f) ? ILPS28QSW_1Hz
                : (Odr <=   4.0f) ? ILPS28QSW_4Hz
                : (Odr <=  10.0f) ? ILPS28QSW_10Hz
                : (Odr <=  25.0f) ? ILPS28QSW_25Hz
                : (Odr <=  50.0f) ? ILPS28QSW_50Hz
                : (Odr <=  75.0f) ? ILPS28QSW_75Hz
                : (Odr <= 100.0f) ? ILPS28QSW_100Hz
                :                   ILPS28QSW_200Hz;

  if (ilps28qsw_mode_set(&(pObj->Ctx), &new_val) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  if (ilps28qsw_mode_get(&(pObj->Ctx), &pObj->last_odr) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  return ILPS28QSW_OK;
}

/**
  * @brief  Set output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t ILPS28QSW_SetOutputDataRate_When_Disabled(ILPS28QSW_Object_t *pObj, float Odr)
{
  pObj->last_odr.odr = (Odr <=   1.0f) ? ILPS28QSW_1Hz
                       : (Odr <=   4.0f) ? ILPS28QSW_4Hz
                       : (Odr <=  10.0f) ? ILPS28QSW_10Hz
                       : (Odr <=  25.0f) ? ILPS28QSW_25Hz
                       : (Odr <=  50.0f) ? ILPS28QSW_50Hz
                       : (Odr <=  75.0f) ? ILPS28QSW_75Hz
                       : (Odr <= 100.0f) ? ILPS28QSW_100Hz
                       :                   ILPS28QSW_200Hz;

  return ILPS28QSW_OK;
}

/**
  * @brief  Initialize the ILPS28QSW sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t ILPS28QSW_Initialize(ILPS28QSW_Object_t *pObj)
{
  ilps28qsw_md_t md;
  ilps28qsw_bus_mode_t bus_mode;

  /* Set bdu and if_inc recommended for driver usage */
  if (ilps28qsw_init_set(&(pObj->Ctx), ILPS28QSW_DRV_RDY) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  bus_mode.filter = ILPS28QSW_AUTO;
  if (ilps28qsw_bus_mode_set(&(pObj->Ctx), &bus_mode) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  /* Set Output Data Rate in Power Down */
  md.odr = ILPS28QSW_ONE_SHOT;
  md.avg = ILPS28QSW_4_AVG;
  md.lpf = ILPS28QSW_LPF_ODR_DIV_4;

  /* Power down the device */
  if (ilps28qsw_mode_set(&(pObj->Ctx), &md) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  if (ilps28qsw_mode_get(&(pObj->Ctx), &pObj->last_odr) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  pObj->last_odr.odr = ILPS28QSW_25Hz;

  return ILPS28QSW_OK;
}

/**
  * @brief  Set the ILPS28QSW One Shot Mode
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_Set_One_Shot(ILPS28QSW_Object_t *pObj)
{
  ilps28qsw_md_t md;

  if (ilps28qsw_mode_get(&(pObj->Ctx), &md) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  /* Start One Shot Measurement */
  if (ilps28qsw_trigger_sw(&(pObj->Ctx), &md) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  return ILPS28QSW_OK;
}

/**
  * @brief  Get the ILPS28QSW One Shot Status
  * @param  pObj the device pObj
  * @param  Status pointer to the one shot status (1 means measurements available, 0 means measurements not available yet)
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_Get_One_Shot_Status(ILPS28QSW_Object_t *pObj, uint8_t *Status)
{
  uint8_t p_da;
  uint8_t t_da;

  /* Get DataReady for pressure */
  if (ILPS28QSW_PRESS_Get_DRDY_Status(pObj, &p_da) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  /* Get DataReady for temperature */
  if (ILPS28QSW_TEMP_Get_DRDY_Status(pObj, &t_da) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  if (p_da && t_da)
  {
    *Status = 1;
  }
  else
  {
    *Status = 0;
  }

  return ILPS28QSW_OK;
}

/**
  * @brief  Set the ILPS28QSW averaging selection
  * @param  pObj the device pObj
  * @param  avg averaging selection to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_Set_AVG(ILPS28QSW_Object_t *pObj, uint8_t avg)
{
  ilps28qsw_md_t md;

  if (avg > 7)
  {
    return ILPS28QSW_ERROR;
  }

  if (ilps28qsw_mode_get(&(pObj->Ctx), &md) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  switch (avg)
  {
    case 0:
    default:
      md.avg = ILPS28QSW_4_AVG;
      break;
    case 1:
      md.avg = ILPS28QSW_8_AVG;
      break;
    case 2:
      md.avg = ILPS28QSW_16_AVG;
      break;
    case 3:
      md.avg = ILPS28QSW_32_AVG;
      break;
    case 4:
      md.avg = ILPS28QSW_64_AVG;
      break;
    case 5:
      md.avg = ILPS28QSW_128_AVG;
      break;
    case 6:
      md.avg = ILPS28QSW_256_AVG;
      break;
    case 7:
      md.avg = ILPS28QSW_512_AVG;
      break;
  }

  if (ilps28qsw_mode_set(&(pObj->Ctx), &md) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  return ILPS28QSW_OK;
}

/**
  * @brief  Set the ILPS28QSW low pass filter
  * @param  pObj the device pObj
  * @param  lpf low pass filter mode to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ILPS28QSW_Set_LPF(ILPS28QSW_Object_t *pObj, uint8_t lpf)
{
  ilps28qsw_md_t md;

  if (lpf != 0 && lpf != 1 && lpf != 3)
  {
    return ILPS28QSW_ERROR;
  }

  if (ilps28qsw_mode_get(&(pObj->Ctx), &md) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  switch (lpf)
  {
    case 0:
    default:
      md.lpf = ILPS28QSW_LPF_DISABLE;
      break;
    case 1:
      md.lpf = ILPS28QSW_LPF_ODR_DIV_4;
      break;
    case 3:
      md.lpf = ILPS28QSW_LPF_ODR_DIV_9;
      break;
  }

  if (ilps28qsw_mode_set(&(pObj->Ctx), &md) != ILPS28QSW_OK)
  {
    return ILPS28QSW_ERROR;
  }

  return ILPS28QSW_OK;
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
  ILPS28QSW_Object_t *pObj = (ILPS28QSW_Object_t *)Handle;

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
  ILPS28QSW_Object_t *pObj = (ILPS28QSW_Object_t *)Handle;

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
