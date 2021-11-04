/**
 ******************************************************************************
 * @file    lis3mdl.c
 * @author  MCD Application Team
 * @brief   lis3mdl driver file
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "lis3mdl.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Component
  * @{
  */

/** @defgroup LIS3MDL LIS3MDL
  * @{
  */

/** @defgroup LIS3MDL_Exported_Variables LIS3MDL Exported Variables
  * @{
  */
LIS3MDL_CommonDrv_t LIS3MDL_COMMON_Driver =
{
  LIS3MDL_Init,
  LIS3MDL_DeInit,
  LIS3MDL_ReadID,
  LIS3MDL_GetCapabilities,
};

LIS3MDL_MAG_Drv_t LIS3MDL_MAG_Driver =
{
  LIS3MDL_MAG_Enable,
  LIS3MDL_MAG_Disable,
  LIS3MDL_MAG_GetSensitivity,
  LIS3MDL_MAG_GetOutputDataRate,
  LIS3MDL_MAG_SetOutputDataRate,
  LIS3MDL_MAG_GetFullScale,
  LIS3MDL_MAG_SetFullScale,
  LIS3MDL_MAG_GetAxes,
  LIS3MDL_MAG_GetAxesRaw,
};
/**
  * @}
  */

/** @defgroup LIS3MDL_Private_Function_Prototypes LIS3MDL Private Function Prototypes
  * @{
  */
static int32_t ReadRegWrap(void *handle, uint8_t Reg, uint8_t *Data, uint16_t Length);
static int32_t WriteRegWrap(void *handle, uint8_t Reg, uint8_t *Data, uint16_t Length);
/**
  * @}
  */

/** @defgroup LIS3MDL_Exported_Functions LIS3MDL Exported Functions
  * @{
  */
/**
  * @brief  Register Component Bus IO operations
  * @param  Component object pointer
  * @retval Component status
  */
int32_t LIS3MDL_RegisterBusIO(LIS3MDL_Object_t *pObj, LIS3MDL_IO_t *pIO)
{
  int32_t ret = LIS3MDL_OK;

  if (pObj == NULL)
  {
    ret = LIS3MDL_ERROR;
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
      ret = LIS3MDL_ERROR;
    }
    else if (pObj->IO.Init() != LIS3MDL_OK)
    {
      ret = LIS3MDL_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == LIS3MDL_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x07;

          if (LIS3MDL_Write_Reg(pObj, LIS3MDL_CTRL_REG3, data) != LIS3MDL_OK)
          {
            ret = LIS3MDL_ERROR;
          }
        }
      }
    }
  }
  return ret;
}

/**
  * @brief  Component initialization
  * @param  Component object pointer
  * @retval Component status
  */
int32_t LIS3MDL_Init(LIS3MDL_Object_t *pObj)
{
  /* Operating mode selection - power down */
  if (lis3mdl_operating_mode_set(&(pObj->Ctx), LIS3MDL_POWER_DOWN) != LIS3MDL_OK)
  {
    return LIS3MDL_ERROR;
  }

  /* Enable BDU */
  if (lis3mdl_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != LIS3MDL_OK)
  {
    return LIS3MDL_ERROR;
  }

  /* Set Output data rate. */
  if (LIS3MDL_MAG_SetOutputDataRate(pObj, 80.0f) != LIS3MDL_OK)
  {
    return LIS3MDL_ERROR;
  }

  /* Full scale selection. */
  if (LIS3MDL_MAG_SetFullScale(pObj, 4) != LIS3MDL_OK)
  {
    return LIS3MDL_ERROR;
  }

  pObj->is_initialized = 1;

  return LIS3MDL_OK;
}

/**
 * @brief Deinitialize the LIS3MDL sensor
 * @param pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LIS3MDL_DeInit(LIS3MDL_Object_t *pObj)
{
  /* Disable the component */
  if (LIS3MDL_MAG_Disable(pObj) != LIS3MDL_OK)
  {
    return LIS3MDL_ERROR;
  }

  pObj->is_initialized = 0;

  return LIS3MDL_OK;
}

/**
  * @brief  Read component ID
  * @param  pObj pointer to Component object
  * @param  Id pointer to Component ID
  * @retval Component status
  */
int32_t LIS3MDL_ReadID(LIS3MDL_Object_t *pObj, uint8_t *Id)
{
  if (lis3mdl_device_id_get(&(pObj->Ctx), Id) != LIS3MDL_OK)
  {
    return LIS3MDL_ERROR;
  }

  return LIS3MDL_OK;
}

