/**
  ******************************************************************************
  * @file    lsm6dso16is.c
  * @author  MEMS Software Solutions Team
  * @brief   LSM6DSO16IS driver file
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
#include "lsm6dso16is.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup LSM6DSO16IS LSM6DSO16IS
  * @{
  */

/** @defgroup LSM6DSO16IS_Exported_Variables LSM6DSO16IS Exported Variables
  * @{
  */

LSM6DSO16IS_CommonDrv_t LSM6DSO16IS_COMMON_Driver =
{
  LSM6DSO16IS_Init,
  LSM6DSO16IS_DeInit,
  LSM6DSO16IS_ReadID,
  LSM6DSO16IS_GetCapabilities,
};

LSM6DSO16IS_ACC_Drv_t LSM6DSO16IS_ACC_Driver =
{
  LSM6DSO16IS_ACC_Enable,
  LSM6DSO16IS_ACC_Disable,
  LSM6DSO16IS_ACC_GetSensitivity,
  LSM6DSO16IS_ACC_GetOutputDataRate,
  LSM6DSO16IS_ACC_SetOutputDataRate,
  LSM6DSO16IS_ACC_GetFullScale,
  LSM6DSO16IS_ACC_SetFullScale,
  LSM6DSO16IS_ACC_GetAxes,
  LSM6DSO16IS_ACC_GetAxesRaw,
};

LSM6DSO16IS_GYRO_Drv_t LSM6DSO16IS_GYRO_Driver =
{
  LSM6DSO16IS_GYRO_Enable,
  LSM6DSO16IS_GYRO_Disable,
  LSM6DSO16IS_GYRO_GetSensitivity,
  LSM6DSO16IS_GYRO_GetOutputDataRate,
  LSM6DSO16IS_GYRO_SetOutputDataRate,
  LSM6DSO16IS_GYRO_GetFullScale,
  LSM6DSO16IS_GYRO_SetFullScale,
  LSM6DSO16IS_GYRO_GetAxes,
  LSM6DSO16IS_GYRO_GetAxesRaw,
};

/**
  * @}
  */

/** @defgroup LSM6DSO16IS_Private_Function_Prototypes LSM6DSO16IS Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t LSM6DSO16IS_ACC_SetOutputDataRate_When_Enabled(LSM6DSO16IS_Object_t *pObj, float_t Odr);
static int32_t LSM6DSO16IS_ACC_SetOutputDataRate_When_Disabled(LSM6DSO16IS_Object_t *pObj, float_t Odr);
static int32_t LSM6DSO16IS_GYRO_SetOutputDataRate_When_Enabled(LSM6DSO16IS_Object_t *pObj, float_t Odr);
static int32_t LSM6DSO16IS_GYRO_SetOutputDataRate_When_Disabled(LSM6DSO16IS_Object_t *pObj, float_t Odr);

/**
  * @}
  */

