/**
  ******************************************************************************
  * @file    lsm6dsv16x.c
  * @author  MEMS Software Solutions Team
  * @brief   LSM6DSV16X driver file
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
#include "lsm6dsv16x.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup LSM6DSV16X LSM6DSV16X
  * @{
  */

/** @defgroup LSM6DSV16X_Exported_Variables LSM6DSV16X Exported Variables
  * @{
  */

LSM6DSV16X_CommonDrv_t LSM6DSV16X_COMMON_Driver =
{
  LSM6DSV16X_Init,
  LSM6DSV16X_DeInit,
  LSM6DSV16X_ReadID,
  LSM6DSV16X_GetCapabilities,
};

LSM6DSV16X_ACC_Drv_t LSM6DSV16X_ACC_Driver =
{
  LSM6DSV16X_ACC_Enable,
  LSM6DSV16X_ACC_Disable,
  LSM6DSV16X_ACC_GetSensitivity,
  LSM6DSV16X_ACC_GetOutputDataRate,
  LSM6DSV16X_ACC_SetOutputDataRate,
  LSM6DSV16X_ACC_GetFullScale,
  LSM6DSV16X_ACC_SetFullScale,
  LSM6DSV16X_ACC_GetAxes,
  LSM6DSV16X_ACC_GetAxesRaw,
};

LSM6DSV16X_GYRO_Drv_t LSM6DSV16X_GYRO_Driver =
{
  LSM6DSV16X_GYRO_Enable,
  LSM6DSV16X_GYRO_Disable,
  LSM6DSV16X_GYRO_GetSensitivity,
  LSM6DSV16X_GYRO_GetOutputDataRate,
  LSM6DSV16X_GYRO_SetOutputDataRate,
  LSM6DSV16X_GYRO_GetFullScale,
  LSM6DSV16X_GYRO_SetFullScale,
  LSM6DSV16X_GYRO_GetAxes,
  LSM6DSV16X_GYRO_GetAxesRaw,
};

/**
  * @}
  */

/** @defgroup LSM6DSV16X_Private_Function_Prototypes LSM6DSV16X Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t LSM6DSV16X_ACC_SetOutputDataRate_When_Enabled(LSM6DSV16X_Object_t *pObj, float Odr);
static int32_t LSM6DSV16X_ACC_SetOutputDataRate_When_Disabled(LSM6DSV16X_Object_t *pObj, float Odr);
static int32_t LSM6DSV16X_GYRO_SetOutputDataRate_When_Enabled(LSM6DSV16X_Object_t *pObj, float Odr);
static int32_t LSM6DSV16X_GYRO_SetOutputDataRate_When_Disabled(LSM6DSV16X_Object_t *pObj, float Odr);

/**
  * @}
  */

