/**
  ******************************************************************************
  * @file    lis2dh12.c
  * @author  MEMS Software Solutions Team
  * @brief   LIS2DH12 driver file
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
#include "lis2dh12.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup LIS2DH12 LIS2DH12
  * @{
  */

/** @defgroup LIS2DH12_Exported_Variables LIS2DH12 Exported Variables
  * @{
  */

LIS2DH12_CommonDrv_t LIS2DH12_COMMON_Driver =
{
  LIS2DH12_Init,
  LIS2DH12_DeInit,
  LIS2DH12_ReadID,
  LIS2DH12_GetCapabilities,
};

LIS2DH12_Drv_t LIS2DH12_Driver =
{
  LIS2DH12_Enable,
  LIS2DH12_Disable,
  LIS2DH12_GetSensitivity,
  LIS2DH12_GetOutputDataRate,
  LIS2DH12_SetOutputDataRate,
  LIS2DH12_GetFullScale,
  LIS2DH12_SetFullScale,
  LIS2DH12_GetAxes,
  LIS2DH12_GetAxesRaw,
};

/**
  * @}
  */

/** @defgroup LIS2DH12_Private_Function_Prototypes LIS2DH12 Private Function Prototypes
  * @{
  */

static int32_t ReadAccRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteAccRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t LIS2DH12_GetSensitivityHR(LIS2DH12_Object_t *pObj, float *Sensitivity);
static int32_t LIS2DH12_GetSensitivityNM(LIS2DH12_Object_t *pObj, float *Sensitivity);
static int32_t LIS2DH12_GetSensitivityLP(LIS2DH12_Object_t *pObj, float *Sensitivity);
static int32_t LIS2DH12_SetOutputDataRate_When_Enabled(LIS2DH12_Object_t *pObj, float Odr);
static int32_t LIS2DH12_SetOutputDataRate_When_Disabled(LIS2DH12_Object_t *pObj, float Odr);

/**
  * @}
  */

