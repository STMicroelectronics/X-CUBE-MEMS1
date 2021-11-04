/**
 ******************************************************************************
 * @file    ism303dac.c
 * @author  MEMS Software Solutions Team
 * @brief   ISM303DAC driver file
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
#include "ism303dac.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @defgroup ISM303DAC ISM303DAC
 * @{
 */

/** @defgroup ISM303DAC_Exported_Variables ISM303DAC Exported Variables
 * @{
 */

ISM303DAC_ACC_CommonDrv_t ISM303DAC_ACC_COMMON_Driver =
{
  ISM303DAC_ACC_Init,
  ISM303DAC_ACC_DeInit,
  ISM303DAC_ACC_ReadID,
  ISM303DAC_ACC_GetCapabilities,
};

ISM303DAC_ACC_Drv_t ISM303DAC_ACC_Driver =
{
  ISM303DAC_ACC_Enable,
  ISM303DAC_ACC_Disable,
  ISM303DAC_ACC_GetSensitivity,
  ISM303DAC_ACC_GetOutputDataRate,
  ISM303DAC_ACC_SetOutputDataRate,
  ISM303DAC_ACC_GetFullScale,
  ISM303DAC_ACC_SetFullScale,
  ISM303DAC_ACC_GetAxes,
  ISM303DAC_ACC_GetAxesRaw,
};

ISM303DAC_MAG_CommonDrv_t ISM303DAC_MAG_COMMON_Driver =
{
  ISM303DAC_MAG_Init,
  ISM303DAC_MAG_DeInit,
  ISM303DAC_MAG_ReadID,
  ISM303DAC_MAG_GetCapabilities,
};

ISM303DAC_MAG_Drv_t ISM303DAC_MAG_Driver =
{
  ISM303DAC_MAG_Enable,
  ISM303DAC_MAG_Disable,
  ISM303DAC_MAG_GetSensitivity,
  ISM303DAC_MAG_GetOutputDataRate,
  ISM303DAC_MAG_SetOutputDataRate,
  ISM303DAC_MAG_GetFullScale,
  ISM303DAC_MAG_SetFullScale,
  ISM303DAC_MAG_GetAxes,
  ISM303DAC_MAG_GetAxesRaw,
};

/**
 * @}
 */

/** @defgroup ISM303DAC_Private_Function_Prototypes ISM303DAC Private Function Prototypes
 * @{
 */

static int32_t ReadAccRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteAccRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t ReadMagRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteMagRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t ISM303DAC_ACC_SetOutputDataRate_When_Enabled(ISM303DAC_ACC_Object_t *pObj, float Odr);
static int32_t ISM303DAC_ACC_SetOutputDataRate_When_Disabled(ISM303DAC_ACC_Object_t *pObj, float Odr);

/**
 * @}
 */

/** @defgroup ISM303DAC_Exported_Functions ISM303DAC Exported Functions
 * @{
 */