/** @defgroup LSM6DSV16X_Exported_Functions LSM6DSV16X Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_RegisterBusIO(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_IO_t *pIO)
{
  int32_t ret = LSM6DSV16X_OK;

  if (pObj == NULL)
  {
    ret = LSM6DSV16X_ERROR;
  }
  else
  {
    pObj->IO.Init       = pIO->Init;
    pObj->IO.DeInit     = pIO->DeInit;
    pObj->IO.BusType    = pIO->BusType;
    pObj->IO.Address    = pIO->Address;
    pObj->IO.WriteReg   = pIO->WriteReg;
    pObj->IO.ReadReg    = pIO->ReadReg;
    pObj->IO.GetTick    = pIO->GetTick;

    pObj->Ctx.read_reg  = ReadRegWrap;
    pObj->Ctx.write_reg = WriteRegWrap;
    pObj->Ctx.mdelay    = pIO->Delay;
    pObj->Ctx.handle    = pObj;

    if (pObj->IO.Init == NULL)
    {
      ret = LSM6DSV16X_ERROR;
    }
    else if (pObj->IO.Init() != LSM6DSV16X_OK)
    {
      ret = LSM6DSV16X_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == LSM6DSV16X_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x04;

          if (LSM6DSV16X_Write_Reg(pObj, LSM6DSV16X_CTRL3, data) != LSM6DSV16X_OK)
          {
            ret = LSM6DSV16X_ERROR;
          }

          if (LSM6DSV16X_Write_Reg(pObj, LSM6DSV16X_IF_CFG, data) != LSM6DSV16X_OK)
          {
            ret = LSM6DSV16X_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  Initialize the LSM6DSV16X sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_Init(LSM6DSV16X_Object_t *pObj)
{
  /* Enable register address automatically incremented during a multiple byte
  access with a serial interface */
  if (lsm6dsv16x_auto_increment_set(&(pObj->Ctx), PROPERTY_ENABLE) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Enable BDU */
  if (lsm6dsv16x_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* WAKE_UP mode selection */
  if (lsm6dsv16x_fifo_mode_set(&(pObj->Ctx), LSM6DSV16X_BYPASS_MODE) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Select default output data rate */
  pObj->acc_odr = LSM6DSV16X_ODR_AT_120Hz;

  /* Output data rate selection - power down */
  if (lsm6dsv16x_xl_data_rate_set(&(pObj->Ctx), LSM6DSV16X_ODR_OFF) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Full scale selection */
  if (lsm6dsv16x_xl_full_scale_set(&(pObj->Ctx), LSM6DSV16X_2g) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Select default output data rate */
  pObj->gyro_odr = LSM6DSV16X_ODR_AT_120Hz;

  /* Output data rate selection - power down */
  if (lsm6dsv16x_gy_data_rate_set(&(pObj->Ctx), LSM6DSV16X_ODR_OFF) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Full scale selection */
  if (lsm6dsv16x_gy_full_scale_set(&(pObj->Ctx), LSM6DSV16X_2000dps) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Enable Qvar functionality */
  lsm6dsv16x_ah_qvar_mode_t mode;
  mode.ah_qvar_en = 1;

  if (lsm6dsv16x_ah_qvar_mode_set(&(pObj->Ctx), mode) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  pObj->is_initialized = 1;

  return LSM6DSV16X_OK;
}

/**
  * @brief  Deinitialize the LSM6DSV16X sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_DeInit(LSM6DSV16X_Object_t *pObj)
{
  /* Disable the component */
  if (LSM6DSV16X_ACC_Disable(pObj) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  if (LSM6DSV16X_GYRO_Disable(pObj) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Reset output data rate */
  pObj->acc_odr = LSM6DSV16X_ODR_OFF;
  pObj->gyro_odr = LSM6DSV16X_ODR_OFF;

  /* Disable Qvar functionality */
  lsm6dsv16x_ah_qvar_mode_t mode;
  mode.ah_qvar_en = 0;

  if (lsm6dsv16x_ah_qvar_mode_set(&(pObj->Ctx), mode) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  pObj->is_initialized = 0;

  return LSM6DSV16X_OK;
}

/**
  * @brief  Read component ID
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ReadID(LSM6DSV16X_Object_t *pObj, uint8_t *Id)
{
  if (lsm6dsv16x_device_id_get(&(pObj->Ctx), Id) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Get LSM6DSV16X sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to LSM6DSV16X sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_GetCapabilities(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Acc          = 1;
  Capabilities->Gyro         = 1;
  Capabilities->Magneto      = 0;
  Capabilities->LowPower     = 1;
  Capabilities->GyroMaxFS    = 4000;
  Capabilities->AccMaxFS     = 16;
  Capabilities->MagMaxFS     = 0;
  Capabilities->GyroMaxOdr   = 7680.0f;
  Capabilities->AccMaxOdr    = 7680.0f;
  Capabilities->MagMaxOdr    = 0.0f;
  return LSM6DSV16X_OK;
}

/**
  * @brief  Enable the LSM6DSV16X accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Enable(LSM6DSV16X_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return LSM6DSV16X_OK;
  }

  /* Output data rate selection */
  if (lsm6dsv16x_xl_data_rate_set(&(pObj->Ctx), pObj->acc_odr) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return LSM6DSV16X_OK;
}

/**
  * @brief  Disable the LSM6DSV16X accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Disable(LSM6DSV16X_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return LSM6DSV16X_OK;
  }

  /* Get current output data rate */
  if (lsm6dsv16x_xl_data_rate_get(&(pObj->Ctx), &pObj->acc_odr) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Output data rate selection - power down */
  if (lsm6dsv16x_xl_data_rate_set(&(pObj->Ctx), LSM6DSV16X_ODR_OFF) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return LSM6DSV16X_OK;
}

/**
  * @brief  Get the LSM6DSV16X accelerometer sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_GetSensitivity(LSM6DSV16X_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = LSM6DSV16X_OK;
  lsm6dsv16x_xl_full_scale_t full_scale;

  /* Read actual full scale selection from sensor */
  if (lsm6dsv16x_xl_full_scale_get(&(pObj->Ctx), &full_scale) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Store the Sensitivity based on actual full scale */
  switch (full_scale)
  {
    case LSM6DSV16X_2g:
      *Sensitivity = LSM6DSV16X_ACC_SENSITIVITY_FS_2G;
      break;

    case LSM6DSV16X_4g:
      *Sensitivity = LSM6DSV16X_ACC_SENSITIVITY_FS_4G;
      break;

    case LSM6DSV16X_8g:
      *Sensitivity = LSM6DSV16X_ACC_SENSITIVITY_FS_8G;
      break;

    case LSM6DSV16X_16g:
      *Sensitivity = LSM6DSV16X_ACC_SENSITIVITY_FS_16G;
      break;

    default:
      ret = LSM6DSV16X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the LSM6DSV16X accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_GetOutputDataRate(LSM6DSV16X_Object_t *pObj, float *Odr)
{
  int32_t ret = LSM6DSV16X_OK;
  lsm6dsv16x_data_rate_t odr_low_level;

  /* Get current output data rate */
  if (lsm6dsv16x_xl_data_rate_get(&(pObj->Ctx), &odr_low_level) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  switch (odr_low_level)
  {
    case LSM6DSV16X_ODR_OFF:
      *Odr = 0.0f;
      break;

    case LSM6DSV16X_ODR_AT_1Hz875:
      *Odr = 1.875f;
      break;

    case LSM6DSV16X_ODR_AT_7Hz5:
      *Odr = 7.5f;
      break;

    case LSM6DSV16X_ODR_AT_15Hz:
      *Odr = 15.0f;
      break;

    case LSM6DSV16X_ODR_AT_30Hz:
      *Odr = 30.0f;
      break;

    case LSM6DSV16X_ODR_AT_60Hz:
      *Odr = 60.0f;
      break;

    case LSM6DSV16X_ODR_AT_120Hz:
      *Odr = 120.0f;
      break;

    case LSM6DSV16X_ODR_AT_240Hz:
      *Odr = 240.0f;
      break;

    case LSM6DSV16X_ODR_AT_480Hz:
      *Odr = 480.0f;
      break;

    case LSM6DSV16X_ODR_AT_960Hz:
      *Odr = 960.0f;
      break;

    case LSM6DSV16X_ODR_AT_1920Hz:
      *Odr = 1920.0f;
      break;

    case LSM6DSV16X_ODR_AT_3840Hz:
      *Odr = 3840.0f;
      break;

    case LSM6DSV16X_ODR_AT_7680Hz:
      *Odr = 7680.0f;
      break;

    default:
      ret = LSM6DSV16X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the LSM6DSV16X accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_SetOutputDataRate(LSM6DSV16X_Object_t *pObj, float Odr)
{
  if (pObj->acc_is_enabled == 1U)
  {
    return LSM6DSV16X_ACC_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return LSM6DSV16X_ACC_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Set the LSM6DSV16X accelerometer sensor output data rate with operating mode
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @param  Mode the accelerometer operating mode
  * @note   This function switches off the gyroscope if Ultra Low Power Mode is set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_SetOutputDataRate_With_Mode(LSM6DSV16X_Object_t *pObj, float Odr,
                                                   LSM6DSV16X_ACC_Operating_Mode_t Mode)
{
  switch (Mode)
  {
    case LSM6DSV16X_ACC_HIGH_PERFORMANCE_MODE:
    {
      if (lsm6dsv16x_xl_mode_set(&(pObj->Ctx), LSM6DSV16X_XL_HIGH_PERFORMANCE_MD) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 7.68kHz */
      Odr = (Odr <    7.5f) ?    7.5f
            : (Odr > 7680.0f) ? 7680.0f
            :                       Odr;
      break;
    }

    case LSM6DSV16X_ACC_HIGH_ACCURACY_MODE:
      return LSM6DSV16X_ERROR;

    case LSM6DSV16X_ACC_NORMAL_MODE:
    {
      if (lsm6dsv16x_xl_mode_set(&(pObj->Ctx), LSM6DSV16X_XL_NORMAL_MD) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 1.92kHz */
      Odr = (Odr <    7.5f) ?    7.5f
            : (Odr > 1920.0f) ? 1920.0f
            :                       Odr;
      break;
    }

    case LSM6DSV16X_ACC_LOW_POWER_MODE1:
    {
      if (lsm6dsv16x_xl_mode_set(&(pObj->Ctx), LSM6DSV16X_XL_LOW_POWER_2_AVG_MD) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      /* Valid ODR: 1.875Hz;  15Hz <= Odr <= 240kHz */
      Odr = (Odr ==  1.875f) ?      Odr
            : (Odr <  15.000f) ?  15.0f
            : (Odr > 240.000f) ? 240.0f
            :                       Odr;
      break;
    }

    case LSM6DSV16X_ACC_LOW_POWER_MODE2:
    {
      if (lsm6dsv16x_xl_mode_set(&(pObj->Ctx), LSM6DSV16X_XL_LOW_POWER_4_AVG_MD) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      /* Valid ODR: 1.875Hz;  15Hz <= Odr <= 240kHz */
      Odr = (Odr ==  1.875f) ?      Odr
            : (Odr <  15.000f) ?  15.0f
            : (Odr > 240.000f) ? 240.0f
            :                       Odr;
      break;
    }

    case LSM6DSV16X_ACC_LOW_POWER_MODE3:
    {
      if (lsm6dsv16x_xl_mode_set(&(pObj->Ctx), LSM6DSV16X_XL_LOW_POWER_8_AVG_MD) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      /* Valid ODR: 1.875Hz;  15Hz <= Odr <= 240kHz */
      Odr = (Odr ==  1.875f) ?      Odr
            : (Odr <  15.000f) ?  15.0f
            : (Odr > 240.000f) ? 240.0f
            :                       Odr;
      break;
    }

    default:
      return LSM6DSV16X_ERROR;
  }

  if (pObj->acc_is_enabled == 1U)
  {
    return LSM6DSV16X_ACC_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return LSM6DSV16X_ACC_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the LSM6DSV16X accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_GetFullScale(LSM6DSV16X_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = LSM6DSV16X_OK;
  lsm6dsv16x_xl_full_scale_t fs_low_level;

  /* Read actual full scale selection from sensor */
  if (lsm6dsv16x_xl_full_scale_get(&(pObj->Ctx), &fs_low_level) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  switch (fs_low_level)
  {
    case LSM6DSV16X_2g:
      *FullScale =  2;
      break;

    case LSM6DSV16X_4g:
      *FullScale =  4;
      break;

    case LSM6DSV16X_8g:
      *FullScale =  8;
      break;

    case LSM6DSV16X_16g:
      *FullScale = 16;
      break;

    default:
      ret = LSM6DSV16X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the LSM6DSV16X accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_SetFullScale(LSM6DSV16X_Object_t *pObj, int32_t FullScale)
{
  lsm6dsv16x_xl_full_scale_t new_fs;

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file statical analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  new_fs = (FullScale <= 2) ? LSM6DSV16X_2g
           : (FullScale <= 4) ? LSM6DSV16X_4g
           : (FullScale <= 8) ? LSM6DSV16X_8g
           :                    LSM6DSV16X_16g;

  if (lsm6dsv16x_xl_full_scale_set(&(pObj->Ctx), new_fs) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Get the LSM6DSV16X accelerometer sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_GetAxesRaw(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_AxesRaw_t *Value)
{
  lsm6dsv16x_axis3bit16_t data_raw;

  /* Read raw data values */
  if (lsm6dsv16x_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Format the data */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return LSM6DSV16X_OK;
}

/**
  * @brief  Get the LSM6DSV16X accelerometer sensor axes
  * @param  pObj the device pObj
  * @param  Acceleration pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_GetAxes(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_Axes_t *Acceleration)
{
  lsm6dsv16x_axis3bit16_t data_raw;
  float sensitivity = 0.0f;

  /* Read raw data values */
  if (lsm6dsv16x_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Get LSM6DSV16X actual sensitivity */
  if (LSM6DSV16X_ACC_GetSensitivity(pObj, &sensitivity) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Calculate the data */
  Acceleration->x = (int32_t)((float)((float)data_raw.i16bit[0] * sensitivity));
  Acceleration->y = (int32_t)((float)((float)data_raw.i16bit[1] * sensitivity));
  Acceleration->z = (int32_t)((float)((float)data_raw.i16bit[2] * sensitivity));

  return LSM6DSV16X_OK;
}

/**
  * @brief  Get the status of all hardware events
  * @param  pObj the device pObj
  * @param  Status the status of all hardware events
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Get_Event_Status(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_Event_Status_t *Status)
{

  lsm6dsv16x_emb_func_status_t emb_func_status;
  lsm6dsv16x_wake_up_src_t wake_up_src;
  lsm6dsv16x_tap_src_t tap_src;
  lsm6dsv16x_d6d_src_t d6d_src;
  lsm6dsv16x_emb_func_src_t func_src;

  lsm6dsv16x_md1_cfg_t md1_cfg;
  lsm6dsv16x_md2_cfg_t md2_cfg;

  lsm6dsv16x_emb_func_int1_t int1_ctrl;
  lsm6dsv16x_emb_func_int2_t int2_ctrl;


  (void)memset((void *)Status, 0x0, sizeof(LSM6DSV16X_Event_Status_t));

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_WAKE_UP_SRC, (uint8_t *)&wake_up_src, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_TAP_SRC, (uint8_t *)&tap_src, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_D6D_SRC, (uint8_t *)&d6d_src, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  if (lsm6dsv16x_mem_bank_set(&(pObj->Ctx), LSM6DSV16X_EMBED_FUNC_MEM_BANK) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_SRC, (uint8_t *)&func_src, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_INT1, (uint8_t *)&int1_ctrl, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_INT2, (uint8_t *)&int2_ctrl, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_STATUS, (uint8_t *)&emb_func_status, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  if (lsm6dsv16x_mem_bank_set(&(pObj->Ctx), LSM6DSV16X_MAIN_MEM_BANK) != 0)
  {
    return LSM6DSV16X_ERROR;
  }



  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&md1_cfg, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&md2_cfg, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
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

  if (int1_ctrl.int1_step_detector == 1U || int2_ctrl.int2_step_detector == 1U)
  {
    if (func_src.step_detected == 1U)
    {
      Status->StepStatus = 1;
    }
  }

  if ((int1_ctrl.int1_tilt == 1U) || (int2_ctrl.int2_tilt == 1U))
  {
    if (emb_func_status.is_tilt == 1U)
    {
      Status->TiltStatus = 1;
    }
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Enable free fall detection
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Enable_Free_Fall_Detection(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_SensorIntPin_t IntPin)
{
  int32_t ret = LSM6DSV16X_OK;
  lsm6dsv16x_md1_cfg_t val1;
  lsm6dsv16x_md2_cfg_t val2;
  lsm6dsv16x_functions_enable_t functions_enable;

  /* Output Data Rate selection */
  if (LSM6DSV16X_ACC_SetOutputDataRate(pObj, 480.0f) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Full scale selection */
  if (LSM6DSV16X_ACC_SetFullScale(pObj, 2) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /*  Set free fall duration */
  if (LSM6DSV16X_ACC_Set_Free_Fall_Duration(pObj, 3) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Set free fall threshold */
  if (LSM6DSV16X_ACC_Set_Free_Fall_Threshold(pObj, 3) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Enable free fall event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case LSM6DSV16X_INT1_PIN:
      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      val1.int1_ff = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      functions_enable.interrupts_enable = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }
      break;

    case LSM6DSV16X_INT2_PIN:
      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      val2.int2_ff = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      functions_enable.interrupts_enable = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }
      break;

    default:
      ret = LSM6DSV16X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Disable free fall detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Disable_Free_Fall_Detection(LSM6DSV16X_Object_t *pObj)
{
  lsm6dsv16x_md1_cfg_t val1;
  lsm6dsv16x_md2_cfg_t val2;

  /* Disable free fall event on both INT1 and INT2 pins */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  val1.int1_ff = PROPERTY_DISABLE;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  val2.int2_ff = PROPERTY_DISABLE;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Reset free fall threshold */
  if (LSM6DSV16X_ACC_Set_Free_Fall_Threshold(pObj, 0) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Reset free fall duration */
  if (LSM6DSV16X_ACC_Set_Free_Fall_Duration(pObj, 0) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Set free fall threshold
  * @param  pObj the device pObj
  * @param  Threshold free fall detection threshold
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Set_Free_Fall_Threshold(LSM6DSV16X_Object_t *pObj, uint8_t Threshold)
{
  lsm6dsv16x_ff_thresholds_t val;
  switch (Threshold)
  {
    case LSM6DSV16X_156_mg:
      val = LSM6DSV16X_156_mg;
      break;

    case LSM6DSV16X_219_mg:
      val = LSM6DSV16X_219_mg;
      break;

    case LSM6DSV16X_250_mg:
      val = LSM6DSV16X_250_mg;
      break;

    case LSM6DSV16X_312_mg:
      val = LSM6DSV16X_312_mg;
      break;

    case LSM6DSV16X_344_mg:
      val = LSM6DSV16X_344_mg;
      break;

    case LSM6DSV16X_406_mg:
      val = LSM6DSV16X_406_mg;
      break;

    case LSM6DSV16X_469_mg:
      val = LSM6DSV16X_469_mg;
      break;

    case LSM6DSV16X_500_mg:
      val = LSM6DSV16X_500_mg;
      break;

    default:
      val = LSM6DSV16X_156_mg;
      break;
  }

  /* Set free fall threshold */
  if (lsm6dsv16x_ff_thresholds_set(&(pObj->Ctx), val) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Set free fall duration
  * @param  pObj the device pObj
  * @param  Duration free fall detection duration
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Set_Free_Fall_Duration(LSM6DSV16X_Object_t *pObj, uint8_t Duration)
{
  /* Set free fall duration */
  if (lsm6dsv16x_ff_time_windows_set(&(pObj->Ctx), Duration) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }
  return LSM6DSV16X_OK;
}

/**
  * @brief  Enable tilt detection
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Enable_Tilt_Detection(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_SensorIntPin_t IntPin)
{
  int32_t ret = LSM6DSV16X_OK;
  lsm6dsv16x_md1_cfg_t val1;
  lsm6dsv16x_md2_cfg_t val2;

  lsm6dsv16x_emb_func_en_a_t emb_func_en_a;
  lsm6dsv16x_emb_func_int1_t emb_func_int1;
  lsm6dsv16x_emb_func_int2_t emb_func_int2;

  /* Output Data Rate selection */
  if (LSM6DSV16X_ACC_SetOutputDataRate(pObj, 30.0f) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Full scale selection */
  if (LSM6DSV16X_ACC_SetFullScale(pObj, 2) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  switch (IntPin)
  {
    case LSM6DSV16X_INT1_PIN:
      /* Enable access to embedded functions registers */
      if (lsm6dsv16x_mem_bank_set(&(pObj->Ctx), LSM6DSV16X_EMBED_FUNC_MEM_BANK) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      /* Enable tilt detection */
      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      emb_func_en_a.tilt_en = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      /* Tilt interrupt driven to INT1 pin */
      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      emb_func_int1.int1_tilt = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      /* Disable access to embedded functions registers */
      if (lsm6dsv16x_mem_bank_set(&(pObj->Ctx), LSM6DSV16X_MAIN_MEM_BANK) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      /* Enable routing the embedded functions interrupt */
      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      val1.int1_emb_func = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }
      break;

    case LSM6DSV16X_INT2_PIN:
      /* Enable access to embedded functions registers */
      if (lsm6dsv16x_mem_bank_set(&(pObj->Ctx), LSM6DSV16X_EMBED_FUNC_MEM_BANK) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      /* Enable tilt detection */
      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      emb_func_en_a.tilt_en = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      /* Tilt interrupt driven to INT2 pin */
      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      emb_func_int2.int2_tilt = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      /* Disable access to embedded functions registers */
      if (lsm6dsv16x_mem_bank_set(&(pObj->Ctx), LSM6DSV16X_MAIN_MEM_BANK) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      /* Enable routing the embedded functions interrupt */
      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      val2.int2_emb_func = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }
      break;

    default:
      ret = LSM6DSV16X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Disable tilt detection
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Disable_Tilt_Detection(LSM6DSV16X_Object_t *pObj)
{
  int32_t ret = LSM6DSV16X_OK;
  lsm6dsv16x_md1_cfg_t val1;
  lsm6dsv16x_md2_cfg_t val2;

  lsm6dsv16x_emb_func_en_a_t emb_func_en_a;
  lsm6dsv16x_emb_func_int1_t emb_func_int1;
  lsm6dsv16x_emb_func_int2_t emb_func_int2;

  /* Disable emb func event on either INT1 or INT2 pin */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  val1.int1_emb_func = PROPERTY_DISABLE;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  val2.int2_emb_func = PROPERTY_DISABLE;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Enable access to embedded functions registers */
  if (lsm6dsv16x_mem_bank_set(&(pObj->Ctx), LSM6DSV16X_EMBED_FUNC_MEM_BANK) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Disable tilt detection */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  emb_func_en_a.tilt_en = PROPERTY_DISABLE;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Reset interrupt driven to INT1 pin */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  emb_func_int1.int1_tilt = PROPERTY_DISABLE;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Reset interrupt driven to INT2 pin */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  emb_func_int2.int2_tilt = PROPERTY_DISABLE;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Disable access to embedded functions registers */
  if (lsm6dsv16x_mem_bank_set(&(pObj->Ctx), LSM6DSV16X_MAIN_MEM_BANK) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return ret;
}

/**
  * @brief  Enable pedometer
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Enable_Pedometer(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_SensorIntPin_t IntPin)
{
  int32_t ret = LSM6DSV16X_OK;
  lsm6dsv16x_stpcnt_mode_t mode;
  lsm6dsv16x_md1_cfg_t val1;
  lsm6dsv16x_md2_cfg_t val2;
  lsm6dsv16x_emb_func_int1_t emb_func_int1;
  lsm6dsv16x_emb_func_int2_t emb_func_int2;

  /* Output Data Rate selection */
  if (LSM6DSV16X_ACC_SetOutputDataRate(pObj, 30.0f) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Full scale selection */
  if (LSM6DSV16X_ACC_SetFullScale(pObj, 8) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  if (lsm6dsv16x_stpcnt_mode_get(&(pObj->Ctx), &mode) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Enable pedometer algorithm */
  mode.step_counter_enable = PROPERTY_ENABLE;
  mode.false_step_rej = PROPERTY_DISABLE;

  /* Turn on embedded features */
  if (lsm6dsv16x_stpcnt_mode_set(&(pObj->Ctx), mode) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Enable free fall event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case LSM6DSV16X_INT1_PIN:
      /* Enable access to embedded functions registers */
      if (lsm6dsv16x_mem_bank_set(&(pObj->Ctx), LSM6DSV16X_EMBED_FUNC_MEM_BANK) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      /* Step detector interrupt driven to INT1 pin */
      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      emb_func_int1.int1_step_detector = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      /* Disable access to embedded functions registers */
      if (lsm6dsv16x_mem_bank_set(&(pObj->Ctx), LSM6DSV16X_MAIN_MEM_BANK) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      val1.int1_emb_func = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }
      break;

    case LSM6DSV16X_INT2_PIN:
      /* Enable access to embedded functions registers */
      if (lsm6dsv16x_mem_bank_set(&(pObj->Ctx), LSM6DSV16X_EMBED_FUNC_MEM_BANK) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      /* Step detector interrupt driven to INT1 pin */
      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      emb_func_int2.int2_step_detector = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      /* Disable access to embedded functions registers */
      if (lsm6dsv16x_mem_bank_set(&(pObj->Ctx), LSM6DSV16X_MAIN_MEM_BANK) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      val2.int2_emb_func = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }
      break;

    default:
      ret = LSM6DSV16X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Disable pedometer
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Disable_Pedometer(LSM6DSV16X_Object_t *pObj)
{
  lsm6dsv16x_md1_cfg_t val1;
  lsm6dsv16x_md2_cfg_t val2;

  lsm6dsv16x_emb_func_int1_t emb_func_int1;
  lsm6dsv16x_emb_func_int2_t emb_func_int2;

  lsm6dsv16x_stpcnt_mode_t mode;


  if (lsm6dsv16x_stpcnt_mode_get(&(pObj->Ctx), &mode) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Enable pedometer algorithm */
  mode.step_counter_enable = PROPERTY_DISABLE;
  mode.false_step_rej = PROPERTY_DISABLE;

  /* Turn off embedded features */
  if (lsm6dsv16x_stpcnt_mode_set(&(pObj->Ctx), mode) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Disable emb func event on either INT1 or INT2 pin */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  val1.int1_emb_func = PROPERTY_DISABLE;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  val2.int2_emb_func = PROPERTY_DISABLE;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Enable access to embedded functions registers */
  if (lsm6dsv16x_mem_bank_set(&(pObj->Ctx), LSM6DSV16X_EMBED_FUNC_MEM_BANK) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Reset interrupt driven to INT1 pin */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  emb_func_int1.int1_step_detector = PROPERTY_DISABLE;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Reset interrupt driven to INT2 pin */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  emb_func_int2.int2_step_detector = PROPERTY_DISABLE;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Disable access to embedded functions registers */
  if (lsm6dsv16x_mem_bank_set(&(pObj->Ctx), LSM6DSV16X_MAIN_MEM_BANK) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }


  return LSM6DSV16X_OK;
}

/**
  * @brief  Get step count
  * @param  pObj the device pObj
  * @param  StepCount step counter
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Get_Step_Count(LSM6DSV16X_Object_t *pObj, uint16_t *StepCount)
{
  if (lsm6dsv16x_stpcnt_steps_get(&(pObj->Ctx), StepCount) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Enable step counter reset
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Step_Counter_Reset(LSM6DSV16X_Object_t *pObj)
{
  if (lsm6dsv16x_stpcnt_rst_step_set(&(pObj->Ctx), PROPERTY_ENABLE) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}


/**
  * @brief  Enable wake up detection
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Enable_Wake_Up_Detection(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_SensorIntPin_t IntPin)
{
  int32_t ret = LSM6DSV16X_OK;
  lsm6dsv16x_md1_cfg_t val1;
  lsm6dsv16x_md2_cfg_t val2;
  lsm6dsv16x_functions_enable_t functions_enable;

  /* Output Data Rate selection */
  if (LSM6DSV16X_ACC_SetOutputDataRate(pObj, 480.0f) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Full scale selection */
  if (LSM6DSV16X_ACC_SetFullScale(pObj, 2) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Set wake-up threshold */
  if (LSM6DSV16X_ACC_Set_Wake_Up_Threshold(pObj, 63) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Set wake-up durantion */
  if (LSM6DSV16X_ACC_Set_Wake_Up_Duration(pObj, 0) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Enable wake up event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case LSM6DSV16X_INT1_PIN:
      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      val1.int1_wu = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      functions_enable.interrupts_enable = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }
      break;

    case LSM6DSV16X_INT2_PIN:
      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      val2.int2_wu = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      functions_enable.interrupts_enable = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }
      break;

    default:
      ret = LSM6DSV16X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Disable wake up detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Disable_Wake_Up_Detection(LSM6DSV16X_Object_t *pObj)
{
  lsm6dsv16x_md1_cfg_t val1;
  lsm6dsv16x_md2_cfg_t val2;

  /* Disable wake up event on both INT1 and INT2 pins */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  val1.int1_wu = PROPERTY_DISABLE;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  val2.int2_wu = PROPERTY_DISABLE;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Reset wake-up threshold */
  if (LSM6DSV16X_ACC_Set_Wake_Up_Threshold(pObj, 0) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Reset wake-up durantion */
  if (LSM6DSV16X_ACC_Set_Wake_Up_Duration(pObj, 0) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Set wake up threshold
  * @param  pObj the device pObj
  * @param  Threshold wake up detection threshold
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Set_Wake_Up_Threshold(LSM6DSV16X_Object_t *pObj, uint32_t Threshold)
{
  lsm6dsv16x_act_thresholds_t wake_up_ths;

  if (lsm6dsv16x_act_thresholds_get(&(pObj->Ctx), &wake_up_ths) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  wake_up_ths.threshold = Threshold;

  if (lsm6dsv16x_act_thresholds_set(&(pObj->Ctx), &wake_up_ths) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Set wake up duration
  * @param  pObj the device pObj
  * @param  Duration wake up detection duration
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Set_Wake_Up_Duration(LSM6DSV16X_Object_t *pObj, uint8_t Duration)
{
  lsm6dsv16x_act_wkup_time_windows_t dur_t;

  if (lsm6dsv16x_act_wkup_time_windows_get(&(pObj->Ctx), &dur_t) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  dur_t.shock = Duration;

  if (lsm6dsv16x_act_wkup_time_windows_set(&(pObj->Ctx), dur_t) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Enable single tap detection
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Enable_Single_Tap_Detection(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_SensorIntPin_t IntPin)
{
  int32_t ret = LSM6DSV16X_OK;
  lsm6dsv16x_md1_cfg_t val1;
  lsm6dsv16x_md2_cfg_t val2;
  lsm6dsv16x_functions_enable_t functions_enable;

  lsm6dsv16x_tap_dur_t tap_dur;
  lsm6dsv16x_tap_cfg0_t tap_cfg0;
  lsm6dsv16x_tap_ths_6d_t tap_ths_6d;

  /* Output Data Rate selection */
  if (LSM6DSV16X_ACC_SetOutputDataRate(pObj, 480.0f) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Full scale selection */
  if (LSM6DSV16X_ACC_SetFullScale(pObj, 8) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Enable tap detection on Z-axis */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_TAP_CFG0, (uint8_t *)&tap_cfg0, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  tap_cfg0.tap_z_en = 0x01U;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_TAP_CFG0, (uint8_t *)&tap_cfg0, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Set Z-axis threshold */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  tap_ths_6d.tap_ths_z = 0x2U;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Set quiet and shock time windows */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_TAP_DUR, (uint8_t *)&tap_dur, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  tap_dur.quiet = (uint8_t)0x02U;
  tap_dur.shock = (uint8_t)0x01U;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_TAP_DUR, (uint8_t *)&tap_dur, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Set tap mode */
  if (lsm6dsv16x_tap_mode_set(&(pObj->Ctx), LSM6DSV16X_ONLY_SINGLE) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Enable single tap event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case LSM6DSV16X_INT1_PIN:
      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      val1.int1_single_tap = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      functions_enable.interrupts_enable = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }
      break;

    case LSM6DSV16X_INT2_PIN:
      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      val2.int2_single_tap = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      functions_enable.interrupts_enable = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }
      break;

    default:
      ret = LSM6DSV16X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Disable single tap detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Disable_Single_Tap_Detection(LSM6DSV16X_Object_t *pObj)
{
  lsm6dsv16x_md1_cfg_t val1;
  lsm6dsv16x_md2_cfg_t val2;

  lsm6dsv16x_tap_dur_t tap_dur;
  lsm6dsv16x_tap_cfg0_t tap_cfg0;
  lsm6dsv16x_tap_ths_6d_t tap_ths_6d;


  /* Disable single tap event on both INT1 and INT2 pins */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  val1.int1_single_tap = PROPERTY_DISABLE;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  val2.int2_single_tap = PROPERTY_DISABLE;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Disable tap detection on Z-axis */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_TAP_CFG0, (uint8_t *)&tap_cfg0, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  tap_cfg0.tap_z_en = 0x0U;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_TAP_CFG0, (uint8_t *)&tap_cfg0, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Reset Z-axis threshold */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  tap_ths_6d.tap_ths_z = 0x0U;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Reset quiet and shock time windows */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_TAP_DUR, (uint8_t *)&tap_dur, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  tap_dur.quiet = (uint8_t)0x0U;
  tap_dur.shock = (uint8_t)0x0U;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_TAP_DUR, (uint8_t *)&tap_dur, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Enable double tap detection
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Enable_Double_Tap_Detection(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_SensorIntPin_t IntPin)
{
  int32_t ret = LSM6DSV16X_OK;
  lsm6dsv16x_md1_cfg_t val1;
  lsm6dsv16x_md2_cfg_t val2;
  lsm6dsv16x_functions_enable_t functions_enable;

  lsm6dsv16x_tap_dur_t tap_dur;
  lsm6dsv16x_tap_cfg0_t tap_cfg0;
  lsm6dsv16x_tap_ths_6d_t tap_ths_6d;


  /* Enable tap detection on Z-axis */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_TAP_CFG0, (uint8_t *)&tap_cfg0, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  tap_cfg0.tap_z_en = 0x01U;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_TAP_CFG0, (uint8_t *)&tap_cfg0, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Set Z-axis threshold */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  tap_ths_6d.tap_ths_z = 0x03U;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Set quiet shock and duration */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_TAP_DUR, (uint8_t *)&tap_dur, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  tap_dur.dur = (uint8_t)0x03U;
  tap_dur.quiet = (uint8_t)0x02U;
  tap_dur.shock = (uint8_t)0x02U;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_TAP_DUR, (uint8_t *)&tap_dur, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Set tap mode */
  if (lsm6dsv16x_tap_mode_set(&(pObj->Ctx), LSM6DSV16X_BOTH_SINGLE_DOUBLE) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Output Data Rate selection */
  if (LSM6DSV16X_ACC_SetOutputDataRate(pObj, 480.0f) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Full scale selection */
  if (LSM6DSV16X_ACC_SetFullScale(pObj, 8) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Enable double tap event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case LSM6DSV16X_INT1_PIN:
      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      val1.int1_double_tap = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      functions_enable.interrupts_enable = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }
      break;

    case LSM6DSV16X_INT2_PIN:
      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      val2.int2_double_tap = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      functions_enable.interrupts_enable = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }
      break;

    default:
      ret = LSM6DSV16X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Disable double tap detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Disable_Double_Tap_Detection(LSM6DSV16X_Object_t *pObj)
{
  lsm6dsv16x_md1_cfg_t val1;
  lsm6dsv16x_md2_cfg_t val2;

  lsm6dsv16x_tap_dur_t tap_dur;
  lsm6dsv16x_tap_cfg0_t tap_cfg0;
  lsm6dsv16x_tap_ths_6d_t tap_ths_6d;

  /* Disable double tap event on both INT1 and INT2 pins */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  val1.int1_ff = PROPERTY_DISABLE;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  val2.int2_ff = PROPERTY_DISABLE;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Disable tap detection on Z-axis */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_TAP_CFG0, (uint8_t *)&tap_cfg0, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  tap_cfg0.tap_z_en = 0x0U;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_TAP_CFG0, (uint8_t *)&tap_cfg0, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Reset Z-axis threshold */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  tap_ths_6d.tap_ths_z = 0x0U;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Reset quiet shock and duration */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_TAP_DUR, (uint8_t *)&tap_dur, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  tap_dur.dur = (uint8_t)0x0U;
  tap_dur.quiet = (uint8_t)0x0U;
  tap_dur.shock = (uint8_t)0x0U;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_TAP_DUR, (uint8_t *)&tap_dur, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Set tap mode */
  if (lsm6dsv16x_tap_mode_set(&(pObj->Ctx), LSM6DSV16X_ONLY_SINGLE) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }


  return LSM6DSV16X_OK;
}

/**
  * @brief  Set tap threshold
  * @param  pObj the device pObj
  * @param  Threshold tap threshold
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Set_Tap_Threshold(LSM6DSV16X_Object_t *pObj, uint8_t Threshold)
{
  lsm6dsv16x_tap_ths_6d_t tap_ths_6d;

  /* Set Z-axis threshold */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  tap_ths_6d.tap_ths_z = Threshold;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Set tap duration time
  * @param  pObj the device pObj
  * @param  Time shock duration time
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Set_Tap_Shock_Time(LSM6DSV16X_Object_t *pObj, uint8_t Time)
{
  lsm6dsv16x_tap_dur_t tap_dur;

  /* Set shock time */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_TAP_DUR, (uint8_t *)&tap_dur, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  tap_dur.shock = Time;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_TAP_DUR, (uint8_t *)&tap_dur, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }
  return LSM6DSV16X_OK;
}

/**
  * @brief  Set tap duration time
  * @param  pObj the device pObj
  * @param  Time tap quiet time
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Set_Tap_Quiet_Time(LSM6DSV16X_Object_t *pObj, uint8_t Time)
{
  lsm6dsv16x_tap_dur_t tap_dur;

  /* Set quiet time */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_TAP_DUR, (uint8_t *)&tap_dur, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  tap_dur.quiet = Time;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_TAP_DUR, (uint8_t *)&tap_dur, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }
  return LSM6DSV16X_OK;
}



/**
  * @brief  Set tap duration time
  * @param  pObj the device pObj
  * @param  Time tap duration time
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Set_Tap_Duration_Time(LSM6DSV16X_Object_t *pObj, uint8_t Time)
{
  lsm6dsv16x_tap_dur_t tap_dur;

  /* Set duration time */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_TAP_DUR, (uint8_t *)&tap_dur, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  tap_dur.dur = Time;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_TAP_DUR, (uint8_t *)&tap_dur, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }
  return LSM6DSV16X_OK;
}


/**
  * @brief  Enable 6d orientation
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Enable_6D_Orientation(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_SensorIntPin_t IntPin)
{
  int32_t ret = LSM6DSV16X_OK;
  lsm6dsv16x_md1_cfg_t val1;
  lsm6dsv16x_md2_cfg_t val2;
  lsm6dsv16x_functions_enable_t functions_enable;

  /* Output Data Rate selection */
  if (LSM6DSV16X_ACC_SetOutputDataRate(pObj, 480.0f) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Full scale selection */
  if (LSM6DSV16X_ACC_SetFullScale(pObj, 2) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Threshold selection */
  if (LSM6DSV16X_ACC_Set_6D_Orientation_Threshold(pObj, 2) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Enable 6D orientation event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case LSM6DSV16X_INT1_PIN:
      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      val1.int1_6d = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      functions_enable.interrupts_enable = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }
      break;

    case LSM6DSV16X_INT2_PIN:
      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      val2.int2_6d = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      functions_enable.interrupts_enable = PROPERTY_ENABLE;

      if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }
      break;

    default:
      ret = LSM6DSV16X_ERROR;
      break;
  }

  return ret;

}


/**
  * @brief  Disable 6D orientation detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Disable_6D_Orientation(LSM6DSV16X_Object_t *pObj)
{
  lsm6dsv16x_md1_cfg_t val1;
  lsm6dsv16x_md2_cfg_t val2;

  /* Reset threshold */
  if (LSM6DSV16X_ACC_Set_6D_Orientation_Threshold(pObj, 0) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Disable 6D orientation event on both INT1 and INT2 pins */
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  val1.int1_6d = PROPERTY_DISABLE;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD1_CFG, (uint8_t *)&val1, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  val2.int2_6d = PROPERTY_DISABLE;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_MD2_CFG, (uint8_t *)&val2, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Set 6D orientation threshold
  * @param  pObj the device pObj
  * @param  Threshold free fall detection threshold
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Set_6D_Orientation_Threshold(LSM6DSV16X_Object_t *pObj, uint8_t Threshold)
{
  int32_t ret = LSM6DSV16X_OK;
  lsm6dsv16x_6d_threshold_t newThreshold = LSM6DSV16X_DEG_80;

  switch (Threshold)
  {
    case 0:
      newThreshold = LSM6DSV16X_DEG_80;
      break;
    case 1:
      newThreshold = LSM6DSV16X_DEG_70;
      break;
    case 2:
      newThreshold = LSM6DSV16X_DEG_60;
      break;
    case 3:
      newThreshold = LSM6DSV16X_DEG_50;
      break;
    default:
      ret = LSM6DSV16X_ERROR;
      break;
  }

  if (ret == LSM6DSV16X_ERROR)
  {
    return LSM6DSV16X_ERROR;
  }

  if (lsm6dsv16x_6d_threshold_set(&(pObj->Ctx), newThreshold) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;

}

/**
  * @brief  Get the status of XLow orientation
  * @param  pObj the device pObj
  * @param  XLow the status of XLow orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Get_6D_Orientation_XL(LSM6DSV16X_Object_t *pObj, uint8_t *XLow)
{
  lsm6dsv16x_d6d_src_t data;

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_D6D_SRC, (uint8_t *)&data, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  *XLow = data.xl;

  return LSM6DSV16X_OK;
}

/**
  * @brief  Get the status of XHigh orientation
  * @param  pObj the device pObj
  * @param  XHigh the status of XHigh orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Get_6D_Orientation_XH(LSM6DSV16X_Object_t *pObj, uint8_t *XHigh)
{
  lsm6dsv16x_d6d_src_t data;

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_D6D_SRC, (uint8_t *)&data, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  *XHigh = data.xh;

  return LSM6DSV16X_OK;
}

/**
  * @brief  Get the status of YLow orientation
  * @param  pObj the device pObj
  * @param  YLow the status of YLow orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Get_6D_Orientation_YL(LSM6DSV16X_Object_t *pObj, uint8_t *YLow)
{
  lsm6dsv16x_d6d_src_t data;

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_D6D_SRC, (uint8_t *)&data, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  *YLow = data.yl;

  return LSM6DSV16X_OK;
}

/**
  * @brief  Get the status of YHigh orientation
  * @param  pObj the device pObj
  * @param  YHigh the status of YHigh orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Get_6D_Orientation_YH(LSM6DSV16X_Object_t *pObj, uint8_t *YHigh)
{
  lsm6dsv16x_d6d_src_t data;

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_D6D_SRC, (uint8_t *)&data, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  *YHigh = data.yh;

  return LSM6DSV16X_OK;
}

/**
  * @brief  Get the status of ZLow orientation
  * @param  pObj the device pObj
  * @param  ZLow the status of ZLow orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Get_6D_Orientation_ZL(LSM6DSV16X_Object_t *pObj, uint8_t *ZLow)
{
  lsm6dsv16x_d6d_src_t data;

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_D6D_SRC, (uint8_t *)&data, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  *ZLow = data.zl;

  return LSM6DSV16X_OK;
}

/**
  * @brief  Get the status of ZHigh orientation
  * @param  pObj the device pObj
  * @param  ZHigh the status of ZHigh orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Get_6D_Orientation_ZH(LSM6DSV16X_Object_t *pObj, uint8_t *ZHigh)
{
  lsm6dsv16x_d6d_src_t data;

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_D6D_SRC, (uint8_t *)&data, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  *ZHigh = data.zh;

  return LSM6DSV16X_OK;
}


/**
  * @brief  Get the LSM6DSV16X FIFO number of samples
  * @param  pObj the device pObj
  * @param  NumSamples number of samples
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_FIFO_Get_Num_Samples(LSM6DSV16X_Object_t *pObj, uint16_t *NumSamples)
{
  lsm6dsv16x_fifo_status_t val;

  if (lsm6dsv16x_fifo_status_get(&(pObj->Ctx), &val) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  *NumSamples = val.fifo_level;

  return LSM6DSV16X_OK;

}

/**
  * @brief  Get the LSM6DSV16X FIFO full status
  * @param  pObj the device pObj
  * @param  Status FIFO full status
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_FIFO_Get_Full_Status(LSM6DSV16X_Object_t *pObj, uint8_t *Status)
{
  lsm6dsv16x_fifo_status_t val;

  if (lsm6dsv16x_fifo_status_get(&(pObj->Ctx), &val) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  *Status = val.fifo_full;

  return LSM6DSV16X_OK;
}

/**
  * @brief  Set the LSM6DSV16X FIFO full interrupt on INT1 pin
  * @param  pObj the device pObj
  * @param  Status FIFO full interrupt on INT1 pin status
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_FIFO_Set_INT1_FIFO_Full(LSM6DSV16X_Object_t *pObj, uint8_t Status)
{
  lsm6dsv16x_int1_ctrl_t reg;

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_INT1_CTRL, (uint8_t *)&reg, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  reg.int1_fifo_full = Status;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_INT1_CTRL, (uint8_t *)&reg, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Set the LSM6DSV16X FIFO full interrupt on INT2 pin
  * @param  pObj the device pObj
  * @param  Status FIFO full interrupt on INT1 pin status
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_FIFO_Set_INT2_FIFO_Full(LSM6DSV16X_Object_t *pObj, uint8_t Status)
{
  lsm6dsv16x_int2_ctrl_t reg;

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_INT2_CTRL, (uint8_t *)&reg, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  reg.int2_fifo_full = Status;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_INT2_CTRL, (uint8_t *)&reg, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Set the LSM6DSV16X FIFO watermark level
  * @param  pObj the device pObj
  * @param  Watermark FIFO watermark level
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_FIFO_Set_Watermark_Level(LSM6DSV16X_Object_t *pObj, uint8_t Watermark)
{
  if (lsm6dsv16x_fifo_watermark_set(&(pObj->Ctx), Watermark) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Set the LSM6DSV16X FIFO stop on watermark
  * @param  pObj the device pObj
  * @param  Status FIFO stop on watermark status
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_FIFO_Set_Stop_On_Fth(LSM6DSV16X_Object_t *pObj, uint8_t Status)
{
  if (lsm6dsv16x_fifo_stop_on_wtm_set(&(pObj->Ctx), Status) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Set the LSM6DSV16X FIFO mode
  * @param  pObj the device pObj
  * @param  Mode FIFO mode
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_FIFO_Set_Mode(LSM6DSV16X_Object_t *pObj, uint8_t Mode)
{
  int32_t ret = LSM6DSV16X_OK;
  lsm6dsv16x_fifo_mode_t newMode = LSM6DSV16X_BYPASS_MODE;

  switch (Mode)
  {
    case 0:
      newMode = LSM6DSV16X_BYPASS_MODE;
      break;
    case 1:
      newMode = LSM6DSV16X_FIFO_MODE;
      break;
    case 3:
      newMode = LSM6DSV16X_STREAM_TO_FIFO_MODE;
      break;
    case 4:
      newMode = LSM6DSV16X_BYPASS_TO_STREAM_MODE;
      break;
    case 6:
      newMode = LSM6DSV16X_STREAM_MODE;
      break;
    case 7:
      newMode = LSM6DSV16X_BYPASS_TO_FIFO_MODE;
      break;
    default:
      ret = LSM6DSV16X_ERROR;
      break;
  }

  if (ret == LSM6DSV16X_ERROR)
  {
    return LSM6DSV16X_ERROR;
  }

  if (lsm6dsv16x_fifo_mode_set(&(pObj->Ctx), newMode) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return ret;
}

/**
  * @brief  Get the LSM6DSV16X FIFO tag
  * @param  pObj the device pObj
  * @param  Tag FIFO tag
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_FIFO_Get_Tag(LSM6DSV16X_Object_t *pObj, uint8_t *Tag)
{
  lsm6dsv16x_fifo_data_out_tag_t tag_local;

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_FIFO_DATA_OUT_TAG, (uint8_t *)&tag_local, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  *Tag = (uint8_t)tag_local.tag_sensor;

  return LSM6DSV16X_OK;
}

/**
  * @brief  Get the LSM6DSV16X FIFO raw data
  * @param  pObj the device pObj
  * @param  Data FIFO raw data array [6]
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_FIFO_Get_Data(LSM6DSV16X_Object_t *pObj, uint8_t *Data)
{
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_FIFO_DATA_OUT_X_L, Data, 6) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Get the LSM6DSV16X FIFO accelero single sample (16-bit data per 3 axes) and calculate acceleration [mg]
  * @param  pObj the device pObj
  * @param  Acceleration FIFO accelero axes [mg]
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_FIFO_ACC_Get_Axes(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_Axes_t *Acceleration)
{
  lsm6dsv16x_axis3bit16_t data_raw;
  float_t sensitivity = 0.0f;
  float_t acceleration_float_t[3];

  if (LSM6DSV16X_FIFO_Get_Data(pObj, data_raw.u8bit) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  if (LSM6DSV16X_ACC_GetSensitivity(pObj, &sensitivity) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }
  acceleration_float_t[0] = (float_t)data_raw.i16bit[0] * sensitivity;
  acceleration_float_t[1] = (float_t)data_raw.i16bit[1] * sensitivity;
  acceleration_float_t[2] = (float_t)data_raw.i16bit[2] * sensitivity;

  Acceleration->x = (int32_t)acceleration_float_t[0];
  Acceleration->y = (int32_t)acceleration_float_t[1];
  Acceleration->z = (int32_t)acceleration_float_t[2];

  return LSM6DSV16X_OK;
}

/**
  * @brief  Set the LSM6DSV16X FIFO accelero BDR value
  * @param  pObj the device pObj
  * @param  Bdr FIFO accelero BDR value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_FIFO_ACC_Set_BDR(LSM6DSV16X_Object_t *pObj, float_t Bdr)
{
  lsm6dsv16x_fifo_xl_batch_t new_bdr;

  new_bdr = (Bdr <=    0.0f) ? LSM6DSV16X_XL_NOT_BATCHED
            : (Bdr <=    1.8f) ? LSM6DSV16X_XL_BATCHED_AT_1Hz875
            : (Bdr <=    7.5f) ? LSM6DSV16X_XL_BATCHED_AT_7Hz5
            : (Bdr <=   15.0f) ? LSM6DSV16X_XL_BATCHED_AT_15Hz
            : (Bdr <=   30.0f) ? LSM6DSV16X_XL_BATCHED_AT_30Hz
            : (Bdr <=   60.0f) ? LSM6DSV16X_XL_BATCHED_AT_60Hz
            : (Bdr <=  120.0f) ? LSM6DSV16X_XL_BATCHED_AT_120Hz
            : (Bdr <=  240.0f) ? LSM6DSV16X_XL_BATCHED_AT_240Hz
            : (Bdr <=  480.0f) ? LSM6DSV16X_XL_BATCHED_AT_480Hz
            : (Bdr <=  960.0f) ? LSM6DSV16X_XL_BATCHED_AT_960Hz
            : (Bdr <= 1920.0f) ? LSM6DSV16X_XL_BATCHED_AT_1920Hz
            : (Bdr <= 3840.0f) ? LSM6DSV16X_XL_BATCHED_AT_3840Hz
            :                    LSM6DSV16X_XL_BATCHED_AT_7680Hz;

  if (lsm6dsv16x_fifo_xl_batch_set(&(pObj->Ctx), new_bdr) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Get the LSM6DSV16X FIFO gyro single sample (16-bit data per 3 axes) and calculate angular velocity [mDPS]
  * @param  pObj the device pObj
  * @param  AngularVelocity FIFO gyro axes [mDPS]
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_FIFO_GYRO_Get_Axes(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_Axes_t *AngularVelocity)
{
  lsm6dsv16x_axis3bit16_t data_raw;
  float_t sensitivity = 0.0f;
  float_t angular_velocity_float_t[3];

  if (LSM6DSV16X_FIFO_Get_Data(pObj, data_raw.u8bit) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  if (LSM6DSV16X_GYRO_GetSensitivity(pObj, &sensitivity) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  angular_velocity_float_t[0] = (float_t)data_raw.i16bit[0] * sensitivity;
  angular_velocity_float_t[1] = (float_t)data_raw.i16bit[1] * sensitivity;
  angular_velocity_float_t[2] = (float_t)data_raw.i16bit[2] * sensitivity;

  AngularVelocity->x = (int32_t)angular_velocity_float_t[0];
  AngularVelocity->y = (int32_t)angular_velocity_float_t[1];
  AngularVelocity->z = (int32_t)angular_velocity_float_t[2];

  return LSM6DSV16X_OK;
}

/**
  * @brief  Set the LSM6DSV16X FIFO gyro BDR value
  * @param  pObj the device pObj
  * @param  Bdr FIFO gyro BDR value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_FIFO_GYRO_Set_BDR(LSM6DSV16X_Object_t *pObj, float_t Bdr)
{
  lsm6dsv16x_fifo_gy_batch_t new_bdr;

  new_bdr = (Bdr <=    0.0f) ? LSM6DSV16X_GY_NOT_BATCHED
            : (Bdr <=    1.8f) ? LSM6DSV16X_GY_BATCHED_AT_1Hz875
            : (Bdr <=    7.5f) ? LSM6DSV16X_GY_BATCHED_AT_7Hz5
            : (Bdr <=   15.0f) ? LSM6DSV16X_GY_BATCHED_AT_15Hz
            : (Bdr <=   30.0f) ? LSM6DSV16X_GY_BATCHED_AT_30Hz
            : (Bdr <=   60.0f) ? LSM6DSV16X_GY_BATCHED_AT_60Hz
            : (Bdr <=  120.0f) ? LSM6DSV16X_GY_BATCHED_AT_120Hz
            : (Bdr <=  240.0f) ? LSM6DSV16X_GY_BATCHED_AT_240Hz
            : (Bdr <=  480.0f) ? LSM6DSV16X_GY_BATCHED_AT_480Hz
            : (Bdr <=  960.0f) ? LSM6DSV16X_GY_BATCHED_AT_960Hz
            : (Bdr <= 1920.0f) ? LSM6DSV16X_GY_BATCHED_AT_1920Hz
            : (Bdr <= 3840.0f) ? LSM6DSV16X_GY_BATCHED_AT_3840Hz
            :                    LSM6DSV16X_GY_BATCHED_AT_7680Hz;

  if (lsm6dsv16x_fifo_gy_batch_set(&(pObj->Ctx), new_bdr) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Enable the LSM6DSV16X gyroscope sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_GYRO_Enable(LSM6DSV16X_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->gyro_is_enabled == 1U)
  {
    return LSM6DSV16X_OK;
  }

  /* Output data rate selection */
  if (lsm6dsv16x_gy_data_rate_set(&(pObj->Ctx), pObj->gyro_odr) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  pObj->gyro_is_enabled = 1;

  return LSM6DSV16X_OK;
}

/**
  * @brief  Disable the LSM6DSV16X gyroscope sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_GYRO_Disable(LSM6DSV16X_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->gyro_is_enabled == 0U)
  {
    return LSM6DSV16X_OK;
  }

  /* Get current output data rate */
  if (lsm6dsv16x_gy_data_rate_get(&(pObj->Ctx), &pObj->gyro_odr) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Output data rate selection - power down */
  if (lsm6dsv16x_gy_data_rate_set(&(pObj->Ctx), LSM6DSV16X_ODR_OFF) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  pObj->gyro_is_enabled = 0;

  return LSM6DSV16X_OK;
}

/**
  * @brief  Get the LSM6DSV16X gyroscope sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_GYRO_GetSensitivity(LSM6DSV16X_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = LSM6DSV16X_OK;
  lsm6dsv16x_gy_full_scale_t full_scale;

  /* Read actual full scale selection from sensor */
  if (lsm6dsv16x_gy_full_scale_get(&(pObj->Ctx), &full_scale) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Store the sensitivity based on actual full scale */
  switch (full_scale)
  {
    case LSM6DSV16X_125dps:
      *Sensitivity = LSM6DSV16X_GYRO_SENSITIVITY_FS_125DPS;
      break;

    case LSM6DSV16X_250dps:
      *Sensitivity = LSM6DSV16X_GYRO_SENSITIVITY_FS_250DPS;
      break;

    case LSM6DSV16X_500dps:
      *Sensitivity = LSM6DSV16X_GYRO_SENSITIVITY_FS_500DPS;
      break;

    case LSM6DSV16X_1000dps:
      *Sensitivity = LSM6DSV16X_GYRO_SENSITIVITY_FS_1000DPS;
      break;

    case LSM6DSV16X_2000dps:
      *Sensitivity = LSM6DSV16X_GYRO_SENSITIVITY_FS_2000DPS;
      break;

    case LSM6DSV16X_4000dps:
      *Sensitivity = LSM6DSV16X_GYRO_SENSITIVITY_FS_4000DPS;
      break;

    default:
      ret = LSM6DSV16X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the LSM6DSV16X gyroscope sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_GYRO_GetOutputDataRate(LSM6DSV16X_Object_t *pObj, float *Odr)
{
  int32_t ret = LSM6DSV16X_OK;
  lsm6dsv16x_data_rate_t odr_low_level;

  /* Get current output data rate */
  if (lsm6dsv16x_gy_data_rate_get(&(pObj->Ctx), &odr_low_level) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  switch (odr_low_level)
  {
    case LSM6DSV16X_ODR_OFF:
      *Odr = 0.0f;
      break;

    case LSM6DSV16X_ODR_AT_7Hz5:
      *Odr = 7.5f;
      break;

    case LSM6DSV16X_ODR_AT_15Hz:
      *Odr = 15.0f;
      break;

    case LSM6DSV16X_ODR_AT_30Hz:
      *Odr = 30.0f;
      break;

    case LSM6DSV16X_ODR_AT_60Hz:
      *Odr = 60.0f;
      break;

    case LSM6DSV16X_ODR_AT_120Hz:
      *Odr = 120.0f;
      break;

    case LSM6DSV16X_ODR_AT_240Hz:
      *Odr = 240.0f;
      break;

    case LSM6DSV16X_ODR_AT_480Hz:
      *Odr = 480.0f;
      break;

    case LSM6DSV16X_ODR_AT_960Hz:
      *Odr = 960.0f;
      break;

    case LSM6DSV16X_ODR_AT_1920Hz:
      *Odr = 1920.0f;
      break;

    case LSM6DSV16X_ODR_AT_3840Hz:
      *Odr = 3840.0f;
      break;

    case LSM6DSV16X_ODR_AT_7680Hz:
      *Odr = 7680.0f;
      break;

    default:
      ret = LSM6DSV16X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the LSM6DSV16X gyroscope sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_GYRO_SetOutputDataRate(LSM6DSV16X_Object_t *pObj, float Odr)
{
  if (pObj->gyro_is_enabled == 1U)
  {
    return LSM6DSV16X_GYRO_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return LSM6DSV16X_GYRO_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Set the LSM6DSV16X gyroscope sensor output data rate with operating mode
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @param  Mode the gyroscope operating mode
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_GYRO_SetOutputDataRate_With_Mode(LSM6DSV16X_Object_t *pObj, float Odr,
                                                    LSM6DSV16X_GYRO_Operating_Mode_t Mode)
{
  switch (Mode)
  {
    case LSM6DSV16X_GYRO_HIGH_PERFORMANCE_MODE:
    {
      if (lsm6dsv16x_gy_mode_set(&(pObj->Ctx), LSM6DSV16X_GY_HIGH_PERFORMANCE_MD) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 7.68kHz */
      Odr = (Odr <    7.5f) ?    7.5f
            : (Odr > 7680.0f) ? 7680.0f
            :                       Odr;
      break;
    }

    case LSM6DSV16X_GYRO_HIGH_ACCURACY_MODE:
      return LSM6DSV16X_ERROR;

    case LSM6DSV16X_GYRO_SLEEP_MODE:
      return LSM6DSV16X_ERROR;

    case LSM6DSV16X_GYRO_LOW_POWER_MODE:
    {
      if (lsm6dsv16x_gy_mode_set(&(pObj->Ctx), LSM6DSV16X_GY_LOW_POWER_MD) != LSM6DSV16X_OK)
      {
        return LSM6DSV16X_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 240kHz */
      Odr = (Odr <   7.5f) ?   7.5f
            : (Odr > 240.0f) ? 240.0f
            :                     Odr;
      break;
    }

    default:
      return LSM6DSV16X_ERROR;
  }

  if (pObj->gyro_is_enabled == 1U)
  {
    return LSM6DSV16X_GYRO_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return LSM6DSV16X_GYRO_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the LSM6DSV16X gyroscope sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_GYRO_GetFullScale(LSM6DSV16X_Object_t *pObj, int32_t  *FullScale)
{
  int32_t ret = LSM6DSV16X_OK;
  lsm6dsv16x_gy_full_scale_t fs_low_level;

  /* Read actual full scale selection from sensor */
  if (lsm6dsv16x_gy_full_scale_get(&(pObj->Ctx), &fs_low_level) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  switch (fs_low_level)
  {
    case LSM6DSV16X_125dps:
      *FullScale =  125;
      break;

    case LSM6DSV16X_250dps:
      *FullScale =  250;
      break;

    case LSM6DSV16X_500dps:
      *FullScale =  500;
      break;

    case LSM6DSV16X_1000dps:
      *FullScale = 1000;
      break;

    case LSM6DSV16X_2000dps:
      *FullScale = 2000;
      break;

    case LSM6DSV16X_4000dps:
      *FullScale = 4000;
      break;

    default:
      ret = LSM6DSV16X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the LSM6DSV16X gyroscope sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_GYRO_SetFullScale(LSM6DSV16X_Object_t *pObj, int32_t FullScale)
{
  lsm6dsv16x_gy_full_scale_t new_fs;

  new_fs = (FullScale <= 125)  ? LSM6DSV16X_125dps
           : (FullScale <= 250)  ? LSM6DSV16X_250dps
           : (FullScale <= 500)  ? LSM6DSV16X_500dps
           : (FullScale <= 1000) ? LSM6DSV16X_1000dps
           : (FullScale <= 2000) ? LSM6DSV16X_2000dps
           :                       LSM6DSV16X_4000dps;

  if (lsm6dsv16x_gy_full_scale_set(&(pObj->Ctx), new_fs) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Get the LSM6DSV16X gyroscope sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_GYRO_GetAxesRaw(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_AxesRaw_t *Value)
{
  lsm6dsv16x_axis3bit16_t data_raw;

  /* Read raw data values */
  if (lsm6dsv16x_angular_rate_raw_get(&(pObj->Ctx), data_raw.i16bit) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Format the data */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return LSM6DSV16X_OK;
}

/**
  * @brief  Get the LSM6DSV16X gyroscope sensor axes
  * @param  pObj the device pObj
  * @param  AngularRate pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_GYRO_GetAxes(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_Axes_t *AngularRate)
{
  lsm6dsv16x_axis3bit16_t data_raw;
  float sensitivity;

  /* Read raw data values */
  if (lsm6dsv16x_angular_rate_raw_get(&(pObj->Ctx), data_raw.i16bit) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Get LSM6DSV16X actual sensitivity */
  if (LSM6DSV16X_GYRO_GetSensitivity(pObj, &sensitivity) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Calculate the data */
  AngularRate->x = (int32_t)((float)((float)data_raw.i16bit[0] * sensitivity));
  AngularRate->y = (int32_t)((float)((float)data_raw.i16bit[1] * sensitivity));
  AngularRate->z = (int32_t)((float)((float)data_raw.i16bit[2] * sensitivity));

  return LSM6DSV16X_OK;
}

/**
  * @brief  Get the LSM6DSV16X register value
  * @param  pObj the device pObj
  * @param  Reg address to be read
  * @param  Data pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_Read_Reg(LSM6DSV16X_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (lsm6dsv16x_read_reg(&(pObj->Ctx), Reg, Data, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Set the LSM6DSV16X register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_Write_Reg(LSM6DSV16X_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (lsm6dsv16x_write_reg(&(pObj->Ctx), Reg, &Data, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Get the LSM6DSV16X ACC data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Get_DRDY_Status(LSM6DSV16X_Object_t *pObj, uint8_t *Status)
{
  lsm6dsv16x_all_sources_t val;

  if (lsm6dsv16x_all_sources_get(&(pObj->Ctx), &val) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  *Status = val.drdy_xl;
  return LSM6DSV16X_OK;
}

/**
  * @brief  Get the LSM6DSV16X GYRO data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_GYRO_Get_DRDY_Status(LSM6DSV16X_Object_t *pObj, uint8_t *Status)
{
  lsm6dsv16x_all_sources_t val;

  if (lsm6dsv16x_all_sources_get(&(pObj->Ctx), &val) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  *Status = val.drdy_gy;
  return LSM6DSV16X_OK;
}

/**
  * @brief  Set the LSM6DSV16X accelerometer power mode
  * @param  pObj the device pObj
  * @param  PowerMode Value of the powerMode
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Set_Power_Mode(LSM6DSV16X_Object_t *pObj, uint8_t PowerMode)
{
  if (lsm6dsv16x_xl_mode_set(&(pObj->Ctx), (lsm6dsv16x_xl_mode_t)PowerMode) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Set the LSM6DSV16X gyroscope power mode
  * @param  pObj the device pObj
  * @param  PowerMode Value of the powerMode
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_GYRO_Set_Power_Mode(LSM6DSV16X_Object_t *pObj, uint8_t PowerMode)
{
  if (lsm6dsv16x_gy_mode_set(&(pObj->Ctx), (lsm6dsv16x_gy_mode_t)PowerMode) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Set the LSM6DSV16X accelerometer filter mode
  * @param  pObj the device pObj
  * @param  LowHighPassFlag 0/1 for setting low-pass/high-pass filter mode
  * @param  FilterMode Value of the filter Mode
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Set_Filter_Mode(LSM6DSV16X_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode)
{
  if (LowHighPassFlag == 0)
  {
    /* Set accelerometer low_pass filter-mode */
    if (lsm6dsv16x_filt_xl_lp2_set(&(pObj->Ctx), 1) != LSM6DSV16X_OK)
    {
      return LSM6DSV16X_ERROR;
    }
    if (lsm6dsv16x_filt_xl_lp2_bandwidth_set(&(pObj->Ctx), (lsm6dsv16x_filt_xl_lp2_bandwidth_t)FilterMode) != LSM6DSV16X_OK)
    {
      return LSM6DSV16X_ERROR;
    }
  }
  else
  {
    /* Set accelerometer high_pass filter-mode */
    if (lsm6dsv16x_filt_xl_lp2_set(&(pObj->Ctx), 0) != LSM6DSV16X_OK)
    {
      return LSM6DSV16X_ERROR;
    }
    if (lsm6dsv16x_filt_xl_lp2_bandwidth_set(&(pObj->Ctx), (lsm6dsv16x_filt_xl_lp2_bandwidth_t)FilterMode) != LSM6DSV16X_OK)
    {
      return LSM6DSV16X_ERROR;
    }
  }
  return LSM6DSV16X_OK;
}

/**
  * @brief  Set the LSM6DSV16X gyroscope filter mode
  * @param  pObj the device pObj
  * @param  LowHighPassFlag 0/1 for setting low-pass/high-pass filter mode
  * @param  FilterMode Value of the filter Mode
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_GYRO_Set_Filter_Mode(LSM6DSV16X_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode)
{
  if (LowHighPassFlag == 0)
  {
    /* Set gyroscope low_pass 1 filter-mode */
    if (lsm6dsv16x_filt_gy_lp1_set(&(pObj->Ctx), 1) != LSM6DSV16X_OK)
    {
      return LSM6DSV16X_ERROR;
    }
    if (lsm6dsv16x_filt_gy_lp1_bandwidth_set(&(pObj->Ctx), (lsm6dsv16x_filt_gy_lp1_bandwidth_t)FilterMode) != LSM6DSV16X_OK)
    {
      return LSM6DSV16X_ERROR;
    }
  }
  else
  {
    /* Set gyroscope high_pass filter-mode */
    if (lsm6dsv16x_filt_gy_lp1_set(&(pObj->Ctx), 0) != LSM6DSV16X_OK)
    {
      return LSM6DSV16X_ERROR;
    }
    if (lsm6dsv16x_filt_gy_lp1_bandwidth_set(&(pObj->Ctx), (lsm6dsv16x_filt_gy_lp1_bandwidth_t)FilterMode) != LSM6DSV16X_OK)
    {
      return LSM6DSV16X_ERROR;
    }
  }
  return LSM6DSV16X_OK;
}

/**
  * @brief  Get the LSM6DSV16X QVAR sensor data
  * @param  pObj the device pObj
  * @param  Data pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_QVAR_GetData(LSM6DSV16X_Object_t *pObj, int16_t *Data)
{
  if (lsm6dsv16x_ah_qvar_raw_get(&(pObj->Ctx), Data) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Get the LSM6DSV16X QVAR DRDY status
  * @param  pObj the device pObj
  * @param  Status pointer where the DRDY status is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_QVAR_GetDRDY(LSM6DSV16X_Object_t *pObj, uint8_t *Status)
{
  lsm6dsv16x_all_sources_t val;

  /* Read all statuses */
  if (lsm6dsv16x_all_sources_get(&(pObj->Ctx), &val) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Store QVAR DRDY status */
  *Status = val.drdy_ah_qvar;

  return LSM6DSV16X_OK;
}

/**
  * @brief  Get the LSM6DSV16X equivalent input impedance of the AH_QVAR buffers in [MOhm]
  * @param  pObj the device pObj
  * @param  ZInMOhm pointer where the equivalent input impedance of the AH_QVAR buffers in [MOhm] is written to
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_QVAR_GetZIn(LSM6DSV16X_Object_t *pObj, uint16_t *ZInMOhm)
{
  int32_t ret = LSM6DSV16X_OK;
  lsm6dsv16x_ah_qvar_zin_t val;

  /* Read ZInMOhm value */
  if (lsm6dsv16x_ah_qvar_zin_get(&(pObj->Ctx), &val) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  switch (val)
  {
    case LSM6DSV16X_2400MOhm:
      *ZInMOhm = 2400;
      break;

    case LSM6DSV16X_730MOhm:
      *ZInMOhm = 730;
      break;

    case LSM6DSV16X_300MOhm:
      *ZInMOhm = 300;
      break;

    case LSM6DSV16X_255MOhm:
      *ZInMOhm = 255;
      break;

    default:
      ret = LSM6DSV16X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the LSM6DSV16X equivalent input impedance of the AH_QVAR buffers in [MOhm]
  * @param  pObj the device pObj
  * @param  ZInMOhm the equivalent input impedance of the AH_QVAR buffers in [MOhm]
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_QVAR_SetZIn(LSM6DSV16X_Object_t *pObj, uint16_t ZInMOhm)
{
  lsm6dsv16x_ah_qvar_zin_t val;

  val = (ZInMOhm <= 255)  ? LSM6DSV16X_255MOhm
        : (ZInMOhm <= 300)  ? LSM6DSV16X_300MOhm
        : (ZInMOhm <= 730)  ? LSM6DSV16X_730MOhm
        :                     LSM6DSV16X_2400MOhm;

  if (lsm6dsv16x_ah_qvar_zin_set(&(pObj->Ctx), val) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Enable LSM6DSV16X accelerometer DRDY interrupt on INT1
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Enable_DRDY_On_INT1(LSM6DSV16X_Object_t *pObj)
{
  lsm6dsv16x_pin_int_route_t pin_int1_route;

  /* Enable accelerometer DRDY Interrupt on INT1 */
  if (lsm6dsv16x_pin_int1_route_get(&(pObj->Ctx), &pin_int1_route) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }
  pin_int1_route.drdy_xl = 1;
  pin_int1_route.drdy_g = 0;
  if (lsm6dsv16x_pin_int1_route_set(&(pObj->Ctx), &pin_int1_route) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Disable LSM6DSV16X accelerometer DRDY interrupt on INT1
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_ACC_Disable_DRDY_On_INT1(LSM6DSV16X_Object_t *pObj)
{
  lsm6dsv16x_pin_int_route_t pin_int1_route;

  /* Disable accelerometer DRDY Interrupt on INT1 */
  if (lsm6dsv16x_pin_int1_route_get(&(pObj->Ctx), &pin_int1_route) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }
  pin_int1_route.drdy_xl = 0;
  if (lsm6dsv16x_pin_int1_route_set(&(pObj->Ctx), &pin_int1_route) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Enable LSM6DSV16X gyroscope DRDY interrupt on INT2
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_GYRO_Enable_DRDY_On_INT2(LSM6DSV16X_Object_t *pObj)
{
  lsm6dsv16x_pin_int_route_t pin_int2_route;

  /* Enable gyroscope DRDY Interrupts on INT2 */
  if (lsm6dsv16x_pin_int2_route_get(&(pObj->Ctx), &pin_int2_route) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }
  pin_int2_route.drdy_xl = 0;
  pin_int2_route.drdy_g = 1;
  if (lsm6dsv16x_pin_int2_route_set(&(pObj->Ctx), &pin_int2_route) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Disable LSM6DSV16X gyroscope DRDY interrupt on INT2
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_GYRO_Disable_DRDY_On_INT2(LSM6DSV16X_Object_t *pObj)
{
  lsm6dsv16x_pin_int_route_t pin_int2_route;

  /* Disable gyroscope DRDY Interrupt on INT2 */
  if (lsm6dsv16x_pin_int2_route_get(&(pObj->Ctx), &pin_int2_route) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }
  pin_int2_route.drdy_g = 0;
  if (lsm6dsv16x_pin_int2_route_set(&(pObj->Ctx), &pin_int2_route) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Set LSM6DSV16X DRDY mode
  * @param  pObj the device pObj
  * @param  Mode DRDY mode
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_DRDY_Set_Mode(LSM6DSV16X_Object_t *pObj, uint8_t Mode)
{
  int32_t ret = LSM6DSV16X_OK;
  lsm6dsv16x_data_ready_mode_t newMode = LSM6DSV16X_DRDY_LATCHED;

  switch (Mode)
  {
    case 0:
      newMode = LSM6DSV16X_DRDY_LATCHED;
      break;
    case 1:
      newMode = LSM6DSV16X_DRDY_PULSED;
      break;
    default:
      ret = LSM6DSV16X_ERROR;
      break;
  }

  if (ret == LSM6DSV16X_ERROR)
  {
    return LSM6DSV16X_ERROR;
  }

  /* Set DRDY mode */
  if (lsm6dsv16x_data_ready_mode_set(&(pObj->Ctx), newMode) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @}
  */

/** @defgroup LSM6DSV16X_Private_Functions LSM6DSV16X Private Functions
  * @{
  */

/**
  * @brief  Set the LSM6DSV16X accelerometer sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LSM6DSV16X_ACC_SetOutputDataRate_When_Enabled(LSM6DSV16X_Object_t *pObj, float Odr)
{
  lsm6dsv16x_data_rate_t new_odr;

  new_odr = (Odr <=  1.875f) ? LSM6DSV16X_ODR_AT_1Hz875
            : (Odr <=    7.5f) ? LSM6DSV16X_ODR_AT_7Hz5
            : (Odr <=   15.0f) ? LSM6DSV16X_ODR_AT_15Hz
            : (Odr <=   30.0f) ? LSM6DSV16X_ODR_AT_30Hz
            : (Odr <=   60.0f) ? LSM6DSV16X_ODR_AT_60Hz
            : (Odr <=  120.0f) ? LSM6DSV16X_ODR_AT_120Hz
            : (Odr <=  240.0f) ? LSM6DSV16X_ODR_AT_240Hz
            : (Odr <=  480.0f) ? LSM6DSV16X_ODR_AT_480Hz
            : (Odr <=  960.0f) ? LSM6DSV16X_ODR_AT_960Hz
            : (Odr <= 1920.0f) ? LSM6DSV16X_ODR_AT_1920Hz
            : (Odr <= 3840.0f) ? LSM6DSV16X_ODR_AT_3840Hz
            :                    LSM6DSV16X_ODR_AT_7680Hz;

  /* Output data rate selection */
  if (lsm6dsv16x_xl_data_rate_set(&(pObj->Ctx), new_odr) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Set the LSM6DSV16X accelerometer sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LSM6DSV16X_ACC_SetOutputDataRate_When_Disabled(LSM6DSV16X_Object_t *pObj, float Odr)
{
  pObj->acc_odr = (Odr <=  1.875f) ? LSM6DSV16X_ODR_AT_1Hz875
                  : (Odr <=    7.5f) ? LSM6DSV16X_ODR_AT_7Hz5
                  : (Odr <=   15.0f) ? LSM6DSV16X_ODR_AT_15Hz
                  : (Odr <=   30.0f) ? LSM6DSV16X_ODR_AT_30Hz
                  : (Odr <=   60.0f) ? LSM6DSV16X_ODR_AT_60Hz
                  : (Odr <=  120.0f) ? LSM6DSV16X_ODR_AT_120Hz
                  : (Odr <=  240.0f) ? LSM6DSV16X_ODR_AT_240Hz
                  : (Odr <=  480.0f) ? LSM6DSV16X_ODR_AT_480Hz
                  : (Odr <=  960.0f) ? LSM6DSV16X_ODR_AT_960Hz
                  : (Odr <= 1920.0f) ? LSM6DSV16X_ODR_AT_1920Hz
                  : (Odr <= 3840.0f) ? LSM6DSV16X_ODR_AT_3840Hz
                  :                    LSM6DSV16X_ODR_AT_7680Hz;

  return LSM6DSV16X_OK;
}

/**
  * @brief  Set the LSM6DSV16X gyroscope sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LSM6DSV16X_GYRO_SetOutputDataRate_When_Enabled(LSM6DSV16X_Object_t *pObj, float Odr)
{
  lsm6dsv16x_data_rate_t new_odr;

  new_odr = (Odr <=    7.5f) ? LSM6DSV16X_ODR_AT_7Hz5
            : (Odr <=   15.0f) ? LSM6DSV16X_ODR_AT_15Hz
            : (Odr <=   30.0f) ? LSM6DSV16X_ODR_AT_30Hz
            : (Odr <=   60.0f) ? LSM6DSV16X_ODR_AT_60Hz
            : (Odr <=  120.0f) ? LSM6DSV16X_ODR_AT_120Hz
            : (Odr <=  240.0f) ? LSM6DSV16X_ODR_AT_240Hz
            : (Odr <=  480.0f) ? LSM6DSV16X_ODR_AT_480Hz
            : (Odr <=  960.0f) ? LSM6DSV16X_ODR_AT_960Hz
            : (Odr <= 1920.0f) ? LSM6DSV16X_ODR_AT_1920Hz
            : (Odr <= 3840.0f) ? LSM6DSV16X_ODR_AT_3840Hz
            :                    LSM6DSV16X_ODR_AT_7680Hz;

  /* Output data rate selection */
  if (lsm6dsv16x_gy_data_rate_set(&(pObj->Ctx), new_odr) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
  * @brief  Set the LSM6DSV16X gyroscope sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LSM6DSV16X_GYRO_SetOutputDataRate_When_Disabled(LSM6DSV16X_Object_t *pObj, float Odr)
{
  pObj->gyro_odr = (Odr <=    7.5f) ? LSM6DSV16X_ODR_AT_7Hz5
                   : (Odr <=   15.0f) ? LSM6DSV16X_ODR_AT_15Hz
                   : (Odr <=   30.0f) ? LSM6DSV16X_ODR_AT_30Hz
                   : (Odr <=   60.0f) ? LSM6DSV16X_ODR_AT_60Hz
                   : (Odr <=  120.0f) ? LSM6DSV16X_ODR_AT_120Hz
                   : (Odr <=  240.0f) ? LSM6DSV16X_ODR_AT_240Hz
                   : (Odr <=  480.0f) ? LSM6DSV16X_ODR_AT_480Hz
                   : (Odr <=  960.0f) ? LSM6DSV16X_ODR_AT_960Hz
                   : (Odr <= 1920.0f) ? LSM6DSV16X_ODR_AT_1920Hz
                   : (Odr <= 3840.0f) ? LSM6DSV16X_ODR_AT_3840Hz
                   :                    LSM6DSV16X_ODR_AT_7680Hz;

  return LSM6DSV16X_OK;
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
  LSM6DSV16X_Object_t *pObj = (LSM6DSV16X_Object_t *)Handle;

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
  LSM6DSV16X_Object_t *pObj = (LSM6DSV16X_Object_t *)Handle;

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
