/**
 ******************************************************************************
 * @file    lsm6dsr.c
 * @author  MEMS Software Solutions Team
 * @brief   LSM6DSR driver file
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
#include "lsm6dsr.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @defgroup LSM6DSR LSM6DSR
 * @{
 */

/** @defgroup LSM6DSR_Exported_Variables LSM6DSR Exported Variables
 * @{
 */

LSM6DSR_CommonDrv_t LSM6DSR_COMMON_Driver =
{
  LSM6DSR_Init,
  LSM6DSR_DeInit,
  LSM6DSR_ReadID,
  LSM6DSR_GetCapabilities,
};

LSM6DSR_ACC_Drv_t LSM6DSR_ACC_Driver =
{
  LSM6DSR_ACC_Enable,
  LSM6DSR_ACC_Disable,
  LSM6DSR_ACC_GetSensitivity,
  LSM6DSR_ACC_GetOutputDataRate,
  LSM6DSR_ACC_SetOutputDataRate,
  LSM6DSR_ACC_GetFullScale,
  LSM6DSR_ACC_SetFullScale,
  LSM6DSR_ACC_GetAxes,
  LSM6DSR_ACC_GetAxesRaw,
};

LSM6DSR_GYRO_Drv_t LSM6DSR_GYRO_Driver =
{
  LSM6DSR_GYRO_Enable,
  LSM6DSR_GYRO_Disable,
  LSM6DSR_GYRO_GetSensitivity,
  LSM6DSR_GYRO_GetOutputDataRate,
  LSM6DSR_GYRO_SetOutputDataRate,
  LSM6DSR_GYRO_GetFullScale,
  LSM6DSR_GYRO_SetFullScale,
  LSM6DSR_GYRO_GetAxes,
  LSM6DSR_GYRO_GetAxesRaw,
};

/**
 * @}
 */

/** @defgroup LSM6DSR_Private_Function_Prototypes LSM6DSR Private Function Prototypes
 * @{
 */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t LSM6DSR_ACC_SetOutputDataRate_When_Enabled(LSM6DSR_Object_t *pObj, float Odr);
static int32_t LSM6DSR_ACC_SetOutputDataRate_When_Disabled(LSM6DSR_Object_t *pObj, float Odr);
static int32_t LSM6DSR_GYRO_SetOutputDataRate_When_Enabled(LSM6DSR_Object_t *pObj, float Odr);
static int32_t LSM6DSR_GYRO_SetOutputDataRate_When_Disabled(LSM6DSR_Object_t *pObj, float Odr);

/**
 * @}
 */

/** @defgroup LSM6DSR_Exported_Functions LSM6DSR Exported Functions
 * @{
 */