/**
 * @brief  Register Component Bus IO operations
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_ACC_RegisterBusIO(ISM303DAC_ACC_Object_t *pObj, ISM303DAC_IO_t *pIO)
{
  int32_t ret = ISM303DAC_OK;

  if (pObj == NULL)
  {
    ret = ISM303DAC_ERROR;
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
      ret = ISM303DAC_ERROR;
    }
    else if (pObj->IO.Init() != ISM303DAC_OK)
    {
      ret = ISM303DAC_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == ISM303DAC_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          ism303dac_xl_sim_t data = ISM303DAC_XL_SPI_3_WIRE;

          if (ism303dac_xl_spi_mode_set(&(pObj->Ctx), data) != ISM303DAC_OK)
          {
            return ISM303DAC_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
 * @brief  Initialize the ISM303DAC sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_ACC_Init(ISM303DAC_ACC_Object_t *pObj)
{
  /* Enable BDU */
  if (ism303dac_xl_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  /* FIFO mode selection */
  if (ism303dac_xl_fifo_mode_set(&(pObj->Ctx), ISM303DAC_XL_BYPASS_MODE) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = ISM303DAC_XL_ODR_100Hz_LP;

  /* Output data rate selection - power down. */
  if (ism303dac_xl_data_rate_set(&(pObj->Ctx), ISM303DAC_XL_ODR_OFF) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  /* Full scale selection. */
  if (ism303dac_xl_full_scale_set(&(pObj->Ctx), ISM303DAC_XL_2g) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  pObj->is_initialized = 1;

  return ISM303DAC_OK;
}

/**
 * @brief  Deinitialize the ISM303DAC accelerometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_ACC_DeInit(ISM303DAC_ACC_Object_t *pObj)
{
  /* Disable the component */
  if (ISM303DAC_ACC_Disable(pObj) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  /* Reset output data rate. */
  pObj->acc_odr = ISM303DAC_XL_ODR_OFF;
  pObj->is_initialized = 0;

  return ISM303DAC_OK;
}

/**
 * @brief  Read component ID
 * @param  pObj the device pObj
 * @param  Id the WHO_AM_I value
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_ACC_ReadID(ISM303DAC_ACC_Object_t *pObj, uint8_t *Id)
{
  if (ism303dac_xl_device_id_get(&(pObj->Ctx), Id) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  return ISM303DAC_OK;
}

/**
 * @brief  Get ISM303DAC accelerometer sensor capabilities
 * @param  pObj Component object pointer
 * @param  Capabilities pointer to ISM303DAC accelerometer sensor capabilities
 * @retval Component status
 */
int32_t ISM303DAC_ACC_GetCapabilities(ISM303DAC_ACC_Object_t *pObj, ISM303DAC_Capabilities_t *Capabilities)
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
  return ISM303DAC_OK;
}

/**
 * @brief  Enable the ISM303DAC accelerometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_ACC_Enable(ISM303DAC_ACC_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return ISM303DAC_OK;
  }

  /* Output data rate selection. */
  if (ism303dac_xl_data_rate_set(&(pObj->Ctx), pObj->acc_odr) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return ISM303DAC_OK;
}

/**
 * @brief  Disable the ISM303DAC accelerometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_ACC_Disable(ISM303DAC_ACC_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return ISM303DAC_OK;
  }

  /* Get current output data rate. */
  if (ism303dac_xl_data_rate_get(&(pObj->Ctx), &pObj->acc_odr) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  /* Output data rate selection - power down. */
  if (ism303dac_xl_data_rate_set(&(pObj->Ctx), ISM303DAC_XL_ODR_OFF) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return ISM303DAC_OK;
}

/**
 * @brief  Get the ISM303DAC accelerometer sensor sensitivity
 * @param  pObj the device pObj
 * @param  Sensitivity pointer
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_ACC_GetSensitivity(ISM303DAC_ACC_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = ISM303DAC_OK;
  ism303dac_xl_fs_t fullscale;

  /* Read actual full scale selection from sensor. */
  if (ism303dac_xl_full_scale_get(&(pObj->Ctx), &fullscale) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  /* Store the sensitivity based on actual full scale. */
  switch (fullscale)
  {
    case ISM303DAC_XL_2g:
      *Sensitivity = (float)ISM303DAC_ACC_SENSITIVITY_FS_2G;
      break;

    case ISM303DAC_XL_4g:
      *Sensitivity = (float)ISM303DAC_ACC_SENSITIVITY_FS_4G;
      break;

    case ISM303DAC_XL_8g:
      *Sensitivity = (float)ISM303DAC_ACC_SENSITIVITY_FS_8G;
      break;

    case ISM303DAC_XL_16g:
      *Sensitivity = (float)ISM303DAC_ACC_SENSITIVITY_FS_16G;
      break;

    default:
      ret = ISM303DAC_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Get the ISM303DAC accelerometer sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr pointer where the output data rate is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_ACC_GetOutputDataRate(ISM303DAC_ACC_Object_t *pObj, float *Odr)
{
  int32_t ret = ISM303DAC_OK;
  ism303dac_xl_odr_t odr_low_level;

  /* Get current output data rate. */
  if (ism303dac_xl_data_rate_get(&(pObj->Ctx), &odr_low_level) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  switch (odr_low_level)
  {
    case ISM303DAC_XL_ODR_OFF:
      *Odr = 0.0f;
      break;

    case ISM303DAC_XL_ODR_1Hz_LP:
      *Odr = 1.0f;
      break;

    case ISM303DAC_XL_ODR_12Hz5_LP:
    case ISM303DAC_XL_ODR_12Hz5_HR:
      *Odr = 12.5f;
      break;

    case ISM303DAC_XL_ODR_25Hz_LP:
    case ISM303DAC_XL_ODR_25Hz_HR:
      *Odr = 25.0f;
      break;

    case ISM303DAC_XL_ODR_50Hz_LP:
    case ISM303DAC_XL_ODR_50Hz_HR:
      *Odr = 50.0f;
      break;

    case ISM303DAC_XL_ODR_100Hz_LP:
    case ISM303DAC_XL_ODR_100Hz_HR:
      *Odr = 100.0f;
      break;

    case ISM303DAC_XL_ODR_200Hz_LP:
    case ISM303DAC_XL_ODR_200Hz_HR:
      *Odr = 200.0f;
      break;

    case ISM303DAC_XL_ODR_400Hz_LP:
    case ISM303DAC_XL_ODR_400Hz_HR:
      *Odr = 400.0f;
      break;

    case ISM303DAC_XL_ODR_800Hz_LP:
    case ISM303DAC_XL_ODR_800Hz_HR:
      *Odr = 800.0f;
      break;

    case ISM303DAC_XL_ODR_1k6Hz_HF:
      *Odr = 1600.0f;
      break;

    case ISM303DAC_XL_ODR_3k2Hz_HF:
      *Odr = 3200.0f;
      break;

    case ISM303DAC_XL_ODR_6k4Hz_HF:
      *Odr = 6400.0f;
      break;

    default:
      ret = ISM303DAC_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the ISM303DAC accelerometer sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_ACC_SetOutputDataRate(ISM303DAC_ACC_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return ISM303DAC_ACC_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return ISM303DAC_ACC_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
 * @brief  Get the ISM303DAC accelerometer sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale pointer where the full scale is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_ACC_GetFullScale(ISM303DAC_ACC_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = ISM303DAC_OK;
  ism303dac_xl_fs_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (ism303dac_xl_full_scale_get(&(pObj->Ctx), &fs_low_level) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  switch (fs_low_level)
  {
    case ISM303DAC_XL_2g:
      *FullScale =  2;
      break;

    case ISM303DAC_XL_4g:
      *FullScale =  4;
      break;

    case ISM303DAC_XL_8g:
      *FullScale =  8;
      break;

    case ISM303DAC_XL_16g:
      *FullScale = 16;
      break;

    default:
      ret = ISM303DAC_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the ISM303DAC accelerometer sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale the functional full scale to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_ACC_SetFullScale(ISM303DAC_ACC_Object_t *pObj, int32_t FullScale)
{
  ism303dac_xl_fs_t new_fs;

  new_fs = (FullScale <= 2) ? ISM303DAC_XL_2g
           : (FullScale <= 4) ? ISM303DAC_XL_4g
           : (FullScale <= 8) ? ISM303DAC_XL_8g
           :                    ISM303DAC_XL_16g;

  if (ism303dac_xl_full_scale_set(&(pObj->Ctx), new_fs) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  return ISM303DAC_OK;
}

/**
 * @brief  Get the ISM303DAC accelerometer sensor raw axes
 * @param  pObj the device pObj
 * @param  Value pointer where the raw values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_ACC_GetAxesRaw(ISM303DAC_ACC_Object_t *pObj, ISM303DAC_AxesRaw_t *Value)
{
  ism303dac_axis3bit16_t data_raw;
  int32_t ret = ISM303DAC_OK;

  /* Read raw data values. */
  if (ism303dac_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  /* Format the data. */
  Value->x = (data_raw.i16bit[0]);
  Value->y = (data_raw.i16bit[1]);
  Value->z = (data_raw.i16bit[2]);

  return ret;
}

/**
 * @brief  Get the ISM303DAC accelerometer sensor axes
 * @param  pObj the device pObj
 * @param  Acceleration pointer where the values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_ACC_GetAxes(ISM303DAC_ACC_Object_t *pObj, ISM303DAC_Axes_t *Acceleration)
{
  ISM303DAC_AxesRaw_t data_raw;
  float sensitivity = 0.0f;

  /* Read raw data values. */
  if (ISM303DAC_ACC_GetAxesRaw(pObj, &data_raw) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  /* Get ISM303DAC actual sensitivity. */
  if (ISM303DAC_ACC_GetSensitivity(pObj, &sensitivity) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  /* Calculate the data. */
  Acceleration->x = (int32_t)((float)((float)data_raw.x * sensitivity));
  Acceleration->y = (int32_t)((float)((float)data_raw.y * sensitivity));
  Acceleration->z = (int32_t)((float)((float)data_raw.z * sensitivity));

  return ISM303DAC_OK;
}

/**
 * @brief  Get the ISM303DAC register value for accelerometer sensor
 * @param  pObj the device pObj
 * @param  Reg address to be read
 * @param  Data pointer where the value is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_ACC_Read_Reg(ISM303DAC_ACC_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (ism303dac_read_reg(&(pObj->Ctx), Reg, Data, 1) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  return ISM303DAC_OK;
}

/**
 * @brief  Set the ISM303DAC register value for accelerometer sensor
 * @param  pObj the device pObj
 * @param  Reg address to be written
 * @param  Data value to be written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_ACC_Write_Reg(ISM303DAC_ACC_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (ism303dac_write_reg(&(pObj->Ctx), Reg, &Data, 1) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  return ISM303DAC_OK;
}

/**
 * @brief  Get the ISM303DAC ACC data ready bit value
 * @param  pObj the device pObj
 * @param  Status the status of data ready bit
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_ACC_Get_DRDY_Status(ISM303DAC_ACC_Object_t *pObj, uint8_t *Status)
{
  if (ism303dac_xl_flag_data_ready_get(&(pObj->Ctx), Status) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  return ISM303DAC_OK;
}

/**
 * @brief  Get the ISM303DAC ACC initialization status
 * @param  pObj the device pObj
 * @param  Status 1 if initialized, 0 otherwise
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_ACC_Get_Init_Status(ISM303DAC_ACC_Object_t *pObj, uint8_t *Status)
{
  if (pObj == NULL)
  {
    return ISM303DAC_ERROR;
  }

  *Status = pObj->is_initialized;

  return ISM303DAC_OK;
}

/**
 * @brief  Register Component Bus IO operations
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_MAG_RegisterBusIO(ISM303DAC_MAG_Object_t *pObj, ISM303DAC_IO_t *pIO)
{
  int32_t ret = ISM303DAC_OK;

  if (pObj == NULL)
  {
    ret = ISM303DAC_ERROR;
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

    pObj->Ctx.read_reg  = ReadMagRegWrap;
    pObj->Ctx.write_reg = WriteMagRegWrap;
    pObj->Ctx.handle    = pObj;

    if (pObj->IO.Init == NULL)
    {
      ret = ISM303DAC_ERROR;
    }
    else if (pObj->IO.Init() != ISM303DAC_OK)
    {
      ret = ISM303DAC_ERROR;
    }
    else
    {
      if (pObj->IO.BusType != ISM303DAC_I2C_BUS) /* If the bus type is not I2C */
      {
        /* Disable I2C interface support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Disable I2C interface on the component */
          if (ism303dac_mg_i2c_interface_set(&(pObj->Ctx), ISM303DAC_MG_I2C_DISABLE) != ISM303DAC_OK)
          {
            return ISM303DAC_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
 * @brief  Initialize the ISM303DAC sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_MAG_Init(ISM303DAC_MAG_Object_t *pObj)
{
  /* Enable BDU */
  if (ism303dac_mg_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  /* Operating mode selection - power down */
  if (ism303dac_mg_operating_mode_set(&(pObj->Ctx), ISM303DAC_MG_POWER_DOWN) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  /* Output data rate selection */
  if (ism303dac_mg_data_rate_set(&(pObj->Ctx), ISM303DAC_MG_ODR_100Hz) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  /* Self Test disabled. */
  if (ism303dac_mg_self_test_set(&(pObj->Ctx), PROPERTY_DISABLE) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  pObj->is_initialized = 1;

  return ISM303DAC_OK;
}

/**
 * @brief  Deinitialize the ISM303DAC magnetometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_MAG_DeInit(ISM303DAC_MAG_Object_t *pObj)
{
  /* Disable the component */
  if (ISM303DAC_MAG_Disable(pObj) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  pObj->is_initialized = 0;

  return ISM303DAC_OK;
}

/**
 * @brief  Read component ID
 * @param  pObj the device pObj
 * @param  Id the WHO_AM_I value
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_MAG_ReadID(ISM303DAC_MAG_Object_t *pObj, uint8_t *Id)
{
  if (ism303dac_mg_device_id_get(&(pObj->Ctx), Id) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  return ISM303DAC_OK;
}

/**
 * @brief  Get ISM303DAC magnetometer sensor capabilities
 * @param  pObj Component object pointer
 * @param  Capabilities pointer to ISM303DAC magnetometer sensor capabilities
 * @retval Component status
 */
int32_t ISM303DAC_MAG_GetCapabilities(ISM303DAC_MAG_Object_t *pObj, ISM303DAC_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Acc          = 0;
  Capabilities->Gyro         = 0;
  Capabilities->Magneto      = 1;
  Capabilities->LowPower     = 0;
  Capabilities->GyroMaxFS    = 0;
  Capabilities->AccMaxFS     = 0;
  Capabilities->MagMaxFS     = 50;
  Capabilities->GyroMaxOdr   = 0.0f;
  Capabilities->AccMaxOdr    = 0.0f;
  Capabilities->MagMaxOdr    = 100.0f;
  return ISM303DAC_OK;
}

/**
 * @brief Enable the ISM303DAC magnetometer sensor
 * @param pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_MAG_Enable(ISM303DAC_MAG_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->mag_is_enabled == 1U)
  {
    return ISM303DAC_OK;
  }

  /* Output data rate selection. */
  if (ism303dac_mg_operating_mode_set(&(pObj->Ctx), ISM303DAC_MG_CONTINUOUS_MODE) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  pObj->mag_is_enabled = 1;

  return ISM303DAC_OK;
}

/**
 * @brief Disable the ISM303DAC magnetometer sensor
 * @param pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_MAG_Disable(ISM303DAC_MAG_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->mag_is_enabled == 0U)
  {
    return ISM303DAC_OK;
  }

  /* Output data rate selection - power down. */
  if (ism303dac_mg_operating_mode_set(&(pObj->Ctx), ISM303DAC_MG_POWER_DOWN) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  pObj->mag_is_enabled = 0;

  return ISM303DAC_OK;
}

/**
 * @brief  Get the ISM303DAC magnetometer sensor sensitivity
 * @param  pObj the device pObj
 * @param  Sensitivity pointer
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_MAG_GetSensitivity(ISM303DAC_MAG_Object_t *pObj, float *Sensitivity)
{
  *Sensitivity = ISM303DAC_MAG_SENSITIVITY_FS_50GAUSS;

  return ISM303DAC_OK;
}

/**
 * @brief  Get the ISM303DAC magnetometer sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr pointer where the output data rate is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_MAG_GetOutputDataRate(ISM303DAC_MAG_Object_t *pObj, float *Odr)
{
  int32_t ret = ISM303DAC_OK;
  ism303dac_mg_odr_t odr_low_level;

  /* Get current output data rate. */
  if (ism303dac_mg_data_rate_get(&(pObj->Ctx), &odr_low_level) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  switch (odr_low_level)
  {
    case ISM303DAC_MG_ODR_10Hz:
      *Odr = 10.0f;
      break;

    case ISM303DAC_MG_ODR_20Hz:
      *Odr = 20.0f;
      break;

    case ISM303DAC_MG_ODR_50Hz:
      *Odr = 50.0f;
      break;

    case ISM303DAC_MG_ODR_100Hz:
      *Odr = 100.0f;
      break;

    default:
      ret = ISM303DAC_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the ISM303DAC magnetometer sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_MAG_SetOutputDataRate(ISM303DAC_MAG_Object_t *pObj, float Odr)
{
  ism303dac_mg_odr_t new_odr;

  new_odr = (Odr <= 10.000f) ? ISM303DAC_MG_ODR_10Hz
            : (Odr <= 20.000f) ? ISM303DAC_MG_ODR_20Hz
            : (Odr <= 50.000f) ? ISM303DAC_MG_ODR_50Hz
            :                    ISM303DAC_MG_ODR_100Hz;

  if (ism303dac_mg_data_rate_set(&(pObj->Ctx), new_odr) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  return ISM303DAC_OK;
}


/**
 * @brief  Get the ISM303DAC magnetometer sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale pointer where the full scale is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_MAG_GetFullScale(ISM303DAC_MAG_Object_t *pObj, int32_t *FullScale)
{
  *FullScale = 50;

  return ISM303DAC_OK;
}

/**
 * @brief  Set the ISM303DAC magnetometer sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale the functional full scale to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_MAG_SetFullScale(ISM303DAC_MAG_Object_t *pObj, int32_t FullScale)
{
  return ISM303DAC_OK;
}

/**
 * @brief  Get the ISM303DAC magnetometer sensor raw axes
 * @param  pObj the device pObj
 * @param  Value pointer where the raw values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_MAG_GetAxesRaw(ISM303DAC_MAG_Object_t *pObj, ISM303DAC_AxesRaw_t *Value)
{
  ism303dac_axis3bit16_t data_raw;

  /* Read raw data values. */
  if (ism303dac_magnetic_raw_get(&(pObj->Ctx), data_raw.i16bit) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return ISM303DAC_OK;
}

/**
 * @brief  Get the ISM303DAC magnetometer sensor axes
 * @param  pObj the device pObj
 * @param  MagneticField pointer where the values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_MAG_GetAxes(ISM303DAC_MAG_Object_t *pObj, ISM303DAC_Axes_t *MagneticField)
{
  ism303dac_axis3bit16_t data_raw;
  float sensitivity;

  /* Read raw data values. */
  if (ism303dac_magnetic_raw_get(&(pObj->Ctx), data_raw.i16bit) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  /* Get ISM303DAC actual sensitivity. */
  (void)ISM303DAC_MAG_GetSensitivity(pObj, &sensitivity);

  /* Calculate the data. */
  MagneticField->x = (int32_t)((float)((float)data_raw.i16bit[0] * sensitivity));
  MagneticField->y = (int32_t)((float)((float)data_raw.i16bit[1] * sensitivity));
  MagneticField->z = (int32_t)((float)((float)data_raw.i16bit[2] * sensitivity));

  return ISM303DAC_OK;
}

/**
 * @brief  Get the ISM303DAC register value for magnetic sensor
 * @param  pObj the device pObj
 * @param  Reg address to be read
 * @param  Data pointer where the value is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_MAG_Read_Reg(ISM303DAC_MAG_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (ism303dac_read_reg(&(pObj->Ctx), Reg, Data, 1) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  return ISM303DAC_OK;
}

/**
 * @brief  Set the ISM303DAC register value for magnetic sensor
 * @param  pObj the device pObj
 * @param  Reg address to be written
 * @param  Data value to be written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_MAG_Write_Reg(ISM303DAC_MAG_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (ism303dac_write_reg(&(pObj->Ctx), Reg, &Data, 1) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  return ISM303DAC_OK;
}

/**
 * @brief  Get the ISM303DAC MAG data ready bit value
 * @param  pObj the device pObj
 * @param  Status the status of data ready bit
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_MAG_Get_DRDY_Status(ISM303DAC_MAG_Object_t *pObj, uint8_t *Status)
{
  if (ism303dac_mg_data_ready_get(&(pObj->Ctx), Status) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  return ISM303DAC_OK;
}

/**
 * @brief  Get the ISM303DAC MAG initialization status
 * @param  pObj the device pObj
 * @param  Status 1 if initialized, 0 otherwise
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM303DAC_MAG_Get_Init_Status(ISM303DAC_MAG_Object_t *pObj, uint8_t *Status)
{
  if (pObj == NULL)
  {
    return ISM303DAC_ERROR;
  }

  *Status = pObj->is_initialized;

  return ISM303DAC_OK;
}

/**
 * @}
 */

/** @defgroup ISM303DAC_Private_Functions ISM303DAC Private Functions
 * @{
 */

/**
 * @brief  Set the ISM303DAC accelerometer sensor output data rate when enabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t ISM303DAC_ACC_SetOutputDataRate_When_Enabled(ISM303DAC_ACC_Object_t *pObj, float Odr)
{
  ism303dac_xl_odr_t new_odr;

  new_odr = (Odr <=    1.0f) ? ISM303DAC_XL_ODR_1Hz_LP
            : (Odr <=   12.5f) ? ISM303DAC_XL_ODR_12Hz5_LP
            : (Odr <=   25.0f) ? ISM303DAC_XL_ODR_25Hz_LP
            : (Odr <=   50.0f) ? ISM303DAC_XL_ODR_50Hz_LP
            : (Odr <=  100.0f) ? ISM303DAC_XL_ODR_100Hz_LP
            : (Odr <=  200.0f) ? ISM303DAC_XL_ODR_200Hz_LP
            : (Odr <=  400.0f) ? ISM303DAC_XL_ODR_400Hz_LP
            :                    ISM303DAC_XL_ODR_800Hz_LP;

  /* Output data rate selection. */
  if (ism303dac_xl_data_rate_set(&(pObj->Ctx), new_odr) != ISM303DAC_OK)
  {
    return ISM303DAC_ERROR;
  }

  return ISM303DAC_OK;
}

/**
 * @brief  Set the ISM303DAC accelerometer sensor output data rate when disabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t ISM303DAC_ACC_SetOutputDataRate_When_Disabled(ISM303DAC_ACC_Object_t *pObj, float Odr)
{
  pObj->acc_odr = (Odr <=    1.0f) ? ISM303DAC_XL_ODR_1Hz_LP
                  : (Odr <=   12.5f) ? ISM303DAC_XL_ODR_12Hz5_LP
                  : (Odr <=   25.0f) ? ISM303DAC_XL_ODR_25Hz_LP
                  : (Odr <=   50.0f) ? ISM303DAC_XL_ODR_50Hz_LP
                  : (Odr <=  100.0f) ? ISM303DAC_XL_ODR_100Hz_LP
                  : (Odr <=  200.0f) ? ISM303DAC_XL_ODR_200Hz_LP
                  : (Odr <=  400.0f) ? ISM303DAC_XL_ODR_400Hz_LP
                  :                    ISM303DAC_XL_ODR_800Hz_LP;

  return ISM303DAC_OK;
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
  ISM303DAC_ACC_Object_t *pObj = (ISM303DAC_ACC_Object_t *)Handle;

  if (pObj->IO.BusType == ISM303DAC_I2C_BUS) /* I2C */
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
  ISM303DAC_ACC_Object_t *pObj = (ISM303DAC_ACC_Object_t *)Handle;

  if (pObj->IO.BusType == ISM303DAC_I2C_BUS) /* I2C */
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
 * @brief  Wrap Read register component function to Bus IO function
 * @param  Handle the device handler
 * @param  Reg the register address
 * @param  pData the stored data pointer
 * @param  Length the length
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t ReadMagRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length)
{
  ISM303DAC_MAG_Object_t *pObj = (ISM303DAC_MAG_Object_t *)Handle;

  if (pObj->IO.BusType == ISM303DAC_I2C_BUS) /* I2C */
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
static int32_t WriteMagRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length)
{
  ISM303DAC_MAG_Object_t *pObj = (ISM303DAC_MAG_Object_t *)Handle;

  if (pObj->IO.BusType == ISM303DAC_I2C_BUS) /* I2C */
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
