/**
  ******************************************************************************
  * @file    ism6hg256x.c
  * @author  MEMS Software Solutions Team
  * @brief   ISM6HG256X driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ism6hg256x.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup ISM6HG256X ISM6HG256X
  * @{
  */

/** @defgroup ISM6HG256X_Exported_Variables ISM6HG256X Exported Variables
  * @{
  */

ISM6HG256X_CommonDrv_t ISM6HG256X_COMMON_Driver =
{
  ISM6HG256X_Init,
  ISM6HG256X_DeInit,
  ISM6HG256X_ReadID,
  ISM6HG256X_GetCapabilities,
};

ISM6HG256X_ACC_Drv_t ISM6HG256X_ACC_Driver =
{
  ISM6HG256X_ACC_Enable,
  ISM6HG256X_ACC_Disable,
  ISM6HG256X_ACC_GetSensitivity,
  ISM6HG256X_ACC_GetOutputDataRate,
  ISM6HG256X_ACC_SetOutputDataRate,
  ISM6HG256X_ACC_GetFullScale,
  ISM6HG256X_ACC_SetFullScale,
  ISM6HG256X_ACC_GetAxes,
  ISM6HG256X_ACC_GetAxesRaw,
};

ISM6HG256X_GYRO_Drv_t ISM6HG256X_GYRO_Driver =
{
  ISM6HG256X_GYRO_Enable,
  ISM6HG256X_GYRO_Disable,
  ISM6HG256X_GYRO_GetSensitivity,
  ISM6HG256X_GYRO_GetOutputDataRate,
  ISM6HG256X_GYRO_SetOutputDataRate,
  ISM6HG256X_GYRO_GetFullScale,
  ISM6HG256X_GYRO_SetFullScale,
  ISM6HG256X_GYRO_GetAxes,
  ISM6HG256X_GYRO_GetAxesRaw,
};

/**
  * @}
  */

/** @defgroup ISM6HG256X_Private_Function_Prototypes ISM6HG256X Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t ISM6HG256X_ACC_SetOutputDataRate_When_Enabled(ISM6HG256X_Object_t *pObj, float_t Odr);
static int32_t ISM6HG256X_ACC_SetOutputDataRate_When_Disabled(ISM6HG256X_Object_t *pObj, float_t Odr);
static int32_t ISM6HG256X_ACC_HG_SetOutputDataRate_When_Enabled(ISM6HG256X_Object_t *pObj, float_t Odr);
static int32_t ISM6HG256X_ACC_HG_SetOutputDataRate_When_Disabled(ISM6HG256X_Object_t *pObj, float_t Odr);
static int32_t ISM6HG256X_GYRO_SetOutputDataRate_When_Enabled(ISM6HG256X_Object_t *pObj, float_t Odr);
static int32_t ISM6HG256X_GYRO_SetOutputDataRate_When_Disabled(ISM6HG256X_Object_t *pObj, float_t Odr);

/**
  * @}
  */