/**
  * @brief  Get LIS3MDL sensor capabilities
  * @param  pObj pointer to Component object
  * @param  Capabilities pointer to LIS3MDL sensor capabilities
  * @retval Component status
  */
int32_t LIS3MDL_GetCapabilities(LIS3MDL_Object_t *pObj, LIS3MDL_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Acc          = 0;
  Capabilities->Gyro         = 0;
  Capabilities->Magneto      = 1;
  Capabilities->LowPower     = 0;
  Capabilities->GyroMaxFS    = 0;
  Capabilities->AccMaxFS     = 0;
  Capabilities->MagMaxFS     = 16;
  Capabilities->GyroMaxOdr   = 0.0f;
  Capabilities->AccMaxOdr    = 0.0f;
  Capabilities->MagMaxOdr    = 80.0f;
  return LIS3MDL_OK;
}

/**
 * @brief Enable the LIS3MDL accelerometer sensor
 * @param pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LIS3MDL_MAG_Enable(LIS3MDL_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->mag_is_enabled == 1U)
  {
    return LIS3MDL_OK;
  }

  /* Operating mode selection - continuous */
  if (lis3mdl_operating_mode_set(&(pObj->Ctx), LIS3MDL_CONTINUOUS_MODE) != LIS3MDL_OK)
  {
    return LIS3MDL_ERROR;
  }

  pObj->mag_is_enabled = 1;

  return LIS3MDL_OK;
}

/**
 * @brief Disable the LIS3MDL accelerometer sensor
 * @param pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LIS3MDL_MAG_Disable(LIS3MDL_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->mag_is_enabled == 0U)
  {
    return LIS3MDL_OK;
  }

  /* Operating mode selection - power down */
  if (lis3mdl_operating_mode_set(&(pObj->Ctx), LIS3MDL_POWER_DOWN) != LIS3MDL_OK)
  {
    return LIS3MDL_ERROR;
  }

  pObj->mag_is_enabled = 0;

  return LIS3MDL_OK;
}

/**
 * @brief Get the LIS3MDL accelerometer sensor sensitivity
 * @param pObj the device pObj
 * @param Sensitivity pointer to Sensitivity value
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LIS3MDL_MAG_GetSensitivity(LIS3MDL_Object_t *pObj, float *Sensitivity)
{
  lis3mdl_fs_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (lis3mdl_full_scale_get(&(pObj->Ctx), &full_scale) != LIS3MDL_OK)
  {
    return LIS3MDL_ERROR;
  }

  /* Store the sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case LIS3MDL_4_GAUSS:
      *Sensitivity = LIS3MDL_MAG_SENSITIVITY_FS_4GAUSS;
      break;
    case LIS3MDL_8_GAUSS:
      *Sensitivity = LIS3MDL_MAG_SENSITIVITY_FS_8GAUSS;
      break;
    case LIS3MDL_12_GAUSS:
      *Sensitivity = LIS3MDL_MAG_SENSITIVITY_FS_12GAUSS;
      break;
    case LIS3MDL_16_GAUSS:
      *Sensitivity = LIS3MDL_MAG_SENSITIVITY_FS_16GAUSS;
      break;
    default:
      break;
  }

  return LIS3MDL_OK;
}

/**
 * @brief Get the LIS3MDL accelerometer sensor output data rate
 * @param pObj the device pObj
 * @param Odr pointer where the output data rate is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LIS3MDL_MAG_GetOutputDataRate(LIS3MDL_Object_t *pObj, float *Odr)
{
  lis3mdl_om_t odr_low_level;

  /* Get current output data rate. */
  if (lis3mdl_data_rate_get(&(pObj->Ctx), &odr_low_level) != LIS3MDL_OK)
  {
    return LIS3MDL_ERROR;
  }

  switch (odr_low_level)
  {
    case LIS3MDL_LP_Hz625:
      *Odr = 0.625f;
      break;
    case LIS3MDL_LP_1Hz25:
    case LIS3MDL_MP_1Hz25:
    case LIS3MDL_HP_1Hz25:
    case LIS3MDL_UHP_1Hz25:
      *Odr = 1.250f;
      break;
    case LIS3MDL_LP_2Hz5:
    case LIS3MDL_MP_2Hz5:
    case LIS3MDL_HP_2Hz5:
    case LIS3MDL_UHP_2Hz5:
      *Odr = 2.500f;
      break;
    case LIS3MDL_LP_5Hz:
    case LIS3MDL_MP_5Hz:
    case LIS3MDL_HP_5Hz:
    case LIS3MDL_UHP_5Hz:
      *Odr = 5.000f;
      break;
    case LIS3MDL_LP_10Hz:
    case LIS3MDL_MP_10Hz:
    case LIS3MDL_HP_10Hz:
    case LIS3MDL_UHP_10Hz:
      *Odr = 10.000f;
      break;
    case LIS3MDL_LP_20Hz:
    case LIS3MDL_MP_20Hz:
    case LIS3MDL_HP_20Hz:
    case LIS3MDL_UHP_20Hz:
      *Odr = 20.000f;
      break;
    case LIS3MDL_LP_40Hz:
    case LIS3MDL_MP_40Hz:
    case LIS3MDL_HP_40Hz:
    case LIS3MDL_UHP_40Hz:
      *Odr = 40.000f;
      break;
    case LIS3MDL_LP_80Hz:
    case LIS3MDL_MP_80Hz:
    case LIS3MDL_HP_80Hz:
    case LIS3MDL_UHP_80Hz:
      *Odr = 80.000f;
      break;
    case LIS3MDL_UHP_155Hz:
      *Odr = 155.000f;
      break;
    case LIS3MDL_HP_300Hz:
      *Odr = 300.000f;
      break;
    case LIS3MDL_MP_560Hz:
      *Odr = 560.000f;
      break;
    case LIS3MDL_LP_1kHz:
      *Odr = 1000.000f;
      break;
    default:
      break;
  }

  return LIS3MDL_OK;
}