/**
 * @brief  Register Component Bus IO operations
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_RegisterBusIO(LSM6DSR_Object_t *pObj, LSM6DSR_IO_t *pIO)
{
  int32_t ret = LSM6DSR_OK;

  if (pObj == NULL)
  {
    ret = LSM6DSR_ERROR;
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
      ret = LSM6DSR_ERROR;
    }
    else if (pObj->IO.Init() != LSM6DSR_OK)
    {
      ret = LSM6DSR_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == LSM6DSR_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x0C;

          if (LSM6DSR_Write_Reg(pObj, LSM6DSR_CTRL3_C, data) != LSM6DSR_OK)
          {
            ret = LSM6DSR_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
 * @brief  Initialize the LSM6DSR sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_Init(LSM6DSR_Object_t *pObj)
{
  /* Disable I3C */
  if (lsm6dsr_i3c_disable_set(&(pObj->Ctx), LSM6DSR_I3C_DISABLE) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Enable register address automatically incremented during a multiple byte
  access with a serial interface. */
  if (lsm6dsr_auto_increment_set(&(pObj->Ctx), PROPERTY_ENABLE) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Enable BDU */
  if (lsm6dsr_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* FIFO mode selection */
  if (lsm6dsr_fifo_mode_set(&(pObj->Ctx), LSM6DSR_BYPASS_MODE) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = LSM6DSR_XL_ODR_104Hz;

  /* Output data rate selection - power down. */
  if (lsm6dsr_xl_data_rate_set(&(pObj->Ctx), LSM6DSR_XL_ODR_OFF) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Full scale selection. */
  if (lsm6dsr_xl_full_scale_set(&(pObj->Ctx), LSM6DSR_2g) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Select default output data rate. */
  pObj->gyro_odr = LSM6DSR_GY_ODR_104Hz;

  /* Output data rate selection - power down. */
  if (lsm6dsr_gy_data_rate_set(&(pObj->Ctx), LSM6DSR_GY_ODR_OFF) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Full scale selection. */
  if (lsm6dsr_gy_full_scale_set(&(pObj->Ctx), LSM6DSR_2000dps) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  pObj->is_initialized = 1;

  return LSM6DSR_OK;
}

/**
 * @brief  Deinitialize the LSM6DSR sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_DeInit(LSM6DSR_Object_t *pObj)
{
  /* Disable the component */
  if (LSM6DSR_ACC_Disable(pObj) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  if (LSM6DSR_GYRO_Disable(pObj) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Reset output data rate. */
  pObj->acc_odr = LSM6DSR_XL_ODR_OFF;
  pObj->gyro_odr = LSM6DSR_GY_ODR_OFF;

  pObj->is_initialized = 0;

  return LSM6DSR_OK;
}

/**
 * @brief  Read component ID
 * @param  pObj the device pObj
 * @param  Id the WHO_AM_I value
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ReadID(LSM6DSR_Object_t *pObj, uint8_t *Id)
{
  if (lsm6dsr_device_id_get(&(pObj->Ctx), Id) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Get LSM6DSR sensor capabilities
 * @param  pObj Component object pointer
 * @param  Capabilities pointer to LSM6DSR sensor capabilities
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_GetCapabilities(LSM6DSR_Object_t *pObj, LSM6DSR_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Acc          = 1;
  Capabilities->Gyro         = 1;
  Capabilities->Magneto      = 0;
  Capabilities->LowPower     = 0;
  Capabilities->GyroMaxFS    = 4000;
  Capabilities->AccMaxFS     = 16;
  Capabilities->MagMaxFS     = 0;
  Capabilities->GyroMaxOdr   = 6660.0f;
  Capabilities->AccMaxOdr    = 6660.0f;
  Capabilities->MagMaxOdr    = 0.0f;
  return LSM6DSR_OK;
}

/**
 * @brief  Enable the LSM6DSR accelerometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Enable(LSM6DSR_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return LSM6DSR_OK;
  }

  /* Output data rate selection. */
  if (lsm6dsr_xl_data_rate_set(&(pObj->Ctx), pObj->acc_odr) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return LSM6DSR_OK;
}

/**
 * @brief  Disable the LSM6DSR accelerometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Disable(LSM6DSR_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return LSM6DSR_OK;
  }

  /* Get current output data rate. */
  if (lsm6dsr_xl_data_rate_get(&(pObj->Ctx), &pObj->acc_odr) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Output data rate selection - power down. */
  if (lsm6dsr_xl_data_rate_set(&(pObj->Ctx), LSM6DSR_XL_ODR_OFF) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return LSM6DSR_OK;
}

/**
 * @brief  Get the LSM6DSR accelerometer sensor sensitivity
 * @param  pObj the device pObj
 * @param  Sensitivity pointer
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_GetSensitivity(LSM6DSR_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = LSM6DSR_OK;
  lsm6dsr_fs_xl_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (lsm6dsr_xl_full_scale_get(&(pObj->Ctx), &full_scale) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Store the Sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case LSM6DSR_2g:
      *Sensitivity = LSM6DSR_ACC_SENSITIVITY_FS_2G;
      break;

    case LSM6DSR_4g:
      *Sensitivity = LSM6DSR_ACC_SENSITIVITY_FS_4G;
      break;

    case LSM6DSR_8g:
      *Sensitivity = LSM6DSR_ACC_SENSITIVITY_FS_8G;
      break;

    case LSM6DSR_16g:
      *Sensitivity = LSM6DSR_ACC_SENSITIVITY_FS_16G;
      break;

    default:
      ret = LSM6DSR_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Get the LSM6DSR accelerometer sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr pointer where the output data rate is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_GetOutputDataRate(LSM6DSR_Object_t *pObj, float *Odr)
{
  int32_t ret = LSM6DSR_OK;
  lsm6dsr_odr_xl_t odr_low_level;

  /* Get current output data rate. */
  if (lsm6dsr_xl_data_rate_get(&(pObj->Ctx), &odr_low_level) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  switch (odr_low_level)
  {
    case LSM6DSR_XL_ODR_OFF:
      *Odr = 0.0f;
      break;

    case LSM6DSR_XL_ODR_12Hz5:
      *Odr = 12.5f;
      break;

    case LSM6DSR_XL_ODR_26Hz:
      *Odr = 26.0f;
      break;

    case LSM6DSR_XL_ODR_52Hz:
      *Odr = 52.0f;
      break;

    case LSM6DSR_XL_ODR_104Hz:
      *Odr = 104.0f;
      break;

    case LSM6DSR_XL_ODR_208Hz:
      *Odr = 208.0f;
      break;

    case LSM6DSR_XL_ODR_417Hz:
      *Odr = 417.0f;
      break;

    case LSM6DSR_XL_ODR_833Hz:
      *Odr = 833.0f;
      break;

    case LSM6DSR_XL_ODR_1667Hz:
      *Odr = 1667.0f;
      break;

    case LSM6DSR_XL_ODR_3333Hz:
      *Odr = 3333.0f;
      break;

    case LSM6DSR_XL_ODR_6667Hz:
      *Odr = 6667.0f;
      break;

    default:
      ret = LSM6DSR_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the LSM6DSR accelerometer sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_SetOutputDataRate(LSM6DSR_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return LSM6DSR_ACC_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return LSM6DSR_ACC_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
 * @brief  Get the LSM6DSR accelerometer sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale pointer where the full scale is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_GetFullScale(LSM6DSR_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = LSM6DSR_OK;
  lsm6dsr_fs_xl_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (lsm6dsr_xl_full_scale_get(&(pObj->Ctx), &fs_low_level) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  switch (fs_low_level)
  {
    case LSM6DSR_2g:
      *FullScale =  2;
      break;

    case LSM6DSR_4g:
      *FullScale =  4;
      break;

    case LSM6DSR_8g:
      *FullScale =  8;
      break;

    case LSM6DSR_16g:
      *FullScale = 16;
      break;

    default:
      ret = LSM6DSR_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the LSM6DSR accelerometer sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale the functional full scale to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_SetFullScale(LSM6DSR_Object_t *pObj, int32_t FullScale)
{
  lsm6dsr_fs_xl_t new_fs;

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file statical analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  new_fs = (FullScale <= 2) ? LSM6DSR_2g
           : (FullScale <= 4) ? LSM6DSR_4g
           : (FullScale <= 8) ? LSM6DSR_8g
           :                    LSM6DSR_16g;

  if (lsm6dsr_xl_full_scale_set(&(pObj->Ctx), new_fs) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Get the LSM6DSR accelerometer sensor raw axes
 * @param  pObj the device pObj
 * @param  Value pointer where the raw values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_GetAxesRaw(LSM6DSR_Object_t *pObj, LSM6DSR_AxesRaw_t *Value)
{
  lsm6dsr_axis3bit16_t data_raw;

  /* Read raw data values. */
  if (lsm6dsr_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return LSM6DSR_OK;
}

/**
 * @brief  Get the LSM6DSR accelerometer sensor axes
 * @param  pObj the device pObj
 * @param  Acceleration pointer where the values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_GetAxes(LSM6DSR_Object_t *pObj, LSM6DSR_Axes_t *Acceleration)
{
  lsm6dsr_axis3bit16_t data_raw;
  float sensitivity = 0.0f;

  /* Read raw data values. */
  if (lsm6dsr_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Get LSM6DSR actual sensitivity. */
  if (LSM6DSR_ACC_GetSensitivity(pObj, &sensitivity) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Calculate the data. */
  Acceleration->x = (int32_t)((float)((float)data_raw.i16bit[0] * sensitivity));
  Acceleration->y = (int32_t)((float)((float)data_raw.i16bit[1] * sensitivity));
  Acceleration->z = (int32_t)((float)((float)data_raw.i16bit[2] * sensitivity));

  return LSM6DSR_OK;
}

/**
 * @brief  Enable the LSM6DSR gyroscope sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_GYRO_Enable(LSM6DSR_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->gyro_is_enabled == 1U)
  {
    return LSM6DSR_OK;
  }

  /* Output data rate selection. */
  if (lsm6dsr_gy_data_rate_set(&(pObj->Ctx), pObj->gyro_odr) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  pObj->gyro_is_enabled = 1;

  return LSM6DSR_OK;
}

/**
 * @brief  Disable the LSM6DSR gyroscope sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_GYRO_Disable(LSM6DSR_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->gyro_is_enabled == 0U)
  {
    return LSM6DSR_OK;
  }

  /* Get current output data rate. */
  if (lsm6dsr_gy_data_rate_get(&(pObj->Ctx), &pObj->gyro_odr) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Output data rate selection - power down. */
  if (lsm6dsr_gy_data_rate_set(&(pObj->Ctx), LSM6DSR_GY_ODR_OFF) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  pObj->gyro_is_enabled = 0;

  return LSM6DSR_OK;
}

/**
 * @brief  Get the LSM6DSR gyroscope sensor sensitivity
 * @param  pObj the device pObj
 * @param  Sensitivity pointer
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_GYRO_GetSensitivity(LSM6DSR_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = LSM6DSR_OK;
  lsm6dsr_fs_g_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (lsm6dsr_gy_full_scale_get(&(pObj->Ctx), &full_scale) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Store the sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case LSM6DSR_125dps:
      *Sensitivity = LSM6DSR_GYRO_SENSITIVITY_FS_125DPS;
      break;

    case LSM6DSR_250dps:
      *Sensitivity = LSM6DSR_GYRO_SENSITIVITY_FS_250DPS;
      break;

    case LSM6DSR_500dps:
      *Sensitivity = LSM6DSR_GYRO_SENSITIVITY_FS_500DPS;
      break;

    case LSM6DSR_1000dps:
      *Sensitivity = LSM6DSR_GYRO_SENSITIVITY_FS_1000DPS;
      break;

    case LSM6DSR_2000dps:
      *Sensitivity = LSM6DSR_GYRO_SENSITIVITY_FS_2000DPS;
      break;

    case LSM6DSR_4000dps:
      *Sensitivity = LSM6DSR_GYRO_SENSITIVITY_FS_4000DPS;
      break;

    default:
      ret = LSM6DSR_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Get the LSM6DSR gyroscope sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr pointer where the output data rate is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_GYRO_GetOutputDataRate(LSM6DSR_Object_t *pObj, float *Odr)
{
  int32_t ret = LSM6DSR_OK;
  lsm6dsr_odr_g_t odr_low_level;

  /* Get current output data rate. */
  if (lsm6dsr_gy_data_rate_get(&(pObj->Ctx), &odr_low_level) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  switch (odr_low_level)
  {
    case LSM6DSR_GY_ODR_OFF:
      *Odr = 0.0f;
      break;

    case LSM6DSR_GY_ODR_12Hz5:
      *Odr = 12.5f;
      break;

    case LSM6DSR_GY_ODR_26Hz:
      *Odr = 26.0f;
      break;

    case LSM6DSR_GY_ODR_52Hz:
      *Odr = 52.0f;
      break;

    case LSM6DSR_GY_ODR_104Hz:
      *Odr = 104.0f;
      break;

    case LSM6DSR_GY_ODR_208Hz:
      *Odr = 208.0f;
      break;

    case LSM6DSR_GY_ODR_417Hz:
      *Odr = 417.0f;
      break;

    case LSM6DSR_GY_ODR_833Hz:
      *Odr = 833.0f;
      break;

    case LSM6DSR_GY_ODR_1667Hz:
      *Odr =  1667.0f;
      break;

    case LSM6DSR_GY_ODR_3333Hz:
      *Odr =  3333.0f;
      break;

    case LSM6DSR_GY_ODR_6667Hz:
      *Odr =  6667.0f;
      break;

    default:
      ret = LSM6DSR_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the LSM6DSR gyroscope sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_GYRO_SetOutputDataRate(LSM6DSR_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->gyro_is_enabled == 1U)
  {
    return LSM6DSR_GYRO_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return LSM6DSR_GYRO_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
 * @brief  Get the LSM6DSR gyroscope sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale pointer where the full scale is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_GYRO_GetFullScale(LSM6DSR_Object_t *pObj, int32_t  *FullScale)
{
  int32_t ret = LSM6DSR_OK;
  lsm6dsr_fs_g_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (lsm6dsr_gy_full_scale_get(&(pObj->Ctx), &fs_low_level) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  switch (fs_low_level)
  {
    case LSM6DSR_125dps:
      *FullScale =  125;
      break;

    case LSM6DSR_250dps:
      *FullScale =  250;
      break;

    case LSM6DSR_500dps:
      *FullScale =  500;
      break;

    case LSM6DSR_1000dps:
      *FullScale = 1000;
      break;

    case LSM6DSR_2000dps:
      *FullScale = 2000;
      break;

    case LSM6DSR_4000dps:
      *FullScale = 4000;
      break;

    default:
      ret = LSM6DSR_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the LSM6DSR gyroscope sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale the functional full scale to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_GYRO_SetFullScale(LSM6DSR_Object_t *pObj, int32_t FullScale)
{
  lsm6dsr_fs_g_t new_fs;

  new_fs = (FullScale <= 125)  ? LSM6DSR_125dps
           : (FullScale <= 250)  ? LSM6DSR_250dps
           : (FullScale <= 500)  ? LSM6DSR_500dps
           : (FullScale <= 1000) ? LSM6DSR_1000dps
           : (FullScale <= 2000) ? LSM6DSR_2000dps
           :                       LSM6DSR_4000dps;

  if (lsm6dsr_gy_full_scale_set(&(pObj->Ctx), new_fs) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Get the LSM6DSR gyroscope sensor raw axes
 * @param  pObj the device pObj
 * @param  Value pointer where the raw values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_GYRO_GetAxesRaw(LSM6DSR_Object_t *pObj, LSM6DSR_AxesRaw_t *Value)
{
  lsm6dsr_axis3bit16_t data_raw;

  /* Read raw data values. */
  if (lsm6dsr_angular_rate_raw_get(&(pObj->Ctx), data_raw.i16bit) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return LSM6DSR_OK;
}

/**
 * @brief  Get the LSM6DSR gyroscope sensor axes
 * @param  pObj the device pObj
 * @param  AngularRate pointer where the values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_GYRO_GetAxes(LSM6DSR_Object_t *pObj, LSM6DSR_Axes_t *AngularRate)
{
  lsm6dsr_axis3bit16_t data_raw;
  float sensitivity;

  /* Read raw data values. */
  if (lsm6dsr_angular_rate_raw_get(&(pObj->Ctx), data_raw.i16bit) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Get LSM6DSR actual sensitivity. */
  if (LSM6DSR_GYRO_GetSensitivity(pObj, &sensitivity) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Calculate the data. */
  AngularRate->x = (int32_t)((float)((float)data_raw.i16bit[0] * sensitivity));
  AngularRate->y = (int32_t)((float)((float)data_raw.i16bit[1] * sensitivity));
  AngularRate->z = (int32_t)((float)((float)data_raw.i16bit[2] * sensitivity));

  return LSM6DSR_OK;
}

/**
 * @brief  Get the LSM6DSR register value
 * @param  pObj the device pObj
 * @param  Reg address to be read
 * @param  Data pointer where the value is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_Read_Reg(LSM6DSR_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (lsm6dsr_read_reg(&(pObj->Ctx), Reg, Data, 1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Set the LSM6DSR register value
 * @param  pObj the device pObj
 * @param  Reg address to be written
 * @param  Data value to be written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_Write_Reg(LSM6DSR_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (lsm6dsr_write_reg(&(pObj->Ctx), Reg, &Data, 1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Set the interrupt latch
 * @param  pObj the device pObj
 * @param  Status value to be written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_Set_Interrupt_Latch(LSM6DSR_Object_t *pObj, uint8_t Status)
{
  if (Status > 1U)
  {
    return LSM6DSR_ERROR;
  }

  if (lsm6dsr_int_notification_set(&(pObj->Ctx), (lsm6dsr_lir_t)Status) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Enable free fall detection
 * @param  pObj the device pObj
 * @param  IntPin interrupt pin line to be used
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Enable_Free_Fall_Detection(LSM6DSR_Object_t *pObj, LSM6DSR_SensorIntPin_t IntPin)
{
  int32_t ret = LSM6DSR_OK;
  lsm6dsr_pin_int1_route_t val1;
  lsm6dsr_pin_int2_route_t val2;

  /* Output Data Rate selection */
  if (LSM6DSR_ACC_SetOutputDataRate(pObj, 416.0f) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Full scale selection */
  if (LSM6DSR_ACC_SetFullScale(pObj, 2) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* FF_DUR setting */
  if (lsm6dsr_ff_dur_set(&(pObj->Ctx), 0x06) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* WAKE_DUR setting */
  if (lsm6dsr_wkup_dur_set(&(pObj->Ctx), 0x00) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* SLEEP_DUR setting */
  if (lsm6dsr_act_sleep_dur_set(&(pObj->Ctx), 0x00) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* FF_THS setting */
  if (lsm6dsr_ff_threshold_set(&(pObj->Ctx), LSM6DSR_FF_TSH_312mg) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Enable free fall event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case LSM6DSR_INT1_PIN:
      if (lsm6dsr_pin_int1_route_get(&(pObj->Ctx), &val1) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }

      val1.md1_cfg.int1_ff = PROPERTY_ENABLE;

      if (lsm6dsr_pin_int1_route_set(&(pObj->Ctx), &val1) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }
      break;

    case LSM6DSR_INT2_PIN:
      if (lsm6dsr_pin_int2_route_get(&(pObj->Ctx), &val2) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }

      val2.md2_cfg.int2_ff = PROPERTY_ENABLE;

      if (lsm6dsr_pin_int2_route_set(&(pObj->Ctx), &val2) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }
      break;

    default:
      ret = LSM6DSR_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Disable free fall detection
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Disable_Free_Fall_Detection(LSM6DSR_Object_t *pObj)
{
  lsm6dsr_pin_int1_route_t val1;
  lsm6dsr_pin_int2_route_t val2;

  /* Disable free fall event on both INT1 and INT2 pins */
  if (lsm6dsr_pin_int1_route_get(&(pObj->Ctx), &val1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  val1.md1_cfg.int1_ff = PROPERTY_DISABLE;

  if (lsm6dsr_pin_int1_route_set(&(pObj->Ctx), &val1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  if (lsm6dsr_pin_int2_route_get(&(pObj->Ctx), &val2) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  val2.md2_cfg.int2_ff = PROPERTY_DISABLE;

  if (lsm6dsr_pin_int2_route_set(&(pObj->Ctx), &val2) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* FF_DUR setting */
  if (lsm6dsr_ff_dur_set(&(pObj->Ctx), 0x00) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* FF_THS setting */
  if (lsm6dsr_ff_threshold_set(&(pObj->Ctx), LSM6DSR_FF_TSH_156mg) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Set free fall threshold
 * @param  pObj the device pObj
 * @param  Threshold free fall detection threshold
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Set_Free_Fall_Threshold(LSM6DSR_Object_t *pObj, uint8_t Threshold)
{
  if (lsm6dsr_ff_threshold_set(&(pObj->Ctx), (lsm6dsr_ff_ths_t)Threshold) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Set free fall duration
 * @param  pObj the device pObj
 * @param  Duration free fall detection duration
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Set_Free_Fall_Duration(LSM6DSR_Object_t *pObj, uint8_t Duration)
{
  if (lsm6dsr_ff_dur_set(&(pObj->Ctx), Duration) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Enable pedometer
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Enable_Pedometer(LSM6DSR_Object_t *pObj)
{
	lsm6dsr_pin_int1_route_t val;

  /* Output Data Rate selection */
  if (LSM6DSR_ACC_SetOutputDataRate(pObj, 26.0f) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Full scale selection */
  if (LSM6DSR_ACC_SetFullScale(pObj, 2) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Enable pedometer algorithm. */
  if (lsm6dsr_pedo_sens_set(&(pObj->Ctx), LSM6DSR_PEDO_BASE) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Enable step detector on INT1 pin */
  if (lsm6dsr_pin_int1_route_get(&(pObj->Ctx), &val) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  val.emb_func_int1.int1_step_detector = PROPERTY_ENABLE;

  if (lsm6dsr_pin_int1_route_set(&(pObj->Ctx), &val) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Disable pedometer
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Disable_Pedometer(LSM6DSR_Object_t *pObj)
{
  lsm6dsr_pin_int1_route_t val1;

  /* Disable step detector on INT1 pin */
  if (lsm6dsr_pin_int1_route_get(&(pObj->Ctx), &val1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  val1.emb_func_int1.int1_step_detector = PROPERTY_DISABLE;

  if (lsm6dsr_pin_int1_route_set(&(pObj->Ctx), &val1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Disable pedometer algorithm. */
  if (lsm6dsr_pedo_sens_set(&(pObj->Ctx), PROPERTY_DISABLE) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Get step count
 * @param  pObj the device pObj
 * @param  StepCount step counter
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Get_Step_Count(LSM6DSR_Object_t *pObj, uint16_t *StepCount)
{
  if (lsm6dsr_number_of_steps_get(&(pObj->Ctx), StepCount) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Enable step counter reset
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Step_Counter_Reset(LSM6DSR_Object_t *pObj)
{
  if (lsm6dsr_steps_reset(&(pObj->Ctx)) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Enable tilt detection
 * @param  pObj the device pObj
 * @param  IntPin interrupt pin line to be used
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Enable_Tilt_Detection(LSM6DSR_Object_t *pObj, LSM6DSR_SensorIntPin_t IntPin)
{
  int32_t ret = LSM6DSR_OK;
  lsm6dsr_pin_int1_route_t val1;
  lsm6dsr_pin_int2_route_t val2;

  /* Output Data Rate selection */
  if (LSM6DSR_ACC_SetOutputDataRate(pObj, 26.0f) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Full scale selection */
  if (LSM6DSR_ACC_SetFullScale(pObj, 2) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Enable tilt calculation. */
  if (lsm6dsr_tilt_sens_set(&(pObj->Ctx), PROPERTY_ENABLE) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Enable tilt event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case LSM6DSR_INT1_PIN:
      if (lsm6dsr_pin_int1_route_get(&(pObj->Ctx), &val1) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }

      val1.emb_func_int1.int1_tilt = PROPERTY_ENABLE;

      if (lsm6dsr_pin_int1_route_set(&(pObj->Ctx), &val1) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }
      break;

    case LSM6DSR_INT2_PIN:
      if (lsm6dsr_pin_int2_route_get(&(pObj->Ctx), &val2) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }

      val2.emb_func_int2.int2_tilt = PROPERTY_ENABLE;

      if (lsm6dsr_pin_int2_route_set(&(pObj->Ctx), &val2) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }
      break;

    default:
      ret = LSM6DSR_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Disable tilt detection
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Disable_Tilt_Detection(LSM6DSR_Object_t *pObj)
{
  lsm6dsr_pin_int1_route_t val1;
  lsm6dsr_pin_int2_route_t val2;

  /* Disable tilt event on both INT1 and INT2 pins */
  if (lsm6dsr_pin_int1_route_get(&(pObj->Ctx), &val1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  val1.emb_func_int1.int1_tilt = PROPERTY_DISABLE;

  if (lsm6dsr_pin_int1_route_set(&(pObj->Ctx), &val1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  if (lsm6dsr_pin_int2_route_get(&(pObj->Ctx), &val2) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  val2.emb_func_int2.int2_tilt = PROPERTY_DISABLE;

  if (lsm6dsr_pin_int2_route_set(&(pObj->Ctx), &val2) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Disable tilt calculation. */
  if (lsm6dsr_tilt_sens_set(&(pObj->Ctx), PROPERTY_DISABLE) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Enable wake up detection
 * @param  pObj the device pObj
 * @param  IntPin interrupt pin line to be used
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Enable_Wake_Up_Detection(LSM6DSR_Object_t *pObj, LSM6DSR_SensorIntPin_t IntPin)
{
  int32_t ret = LSM6DSR_OK;
  lsm6dsr_pin_int1_route_t val1;
  lsm6dsr_pin_int2_route_t val2;

  /* Output Data Rate selection */
  if (LSM6DSR_ACC_SetOutputDataRate(pObj, 416.0f) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Full scale selection */
  if (LSM6DSR_ACC_SetFullScale(pObj, 2) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* WAKE_DUR setting */
  if (lsm6dsr_wkup_dur_set(&(pObj->Ctx), 0x00) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Set wake up threshold. */
  if (lsm6dsr_wkup_threshold_set(&(pObj->Ctx), 0x02) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Enable wake up event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case LSM6DSR_INT1_PIN:
      if (lsm6dsr_pin_int1_route_get(&(pObj->Ctx), &val1) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }

      val1.md1_cfg.int1_wu = PROPERTY_ENABLE;

      if (lsm6dsr_pin_int1_route_set(&(pObj->Ctx), &val1) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }
      break;

    case LSM6DSR_INT2_PIN:
      if (lsm6dsr_pin_int2_route_get(&(pObj->Ctx), &val2) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }

      val2.md2_cfg.int2_wu = PROPERTY_ENABLE;

      if (lsm6dsr_pin_int2_route_set(&(pObj->Ctx), &val2) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }
      break;

    default:
      ret = LSM6DSR_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Disable wake up detection
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Disable_Wake_Up_Detection(LSM6DSR_Object_t *pObj)
{
	lsm6dsr_pin_int1_route_t val1;
	lsm6dsr_pin_int2_route_t val2;

  /* Disable wake up event on both INT1 and INT2 pins */
  if (lsm6dsr_pin_int1_route_get(&(pObj->Ctx), &val1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  val1.md1_cfg.int1_wu = PROPERTY_DISABLE;

  if (lsm6dsr_pin_int1_route_set(&(pObj->Ctx), &val1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  if (lsm6dsr_pin_int2_route_get(&(pObj->Ctx), &val2) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  val2.md2_cfg.int2_wu = PROPERTY_DISABLE;

  if (lsm6dsr_pin_int2_route_set(&(pObj->Ctx), &val2) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Reset wake up threshold. */
  if (lsm6dsr_wkup_threshold_set(&(pObj->Ctx), 0x00) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* WAKE_DUR setting */
  if (lsm6dsr_wkup_dur_set(&(pObj->Ctx), 0x00) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Set wake up threshold
 * @param  pObj the device pObj
 * @param  Threshold wake up detection threshold
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Set_Wake_Up_Threshold(LSM6DSR_Object_t *pObj, uint8_t Threshold)
{
  /* Set wake up threshold. */
  if (lsm6dsr_wkup_threshold_set(&(pObj->Ctx), Threshold) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Set wake up duration
 * @param  pObj the device pObj
 * @param  Duration wake up detection duration
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Set_Wake_Up_Duration(LSM6DSR_Object_t *pObj, uint8_t Duration)
{
  /* Set wake up duration. */
  if (lsm6dsr_wkup_dur_set(&(pObj->Ctx), Duration) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Enable single tap detection
 * @param  pObj the device pObj
 * @param  IntPin interrupt pin line to be used
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Enable_Single_Tap_Detection(LSM6DSR_Object_t *pObj, LSM6DSR_SensorIntPin_t IntPin)
{
  int32_t ret = LSM6DSR_OK;
  lsm6dsr_pin_int1_route_t val1;
  lsm6dsr_pin_int2_route_t val2;

  /* Output Data Rate selection */
  if (LSM6DSR_ACC_SetOutputDataRate(pObj, 416.0f) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Full scale selection */
  if (LSM6DSR_ACC_SetFullScale(pObj, 2) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Enable X direction in tap recognition. */
  if (lsm6dsr_tap_detection_on_x_set(&(pObj->Ctx), PROPERTY_ENABLE) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Enable Y direction in tap recognition. */
  if (lsm6dsr_tap_detection_on_y_set(&(pObj->Ctx), PROPERTY_ENABLE) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Enable Z direction in tap recognition. */
  if (lsm6dsr_tap_detection_on_z_set(&(pObj->Ctx), PROPERTY_ENABLE) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Set tap threshold. */
  if (lsm6dsr_tap_threshold_x_set(&(pObj->Ctx), 0x08) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Set tap shock time window. */
  if (lsm6dsr_tap_shock_set(&(pObj->Ctx), 0x02) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Set tap quiet time window. */
  if (lsm6dsr_tap_quiet_set(&(pObj->Ctx), 0x01) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* _NOTE_: Tap duration time window - don't care for single tap. */

  /* _NOTE_: Single/Double Tap event - don't care of this flag for single tap. */

  /* Enable single tap event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case LSM6DSR_INT1_PIN:
      if (lsm6dsr_pin_int1_route_get(&(pObj->Ctx), &val1) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }

      val1.md1_cfg.int1_single_tap = PROPERTY_ENABLE;

      if (lsm6dsr_pin_int1_route_set(&(pObj->Ctx), &val1) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }
      break;

    case LSM6DSR_INT2_PIN:
      if (lsm6dsr_pin_int2_route_get(&(pObj->Ctx), &val2) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }

      val2.md2_cfg.int2_single_tap = PROPERTY_ENABLE;

      if (lsm6dsr_pin_int2_route_set(&(pObj->Ctx), &val2) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }
      break;

    default:
      ret = LSM6DSR_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Disable single tap detection
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Disable_Single_Tap_Detection(LSM6DSR_Object_t *pObj)
{
	lsm6dsr_pin_int1_route_t val1;
	lsm6dsr_pin_int2_route_t val2;

  /* Disable single tap event on both INT1 and INT2 pins */
  if (lsm6dsr_pin_int1_route_get(&(pObj->Ctx), &val1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  val1.md1_cfg.int1_single_tap = PROPERTY_DISABLE;

  if (lsm6dsr_pin_int1_route_set(&(pObj->Ctx), &val1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  if (lsm6dsr_pin_int2_route_get(&(pObj->Ctx), &val2) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  val2.md2_cfg.int2_single_tap = PROPERTY_DISABLE;

  if (lsm6dsr_pin_int2_route_set(&(pObj->Ctx), &val2) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Reset tap quiet time window. */
  if (lsm6dsr_tap_quiet_set(&(pObj->Ctx), 0x00) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Reset tap shock time window. */
  if (lsm6dsr_tap_shock_set(&(pObj->Ctx), 0x00) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Reset tap threshold. */
  if (lsm6dsr_tap_threshold_x_set(&(pObj->Ctx), 0x00) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Disable Z direction in tap recognition. */
  if (lsm6dsr_tap_detection_on_z_set(&(pObj->Ctx), PROPERTY_DISABLE) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Disable Y direction in tap recognition. */
  if (lsm6dsr_tap_detection_on_y_set(&(pObj->Ctx), PROPERTY_DISABLE) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Disable X direction in tap recognition. */
  if (lsm6dsr_tap_detection_on_x_set(&(pObj->Ctx), PROPERTY_DISABLE) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Enable double tap detection
 * @param  pObj the device pObj
 * @param  IntPin interrupt pin line to be used
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Enable_Double_Tap_Detection(LSM6DSR_Object_t *pObj, LSM6DSR_SensorIntPin_t IntPin)
{
  int32_t ret = LSM6DSR_OK;
  lsm6dsr_pin_int1_route_t val1;
  lsm6dsr_pin_int2_route_t val2;

  /* Output Data Rate selection */
  if (LSM6DSR_ACC_SetOutputDataRate(pObj, 416.0f) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Full scale selection */
  if (LSM6DSR_ACC_SetFullScale(pObj, 2) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Enable X direction in tap recognition. */
  if (lsm6dsr_tap_detection_on_x_set(&(pObj->Ctx), PROPERTY_ENABLE) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Enable Y direction in tap recognition. */
  if (lsm6dsr_tap_detection_on_y_set(&(pObj->Ctx), PROPERTY_ENABLE) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Enable Z direction in tap recognition. */
  if (lsm6dsr_tap_detection_on_z_set(&(pObj->Ctx), PROPERTY_ENABLE) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Set tap threshold. */
  if (lsm6dsr_tap_threshold_x_set(&(pObj->Ctx), 0x08) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Set tap shock time window. */
  if (lsm6dsr_tap_shock_set(&(pObj->Ctx), 0x03) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Set tap quiet time window. */
  if (lsm6dsr_tap_quiet_set(&(pObj->Ctx), 0x03) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Set tap duration time window. */
  if (lsm6dsr_tap_dur_set(&(pObj->Ctx), 0x08) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Single and double tap enabled. */
  if (lsm6dsr_tap_mode_set(&(pObj->Ctx), LSM6DSR_BOTH_SINGLE_DOUBLE) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Enable double tap event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case LSM6DSR_INT1_PIN:
      if (lsm6dsr_pin_int1_route_get(&(pObj->Ctx), &val1) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }

      val1.md1_cfg.int1_double_tap = PROPERTY_ENABLE;

      if (lsm6dsr_pin_int1_route_set(&(pObj->Ctx), &val1) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }
      break;

    case LSM6DSR_INT2_PIN:
      if (lsm6dsr_pin_int2_route_get(&(pObj->Ctx), &val2) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }

      val2.md2_cfg.int2_double_tap = PROPERTY_ENABLE;

      if (lsm6dsr_pin_int2_route_set(&(pObj->Ctx), &val2) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }
      break;

    default:
      ret = LSM6DSR_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Disable double tap detection
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Disable_Double_Tap_Detection(LSM6DSR_Object_t *pObj)
{
  lsm6dsr_pin_int1_route_t val1;
  lsm6dsr_pin_int2_route_t val2;

  /* Disable double tap event on both INT1 and INT2 pins */
  if (lsm6dsr_pin_int1_route_get(&(pObj->Ctx), &val1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  val1.md1_cfg.int1_double_tap = PROPERTY_DISABLE;

  if (lsm6dsr_pin_int1_route_set(&(pObj->Ctx), &val1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  if (lsm6dsr_pin_int2_route_get(&(pObj->Ctx), &val2) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  val2.md2_cfg.int2_double_tap = PROPERTY_DISABLE;

  if (lsm6dsr_pin_int2_route_set(&(pObj->Ctx), &val2) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Only single tap enabled. */
  if (lsm6dsr_tap_mode_set(&(pObj->Ctx), LSM6DSR_ONLY_SINGLE) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Reset tap duration time window. */
  if (lsm6dsr_tap_dur_set(&(pObj->Ctx), 0x00) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Reset tap quiet time window. */
  if (lsm6dsr_tap_quiet_set(&(pObj->Ctx), 0x00) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Reset tap shock time window. */
  if (lsm6dsr_tap_shock_set(&(pObj->Ctx), 0x00) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Reset tap threshold. */
  if (lsm6dsr_tap_threshold_x_set(&(pObj->Ctx), 0x00) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Disable Z direction in tap recognition. */
  if (lsm6dsr_tap_detection_on_z_set(&(pObj->Ctx), PROPERTY_DISABLE) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Disable Y direction in tap recognition. */
  if (lsm6dsr_tap_detection_on_y_set(&(pObj->Ctx), PROPERTY_DISABLE) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Disable X direction in tap recognition. */
  if (lsm6dsr_tap_detection_on_x_set(&(pObj->Ctx), PROPERTY_DISABLE) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Set tap threshold
 * @param  pObj the device pObj
 * @param  Threshold tap threshold
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Set_Tap_Threshold(LSM6DSR_Object_t *pObj, uint8_t Threshold)
{
  /* Set tap threshold. */
  if (lsm6dsr_tap_threshold_x_set(&(pObj->Ctx), Threshold) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Set tap shock time
 * @param  pObj the device pObj
 * @param  Time tap shock time
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Set_Tap_Shock_Time(LSM6DSR_Object_t *pObj, uint8_t Time)
{
  /* Set tap shock time window. */
  if (lsm6dsr_tap_shock_set(&(pObj->Ctx), Time) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Set tap quiet time
 * @param  pObj the device pObj
 * @param  Time tap quiet time
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Set_Tap_Quiet_Time(LSM6DSR_Object_t *pObj, uint8_t Time)
{
  /* Set tap quiet time window. */
  if (lsm6dsr_tap_quiet_set(&(pObj->Ctx), Time) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Set tap duration time
 * @param  pObj the device pObj
 * @param  Time tap duration time
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Set_Tap_Duration_Time(LSM6DSR_Object_t *pObj, uint8_t Time)
{
  /* Set tap duration time window. */
  if (lsm6dsr_tap_dur_set(&(pObj->Ctx), Time) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Enable 6D orientation detection
 * @param  pObj the device pObj
 * @param  IntPin interrupt pin line to be used
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Enable_6D_Orientation(LSM6DSR_Object_t *pObj, LSM6DSR_SensorIntPin_t IntPin)
{
  int32_t ret = LSM6DSR_OK;
  lsm6dsr_pin_int1_route_t val1;
  lsm6dsr_pin_int2_route_t val2;

  /* Output Data Rate selection */
  if (LSM6DSR_ACC_SetOutputDataRate(pObj, 416.0f) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Full scale selection */
  if (LSM6DSR_ACC_SetFullScale(pObj, 2) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* 6D orientation enabled. */
  if (lsm6dsr_6d_threshold_set(&(pObj->Ctx), LSM6DSR_DEG_60) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Enable 6D orientation event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case LSM6DSR_INT1_PIN:
      if (lsm6dsr_pin_int1_route_get(&(pObj->Ctx), &val1) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }

      val1.md1_cfg.int1_6d = PROPERTY_ENABLE;

      if (lsm6dsr_pin_int1_route_set(&(pObj->Ctx), &val1) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }
      break;

    case LSM6DSR_INT2_PIN:
      if (lsm6dsr_pin_int2_route_get(&(pObj->Ctx), &val2) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }

      val2.md2_cfg.int2_6d = PROPERTY_ENABLE;

      if (lsm6dsr_pin_int2_route_set(&(pObj->Ctx), &val2) != LSM6DSR_OK)
      {
        return LSM6DSR_ERROR;
      }
      break;

    default:
      ret = LSM6DSR_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Disable 6D orientation detection
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Disable_6D_Orientation(LSM6DSR_Object_t *pObj)
{
  lsm6dsr_pin_int1_route_t val1;
  lsm6dsr_pin_int2_route_t val2;

  /* Disable 6D orientation event on both INT1 and INT2 pins */
  if (lsm6dsr_pin_int1_route_get(&(pObj->Ctx), &val1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  val1.md1_cfg.int1_6d = PROPERTY_DISABLE;

  if (lsm6dsr_pin_int1_route_set(&(pObj->Ctx), &val1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  if (lsm6dsr_pin_int2_route_get(&(pObj->Ctx), &val2) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  val2.md2_cfg.int2_6d = PROPERTY_DISABLE;

  if (lsm6dsr_pin_int2_route_set(&(pObj->Ctx), &val2) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  /* Reset 6D orientation. */
  if (lsm6dsr_6d_threshold_set(&(pObj->Ctx), LSM6DSR_DEG_80) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Set 6D orientation threshold
 * @param  pObj the device pObj
 * @param  Threshold 6D Orientation detection threshold
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Set_6D_Orientation_Threshold(LSM6DSR_Object_t *pObj, uint8_t Threshold)
{
  if (lsm6dsr_6d_threshold_set(&(pObj->Ctx), (lsm6dsr_sixd_ths_t)Threshold) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Get the status of XLow orientation
 * @param  pObj the device pObj
 * @param  XLow the status of XLow orientation
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Get_6D_Orientation_XL(LSM6DSR_Object_t *pObj, uint8_t *XLow)
{
  lsm6dsr_d6d_src_t data;

  if (lsm6dsr_read_reg(&(pObj->Ctx), LSM6DSR_D6D_SRC, (uint8_t *)&data, 1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  *XLow = data.xl;

  return LSM6DSR_OK;
}

/**
 * @brief  Get the status of XHigh orientation
 * @param  pObj the device pObj
 * @param  XHigh the status of XHigh orientation
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Get_6D_Orientation_XH(LSM6DSR_Object_t *pObj, uint8_t *XHigh)
{
  lsm6dsr_d6d_src_t data;

  if (lsm6dsr_read_reg(&(pObj->Ctx), LSM6DSR_D6D_SRC, (uint8_t *)&data, 1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  *XHigh = data.xh;

  return LSM6DSR_OK;
}

/**
 * @brief  Get the status of YLow orientation
 * @param  pObj the device pObj
 * @param  YLow the status of YLow orientation
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Get_6D_Orientation_YL(LSM6DSR_Object_t *pObj, uint8_t *YLow)
{
  lsm6dsr_d6d_src_t data;

  if (lsm6dsr_read_reg(&(pObj->Ctx), LSM6DSR_D6D_SRC, (uint8_t *)&data, 1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  *YLow = data.yl;

  return LSM6DSR_OK;
}

/**
 * @brief  Get the status of YHigh orientation
 * @param  pObj the device pObj
 * @param  YHigh the status of YHigh orientation
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Get_6D_Orientation_YH(LSM6DSR_Object_t *pObj, uint8_t *YHigh)
{
  lsm6dsr_d6d_src_t data;

  if (lsm6dsr_read_reg(&(pObj->Ctx), LSM6DSR_D6D_SRC, (uint8_t *)&data, 1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  *YHigh = data.yh;

  return LSM6DSR_OK;
}

/**
 * @brief  Get the status of ZLow orientation
 * @param  pObj the device pObj
 * @param  ZLow the status of ZLow orientation
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Get_6D_Orientation_ZL(LSM6DSR_Object_t *pObj, uint8_t *ZLow)
{
  lsm6dsr_d6d_src_t data;

  if (lsm6dsr_read_reg(&(pObj->Ctx), LSM6DSR_D6D_SRC, (uint8_t *)&data, 1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  *ZLow = data.zl;

  return LSM6DSR_OK;
}

/**
 * @brief  Get the status of ZHigh orientation
 * @param  pObj the device pObj
 * @param  ZHigh the status of ZHigh orientation
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Get_6D_Orientation_ZH(LSM6DSR_Object_t *pObj, uint8_t *ZHigh)
{
  lsm6dsr_d6d_src_t data;

  if (lsm6dsr_read_reg(&(pObj->Ctx), LSM6DSR_D6D_SRC, (uint8_t *)&data, 1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  *ZHigh = data.zh;

  return LSM6DSR_OK;
}

/**
 * @brief  Get the LSM6DSR ACC data ready bit value
 * @param  pObj the device pObj
 * @param  Status the status of data ready bit
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Get_DRDY_Status(LSM6DSR_Object_t *pObj, uint8_t *Status)
{
  if (lsm6dsr_xl_flag_data_ready_get(&(pObj->Ctx), Status) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Get the status of all hardware events
 * @param  pObj the device pObj
 * @param  Status the status of all hardware events
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Get_Event_Status(LSM6DSR_Object_t *pObj, LSM6DSR_Event_Status_t *Status)
{
  uint8_t tilt_ia;
  lsm6dsr_wake_up_src_t wake_up_src;
  lsm6dsr_tap_src_t tap_src;
  lsm6dsr_d6d_src_t d6d_src;
  lsm6dsr_emb_func_src_t func_src;
  lsm6dsr_md1_cfg_t md1_cfg;
  lsm6dsr_md2_cfg_t md2_cfg;
  lsm6dsr_emb_func_int1_t int1_ctrl;
  lsm6dsr_emb_func_int2_t int2_ctrl;

  (void)memset((void *)Status, 0x0, sizeof(LSM6DSR_Event_Status_t));

  if (lsm6dsr_read_reg(&(pObj->Ctx), LSM6DSR_WAKE_UP_SRC, (uint8_t *)&wake_up_src, 1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  if (lsm6dsr_read_reg(&(pObj->Ctx), LSM6DSR_TAP_SRC, (uint8_t *)&tap_src, 1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  if (lsm6dsr_read_reg(&(pObj->Ctx), LSM6DSR_D6D_SRC, (uint8_t *)&d6d_src, 1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  if (lsm6dsr_mem_bank_set(&(pObj->Ctx), LSM6DSR_EMBEDDED_FUNC_BANK) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  if (lsm6dsr_read_reg(&(pObj->Ctx), LSM6DSR_EMB_FUNC_SRC, (uint8_t *)&func_src, 1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  if (lsm6dsr_read_reg(&(pObj->Ctx), LSM6DSR_EMB_FUNC_INT1, (uint8_t *)&int1_ctrl, 1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  if (lsm6dsr_read_reg(&(pObj->Ctx), LSM6DSR_EMB_FUNC_INT2, (uint8_t *)&int2_ctrl, 1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  if (lsm6dsr_mem_bank_set(&(pObj->Ctx), LSM6DSR_USER_BANK) != 0)
  {
    return LSM6DSR_ERROR;
  }

  if (lsm6dsr_read_reg(&(pObj->Ctx), LSM6DSR_MD1_CFG, (uint8_t *)&md1_cfg, 1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  if (lsm6dsr_read_reg(&(pObj->Ctx), LSM6DSR_MD2_CFG, (uint8_t *)&md2_cfg, 1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  if (lsm6dsr_tilt_flag_data_ready_get(&(pObj->Ctx), &tilt_ia) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  if ((md1_cfg.int1_ff == 1U) || (md2_cfg.int2_ff == 1U))
  {
    if (wake_up_src.ff_ia == 1U)
    {
      Status->FreeFallStatus = 1;
    }
  }

  if ((md1_cfg.int1_wu == 1U) || (md2_cfg.int2_wu == 1U))
  {
    if (wake_up_src.wu_ia == 1U)
    {
      Status->WakeUpStatus = 1;
    }
  }

  if ((md1_cfg.int1_single_tap == 1U) || (md2_cfg.int2_single_tap == 1U))
  {
    if (tap_src.single_tap == 1U)
    {
      Status->TapStatus = 1;
    }
  }

  if ((md1_cfg.int1_double_tap == 1U) || (md2_cfg.int2_double_tap == 1U))
  {
    if (tap_src.double_tap == 1U)
    {
      Status->DoubleTapStatus = 1;
    }
  }

  if ((md1_cfg.int1_6d == 1U) || (md2_cfg.int2_6d == 1U))
  {
    if (d6d_src.d6d_ia == 1U)
    {
      Status->D6DOrientationStatus = 1;
    }
  }

  if (int1_ctrl.int1_step_detector == 1U)
  {
    if (func_src.step_detected == 1U)
    {
      Status->StepStatus = 1;
    }
  }

  if ((int1_ctrl.int1_tilt == 1U) || (int2_ctrl.int2_tilt == 1U))
  {
    if (tilt_ia == 1U)
    {
      Status->TiltStatus = 1;
    }
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Set self test
 * @param  pObj the device pObj
 * @param  val the value of st_xl in reg CTRL5_C
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_ACC_Set_SelfTest(LSM6DSR_Object_t *pObj, uint8_t val)
{
  lsm6dsr_st_xl_t reg;

  reg = (val == 0U)  ? LSM6DSR_XL_ST_DISABLE
        : (val == 1U)  ? LSM6DSR_XL_ST_POSITIVE
        : (val == 2U)  ? LSM6DSR_XL_ST_NEGATIVE
        :                LSM6DSR_XL_ST_DISABLE;

  if (lsm6dsr_xl_self_test_set(&(pObj->Ctx), reg) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Get the LSM6DSR GYRO data ready bit value
 * @param  pObj the device pObj
 * @param  Status the status of data ready bit
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_GYRO_Get_DRDY_Status(LSM6DSR_Object_t *pObj, uint8_t *Status)
{
  if (lsm6dsr_gy_flag_data_ready_get(&(pObj->Ctx), Status) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Set self test
 * @param  pObj the device pObj
 * @param  val the value of st_xl in reg CTRL5_C
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_GYRO_Set_SelfTest(LSM6DSR_Object_t *pObj, uint8_t val)
{
  lsm6dsr_st_g_t reg;

  reg = (val == 0U)  ? LSM6DSR_GY_ST_DISABLE
        : (val == 1U)  ? LSM6DSR_GY_ST_POSITIVE
        : (val == 2U)  ? LSM6DSR_GY_ST_NEGATIVE
        :                LSM6DSR_GY_ST_DISABLE;


  if (lsm6dsr_gy_self_test_set(&(pObj->Ctx), reg) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Get the LSM6DSR FIFO number of samples
 * @param  pObj the device pObj
 * @param  NumSamples number of samples
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_FIFO_Get_Num_Samples(LSM6DSR_Object_t *pObj, uint16_t *NumSamples)
{
  if (lsm6dsr_fifo_data_level_get(&(pObj->Ctx), NumSamples) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Get the LSM6DSR FIFO full status
 * @param  pObj the device pObj
 * @param  Status FIFO full status
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_FIFO_Get_Full_Status(LSM6DSR_Object_t *pObj, uint8_t *Status)
{
  lsm6dsr_reg_t reg;

  if (lsm6dsr_read_reg(&(pObj->Ctx), LSM6DSR_FIFO_STATUS2, &reg.byte, 1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  *Status = reg.fifo_status2.fifo_full_ia;

  return LSM6DSR_OK;
}

/**
 * @brief  Set the LSM6DSR FIFO full interrupt on INT1 pin
 * @param  pObj the device pObj
 * @param  Status FIFO full interrupt on INT1 pin status
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_FIFO_Set_INT1_FIFO_Full(LSM6DSR_Object_t *pObj, uint8_t Status)
{
  lsm6dsr_reg_t reg;

  if (lsm6dsr_read_reg(&(pObj->Ctx), LSM6DSR_INT1_CTRL, &reg.byte, 1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  reg.int1_ctrl.int1_fifo_full = Status;

  if (lsm6dsr_write_reg(&(pObj->Ctx), LSM6DSR_INT1_CTRL, &reg.byte, 1) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Set the LSM6DSR FIFO watermark level
 * @param  pObj the device pObj
 * @param  Watermark FIFO watermark level
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_FIFO_Set_Watermark_Level(LSM6DSR_Object_t *pObj, uint16_t Watermark)
{
  if (lsm6dsr_fifo_watermark_set(&(pObj->Ctx), Watermark) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Set the LSM6DSR FIFO stop on watermark
 * @param  pObj the device pObj
 * @param  Status FIFO stop on watermark status
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_FIFO_Set_Stop_On_Fth(LSM6DSR_Object_t *pObj, uint8_t Status)
{
  if (lsm6dsr_fifo_stop_on_wtm_set(&(pObj->Ctx), Status) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Set the LSM6DSR FIFO mode
 * @param  pObj the device pObj
 * @param  Mode FIFO mode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_FIFO_Set_Mode(LSM6DSR_Object_t *pObj, uint8_t Mode)
{
  int32_t ret = LSM6DSR_OK;

  /* Verify that the passed parameter contains one of the valid values. */
  switch ((lsm6dsr_fifo_mode_t)Mode)
  {
    case LSM6DSR_BYPASS_MODE:
    case LSM6DSR_FIFO_MODE:
    case LSM6DSR_STREAM_TO_FIFO_MODE:
    case LSM6DSR_BYPASS_TO_STREAM_MODE:
    case LSM6DSR_STREAM_MODE:
      break;

    default:
      ret = LSM6DSR_ERROR;
      break;
  }

  if (ret == LSM6DSR_ERROR)
  {
    return ret;
  }

  if (lsm6dsr_fifo_mode_set(&(pObj->Ctx), (lsm6dsr_fifo_mode_t)Mode) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return ret;
}

/**
 * @brief  Get the LSM6DSR FIFO tag
 * @param  pObj the device pObj
 * @param  Tag FIFO tag
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_FIFO_Get_Tag(LSM6DSR_Object_t *pObj, uint8_t *Tag)
{
  lsm6dsr_fifo_tag_t tag_local;

	if (lsm6dsr_fifo_sensor_tag_get(&(pObj->Ctx), &tag_local) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

	*Tag = (uint8_t)tag_local;

  return LSM6DSR_OK;
}

/**
 * @brief  Get the LSM6DSR FIFO raw data
 * @param  pObj the device pObj
 * @param  Data FIFO raw data array [6]
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_FIFO_Get_Data(LSM6DSR_Object_t *pObj, uint8_t *Data)
{
  if (lsm6dsr_read_reg(&(pObj->Ctx), LSM6DSR_FIFO_DATA_OUT_X_L, Data, 6) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Get the LSM6DSR FIFO accelero single sample (16-bit data per 3 axes) and calculate acceleration [mg]
 * @param  pObj the device pObj
 * @param  Acceleration FIFO accelero axes [mg]
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_FIFO_ACC_Get_Axes(LSM6DSR_Object_t *pObj, LSM6DSR_Axes_t *Acceleration)
{
  uint8_t data[6];
  int16_t data_raw[3];
  float sensitivity = 0.0f;
  float acceleration_float[3];

  if (LSM6DSR_FIFO_Get_Data(pObj, data) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  data_raw[0] = ((int16_t)data[1] << 8) | data[0];
  data_raw[1] = ((int16_t)data[3] << 8) | data[2];
  data_raw[2] = ((int16_t)data[5] << 8) | data[4];

  if (LSM6DSR_ACC_GetSensitivity(pObj, &sensitivity) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  acceleration_float[0] = (float)data_raw[0] * sensitivity;
  acceleration_float[1] = (float)data_raw[1] * sensitivity;
  acceleration_float[2] = (float)data_raw[2] * sensitivity;

  Acceleration->x = (int32_t)acceleration_float[0];
  Acceleration->y = (int32_t)acceleration_float[1];
  Acceleration->z = (int32_t)acceleration_float[2];

  return LSM6DSR_OK;
}

/**
 * @brief  Set the LSM6DSR FIFO accelero BDR value
 * @param  pObj the device pObj
 * @param  Bdr FIFO accelero BDR value
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_FIFO_ACC_Set_BDR(LSM6DSR_Object_t *pObj, float Bdr)
{
  lsm6dsr_bdr_xl_t new_bdr;

  new_bdr = (Bdr <=    0.0f) ? LSM6DSR_XL_NOT_BATCHED
            : (Bdr <=   12.5f) ? LSM6DSR_XL_BATCHED_AT_12Hz5
            : (Bdr <=   26.0f) ? LSM6DSR_XL_BATCHED_AT_26Hz
            : (Bdr <=   52.0f) ? LSM6DSR_XL_BATCHED_AT_52Hz
            : (Bdr <=  104.0f) ? LSM6DSR_XL_BATCHED_AT_104Hz
            : (Bdr <=  208.0f) ? LSM6DSR_XL_BATCHED_AT_208Hz
            : (Bdr <=  416.0f) ? LSM6DSR_XL_BATCHED_AT_417Hz
            : (Bdr <=  833.0f) ? LSM6DSR_XL_BATCHED_AT_833Hz
            : (Bdr <= 1660.0f) ? LSM6DSR_XL_BATCHED_AT_1667Hz
            : (Bdr <= 3330.0f) ? LSM6DSR_XL_BATCHED_AT_3333Hz
            :                    LSM6DSR_XL_BATCHED_AT_6667Hz;

  if (lsm6dsr_fifo_xl_batch_set(&(pObj->Ctx), new_bdr) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Get the LSM6DSR FIFO gyro single sample (16-bit data per 3 axes) and calculate angular velocity [mDPS]
 * @param  pObj the device pObj
 * @param  AngularVelocity FIFO gyro axes [mDPS]
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_FIFO_GYRO_Get_Axes(LSM6DSR_Object_t *pObj, LSM6DSR_Axes_t *AngularVelocity)
{
  uint8_t data[6];
  int16_t data_raw[3];
  float sensitivity = 0.0f;
  float angular_velocity_float[3];

  if (LSM6DSR_FIFO_Get_Data(pObj, data) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  data_raw[0] = ((int16_t)data[1] << 8) | data[0];
  data_raw[1] = ((int16_t)data[3] << 8) | data[2];
  data_raw[2] = ((int16_t)data[5] << 8) | data[4];

  if (LSM6DSR_GYRO_GetSensitivity(pObj, &sensitivity) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  angular_velocity_float[0] = (float)data_raw[0] * sensitivity;
  angular_velocity_float[1] = (float)data_raw[1] * sensitivity;
  angular_velocity_float[2] = (float)data_raw[2] * sensitivity;

  AngularVelocity->x = (int32_t)angular_velocity_float[0];
  AngularVelocity->y = (int32_t)angular_velocity_float[1];
  AngularVelocity->z = (int32_t)angular_velocity_float[2];

  return LSM6DSR_OK;
}

/**
 * @brief  Set the LSM6DSR FIFO gyro BDR value
 * @param  pObj the device pObj
 * @param  Bdr FIFO gyro BDR value
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSR_FIFO_GYRO_Set_BDR(LSM6DSR_Object_t *pObj, float Bdr)
{
  lsm6dsr_bdr_gy_t new_bdr;

  new_bdr = (Bdr <=    0.0f) ? LSM6DSR_GY_NOT_BATCHED
            : (Bdr <=   12.5f) ? LSM6DSR_GY_BATCHED_AT_12Hz5
            : (Bdr <=   26.0f) ? LSM6DSR_GY_BATCHED_AT_26Hz
            : (Bdr <=   52.0f) ? LSM6DSR_GY_BATCHED_AT_52Hz
            : (Bdr <=  104.0f) ? LSM6DSR_GY_BATCHED_AT_104Hz
            : (Bdr <=  208.0f) ? LSM6DSR_GY_BATCHED_AT_208Hz
            : (Bdr <=  416.0f) ? LSM6DSR_GY_BATCHED_AT_417Hz
            : (Bdr <=  833.0f) ? LSM6DSR_GY_BATCHED_AT_833Hz
            : (Bdr <= 1660.0f) ? LSM6DSR_GY_BATCHED_AT_1667Hz
            : (Bdr <= 3330.0f) ? LSM6DSR_GY_BATCHED_AT_3333Hz
            :                    LSM6DSR_GY_BATCHED_AT_6667Hz;

  if (lsm6dsr_fifo_gy_batch_set(&(pObj->Ctx), new_bdr) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @}
 */

/** @defgroup LSM6DSR_Private_Functions LSM6DSR Private Functions
 * @{
 */

/**
 * @brief  Set the LSM6DSR accelerometer sensor output data rate when enabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t LSM6DSR_ACC_SetOutputDataRate_When_Enabled(LSM6DSR_Object_t *pObj, float Odr)
{
  lsm6dsr_odr_xl_t new_odr;

  new_odr = (Odr <=   12.5f) ? LSM6DSR_XL_ODR_12Hz5
          : (Odr <=   26.0f) ? LSM6DSR_XL_ODR_26Hz
          : (Odr <=   52.0f) ? LSM6DSR_XL_ODR_52Hz
          : (Odr <=  104.0f) ? LSM6DSR_XL_ODR_104Hz
          : (Odr <=  208.0f) ? LSM6DSR_XL_ODR_208Hz
          : (Odr <=  417.0f) ? LSM6DSR_XL_ODR_417Hz
          : (Odr <=  833.0f) ? LSM6DSR_XL_ODR_833Hz
          : (Odr <= 1667.0f) ? LSM6DSR_XL_ODR_1667Hz
          : (Odr <= 3333.0f) ? LSM6DSR_XL_ODR_3333Hz
          :                    LSM6DSR_XL_ODR_6667Hz;

  /* Output data rate selection. */
  if (lsm6dsr_xl_data_rate_set(&(pObj->Ctx), new_odr) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Set the LSM6DSR accelerometer sensor output data rate when disabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t LSM6DSR_ACC_SetOutputDataRate_When_Disabled(LSM6DSR_Object_t *pObj, float Odr)
{
  pObj->acc_odr = (Odr <=   12.5f) ? LSM6DSR_XL_ODR_12Hz5
                : (Odr <=   26.0f) ? LSM6DSR_XL_ODR_26Hz
                : (Odr <=   52.0f) ? LSM6DSR_XL_ODR_52Hz
                : (Odr <=  104.0f) ? LSM6DSR_XL_ODR_104Hz
                : (Odr <=  208.0f) ? LSM6DSR_XL_ODR_208Hz
                : (Odr <=  417.0f) ? LSM6DSR_XL_ODR_417Hz
                : (Odr <=  833.0f) ? LSM6DSR_XL_ODR_833Hz
                : (Odr <= 1667.0f) ? LSM6DSR_XL_ODR_1667Hz
                : (Odr <= 3333.0f) ? LSM6DSR_XL_ODR_3333Hz
                :                    LSM6DSR_XL_ODR_6667Hz;

  return LSM6DSR_OK;
}

/**
 * @brief  Set the LSM6DSR gyroscope sensor output data rate when enabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t LSM6DSR_GYRO_SetOutputDataRate_When_Enabled(LSM6DSR_Object_t *pObj, float Odr)
{
  lsm6dsr_odr_g_t new_odr;

  new_odr = (Odr <=   12.5f) ? LSM6DSR_GY_ODR_12Hz5
          : (Odr <=   26.0f) ? LSM6DSR_GY_ODR_26Hz
          : (Odr <=   52.0f) ? LSM6DSR_GY_ODR_52Hz
          : (Odr <=  104.0f) ? LSM6DSR_GY_ODR_104Hz
          : (Odr <=  208.0f) ? LSM6DSR_GY_ODR_208Hz
          : (Odr <=  417.0f) ? LSM6DSR_GY_ODR_417Hz
          : (Odr <=  833.0f) ? LSM6DSR_GY_ODR_833Hz
          : (Odr <= 1667.0f) ? LSM6DSR_GY_ODR_1667Hz
          : (Odr <= 3333.0f) ? LSM6DSR_GY_ODR_3333Hz
          :                    LSM6DSR_GY_ODR_6667Hz;

  /* Output data rate selection. */
  if (lsm6dsr_gy_data_rate_set(&(pObj->Ctx), new_odr) != LSM6DSR_OK)
  {
    return LSM6DSR_ERROR;
  }

  return LSM6DSR_OK;
}

/**
 * @brief  Set the LSM6DSR gyroscope sensor output data rate when disabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t LSM6DSR_GYRO_SetOutputDataRate_When_Disabled(LSM6DSR_Object_t *pObj, float Odr)
{
  pObj->gyro_odr = (Odr <=   12.5f) ? LSM6DSR_GY_ODR_12Hz5
                 : (Odr <=   26.0f) ? LSM6DSR_GY_ODR_26Hz
                 : (Odr <=   52.0f) ? LSM6DSR_GY_ODR_52Hz
                 : (Odr <=  104.0f) ? LSM6DSR_GY_ODR_104Hz
                 : (Odr <=  208.0f) ? LSM6DSR_GY_ODR_208Hz
                 : (Odr <=  417.0f) ? LSM6DSR_GY_ODR_417Hz
                 : (Odr <=  833.0f) ? LSM6DSR_GY_ODR_833Hz
                 : (Odr <= 1667.0f) ? LSM6DSR_GY_ODR_1667Hz
                 : (Odr <= 3333.0f) ? LSM6DSR_GY_ODR_3333Hz
                 :                    LSM6DSR_GY_ODR_6667Hz;

  return LSM6DSR_OK;
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
  LSM6DSR_Object_t *pObj = (LSM6DSR_Object_t *)Handle;

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
  LSM6DSR_Object_t *pObj = (LSM6DSR_Object_t *)Handle;

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