/** @defgroup LIS2DH12_Exported_Functions LIS2DH12 Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DH12_RegisterBusIO(LIS2DH12_Object_t *pObj, LIS2DH12_IO_t *pIO)
{
  int32_t ret = LIS2DH12_OK;

  if (pObj == NULL)
  {
    ret = LIS2DH12_ERROR;
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

    pObj->Ctx.read_reg  = ReadAccRegWrap;
    pObj->Ctx.write_reg = WriteAccRegWrap;
    pObj->Ctx.handle    = pObj;

    if (pObj->IO.Init == NULL)
    {
      ret = LIS2DH12_ERROR;
    }
    else if (pObj->IO.Init() != LIS2DH12_OK)
    {
      ret = LIS2DH12_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == LIS2DH12_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          lis2dh12_sim_t data = LIS2DH12_SPI_3_WIRE;

          if (lis2dh12_spi_mode_set(&(pObj->Ctx), data) != LIS2DH12_OK)
          {
            return LIS2DH12_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  Initialize the LIS2DH12 sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DH12_Init(LIS2DH12_Object_t *pObj)
{
  /* Enable BDU */
  if (lis2dh12_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  /* FIFO mode selection */
  if (lis2dh12_fifo_mode_set(&(pObj->Ctx), LIS2DH12_BYPASS_MODE) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = LIS2DH12_ODR_100Hz;

  /* Output data rate selection - power down. */
  if (lis2dh12_data_rate_set(&(pObj->Ctx), LIS2DH12_POWER_DOWN) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  /* Full scale selection. */
  if (lis2dh12_full_scale_set(&(pObj->Ctx), LIS2DH12_2g) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  pObj->is_initialized = 1;

  return LIS2DH12_OK;
}

/**
  * @brief  Deinitialize the LIS2DH12 accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DH12_DeInit(LIS2DH12_Object_t *pObj)
{
  /* Disable the component */
  if (LIS2DH12_Disable(pObj) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  /* Reset output data rate. */
  pObj->acc_odr = LIS2DH12_POWER_DOWN;
  pObj->is_initialized = 0;

  return LIS2DH12_OK;
}

/**
  * @brief  Read component ID
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DH12_ReadID(LIS2DH12_Object_t *pObj, uint8_t *Id)
{
  if (lis2dh12_device_id_get(&(pObj->Ctx), Id) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  return LIS2DH12_OK;
}

/**
  * @brief  Get LIS2DH12 accelerometer sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to LIS2DH12 accelerometer sensor capabilities
  * @retval Component status
  */
int32_t LIS2DH12_GetCapabilities(LIS2DH12_Object_t *pObj, LIS2DH12_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Acc          = 1;
  Capabilities->Gyro         = 0;
  Capabilities->Magneto      = 0;
  Capabilities->LowPower     = 0;
  Capabilities->GyroMaxFS    = 0;
  Capabilities->AccMaxFS     = 16;
  Capabilities->MagMaxFS     = 0;
  Capabilities->GyroMaxOdr   = 0.0f;
  Capabilities->AccMaxOdr    = 400.0f;
  Capabilities->MagMaxOdr    = 0.0f;
  return LIS2DH12_OK;
}

/**
  * @brief  Enable the LIS2DH12 accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DH12_Enable(LIS2DH12_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return LIS2DH12_OK;
  }

  /* Output data rate selection. */
  if (lis2dh12_data_rate_set(&(pObj->Ctx), pObj->acc_odr) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return LIS2DH12_OK;
}

/**
  * @brief  Disable the LIS2DH12 accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DH12_Disable(LIS2DH12_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return LIS2DH12_OK;
  }

  /* Get current output data rate. */
  if (lis2dh12_data_rate_get(&(pObj->Ctx), &pObj->acc_odr) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  /* Output data rate selection - power down. */
  if (lis2dh12_data_rate_set(&(pObj->Ctx), LIS2DH12_POWER_DOWN) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return LIS2DH12_OK;
}

/**
  * @brief  Get the LIS2DH12 accelerometer sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DH12_GetSensitivity(LIS2DH12_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = LIS2DH12_OK;
  lis2dh12_op_md_t op_mode;

  /* Read operative mode from sensor. */
  if (lis2dh12_operating_mode_get(&(pObj->Ctx), &op_mode) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  /* Store the Sensitivity based on actual full scale. */
  switch (op_mode)
  {
    case LIS2DH12_HR_12bit:
      if (LIS2DH12_GetSensitivityHR(pObj, Sensitivity) != LIS2DH12_OK)
      {
        return LIS2DH12_ERROR;
      }
      break;

    case LIS2DH12_NM_10bit:
      if (LIS2DH12_GetSensitivityNM(pObj, Sensitivity) != LIS2DH12_OK)
      {
        return LIS2DH12_ERROR;
      }
      break;

    case LIS2DH12_LP_8bit:
      if (LIS2DH12_GetSensitivityLP(pObj, Sensitivity) != LIS2DH12_OK)
      {
        return LIS2DH12_ERROR;
      }
      break;

    default:
      ret = LIS2DH12_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the LIS2DH12 accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DH12_GetOutputDataRate(LIS2DH12_Object_t *pObj, float *Odr)
{
  int32_t ret = LIS2DH12_OK;
  lis2dh12_op_md_t op_mode;
  lis2dh12_odr_t odr_low_level;

  /* Read operative mode from sensor. */
  if (lis2dh12_operating_mode_get(&(pObj->Ctx), &op_mode) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  /* Get current output data rate. */
  if (lis2dh12_data_rate_get(&(pObj->Ctx), &odr_low_level) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  if (op_mode == LIS2DH12_LP_8bit) /* LP mode */
  {
    switch (odr_low_level)
    {
      case LIS2DH12_POWER_DOWN:
        *Odr = 0.0f;
        break;

      case LIS2DH12_ODR_1Hz:
        *Odr = 1.0f;
        break;

      case LIS2DH12_ODR_10Hz:
        *Odr = 10.0f;
        break;

      case LIS2DH12_ODR_25Hz:
        *Odr = 25.0f;
        break;

      case LIS2DH12_ODR_50Hz:
        *Odr = 50.0f;
        break;

      case LIS2DH12_ODR_100Hz:
        *Odr = 100.0f;
        break;

      case LIS2DH12_ODR_200Hz:
        *Odr = 200.0f;
        break;

      case LIS2DH12_ODR_400Hz:
        *Odr = 400.0f;
        break;

      case LIS2DH12_ODR_1kHz620_LP:
        *Odr = 1620.0f;
        break;

      case LIS2DH12_ODR_5kHz376_LP_1kHz344_NM_HP:
        *Odr = 5376.0f;
        break;

      default:
        ret = LIS2DH12_ERROR;
        break;
    }
  }
  else /* HR and NM modes */
  {
    switch (odr_low_level)
    {
      case LIS2DH12_POWER_DOWN:
        *Odr = 0.0f;
        break;

      case LIS2DH12_ODR_1Hz:
        *Odr = 1.0f;
        break;

      case LIS2DH12_ODR_10Hz:
        *Odr = 10.0f;
        break;

      case LIS2DH12_ODR_25Hz:
        *Odr = 25.0f;
        break;

      case LIS2DH12_ODR_50Hz:
        *Odr = 50.0f;
        break;

      case LIS2DH12_ODR_100Hz:
        *Odr = 100.0f;
        break;

      case LIS2DH12_ODR_200Hz:
        *Odr = 200.0f;
        break;

      case LIS2DH12_ODR_400Hz:
        *Odr = 400.0f;
        break;

      case LIS2DH12_ODR_1kHz620_LP:
        *Odr = 1620.0f;
        break;

      case LIS2DH12_ODR_5kHz376_LP_1kHz344_NM_HP:
        *Odr = 1344.0f;
        break;

      default:
        ret = LIS2DH12_ERROR;
        break;
    }
  }

  return ret;
}

/**
  * @brief  Set the LIS2DH12 accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DH12_SetOutputDataRate(LIS2DH12_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return LIS2DH12_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return LIS2DH12_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the LIS2DH12 accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DH12_GetFullScale(LIS2DH12_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = LIS2DH12_OK;
  lis2dh12_fs_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (lis2dh12_full_scale_get(&(pObj->Ctx), &fs_low_level) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  switch (fs_low_level)
  {
    case LIS2DH12_2g:
      *FullScale =  2;
      break;

    case LIS2DH12_4g:
      *FullScale =  4;
      break;

    case LIS2DH12_8g:
      *FullScale =  8;
      break;

    case LIS2DH12_16g:
      *FullScale = 16;
      break;

    default:
      ret = LIS2DH12_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the LIS2DH12 accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DH12_SetFullScale(LIS2DH12_Object_t *pObj, int32_t FullScale)
{
  lis2dh12_fs_t new_fs;

  new_fs = (FullScale <= 2) ? LIS2DH12_2g
           : (FullScale <= 4) ? LIS2DH12_4g
           : (FullScale <= 8) ? LIS2DH12_8g
           :                    LIS2DH12_16g;

  if (lis2dh12_full_scale_set(&(pObj->Ctx), new_fs) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  return LIS2DH12_OK;
}

/**
  * @brief  Get the LIS2DH12 accelerometer sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DH12_GetAxesRaw(LIS2DH12_Object_t *pObj, LIS2DH12_AxesRaw_t *Value)
{
  int16_t divisor = 1;
  lis2dh12_axis3bit16_t data_raw;
  int32_t ret = LIS2DH12_OK;
  lis2dh12_op_md_t op_mode;

  /* Read operative mode from sensor. */
  if (lis2dh12_operating_mode_get(&(pObj->Ctx), &op_mode) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  /* Store the sensitivity based on actual full scale. */
  switch (op_mode)
  {
    case LIS2DH12_HR_12bit:
      divisor = 16;
      break;

    case LIS2DH12_NM_10bit:
      divisor = 64;
      break;

    case LIS2DH12_LP_8bit:
      divisor = 256;
      break;

    default:
      ret = LIS2DH12_ERROR;
      break;
  }

  if (ret == LIS2DH12_ERROR)
  {
    return ret;
  }

  /* Read raw data values. */
  if (lis2dh12_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  /* Format the data. */
  Value->x = (data_raw.i16bit[0] / divisor);
  Value->y = (data_raw.i16bit[1] / divisor);
  Value->z = (data_raw.i16bit[2] / divisor);

  return ret;
}

/**
  * @brief  Get the LIS2DH12 accelerometer sensor axes
  * @param  pObj the device pObj
  * @param  Acceleration pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DH12_GetAxes(LIS2DH12_Object_t *pObj, LIS2DH12_Axes_t *Acceleration)
{
  LIS2DH12_AxesRaw_t data_raw;
  float sensitivity = 0.0f;

  /* Read raw data values. */
  if (LIS2DH12_GetAxesRaw(pObj, &data_raw) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  /* Get LIS2DH12 actual sensitivity. */
  if (LIS2DH12_GetSensitivity(pObj, &sensitivity) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  /* Calculate the data. */
  Acceleration->x = (int32_t)((float)((float)data_raw.x * sensitivity));
  Acceleration->y = (int32_t)((float)((float)data_raw.y * sensitivity));
  Acceleration->z = (int32_t)((float)((float)data_raw.z * sensitivity));

  return LIS2DH12_OK;
}

/**
  * @brief  Get the LIS2DH12 register value for accelerometer sensor
  * @param  pObj the device pObj
  * @param  Reg address to be read
  * @param  Data pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DH12_Read_Reg(LIS2DH12_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (lis2dh12_read_reg(&(pObj->Ctx), Reg, Data, 1) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  return LIS2DH12_OK;
}

/**
  * @brief  Set the LIS2DH12 register value for accelerometer sensor
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DH12_Write_Reg(LIS2DH12_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (lis2dh12_write_reg(&(pObj->Ctx), Reg, &Data, 1) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  return LIS2DH12_OK;
}

/**
  * @brief  Get the LIS2DH12 ACC data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DH12_Get_DRDY_Status(LIS2DH12_Object_t *pObj, uint8_t *Status)
{
  if (lis2dh12_xl_data_ready_get(&(pObj->Ctx), Status) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  return LIS2DH12_OK;
}

/**
  * @brief  Get the LIS2DH12 ACC initialization status
  * @param  pObj the device pObj
  * @param  Status 1 if initialized, 0 otherwise
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DH12_Get_Init_Status(LIS2DH12_Object_t *pObj, uint8_t *Status)
{
  if (pObj == NULL)
  {
    return LIS2DH12_ERROR;
  }

  *Status = pObj->is_initialized;

  return LIS2DH12_OK;
}

/**
  * @}
  */

/** @defgroup LIS2DH12_Private_Functions LIS2DH12 Private Functions
  * @{
  */

/**
  * @brief  Get the LIS2DH12 accelerometer sensor sensitivity for HR mode
  * @param  pObj the device pObj
  * @param  Sensitivity pointer to sensitivity
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LIS2DH12_GetSensitivityHR(LIS2DH12_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = LIS2DH12_OK;
  lis2dh12_fs_t fullscale;

  /* Read actual full scale selection from sensor. */
  if (lis2dh12_full_scale_get(&(pObj->Ctx), &fullscale) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  /* Store the sensitivity based on actual full scale. */
  switch (fullscale)
  {
    case LIS2DH12_2g:
      *Sensitivity = (float)LIS2DH12_SENSITIVITY_FS_2G_HIGH_RESOLUTION_MODE;
      break;

    case LIS2DH12_4g:
      *Sensitivity = (float)LIS2DH12_SENSITIVITY_FS_4G_HIGH_RESOLUTION_MODE;
      break;

    case LIS2DH12_8g:
      *Sensitivity = (float)LIS2DH12_SENSITIVITY_FS_8G_HIGH_RESOLUTION_MODE;
      break;

    case LIS2DH12_16g:
      *Sensitivity = (float)LIS2DH12_SENSITIVITY_FS_16G_HIGH_RESOLUTION_MODE;
      break;

    default:
      ret = LIS2DH12_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the LIS2DH12 accelerometer sensor sensitivity for NM mode
  * @param  pObj the device pObj
  * @param  Sensitivity pointer to sensitivity
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LIS2DH12_GetSensitivityNM(LIS2DH12_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = LIS2DH12_OK;
  lis2dh12_fs_t fullscale;

  /* Read actual full scale selection from sensor. */
  if (lis2dh12_full_scale_get(&(pObj->Ctx), &fullscale) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  /* Store the sensitivity based on actual full scale. */
  switch (fullscale)
  {
    case LIS2DH12_2g:
      *Sensitivity = (float)LIS2DH12_SENSITIVITY_FS_2G_NORMAL_MODE;
      break;

    case LIS2DH12_4g:
      *Sensitivity = (float)LIS2DH12_SENSITIVITY_FS_4G_NORMAL_MODE;
      break;

    case LIS2DH12_8g:
      *Sensitivity = (float)LIS2DH12_SENSITIVITY_FS_8G_NORMAL_MODE;
      break;

    case LIS2DH12_16g:
      *Sensitivity = (float)LIS2DH12_SENSITIVITY_FS_16G_NORMAL_MODE;
      break;

    default:
      ret = LIS2DH12_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the LIS2DH12 accelerometer sensor sensitivity for LP mode
  * @param  pObj the device pObj
  * @param  Sensitivity pointer to sensitivity
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LIS2DH12_GetSensitivityLP(LIS2DH12_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = LIS2DH12_OK;
  lis2dh12_fs_t fullscale;

  /* Read actual full scale selection from sensor. */
  if (lis2dh12_full_scale_get(&(pObj->Ctx), &fullscale) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  /* Store the sensitivity based on actual full scale. */
  switch (fullscale)
  {
    case LIS2DH12_2g:
      *Sensitivity = (float)LIS2DH12_SENSITIVITY_FS_2G_LOW_POWER_MODE;
      break;

    case LIS2DH12_4g:
      *Sensitivity = (float)LIS2DH12_SENSITIVITY_FS_4G_LOW_POWER_MODE;
      break;

    case LIS2DH12_8g:
      *Sensitivity = (float)LIS2DH12_SENSITIVITY_FS_8G_LOW_POWER_MODE;
      break;

    case LIS2DH12_16g:
      *Sensitivity = (float)LIS2DH12_SENSITIVITY_FS_16G_LOW_POWER_MODE;
      break;

    default:
      ret = LIS2DH12_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the LIS2DH12 accelerometer sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LIS2DH12_SetOutputDataRate_When_Enabled(LIS2DH12_Object_t *pObj, float Odr)
{
  lis2dh12_odr_t new_odr;

  new_odr = (Odr <=    1.0f) ? LIS2DH12_ODR_1Hz
            : (Odr <=   10.0f) ? LIS2DH12_ODR_10Hz
            : (Odr <=   25.0f) ? LIS2DH12_ODR_25Hz
            : (Odr <=   50.0f) ? LIS2DH12_ODR_50Hz
            : (Odr <=  100.0f) ? LIS2DH12_ODR_100Hz
            : (Odr <=  200.0f) ? LIS2DH12_ODR_200Hz
            :                    LIS2DH12_ODR_400Hz;

  /* Output data rate selection. */
  if (lis2dh12_data_rate_set(&(pObj->Ctx), new_odr) != LIS2DH12_OK)
  {
    return LIS2DH12_ERROR;
  }

  return LIS2DH12_OK;
}

/**
  * @brief  Set the LIS2DH12 accelerometer sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LIS2DH12_SetOutputDataRate_When_Disabled(LIS2DH12_Object_t *pObj, float Odr)
{
  pObj->acc_odr = (Odr <=    1.0f) ? LIS2DH12_ODR_1Hz
                  : (Odr <=   10.0f) ? LIS2DH12_ODR_10Hz
                  : (Odr <=   25.0f) ? LIS2DH12_ODR_25Hz
                  : (Odr <=   50.0f) ? LIS2DH12_ODR_50Hz
                  : (Odr <=  100.0f) ? LIS2DH12_ODR_100Hz
                  : (Odr <=  200.0f) ? LIS2DH12_ODR_200Hz
                  :                    LIS2DH12_ODR_400Hz;

  return LIS2DH12_OK;
}

/**
  * @brief  Wrap Read register component function to Bus IO function
  * @param  Handle the device handler
  * @param  Reg the register address
  * @param  pData the stored data pointer
  * @param  Length the length
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t ReadAccRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length)
{
  LIS2DH12_Object_t *pObj = (LIS2DH12_Object_t *)Handle;

  if (pObj->IO.BusType == LIS2DH12_I2C_BUS) /* I2C */
  {
    /* Enable Multi-byte read */
    return pObj->IO.ReadReg(pObj->IO.Address, (Reg | 0x80U), pData, Length);
  }
  else   /* SPI 3-Wires */
  {
    /* Enable Multi-byte read */
    return pObj->IO.ReadReg(pObj->IO.Address, (Reg | 0x40U), pData, Length);
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
static int32_t WriteAccRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length)
{
  LIS2DH12_Object_t *pObj = (LIS2DH12_Object_t *)Handle;

  if (pObj->IO.BusType == LIS2DH12_I2C_BUS) /* I2C */
  {
    /* Enable Multi-byte write */
    return pObj->IO.WriteReg(pObj->IO.Address, (Reg | 0x80U), pData, Length);
  }
  else   /* SPI 3-Wires */
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
