/**
 ******************************************************************************
 * @file    lsm6dsv16bx.c
 * @author  MEMS Software Solutions Team
 * @brief   LSM6DSV16BX driver file
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
#include "lsm6dsv16bx.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @defgroup LSM6DSV16BX LSM6DSV16BX
 * @{
 */

/** @defgroup LSM6DSV16BX_Exported_Variables LSM6DSV16BX Exported Variables
 * @{
 */

LSM6DSV16BX_CommonDrv_t LSM6DSV16BX_COMMON_Driver =
{
  LSM6DSV16BX_Init,
  LSM6DSV16BX_DeInit,
  LSM6DSV16BX_ReadID,
  LSM6DSV16BX_GetCapabilities,
};

LSM6DSV16BX_ACC_Drv_t LSM6DSV16BX_ACC_Driver =
{
  LSM6DSV16BX_ACC_Enable,
  LSM6DSV16BX_ACC_Disable,
  LSM6DSV16BX_ACC_GetSensitivity,
  LSM6DSV16BX_ACC_GetOutputDataRate,
  LSM6DSV16BX_ACC_SetOutputDataRate,
  LSM6DSV16BX_ACC_GetFullScale,
  LSM6DSV16BX_ACC_SetFullScale,
  LSM6DSV16BX_ACC_GetAxes,
  LSM6DSV16BX_ACC_GetAxesRaw,
};

LSM6DSV16BX_GYRO_Drv_t LSM6DSV16BX_GYRO_Driver =
{
  LSM6DSV16BX_GYRO_Enable,
  LSM6DSV16BX_GYRO_Disable,
  LSM6DSV16BX_GYRO_GetSensitivity,
  LSM6DSV16BX_GYRO_GetOutputDataRate,
  LSM6DSV16BX_GYRO_SetOutputDataRate,
  LSM6DSV16BX_GYRO_GetFullScale,
  LSM6DSV16BX_GYRO_SetFullScale,
  LSM6DSV16BX_GYRO_GetAxes,
  LSM6DSV16BX_GYRO_GetAxesRaw,
};

/**
 * @}
 */

/** @defgroup LSM6DSV16BX_Private_Function_Prototypes LSM6DSV16BX Private Function Prototypes
 * @{
 */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t LSM6DSV16BX_ACC_SetOutputDataRate_When_Enabled(LSM6DSV16BX_Object_t *pObj, float Odr);
static int32_t LSM6DSV16BX_ACC_SetOutputDataRate_When_Disabled(LSM6DSV16BX_Object_t *pObj, float Odr);
static int32_t LSM6DSV16BX_GYRO_SetOutputDataRate_When_Enabled(LSM6DSV16BX_Object_t *pObj, float Odr);
static int32_t LSM6DSV16BX_GYRO_SetOutputDataRate_When_Disabled(LSM6DSV16BX_Object_t *pObj, float Odr);

/**
 * @}
 */

/** @defgroup LSM6DSV16BX_Exported_Functions LSM6DSV16BX Exported Functions
 * @{
 */

