/**
 ******************************************************************************
 * @file    lsm6dsv.c
 * @author  MEMS Software Solutions Team
 * @brief   LSM6DSV driver file
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
#include "lsm6dsv.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @defgroup LSM6DSV LSM6DSV
 * @{
 */

/** @defgroup LSM6DSV_Exported_Variables LSM6DSV Exported Variables
 * @{
 */

LSM6DSV_CommonDrv_t LSM6DSV_COMMON_Driver =
{
  LSM6DSV_Init,
  LSM6DSV_DeInit,
  LSM6DSV_ReadID,
  LSM6DSV_GetCapabilities,
};

LSM6DSV_ACC_Drv_t LSM6DSV_ACC_Driver =
{
  LSM6DSV_ACC_Enable,
  LSM6DSV_ACC_Disable,
  LSM6DSV_ACC_GetSensitivity,
  LSM6DSV_ACC_GetOutputDataRate,
  LSM6DSV_ACC_SetOutputDataRate,
  LSM6DSV_ACC_GetFullScale,
  LSM6DSV_ACC_SetFullScale,
  LSM6DSV_ACC_GetAxes,
  LSM6DSV_ACC_GetAxesRaw,
};

LSM6DSV_GYRO_Drv_t LSM6DSV_GYRO_Driver =
{
  LSM6DSV_GYRO_Enable,
  LSM6DSV_GYRO_Disable,
  LSM6DSV_GYRO_GetSensitivity,
  LSM6DSV_GYRO_GetOutputDataRate,
  LSM6DSV_GYRO_SetOutputDataRate,
  LSM6DSV_GYRO_GetFullScale,
  LSM6DSV_GYRO_SetFullScale,
  LSM6DSV_GYRO_GetAxes,
  LSM6DSV_GYRO_GetAxesRaw,
};

/**
 * @}
 */

/** @defgroup LSM6DSV_Private_Function_Prototypes LSM6DSV Private Function Prototypes
 * @{
 */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t LSM6DSV_ACC_SetOutputDataRate_When_Enabled(LSM6DSV_Object_t *pObj, float_t Odr);
static int32_t LSM6DSV_ACC_SetOutputDataRate_When_Disabled(LSM6DSV_Object_t *pObj, float_t Odr);
static int32_t LSM6DSV_GYRO_SetOutputDataRate_When_Enabled(LSM6DSV_Object_t *pObj, float_t Odr);
static int32_t LSM6DSV_GYRO_SetOutputDataRate_When_Disabled(LSM6DSV_Object_t *pObj, float_t Odr);

/**
 * @}
 */

/** @defgroup LSM6DSV_Exported_Functions LSM6DSV Exported Functions
 * @{
 */