/**
 * @brief Set the LIS3MDL accelerometer sensor output data rate
 * @param pObj the device pObj
 * @param Odr the output data rate value to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LIS3MDL_MAG_SetOutputDataRate(LIS3MDL_Object_t *pObj, float Odr)
{
  lis3mdl_om_t new_odr;

  new_odr = (Odr <=  0.625f) ? LIS3MDL_LP_Hz625
            : (Odr <=  1.250f) ? LIS3MDL_LP_1Hz25
            : (Odr <=  2.500f) ? LIS3MDL_LP_2Hz5
            : (Odr <=  5.000f) ? LIS3MDL_LP_5Hz
            : (Odr <= 10.000f) ? LIS3MDL_LP_10Hz
            : (Odr <= 20.000f) ? LIS3MDL_LP_20Hz
            : (Odr <= 40.000f) ? LIS3MDL_LP_40Hz
            :                      LIS3MDL_LP_80Hz;

  if (lis3mdl_data_rate_set(&(pObj->Ctx), new_odr) != LIS3MDL_OK)
  {
    return LIS3MDL_ERROR;
  }

  return LIS3MDL_OK;
}

/**
 * @brief Get the LIS3MDL accelerometer sensor full scale
 * @param pObj the device pObj
 * @param FullScale pointer where the full scale is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LIS3MDL_MAG_GetFullScale(LIS3MDL_Object_t *pObj, int32_t  *FullScale)
{
  lis3mdl_fs_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (lis3mdl_full_scale_get(&(pObj->Ctx), &fs_low_level) != LIS3MDL_OK)
  {
    return LIS3MDL_ERROR;
  }

  switch (fs_low_level)
  {
    case LIS3MDL_4_GAUSS:
      *FullScale = 4;
      break;
    case LIS3MDL_8_GAUSS:
      *FullScale = 8;
      break;
    case LIS3MDL_12_GAUSS:
      *FullScale = 12;
      break;
    case LIS3MDL_16_GAUSS:
      *FullScale = 16;
      break;
    default:
      break;
  }

  return LIS3MDL_OK;
}

/**
 * @brief Set the LIS3MDL accelerometer sensor full scale
 * @param pObj the device pObj
 * @param FullScale the functional full scale to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LIS3MDL_MAG_SetFullScale(LIS3MDL_Object_t *pObj, int32_t FullScale)
{
  lis3mdl_fs_t new_fs;

  new_fs = (FullScale <= 4) ? LIS3MDL_4_GAUSS
           : (FullScale <= 8) ? LIS3MDL_8_GAUSS
           : (FullScale <= 12) ? LIS3MDL_12_GAUSS
           :                       LIS3MDL_16_GAUSS;

  if (lis3mdl_full_scale_set(&(pObj->Ctx), new_fs) != LIS3MDL_OK)
  {
    return LIS3MDL_ERROR;
  }

  return LIS3MDL_OK;
}

/**
 * @brief Get the LIS3MDL magnetic sensor raw axes
 * @param pObj the device pObj
 * @param Value pointer where the raw values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LIS3MDL_MAG_GetAxesRaw(LIS3MDL_Object_t *pObj, LIS3MDL_AxesRaw_t *Value)
{
  lis3mdl_axis3bit16_t data_raw;

  /* Read raw data values. */
  if (lis3mdl_magnetic_raw_get(&(pObj->Ctx), data_raw.i16bit) != LIS3MDL_OK)
  {
    return LIS3MDL_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return LIS3MDL_OK;
}