/** @defgroup LSM6DSO16IS_Exported_Functions LSM6DSO16IS Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_RegisterBusIO(LSM6DSO16IS_Object_t *pObj, LSM6DSO16IS_IO_t *pIO)
{
  int32_t ret = LSM6DSO16IS_OK;

  if (pObj == NULL)
  {
    ret = LSM6DSO16IS_ERROR;
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
      ret = LSM6DSO16IS_ERROR;
    }
    else if (pObj->IO.Init() != LSM6DSO16IS_OK)
    {
      ret = LSM6DSO16IS_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == LSM6DSO16IS_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x0C;

          if (LSM6DSO16IS_Write_Reg(pObj, LSM6DSO16IS_CTRL3_C, data) != LSM6DSO16IS_OK)
          {
            ret = LSM6DSO16IS_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  Initialize the LSM6DSO16IS sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_Init(LSM6DSO16IS_Object_t *pObj)
{
  int32_t ret = LSM6DSO16IS_OK;

  /* Set main memory bank */
  if (LSM6DSO16IS_Set_Mem_Bank(pObj, (uint8_t)LSM6DSO16IS_MAIN_MEM_BANK) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  /* Enable register address automatically incremented during a multiple byte
  access with a serial interface. */
  if (lsm6dso16is_auto_increment_set(&(pObj->Ctx), PROPERTY_ENABLE) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  /* Enable BDU */
  if (lsm6dso16is_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = LSM6DSO16IS_XL_ODR_AT_104Hz_HP;

  /* Output data rate selection - power down. */
  if (lsm6dso16is_xl_data_rate_set(&(pObj->Ctx), LSM6DSO16IS_XL_ODR_OFF) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  /* Full scale selection. */
  if (lsm6dso16is_xl_full_scale_set(&(pObj->Ctx), LSM6DSO16IS_2g) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  /* Select default output data rate. */
  pObj->gyro_odr = LSM6DSO16IS_GY_ODR_AT_104Hz_HP;

  /* Output data rate selection - power down. */
  if (lsm6dso16is_gy_data_rate_set(&(pObj->Ctx), LSM6DSO16IS_GY_ODR_OFF) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  /* Full scale selection. */
  if (lsm6dso16is_gy_full_scale_set(&(pObj->Ctx), LSM6DSO16IS_2000dps) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  if (ret == LSM6DSO16IS_OK)
  {
    pObj->is_initialized = 1;
  }

  return ret;
}

/**
  * @brief  Deinitialize the LSM6DSO16IS sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_DeInit(LSM6DSO16IS_Object_t *pObj)
{
  int32_t ret = LSM6DSO16IS_OK;

  /* Disable the component */
  if (LSM6DSO16IS_ACC_Disable(pObj) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  if (LSM6DSO16IS_GYRO_Disable(pObj) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  if (ret == LSM6DSO16IS_OK)
  {
    /* Reset output data rate. */
    pObj->acc_odr = LSM6DSO16IS_XL_ODR_OFF;
    pObj->gyro_odr = LSM6DSO16IS_GY_ODR_OFF;

    pObj->is_initialized = 0;
  }

  return ret;
}

/**
  * @brief  Read component ID
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_ReadID(LSM6DSO16IS_Object_t *pObj, uint8_t *Id)
{
  int32_t ret = LSM6DSO16IS_OK;

  if (lsm6dso16is_device_id_get(&(pObj->Ctx), Id) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  return ret;
}

/**
  * @brief  Get LSM6DSO16IS sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to LSM6DSO16IS sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_GetCapabilities(LSM6DSO16IS_Object_t *pObj, LSM6DSO16IS_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Acc          = 1;
  Capabilities->Gyro         = 1;
  Capabilities->Magneto      = 0;
  Capabilities->LowPower     = 0;
  Capabilities->GyroMaxFS    = 2000;
  Capabilities->AccMaxFS     = 16;
  Capabilities->MagMaxFS     = 0;
  Capabilities->GyroMaxOdr   = 3333.0f;
  Capabilities->AccMaxOdr    = 3333.0f;
  Capabilities->MagMaxOdr    = 0.0f;

  return LSM6DSO16IS_OK;
}

/**
  * @brief  Enable the LSM6DSO16IS accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_ACC_Enable(LSM6DSO16IS_Object_t *pObj)
{
  int32_t ret = LSM6DSO16IS_OK;

  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    ret = LSM6DSO16IS_OK;
  }
  else
  {
    /* Output data rate selection. */
    if (lsm6dso16is_xl_data_rate_set(&(pObj->Ctx), pObj->acc_odr) != LSM6DSO16IS_OK)
    {
      ret = LSM6DSO16IS_ERROR;
    }

    pObj->acc_is_enabled = 1;
  }

  return ret;
}

/**
  * @brief  Disable the LSM6DSO16IS accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_ACC_Disable(LSM6DSO16IS_Object_t *pObj)
{
  int32_t ret = LSM6DSO16IS_OK;

  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    ret = LSM6DSO16IS_OK;
  }
  else
  {
    /* Get current output data rate. */
    if (lsm6dso16is_xl_data_rate_get(&(pObj->Ctx), &pObj->acc_odr) != LSM6DSO16IS_OK)
    {
      ret = LSM6DSO16IS_ERROR;
    }

    /* Output data rate selection - power down. */
    if (lsm6dso16is_xl_data_rate_set(&(pObj->Ctx), LSM6DSO16IS_XL_ODR_OFF) != LSM6DSO16IS_OK)
    {
      ret = LSM6DSO16IS_ERROR;
    }

    pObj->acc_is_enabled = 0;
  }

  return ret;
}

/**
  * @brief  Get the LSM6DSO16IS accelerometer sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_ACC_GetSensitivity(LSM6DSO16IS_Object_t *pObj, float_t *Sensitivity)
{
  int32_t ret = LSM6DSO16IS_OK;
  lsm6dso16is_xl_full_scale_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (lsm6dso16is_xl_full_scale_get(&(pObj->Ctx), &full_scale) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  /* Store the Sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case LSM6DSO16IS_2g:
      *Sensitivity = LSM6DSO16IS_ACC_SENSITIVITY_FS_2G;
      break;

    case LSM6DSO16IS_4g:
      *Sensitivity = LSM6DSO16IS_ACC_SENSITIVITY_FS_4G;
      break;

    case LSM6DSO16IS_8g:
      *Sensitivity = LSM6DSO16IS_ACC_SENSITIVITY_FS_8G;
      break;

    case LSM6DSO16IS_16g:
      *Sensitivity = LSM6DSO16IS_ACC_SENSITIVITY_FS_16G;
      break;

    default:
      ret = LSM6DSO16IS_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the LSM6DSO16IS accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_ACC_GetOutputDataRate(LSM6DSO16IS_Object_t *pObj, float_t *Odr)
{
  int32_t ret = LSM6DSO16IS_OK;
  lsm6dso16is_xl_data_rate_t odr_low_level;

  /* Get current output data rate. */
  if (lsm6dso16is_xl_data_rate_get(&(pObj->Ctx), &odr_low_level) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  switch (odr_low_level)
  {
    case LSM6DSO16IS_XL_ODR_OFF:
      *Odr = 0.0f;
      break;

    case LSM6DSO16IS_XL_ODR_AT_1Hz6_LP:
      *Odr = 1.6f;
      break;

    case LSM6DSO16IS_XL_ODR_AT_12Hz5_LP:
    case LSM6DSO16IS_XL_ODR_AT_12Hz5_HP:
      *Odr = 12.5f;
      break;

    case LSM6DSO16IS_XL_ODR_AT_26H_LP:
    case LSM6DSO16IS_XL_ODR_AT_26H_HP:
      *Odr = 26.0f;
      break;

    case LSM6DSO16IS_XL_ODR_AT_52Hz_LP:
    case LSM6DSO16IS_XL_ODR_AT_52Hz_HP:
      *Odr = 52.0f;
      break;

    case LSM6DSO16IS_XL_ODR_AT_104Hz_LP:
    case LSM6DSO16IS_XL_ODR_AT_104Hz_HP:
      *Odr = 104.0f;
      break;

    case LSM6DSO16IS_XL_ODR_AT_208Hz_LP:
    case LSM6DSO16IS_XL_ODR_AT_208Hz_HP:
      *Odr = 208.0f;
      break;

    case LSM6DSO16IS_XL_ODR_AT_416Hz_LP:
    case LSM6DSO16IS_XL_ODR_AT_416Hz_HP:
      *Odr = 416.0f;
      break;

    case LSM6DSO16IS_XL_ODR_AT_833Hz_LP:
    case LSM6DSO16IS_XL_ODR_AT_833Hz_HP:
      *Odr = 833.0f;
      break;

    case LSM6DSO16IS_XL_ODR_AT_1667Hz_LP:
    case LSM6DSO16IS_XL_ODR_AT_1667Hz_HP:
      *Odr = 1667.0f;
      break;

    case LSM6DSO16IS_XL_ODR_AT_3333Hz_LP:
    case LSM6DSO16IS_XL_ODR_AT_3333Hz_HP:
      *Odr = 3333.0f;
      break;

    case LSM6DSO16IS_XL_ODR_AT_6667Hz_LP:
    case LSM6DSO16IS_XL_ODR_AT_6667Hz_HP:
      *Odr = 6667.0f;
      break;

    default:
      ret = LSM6DSO16IS_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the LSM6DSO16IS accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_ACC_SetOutputDataRate(LSM6DSO16IS_Object_t *pObj, float_t Odr)
{
  int32_t ret;

  /* Check if the component is enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    ret = LSM6DSO16IS_ACC_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    ret = LSM6DSO16IS_ACC_SetOutputDataRate_When_Disabled(pObj, Odr);
  }

  return ret;
}

/**
  * @brief  Get the LSM6DSO16IS accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_ACC_GetFullScale(LSM6DSO16IS_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = LSM6DSO16IS_OK;
  lsm6dso16is_xl_full_scale_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (lsm6dso16is_xl_full_scale_get(&(pObj->Ctx), &fs_low_level) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  switch (fs_low_level)
  {
    case LSM6DSO16IS_2g:
      *FullScale =  2;
      break;

    case LSM6DSO16IS_4g:
      *FullScale =  4;
      break;

    case LSM6DSO16IS_8g:
      *FullScale =  8;
      break;

    case LSM6DSO16IS_16g:
      *FullScale = 16;
      break;

    default:
      ret = LSM6DSO16IS_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the LSM6DSO16IS accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_ACC_SetFullScale(LSM6DSO16IS_Object_t *pObj, int32_t FullScale)
{
  int32_t ret = LSM6DSO16IS_OK;
  lsm6dso16is_xl_full_scale_t new_fs;

  new_fs = (FullScale <= 2) ? LSM6DSO16IS_2g
           : (FullScale <= 4) ? LSM6DSO16IS_4g
           : (FullScale <= 8) ? LSM6DSO16IS_8g
           :                    LSM6DSO16IS_16g;

  if (lsm6dso16is_xl_full_scale_set(&(pObj->Ctx), new_fs) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  return ret;
}

/**
  * @brief  Get the LSM6DSO16IS accelerometer sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_ACC_GetAxesRaw(LSM6DSO16IS_Object_t *pObj, LSM6DSO16IS_AxesRaw_t *Value)
{
  int32_t ret = LSM6DSO16IS_OK;
  int16_t data_raw[3];

  /* Read raw data values. */
  if (lsm6dso16is_acceleration_raw_get(&(pObj->Ctx), data_raw) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw[0];
  Value->y = data_raw[1];
  Value->z = data_raw[2];

  return ret;
}

/**
  * @brief  Get the LSM6DSO16IS accelerometer sensor axes
  * @param  pObj the device pObj
  * @param  Acceleration pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_ACC_GetAxes(LSM6DSO16IS_Object_t *pObj, LSM6DSO16IS_Axes_t *Acceleration)
{
  int32_t ret = LSM6DSO16IS_OK;
  int16_t data_raw[3];
  float_t sensitivity = 0.0f;

  /* Read raw data values. */
  if (lsm6dso16is_acceleration_raw_get(&(pObj->Ctx), data_raw) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  /* Get LSM6DSO16IS actual sensitivity. */
  if (LSM6DSO16IS_ACC_GetSensitivity(pObj, &sensitivity) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  /* Calculate the data. */
  Acceleration->x = (int32_t)((float_t)((float_t)data_raw[0] * sensitivity));
  Acceleration->y = (int32_t)((float_t)((float_t)data_raw[1] * sensitivity));
  Acceleration->z = (int32_t)((float_t)((float_t)data_raw[2] * sensitivity));

  return ret;
}

/**
  * @brief  Enable the LSM6DSO16IS gyroscope sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_GYRO_Enable(LSM6DSO16IS_Object_t *pObj)
{
  int32_t ret = LSM6DSO16IS_OK;

  /* Check if the component is already enabled */
  if (pObj->gyro_is_enabled == 1U)
  {
    ret = LSM6DSO16IS_OK;
  }
  else
  {
    /* Output data rate selection. */
    if (lsm6dso16is_gy_data_rate_set(&(pObj->Ctx), pObj->gyro_odr) != LSM6DSO16IS_OK)
    {
      ret = LSM6DSO16IS_ERROR;
    }

    pObj->gyro_is_enabled = 1;
  }

  return ret;
}

/**
  * @brief  Disable the LSM6DSO16IS gyroscope sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_GYRO_Disable(LSM6DSO16IS_Object_t *pObj)
{
  int32_t ret = LSM6DSO16IS_OK;

  /* Check if the component is already disabled */
  if (pObj->gyro_is_enabled == 0U)
  {
    ret = LSM6DSO16IS_OK;
  }
  else
  {
    /* Get current output data rate. */
    if (lsm6dso16is_gy_data_rate_get(&(pObj->Ctx), &pObj->gyro_odr) != LSM6DSO16IS_OK)
    {
      ret = LSM6DSO16IS_ERROR;
    }

    /* Output data rate selection - power down. */
    if (lsm6dso16is_gy_data_rate_set(&(pObj->Ctx), LSM6DSO16IS_GY_ODR_OFF) != LSM6DSO16IS_OK)
    {
      ret = LSM6DSO16IS_ERROR;
    }

    pObj->gyro_is_enabled = 0;
  }

  return ret;
}

/**
  * @brief  Get the LSM6DSO16IS gyroscope sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_GYRO_GetSensitivity(LSM6DSO16IS_Object_t *pObj, float_t *Sensitivity)
{
  int32_t ret = LSM6DSO16IS_OK;
  lsm6dso16is_gy_full_scale_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (lsm6dso16is_gy_full_scale_get(&(pObj->Ctx), &full_scale) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  /* Store the sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case LSM6DSO16IS_125dps:
      *Sensitivity = LSM6DSO16IS_GYRO_SENSITIVITY_FS_125DPS;
      break;

    case LSM6DSO16IS_250dps:
      *Sensitivity = LSM6DSO16IS_GYRO_SENSITIVITY_FS_250DPS;
      break;

    case LSM6DSO16IS_500dps:
      *Sensitivity = LSM6DSO16IS_GYRO_SENSITIVITY_FS_500DPS;
      break;

    case LSM6DSO16IS_1000dps:
      *Sensitivity = LSM6DSO16IS_GYRO_SENSITIVITY_FS_1000DPS;
      break;

    case LSM6DSO16IS_2000dps:
      *Sensitivity = LSM6DSO16IS_GYRO_SENSITIVITY_FS_2000DPS;
      break;

    default:
      ret = LSM6DSO16IS_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the LSM6DSO16IS gyroscope sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_GYRO_GetOutputDataRate(LSM6DSO16IS_Object_t *pObj, float_t *Odr)
{
  int32_t ret = LSM6DSO16IS_OK;
  lsm6dso16is_gy_data_rate_t odr_low_level;

  /* Get current output data rate. */
  if (lsm6dso16is_gy_data_rate_get(&(pObj->Ctx), &odr_low_level) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  switch (odr_low_level)
  {
    case LSM6DSO16IS_GY_ODR_OFF:
      *Odr = 0.0f;
      break;

    case LSM6DSO16IS_GY_ODR_AT_12Hz5_LP:
    case LSM6DSO16IS_GY_ODR_AT_12Hz5_HP:
      *Odr = 12.5f;
      break;

    case LSM6DSO16IS_GY_ODR_AT_26H_LP:
    case LSM6DSO16IS_GY_ODR_AT_26H_HP:
      *Odr = 26.0f;
      break;

    case LSM6DSO16IS_GY_ODR_AT_52Hz_LP:
    case LSM6DSO16IS_GY_ODR_AT_52Hz_HP:
      *Odr = 52.0f;
      break;

    case LSM6DSO16IS_GY_ODR_AT_104Hz_LP:
    case LSM6DSO16IS_GY_ODR_AT_104Hz_HP:
      *Odr = 104.0f;
      break;

    case LSM6DSO16IS_GY_ODR_AT_208Hz_LP:
    case LSM6DSO16IS_GY_ODR_AT_208Hz_HP:
      *Odr = 208.0f;
      break;

    case LSM6DSO16IS_GY_ODR_AT_416Hz_LP:
    case LSM6DSO16IS_GY_ODR_AT_416Hz_HP:
      *Odr = 416.0f;
      break;

    case LSM6DSO16IS_GY_ODR_AT_833Hz_LP:
    case LSM6DSO16IS_GY_ODR_AT_833Hz_HP:
      *Odr = 833.0f;
      break;

    case LSM6DSO16IS_GY_ODR_AT_1667Hz_LP:
    case LSM6DSO16IS_GY_ODR_AT_1667Hz_HP:
      *Odr =  1667.0f;
      break;

    case LSM6DSO16IS_GY_ODR_AT_3333Hz_LP:
    case LSM6DSO16IS_GY_ODR_AT_3333Hz_HP:
      *Odr =  3333.0f;
      break;

    case LSM6DSO16IS_GY_ODR_AT_6667Hz_LP:
    case LSM6DSO16IS_GY_ODR_AT_6667Hz_HP:
      *Odr =  6667.0f;
      break;

    default:
      ret = LSM6DSO16IS_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the LSM6DSO16IS gyroscope sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_GYRO_SetOutputDataRate(LSM6DSO16IS_Object_t *pObj, float_t Odr)
{
  int32_t ret;

  /* Check if the component is enabled */
  if (pObj->gyro_is_enabled == 1U)
  {
    ret = LSM6DSO16IS_GYRO_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    ret = LSM6DSO16IS_GYRO_SetOutputDataRate_When_Disabled(pObj, Odr);
  }

  return ret;
}

/**
  * @brief  Get the LSM6DSO16IS gyroscope sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_GYRO_GetFullScale(LSM6DSO16IS_Object_t *pObj, int32_t  *FullScale)
{
  int32_t ret = LSM6DSO16IS_OK;
  lsm6dso16is_gy_full_scale_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (lsm6dso16is_gy_full_scale_get(&(pObj->Ctx), &fs_low_level) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  switch (fs_low_level)
  {
    case LSM6DSO16IS_125dps:
      *FullScale =  125;
      break;

    case LSM6DSO16IS_250dps:
      *FullScale =  250;
      break;

    case LSM6DSO16IS_500dps:
      *FullScale =  500;
      break;

    case LSM6DSO16IS_1000dps:
      *FullScale = 1000;
      break;

    case LSM6DSO16IS_2000dps:
      *FullScale = 2000;
      break;

    default:
      ret = LSM6DSO16IS_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the LSM6DSO16IS gyroscope sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_GYRO_SetFullScale(LSM6DSO16IS_Object_t *pObj, int32_t FullScale)
{
  int32_t ret = LSM6DSO16IS_OK;
  lsm6dso16is_gy_full_scale_t new_fs;

  new_fs = (FullScale <= 125)  ? LSM6DSO16IS_125dps
           : (FullScale <= 250)  ? LSM6DSO16IS_250dps
           : (FullScale <= 500)  ? LSM6DSO16IS_500dps
           : (FullScale <= 1000) ? LSM6DSO16IS_1000dps
           :                       LSM6DSO16IS_2000dps;

  if (lsm6dso16is_gy_full_scale_set(&(pObj->Ctx), new_fs) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  return ret;
}

/**
  * @brief  Get the LSM6DSO16IS gyroscope sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_GYRO_GetAxesRaw(LSM6DSO16IS_Object_t *pObj, LSM6DSO16IS_AxesRaw_t *Value)
{
  int32_t ret = LSM6DSO16IS_OK;
  int16_t data_raw[3];

  /* Read raw data values. */
  if (lsm6dso16is_angular_rate_raw_get(&(pObj->Ctx), data_raw) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw[0];
  Value->y = data_raw[1];
  Value->z = data_raw[2];

  return ret;
}

/**
  * @brief  Get the LSM6DSO16IS gyroscope sensor axes
  * @param  pObj the device pObj
  * @param  AngularRate pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_GYRO_GetAxes(LSM6DSO16IS_Object_t *pObj, LSM6DSO16IS_Axes_t *AngularRate)
{
  int32_t ret = LSM6DSO16IS_OK;
  int16_t data_raw[3];
  float_t sensitivity = 0.0f;

  /* Read raw data values. */
  if (lsm6dso16is_angular_rate_raw_get(&(pObj->Ctx), data_raw) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  /* Get LSM6DSO16IS actual sensitivity. */
  if (LSM6DSO16IS_GYRO_GetSensitivity(pObj, &sensitivity) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  /* Calculate the data. */
  AngularRate->x = (int32_t)((float_t)((float_t)data_raw[0] * sensitivity));
  AngularRate->y = (int32_t)((float_t)((float_t)data_raw[1] * sensitivity));
  AngularRate->z = (int32_t)((float_t)((float_t)data_raw[2] * sensitivity));

  return ret;
}

/**
  * @brief  Get the LSM6DSO16IS register value
  * @param  pObj the device pObj
  * @param  Reg address to be read
  * @param  Data pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_Read_Reg(LSM6DSO16IS_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  int32_t ret = LSM6DSO16IS_OK;

  if (lsm6dso16is_read_reg(&(pObj->Ctx), Reg, Data, 1) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  return ret;
}

/**
  * @brief  Set the LSM6DSO16IS register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_Write_Reg(LSM6DSO16IS_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  int32_t ret = LSM6DSO16IS_OK;

  if (lsm6dso16is_write_reg(&(pObj->Ctx), Reg, &Data, 1) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  return ret;
}

/**
  * @brief  Set self test
  * @param  pObj the device pObj
  * @param  Val the value of st_xl in reg CTRL5_C
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_ACC_Set_SelfTest(LSM6DSO16IS_Object_t *pObj, uint8_t Val)
{
  int32_t ret = LSM6DSO16IS_OK;
  lsm6dso16is_xl_self_test_t reg;

  reg = (Val == 1U)  ? LSM6DSO16IS_XL_ST_POSITIVE
        : (Val == 2U)  ? LSM6DSO16IS_XL_ST_NEGATIVE
        :                LSM6DSO16IS_XL_ST_DISABLE;

  if (lsm6dso16is_xl_self_test_set(&(pObj->Ctx), reg) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  return ret;
}

/**
  * @brief  Get the LSM6DSO16IS ACC data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_ACC_Get_DRDY_Status(LSM6DSO16IS_Object_t *pObj, uint8_t *Status)
{
  int32_t ret = LSM6DSO16IS_OK;

  if (lsm6dso16is_xl_flag_data_ready_get(&(pObj->Ctx), Status) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  return ret;
}

/**
  * @brief  Get the LSM6DSO16IS ACC initialization status
  * @param  pObj the device pObj
  * @param  Status 1 if initialized, 0 otherwise
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_ACC_Get_Init_Status(LSM6DSO16IS_Object_t *pObj, uint8_t *Status)
{
  int32_t ret = LSM6DSO16IS_OK;

  if (pObj == NULL)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  *Status = pObj->is_initialized;

  return ret;
}

/**
  * @brief  Set DRDY on INT1
  * @param  pObj the device pObj
  * @param  Val the value of int1_drdy_xl in reg INT1_CTRL
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_ACC_Set_INT1_DRDY(LSM6DSO16IS_Object_t *pObj, uint8_t Val)
{
  int32_t ret = LSM6DSO16IS_OK;
  lsm6dso16is_pin_int1_route_t reg;

  if (lsm6dso16is_pin_int1_route_get(&(pObj->Ctx), &reg) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  if (Val <= 1U)
  {
    reg.drdy_xl = Val;
  }
  else
  {
    ret = LSM6DSO16IS_ERROR;
  }

  if (lsm6dso16is_pin_int1_route_set(&(pObj->Ctx), reg) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  return ret;
}

/**
  * @brief  Set self test
  * @param  pObj the device pObj
  * @param  Val the value of st_xl in reg CTRL5_C
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_GYRO_Set_SelfTest(LSM6DSO16IS_Object_t *pObj, uint8_t Val)
{
  int32_t ret = LSM6DSO16IS_OK;
  lsm6dso16is_gy_self_test_t reg;

  reg = (Val == 1U)  ? LSM6DSO16IS_GY_ST_POSITIVE
        : (Val == 2U)  ? LSM6DSO16IS_GY_ST_NEGATIVE
        :                LSM6DSO16IS_GY_ST_DISABLE;


  if (lsm6dso16is_gy_self_test_set(&(pObj->Ctx), reg) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  return ret;
}

/**
  * @brief  Get the LSM6DSO16IS GYRO data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_GYRO_Get_DRDY_Status(LSM6DSO16IS_Object_t *pObj, uint8_t *Status)
{
  int32_t ret = LSM6DSO16IS_OK;

  if (lsm6dso16is_gy_flag_data_ready_get(&(pObj->Ctx), Status) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  return ret;
}

/**
  * @brief  Get the LSM6DSO16IS GYRO initialization status
  * @param  pObj the device pObj
  * @param  Status 1 if initialized, 0 otherwise
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_GYRO_Get_Init_Status(LSM6DSO16IS_Object_t *pObj, uint8_t *Status)
{
  int32_t ret = LSM6DSO16IS_OK;

  if (pObj == NULL)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  *Status = pObj->is_initialized;

  return ret;
}

/**
  * @brief  Set DRDY on INT1
  * @param  pObj the device pObj
  * @param  Val the value of int1_drdy_g in reg INT1_CTRL
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_GYRO_Set_INT1_DRDY(LSM6DSO16IS_Object_t *pObj, uint8_t Val)
{
  int32_t ret = LSM6DSO16IS_OK;
  lsm6dso16is_pin_int1_route_t reg;

  if (lsm6dso16is_pin_int1_route_get(&(pObj->Ctx), &reg) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  if (Val <= 1U)
  {
    reg.drdy_gy = Val;
  }
  else
  {
    ret = LSM6DSO16IS_ERROR;
  }

  if (lsm6dso16is_pin_int1_route_set(&(pObj->Ctx), reg) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  return ret;
}

/**
  * @brief  Set DRDY mode
  * @param  pObj the device pObj
  * @param  Val the value of drdy_pulsed in reg LSM6DSO16IS_DRDY_PULSE_CFG_G
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_Set_DRDY_Mode(LSM6DSO16IS_Object_t *pObj, uint8_t Val)
{
  int32_t ret = LSM6DSO16IS_OK;
  lsm6dso16is_data_ready_mode_t reg;

  reg = (Val == 0U)  ? LSM6DSO16IS_DRDY_LATCHED
        :                LSM6DSO16IS_DRDY_PULSED;

  if (lsm6dso16is_data_ready_mode_set(&(pObj->Ctx), reg) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  return ret;
}

/**
  * @brief  Set memory bank
  * @param  pObj the device pObj
  * @param  Val the value of memory bank in reg FUNC_CFG_ACCESS
  *         0 - LSM6DSO16IS_MAIN_MEM_BANK, 2 - LSM6DSO16IS_SENSOR_HUB_MEM_BANK, 3 - LSM6DSO16IS_ISPU_MEM_BANK
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO16IS_Set_Mem_Bank(LSM6DSO16IS_Object_t *pObj, uint8_t Val)
{
  int32_t ret = LSM6DSO16IS_OK;
  lsm6dso16is_mem_bank_t reg;

  reg = (Val == 2U) ? LSM6DSO16IS_SENSOR_HUB_MEM_BANK
        : (Val == 3U) ? LSM6DSO16IS_ISPU_MEM_BANK
        :               LSM6DSO16IS_MAIN_MEM_BANK;

  if (lsm6dso16is_mem_bank_set(&(pObj->Ctx), reg) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  return ret;
}

/**
  * @}
  */

/** @defgroup LSM6DSO16IS_Private_Functions LSM6DSO16IS Private Functions
  * @{
  */

/**
  * @brief  Set the LSM6DSO16IS accelerometer sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LSM6DSO16IS_ACC_SetOutputDataRate_When_Enabled(LSM6DSO16IS_Object_t *pObj, float_t Odr)
{
  int32_t ret = LSM6DSO16IS_OK;
  lsm6dso16is_xl_data_rate_t new_odr;

  new_odr = (Odr <=   12.5f) ? LSM6DSO16IS_XL_ODR_AT_12Hz5_HP
            : (Odr <=   26.0f) ? LSM6DSO16IS_XL_ODR_AT_26H_HP
            : (Odr <=   52.0f) ? LSM6DSO16IS_XL_ODR_AT_52Hz_HP
            : (Odr <=  104.0f) ? LSM6DSO16IS_XL_ODR_AT_104Hz_HP
            : (Odr <=  208.0f) ? LSM6DSO16IS_XL_ODR_AT_208Hz_HP
            : (Odr <=  416.0f) ? LSM6DSO16IS_XL_ODR_AT_416Hz_HP
            : (Odr <=  833.0f) ? LSM6DSO16IS_XL_ODR_AT_833Hz_HP
            : (Odr <= 1667.0f) ? LSM6DSO16IS_XL_ODR_AT_1667Hz_HP
            : (Odr <= 3333.0f) ? LSM6DSO16IS_XL_ODR_AT_3333Hz_HP
            :                    LSM6DSO16IS_XL_ODR_AT_6667Hz_HP;

  /* Output data rate selection. */
  if (lsm6dso16is_xl_data_rate_set(&(pObj->Ctx), new_odr) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  return ret;
}

/**
  * @brief  Set the LSM6DSO16IS accelerometer sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LSM6DSO16IS_ACC_SetOutputDataRate_When_Disabled(LSM6DSO16IS_Object_t *pObj, float_t Odr)
{
  pObj->acc_odr = (Odr <=   12.5f) ? LSM6DSO16IS_XL_ODR_AT_12Hz5_HP
                  : (Odr <=   26.0f) ? LSM6DSO16IS_XL_ODR_AT_26H_HP
                  : (Odr <=   52.0f) ? LSM6DSO16IS_XL_ODR_AT_52Hz_HP
                  : (Odr <=  104.0f) ? LSM6DSO16IS_XL_ODR_AT_104Hz_HP
                  : (Odr <=  208.0f) ? LSM6DSO16IS_XL_ODR_AT_208Hz_HP
                  : (Odr <=  416.0f) ? LSM6DSO16IS_XL_ODR_AT_416Hz_HP
                  : (Odr <=  833.0f) ? LSM6DSO16IS_XL_ODR_AT_833Hz_HP
                  : (Odr <= 1667.0f) ? LSM6DSO16IS_XL_ODR_AT_1667Hz_HP
                  : (Odr <= 3333.0f) ? LSM6DSO16IS_XL_ODR_AT_3333Hz_HP
                  :                    LSM6DSO16IS_XL_ODR_AT_6667Hz_HP;

  return LSM6DSO16IS_OK;
}

/**
  * @brief  Set the LSM6DSO16IS gyroscope sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LSM6DSO16IS_GYRO_SetOutputDataRate_When_Enabled(LSM6DSO16IS_Object_t *pObj, float_t Odr)
{
  int32_t ret = LSM6DSO16IS_OK;
  lsm6dso16is_gy_data_rate_t new_odr;

  new_odr = (Odr <=   12.5f) ? LSM6DSO16IS_GY_ODR_AT_12Hz5_HP
            : (Odr <=   26.0f) ? LSM6DSO16IS_GY_ODR_AT_26H_HP
            : (Odr <=   52.0f) ? LSM6DSO16IS_GY_ODR_AT_52Hz_HP
            : (Odr <=  104.0f) ? LSM6DSO16IS_GY_ODR_AT_104Hz_HP
            : (Odr <=  208.0f) ? LSM6DSO16IS_GY_ODR_AT_208Hz_HP
            : (Odr <=  416.0f) ? LSM6DSO16IS_GY_ODR_AT_416Hz_HP
            : (Odr <=  833.0f) ? LSM6DSO16IS_GY_ODR_AT_833Hz_HP
            : (Odr <= 1667.0f) ? LSM6DSO16IS_GY_ODR_AT_1667Hz_HP
            : (Odr <= 3333.0f) ? LSM6DSO16IS_GY_ODR_AT_3333Hz_HP
            :                    LSM6DSO16IS_GY_ODR_AT_6667Hz_HP;

  /* Output data rate selection. */
  if (lsm6dso16is_gy_data_rate_set(&(pObj->Ctx), new_odr) != LSM6DSO16IS_OK)
  {
    ret = LSM6DSO16IS_ERROR;
  }

  return ret;
}

/**
  * @brief  Set the LSM6DSO16IS gyroscope sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LSM6DSO16IS_GYRO_SetOutputDataRate_When_Disabled(LSM6DSO16IS_Object_t *pObj, float_t Odr)
{
  pObj->gyro_odr = (Odr <=   12.5f) ? LSM6DSO16IS_GY_ODR_AT_12Hz5_HP
                   : (Odr <=   26.0f) ? LSM6DSO16IS_GY_ODR_AT_26H_HP
                   : (Odr <=   52.0f) ? LSM6DSO16IS_GY_ODR_AT_52Hz_HP
                   : (Odr <=  104.0f) ? LSM6DSO16IS_GY_ODR_AT_104Hz_HP
                   : (Odr <=  208.0f) ? LSM6DSO16IS_GY_ODR_AT_208Hz_HP
                   : (Odr <=  416.0f) ? LSM6DSO16IS_GY_ODR_AT_416Hz_HP
                   : (Odr <=  833.0f) ? LSM6DSO16IS_GY_ODR_AT_833Hz_HP
                   : (Odr <= 1667.0f) ? LSM6DSO16IS_GY_ODR_AT_1667Hz_HP
                   : (Odr <= 3333.0f) ? LSM6DSO16IS_GY_ODR_AT_3333Hz_HP
                   :                    LSM6DSO16IS_GY_ODR_AT_6667Hz_HP;

  return LSM6DSO16IS_OK;
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
  LSM6DSO16IS_Object_t *pObj = (LSM6DSO16IS_Object_t *)Handle;

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
  LSM6DSO16IS_Object_t *pObj = (LSM6DSO16IS_Object_t *)Handle;

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