/**
 * @brief  Register Component Bus IO operations
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_RegisterBusIO(LSM6DSV_Object_t *pObj, LSM6DSV_IO_t *pIO)
{
  int32_t ret = LSM6DSV_OK;

  if (pObj == NULL)
  {
    ret = LSM6DSV_ERROR;
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
    pObj->Ctx.handle   = pObj;

    if (pObj->IO.Init == NULL)
    {
      ret = LSM6DSV_ERROR;
    }
    else if (pObj->IO.Init() != LSM6DSV_OK)
    {
      ret = LSM6DSV_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == LSM6DSV_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x04;

          if (LSM6DSV_Write_Reg(pObj, LSM6DSV_CTRL3, data) != LSM6DSV_OK)
          {
            ret = LSM6DSV_ERROR;
          }

          if (LSM6DSV_Write_Reg(pObj, LSM6DSV_IF_CFG, data) != LSM6DSV_OK)
          {
            ret = LSM6DSV_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
 * @brief  Initialize the LSM6DSV sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_Init(LSM6DSV_Object_t *pObj)
{
  /* Set main memory bank */
  if (LSM6DSV_Set_Mem_Bank(pObj, (uint8_t)LSM6DSV_MAIN_MEM_BANK) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  /* Enable register address automatically incremented during a multiple byte
  access with a serial interface. */
  if (lsm6dsv_auto_increment_set(&(pObj->Ctx), PROPERTY_ENABLE) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  /* Enable BDU */
  if (lsm6dsv_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  /* FIFO mode selection */
  if (lsm6dsv_fifo_mode_set(&(pObj->Ctx), LSM6DSV_BYPASS_MODE) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = LSM6DSV_ODR_AT_120Hz;

  /* Output data rate selection - power down. */
  if (lsm6dsv_xl_data_rate_set(&(pObj->Ctx), LSM6DSV_ODR_OFF) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  /* Full scale selection. */
  if (lsm6dsv_xl_full_scale_set(&(pObj->Ctx), LSM6DSV_2g) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  /* Select default output data rate. */
  pObj->gyro_odr = LSM6DSV_ODR_AT_120Hz;

  /* Output data rate selection - power down. */
  if (lsm6dsv_gy_data_rate_set(&(pObj->Ctx), LSM6DSV_ODR_OFF) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  /* Full scale selection. */
  if (lsm6dsv_gy_full_scale_set(&(pObj->Ctx), LSM6DSV_2000dps) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  pObj->is_initialized = 1;

  return LSM6DSV_OK;
}

/**
 * @brief  Deinitialize the LSM6DSV sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_DeInit(LSM6DSV_Object_t *pObj)
{
  /* Disable the component */
  if (LSM6DSV_ACC_Disable(pObj) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  if (LSM6DSV_GYRO_Disable(pObj) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  /* Reset output data rate. */
  pObj->acc_odr = LSM6DSV_ODR_OFF;
  pObj->gyro_odr = LSM6DSV_ODR_OFF;

  pObj->is_initialized = 0;

  return LSM6DSV_OK;
}

/**
 * @brief  Read component ID
 * @param  pObj the device pObj
 * @param  Id the WHO_AM_I value
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_ReadID(LSM6DSV_Object_t *pObj, uint8_t *Id)
{
  if (lsm6dsv_device_id_get(&(pObj->Ctx), Id) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  return LSM6DSV_OK;
}

/**
 * @brief  Get LSM6DSV sensor capabilities
 * @param  pObj Component object pointer
 * @param  Capabilities pointer to LSM6DSV sensor capabilities
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_GetCapabilities(LSM6DSV_Object_t *pObj, LSM6DSV_Capabilities_t *Capabilities)
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
  return LSM6DSV_OK;
}

/**
 * @brief  Enable the LSM6DSV accelerometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_ACC_Enable(LSM6DSV_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return LSM6DSV_OK;
  }

  /* Output data rate selection. */
  if (lsm6dsv_xl_data_rate_set(&(pObj->Ctx), pObj->acc_odr) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return LSM6DSV_OK;
}

/**
 * @brief  Disable the LSM6DSV accelerometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_ACC_Disable(LSM6DSV_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return LSM6DSV_OK;
  }

  /* Get current output data rate. */
  if (lsm6dsv_xl_data_rate_get(&(pObj->Ctx), &pObj->acc_odr) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  /* Output data rate selection - power down. */
  if (lsm6dsv_xl_data_rate_set(&(pObj->Ctx), LSM6DSV_ODR_OFF) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return LSM6DSV_OK;
}

/**
 * @brief  Get the LSM6DSV accelerometer sensor sensitivity
 * @param  pObj the device pObj
 * @param  Sensitivity pointer
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_ACC_GetSensitivity(LSM6DSV_Object_t *pObj, float_t *Sensitivity)
{
  int32_t ret = LSM6DSV_OK;
  lsm6dsv_xl_full_scale_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (lsm6dsv_xl_full_scale_get(&(pObj->Ctx), &full_scale) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  /* Store the Sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case LSM6DSV_2g:
      *Sensitivity = LSM6DSV_ACC_SENSITIVITY_FS_2G;
      break;

    case LSM6DSV_4g:
      *Sensitivity = LSM6DSV_ACC_SENSITIVITY_FS_4G;
      break;

    case LSM6DSV_8g:
      *Sensitivity = LSM6DSV_ACC_SENSITIVITY_FS_8G;
      break;

    case LSM6DSV_16g:
      *Sensitivity = LSM6DSV_ACC_SENSITIVITY_FS_16G;
      break;

    default:
      ret = LSM6DSV_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Get the LSM6DSV accelerometer sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr pointer where the output data rate is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_ACC_GetOutputDataRate(LSM6DSV_Object_t *pObj, float_t *Odr)
{
  int32_t ret = LSM6DSV_OK;
  lsm6dsv_data_rate_t odr_low_level;

  /* Get current output data rate. */
  if (lsm6dsv_xl_data_rate_get(&(pObj->Ctx), &odr_low_level) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  switch (odr_low_level)
  {
    case LSM6DSV_ODR_OFF:
      *Odr = 0.0f;
      break;

    case LSM6DSV_ODR_AT_1Hz875:
      *Odr = 1.875f;
      break;

    case LSM6DSV_ODR_AT_7Hz5:
      *Odr = 7.5f;
      break;

    case LSM6DSV_ODR_HA02_AT_12Hz5:
      *Odr = 12.5f;
      break;

    case LSM6DSV_ODR_AT_15Hz:
      *Odr = 15.0f;
      break;

    case LSM6DSV_ODR_HA01_AT_15Hz625:
      *Odr = 15.625f;
      break;

    case LSM6DSV_ODR_HA02_AT_25Hz:
      *Odr = 25.0f;
      break;

    case LSM6DSV_ODR_AT_30Hz:
      *Odr = 30.0f;
      break;

    case LSM6DSV_ODR_HA01_AT_31Hz25:
      *Odr = 31.25f;
      break;

    case LSM6DSV_ODR_HA02_AT_50Hz:
      *Odr = 50.0f;
      break;

    case LSM6DSV_ODR_AT_60Hz:
      *Odr = 60.0f;
      break;

    case LSM6DSV_ODR_HA01_AT_62Hz5:
      *Odr = 62.5f;
      break;

    case LSM6DSV_ODR_HA02_AT_100Hz:
      *Odr = 100.0f;
      break;

    case LSM6DSV_ODR_AT_120Hz:
      *Odr = 120.0f;
      break;

    case LSM6DSV_ODR_HA01_AT_125Hz:
      *Odr = 125.0f;
      break;

    case LSM6DSV_ODR_HA02_AT_200Hz:
      *Odr = 200.0f;
      break;

    case LSM6DSV_ODR_AT_240Hz:
      *Odr = 240.0f;
      break;

    case LSM6DSV_ODR_HA01_AT_250Hz:
      *Odr = 250.0f;
      break;

    case LSM6DSV_ODR_HA02_AT_400Hz:
      *Odr = 400.0f;
      break;

    case LSM6DSV_ODR_AT_480Hz:
      *Odr = 480.0f;
      break;

    case LSM6DSV_ODR_HA01_AT_500Hz:
      *Odr = 500.0f;
      break;

    case LSM6DSV_ODR_HA02_AT_800Hz:
      *Odr = 800.0f;
      break;

    case LSM6DSV_ODR_AT_960Hz:
      *Odr = 960.0f;
      break;

    case LSM6DSV_ODR_HA01_AT_1000Hz:
      *Odr = 1000.0f;
      break;

    case LSM6DSV_ODR_HA02_AT_1600Hz:
      *Odr = 1600.0f;
      break;

    case LSM6DSV_ODR_AT_1920Hz:
      *Odr = 1920.0f;
      break;

    case LSM6DSV_ODR_HA01_AT_2000Hz:
      *Odr = 2000.0f;
      break;

    case LSM6DSV_ODR_HA02_AT_3200Hz:
      *Odr = 3200.0f;
      break;

    case LSM6DSV_ODR_AT_3840Hz:
      *Odr = 3840.0f;
      break;

    case LSM6DSV_ODR_HA01_AT_4000Hz:
      *Odr = 4000.0f;
      break;

    case LSM6DSV_ODR_HA02_AT_6400Hz:
      *Odr = 6400.0f;
      break;

    case LSM6DSV_ODR_AT_7680Hz:
      *Odr = 7680.0f;
      break;

    case LSM6DSV_ODR_HA01_AT_8000Hz:
      *Odr = 8000.0f;
      break;

    default:
      ret = LSM6DSV_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the LSM6DSV accelerometer sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_ACC_SetOutputDataRate(LSM6DSV_Object_t *pObj, float_t Odr)
{
  return LSM6DSV_ACC_SetOutputDataRate_With_Mode(pObj, Odr, LSM6DSV_ACC_HIGH_PERFORMANCE_MODE);
}

/**
 * @brief  Set the LSM6DSV accelerometer sensor output data rate with operating mode
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @param  Mode the accelerometer operating mode
 * @note   This function switches off the gyroscope if Ultra Low Power Mode is set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_ACC_SetOutputDataRate_With_Mode(LSM6DSV_Object_t *pObj, float_t Odr, LSM6DSV_ACC_Operating_Mode_t Mode)
{
  int32_t ret = LSM6DSV_OK;
  float_t new_odr = 15.0f;

  switch (Mode)
  {
    case LSM6DSV_ACC_HIGH_PERFORMANCE_MODE:
      if (lsm6dsv_xl_mode_set(&(pObj->Ctx), LSM6DSV_XL_HIGH_PERFORMANCE_MD) != LSM6DSV_OK)
      {
        ret = LSM6DSV_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 7.68kHz */
      new_odr = (Odr <    7.5f) ?    7.5f
                : (Odr > 7680.0f) ? 7680.0f
                :                       Odr;
      break;

    case LSM6DSV_ACC_HIGH_ACCURACY_MODE:
      ret = LSM6DSV_ERROR;
      break;

    case LSM6DSV_ACC_NORMAL_MODE:
      if (lsm6dsv_xl_mode_set(&(pObj->Ctx), LSM6DSV_XL_NORMAL_MD) != LSM6DSV_OK)
      {
        ret = LSM6DSV_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 1.92kHz */
      new_odr = (Odr <    7.5f) ?    7.5f
                : (Odr > 1920.0f) ? 1920.0f
                :                       Odr;
      break;

    case LSM6DSV_ACC_LOW_POWER_MODE1:
      if (lsm6dsv_xl_mode_set(&(pObj->Ctx), LSM6DSV_XL_LOW_POWER_2_AVG_MD) != LSM6DSV_OK)
      {
        ret = LSM6DSV_ERROR;
      }

      /* Valid ODR: 1.875Hz;  15Hz <= Odr <= 240kHz */
      new_odr = (Odr ==   1.875f) ?    Odr
                : (Odr <   15.000f) ?  15.0f
                : (Odr >  240.000f) ? 240.0f
                :                        Odr;
      break;

    case LSM6DSV_ACC_LOW_POWER_MODE2:
      if (lsm6dsv_xl_mode_set(&(pObj->Ctx), LSM6DSV_XL_LOW_POWER_4_AVG_MD) != LSM6DSV_OK)
      {
        ret = LSM6DSV_ERROR;
      }

      /* Valid ODR: 1.875Hz;  15Hz <= Odr <= 240kHz */
      new_odr = (Odr ==   1.875f) ?    Odr
                : (Odr <   15.000f) ?  15.0f
                : (Odr >  240.000f) ? 240.0f
                :                        Odr;
      break;

    case LSM6DSV_ACC_LOW_POWER_MODE3:
      if (lsm6dsv_xl_mode_set(&(pObj->Ctx), LSM6DSV_XL_LOW_POWER_8_AVG_MD) != LSM6DSV_OK)
      {
        ret = LSM6DSV_ERROR;
      }

      /* Valid ODR: 1.875Hz;  15Hz <= Odr <= 240kHz */
      new_odr = (Odr ==   1.875f) ?    Odr
                : (Odr <   15.000f) ?  15.0f
                : (Odr >  240.000f) ? 240.0f
                :                        Odr;
      break;

    default:
      ret = LSM6DSV_ERROR;
      break;
  }

  if (ret != LSM6DSV_OK)
  {
    return ret;
  }

  if (pObj->acc_is_enabled == 1U)
  {
    return LSM6DSV_ACC_SetOutputDataRate_When_Enabled(pObj, new_odr);
  }
  else
  {
    return LSM6DSV_ACC_SetOutputDataRate_When_Disabled(pObj, new_odr);
  }
}

/**
 * @brief  Get the LSM6DSV accelerometer sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale pointer where the full scale is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_ACC_GetFullScale(LSM6DSV_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = LSM6DSV_OK;
  lsm6dsv_xl_full_scale_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (lsm6dsv_xl_full_scale_get(&(pObj->Ctx), &fs_low_level) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  switch (fs_low_level)
  {
    case LSM6DSV_2g:
      *FullScale =  2;
      break;

    case LSM6DSV_4g:
      *FullScale =  4;
      break;

    case LSM6DSV_8g:
      *FullScale =  8;
      break;

    case LSM6DSV_16g:
      *FullScale = 16;
      break;

    default:
      ret = LSM6DSV_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the LSM6DSV accelerometer sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale the functional full scale to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_ACC_SetFullScale(LSM6DSV_Object_t *pObj, int32_t FullScale)
{
  lsm6dsv_xl_full_scale_t new_fs;

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file statical analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  new_fs = (FullScale <= 2) ? LSM6DSV_2g
           : (FullScale <= 4) ? LSM6DSV_4g
           : (FullScale <= 8) ? LSM6DSV_8g
           :                    LSM6DSV_16g;

  if (lsm6dsv_xl_full_scale_set(&(pObj->Ctx), new_fs) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  return LSM6DSV_OK;
}

/**
 * @brief  Get the LSM6DSV accelerometer sensor raw axes
 * @param  pObj the device pObj
 * @param  Value pointer where the raw values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_ACC_GetAxesRaw(LSM6DSV_Object_t *pObj, LSM6DSV_AxesRaw_t *Value)
{
  lsm6dsv_axis3bit16_t data_raw;

  /* Read raw data values. */
  if (lsm6dsv_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return LSM6DSV_OK;
}

/**
 * @brief  Get the LSM6DSV accelerometer sensor axes
 * @param  pObj the device pObj
 * @param  Acceleration pointer where the values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_ACC_GetAxes(LSM6DSV_Object_t *pObj, LSM6DSV_Axes_t *Acceleration)
{
  lsm6dsv_axis3bit16_t data_raw;
  float_t sensitivity = 0.0f;

  /* Read raw data values. */
  if (lsm6dsv_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  /* Get LSM6DSV actual sensitivity. */
  if (LSM6DSV_ACC_GetSensitivity(pObj, &sensitivity) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  /* Calculate the data. */
  Acceleration->x = (int32_t)((float_t)((float_t)data_raw.i16bit[0] * sensitivity));
  Acceleration->y = (int32_t)((float_t)((float_t)data_raw.i16bit[1] * sensitivity));
  Acceleration->z = (int32_t)((float_t)((float_t)data_raw.i16bit[2] * sensitivity));

  return LSM6DSV_OK;
}

/**
 * @brief  Enable the LSM6DSV gyroscope sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_GYRO_Enable(LSM6DSV_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->gyro_is_enabled == 1U)
  {
    return LSM6DSV_OK;
  }

  /* Output data rate selection. */
  if (lsm6dsv_gy_data_rate_set(&(pObj->Ctx), pObj->gyro_odr) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  pObj->gyro_is_enabled = 1;

  return LSM6DSV_OK;
}

/**
 * @brief  Disable the LSM6DSV gyroscope sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_GYRO_Disable(LSM6DSV_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->gyro_is_enabled == 0U)
  {
    return LSM6DSV_OK;
  }

  /* Get current output data rate. */
  if (lsm6dsv_gy_data_rate_get(&(pObj->Ctx), &pObj->gyro_odr) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  /* Output data rate selection - power down. */
  if (lsm6dsv_gy_data_rate_set(&(pObj->Ctx), LSM6DSV_ODR_OFF) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  pObj->gyro_is_enabled = 0;

  return LSM6DSV_OK;
}

/**
 * @brief  Get the LSM6DSV gyroscope sensor sensitivity
 * @param  pObj the device pObj
 * @param  Sensitivity pointer
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_GYRO_GetSensitivity(LSM6DSV_Object_t *pObj, float_t *Sensitivity)
{
  int32_t ret = LSM6DSV_OK;
  lsm6dsv_gy_full_scale_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (lsm6dsv_gy_full_scale_get(&(pObj->Ctx), &full_scale) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  /* Store the sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case LSM6DSV_125dps:
      *Sensitivity = LSM6DSV_GYRO_SENSITIVITY_FS_125DPS;
      break;

    case LSM6DSV_250dps:
      *Sensitivity = LSM6DSV_GYRO_SENSITIVITY_FS_250DPS;
      break;

    case LSM6DSV_500dps:
      *Sensitivity = LSM6DSV_GYRO_SENSITIVITY_FS_500DPS;
      break;

    case LSM6DSV_1000dps:
      *Sensitivity = LSM6DSV_GYRO_SENSITIVITY_FS_1000DPS;
      break;

    case LSM6DSV_2000dps:
      *Sensitivity = LSM6DSV_GYRO_SENSITIVITY_FS_2000DPS;
      break;

    case LSM6DSV_4000dps:
      *Sensitivity = LSM6DSV_GYRO_SENSITIVITY_FS_4000DPS;
      break;

    default:
      ret = LSM6DSV_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Get the LSM6DSV gyroscope sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr pointer where the output data rate is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_GYRO_GetOutputDataRate(LSM6DSV_Object_t *pObj, float_t *Odr)
{
  int32_t ret = LSM6DSV_OK;
  lsm6dsv_data_rate_t odr_low_level;

  /* Get current output data rate. */
  if (lsm6dsv_gy_data_rate_get(&(pObj->Ctx), &odr_low_level) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  switch (odr_low_level)
  {
    case LSM6DSV_ODR_OFF:
      *Odr = 0.0f;
      break;

    case LSM6DSV_ODR_AT_1Hz875:
      *Odr = 1.875f;
      break;

    case LSM6DSV_ODR_AT_7Hz5:
      *Odr = 7.5f;
      break;

    case LSM6DSV_ODR_HA02_AT_12Hz5:
      *Odr = 12.5f;
      break;

    case LSM6DSV_ODR_AT_15Hz:
      *Odr = 15.0f;
      break;

    case LSM6DSV_ODR_HA01_AT_15Hz625:
      *Odr = 15.625f;
      break;

    case LSM6DSV_ODR_HA02_AT_25Hz:
      *Odr = 25.0f;
      break;

    case LSM6DSV_ODR_AT_30Hz:
      *Odr = 30.0f;
      break;

    case LSM6DSV_ODR_HA01_AT_31Hz25:
      *Odr = 31.25f;
      break;

    case LSM6DSV_ODR_HA02_AT_50Hz:
      *Odr = 50.0f;
      break;

    case LSM6DSV_ODR_AT_60Hz:
      *Odr = 60.0f;
      break;

    case LSM6DSV_ODR_HA01_AT_62Hz5:
      *Odr = 62.5f;
      break;

    case LSM6DSV_ODR_HA02_AT_100Hz:
      *Odr = 100.0f;
      break;

    case LSM6DSV_ODR_AT_120Hz:
      *Odr = 120.0f;
      break;

    case LSM6DSV_ODR_HA01_AT_125Hz:
      *Odr = 125.0f;
      break;

    case LSM6DSV_ODR_HA02_AT_200Hz:
      *Odr = 200.0f;
      break;

    case LSM6DSV_ODR_AT_240Hz:
      *Odr = 240.0f;
      break;

    case LSM6DSV_ODR_HA01_AT_250Hz:
      *Odr = 250.0f;
      break;

    case LSM6DSV_ODR_HA02_AT_400Hz:
      *Odr = 400.0f;
      break;

    case LSM6DSV_ODR_AT_480Hz:
      *Odr = 480.0f;
      break;

    case LSM6DSV_ODR_HA01_AT_500Hz:
      *Odr = 500.0f;
      break;

    case LSM6DSV_ODR_HA02_AT_800Hz:
      *Odr = 800.0f;
      break;

    case LSM6DSV_ODR_AT_960Hz:
      *Odr = 960.0f;
      break;

    case LSM6DSV_ODR_HA01_AT_1000Hz:
      *Odr = 1000.0f;
      break;

    case LSM6DSV_ODR_HA02_AT_1600Hz:
      *Odr = 1600.0f;
      break;

    case LSM6DSV_ODR_AT_1920Hz:
      *Odr = 1920.0f;
      break;

    case LSM6DSV_ODR_HA01_AT_2000Hz:
      *Odr = 2000.0f;
      break;

    case LSM6DSV_ODR_HA02_AT_3200Hz:
      *Odr = 3200.0f;
      break;

    case LSM6DSV_ODR_AT_3840Hz:
      *Odr = 3840.0f;
      break;

    case LSM6DSV_ODR_HA01_AT_4000Hz:
      *Odr = 4000.0f;
      break;

    case LSM6DSV_ODR_HA02_AT_6400Hz:
      *Odr = 6400.0f;
      break;

    case LSM6DSV_ODR_AT_7680Hz:
      *Odr = 7680.0f;
      break;

    case LSM6DSV_ODR_HA01_AT_8000Hz:
      *Odr = 8000.0f;
      break;

    default:
      ret = LSM6DSV_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the LSM6DSV gyroscope sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_GYRO_SetOutputDataRate(LSM6DSV_Object_t *pObj, float_t Odr)
{
  return LSM6DSV_GYRO_SetOutputDataRate_With_Mode(pObj, Odr, LSM6DSV_GYRO_HIGH_PERFORMANCE_MODE);
}

/**
 * @brief  Set the LSM6DSV gyroscope sensor output data rate with operating mode
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @param  Mode the gyroscope operating mode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_GYRO_SetOutputDataRate_With_Mode(LSM6DSV_Object_t *pObj, float_t Odr, LSM6DSV_GYRO_Operating_Mode_t Mode)
{
  int32_t ret = LSM6DSV_OK;
  float_t new_odr = 15.0f;

  switch (Mode)
  {
    case LSM6DSV_GYRO_HIGH_PERFORMANCE_MODE:
      if (lsm6dsv_gy_mode_set(&(pObj->Ctx), LSM6DSV_GY_HIGH_PERFORMANCE_MD) != LSM6DSV_OK)
      {
        ret = LSM6DSV_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 7.68kHz */
      new_odr = (Odr <    7.5f) ?    7.5f
                : (Odr > 7680.0f) ? 7680.0f
                :                       Odr;
      break;

    case LSM6DSV_GYRO_HIGH_ACCURACY_MODE:
      ret = LSM6DSV_ERROR;
      break;

    case LSM6DSV_GYRO_SLEEP_MODE:
      ret = LSM6DSV_ERROR;
      break;

    case LSM6DSV_GYRO_LOW_POWER_MODE:
      if (lsm6dsv_gy_mode_set(&(pObj->Ctx), LSM6DSV_GY_LOW_POWER_MD) != LSM6DSV_OK)
      {
        ret = LSM6DSV_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 240kHz */
      new_odr = (Odr <   7.5f) ?   7.5f
                : (Odr > 240.0f) ? 240.0f
                :                     Odr;
      break;

    default:
      ret = LSM6DSV_ERROR;
      break;
  }

  if (ret != LSM6DSV_OK)
  {
    return ret;
  }

  if (pObj->gyro_is_enabled == 1U)
  {
    return LSM6DSV_GYRO_SetOutputDataRate_When_Enabled(pObj, new_odr);
  }
  else
  {
    return LSM6DSV_GYRO_SetOutputDataRate_When_Disabled(pObj, new_odr);
  }
}

/**
 * @brief  Get the LSM6DSV gyroscope sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale pointer where the full scale is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_GYRO_GetFullScale(LSM6DSV_Object_t *pObj, int32_t  *FullScale)
{
  int32_t ret = LSM6DSV_OK;
  lsm6dsv_gy_full_scale_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (lsm6dsv_gy_full_scale_get(&(pObj->Ctx), &fs_low_level) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  switch (fs_low_level)
  {
    case LSM6DSV_125dps:
      *FullScale =  125;
      break;

    case LSM6DSV_250dps:
      *FullScale =  250;
      break;

    case LSM6DSV_500dps:
      *FullScale =  500;
      break;

    case LSM6DSV_1000dps:
      *FullScale = 1000;
      break;

    case LSM6DSV_2000dps:
      *FullScale = 2000;
      break;

    case LSM6DSV_4000dps:
      *FullScale = 4000;
      break;

    default:
      ret = LSM6DSV_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the LSM6DSV gyroscope sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale the functional full scale to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_GYRO_SetFullScale(LSM6DSV_Object_t *pObj, int32_t FullScale)
{
  lsm6dsv_gy_full_scale_t new_fs;

  new_fs = (FullScale <= 125)  ? LSM6DSV_125dps
           : (FullScale <= 250)  ? LSM6DSV_250dps
           : (FullScale <= 500)  ? LSM6DSV_500dps
           : (FullScale <= 1000) ? LSM6DSV_1000dps
           : (FullScale <= 2000) ? LSM6DSV_2000dps
           :                       LSM6DSV_4000dps;

  if (lsm6dsv_gy_full_scale_set(&(pObj->Ctx), new_fs) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  return LSM6DSV_OK;
}

/**
 * @brief  Get the LSM6DSV gyroscope sensor raw axes
 * @param  pObj the device pObj
 * @param  Value pointer where the raw values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_GYRO_GetAxesRaw(LSM6DSV_Object_t *pObj, LSM6DSV_AxesRaw_t *Value)
{
  lsm6dsv_axis3bit16_t data_raw;

  /* Read raw data values. */
  if (lsm6dsv_angular_rate_raw_get(&(pObj->Ctx), data_raw.i16bit) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return LSM6DSV_OK;
}

/**
 * @brief  Get the LSM6DSV gyroscope sensor axes
 * @param  pObj the device pObj
 * @param  AngularRate pointer where the values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_GYRO_GetAxes(LSM6DSV_Object_t *pObj, LSM6DSV_Axes_t *AngularRate)
{
  lsm6dsv_axis3bit16_t data_raw;
  float_t sensitivity = 0.0f;

  /* Read raw data values. */
  if (lsm6dsv_angular_rate_raw_get(&(pObj->Ctx), data_raw.i16bit) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  /* Get LSM6DSV actual sensitivity. */
  if (LSM6DSV_GYRO_GetSensitivity(pObj, &sensitivity) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  /* Calculate the data. */
  AngularRate->x = (int32_t)((float_t)((float_t)data_raw.i16bit[0] * sensitivity));
  AngularRate->y = (int32_t)((float_t)((float_t)data_raw.i16bit[1] * sensitivity));
  AngularRate->z = (int32_t)((float_t)((float_t)data_raw.i16bit[2] * sensitivity));

  return LSM6DSV_OK;
}

/**
 * @brief  Get the LSM6DSV register value
 * @param  pObj the device pObj
 * @param  Reg address to be read
 * @param  Data pointer where the value is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_Read_Reg(LSM6DSV_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (lsm6dsv_read_reg(&(pObj->Ctx), Reg, Data, 1) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  return LSM6DSV_OK;
}

/**
 * @brief  Set the LSM6DSV register value
 * @param  pObj the device pObj
 * @param  Reg address to be written
 * @param  Data value to be written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_Write_Reg(LSM6DSV_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (lsm6dsv_write_reg(&(pObj->Ctx), Reg, &Data, 1) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  return LSM6DSV_OK;
}

/**
 * @brief  Get the LSM6DSV ACC data ready bit value
 * @param  pObj the device pObj
 * @param  Status the status of data ready bit
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_ACC_Get_DRDY_Status(LSM6DSV_Object_t *pObj, uint8_t *Status)
{
  lsm6dsv_all_sources_t val;

  if (lsm6dsv_all_sources_get(&(pObj->Ctx), &val) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  *Status = val.drdy_xl;
  return LSM6DSV_OK;
}

/**
 * @brief  Get the LSM6DSV GYRO data ready bit value
 * @param  pObj the device pObj
 * @param  Status the status of data ready bit
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_GYRO_Get_DRDY_Status(LSM6DSV_Object_t *pObj, uint8_t *Status)
{
  lsm6dsv_all_sources_t val;

  if (lsm6dsv_all_sources_get(&(pObj->Ctx), &val) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  *Status = val.drdy_gy;
  return LSM6DSV_OK;
}

/**
  * @brief  Set memory bank
  * @param  pObj the device pObj
  * @param  Val the value of memory bank in reg FUNC_CFG_ACCESS
  *         0 - LSM6DSV_MAIN_MEM_BANK, 1 - LSM6DSV_EMBED_FUNC_MEM_BANK, 2 - LSM6DSV_SENSOR_HUB_MEM_BANK
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV_Set_Mem_Bank(LSM6DSV_Object_t *pObj, uint8_t Val)
{
  int32_t ret = LSM6DSV_OK;
  lsm6dsv_mem_bank_t reg;

  reg = (Val == 1U) ? LSM6DSV_EMBED_FUNC_MEM_BANK
        : (Val == 2U) ? LSM6DSV_SENSOR_HUB_MEM_BANK
        :               LSM6DSV_MAIN_MEM_BANK;

  if (lsm6dsv_mem_bank_set(&(pObj->Ctx), reg) != LSM6DSV_OK)
  {
    ret = LSM6DSV_ERROR;
  }

  return ret;
}

/**
 * @}
 */

/** @defgroup LSM6DSV_Private_Functions LSM6DSV Private Functions
 * @{
 */

/**
 * @brief  Set the LSM6DSV accelerometer sensor output data rate when enabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t LSM6DSV_ACC_SetOutputDataRate_When_Enabled(LSM6DSV_Object_t *pObj, float_t Odr)
{
  lsm6dsv_data_rate_t new_odr;

  new_odr = (Odr <=    1.875f) ? LSM6DSV_ODR_AT_1Hz875
            : (Odr <=    7.5f) ? LSM6DSV_ODR_AT_7Hz5
            : (Odr <=   15.0f) ? LSM6DSV_ODR_AT_15Hz
            : (Odr <=   30.0f) ? LSM6DSV_ODR_AT_30Hz
            : (Odr <=   60.0f) ? LSM6DSV_ODR_AT_60Hz
            : (Odr <=  120.0f) ? LSM6DSV_ODR_AT_120Hz
            : (Odr <=  240.0f) ? LSM6DSV_ODR_AT_240Hz
            : (Odr <=  480.0f) ? LSM6DSV_ODR_AT_480Hz
            : (Odr <=  960.0f) ? LSM6DSV_ODR_AT_960Hz
            : (Odr <= 1920.0f) ? LSM6DSV_ODR_AT_1920Hz
            : (Odr <= 3840.0f) ? LSM6DSV_ODR_AT_3840Hz
            :                    LSM6DSV_ODR_AT_7680Hz;

  /* Output data rate selection. */
  if (lsm6dsv_xl_data_rate_set(&(pObj->Ctx), new_odr) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  return LSM6DSV_OK;
}

/**
 * @brief  Set the LSM6DSV accelerometer sensor output data rate when disabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t LSM6DSV_ACC_SetOutputDataRate_When_Disabled(LSM6DSV_Object_t *pObj, float_t Odr)
{
  pObj->acc_odr = (Odr <=    1.875f) ? LSM6DSV_ODR_AT_1Hz875
                  : (Odr <=    7.5f) ? LSM6DSV_ODR_AT_7Hz5
                  : (Odr <=   15.0f) ? LSM6DSV_ODR_AT_15Hz
                  : (Odr <=   30.0f) ? LSM6DSV_ODR_AT_30Hz
                  : (Odr <=   60.0f) ? LSM6DSV_ODR_AT_60Hz
                  : (Odr <=  120.0f) ? LSM6DSV_ODR_AT_120Hz
                  : (Odr <=  240.0f) ? LSM6DSV_ODR_AT_240Hz
                  : (Odr <=  480.0f) ? LSM6DSV_ODR_AT_480Hz
                  : (Odr <=  960.0f) ? LSM6DSV_ODR_AT_960Hz
                  : (Odr <= 1920.0f) ? LSM6DSV_ODR_AT_1920Hz
                  : (Odr <= 3840.0f) ? LSM6DSV_ODR_AT_3840Hz
                  :                    LSM6DSV_ODR_AT_7680Hz;

  return LSM6DSV_OK;
}

/**
 * @brief  Set the LSM6DSV gyroscope sensor output data rate when enabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t LSM6DSV_GYRO_SetOutputDataRate_When_Enabled(LSM6DSV_Object_t *pObj, float_t Odr)
{
  lsm6dsv_data_rate_t new_odr;

  new_odr = (Odr <=    7.5f) ? LSM6DSV_ODR_AT_7Hz5
            : (Odr <=   15.0f) ? LSM6DSV_ODR_AT_15Hz
            : (Odr <=   30.0f) ? LSM6DSV_ODR_AT_30Hz
            : (Odr <=   60.0f) ? LSM6DSV_ODR_AT_60Hz
            : (Odr <=  120.0f) ? LSM6DSV_ODR_AT_120Hz
            : (Odr <=  240.0f) ? LSM6DSV_ODR_AT_240Hz
            : (Odr <=  480.0f) ? LSM6DSV_ODR_AT_480Hz
            : (Odr <=  960.0f) ? LSM6DSV_ODR_AT_960Hz
            : (Odr <= 1920.0f) ? LSM6DSV_ODR_AT_1920Hz
            : (Odr <= 3840.0f) ? LSM6DSV_ODR_AT_3840Hz
            :                    LSM6DSV_ODR_AT_7680Hz;

  /* Output data rate selection. */
  if (lsm6dsv_gy_data_rate_set(&(pObj->Ctx), new_odr) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  return LSM6DSV_OK;
}

/**
 * @brief  Set the LSM6DSV gyroscope sensor output data rate when disabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t LSM6DSV_GYRO_SetOutputDataRate_When_Disabled(LSM6DSV_Object_t *pObj, float_t Odr)
{
  pObj->gyro_odr = (Odr <=    7.5f) ? LSM6DSV_ODR_AT_7Hz5
                   : (Odr <=   15.0f) ? LSM6DSV_ODR_AT_15Hz
                   : (Odr <=   30.0f) ? LSM6DSV_ODR_AT_30Hz
                   : (Odr <=   60.0f) ? LSM6DSV_ODR_AT_60Hz
                   : (Odr <=  120.0f) ? LSM6DSV_ODR_AT_120Hz
                   : (Odr <=  240.0f) ? LSM6DSV_ODR_AT_240Hz
                   : (Odr <=  480.0f) ? LSM6DSV_ODR_AT_480Hz
                   : (Odr <=  960.0f) ? LSM6DSV_ODR_AT_960Hz
                   : (Odr <= 1920.0f) ? LSM6DSV_ODR_AT_1920Hz
                   : (Odr <= 3840.0f) ? LSM6DSV_ODR_AT_3840Hz
                   :                    LSM6DSV_ODR_AT_7680Hz;

  return LSM6DSV_OK;
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
  LSM6DSV_Object_t *pObj = (LSM6DSV_Object_t *)Handle;

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
  LSM6DSV_Object_t *pObj = (LSM6DSV_Object_t *)Handle;

  return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
}


/**
 * @brief  Set the LSM6DSV accelerometer power mode
 * @param  pObj the device pObj
 * @param  PowerMode Value of the powerMode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_ACC_Set_Power_Mode(LSM6DSV_Object_t *pObj, uint8_t PowerMode)
{
  lsm6dsv_xl_mode_t new_pm;

  new_pm = (PowerMode == 0x0U) ? LSM6DSV_XL_HIGH_PERFORMANCE_MD
           : (PowerMode == 0x4U) ? LSM6DSV_XL_LOW_POWER_2_AVG_MD
           : (PowerMode == 0x5U) ? LSM6DSV_XL_LOW_POWER_4_AVG_MD
           : (PowerMode == 0x6U) ? LSM6DSV_XL_LOW_POWER_8_AVG_MD
           :                       LSM6DSV_XL_NORMAL_MD;

  if (lsm6dsv_xl_mode_set(&(pObj->Ctx), new_pm) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  return LSM6DSV_OK;
}


/**
 * @brief  Set the LSM6DSV gyroscope power mode
 * @param  pObj the device pObj
 * @param  PowerMode Value of the powerMode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_GYRO_Set_Power_Mode(LSM6DSV_Object_t *pObj, uint8_t PowerMode)
{
  lsm6dsv_gy_mode_t new_pm;

  new_pm = (PowerMode == 0x0U) ? LSM6DSV_GY_HIGH_PERFORMANCE_MD
           : (PowerMode == 0x4U) ? LSM6DSV_GY_SLEEP_MD
           :                       LSM6DSV_GY_LOW_POWER_MD;

  if (lsm6dsv_gy_mode_set(&(pObj->Ctx), new_pm) != LSM6DSV_OK)
  {
    return LSM6DSV_ERROR;
  }

  return LSM6DSV_OK;
}


/**
 * @brief  Set the LSM6DSV accelerometer filter mode
 * @param  pObj the device pObj
 * @param  LowHighPassFlag 0/1 for setting low-pass/high-pass filter mode
 * @param  FilterMode Value of the filter Mode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_ACC_Set_Filter_Mode(LSM6DSV_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode)
{
  lsm6dsv_filt_xl_lp2_bandwidth_t new_fm;

  new_fm = (FilterMode == 0x0U) ? LSM6DSV_XL_ULTRA_LIGHT
           : (FilterMode == 0x1U) ? LSM6DSV_XL_VERY_LIGHT
           : (FilterMode == 0x2U) ? LSM6DSV_XL_LIGHT
           : (FilterMode == 0x3U) ? LSM6DSV_XL_MEDIUM
           : (FilterMode == 0x4U) ? LSM6DSV_XL_STRONG
           : (FilterMode == 0x5U) ? LSM6DSV_XL_VERY_STRONG
           : (FilterMode == 0x6U) ? LSM6DSV_XL_AGGRESSIVE
           :                        LSM6DSV_XL_XTREME;

  if (LowHighPassFlag == 0U)
  {
    /* Set accelerometer low_pass filter-mode */
    /* Set to 1 LPF2 bit (CTRL8_XL) */
    if (lsm6dsv_filt_xl_lp2_set(&(pObj->Ctx), 1) != LSM6DSV_OK)
    {
      return LSM6DSV_ERROR;
    }
    if (lsm6dsv_filt_xl_lp2_bandwidth_set(&(pObj->Ctx), new_fm) != LSM6DSV_OK)
    {
      return LSM6DSV_ERROR;
    }
  }
  else
  {
    if (lsm6dsv_filt_xl_lp2_set(&(pObj->Ctx), 0) != LSM6DSV_OK)
    {
      return LSM6DSV_ERROR;
    }
    /* Set accelerometer high_pass filter-mode */
    if (lsm6dsv_filt_xl_lp2_bandwidth_set(&(pObj->Ctx), new_fm) != LSM6DSV_OK)
    {
      return LSM6DSV_ERROR;
    }
  }
  return LSM6DSV_OK;
}


/**
 * @brief  Set the LSM6DSV gyroscope filter mode
 * @param  pObj the device pObj
 * @param  LowHighPassFlag 0/1 for setting low-pass/high-pass filter mode
 * @param  FilterMode Value of the filter Mode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV_GYRO_Set_Filter_Mode(LSM6DSV_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode)
{
  lsm6dsv_filt_gy_lp1_bandwidth_t new_fm;

  new_fm = (FilterMode == 0x0U) ? LSM6DSV_GY_ULTRA_LIGHT
           : (FilterMode == 0x1U) ? LSM6DSV_GY_VERY_LIGHT
           : (FilterMode == 0x2U) ? LSM6DSV_GY_LIGHT
           : (FilterMode == 0x3U) ? LSM6DSV_GY_MEDIUM
           : (FilterMode == 0x4U) ? LSM6DSV_GY_STRONG
           : (FilterMode == 0x5U) ? LSM6DSV_GY_VERY_STRONG
           : (FilterMode == 0x6U) ? LSM6DSV_GY_AGGRESSIVE
           :                        LSM6DSV_GY_XTREME;

  if (LowHighPassFlag == 0U)
  {
    /* Set gyroscope low_pass 1 filter-mode */
    /* Enable low-pass filter */
    if (lsm6dsv_filt_gy_lp1_set(&(pObj->Ctx), 1) != LSM6DSV_OK)
    {
      return LSM6DSV_ERROR;
    }
    if (lsm6dsv_filt_gy_lp1_bandwidth_set(&(pObj->Ctx), new_fm) != LSM6DSV_OK)
    {
      return LSM6DSV_ERROR;
    }
  }
  else
  {
    /* Set gyroscope high_pass filter-mode */
    /* Enable high-pass filter */
    if (lsm6dsv_filt_gy_lp1_set(&(pObj->Ctx), 0) != LSM6DSV_OK)
    {
      return LSM6DSV_ERROR;
    }
    if (lsm6dsv_filt_gy_lp1_bandwidth_set(&(pObj->Ctx), new_fm) != LSM6DSV_OK)
    {
      return LSM6DSV_ERROR;
    }
  }
  return LSM6DSV_OK;
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