/**
 * @brief Get the LIS3MDL magnetic sensor axes
 * @param pObj the device pObj
 * @param MagneticField pointer where the values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LIS3MDL_MAG_GetAxes(LIS3MDL_Object_t *pObj, LIS3MDL_Axes_t *MagneticField)
{
  lis3mdl_axis3bit16_t data_raw;
  float sensitivity = 0.0f;

  /* Read raw data values. */
  if (lis3mdl_magnetic_raw_get(&(pObj->Ctx), data_raw.i16bit) != LIS3MDL_OK)
  {
    return LIS3MDL_ERROR;
  }

  /* Get LIS3MDL actual sensitivity. */
  if (LIS3MDL_MAG_GetSensitivity(pObj, &sensitivity) != LIS3MDL_OK)
  {
    return LIS3MDL_ERROR;
  }

  /* Calculate the data. */
  MagneticField->x = (int32_t)((float)((float)data_raw.i16bit[0] * sensitivity));
  MagneticField->y = (int32_t)((float)((float)data_raw.i16bit[1] * sensitivity));
  MagneticField->z = (int32_t)((float)((float)data_raw.i16bit[2] * sensitivity));

  return LIS3MDL_OK;
}

/**
 * @brief Get the LIS3MDL register value
 * @param pObj the device pObj
 * @param Reg address to be read
 * @param pData pointer where the value is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LIS3MDL_Read_Reg(LIS3MDL_Object_t *pObj, uint8_t Reg, uint8_t *pData)
{
  if (lis3mdl_read_reg(&(pObj->Ctx), Reg, pData, 1) != LIS3MDL_OK)
  {
    return LIS3MDL_ERROR;
  }

  return LIS3MDL_OK;
}

/**
 * @brief Set the LIS3MDL register value
 * @param pObj the device pObj
 * @param Reg address to be written
 * @param Data value to be written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LIS3MDL_Write_Reg(LIS3MDL_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (lis3mdl_write_reg(&(pObj->Ctx), Reg, &Data, 1) != LIS3MDL_OK)
  {
    return LIS3MDL_ERROR;
  }

  return LIS3MDL_OK;
}

/**
 * @brief  Get the LIS3MDL data ready bit value
 * @param  pObj the device pObj
 * @param  Status the status of data ready bit
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LIS3MDL_MAG_Get_DRDY_Status(LIS3MDL_Object_t *pObj, uint8_t *Status)
{
  if (lis3mdl_mag_data_ready_get(&(pObj->Ctx), Status) != LIS3MDL_OK)
  {
    return LIS3MDL_ERROR;
  }

  return LIS3MDL_OK;
}
/**
  * @}
  */

/** @defgroup LIS3MDL_Private_Function LIS3MDL Private Function
  * @{
  */

/**
  * @brief  Wrap Read register component function to Bus IO function
  * @param  Component object pointer
  * @retval Component status
  */
static int32_t ReadRegWrap(void *handle, uint8_t Reg, uint8_t *pData, uint16_t Length)
{
  LIS3MDL_Object_t *pObj = (LIS3MDL_Object_t *)handle;

  if (pObj->IO.BusType == LIS3MDL_I2C_BUS) /* I2C */
  {
    /* Enable Multi-byte read */
    return pObj->IO.ReadReg(pObj->IO.Address, (Reg | 0x80U), pData, Length);
  }
  else /* SPI 4-Wires or SPI 3-Wires */
  {
    /* Enable Multi-byte read */
    return pObj->IO.ReadReg(pObj->IO.Address, (Reg | 0x40U), pData, Length);
  }
}

/**
  * @brief  Wrap Write register component function to Bus IO function
  * @param  Component object pointer
  * @retval Component status
  */
static int32_t WriteRegWrap(void *handle, uint8_t Reg, uint8_t *pData, uint16_t Length)
{
  LIS3MDL_Object_t *pObj = (LIS3MDL_Object_t *)handle;

  if (pObj->IO.BusType == LIS3MDL_I2C_BUS) /* I2C */
  {
    /* Enable Multi-byte write */
    return pObj->IO.WriteReg(pObj->IO.Address, (Reg | 0x80U), pData, Length);
  }
  else   /* SPI 4-Wires or SPI 3-Wires */
  {
    /* Enable Multi-byte write */
    return pObj->IO.WriteReg(pObj->IO.Address, (Reg | 0x40U), pData, Length);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