/**
 * @brief  Register Component Bus IO operations
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_RegisterBusIO(LSM6DSV16BX_Object_t *pObj, LSM6DSV16BX_IO_t *pIO)
{
  int32_t ret = LSM6DSV16BX_OK;

  if (pObj == NULL)
  {
    ret = LSM6DSV16BX_ERROR;
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
      ret = LSM6DSV16BX_ERROR;
    }
    else if (pObj->IO.Init() != LSM6DSV16BX_OK)
    {
      ret = LSM6DSV16BX_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == LSM6DSV16BX_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x04;

          if (LSM6DSV16BX_Write_Reg(pObj, LSM6DSV16BX_CTRL3, data) != LSM6DSV16BX_OK)
          {
            ret = LSM6DSV16BX_ERROR;
          }

          if (LSM6DSV16BX_Write_Reg(pObj, LSM6DSV16BX_IF_CFG, data) != LSM6DSV16BX_OK)
          {
            ret = LSM6DSV16BX_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
 * @brief  Initialize the LSM6DSV16BX sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_Init(LSM6DSV16BX_Object_t *pObj)
{
  /* Enable register address automatically incremented during a multiple byte
  access with a serial interface. */
  if (lsm6dsv16bx_auto_increment_set(&(pObj->Ctx), PROPERTY_ENABLE) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  /* Enable BDU */
  if (lsm6dsv16bx_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  /* FIFO mode selection */
  if (lsm6dsv16bx_fifo_mode_set(&(pObj->Ctx), LSM6DSV16BX_BYPASS_MODE) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = LSM6DSV16BX_XL_ODR_AT_120Hz;

  /* Output data rate selection - power down. */
  if (lsm6dsv16bx_xl_data_rate_set(&(pObj->Ctx), LSM6DSV16BX_XL_ODR_OFF) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  /* Full scale selection. */
  if (lsm6dsv16bx_xl_full_scale_set(&(pObj->Ctx), LSM6DSV16BX_2g) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  /* Select default output data rate. */
  pObj->gyro_odr = LSM6DSV16BX_GY_ODR_AT_120Hz;

  /* Output data rate selection - power down. */
  if (lsm6dsv16bx_gy_data_rate_set(&(pObj->Ctx), LSM6DSV16BX_GY_ODR_OFF) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  /* Full scale selection. */
  if (lsm6dsv16bx_gy_full_scale_set(&(pObj->Ctx), LSM6DSV16BX_2000dps) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  /* Enable Qvar functionality */
  lsm6dsv16bx_ah_qvar_mode_t mode;

  mode.ah_qvar1_en = 1;
  mode.ah_qvar2_en = 0;
  mode.swaps       = 0;

  if (lsm6dsv16bx_ah_qvar_mode_set(&(pObj->Ctx), mode) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  pObj->is_initialized = 1;

  return LSM6DSV16BX_OK;
}

/**
 * @brief  Deinitialize the LSM6DSV16BX sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_DeInit(LSM6DSV16BX_Object_t *pObj)
{
  /* Disable the component */
  if (LSM6DSV16BX_ACC_Disable(pObj) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  if (LSM6DSV16BX_GYRO_Disable(pObj) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  /* Reset output data rate. */
  pObj->acc_odr = LSM6DSV16BX_XL_ODR_OFF;
  pObj->gyro_odr = LSM6DSV16BX_GY_ODR_OFF;

  /* Disable Qvar functionality */
  lsm6dsv16bx_ah_qvar_mode_t mode;

  mode.ah_qvar1_en = 0;
  mode.ah_qvar2_en = 0;
  mode.swaps       = 0;

  if (lsm6dsv16bx_ah_qvar_mode_set(&(pObj->Ctx), mode) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  pObj->is_initialized = 0;

  return LSM6DSV16BX_OK;
}

/**
 * @brief  Read component ID
 * @param  pObj the device pObj
 * @param  Id the WHO_AM_I value
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_ReadID(LSM6DSV16BX_Object_t *pObj, uint8_t *Id)
{
  if (lsm6dsv16bx_device_id_get(&(pObj->Ctx), Id) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  return LSM6DSV16BX_OK;
}

/**
 * @brief  Get LSM6DSV16BX sensor capabilities
 * @param  pObj Component object pointer
 * @param  Capabilities pointer to LSM6DSV16BX sensor capabilities
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_GetCapabilities(LSM6DSV16BX_Object_t *pObj, LSM6DSV16BX_Capabilities_t *Capabilities)
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
  return LSM6DSV16BX_OK;
}

/**
 * @brief  Enable the LSM6DSV16BX accelerometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_ACC_Enable(LSM6DSV16BX_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return LSM6DSV16BX_OK;
  }

  /* Output data rate selection. */
  if (lsm6dsv16bx_xl_data_rate_set(&(pObj->Ctx), pObj->acc_odr) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return LSM6DSV16BX_OK;
}

/**
 * @brief  Disable the LSM6DSV16BX accelerometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_ACC_Disable(LSM6DSV16BX_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return LSM6DSV16BX_OK;
  }

  /* Get current output data rate. */
  if (lsm6dsv16bx_xl_data_rate_get(&(pObj->Ctx), &pObj->acc_odr) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  /* Output data rate selection - power down. */
  if (lsm6dsv16bx_xl_data_rate_set(&(pObj->Ctx), LSM6DSV16BX_XL_ODR_OFF) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return LSM6DSV16BX_OK;
}

/**
 * @brief  Get the LSM6DSV16BX accelerometer sensor sensitivity
 * @param  pObj the device pObj
 * @param  Sensitivity pointer
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_ACC_GetSensitivity(LSM6DSV16BX_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = LSM6DSV16BX_OK;
  lsm6dsv16bx_xl_full_scale_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (lsm6dsv16bx_xl_full_scale_get(&(pObj->Ctx), &full_scale) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  /* Store the Sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case LSM6DSV16BX_2g:
      *Sensitivity = LSM6DSV16BX_ACC_SENSITIVITY_FS_2G;
      break;

    case LSM6DSV16BX_4g:
      *Sensitivity = LSM6DSV16BX_ACC_SENSITIVITY_FS_4G;
      break;

    case LSM6DSV16BX_8g:
      *Sensitivity = LSM6DSV16BX_ACC_SENSITIVITY_FS_8G;
      break;

    case LSM6DSV16BX_16g:
      *Sensitivity = LSM6DSV16BX_ACC_SENSITIVITY_FS_16G;
      break;

    default:
      ret = LSM6DSV16BX_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Get the LSM6DSV16BX accelerometer sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr pointer where the output data rate is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_ACC_GetOutputDataRate(LSM6DSV16BX_Object_t *pObj, float *Odr)
{
  int32_t ret = LSM6DSV16BX_OK;
  lsm6dsv16bx_xl_data_rate_t odr_low_level;

  /* Get current output data rate. */
  if (lsm6dsv16bx_xl_data_rate_get(&(pObj->Ctx), &odr_low_level) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  switch (odr_low_level)
  {
    case LSM6DSV16BX_XL_ODR_OFF:
      *Odr = 0.0f;
      break;

    case LSM6DSV16BX_XL_ODR_AT_1Hz875:
      *Odr = 1.875f;
      break;

    case LSM6DSV16BX_XL_ODR_AT_7Hz5:
      *Odr = 7.5f;
      break;

    case LSM6DSV16BX_XL_ODR_AT_15Hz:
      *Odr = 15.0f;
      break;

    case LSM6DSV16BX_XL_ODR_AT_30Hz:
      *Odr = 30.0f;
      break;

    case LSM6DSV16BX_XL_ODR_AT_60Hz:
      *Odr = 60.0f;
      break;

    case LSM6DSV16BX_XL_ODR_AT_120Hz:
      *Odr = 120.0f;
      break;

    case LSM6DSV16BX_XL_ODR_AT_240Hz:
      *Odr = 240.0f;
      break;

    case LSM6DSV16BX_XL_ODR_AT_480Hz:
      *Odr = 480.0f;
      break;

    case LSM6DSV16BX_XL_ODR_AT_960Hz:
      *Odr = 960.0f;
      break;

    case LSM6DSV16BX_XL_ODR_AT_1920Hz:
      *Odr = 1920.0f;
      break;

    case LSM6DSV16BX_XL_ODR_AT_3840Hz:
      *Odr = 3840.0f;
      break;

    case LSM6DSV16BX_XL_ODR_AT_7680Hz:
      *Odr = 7680.0f;
      break;

    default:
      ret = LSM6DSV16BX_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the LSM6DSV16BX accelerometer sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_ACC_SetOutputDataRate(LSM6DSV16BX_Object_t *pObj, float Odr)
{
  return LSM6DSV16BX_ACC_SetOutputDataRate_With_Mode(pObj, Odr, LSM6DSV16BX_ACC_HIGH_PERFORMANCE_MODE);
}

/**
 * @brief  Set the LSM6DSV16BX accelerometer sensor output data rate with operating mode
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @param  Mode the accelerometer operating mode
 * @note   This function switches off the gyroscope if Ultra Low Power Mode is set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_ACC_SetOutputDataRate_With_Mode(LSM6DSV16BX_Object_t *pObj, float Odr, LSM6DSV16BX_ACC_Operating_Mode_t Mode)
{
  switch (Mode)
  {
    case LSM6DSV16BX_ACC_HIGH_PERFORMANCE_MODE:
    {
      if (lsm6dsv16bx_xl_mode_set(&(pObj->Ctx), LSM6DSV16BX_XL_HIGH_PERFORMANCE_MD) != LSM6DSV16BX_OK)
      {
        return LSM6DSV16BX_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 7.68kHz */
      Odr = (Odr <    7.5f) ?    7.5f
    	  : (Odr > 7680.0f) ? 7680.0f
    	  :                       Odr;
      break;
    }

    case LSM6DSV16BX_ACC_HIGH_ACCURACY_MODE:
      return LSM6DSV16BX_ERROR;

    case LSM6DSV16BX_ACC_NORMAL_MODE:
    {
      if (lsm6dsv16bx_xl_mode_set(&(pObj->Ctx), LSM6DSV16BX_XL_NORMAL_MD) != LSM6DSV16BX_OK)
      {
        return LSM6DSV16BX_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 1.92kHz */
      Odr = (Odr <    7.5f) ?    7.5f
    	  : (Odr > 1920.0f) ? 1920.0f
    	  :                       Odr;
      break;
    }

    case LSM6DSV16BX_ACC_LOW_POWER_MODE1:
    {
      if (lsm6dsv16bx_xl_mode_set(&(pObj->Ctx), LSM6DSV16BX_XL_LOW_POWER_2_AVG_MD) != LSM6DSV16BX_OK)
      {
        return LSM6DSV16BX_ERROR;
      }

      /* Valid ODR: 1.875Hz;  15Hz <= Odr <= 240kHz */
      Odr = (Odr ==   1.875f) ?    Odr
    	  : (Odr <   15.000f) ?  15.0f
    	  : (Odr >  240.000f) ? 240.0f
    	  :                        Odr;
      break;
    }

    case LSM6DSV16BX_ACC_LOW_POWER_MODE2:
    {
      if (lsm6dsv16bx_xl_mode_set(&(pObj->Ctx), LSM6DSV16BX_XL_LOW_POWER_4_AVG_MD) != LSM6DSV16BX_OK)
      {
        return LSM6DSV16BX_ERROR;
      }

      /* Valid ODR: 1.875Hz;  15Hz <= Odr <= 240kHz */
      Odr = (Odr ==   1.875f) ?    Odr
    	  : (Odr <   15.000f) ?  15.0f
    	  : (Odr >  240.000f) ? 240.0f
    	  :                        Odr;
      break;
    }

    case LSM6DSV16BX_ACC_LOW_POWER_MODE3:
    {
      if (lsm6dsv16bx_xl_mode_set(&(pObj->Ctx), LSM6DSV16BX_XL_LOW_POWER_8_AVG_MD) != LSM6DSV16BX_OK)
      {
        return LSM6DSV16BX_ERROR;
      }

      /* Valid ODR: 1.875Hz;  15Hz <= Odr <= 240kHz */
      Odr = (Odr ==   1.875f) ?    Odr
    	  : (Odr <   15.000f) ?  15.0f
    	  : (Odr >  240.000f) ? 240.0f
    	  :                        Odr;
      break;
    }

    default:
      return LSM6DSV16BX_ERROR;
  }

  if (pObj->acc_is_enabled == 1U)
  {
    return LSM6DSV16BX_ACC_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return LSM6DSV16BX_ACC_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
 * @brief  Get the LSM6DSV16BX accelerometer sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale pointer where the full scale is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_ACC_GetFullScale(LSM6DSV16BX_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = LSM6DSV16BX_OK;
  lsm6dsv16bx_xl_full_scale_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (lsm6dsv16bx_xl_full_scale_get(&(pObj->Ctx), &fs_low_level) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  switch (fs_low_level)
  {
    case LSM6DSV16BX_2g:
      *FullScale =  2;
      break;

    case LSM6DSV16BX_4g:
      *FullScale =  4;
      break;

    case LSM6DSV16BX_8g:
      *FullScale =  8;
      break;

    case LSM6DSV16BX_16g:
      *FullScale = 16;
      break;

    default:
      ret = LSM6DSV16BX_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the LSM6DSV16BX accelerometer sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale the functional full scale to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_ACC_SetFullScale(LSM6DSV16BX_Object_t *pObj, int32_t FullScale)
{
  lsm6dsv16bx_xl_full_scale_t new_fs;

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file statical analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  new_fs = (FullScale <= 2) ? LSM6DSV16BX_2g
         : (FullScale <= 4) ? LSM6DSV16BX_4g
         : (FullScale <= 8) ? LSM6DSV16BX_8g
         :                    LSM6DSV16BX_16g;

  if (lsm6dsv16bx_xl_full_scale_set(&(pObj->Ctx), new_fs) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  return LSM6DSV16BX_OK;
}

/**
 * @brief  Get the LSM6DSV16BX accelerometer sensor raw axes
 * @param  pObj the device pObj
 * @param  Value pointer where the raw values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_ACC_GetAxesRaw(LSM6DSV16BX_Object_t *pObj, LSM6DSV16BX_AxesRaw_t *Value)
{
  lsm6dsv16bx_axis3bit16_t data_raw;

  /* Read raw data values. */
  if (lsm6dsv16bx_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return LSM6DSV16BX_OK;
}

/**
 * @brief  Get the LSM6DSV16BX accelerometer sensor axes
 * @param  pObj the device pObj
 * @param  Acceleration pointer where the values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_ACC_GetAxes(LSM6DSV16BX_Object_t *pObj, LSM6DSV16BX_Axes_t *Acceleration)
{
  lsm6dsv16bx_axis3bit16_t data_raw;
  float sensitivity = 0.0f;

  /* Read raw data values. */
  if (lsm6dsv16bx_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  /* Get LSM6DSV16BX actual sensitivity. */
  if (LSM6DSV16BX_ACC_GetSensitivity(pObj, &sensitivity) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  /* Calculate the data. */
  Acceleration->x = (int32_t)((float)((float)data_raw.i16bit[0] * sensitivity));
  Acceleration->y = (int32_t)((float)((float)data_raw.i16bit[1] * sensitivity));
  Acceleration->z = (int32_t)((float)((float)data_raw.i16bit[2] * sensitivity));

  return LSM6DSV16BX_OK;
}

/**
 * @brief  Enable the LSM6DSV16BX gyroscope sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_GYRO_Enable(LSM6DSV16BX_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->gyro_is_enabled == 1U)
  {
    return LSM6DSV16BX_OK;
  }

  /* Output data rate selection. */
  if (lsm6dsv16bx_gy_data_rate_set(&(pObj->Ctx), pObj->gyro_odr) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  pObj->gyro_is_enabled = 1;

  return LSM6DSV16BX_OK;
}

/**
 * @brief  Disable the LSM6DSV16BX gyroscope sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_GYRO_Disable(LSM6DSV16BX_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->gyro_is_enabled == 0U)
  {
    return LSM6DSV16BX_OK;
  }

  /* Get current output data rate. */
  if (lsm6dsv16bx_gy_data_rate_get(&(pObj->Ctx), &pObj->gyro_odr) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  /* Output data rate selection - power down. */
  if (lsm6dsv16bx_gy_data_rate_set(&(pObj->Ctx), LSM6DSV16BX_GY_ODR_OFF) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  pObj->gyro_is_enabled = 0;

  return LSM6DSV16BX_OK;
}

/**
 * @brief  Get the LSM6DSV16BX gyroscope sensor sensitivity
 * @param  pObj the device pObj
 * @param  Sensitivity pointer
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_GYRO_GetSensitivity(LSM6DSV16BX_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = LSM6DSV16BX_OK;
  lsm6dsv16bx_gy_full_scale_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (lsm6dsv16bx_gy_full_scale_get(&(pObj->Ctx), &full_scale) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  /* Store the sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case LSM6DSV16BX_125dps:
      *Sensitivity = LSM6DSV16BX_GYRO_SENSITIVITY_FS_125DPS;
      break;

    case LSM6DSV16BX_250dps:
      *Sensitivity = LSM6DSV16BX_GYRO_SENSITIVITY_FS_250DPS;
      break;

    case LSM6DSV16BX_500dps:
      *Sensitivity = LSM6DSV16BX_GYRO_SENSITIVITY_FS_500DPS;
      break;

    case LSM6DSV16BX_1000dps:
      *Sensitivity = LSM6DSV16BX_GYRO_SENSITIVITY_FS_1000DPS;
      break;

    case LSM6DSV16BX_2000dps:
      *Sensitivity = LSM6DSV16BX_GYRO_SENSITIVITY_FS_2000DPS;
      break;

    case LSM6DSV16BX_4000dps:
      *Sensitivity = LSM6DSV16BX_GYRO_SENSITIVITY_FS_4000DPS;
      break;

    default:
      ret = LSM6DSV16BX_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Get the LSM6DSV16BX gyroscope sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr pointer where the output data rate is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_GYRO_GetOutputDataRate(LSM6DSV16BX_Object_t *pObj, float *Odr)
{
  int32_t ret = LSM6DSV16BX_OK;
  lsm6dsv16bx_gy_data_rate_t odr_low_level;

  /* Get current output data rate. */
  if (lsm6dsv16bx_gy_data_rate_get(&(pObj->Ctx), &odr_low_level) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  switch (odr_low_level)
  {
    case LSM6DSV16BX_GY_ODR_OFF:
      *Odr = 0.0f;
      break;

    case LSM6DSV16BX_GY_ODR_AT_7Hz5:
      *Odr = 7.5f;
      break;

    case LSM6DSV16BX_GY_ODR_AT_15Hz:
      *Odr = 15.0f;
      break;

    case LSM6DSV16BX_GY_ODR_AT_30Hz:
      *Odr = 30.0f;
      break;

    case LSM6DSV16BX_GY_ODR_AT_60Hz:
      *Odr = 60.0f;
      break;

    case LSM6DSV16BX_GY_ODR_AT_120Hz:
      *Odr = 120.0f;
      break;

    case LSM6DSV16BX_GY_ODR_AT_240Hz:
      *Odr = 240.0f;
      break;

    case LSM6DSV16BX_GY_ODR_AT_480Hz:
      *Odr = 480.0f;
      break;

    case LSM6DSV16BX_GY_ODR_AT_960Hz:
      *Odr = 960.0f;
      break;

    case LSM6DSV16BX_GY_ODR_AT_1920Hz:
      *Odr = 1920.0f;
      break;

    case LSM6DSV16BX_GY_ODR_AT_3840Hz:
      *Odr = 3840.0f;
      break;

    case LSM6DSV16BX_GY_ODR_AT_7680Hz:
      *Odr = 7680.0f;
      break;

    default:
      ret = LSM6DSV16BX_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the LSM6DSV16BX gyroscope sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_GYRO_SetOutputDataRate(LSM6DSV16BX_Object_t *pObj, float Odr)
{
  return LSM6DSV16BX_GYRO_SetOutputDataRate_With_Mode(pObj, Odr, LSM6DSV16BX_GYRO_HIGH_PERFORMANCE_MODE);
}

/**
 * @brief  Set the LSM6DSV16BX gyroscope sensor output data rate with operating mode
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @param  Mode the gyroscope operating mode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_GYRO_SetOutputDataRate_With_Mode(LSM6DSV16BX_Object_t *pObj, float Odr, LSM6DSV16BX_GYRO_Operating_Mode_t Mode)
{
  switch (Mode)
  {
    case LSM6DSV16BX_GYRO_HIGH_PERFORMANCE_MODE:
    {
      if (lsm6dsv16bx_gy_mode_set(&(pObj->Ctx), LSM6DSV16BX_GY_HIGH_PERFORMANCE_MD) != LSM6DSV16BX_OK)
      {
        return LSM6DSV16BX_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 7.68kHz */
      Odr = (Odr <    7.5f) ?    7.5f
    	  : (Odr > 7680.0f) ? 7680.0f
    	  :                       Odr;
      break;
    }

    case LSM6DSV16BX_GYRO_HIGH_ACCURACY_MODE:
      return LSM6DSV16BX_ERROR;

    case LSM6DSV16BX_GYRO_SLEEP_MODE:
      return LSM6DSV16BX_ERROR;

    case LSM6DSV16BX_GYRO_LOW_POWER_MODE:
    {
      if (lsm6dsv16bx_gy_mode_set(&(pObj->Ctx), LSM6DSV16BX_GY_LOW_POWER_MD) != LSM6DSV16BX_OK)
      {
        return LSM6DSV16BX_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 240kHz */
      Odr = (Odr <   7.5f) ?   7.5f
    	  : (Odr > 240.0f) ? 240.0f
    	  :                     Odr;
      break;
    }

    default:
      return LSM6DSV16BX_ERROR;
  }

  if (pObj->gyro_is_enabled == 1U)
  {
    return LSM6DSV16BX_GYRO_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return LSM6DSV16BX_GYRO_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
 * @brief  Get the LSM6DSV16BX gyroscope sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale pointer where the full scale is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_GYRO_GetFullScale(LSM6DSV16BX_Object_t *pObj, int32_t  *FullScale)
{
  int32_t ret = LSM6DSV16BX_OK;
  lsm6dsv16bx_gy_full_scale_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (lsm6dsv16bx_gy_full_scale_get(&(pObj->Ctx), &fs_low_level) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  switch (fs_low_level)
  {
    case LSM6DSV16BX_125dps:
      *FullScale =  125;
      break;

    case LSM6DSV16BX_250dps:
      *FullScale =  250;
      break;

    case LSM6DSV16BX_500dps:
      *FullScale =  500;
      break;

    case LSM6DSV16BX_1000dps:
      *FullScale = 1000;
      break;

    case LSM6DSV16BX_2000dps:
      *FullScale = 2000;
      break;

    case LSM6DSV16BX_4000dps:
      *FullScale = 4000;
      break;

    default:
      ret = LSM6DSV16BX_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the LSM6DSV16BX gyroscope sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale the functional full scale to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_GYRO_SetFullScale(LSM6DSV16BX_Object_t *pObj, int32_t FullScale)
{
	lsm6dsv16bx_gy_full_scale_t new_fs;

  new_fs = (FullScale <= 125)  ? LSM6DSV16BX_125dps
         : (FullScale <= 250)  ? LSM6DSV16BX_250dps
         : (FullScale <= 500)  ? LSM6DSV16BX_500dps
         : (FullScale <= 1000) ? LSM6DSV16BX_1000dps
         : (FullScale <= 2000) ? LSM6DSV16BX_2000dps
         :                       LSM6DSV16BX_4000dps;

  if (lsm6dsv16bx_gy_full_scale_set(&(pObj->Ctx), new_fs) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  return LSM6DSV16BX_OK;
}

/**
 * @brief  Get the LSM6DSV16BX gyroscope sensor raw axes
 * @param  pObj the device pObj
 * @param  Value pointer where the raw values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_GYRO_GetAxesRaw(LSM6DSV16BX_Object_t *pObj, LSM6DSV16BX_AxesRaw_t *Value)
{
  lsm6dsv16bx_axis3bit16_t data_raw;

  /* Read raw data values. */
  if (lsm6dsv16bx_angular_rate_raw_get(&(pObj->Ctx), data_raw.i16bit) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return LSM6DSV16BX_OK;
}

/**
 * @brief  Get the LSM6DSV16BX gyroscope sensor axes
 * @param  pObj the device pObj
 * @param  AngularRate pointer where the values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_GYRO_GetAxes(LSM6DSV16BX_Object_t *pObj, LSM6DSV16BX_Axes_t *AngularRate)
{
  lsm6dsv16bx_axis3bit16_t data_raw;
  float sensitivity;

  /* Read raw data values. */
  if (lsm6dsv16bx_angular_rate_raw_get(&(pObj->Ctx), data_raw.i16bit) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  /* Get LSM6DSV16BX actual sensitivity. */
  if (LSM6DSV16BX_GYRO_GetSensitivity(pObj, &sensitivity) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  /* Calculate the data. */
  AngularRate->x = (int32_t)((float)((float)data_raw.i16bit[0] * sensitivity));
  AngularRate->y = (int32_t)((float)((float)data_raw.i16bit[1] * sensitivity));
  AngularRate->z = (int32_t)((float)((float)data_raw.i16bit[2] * sensitivity));

  return LSM6DSV16BX_OK;
}

/**
 * @brief  Get the LSM6DSV16BX register value
 * @param  pObj the device pObj
 * @param  Reg address to be read
 * @param  Data pointer where the value is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_Read_Reg(LSM6DSV16BX_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (lsm6dsv16bx_read_reg(&(pObj->Ctx), Reg, Data, 1) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  return LSM6DSV16BX_OK;
}

/**
 * @brief  Set the LSM6DSV16BX register value
 * @param  pObj the device pObj
 * @param  Reg address to be written
 * @param  Data value to be written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_Write_Reg(LSM6DSV16BX_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (lsm6dsv16bx_write_reg(&(pObj->Ctx), Reg, &Data, 1) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  return LSM6DSV16BX_OK;
}

/**
 * @brief  Get the LSM6DSV16BX ACC data ready bit value
 * @param  pObj the device pObj
 * @param  Status the status of data ready bit
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_ACC_Get_DRDY_Status(LSM6DSV16BX_Object_t *pObj, uint8_t *Status)
{
  lsm6dsv16bx_all_sources_t val;

  if (lsm6dsv16bx_all_sources_get(&(pObj->Ctx), &val) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  *Status = val.drdy_xl;
  return LSM6DSV16BX_OK;
}

/**
 * @brief  Get the LSM6DSV16BX GYRO data ready bit value
 * @param  pObj the device pObj
 * @param  Status the status of data ready bit
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_GYRO_Get_DRDY_Status(LSM6DSV16BX_Object_t *pObj, uint8_t *Status)
{
  lsm6dsv16bx_all_sources_t val;

  if (lsm6dsv16bx_all_sources_get(&(pObj->Ctx), &val) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  *Status = val.drdy_gy;
  return LSM6DSV16BX_OK;
}

/**
 * @}
 */

/** @defgroup LSM6DSV16BX_Private_Functions LSM6DSV16BX Private Functions
 * @{
 */

/**
 * @brief  Set the LSM6DSV16BX accelerometer sensor output data rate when enabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t LSM6DSV16BX_ACC_SetOutputDataRate_When_Enabled(LSM6DSV16BX_Object_t *pObj, float Odr)
{
  lsm6dsv16bx_xl_data_rate_t new_odr;

  new_odr = (Odr <=    1.875f) ? LSM6DSV16BX_XL_ODR_AT_1Hz875
          : (Odr <=    7.5f  ) ? LSM6DSV16BX_XL_ODR_AT_7Hz5
          : (Odr <=   15.0f  ) ? LSM6DSV16BX_XL_ODR_AT_15Hz
          : (Odr <=   30.0f  ) ? LSM6DSV16BX_XL_ODR_AT_30Hz
          : (Odr <=   60.0f  ) ? LSM6DSV16BX_XL_ODR_AT_60Hz
          : (Odr <=  120.0f  ) ? LSM6DSV16BX_XL_ODR_AT_120Hz
          : (Odr <=  240.0f  ) ? LSM6DSV16BX_XL_ODR_AT_240Hz
          : (Odr <=  480.0f  ) ? LSM6DSV16BX_XL_ODR_AT_480Hz
          : (Odr <=  960.0f  ) ? LSM6DSV16BX_XL_ODR_AT_960Hz
          : (Odr <= 1920.0f  ) ? LSM6DSV16BX_XL_ODR_AT_1920Hz
          : (Odr <= 3840.0f  ) ? LSM6DSV16BX_XL_ODR_AT_3840Hz
          :                      LSM6DSV16BX_XL_ODR_AT_7680Hz;

  /* Output data rate selection. */
  if (lsm6dsv16bx_xl_data_rate_set(&(pObj->Ctx), new_odr) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  return LSM6DSV16BX_OK;
}

/**
 * @brief  Set the LSM6DSV16BX accelerometer sensor output data rate when disabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t LSM6DSV16BX_ACC_SetOutputDataRate_When_Disabled(LSM6DSV16BX_Object_t *pObj, float Odr)
{
  pObj->acc_odr = (Odr <=    1.875f) ? LSM6DSV16BX_XL_ODR_AT_1Hz875
  	            : (Odr <=    7.5f  ) ? LSM6DSV16BX_XL_ODR_AT_7Hz5
	            : (Odr <=   15.0f  ) ? LSM6DSV16BX_XL_ODR_AT_15Hz
	            : (Odr <=   30.0f  ) ? LSM6DSV16BX_XL_ODR_AT_30Hz
	            : (Odr <=   60.0f  ) ? LSM6DSV16BX_XL_ODR_AT_60Hz
	            : (Odr <=  120.0f  ) ? LSM6DSV16BX_XL_ODR_AT_120Hz
	            : (Odr <=  240.0f  ) ? LSM6DSV16BX_XL_ODR_AT_240Hz
	            : (Odr <=  480.0f  ) ? LSM6DSV16BX_XL_ODR_AT_480Hz
  	            : (Odr <=  960.0f  ) ? LSM6DSV16BX_XL_ODR_AT_960Hz
	            : (Odr <= 1920.0f  ) ? LSM6DSV16BX_XL_ODR_AT_1920Hz
	            : (Odr <= 3840.0f  ) ? LSM6DSV16BX_XL_ODR_AT_3840Hz
	            :                      LSM6DSV16BX_XL_ODR_AT_7680Hz;

  return LSM6DSV16BX_OK;
}

/**
 * @brief  Set the LSM6DSV16BX gyroscope sensor output data rate when enabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t LSM6DSV16BX_GYRO_SetOutputDataRate_When_Enabled(LSM6DSV16BX_Object_t *pObj, float Odr)
{
  lsm6dsv16bx_gy_data_rate_t new_odr;

  new_odr = (Odr <=    7.5f) ? LSM6DSV16BX_GY_ODR_AT_7Hz5
          : (Odr <=   15.0f) ? LSM6DSV16BX_GY_ODR_AT_15Hz
          : (Odr <=   30.0f) ? LSM6DSV16BX_GY_ODR_AT_30Hz
          : (Odr <=   60.0f) ? LSM6DSV16BX_GY_ODR_AT_60Hz
          : (Odr <=  120.0f) ? LSM6DSV16BX_GY_ODR_AT_120Hz
          : (Odr <=  240.0f) ? LSM6DSV16BX_GY_ODR_AT_240Hz
          : (Odr <=  480.0f) ? LSM6DSV16BX_GY_ODR_AT_480Hz
          : (Odr <=  960.0f) ? LSM6DSV16BX_GY_ODR_AT_960Hz
          : (Odr <= 1920.0f) ? LSM6DSV16BX_GY_ODR_AT_1920Hz
          : (Odr <= 3840.0f) ? LSM6DSV16BX_GY_ODR_AT_3840Hz
          :                    LSM6DSV16BX_GY_ODR_AT_7680Hz;

  /* Output data rate selection. */
  if (lsm6dsv16bx_gy_data_rate_set(&(pObj->Ctx), new_odr) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  return LSM6DSV16BX_OK;
}

/**
 * @brief  Set the LSM6DSV16BX gyroscope sensor output data rate when disabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t LSM6DSV16BX_GYRO_SetOutputDataRate_When_Disabled(LSM6DSV16BX_Object_t *pObj, float Odr)
{
  pObj->gyro_odr = (Odr <=    7.5f) ? LSM6DSV16BX_GY_ODR_AT_7Hz5
	             : (Odr <=   15.0f) ? LSM6DSV16BX_GY_ODR_AT_15Hz
	             : (Odr <=   30.0f) ? LSM6DSV16BX_GY_ODR_AT_30Hz
	             : (Odr <=   60.0f) ? LSM6DSV16BX_GY_ODR_AT_60Hz
	             : (Odr <=  120.0f) ? LSM6DSV16BX_GY_ODR_AT_120Hz
	             : (Odr <=  240.0f) ? LSM6DSV16BX_GY_ODR_AT_240Hz
	             : (Odr <=  480.0f) ? LSM6DSV16BX_GY_ODR_AT_480Hz
	             : (Odr <=  960.0f) ? LSM6DSV16BX_GY_ODR_AT_960Hz
	             : (Odr <= 1920.0f) ? LSM6DSV16BX_GY_ODR_AT_1920Hz
	             : (Odr <= 3840.0f) ? LSM6DSV16BX_GY_ODR_AT_3840Hz
	             :                    LSM6DSV16BX_GY_ODR_AT_7680Hz;

  return LSM6DSV16BX_OK;
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
  LSM6DSV16BX_Object_t *pObj = (LSM6DSV16BX_Object_t *)Handle;

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
  LSM6DSV16BX_Object_t *pObj = (LSM6DSV16BX_Object_t *)Handle;

  return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
}


/**
 * @brief  Set the LSM6DSV16BX accelerometer power mode
 * @param  pObj the device pObj
 * @param  PowerMode Value of the powerMode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_ACC_Set_Power_Mode(LSM6DSV16BX_Object_t *pObj, uint8_t PowerMode)
{
  if(lsm6dsv16bx_xl_mode_set(&(pObj->Ctx), (lsm6dsv16bx_xl_mode_t)PowerMode) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  return LSM6DSV16BX_OK;
}


/**
 * @brief  Set the LSM6DSV16BX gyroscope power mode
 * @param  pObj the device pObj
 * @param  PowerMode Value of the powerMode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_GYRO_Set_Power_Mode(LSM6DSV16BX_Object_t *pObj, uint8_t PowerMode)
{
  if(lsm6dsv16bx_gy_mode_set(&(pObj->Ctx), (lsm6dsv16bx_gy_mode_t)PowerMode) != LSM6DSV16BX_OK)
  {
    return LSM6DSV16BX_ERROR;
  }

  return LSM6DSV16BX_OK;
}


/**
 * @brief  Set the LSM6DSV16BX accelerometer filter mode
 * @param  pObj the device pObj
 * @param  LowHighPassFlag 0/1 for setting low-pass/high-pass filter mode
 * @param  FilterMode Value of the filter Mode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_ACC_Set_Filter_Mode(LSM6DSV16BX_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode)
{
  if(LowHighPassFlag == 0)
  {
    /*Set accelerometer low_pass filter-mode*/

    /*Set to 1 LPF2 bit (CTRL8_XL)*/
    if(lsm6dsv16bx_filt_xl_lp2_set(&(pObj->Ctx), 1) != LSM6DSV16BX_OK)
    {
      return LSM6DSV16BX_ERROR;
    }
    if(lsm6dsv16bx_filt_xl_lp2_bandwidth_set(&(pObj->Ctx), (lsm6dsv16bx_filt_xl_lp2_bandwidth_t)FilterMode) != LSM6DSV16BX_OK)
    {
      return LSM6DSV16BX_ERROR;
    }
  }
  else
  {
    if(lsm6dsv16bx_filt_xl_lp2_set(&(pObj->Ctx), 0) != LSM6DSV16BX_OK)
    {
      return LSM6DSV16BX_ERROR;
    }
    /*Set accelerometer high_pass filter-mode*/
    if(lsm6dsv16bx_filt_xl_lp2_bandwidth_set(&(pObj->Ctx), (lsm6dsv16bx_filt_xl_lp2_bandwidth_t)FilterMode) != LSM6DSV16BX_OK)
    {
      return LSM6DSV16BX_ERROR;
    }
  }
  return LSM6DSV16BX_OK;
}


/**
 * @brief  Set the LSM6DSV16BX gyroscope filter mode
 * @param  pObj the device pObj
 * @param  LowHighPassFlag 0/1 for setting low-pass/high-pass filter mode
 * @param  FilterMode Value of the filter Mode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t LSM6DSV16BX_GYRO_Set_Filter_Mode(LSM6DSV16BX_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode)
{
  if(LowHighPassFlag == 0)
  {
    /*Set gyroscope low_pass 1 filter-mode*/
    /* Enable low-pass filter */
    if(lsm6dsv16bx_filt_gy_lp1_set(&(pObj->Ctx), 1) != LSM6DSV16BX_OK)
    {
      return LSM6DSV16BX_ERROR;
    }
    if(lsm6dsv16bx_filt_gy_lp1_bandwidth_set(&(pObj->Ctx), (lsm6dsv16bx_filt_gy_lp1_bandwidth_t)FilterMode) != LSM6DSV16BX_OK)
    {
      return LSM6DSV16BX_ERROR;
    }
  }
  else
  {
    /*Set gyroscope high_pass filter-mode*/
    /* Enable high-pass filter */
    if(lsm6dsv16bx_filt_gy_lp1_set(&(pObj->Ctx), 0) != LSM6DSV16BX_OK)
    {
      return LSM6DSV16BX_ERROR;
    }
    if(lsm6dsv16bx_filt_gy_lp1_bandwidth_set(&(pObj->Ctx), (lsm6dsv16bx_filt_gy_lp1_bandwidth_t)FilterMode) != LSM6DSV16BX_OK)
    {
      return LSM6DSV16BX_ERROR;
    }
  }
  return LSM6DSV16BX_OK;
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