/** @defgroup ISM6HG256X_Exported_Functions ISM6HG256X Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_RegisterBusIO(ISM6HG256X_Object_t *pObj, ISM6HG256X_IO_t *pIO)
{
  int32_t ret = ISM6HG256X_OK;

  if (pObj == NULL)
  {
    ret = ISM6HG256X_ERROR;
  }
  else
  {
    pObj->IO.Init     = pIO->Init;
    pObj->IO.DeInit   = pIO->DeInit;
    pObj->IO.BusType  = pIO->BusType;
    pObj->IO.Address  = pIO->Address;
    pObj->IO.WriteReg = pIO->WriteReg;
    pObj->IO.ReadReg  = pIO->ReadReg;
    pObj->IO.GetTick  = pIO->GetTick;

    pObj->Ctx.read_reg  = ReadRegWrap;
    pObj->Ctx.write_reg = WriteRegWrap;
    pObj->Ctx.mdelay    = pIO->Delay;
    pObj->Ctx.handle    = pObj;

    if (pObj->IO.Init == NULL)
    {
      ret = ISM6HG256X_ERROR;
    }
    else if (pObj->IO.Init() != ISM6HG256X_OK)
    {
      ret = ISM6HG256X_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == ISM6HG256X_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x04;

          if (ISM6HG256X_Write_Reg(pObj, ISM6HG256X_CTRL3, data) != ISM6HG256X_OK)
          {
            ret = ISM6HG256X_ERROR;
          }

          if (ISM6HG256X_Write_Reg(pObj, ISM6HG256X_IF_CFG, data) != ISM6HG256X_OK)
          {
            ret = ISM6HG256X_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  Initialize the ISM6HG256X sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_Init(ISM6HG256X_Object_t *pObj)
{
  /* Set main memory bank */
  if (ISM6HG256X_Set_Mem_Bank(pObj, (uint8_t)ISM6HG256X_MAIN_MEM_BANK) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Enable register address automatically incremented during a multiple byte
  access with a serial interface. */
  if (ism6hg256x_auto_increment_set(&(pObj->Ctx), PROPERTY_ENABLE) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Enable BDU */
  if (ism6hg256x_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* WAKE_UP mode selection */
  if (ism6hg256x_fifo_mode_set(&(pObj->Ctx), ISM6HG256X_BYPASS_MODE) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = ISM6HG256X_ODR_AT_120Hz;

  /* Output data rate selection - power down. */
  if (ism6hg256x_xl_data_rate_set(&(pObj->Ctx), ISM6HG256X_ODR_OFF) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Full scale selection. */
  if (ism6hg256x_xl_full_scale_set(&(pObj->Ctx), ISM6HG256X_4g) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_hg_odr = ISM6HG256X_HG_XL_ODR_AT_480Hz;

  /* Output data rate selection - power down. */
  if (ism6hg256x_hg_xl_data_rate_set(&(pObj->Ctx), ISM6HG256X_HG_XL_ODR_OFF, PROPERTY_ENABLE) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Full scale selection. */
  if (ism6hg256x_hg_xl_full_scale_set(&(pObj->Ctx), ISM6HG256X_32g) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Select default output data rate. */
  pObj->gyro_odr = ISM6HG256X_ODR_AT_120Hz;

  /* Output data rate selection - power down. */
  if (ism6hg256x_gy_data_rate_set(&(pObj->Ctx), ISM6HG256X_ODR_OFF) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Full scale selection. */
  if (ism6hg256x_gy_full_scale_set(&(pObj->Ctx), ISM6HG256X_2000dps) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  pObj->is_initialized = 1;

  return ISM6HG256X_OK;
}

/**
  * @brief  Deinitialize the ISM6HG256X sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_DeInit(ISM6HG256X_Object_t *pObj)
{
  /* Disable the component */
  if (ISM6HG256X_ACC_Disable(pObj) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  if (ISM6HG256X_ACC_HG_Disable(pObj) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  if (ISM6HG256X_GYRO_Disable(pObj) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Reset output data rate. */
  pObj->acc_odr = ISM6HG256X_ODR_OFF;
  pObj->acc_hg_odr = ISM6HG256X_HG_XL_ODR_OFF;
  pObj->gyro_odr = ISM6HG256X_ODR_OFF;

  pObj->is_initialized = 0;

  return ISM6HG256X_OK;
}

/**
  * @brief  Read component ID
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ReadID(ISM6HG256X_Object_t *pObj, uint8_t *Id)
{
  if (ism6hg256x_device_id_get(&(pObj->Ctx), Id) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Get ISM6HG256X sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to ISM6HG256X sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_GetCapabilities(ISM6HG256X_Object_t *pObj, ISM6HG256X_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Acc        = 1;
  Capabilities->Gyro       = 1;
  Capabilities->Magneto    = 0;
  Capabilities->LowPower   = 1;
  Capabilities->GyroMaxFS  = 4000;
  Capabilities->AccMaxFS   = 80;
  Capabilities->MagMaxFS   = 0;
  Capabilities->GyroMaxOdr = 7680.0f;
  Capabilities->AccMaxOdr  = 7680.0f;
  Capabilities->MagMaxOdr  = 0.0f;
  return ISM6HG256X_OK;
}

/**
  * @brief  Enable the ISM6HG256X accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Enable(ISM6HG256X_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return ISM6HG256X_OK;
  }

  /* Output data rate selection. */
  if (ism6hg256x_xl_data_rate_set(&(pObj->Ctx), pObj->acc_odr) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return ISM6HG256X_OK;
}

/**
  * @brief  Disable the ISM6HG256X accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Disable(ISM6HG256X_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return ISM6HG256X_OK;
  }

  /* Get current output data rate. */
  if (ism6hg256x_xl_data_rate_get(&(pObj->Ctx), &pObj->acc_odr) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Output data rate selection - power down. */
  if (ism6hg256x_xl_data_rate_set(&(pObj->Ctx), ISM6HG256X_ODR_OFF) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return ISM6HG256X_OK;
}

/**
  * @brief  Get the ISM6HG256X accelerometer sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_GetSensitivity(ISM6HG256X_Object_t *pObj, float_t *Sensitivity)
{
  int32_t ret = ISM6HG256X_OK;
  ism6hg256x_xl_full_scale_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (ism6hg256x_xl_full_scale_get(&(pObj->Ctx), &full_scale) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Store the Sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case ISM6HG256X_2g:
      *Sensitivity = ISM6HG256X_ACC_SENSITIVITY_FS_2G;
      break;

    case ISM6HG256X_4g:
      *Sensitivity = ISM6HG256X_ACC_SENSITIVITY_FS_4G;
      break;

    case ISM6HG256X_8g:
      *Sensitivity = ISM6HG256X_ACC_SENSITIVITY_FS_8G;
      break;

    case ISM6HG256X_16g:
      *Sensitivity = ISM6HG256X_ACC_SENSITIVITY_FS_16G;
      break;

    default:
      ret = ISM6HG256X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the ISM6HG256X accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_GetOutputDataRate(ISM6HG256X_Object_t *pObj, float_t *Odr)
{
  int32_t ret = ISM6HG256X_OK;
  ism6hg256x_data_rate_t odr_low_level;

  /* Get current output data rate. */
  if (ism6hg256x_xl_data_rate_get(&(pObj->Ctx), &odr_low_level) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  switch (odr_low_level)
  {
    case ISM6HG256X_ODR_OFF:
      *Odr = 0.0f;
      break;

    case ISM6HG256X_ODR_AT_1Hz875:
      *Odr = 1.875f;
      break;

    case ISM6HG256X_ODR_AT_7Hz5:
      *Odr = 7.5f;
      break;

    case ISM6HG256X_ODR_AT_15Hz:
      *Odr = 15.0f;
      break;

    case ISM6HG256X_ODR_AT_30Hz:
      *Odr = 30.0f;
      break;

    case ISM6HG256X_ODR_AT_60Hz:
      *Odr = 60.0f;
      break;

    case ISM6HG256X_ODR_AT_120Hz:
      *Odr = 120.0f;
      break;

    case ISM6HG256X_ODR_AT_240Hz:
      *Odr = 240.0f;
      break;

    case ISM6HG256X_ODR_AT_480Hz:
      *Odr = 480.0f;
      break;

    case ISM6HG256X_ODR_AT_960Hz:
      *Odr = 960.0f;
      break;

    case ISM6HG256X_ODR_AT_1920Hz:
      *Odr = 1920.0f;
      break;

    case ISM6HG256X_ODR_AT_3840Hz:
      *Odr = 3840.0f;
      break;

    case ISM6HG256X_ODR_AT_7680Hz:
      *Odr = 7680.0f;
      break;

    default:
      ret = ISM6HG256X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the ISM6HG256X accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_SetOutputDataRate(ISM6HG256X_Object_t *pObj, float_t Odr)
{
  if (pObj->acc_is_enabled == 1U)
  {
    return ISM6HG256X_ACC_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return ISM6HG256X_ACC_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Set the ISM6HG256X accelerometer sensor output data rate with operating mode
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @param  Mode the accelerometer operating mode
  * @note   This function switches off the gyroscope if Ultra Low Power Mode is set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_SetOutputDataRate_With_Mode(ISM6HG256X_Object_t *pObj, float_t Odr,
                                                   ISM6HG256X_ACC_Operating_Mode_t Mode)
{
  int32_t ret = ISM6HG256X_OK;
  float_t odr_new = 0.0f;

  switch (Mode)
  {
    case ISM6HG256X_ACC_HIGH_PERFORMANCE_MODE:
      if (ism6hg256x_xl_mode_set(&(pObj->Ctx), ISM6HG256X_XL_HIGH_PERFORMANCE_MD) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 7.68kHz */
      odr_new = (Odr <    7.5f) ?    7.5f
              : (Odr > 7680.0f) ? 7680.0f
              :                       Odr;
      break;

    case ISM6HG256X_ACC_HIGH_ACCURACY_ODR_MODE:
      ret = ISM6HG256X_ERROR;
      break;

    case ISM6HG256X_ACC_ODR_TRIGGERED_MODE:
      ret = ISM6HG256X_ERROR;
      break;

    case ISM6HG256X_ACC_NORMAL_MODE:
      if (ism6hg256x_xl_mode_set(&(pObj->Ctx), ISM6HG256X_XL_NORMAL_MD) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 1.92kHz */
      odr_new = (Odr <    7.5f) ?    7.5f
              : (Odr > 1920.0f) ? 1920.0f
              :                       Odr;
      break;

    case ISM6HG256X_ACC_LOW_POWER_MODE1:
      if (ism6hg256x_xl_mode_set(&(pObj->Ctx), ISM6HG256X_XL_LOW_POWER_2_AVG_MD) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      /* Valid ODR: 1.875Hz;  15Hz <= Odr <= 240kHz */
      odr_new = (Odr ==   1.875f) ?    Odr
              : (Odr <   15.000f) ?  15.0f
              : (Odr >  240.000f) ? 240.0f
              :                        Odr;
      break;

    case ISM6HG256X_ACC_LOW_POWER_MODE2:
      if (ism6hg256x_xl_mode_set(&(pObj->Ctx), ISM6HG256X_XL_LOW_POWER_4_AVG_MD) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      /* Valid ODR: 1.875Hz;  15Hz <= Odr <= 240kHz */
      odr_new = (Odr ==   1.875f) ?    Odr
              : (Odr <   15.000f) ?  15.0f
              : (Odr >  240.000f) ? 240.0f
              :                        Odr;
      break;

    case ISM6HG256X_ACC_LOW_POWER_MODE3:
      if (ism6hg256x_xl_mode_set(&(pObj->Ctx), ISM6HG256X_XL_LOW_POWER_8_AVG_MD) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      /* Valid ODR: 1.875Hz;  15Hz <= Odr <= 240kHz */
      odr_new = (Odr ==   1.875f) ?    Odr
              : (Odr <   15.000f) ?  15.0f
              : (Odr >  240.000f) ? 240.0f
              :                        Odr;
      break;

    default:
      ret = ISM6HG256X_ERROR;
      break;
  }

  if (ret != ISM6HG256X_OK)
  {
    return ret;
  }

  if (pObj->acc_is_enabled == 1U)
  {
    ret = ISM6HG256X_ACC_SetOutputDataRate_When_Enabled(pObj, odr_new);
  }
  else
  {
    ret = ISM6HG256X_ACC_SetOutputDataRate_When_Disabled(pObj, odr_new);
  }

  return ret;
}

/**
  * @brief  Get the ISM6HG256X accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_GetFullScale(ISM6HG256X_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = ISM6HG256X_OK;
  ism6hg256x_xl_full_scale_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (ism6hg256x_xl_full_scale_get(&(pObj->Ctx), &fs_low_level) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  switch (fs_low_level)
  {
    case ISM6HG256X_2g:
      *FullScale =  2;
      break;

    case ISM6HG256X_4g:
      *FullScale =  4;
      break;

    case ISM6HG256X_8g:
      *FullScale =  8;
      break;

    case ISM6HG256X_16g:
      *FullScale = 16;
      break;

    default:
      ret = ISM6HG256X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the ISM6HG256X accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_SetFullScale(ISM6HG256X_Object_t *pObj, int32_t FullScale)
{
  ism6hg256x_xl_full_scale_t new_fs;

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file static analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  new_fs = (FullScale <= 2) ? ISM6HG256X_2g
         : (FullScale <= 4) ? ISM6HG256X_4g
         : (FullScale <= 8) ? ISM6HG256X_8g
         :                    ISM6HG256X_16g;

  if (ism6hg256x_xl_full_scale_set(&(pObj->Ctx), new_fs) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Get the ISM6HG256X accelerometer sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_GetAxesRaw(ISM6HG256X_Object_t *pObj, ISM6HG256X_AxesRaw_t *Value)
{
  ism6hg256x_axis3bit16_t data_raw;

  /* Read raw data values. */
  if (ism6hg256x_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return ISM6HG256X_OK;
}

/**
  * @brief  Get the ISM6HG256X accelerometer sensor axes
  * @param  pObj the device pObj
  * @param  Acceleration pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_GetAxes(ISM6HG256X_Object_t *pObj, ISM6HG256X_Axes_t *Acceleration)
{
  ism6hg256x_axis3bit16_t data_raw;
  float_t sensitivity = 0.0f;

  /* Read raw data values. */
  if (ism6hg256x_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Get ISM6HG256X actual sensitivity. */
  if (ISM6HG256X_ACC_GetSensitivity(pObj, &sensitivity) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Calculate the data. */
  Acceleration->x = (int32_t)((float_t)((float_t)data_raw.i16bit[0] * sensitivity));
  Acceleration->y = (int32_t)((float_t)((float_t)data_raw.i16bit[1] * sensitivity));
  Acceleration->z = (int32_t)((float_t)((float_t)data_raw.i16bit[2] * sensitivity));

  return ISM6HG256X_OK;
}

/**
  * @brief  Get the status of all hardware events
  * @param  pObj the device pObj
  * @param  Status the status of all hardware events
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Get_Event_Status(ISM6HG256X_Object_t *pObj, ISM6HG256X_Event_Status_t *Status)
{

  ism6hg256x_emb_func_status_t emb_func_status;
  ism6hg256x_wake_up_src_t wake_up_src;
  ism6hg256x_tap_src_t tap_src;
  ism6hg256x_d6d_src_t d6d_src;
  ism6hg256x_emb_func_src_t func_src;

  ism6hg256x_md1_cfg_t md1_cfg;
  ism6hg256x_md2_cfg_t md2_cfg;

  ism6hg256x_emb_func_int1_t int1_ctrl;
  ism6hg256x_emb_func_int2_t int2_ctrl;


  (void)memset((void *)Status, 0x0, sizeof(ISM6HG256X_Event_Status_t));

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_WAKE_UP_SRC, (uint8_t *)&wake_up_src, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_TAP_SRC, (uint8_t *)&tap_src, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_D6D_SRC, (uint8_t *)&d6d_src, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  if (ism6hg256x_mem_bank_set(&(pObj->Ctx), ISM6HG256X_EMBED_FUNC_MEM_BANK) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_SRC, (uint8_t *)&func_src, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_INT1, (uint8_t *)&int1_ctrl, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_INT2, (uint8_t *)&int2_ctrl, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_STATUS, (uint8_t *)&emb_func_status, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  if (ism6hg256x_mem_bank_set(&(pObj->Ctx), ISM6HG256X_MAIN_MEM_BANK) != 0)
  {
    return ISM6HG256X_ERROR;
  }


  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&md1_cfg, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&md2_cfg, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
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

  if ((int1_ctrl.int1_step_detector == 1U) || (int2_ctrl.int2_step_detector == 1U))
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

  return ISM6HG256X_OK;
}

/**
  * @brief  Enable free fall detection
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Enable_Free_Fall_Detection(ISM6HG256X_Object_t *pObj, ISM6HG256X_SensorIntPin_t IntPin)
{
  int32_t ret = ISM6HG256X_OK;
  ism6hg256x_md1_cfg_t val1;
  ism6hg256x_md2_cfg_t val2;
  ism6hg256x_functions_enable_t functions_enable;

  /* Output Data Rate selection */
  if (ISM6HG256X_ACC_SetOutputDataRate(pObj, 480.0f) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Full scale selection */
  if (ISM6HG256X_ACC_SetFullScale(pObj, 2) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /*  Set free fall duration.*/
  if (ISM6HG256X_ACC_Set_Free_Fall_Duration(pObj, 3) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Set free fall threshold. */
  if (ISM6HG256X_ACC_Set_Free_Fall_Threshold(pObj, 3) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Enable free fall event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case ISM6HG256X_INT1_PIN:
      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      val1.int1_ff = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      functions_enable.interrupts_enable = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }
      break;

    case ISM6HG256X_INT2_PIN:
      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      val2.int2_ff = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      functions_enable.interrupts_enable = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }
      break;

    default:
      ret = ISM6HG256X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Disable free fall detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Disable_Free_Fall_Detection(ISM6HG256X_Object_t *pObj)
{
  ism6hg256x_md1_cfg_t val1;
  ism6hg256x_md2_cfg_t val2;

  /* Disable free fall event on both INT1 and INT2 pins */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  val1.int1_ff = PROPERTY_DISABLE;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  val2.int2_ff = PROPERTY_DISABLE;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Reset free fall threshold. */
  if (ISM6HG256X_ACC_Set_Free_Fall_Threshold(pObj, 0) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Reset free fall duration */
  if (ISM6HG256X_ACC_Set_Free_Fall_Duration(pObj, 0) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Set free fall threshold
  * @param  pObj the device pObj
  * @param  Threshold free fall detection threshold
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Set_Free_Fall_Threshold(ISM6HG256X_Object_t *pObj, uint8_t Threshold)
{
  ism6hg256x_ff_thresholds_t val;
  switch (Threshold)
  {
    case ISM6HG256X_156_mg:
      val = ISM6HG256X_156_mg;
      break;

    case ISM6HG256X_219_mg:
      val = ISM6HG256X_219_mg;
      break;

    case ISM6HG256X_250_mg:
      val = ISM6HG256X_250_mg;
      break;

    case ISM6HG256X_312_mg:
      val = ISM6HG256X_312_mg;
      break;

    case ISM6HG256X_344_mg:
      val = ISM6HG256X_344_mg;
      break;

    case ISM6HG256X_406_mg:
      val = ISM6HG256X_406_mg;
      break;

    case ISM6HG256X_469_mg:
      val = ISM6HG256X_469_mg;
      break;

    case ISM6HG256X_500_mg:
      val = ISM6HG256X_500_mg;
      break;

    default:
      val = ISM6HG256X_156_mg;
      break;
  }

  /* Set free fall threshold. */
  if (ism6hg256x_ff_thresholds_set(&(pObj->Ctx), val) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Set free fall duration
  * @param  pObj the device pObj
  * @param  Duration free fall detection duration
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Set_Free_Fall_Duration(ISM6HG256X_Object_t *pObj, uint8_t Duration)
{
  /* Set free fall duration. */
  if (ism6hg256x_ff_time_windows_set(&(pObj->Ctx), Duration) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }
  return ISM6HG256X_OK;
}

/**
  * @brief  Enable tilt detection
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Enable_Tilt_Detection(ISM6HG256X_Object_t *pObj, ISM6HG256X_SensorIntPin_t IntPin)
{
  int32_t ret = ISM6HG256X_OK;
  ism6hg256x_md1_cfg_t val1;
  ism6hg256x_md2_cfg_t val2;

  ism6hg256x_emb_func_en_a_t emb_func_en_a;
  ism6hg256x_emb_func_int1_t emb_func_int1;
  ism6hg256x_emb_func_int2_t emb_func_int2;

  /* Output Data Rate selection */
  if (ISM6HG256X_ACC_SetOutputDataRate(pObj, 30.0f) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Full scale selection */
  if (ISM6HG256X_ACC_SetFullScale(pObj, 2) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  switch (IntPin)
  {
    case ISM6HG256X_INT1_PIN:
      /* Enable access to embedded functions registers */
      if (ism6hg256x_mem_bank_set(&(pObj->Ctx), ISM6HG256X_EMBED_FUNC_MEM_BANK) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      /* Enable tilt detection */
      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      emb_func_en_a.tilt_en = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      /* Tilt interrupt driven to INT1 pin */
      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      emb_func_int1.int1_tilt = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      /* Disable access to embedded functions registers */
      if (ism6hg256x_mem_bank_set(&(pObj->Ctx), ISM6HG256X_MAIN_MEM_BANK) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      /* Enable routing the embedded functions interrupt */
      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      val1.int1_emb_func = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }
      break;

    case ISM6HG256X_INT2_PIN:
      /* Enable access to embedded functions registers */
      if (ism6hg256x_mem_bank_set(&(pObj->Ctx), ISM6HG256X_EMBED_FUNC_MEM_BANK) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      /* Enable tilt detection */
      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      emb_func_en_a.tilt_en = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      /* Tilt interrupt driven to INT2 pin */
      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      emb_func_int2.int2_tilt = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      /* Disable access to embedded functions registers */
      if (ism6hg256x_mem_bank_set(&(pObj->Ctx), ISM6HG256X_MAIN_MEM_BANK) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      /* Enable routing the embedded functions interrupt */
      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      val2.int2_emb_func = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }
      break;

    default:
      ret = ISM6HG256X_ERROR;
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
int32_t ISM6HG256X_ACC_Disable_Tilt_Detection(ISM6HG256X_Object_t *pObj)
{
  int32_t ret = ISM6HG256X_OK;
  ism6hg256x_md1_cfg_t val1;
  ism6hg256x_md2_cfg_t val2;

  ism6hg256x_emb_func_en_a_t emb_func_en_a;
  ism6hg256x_emb_func_int1_t emb_func_int1;
  ism6hg256x_emb_func_int2_t emb_func_int2;

  /* Disable emb func event on either INT1 or INT2 pin */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  val1.int1_emb_func = PROPERTY_DISABLE;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  val2.int2_emb_func = PROPERTY_DISABLE;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Enable access to embedded functions registers. */
  if (ism6hg256x_mem_bank_set(&(pObj->Ctx), ISM6HG256X_EMBED_FUNC_MEM_BANK) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Disable tilt detection. */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  emb_func_en_a.tilt_en = PROPERTY_DISABLE;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Reset interrupt driven to INT1 pin. */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  emb_func_int1.int1_tilt = PROPERTY_DISABLE;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Reset interrupt driven to INT2 pin. */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  emb_func_int2.int2_tilt = PROPERTY_DISABLE;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Disable access to embedded functions registers. */
  if (ism6hg256x_mem_bank_set(&(pObj->Ctx), ISM6HG256X_MAIN_MEM_BANK) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ret;
}

/**
  * @brief  Enable pedometer
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Enable_Pedometer(ISM6HG256X_Object_t *pObj, ISM6HG256X_SensorIntPin_t IntPin)
{
  int32_t ret = ISM6HG256X_OK;
  ism6hg256x_stpcnt_mode_t mode;
  ism6hg256x_md1_cfg_t val1;
  ism6hg256x_md2_cfg_t val2;
  ism6hg256x_emb_func_int1_t emb_func_int1;
  ism6hg256x_emb_func_int2_t emb_func_int2;

  /* Output Data Rate selection */
  if (ISM6HG256X_ACC_SetOutputDataRate(pObj, 30.0f) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Full scale selection */
  if (ISM6HG256X_ACC_SetFullScale(pObj, 8) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  if (ism6hg256x_stpcnt_mode_get(&(pObj->Ctx), &mode) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Enable pedometer algorithm. */
  mode.step_counter_enable = PROPERTY_ENABLE;
  mode.false_step_rej = PROPERTY_DISABLE;

  /* Turn on embedded features */
  if (ism6hg256x_stpcnt_mode_set(&(pObj->Ctx), mode) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Enable free fall event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case ISM6HG256X_INT1_PIN:
      /* Enable access to embedded functions registers. */
      if (ism6hg256x_mem_bank_set(&(pObj->Ctx), ISM6HG256X_EMBED_FUNC_MEM_BANK) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      /* Step detector interrupt driven to INT1 pin */
      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      emb_func_int1.int1_step_detector = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      /* Disable access to embedded functions registers */
      if (ism6hg256x_mem_bank_set(&(pObj->Ctx), ISM6HG256X_MAIN_MEM_BANK) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      val1.int1_emb_func = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }
      break;

    case ISM6HG256X_INT2_PIN:
      /* Enable access to embedded functions registers. */
      if (ism6hg256x_mem_bank_set(&(pObj->Ctx), ISM6HG256X_EMBED_FUNC_MEM_BANK) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      /* Step detector interrupt driven to INT1 pin */
      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      emb_func_int2.int2_step_detector = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      /* Disable access to embedded functions registers */
      if (ism6hg256x_mem_bank_set(&(pObj->Ctx), ISM6HG256X_MAIN_MEM_BANK) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      val2.int2_emb_func = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }
      break;

    default:
      ret = ISM6HG256X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Disable pedometer
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Disable_Pedometer(ISM6HG256X_Object_t *pObj)
{
  ism6hg256x_md1_cfg_t val1;
  ism6hg256x_md2_cfg_t val2;

  ism6hg256x_emb_func_int1_t emb_func_int1;
  ism6hg256x_emb_func_int2_t emb_func_int2;

  ism6hg256x_stpcnt_mode_t mode;


  if (ism6hg256x_stpcnt_mode_get(&(pObj->Ctx), &mode) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Enable pedometer algorithm. */
  mode.step_counter_enable = PROPERTY_DISABLE;
  mode.false_step_rej = PROPERTY_DISABLE;

  /* Turn off embedded features */
  if (ism6hg256x_stpcnt_mode_set(&(pObj->Ctx), mode) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Disable emb func event on either INT1 or INT2 pin */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  val1.int1_emb_func = PROPERTY_DISABLE;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  val2.int2_emb_func = PROPERTY_DISABLE;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Enable access to embedded functions registers. */
  if (ism6hg256x_mem_bank_set(&(pObj->Ctx), ISM6HG256X_EMBED_FUNC_MEM_BANK) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Reset interrupt driven to INT1 pin. */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  emb_func_int1.int1_step_detector = PROPERTY_DISABLE;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Reset interrupt driven to INT2 pin. */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  emb_func_int2.int2_step_detector = PROPERTY_DISABLE;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Disable access to embedded functions registers. */
  if (ism6hg256x_mem_bank_set(&(pObj->Ctx), ISM6HG256X_MAIN_MEM_BANK) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }


  return ISM6HG256X_OK;
}

/**
  * @brief  Get step count
  * @param  pObj the device pObj
  * @param  StepCount step counter
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Get_Step_Count(ISM6HG256X_Object_t *pObj, uint16_t *StepCount)
{
  if (ism6hg256x_stpcnt_steps_get(&(pObj->Ctx), StepCount) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Enable step counter reset
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Step_Counter_Reset(ISM6HG256X_Object_t *pObj)
{
  if (ism6hg256x_stpcnt_rst_step_set(&(pObj->Ctx), PROPERTY_ENABLE) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}


/**
  * @brief  Enable wake up detection
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Enable_Wake_Up_Detection(ISM6HG256X_Object_t *pObj, ISM6HG256X_SensorIntPin_t IntPin)
{
  int32_t ret = ISM6HG256X_OK;
  ism6hg256x_md1_cfg_t val1;
  ism6hg256x_md2_cfg_t val2;
  ism6hg256x_functions_enable_t functions_enable;

  /* Output Data Rate selection */
  if (ISM6HG256X_ACC_SetOutputDataRate(pObj, 480.0f) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Full scale selection */
  if (ISM6HG256X_ACC_SetFullScale(pObj, 2) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Set wake-up threshold */
  if (ISM6HG256X_ACC_Set_Wake_Up_Threshold(pObj, 63) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Set wake-up durantion */
  if (ISM6HG256X_ACC_Set_Wake_Up_Duration(pObj, 0) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Enable wake up event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case ISM6HG256X_INT1_PIN:
      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      val1.int1_wu = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      functions_enable.interrupts_enable = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }
      break;

    case ISM6HG256X_INT2_PIN:
      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      val2.int2_wu = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      functions_enable.interrupts_enable = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }
      break;

    default:
      ret = ISM6HG256X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Disable wake up detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Disable_Wake_Up_Detection(ISM6HG256X_Object_t *pObj)
{
  ism6hg256x_md1_cfg_t val1;
  ism6hg256x_md2_cfg_t val2;

  /* Disable wake up event on both INT1 and INT2 pins */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  val1.int1_wu = PROPERTY_DISABLE;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  val2.int2_wu = PROPERTY_DISABLE;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Reset wake-up threshold */
  if (ISM6HG256X_ACC_Set_Wake_Up_Threshold(pObj, 0) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Reset wake-up durantion */
  if (ISM6HG256X_ACC_Set_Wake_Up_Duration(pObj, 0) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Set wake up threshold
  * @param  pObj the device pObj
  * @param  Threshold wake up detection threshold
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Set_Wake_Up_Threshold(ISM6HG256X_Object_t *pObj, uint32_t Threshold)
{
  ism6hg256x_act_thresholds_t wake_up_ths;

  if (ism6hg256x_act_thresholds_get(&(pObj->Ctx), &wake_up_ths) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  wake_up_ths.threshold = (uint8_t)Threshold;

  if (ism6hg256x_act_thresholds_set(&(pObj->Ctx), &wake_up_ths) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Set wake up duration
  * @param  pObj the device pObj
  * @param  Duration wake up detection duration
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Set_Wake_Up_Duration(ISM6HG256X_Object_t *pObj, uint8_t Duration)
{
  ism6hg256x_act_wkup_time_windows_t dur_t;

  if (ism6hg256x_act_wkup_time_windows_get(&(pObj->Ctx), &dur_t) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  dur_t.shock = Duration;

  if (ism6hg256x_act_wkup_time_windows_set(&(pObj->Ctx), dur_t) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Enable single tap detection
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Enable_Single_Tap_Detection(ISM6HG256X_Object_t *pObj, ISM6HG256X_SensorIntPin_t IntPin)
{
  int32_t ret = ISM6HG256X_OK;
  ism6hg256x_md1_cfg_t val1;
  ism6hg256x_md2_cfg_t val2;
  ism6hg256x_functions_enable_t functions_enable;

  ism6hg256x_tap_dur_t tap_dur;
  ism6hg256x_tap_cfg0_t tap_cfg0;
  ism6hg256x_tap_ths_6d_t tap_ths_6d;

  /* Output Data Rate selection */
  if (ISM6HG256X_ACC_SetOutputDataRate(pObj, 480.0f) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Full scale selection */
  if (ISM6HG256X_ACC_SetFullScale(pObj, 8) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Enable tap detection on Z-axis. */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_TAP_CFG0, (uint8_t *)&tap_cfg0, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  tap_cfg0.tap_z_en = 0x01U;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_TAP_CFG0, (uint8_t *)&tap_cfg0, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Set Z-axis threshold. */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  tap_ths_6d.tap_ths_z = 0x2U;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Set quiet and shock time windows. */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_TAP_DUR, (uint8_t *)&tap_dur, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  tap_dur.quiet = (uint8_t)0x02U;
  tap_dur.shock = (uint8_t)0x01U;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_TAP_DUR, (uint8_t *)&tap_dur, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Set tap mode. */
  if (ism6hg256x_tap_mode_set(&(pObj->Ctx), ISM6HG256X_ONLY_SINGLE) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Enable single tap event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case ISM6HG256X_INT1_PIN:
      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      val1.int1_single_tap = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      functions_enable.interrupts_enable = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }
      break;

    case ISM6HG256X_INT2_PIN:
      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      val2.int2_single_tap = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      functions_enable.interrupts_enable = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }
      break;

    default:
      ret = ISM6HG256X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Disable single tap detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Disable_Single_Tap_Detection(ISM6HG256X_Object_t *pObj)
{
  ism6hg256x_md1_cfg_t val1;
  ism6hg256x_md2_cfg_t val2;

  ism6hg256x_tap_dur_t tap_dur;
  ism6hg256x_tap_cfg0_t tap_cfg0;
  ism6hg256x_tap_ths_6d_t tap_ths_6d;


  /* Disable single tap event on both INT1 and INT2 pins */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  val1.int1_single_tap = PROPERTY_DISABLE;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  val2.int2_single_tap = PROPERTY_DISABLE;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Disable tap detection on Z-axis. */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_TAP_CFG0, (uint8_t *)&tap_cfg0, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  tap_cfg0.tap_z_en = 0x0U;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_TAP_CFG0, (uint8_t *)&tap_cfg0, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Reset Z-axis threshold. */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  tap_ths_6d.tap_ths_z = 0x0U;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Reset quiet and shock time windows. */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_TAP_DUR, (uint8_t *)&tap_dur, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  tap_dur.quiet = (uint8_t)0x0U;
  tap_dur.shock = (uint8_t)0x0U;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_TAP_DUR, (uint8_t *)&tap_dur, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Enable double tap detection
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Enable_Double_Tap_Detection(ISM6HG256X_Object_t *pObj, ISM6HG256X_SensorIntPin_t IntPin)
{
  int32_t ret = ISM6HG256X_OK;
  ism6hg256x_md1_cfg_t val1;
  ism6hg256x_md2_cfg_t val2;
  ism6hg256x_functions_enable_t functions_enable;

  ism6hg256x_tap_dur_t tap_dur;
  ism6hg256x_tap_cfg0_t tap_cfg0;
  ism6hg256x_tap_ths_6d_t tap_ths_6d;


  /* Enable tap detection on Z-axis. */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_TAP_CFG0, (uint8_t *)&tap_cfg0, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  tap_cfg0.tap_z_en = 0x01U;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_TAP_CFG0, (uint8_t *)&tap_cfg0, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Set Z-axis threshold. */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  tap_ths_6d.tap_ths_z = 0x03U;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Set quiet shock and duration. */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_TAP_DUR, (uint8_t *)&tap_dur, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  tap_dur.dur = (uint8_t)0x03U;
  tap_dur.quiet = (uint8_t)0x02U;
  tap_dur.shock = (uint8_t)0x02U;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_TAP_DUR, (uint8_t *)&tap_dur, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Set tap mode. */
  if (ism6hg256x_tap_mode_set(&(pObj->Ctx), ISM6HG256X_BOTH_SINGLE_DOUBLE) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Output Data Rate selection */
  if (ISM6HG256X_ACC_SetOutputDataRate(pObj, 480.0f) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Full scale selection */
  if (ISM6HG256X_ACC_SetFullScale(pObj, 8) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Enable double tap event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case ISM6HG256X_INT1_PIN:
      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      val1.int1_double_tap = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      functions_enable.interrupts_enable = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }
      break;

    case ISM6HG256X_INT2_PIN:
      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      val2.int2_double_tap = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      functions_enable.interrupts_enable = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }
      break;

    default:
      ret = ISM6HG256X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Disable double tap detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Disable_Double_Tap_Detection(ISM6HG256X_Object_t *pObj)
{
  ism6hg256x_md1_cfg_t val1;
  ism6hg256x_md2_cfg_t val2;

  ism6hg256x_tap_dur_t tap_dur;
  ism6hg256x_tap_cfg0_t tap_cfg0;
  ism6hg256x_tap_ths_6d_t tap_ths_6d;

  /* Disable double tap event on both INT1 and INT2 pins */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  val1.int1_ff = PROPERTY_DISABLE;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  val2.int2_ff = PROPERTY_DISABLE;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Disable tap detection on Z-axis. */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_TAP_CFG0, (uint8_t *)&tap_cfg0, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  tap_cfg0.tap_z_en = 0x0U;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_TAP_CFG0, (uint8_t *)&tap_cfg0, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Reset Z-axis threshold. */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  tap_ths_6d.tap_ths_z = 0x0U;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_TAP_CFG0, (uint8_t *)&tap_ths_6d, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Reset quiet shock and duration. */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_TAP_DUR, (uint8_t *)&tap_dur, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  tap_dur.dur = (uint8_t)0x0U;
  tap_dur.quiet = (uint8_t)0x0U;
  tap_dur.shock = (uint8_t)0x0U;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_TAP_DUR, (uint8_t *)&tap_dur, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Set tap mode. */
  if (ism6hg256x_tap_mode_set(&(pObj->Ctx), ISM6HG256X_ONLY_SINGLE) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }


  return ISM6HG256X_OK;
}

/**
  * @brief  Set tap threshold
  * @param  pObj the device pObj
  * @param  Threshold tap threshold
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Set_Tap_Threshold(ISM6HG256X_Object_t *pObj, uint8_t Threshold)
{
  ism6hg256x_tap_ths_6d_t tap_ths_6d;

  /* Set Z-axis threshold */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  tap_ths_6d.tap_ths_z = Threshold;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_TAP_CFG0, (uint8_t *)&tap_ths_6d, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Set tap duration time
  * @param  pObj the device pObj
  * @param  Time shock duration time
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Set_Tap_Shock_Time(ISM6HG256X_Object_t *pObj, uint8_t Time)
{
  ism6hg256x_tap_dur_t tap_dur;

  /* Set shock time */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_TAP_DUR, (uint8_t *)&tap_dur, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  tap_dur.shock = Time;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_TAP_DUR, (uint8_t *)&tap_dur, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }
  return ISM6HG256X_OK;
}

/**
  * @brief  Set tap duration time
  * @param  pObj the device pObj
  * @param  Time tap quiet time
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Set_Tap_Quiet_Time(ISM6HG256X_Object_t *pObj, uint8_t Time)
{
  ism6hg256x_tap_dur_t tap_dur;

  /* Set quiet time */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_TAP_DUR, (uint8_t *)&tap_dur, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  tap_dur.quiet = Time;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_TAP_DUR, (uint8_t *)&tap_dur, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }
  return ISM6HG256X_OK;
}



/**
  * @brief  Set tap duration time
  * @param  pObj the device pObj
  * @param  Time tap duration time
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Set_Tap_Duration_Time(ISM6HG256X_Object_t *pObj, uint8_t Time)
{
  ism6hg256x_tap_dur_t tap_dur;

  /* Set duration time */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_TAP_DUR, (uint8_t *)&tap_dur, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  tap_dur.dur = Time;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_TAP_DUR, (uint8_t *)&tap_dur, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }
  return ISM6HG256X_OK;
}


/**
  * @brief  Enable 6d orientation
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Enable_6D_Orientation(ISM6HG256X_Object_t *pObj, ISM6HG256X_SensorIntPin_t IntPin)
{
  int32_t ret = ISM6HG256X_OK;
  ism6hg256x_md1_cfg_t val1;
  ism6hg256x_md2_cfg_t val2;
  ism6hg256x_functions_enable_t functions_enable;

  /* Output Data Rate selection */
  if (ISM6HG256X_ACC_SetOutputDataRate(pObj, 480.0f) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Full scale selection */
  if (ISM6HG256X_ACC_SetFullScale(pObj, 2) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Threshold selection*/
  if (ISM6HG256X_ACC_Set_6D_Orientation_Threshold(pObj, 2) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Enable 6D orientation event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case ISM6HG256X_INT1_PIN:
      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      val1.int1_6d = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      functions_enable.interrupts_enable = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }
      break;

    case ISM6HG256X_INT2_PIN:
      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      val2.int2_6d = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      functions_enable.interrupts_enable = PROPERTY_ENABLE;

      if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }
      break;

    default:
      ret = ISM6HG256X_ERROR;
      break;
  }

  return ret;

}


/**
  * @brief  Disable 6D orientation detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Disable_6D_Orientation(ISM6HG256X_Object_t *pObj)
{
  ism6hg256x_md1_cfg_t val1;
  ism6hg256x_md2_cfg_t val2;

  /* Reset threshold */
  if (ISM6HG256X_ACC_Set_6D_Orientation_Threshold(pObj, 0) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Disable 6D orientation event on both INT1 and INT2 pins */
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  val1.int1_6d = PROPERTY_DISABLE;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD1_CFG, (uint8_t *)&val1, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  val2.int2_6d = PROPERTY_DISABLE;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_MD2_CFG, (uint8_t *)&val2, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Set 6D orientation threshold
  * @param  pObj the device pObj
  * @param  Threshold free fall detection threshold
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Set_6D_Orientation_Threshold(ISM6HG256X_Object_t *pObj, uint8_t Threshold)
{
  int32_t ret = ISM6HG256X_OK;
  ism6hg256x_6d_threshold_t newThreshold = ISM6HG256X_DEG_80;

  switch (Threshold)
  {
    case 0:
      newThreshold = ISM6HG256X_DEG_80;
      break;
    case 1:
      newThreshold = ISM6HG256X_DEG_70;
      break;
    case 2:
      newThreshold = ISM6HG256X_DEG_60;
      break;
    case 3:
      newThreshold = ISM6HG256X_DEG_50;
      break;
    default:
      ret = ISM6HG256X_ERROR;
      break;
  }

  if (ret == ISM6HG256X_ERROR)
  {
    return ISM6HG256X_ERROR;
  }

  if (ism6hg256x_6d_threshold_set(&(pObj->Ctx), newThreshold) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;

}

/**
  * @brief  Get the status of XLow orientation
  * @param  pObj the device pObj
  * @param  XLow the status of XLow orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Get_6D_Orientation_XL(ISM6HG256X_Object_t *pObj, uint8_t *XLow)
{
  ism6hg256x_d6d_src_t data;

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_D6D_SRC, (uint8_t *)&data, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  *XLow = data.xl;

  return ISM6HG256X_OK;
}

/**
  * @brief  Get the status of XHigh orientation
  * @param  pObj the device pObj
  * @param  XHigh the status of XHigh orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Get_6D_Orientation_XH(ISM6HG256X_Object_t *pObj, uint8_t *XHigh)
{
  ism6hg256x_d6d_src_t data;

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_D6D_SRC, (uint8_t *)&data, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  *XHigh = data.xh;

  return ISM6HG256X_OK;
}

/**
  * @brief  Get the status of YLow orientation
  * @param  pObj the device pObj
  * @param  YLow the status of YLow orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Get_6D_Orientation_YL(ISM6HG256X_Object_t *pObj, uint8_t *YLow)
{
  ism6hg256x_d6d_src_t data;

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_D6D_SRC, (uint8_t *)&data, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  *YLow = data.yl;

  return ISM6HG256X_OK;
}

/**
  * @brief  Get the status of YHigh orientation
  * @param  pObj the device pObj
  * @param  YHigh the status of YHigh orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Get_6D_Orientation_YH(ISM6HG256X_Object_t *pObj, uint8_t *YHigh)
{
  ism6hg256x_d6d_src_t data;

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_D6D_SRC, (uint8_t *)&data, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  *YHigh = data.yh;

  return ISM6HG256X_OK;
}

/**
  * @brief  Get the status of ZLow orientation
  * @param  pObj the device pObj
  * @param  ZLow the status of ZLow orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Get_6D_Orientation_ZL(ISM6HG256X_Object_t *pObj, uint8_t *ZLow)
{
  ism6hg256x_d6d_src_t data;

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_D6D_SRC, (uint8_t *)&data, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  *ZLow = data.zl;

  return ISM6HG256X_OK;
}

/**
  * @brief  Get the status of ZHigh orientation
  * @param  pObj the device pObj
  * @param  ZHigh the status of ZHigh orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Get_6D_Orientation_ZH(ISM6HG256X_Object_t *pObj, uint8_t *ZHigh)
{
  ism6hg256x_d6d_src_t data;

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_D6D_SRC, (uint8_t *)&data, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  *ZHigh = data.zh;

  return ISM6HG256X_OK;
}

/**
  * @brief  Enable the ISM6HG256X high-g accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_HG_Enable(ISM6HG256X_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_hg_is_enabled == 1U)
  {
    return ISM6HG256X_OK;
  }

  /* Output data rate selection. */
  if (ism6hg256x_hg_xl_data_rate_set(&(pObj->Ctx), pObj->acc_hg_odr, PROPERTY_ENABLE) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  pObj->acc_hg_is_enabled = 1;

  return ISM6HG256X_OK;
}

/**
  * @brief  Disable the ISM6HG256X high-g accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_HG_Disable(ISM6HG256X_Object_t *pObj)
{
  uint8_t output_enable;

  /* Check if the component is already disabled */
  if (pObj->acc_hg_is_enabled == 0U)
  {
    return ISM6HG256X_OK;
  }

  /* Get current output data rate. */
  if (ism6hg256x_hg_xl_data_rate_get(&(pObj->Ctx), &pObj->acc_hg_odr, &output_enable) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Output data rate selection - power down. */
  if (ism6hg256x_hg_xl_data_rate_set(&(pObj->Ctx), ISM6HG256X_HG_XL_ODR_OFF, PROPERTY_ENABLE) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  pObj->acc_hg_is_enabled = 0;

  return ISM6HG256X_OK;
}

/**
  * @brief  Get the ISM6HG256X high-g accelerometer sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_HG_GetSensitivity(ISM6HG256X_Object_t *pObj, float_t *Sensitivity)
{
  int32_t ret = ISM6HG256X_OK;
  ism6hg256x_hg_xl_full_scale_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (ism6hg256x_hg_xl_full_scale_get(&(pObj->Ctx), &full_scale) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Store the Sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case ISM6HG256X_32g:
      *Sensitivity = ISM6HG256X_ACC_SENSITIVITY_FS_32G;
      break;

    case ISM6HG256X_64g:
      *Sensitivity = ISM6HG256X_ACC_SENSITIVITY_FS_64G;
      break;

    case ISM6HG256X_128g:
      *Sensitivity = ISM6HG256X_ACC_SENSITIVITY_FS_128G;
      break;

    case ISM6HG256X_256g:
      *Sensitivity = ISM6HG256X_ACC_SENSITIVITY_FS_256G;
      break;

    default:
      ret = ISM6HG256X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the ISM6HG256X high-g accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_HG_GetOutputDataRate(ISM6HG256X_Object_t *pObj, float_t *Odr)
{
  int32_t ret = ISM6HG256X_OK;
  ism6hg256x_hg_xl_data_rate_t odr_low_level;
  uint8_t output_enable;

  /* Get current output data rate. */
  if (ism6hg256x_hg_xl_data_rate_get(&(pObj->Ctx), &odr_low_level, &output_enable) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  switch (odr_low_level)
  {
    case ISM6HG256X_HG_XL_ODR_OFF:
      *Odr = 0.0f;
      break;

    case ISM6HG256X_HG_XL_ODR_AT_480Hz:
      *Odr = 480.0f;
      break;

    case ISM6HG256X_HG_XL_ODR_AT_960Hz:
      *Odr = 960.0f;
      break;

    case ISM6HG256X_HG_XL_ODR_AT_1920Hz:
      *Odr = 1920.0f;
      break;

    case ISM6HG256X_HG_XL_ODR_AT_3840Hz:
      *Odr = 3840.0f;
      break;

    case ISM6HG256X_HG_XL_ODR_AT_7680Hz:
      *Odr = 7680.0f;
      break;

    default:
      ret = ISM6HG256X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the ISM6HG256X high-g accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_HG_SetOutputDataRate(ISM6HG256X_Object_t *pObj, float_t Odr)
{
  if (pObj->acc_is_enabled == 1U)
  {
    return ISM6HG256X_ACC_HG_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return ISM6HG256X_ACC_HG_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the ISM6HG256X high-g accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_HG_GetFullScale(ISM6HG256X_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = ISM6HG256X_OK;
  ism6hg256x_hg_xl_full_scale_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (ism6hg256x_hg_xl_full_scale_get(&(pObj->Ctx), &fs_low_level) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  switch (fs_low_level)
  {
    case ISM6HG256X_32g:
      *FullScale =  32;
      break;

    case ISM6HG256X_64g:
      *FullScale =  64;
      break;

    case ISM6HG256X_128g:
      *FullScale =  128;
      break;

    case ISM6HG256X_256g:
      *FullScale =  256;
      break;

    default:
      ret = ISM6HG256X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the ISM6HG256X high-g accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_HG_SetFullScale(ISM6HG256X_Object_t *pObj, int32_t FullScale)
{
  ism6hg256x_hg_xl_full_scale_t new_fs;

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file static analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  new_fs = (FullScale <= 32) ?  ISM6HG256X_32g
         : (FullScale <= 64) ?  ISM6HG256X_64g
         : (FullScale <= 128) ? ISM6HG256X_128g
         :                      ISM6HG256X_256g;

  if (ism6hg256x_hg_xl_full_scale_set(&(pObj->Ctx), new_fs) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Get the ISM6HG256X high-g accelerometer sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_HG_GetAxesRaw(ISM6HG256X_Object_t *pObj, ISM6HG256X_AxesRaw_t *Value)
{
  ism6hg256x_axis3bit16_t data_raw;

  /* Read raw data values. */
  if (ism6hg256x_hg_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return ISM6HG256X_OK;
}

/**
  * @brief  Get the ISM6HG256X high-g accelerometer sensor axes
  * @param  pObj the device pObj
  * @param  Acceleration pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_HG_GetAxes(ISM6HG256X_Object_t *pObj, ISM6HG256X_Axes_t *Acceleration)
{
  ism6hg256x_axis3bit16_t data_raw;
  float_t sensitivity = 0.0f;

  /* Read raw data values. */
  if (ism6hg256x_hg_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Get ISM6HG256X actual sensitivity. */
  if (ISM6HG256X_ACC_HG_GetSensitivity(pObj, &sensitivity) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Calculate the data. */
  Acceleration->x = (int32_t)((float_t)((float_t)data_raw.i16bit[0] * sensitivity));
  Acceleration->y = (int32_t)((float_t)((float_t)data_raw.i16bit[1] * sensitivity));
  Acceleration->z = (int32_t)((float_t)((float_t)data_raw.i16bit[2] * sensitivity));

  return ISM6HG256X_OK;
}

/**
  * @brief  Get the ISM6HG256X FIFO number of samples
  * @param  pObj the device pObj
  * @param  NumSamples number of samples
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_FIFO_Get_Num_Samples(ISM6HG256X_Object_t *pObj, uint16_t *NumSamples)
{
  ism6hg256x_fifo_status_t val;

  if (ism6hg256x_fifo_status_get(&(pObj->Ctx), &val) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  *NumSamples = val.fifo_level;

  return ISM6HG256X_OK;

}

/**
  * @brief  Get the ISM6HG256X FIFO full status
  * @param  pObj the device pObj
  * @param  Status FIFO full status
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_FIFO_Get_Full_Status(ISM6HG256X_Object_t *pObj, uint8_t *Status)
{
  ism6hg256x_fifo_status_t val;

  if (ism6hg256x_fifo_status_get(&(pObj->Ctx), &val) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  *Status = val.fifo_full;

  return ISM6HG256X_OK;
}

/**
  * @brief  Set the ISM6HG256X FIFO full interrupt on INT1 pin
  * @param  pObj the device pObj
  * @param  Status FIFO full interrupt on INT1 pin status
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_FIFO_Set_INT1_FIFO_Full(ISM6HG256X_Object_t *pObj, uint8_t Status)
{
  ism6hg256x_int1_ctrl_t reg;

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_INT1_CTRL, (uint8_t *)&reg, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  reg.int1_fifo_full = Status;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_INT1_CTRL, (uint8_t *)&reg, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Set the ISM6HG256X FIFO full interrupt on INT2 pin
  * @param  pObj the device pObj
  * @param  Status FIFO full interrupt on INT1 pin status
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_FIFO_Set_INT2_FIFO_Full(ISM6HG256X_Object_t *pObj, uint8_t Status)
{
  ism6hg256x_int2_ctrl_t reg;

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_INT2_CTRL, (uint8_t *)&reg, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  reg.int2_fifo_full = Status;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_INT2_CTRL, (uint8_t *)&reg, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Set the ISM6HG256X FIFO watermark level
  * @param  pObj the device pObj
  * @param  Watermark FIFO watermark level
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_FIFO_Set_Watermark_Level(ISM6HG256X_Object_t *pObj, uint8_t Watermark)
{
  if (ism6hg256x_fifo_watermark_set(&(pObj->Ctx), Watermark) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Set the ISM6HG256X FIFO stop on watermark
  * @param  pObj the device pObj
  * @param  Status FIFO stop on watermark status
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_FIFO_Set_Stop_On_Fth(ISM6HG256X_Object_t *pObj, uint8_t Status)
{
  if (ism6hg256x_fifo_stop_on_wtm_set(&(pObj->Ctx), Status) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Set the ISM6HG256X FIFO mode
  * @param  pObj the device pObj
  * @param  Mode FIFO mode
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_FIFO_Set_Mode(ISM6HG256X_Object_t *pObj, uint8_t Mode)
{
  int32_t ret = ISM6HG256X_OK;
  ism6hg256x_fifo_mode_t newMode = ISM6HG256X_BYPASS_MODE;

  switch (Mode)
  {
    case 0:
      newMode = ISM6HG256X_BYPASS_MODE;
      break;
    case 1:
      newMode = ISM6HG256X_FIFO_MODE;
      break;
    case 3:
      newMode = ISM6HG256X_STREAM_TO_FIFO_MODE;
      break;
    case 4:
      newMode = ISM6HG256X_BYPASS_TO_STREAM_MODE;
      break;
    case 6:
      newMode = ISM6HG256X_STREAM_MODE;
      break;
    case 7:
      newMode = ISM6HG256X_BYPASS_TO_FIFO_MODE;
      break;
    default:
      ret = ISM6HG256X_ERROR;
      break;
  }

  if (ret == ISM6HG256X_ERROR)
  {
    return ISM6HG256X_ERROR;
  }

  if (ism6hg256x_fifo_mode_set(&(pObj->Ctx), newMode) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ret;
}

/**
  * @brief  Get the ISM6HG256X FIFO tag
  * @param  pObj the device pObj
  * @param  Tag FIFO tag
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_FIFO_Get_Tag(ISM6HG256X_Object_t *pObj, uint8_t *Tag)
{
  ism6hg256x_fifo_data_out_tag_t tag_local;

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_FIFO_DATA_OUT_TAG, (uint8_t *)&tag_local, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  *Tag = (uint8_t)tag_local.tag_sensor;

  return ISM6HG256X_OK;
}

/**
  * @brief  Get the ISM6HG256X FIFO raw data
  * @param  pObj the device pObj
  * @param  Data FIFO raw data array [6]
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_FIFO_Get_Data(ISM6HG256X_Object_t *pObj, uint8_t *Data)
{
  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_FIFO_DATA_OUT_X_L, Data, 6) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Get the ISM6HG256X FIFO accelero single sample (16-bit data per 3 axes) and calculate acceleration [mg]
  * @param  pObj the device pObj
  * @param  Acceleration FIFO accelero axes [mg]
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_FIFO_ACC_Get_Axes(ISM6HG256X_Object_t *pObj, ISM6HG256X_Axes_t *Acceleration)
{
  ism6hg256x_axis3bit16_t data_raw;
  float_t sensitivity = 0.0f;
  float_t acceleration_float_t[3];

  if (ISM6HG256X_FIFO_Get_Data(pObj, data_raw.u8bit) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  if (ISM6HG256X_ACC_GetSensitivity(pObj, &sensitivity) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }
  acceleration_float_t[0] = (float_t)data_raw.i16bit[0] * sensitivity;
  acceleration_float_t[1] = (float_t)data_raw.i16bit[1] * sensitivity;
  acceleration_float_t[2] = (float_t)data_raw.i16bit[2] * sensitivity;

  Acceleration->x = (int32_t)acceleration_float_t[0];
  Acceleration->y = (int32_t)acceleration_float_t[1];
  Acceleration->z = (int32_t)acceleration_float_t[2];

  return ISM6HG256X_OK;

}

/**
  * @brief  Set the ISM6HG256X FIFO accelero BDR value
  * @param  pObj the device pObj
  * @param  Bdr FIFO accelero BDR value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_FIFO_ACC_Set_BDR(ISM6HG256X_Object_t *pObj, float_t Bdr)
{
  ism6hg256x_fifo_xl_batch_t new_bdr;

  new_bdr = (Bdr <=    0.0f) ? ISM6HG256X_XL_NOT_BATCHED
          : (Bdr <=    1.8f) ? ISM6HG256X_XL_BATCHED_AT_1Hz875
          : (Bdr <=    7.5f) ? ISM6HG256X_XL_BATCHED_AT_7Hz5
          : (Bdr <=   15.0f) ? ISM6HG256X_XL_BATCHED_AT_15Hz
          : (Bdr <=   30.0f) ? ISM6HG256X_XL_BATCHED_AT_30Hz
          : (Bdr <=   60.0f) ? ISM6HG256X_XL_BATCHED_AT_60Hz
          : (Bdr <=  120.0f) ? ISM6HG256X_XL_BATCHED_AT_120Hz
          : (Bdr <=  240.0f) ? ISM6HG256X_XL_BATCHED_AT_240Hz
          : (Bdr <=  480.0f) ? ISM6HG256X_XL_BATCHED_AT_480Hz
          : (Bdr <=  960.0f) ? ISM6HG256X_XL_BATCHED_AT_960Hz
          : (Bdr <= 1920.0f) ? ISM6HG256X_XL_BATCHED_AT_1920Hz
          : (Bdr <= 3840.0f) ? ISM6HG256X_XL_BATCHED_AT_3840Hz
          :                    ISM6HG256X_XL_BATCHED_AT_7680Hz;

  if (ism6hg256x_fifo_xl_batch_set(&(pObj->Ctx), new_bdr) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Get the ISM6HG256X FIFO gyro single sample (16-bit data per 3 axes) and calculate angular velocity [mDPS]
  * @param  pObj the device pObj
  * @param  AngularVelocity FIFO gyro axes [mDPS]
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_FIFO_GYRO_Get_Axes(ISM6HG256X_Object_t *pObj, ISM6HG256X_Axes_t *AngularVelocity)
{
  ism6hg256x_axis3bit16_t data_raw;
  float_t sensitivity = 0.0f;
  float_t angular_velocity_float_t[3];

  if (ISM6HG256X_FIFO_Get_Data(pObj, data_raw.u8bit) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  if (ISM6HG256X_GYRO_GetSensitivity(pObj, &sensitivity) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  angular_velocity_float_t[0] = (float_t)data_raw.i16bit[0] * sensitivity;
  angular_velocity_float_t[1] = (float_t)data_raw.i16bit[1] * sensitivity;
  angular_velocity_float_t[2] = (float_t)data_raw.i16bit[2] * sensitivity;

  AngularVelocity->x = (int32_t)angular_velocity_float_t[0];
  AngularVelocity->y = (int32_t)angular_velocity_float_t[1];
  AngularVelocity->z = (int32_t)angular_velocity_float_t[2];

  return ISM6HG256X_OK;
}

/**
  * @brief  Set the ISM6HG256X FIFO gyro BDR value
  * @param  pObj the device pObj
  * @param  Bdr FIFO gyro BDR value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_FIFO_GYRO_Set_BDR(ISM6HG256X_Object_t *pObj, float_t Bdr)
{
  ism6hg256x_fifo_gy_batch_t new_bdr;

  new_bdr = (Bdr <=    0.0f) ? ISM6HG256X_GY_NOT_BATCHED
          : (Bdr <=    1.8f) ? ISM6HG256X_GY_BATCHED_AT_1Hz875
          : (Bdr <=    7.5f) ? ISM6HG256X_GY_BATCHED_AT_7Hz5
          : (Bdr <=   15.0f) ? ISM6HG256X_GY_BATCHED_AT_15Hz
          : (Bdr <=   30.0f) ? ISM6HG256X_GY_BATCHED_AT_30Hz
          : (Bdr <=   60.0f) ? ISM6HG256X_GY_BATCHED_AT_60Hz
          : (Bdr <=  120.0f) ? ISM6HG256X_GY_BATCHED_AT_120Hz
          : (Bdr <=  240.0f) ? ISM6HG256X_GY_BATCHED_AT_240Hz
          : (Bdr <=  480.0f) ? ISM6HG256X_GY_BATCHED_AT_480Hz
          : (Bdr <=  960.0f) ? ISM6HG256X_GY_BATCHED_AT_960Hz
          : (Bdr <= 1920.0f) ? ISM6HG256X_GY_BATCHED_AT_1920Hz
          : (Bdr <= 3840.0f) ? ISM6HG256X_GY_BATCHED_AT_3840Hz
          :                    ISM6HG256X_GY_BATCHED_AT_7680Hz;

  if (ism6hg256x_fifo_gy_batch_set(&(pObj->Ctx), new_bdr) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Enable the ISM6HG256X gyroscope sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_GYRO_Enable(ISM6HG256X_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->gyro_is_enabled == 1U)
  {
    return ISM6HG256X_OK;
  }

  /* Output data rate selection. */
  if (ism6hg256x_gy_data_rate_set(&(pObj->Ctx), pObj->gyro_odr) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  pObj->gyro_is_enabled = 1;

  return ISM6HG256X_OK;
}

/**
  * @brief  Disable the ISM6HG256X gyroscope sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_GYRO_Disable(ISM6HG256X_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->gyro_is_enabled == 0U)
  {
    return ISM6HG256X_OK;
  }

  /* Get current output data rate. */
  if (ism6hg256x_gy_data_rate_get(&(pObj->Ctx), &pObj->gyro_odr) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Output data rate selection - power down. */
  if (ism6hg256x_gy_data_rate_set(&(pObj->Ctx), ISM6HG256X_ODR_OFF) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  pObj->gyro_is_enabled = 0;

  return ISM6HG256X_OK;
}

/**
  * @brief  Get the ISM6HG256X gyroscope sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_GYRO_GetSensitivity(ISM6HG256X_Object_t *pObj, float_t *Sensitivity)
{
  int32_t ret = ISM6HG256X_OK;
  ism6hg256x_gy_full_scale_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (ism6hg256x_gy_full_scale_get(&(pObj->Ctx), &full_scale) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Store the sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case ISM6HG256X_250dps:
      *Sensitivity = ISM6HG256X_GYRO_SENSITIVITY_FS_250DPS;
      break;

    case ISM6HG256X_500dps:
      *Sensitivity = ISM6HG256X_GYRO_SENSITIVITY_FS_500DPS;
      break;

    case ISM6HG256X_1000dps:
      *Sensitivity = ISM6HG256X_GYRO_SENSITIVITY_FS_1000DPS;
      break;

    case ISM6HG256X_2000dps:
      *Sensitivity = ISM6HG256X_GYRO_SENSITIVITY_FS_2000DPS;
      break;

    case ISM6HG256X_4000dps:
      *Sensitivity = ISM6HG256X_GYRO_SENSITIVITY_FS_4000DPS;
      break;

    default:
      ret = ISM6HG256X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the ISM6HG256X gyroscope sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_GYRO_GetOutputDataRate(ISM6HG256X_Object_t *pObj, float_t *Odr)
{
  int32_t ret = ISM6HG256X_OK;
  ism6hg256x_data_rate_t odr_low_level;

  /* Get current output data rate. */
  if (ism6hg256x_gy_data_rate_get(&(pObj->Ctx), &odr_low_level) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  switch (odr_low_level)
  {
    case ISM6HG256X_ODR_OFF:
      *Odr = 0.0f;
      break;

    case ISM6HG256X_ODR_AT_7Hz5:
      *Odr = 7.5f;
      break;

    case ISM6HG256X_ODR_AT_15Hz:
      *Odr = 15.0f;
      break;

    case ISM6HG256X_ODR_AT_30Hz:
      *Odr = 30.0f;
      break;

    case ISM6HG256X_ODR_AT_60Hz:
      *Odr = 60.0f;
      break;

    case ISM6HG256X_ODR_AT_120Hz:
      *Odr = 120.0f;
      break;

    case ISM6HG256X_ODR_AT_240Hz:
      *Odr = 240.0f;
      break;

    case ISM6HG256X_ODR_AT_480Hz:
      *Odr = 480.0f;
      break;

    case ISM6HG256X_ODR_AT_960Hz:
      *Odr = 960.0f;
      break;

    case ISM6HG256X_ODR_AT_1920Hz:
      *Odr = 1920.0f;
      break;

    case ISM6HG256X_ODR_AT_3840Hz:
      *Odr = 3840.0f;
      break;

    case ISM6HG256X_ODR_AT_7680Hz:
      *Odr = 7680.0f;
      break;

    default:
      ret = ISM6HG256X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the ISM6HG256X gyroscope sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_GYRO_SetOutputDataRate(ISM6HG256X_Object_t *pObj, float_t Odr)
{
  if (pObj->gyro_is_enabled == 1U)
  {
    return ISM6HG256X_GYRO_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return ISM6HG256X_GYRO_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Set the ISM6HG256X gyroscope sensor output data rate with operating mode
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @param  Mode the gyroscope operating mode
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_GYRO_SetOutputDataRate_With_Mode(ISM6HG256X_Object_t *pObj, float_t Odr,
                                                    ISM6HG256X_GYRO_Operating_Mode_t Mode)
{
  int32_t ret = ISM6HG256X_OK;
  float_t odr_new = 0.0f;

  switch (Mode)
  {
    case ISM6HG256X_GYRO_HIGH_PERFORMANCE_MODE:
      if (ism6hg256x_gy_mode_set(&(pObj->Ctx), ISM6HG256X_GY_HIGH_PERFORMANCE_MD) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 7.68kHz */
      odr_new = (Odr <    7.5f) ?    7.5f
              : (Odr > 7680.0f) ? 7680.0f
              :                       Odr;
      break;

    case ISM6HG256X_GYRO_HIGH_ACCURACY_ODR_MODE:
      ret = ISM6HG256X_ERROR;
      break;

    case ISM6HG256X_GYRO_ODR_TRIGGERED_MODE:
      ret = ISM6HG256X_ERROR;
      break;

    case ISM6HG256X_GYRO_SLEEP_MODE:
      ret = ISM6HG256X_ERROR;
      break;

    case ISM6HG256X_GYRO_LOW_POWER_MODE:
      if (ism6hg256x_gy_mode_set(&(pObj->Ctx), ISM6HG256X_GY_LOW_POWER_MD) != ISM6HG256X_OK)
      {
        return ISM6HG256X_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 240kHz */
      odr_new = (Odr <   7.5f) ?   7.5f
              : (Odr > 240.0f) ? 240.0f
              :                     Odr;
      break;

    default:
      ret = ISM6HG256X_ERROR;
      break;
  }

  if (ret != ISM6HG256X_OK)
  {
    return ret;
  }

  if (pObj->gyro_is_enabled == 1U)
  {
    ret = ISM6HG256X_GYRO_SetOutputDataRate_When_Enabled(pObj, odr_new);
  }
  else
  {
    ret = ISM6HG256X_GYRO_SetOutputDataRate_When_Disabled(pObj, odr_new);
  }

  return ret;
}

/**
  * @brief  Get the ISM6HG256X gyroscope sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_GYRO_GetFullScale(ISM6HG256X_Object_t *pObj, int32_t  *FullScale)
{
  int32_t ret = ISM6HG256X_OK;
  ism6hg256x_gy_full_scale_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (ism6hg256x_gy_full_scale_get(&(pObj->Ctx), &fs_low_level) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  switch (fs_low_level)
  {
    case ISM6HG256X_250dps:
      *FullScale =  250;
      break;

    case ISM6HG256X_500dps:
      *FullScale =  500;
      break;

    case ISM6HG256X_1000dps:
      *FullScale = 1000;
      break;

    case ISM6HG256X_2000dps:
      *FullScale = 2000;
      break;

    case ISM6HG256X_4000dps:
      *FullScale = 4000;
      break;

    default:
      ret = ISM6HG256X_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the ISM6HG256X gyroscope sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_GYRO_SetFullScale(ISM6HG256X_Object_t *pObj, int32_t FullScale)
{
  ism6hg256x_gy_full_scale_t new_fs;

  new_fs = (FullScale <=  250) ? ISM6HG256X_250dps
         : (FullScale <=  500) ? ISM6HG256X_500dps
         : (FullScale <= 1000) ? ISM6HG256X_1000dps
         : (FullScale <= 2000) ? ISM6HG256X_2000dps
         :                       ISM6HG256X_4000dps;

  if (ism6hg256x_gy_full_scale_set(&(pObj->Ctx), new_fs) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Get the ISM6HG256X gyroscope sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_GYRO_GetAxesRaw(ISM6HG256X_Object_t *pObj, ISM6HG256X_AxesRaw_t *Value)
{
  ism6hg256x_axis3bit16_t data_raw;

  /* Read raw data values. */
  if (ism6hg256x_angular_rate_raw_get(&(pObj->Ctx), data_raw.i16bit) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return ISM6HG256X_OK;
}

/**
  * @brief  Get the ISM6HG256X gyroscope sensor axes
  * @param  pObj the device pObj
  * @param  AngularRate pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_GYRO_GetAxes(ISM6HG256X_Object_t *pObj, ISM6HG256X_Axes_t *AngularRate)
{
  ism6hg256x_axis3bit16_t data_raw;
  float_t sensitivity = 0.0f;

  /* Read raw data values. */
  if (ism6hg256x_angular_rate_raw_get(&(pObj->Ctx), data_raw.i16bit) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Get ISM6HG256X actual sensitivity. */
  if (ISM6HG256X_GYRO_GetSensitivity(pObj, &sensitivity) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  /* Calculate the data. */
  AngularRate->x = (int32_t)((float_t)((float_t)data_raw.i16bit[0] * sensitivity));
  AngularRate->y = (int32_t)((float_t)((float_t)data_raw.i16bit[1] * sensitivity));
  AngularRate->z = (int32_t)((float_t)((float_t)data_raw.i16bit[2] * sensitivity));

  return ISM6HG256X_OK;
}

/**
  * @brief  Get the ISM6HG256X register value
  * @param  pObj the device pObj
  * @param  Reg address to be read
  * @param  Data pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_Read_Reg(ISM6HG256X_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (ism6hg256x_read_reg(&(pObj->Ctx), Reg, Data, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Set the ISM6HG256X register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_Write_Reg(ISM6HG256X_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (ism6hg256x_write_reg(&(pObj->Ctx), Reg, &Data, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Get the ISM6HG256X ACC data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Get_DRDY_Status(ISM6HG256X_Object_t *pObj, uint8_t *Status)
{
  ism6hg256x_data_ready_t val;

  if (ism6hg256x_flag_data_ready_get(&(pObj->Ctx), &val) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  *Status = val.drdy_xl;
  return ISM6HG256X_OK;
}

/**
  * @brief  Get the ISM6HG256X high-g ACC data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_HG_Get_DRDY_Status(ISM6HG256X_Object_t *pObj, uint8_t *Status)
{
  ism6hg256x_data_ready_t val;

  if (ism6hg256x_flag_data_ready_get(&(pObj->Ctx), &val) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  *Status = val.drdy_hgxl;
  return ISM6HG256X_OK;
}

/**
  * @brief  Get the ISM6HG256X GYRO data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_GYRO_Get_DRDY_Status(ISM6HG256X_Object_t *pObj, uint8_t *Status)
{
  ism6hg256x_data_ready_t val;

  if (ism6hg256x_flag_data_ready_get(&(pObj->Ctx), &val) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  *Status = val.drdy_gy;
  return ISM6HG256X_OK;
}

/**
  * @brief  Set memory bank
  * @param  pObj the device pObj
  * @param  Val the value of memory bank in reg FUNC_CFG_ACCESS
  *         0 - ISM6HG256X_MAIN_MEM_BANK, 1 - ISM6HG256X_EMBED_FUNC_MEM_BANK, 2 - ISM6HG256X_SENSOR_HUB_MEM_BANK
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_Set_Mem_Bank(ISM6HG256X_Object_t *pObj, uint8_t Val)
{
  int32_t ret = ISM6HG256X_OK;
  ism6hg256x_mem_bank_t reg;

  reg = (Val == 1U) ? ISM6HG256X_EMBED_FUNC_MEM_BANK
      : (Val == 2U) ? ISM6HG256X_SENSOR_HUB_MEM_BANK
      :               ISM6HG256X_MAIN_MEM_BANK;

  if (ism6hg256x_mem_bank_set(&(pObj->Ctx), reg) != ISM6HG256X_OK)
  {
    ret = ISM6HG256X_ERROR;
  }

  return ret;
}

/**
  * @}
  */

/** @defgroup ISM6HG256X_Private_Functions ISM6HG256X Private Functions
  * @{
  */

/**
  * @brief  Set the ISM6HG256X accelerometer sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t ISM6HG256X_ACC_SetOutputDataRate_When_Enabled(ISM6HG256X_Object_t *pObj, float_t Odr)
{
  ism6hg256x_data_rate_t new_odr;

  new_odr = (Odr <=    1.875f) ? ISM6HG256X_ODR_AT_1Hz875
          : (Odr <=    7.5f)   ? ISM6HG256X_ODR_AT_7Hz5
          : (Odr <=   15.0f)   ? ISM6HG256X_ODR_AT_15Hz
          : (Odr <=   30.0f)   ? ISM6HG256X_ODR_AT_30Hz
          : (Odr <=   60.0f)   ? ISM6HG256X_ODR_AT_60Hz
          : (Odr <=  120.0f)   ? ISM6HG256X_ODR_AT_120Hz
          : (Odr <=  240.0f)   ? ISM6HG256X_ODR_AT_240Hz
          : (Odr <=  480.0f)   ? ISM6HG256X_ODR_AT_480Hz
          : (Odr <=  960.0f)   ? ISM6HG256X_ODR_AT_960Hz
          : (Odr <= 1920.0f)   ? ISM6HG256X_ODR_AT_1920Hz
          : (Odr <= 3840.0f)   ? ISM6HG256X_ODR_AT_3840Hz
          :                      ISM6HG256X_ODR_AT_7680Hz;

  /* Output data rate selection. */
  if (ism6hg256x_xl_data_rate_set(&(pObj->Ctx), new_odr) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Set the ISM6HG256X accelerometer sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t ISM6HG256X_ACC_SetOutputDataRate_When_Disabled(ISM6HG256X_Object_t *pObj, float_t Odr)
{
  pObj->acc_odr = (Odr <=    1.875f) ? ISM6HG256X_ODR_AT_1Hz875
                : (Odr <=    7.5f)   ? ISM6HG256X_ODR_AT_7Hz5
                : (Odr <=   15.0f)   ? ISM6HG256X_ODR_AT_15Hz
                : (Odr <=   30.0f)   ? ISM6HG256X_ODR_AT_30Hz
                : (Odr <=   60.0f)   ? ISM6HG256X_ODR_AT_60Hz
                : (Odr <=  120.0f)   ? ISM6HG256X_ODR_AT_120Hz
                : (Odr <=  240.0f)   ? ISM6HG256X_ODR_AT_240Hz
                : (Odr <=  480.0f)   ? ISM6HG256X_ODR_AT_480Hz
                : (Odr <=  960.0f)   ? ISM6HG256X_ODR_AT_960Hz
                : (Odr <= 1920.0f)   ? ISM6HG256X_ODR_AT_1920Hz
                : (Odr <= 3840.0f)   ? ISM6HG256X_ODR_AT_3840Hz
                :                      ISM6HG256X_ODR_AT_7680Hz;

  return ISM6HG256X_OK;
}

/**
  * @brief  Set the ISM6HG256X high-g accelerometer sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t ISM6HG256X_ACC_HG_SetOutputDataRate_When_Enabled(ISM6HG256X_Object_t *pObj, float_t Odr)
{
  ism6hg256x_hg_xl_data_rate_t new_odr;

  new_odr = (Odr <=  480.0f) ? ISM6HG256X_HG_XL_ODR_AT_480Hz
          : (Odr <=  960.0f) ? ISM6HG256X_HG_XL_ODR_AT_960Hz
          : (Odr <= 1920.0f) ? ISM6HG256X_HG_XL_ODR_AT_1920Hz
          : (Odr <= 3840.0f) ? ISM6HG256X_HG_XL_ODR_AT_3840Hz
          :                    ISM6HG256X_HG_XL_ODR_AT_7680Hz;

  /* Output data rate selection. */
  if (ism6hg256x_hg_xl_data_rate_set(&(pObj->Ctx), new_odr, PROPERTY_ENABLE) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Set the ISM6HG256X high-g accelerometer sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t ISM6HG256X_ACC_HG_SetOutputDataRate_When_Disabled(ISM6HG256X_Object_t *pObj, float_t Odr)
{
  pObj->acc_hg_odr = (Odr <=  480.0f) ? ISM6HG256X_HG_XL_ODR_AT_480Hz
                   : (Odr <=  960.0f) ? ISM6HG256X_HG_XL_ODR_AT_960Hz
                   : (Odr <= 1920.0f) ? ISM6HG256X_HG_XL_ODR_AT_1920Hz
                   : (Odr <= 3840.0f) ? ISM6HG256X_HG_XL_ODR_AT_3840Hz
                   :                    ISM6HG256X_HG_XL_ODR_AT_7680Hz;

  return ISM6HG256X_OK;
}

/**
  * @brief  Set the ISM6HG256X gyroscope sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t ISM6HG256X_GYRO_SetOutputDataRate_When_Enabled(ISM6HG256X_Object_t *pObj, float_t Odr)
{
  ism6hg256x_data_rate_t new_odr;

  new_odr = (Odr <=    7.5f) ? ISM6HG256X_ODR_AT_7Hz5
          : (Odr <=   15.0f) ? ISM6HG256X_ODR_AT_15Hz
          : (Odr <=   30.0f) ? ISM6HG256X_ODR_AT_30Hz
          : (Odr <=   60.0f) ? ISM6HG256X_ODR_AT_60Hz
          : (Odr <=  120.0f) ? ISM6HG256X_ODR_AT_120Hz
          : (Odr <=  240.0f) ? ISM6HG256X_ODR_AT_240Hz
          : (Odr <=  480.0f) ? ISM6HG256X_ODR_AT_480Hz
          : (Odr <=  960.0f) ? ISM6HG256X_ODR_AT_960Hz
          : (Odr <= 1920.0f) ? ISM6HG256X_ODR_AT_1920Hz
          : (Odr <= 3840.0f) ? ISM6HG256X_ODR_AT_3840Hz
          :                    ISM6HG256X_ODR_AT_7680Hz;

  /* Output data rate selection. */
  if (ism6hg256x_gy_data_rate_set(&(pObj->Ctx), new_odr) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Set the ISM6HG256X gyroscope sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t ISM6HG256X_GYRO_SetOutputDataRate_When_Disabled(ISM6HG256X_Object_t *pObj, float_t Odr)
{
  pObj->gyro_odr = (Odr <=    7.5f) ? ISM6HG256X_ODR_AT_7Hz5
                 : (Odr <=   15.0f) ? ISM6HG256X_ODR_AT_15Hz
                 : (Odr <=   30.0f) ? ISM6HG256X_ODR_AT_30Hz
                 : (Odr <=   60.0f) ? ISM6HG256X_ODR_AT_60Hz
                 : (Odr <=  120.0f) ? ISM6HG256X_ODR_AT_120Hz
                 : (Odr <=  240.0f) ? ISM6HG256X_ODR_AT_240Hz
                 : (Odr <=  480.0f) ? ISM6HG256X_ODR_AT_480Hz
                 : (Odr <=  960.0f) ? ISM6HG256X_ODR_AT_960Hz
                 : (Odr <= 1920.0f) ? ISM6HG256X_ODR_AT_1920Hz
                 : (Odr <= 3840.0f) ? ISM6HG256X_ODR_AT_3840Hz
                 :                    ISM6HG256X_ODR_AT_7680Hz;

  return ISM6HG256X_OK;
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
  ISM6HG256X_Object_t *pObj = (ISM6HG256X_Object_t *)Handle;

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
  ISM6HG256X_Object_t *pObj = (ISM6HG256X_Object_t *)Handle;

  return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
}

/**
  * @brief  Set the ISM6HG256X accelerometer power mode
  * @param  pObj the device pObj
  * @param  PowerMode Value of the powerMode
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Set_Power_Mode(ISM6HG256X_Object_t *pObj, uint8_t PowerMode)
{
  ism6hg256x_xl_mode_t new_mode;

  new_mode = (PowerMode == 0x00U) ? ISM6HG256X_XL_HIGH_PERFORMANCE_MD
           : (PowerMode == 0x01U) ? ISM6HG256X_XL_HIGH_ACCURACY_ODR_MD
           : (PowerMode == 0x03U) ? ISM6HG256X_XL_ODR_TRIGGERED_MD
           : (PowerMode == 0x04U) ? ISM6HG256X_XL_LOW_POWER_2_AVG_MD
           : (PowerMode == 0x05U) ? ISM6HG256X_XL_LOW_POWER_4_AVG_MD
           : (PowerMode == 0x06U) ? ISM6HG256X_XL_LOW_POWER_8_AVG_MD
           :                        ISM6HG256X_XL_NORMAL_MD;

  if (ism6hg256x_xl_mode_set(&(pObj->Ctx), new_mode) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Set the ISM6HG256X gyroscope power mode
  * @param  pObj the device pObj
  * @param  PowerMode Value of the powerMode
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_GYRO_Set_Power_Mode(ISM6HG256X_Object_t *pObj, uint8_t PowerMode)
{
  ism6hg256x_gy_mode_t new_mode;

  new_mode = (PowerMode == 0x00U) ? ISM6HG256X_GY_HIGH_PERFORMANCE_MD
           : (PowerMode == 0x01U) ? ISM6HG256X_GY_HIGH_ACCURACY_ODR_MD
           : (PowerMode == 0x03U) ? ISM6HG256X_GY_ODR_TRIGGERED_MD
           : (PowerMode == 0x04U) ? ISM6HG256X_GY_SLEEP_MD
           :                        ISM6HG256X_GY_LOW_POWER_MD;

  if (ism6hg256x_gy_mode_set(&(pObj->Ctx), new_mode) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Set the ISM6HG256X accelerometer filter mode
  * @param  pObj the device pObj
  * @param  LowHighPassFlag 0/1 for setting low-pass/high-pass filter mode
  * @param  FilterMode Value of the filter Mode
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_ACC_Set_Filter_Mode(ISM6HG256X_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode)
{
  ism6hg256x_filt_xl_lp2_bandwidth_t new_mode;

  new_mode = (FilterMode == 0x00U) ? ISM6HG256X_XL_ULTRA_LIGHT
           : (FilterMode == 0x01U) ? ISM6HG256X_XL_VERY_LIGHT
           : (FilterMode == 0x02U) ? ISM6HG256X_XL_LIGHT
           : (FilterMode == 0x03U) ? ISM6HG256X_XL_MEDIUM
           : (FilterMode == 0x04U) ? ISM6HG256X_XL_STRONG
           : (FilterMode == 0x05U) ? ISM6HG256X_XL_VERY_STRONG
           : (FilterMode == 0x06U) ? ISM6HG256X_XL_AGGRESSIVE
           :                         ISM6HG256X_XL_XTREME;

  if (LowHighPassFlag == 0U)
  {
    /*Set accelerometer low_pass filter-mode*/
    /*Set to 1 LPF2 bit (CTRL8_XL)*/
    if (ism6hg256x_filt_xl_lp2_set(&(pObj->Ctx), 1) != ISM6HG256X_OK)
    {
      return ISM6HG256X_ERROR;
    }
    if (ism6hg256x_filt_xl_lp2_bandwidth_set(&(pObj->Ctx), new_mode) != ISM6HG256X_OK)
    {
      return ISM6HG256X_ERROR;
    }
  }
  else
  {
    if (ism6hg256x_filt_xl_lp2_set(&(pObj->Ctx), 0) != ISM6HG256X_OK)
    {
      return ISM6HG256X_ERROR;
    }
    /*Set accelerometer high_pass filter-mode*/
    if (ism6hg256x_filt_xl_lp2_bandwidth_set(&(pObj->Ctx), new_mode) != ISM6HG256X_OK)
    {
      return ISM6HG256X_ERROR;
    }
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Set the ISM6HG256X gyroscope filter mode
  * @param  pObj the device pObj
  * @param  LowHighPassFlag 0/1 for setting low-pass/high-pass filter mode
  * @param  FilterMode Value of the filter Mode
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_GYRO_Set_Filter_Mode(ISM6HG256X_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode)
{
  ism6hg256x_filt_gy_lp1_bandwidth_t new_mode;

  new_mode = (FilterMode == 0x00U) ? ISM6HG256X_GY_ULTRA_LIGHT
           : (FilterMode == 0x01U) ? ISM6HG256X_GY_VERY_LIGHT
           : (FilterMode == 0x02U) ? ISM6HG256X_GY_LIGHT
           : (FilterMode == 0x03U) ? ISM6HG256X_GY_MEDIUM
           : (FilterMode == 0x04U) ? ISM6HG256X_GY_STRONG
           : (FilterMode == 0x05U) ? ISM6HG256X_GY_VERY_STRONG
           : (FilterMode == 0x06U) ? ISM6HG256X_GY_AGGRESSIVE
           :                         ISM6HG256X_GY_XTREME;

  if (LowHighPassFlag == 0U)
  {
    /*Set gyroscope low_pass 1 filter-mode*/
    /* Enable low-pass filter */
    if (ism6hg256x_filt_gy_lp1_set(&(pObj->Ctx), 1) != ISM6HG256X_OK)
    {
      return ISM6HG256X_ERROR;
    }
    if (ism6hg256x_filt_gy_lp1_bandwidth_set(&(pObj->Ctx), new_mode) != ISM6HG256X_OK)
    {
      return ISM6HG256X_ERROR;
    }
  }
  else
  {
    /*Set gyroscope high_pass filter-mode*/
    /* Enable high-pass filter */
    if (ism6hg256x_filt_gy_lp1_set(&(pObj->Ctx), 0) != ISM6HG256X_OK)
    {
      return ISM6HG256X_ERROR;
    }
    if (ism6hg256x_filt_gy_lp1_bandwidth_set(&(pObj->Ctx), new_mode) != ISM6HG256X_OK)
    {
      return ISM6HG256X_ERROR;
    }
  }

  return ISM6HG256X_OK;
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
