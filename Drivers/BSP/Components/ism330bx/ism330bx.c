/**
 ******************************************************************************
 * @file    ism330bx.c
 * @author  MEMS Software Solutions Team
 * @brief   ISM330BX driver file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "ism330bx.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @defgroup ISM330BX ISM330BX
 * @{
 */

/** @defgroup ISM330BX_Exported_Variables ISM330BX Exported Variables
 * @{
 */

ISM330BX_CommonDrv_t ISM330BX_COMMON_Driver =
{
  ISM330BX_Init,
  ISM330BX_DeInit,
  ISM330BX_ReadID,
  ISM330BX_GetCapabilities,
};

ISM330BX_ACC_Drv_t ISM330BX_ACC_Driver =
{
  ISM330BX_ACC_Enable,
  ISM330BX_ACC_Disable,
  ISM330BX_ACC_GetSensitivity,
  ISM330BX_ACC_GetOutputDataRate,
  ISM330BX_ACC_SetOutputDataRate,
  ISM330BX_ACC_GetFullScale,
  ISM330BX_ACC_SetFullScale,
  ISM330BX_ACC_GetAxes,
  ISM330BX_ACC_GetAxesRaw,
};

ISM330BX_GYRO_Drv_t ISM330BX_GYRO_Driver =
{
  ISM330BX_GYRO_Enable,
  ISM330BX_GYRO_Disable,
  ISM330BX_GYRO_GetSensitivity,
  ISM330BX_GYRO_GetOutputDataRate,
  ISM330BX_GYRO_SetOutputDataRate,
  ISM330BX_GYRO_GetFullScale,
  ISM330BX_GYRO_SetFullScale,
  ISM330BX_GYRO_GetAxes,
  ISM330BX_GYRO_GetAxesRaw,
};

/**
 * @}
 */

/** @defgroup ISM330BX_Private_Function_Prototypes ISM330BX Private Function Prototypes
 * @{
 */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t ISM330BX_ACC_SetOutputDataRate_When_Enabled(ISM330BX_Object_t *pObj, float_t Odr);
static int32_t ISM330BX_ACC_SetOutputDataRate_When_Disabled(ISM330BX_Object_t *pObj, float_t Odr);
static int32_t ISM330BX_GYRO_SetOutputDataRate_When_Enabled(ISM330BX_Object_t *pObj, float_t Odr);
static int32_t ISM330BX_GYRO_SetOutputDataRate_When_Disabled(ISM330BX_Object_t *pObj, float_t Odr);

/**
 * @}
 */

/** @defgroup ISM330BX_Exported_Functions ISM330BX Exported Functions
 * @{
 */

/**
 * @brief  Register Component Bus IO operations
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_RegisterBusIO(ISM330BX_Object_t *pObj, ISM330BX_IO_t *pIO)
{
  int32_t ret = ISM330BX_OK;

  if (pObj == NULL)
  {
    ret = ISM330BX_ERROR;
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
      ret = ISM330BX_ERROR;
    }
    else if (pObj->IO.Init() != ISM330BX_OK)
    {
      ret = ISM330BX_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == ISM330BX_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x04;

          if (ISM330BX_Write_Reg(pObj, ISM330BX_CTRL3, data) != ISM330BX_OK)
          {
            ret = ISM330BX_ERROR;
          }

          if (ISM330BX_Write_Reg(pObj, ISM330BX_IF_CFG, data) != ISM330BX_OK)
          {
            ret = ISM330BX_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
 * @brief  Initialize the ISM330BX sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_Init(ISM330BX_Object_t *pObj)
{
  /* Set main memory bank */
  if (ISM330BX_Set_Mem_Bank(pObj, (uint8_t)ISM330BX_MAIN_MEM_BANK) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  /* Enable register address automatically incremented during a multiple byte
  access with a serial interface. */
  if (ism330bx_auto_increment_set(&(pObj->Ctx), PROPERTY_ENABLE) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  /* Enable BDU */
  if (ism330bx_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  /* FIFO mode selection */
  if (ism330bx_fifo_mode_set(&(pObj->Ctx), ISM330BX_BYPASS_MODE) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = ISM330BX_XL_ODR_AT_120Hz;

  /* Output data rate selection - power down. */
  if (ism330bx_xl_data_rate_set(&(pObj->Ctx), ISM330BX_XL_ODR_OFF) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  /* Full scale selection. */
  if (ism330bx_xl_full_scale_set(&(pObj->Ctx), ISM330BX_2g) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  /* Select default output data rate. */
  pObj->gyro_odr = ISM330BX_GY_ODR_AT_120Hz;

  /* Output data rate selection - power down. */
  if (ism330bx_gy_data_rate_set(&(pObj->Ctx), ISM330BX_GY_ODR_OFF) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  /* Full scale selection. */
  if (ism330bx_gy_full_scale_set(&(pObj->Ctx), ISM330BX_2000dps) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  /* Enable Qvar functionality */
  ism330bx_ah_qvar_mode_t mode;

  mode.ah_qvar1_en = 1;
  mode.ah_qvar2_en = 0;
  mode.swaps       = 0;

  if (ism330bx_ah_qvar_mode_set(&(pObj->Ctx), mode) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  pObj->is_initialized = 1;

  return ISM330BX_OK;
}

/**
 * @brief  Deinitialize the ISM330BX sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_DeInit(ISM330BX_Object_t *pObj)
{
  /* Disable the component */
  if (ISM330BX_ACC_Disable(pObj) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  if (ISM330BX_GYRO_Disable(pObj) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  /* Reset output data rate. */
  pObj->acc_odr = ISM330BX_XL_ODR_OFF;
  pObj->gyro_odr = ISM330BX_GY_ODR_OFF;

  /* Disable Qvar functionality */
  ism330bx_ah_qvar_mode_t mode;

  mode.ah_qvar1_en = 0;
  mode.ah_qvar2_en = 0;
  mode.swaps       = 0;

  if (ism330bx_ah_qvar_mode_set(&(pObj->Ctx), mode) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  pObj->is_initialized = 0;

  return ISM330BX_OK;
}

/**
 * @brief  Read component ID
 * @param  pObj the device pObj
 * @param  Id the WHO_AM_I value
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_ReadID(ISM330BX_Object_t *pObj, uint8_t *Id)
{
  if (ism330bx_device_id_get(&(pObj->Ctx), Id) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  return ISM330BX_OK;
}

/**
 * @brief  Get ISM330BX sensor capabilities
 * @param  pObj Component object pointer
 * @param  Capabilities pointer to ISM330BX sensor capabilities
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_GetCapabilities(ISM330BX_Object_t *pObj, ISM330BX_Capabilities_t *Capabilities)
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
  Capabilities->GyroMaxOdr   = 3840.0f;
  Capabilities->AccMaxOdr    = 3840.0f;
  Capabilities->MagMaxOdr    = 0.0f;
  return ISM330BX_OK;
}

/**
 * @brief  Enable the ISM330BX accelerometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_ACC_Enable(ISM330BX_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return ISM330BX_OK;
  }

  /* Output data rate selection. */
  if (ism330bx_xl_data_rate_set(&(pObj->Ctx), pObj->acc_odr) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return ISM330BX_OK;
}

/**
 * @brief  Disable the ISM330BX accelerometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_ACC_Disable(ISM330BX_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return ISM330BX_OK;
  }

  /* Get current output data rate. */
  if (ism330bx_xl_data_rate_get(&(pObj->Ctx), &pObj->acc_odr) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  /* Output data rate selection - power down. */
  if (ism330bx_xl_data_rate_set(&(pObj->Ctx), ISM330BX_XL_ODR_OFF) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return ISM330BX_OK;
}

/**
 * @brief  Get the ISM330BX accelerometer sensor sensitivity
 * @param  pObj the device pObj
 * @param  Sensitivity pointer
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_ACC_GetSensitivity(ISM330BX_Object_t *pObj, float_t *Sensitivity)
{
  int32_t ret = ISM330BX_OK;
  ism330bx_xl_full_scale_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (ism330bx_xl_full_scale_get(&(pObj->Ctx), &full_scale) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  /* Store the Sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case ISM330BX_2g:
      *Sensitivity = ISM330BX_ACC_SENSITIVITY_FS_2G;
      break;

    case ISM330BX_4g:
      *Sensitivity = ISM330BX_ACC_SENSITIVITY_FS_4G;
      break;

    case ISM330BX_8g:
      *Sensitivity = ISM330BX_ACC_SENSITIVITY_FS_8G;
      break;

    default:
      ret = ISM330BX_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Get the ISM330BX accelerometer sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr pointer where the output data rate is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_ACC_GetOutputDataRate(ISM330BX_Object_t *pObj, float_t *Odr)
{
  int32_t ret = ISM330BX_OK;
  ism330bx_xl_data_rate_t odr_low_level;

  /* Get current output data rate. */
  if (ism330bx_xl_data_rate_get(&(pObj->Ctx), &odr_low_level) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  switch (odr_low_level)
  {
    case ISM330BX_XL_ODR_OFF:
      *Odr = 0.0f;
      break;

    case ISM330BX_XL_ODR_AT_1Hz875:
      *Odr = 1.875f;
      break;

    case ISM330BX_XL_ODR_AT_7Hz5:
      *Odr = 7.5f;
      break;

    case ISM330BX_XL_ODR_AT_15Hz:
      *Odr = 15.0f;
      break;

    case ISM330BX_XL_ODR_AT_30Hz:
      *Odr = 30.0f;
      break;

    case ISM330BX_XL_ODR_AT_60Hz:
      *Odr = 60.0f;
      break;

    case ISM330BX_XL_ODR_AT_120Hz:
      *Odr = 120.0f;
      break;

    case ISM330BX_XL_ODR_AT_240Hz:
      *Odr = 240.0f;
      break;

    case ISM330BX_XL_ODR_AT_480Hz:
      *Odr = 480.0f;
      break;

    case ISM330BX_XL_ODR_AT_960Hz:
      *Odr = 960.0f;
      break;

    case ISM330BX_XL_ODR_AT_1920Hz:
      *Odr = 1920.0f;
      break;

    case ISM330BX_XL_ODR_AT_3840Hz:
      *Odr = 3840.0f;
      break;

    default:
      ret = ISM330BX_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the ISM330BX accelerometer sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_ACC_SetOutputDataRate(ISM330BX_Object_t *pObj, float_t Odr)
{
  return ISM330BX_ACC_SetOutputDataRate_With_Mode(pObj, Odr, ISM330BX_ACC_HIGH_PERFORMANCE_MODE);
}

/**
 * @brief  Set the ISM330BX accelerometer sensor output data rate with operating mode
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @param  Mode the accelerometer operating mode
 * @note   This function switches off the gyroscope if Ultra Low Power Mode is set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_ACC_SetOutputDataRate_With_Mode(ISM330BX_Object_t *pObj, float_t Odr, ISM330BX_ACC_Operating_Mode_t Mode)
{
  int32_t ret = ISM330BX_OK;
  float_t odr_local = 0.0f;

  switch (Mode)
  {
    case ISM330BX_ACC_HIGH_PERFORMANCE_MODE:
      if (ism330bx_xl_mode_set(&(pObj->Ctx), ISM330BX_XL_HIGH_PERFORMANCE_MD) != ISM330BX_OK)
      {
        return ISM330BX_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 3.84kHz */
      odr_local = (Odr <    7.5f) ?    7.5f
    	          : (Odr > 3840.0f) ? 3840.0f
    	          :                       Odr;
      break;

    case ISM330BX_ACC_HIGH_PERFORMANCE_TDM_MODE:
      if (ism330bx_xl_mode_set(&(pObj->Ctx), ISM330BX_XL_HIGH_PERFORMANCE_TDM_MD) != ISM330BX_OK)
      {
        return ISM330BX_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 3.84kHz */
      odr_local = (Odr <    7.5f) ?    7.5f
    	          : (Odr > 3840.0f) ? 3840.0f
    	          :                       Odr;
      break;

    case ISM330BX_ACC_LOW_POWER_MODE1:
      if (ism330bx_xl_mode_set(&(pObj->Ctx), ISM330BX_XL_LOW_POWER_2_AVG_MD) != ISM330BX_OK)
      {
        return ISM330BX_ERROR;
      }

      /* Valid ODR: 1.875Hz;  15Hz <= Odr <= 240kHz */
      odr_local = (Odr ==   1.875f) ?    Odr
    	          : (Odr <   15.000f) ?  15.0f
    	          : (Odr >  240.000f) ? 240.0f
    	          :                        Odr;
      break;

    case ISM330BX_ACC_LOW_POWER_MODE2:
      if (ism330bx_xl_mode_set(&(pObj->Ctx), ISM330BX_XL_LOW_POWER_4_AVG_MD) != ISM330BX_OK)
      {
        return ISM330BX_ERROR;
      }

      /* Valid ODR: 1.875Hz;  15Hz <= Odr <= 240kHz */
      odr_local = (Odr ==   1.875f) ?    Odr
    	          : (Odr <   15.000f) ?  15.0f
    	          : (Odr >  240.000f) ? 240.0f
    	          :                        Odr;
      break;

    case ISM330BX_ACC_LOW_POWER_MODE3:
      if (ism330bx_xl_mode_set(&(pObj->Ctx), ISM330BX_XL_LOW_POWER_8_AVG_MD) != ISM330BX_OK)
      {
        return ISM330BX_ERROR;
      }

      /* Valid ODR: 1.875Hz;  15Hz <= Odr <= 240kHz */
      odr_local = (Odr ==   1.875f) ?    Odr
    	          : (Odr <   15.000f) ?  15.0f
    	          : (Odr >  240.000f) ? 240.0f
    	          :                        Odr;
      break;

    default:
      ret = ISM330BX_ERROR;
      break;
  }

  if (ret != ISM330BX_OK)
  {
    return ret;
  }

  if (pObj->acc_is_enabled == 1U)
  {
    return ISM330BX_ACC_SetOutputDataRate_When_Enabled(pObj, odr_local);
  }
  else
  {
    return ISM330BX_ACC_SetOutputDataRate_When_Disabled(pObj, odr_local);
  }
}

/**
 * @brief  Get the ISM330BX accelerometer sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale pointer where the full scale is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_ACC_GetFullScale(ISM330BX_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = ISM330BX_OK;
  ism330bx_xl_full_scale_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (ism330bx_xl_full_scale_get(&(pObj->Ctx), &fs_low_level) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  switch (fs_low_level)
  {
    case ISM330BX_2g:
      *FullScale =  2;
      break;

    case ISM330BX_4g:
      *FullScale =  4;
      break;

    case ISM330BX_8g:
      *FullScale =  8;
      break;

    default:
      ret = ISM330BX_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the ISM330BX accelerometer sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale the functional full scale to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_ACC_SetFullScale(ISM330BX_Object_t *pObj, int32_t FullScale)
{
  ism330bx_xl_full_scale_t new_fs;

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file statical analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  new_fs = (FullScale <= 2) ? ISM330BX_2g
         : (FullScale <= 4) ? ISM330BX_4g
         :                    ISM330BX_8g;

  if (ism330bx_xl_full_scale_set(&(pObj->Ctx), new_fs) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  return ISM330BX_OK;
}

/**
 * @brief  Get the ISM330BX accelerometer sensor raw axes
 * @param  pObj the device pObj
 * @param  Value pointer where the raw values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_ACC_GetAxesRaw(ISM330BX_Object_t *pObj, ISM330BX_AxesRaw_t *Value)
{
  ism330bx_axis3bit16_t data_raw;

  /* Read raw data values. */
  if (ism330bx_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return ISM330BX_OK;
}

/**
 * @brief  Get the ISM330BX accelerometer sensor axes
 * @param  pObj the device pObj
 * @param  Acceleration pointer where the values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_ACC_GetAxes(ISM330BX_Object_t *pObj, ISM330BX_Axes_t *Acceleration)
{
  ism330bx_axis3bit16_t data_raw;
  float_t sensitivity = 0.0f;

  /* Read raw data values. */
  if (ism330bx_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  /* Get ISM330BX actual sensitivity. */
  if (ISM330BX_ACC_GetSensitivity(pObj, &sensitivity) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  /* Calculate the data. */
  Acceleration->x = (int32_t)((float_t)((float_t)data_raw.i16bit[0] * sensitivity));
  Acceleration->y = (int32_t)((float_t)((float_t)data_raw.i16bit[1] * sensitivity));
  Acceleration->z = (int32_t)((float_t)((float_t)data_raw.i16bit[2] * sensitivity));

  return ISM330BX_OK;
}

/**
 * @brief  Enable the ISM330BX gyroscope sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_GYRO_Enable(ISM330BX_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->gyro_is_enabled == 1U)
  {
    return ISM330BX_OK;
  }

  /* Output data rate selection. */
  if (ism330bx_gy_data_rate_set(&(pObj->Ctx), pObj->gyro_odr) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  pObj->gyro_is_enabled = 1;

  return ISM330BX_OK;
}

/**
 * @brief  Disable the ISM330BX gyroscope sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_GYRO_Disable(ISM330BX_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->gyro_is_enabled == 0U)
  {
    return ISM330BX_OK;
  }

  /* Get current output data rate. */
  if (ism330bx_gy_data_rate_get(&(pObj->Ctx), &pObj->gyro_odr) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  /* Output data rate selection - power down. */
  if (ism330bx_gy_data_rate_set(&(pObj->Ctx), ISM330BX_GY_ODR_OFF) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  pObj->gyro_is_enabled = 0;

  return ISM330BX_OK;
}

/**
 * @brief  Get the ISM330BX gyroscope sensor sensitivity
 * @param  pObj the device pObj
 * @param  Sensitivity pointer
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_GYRO_GetSensitivity(ISM330BX_Object_t *pObj, float_t *Sensitivity)
{
  int32_t ret = ISM330BX_OK;
  ism330bx_gy_full_scale_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (ism330bx_gy_full_scale_get(&(pObj->Ctx), &full_scale) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  /* Store the sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case ISM330BX_125dps:
      *Sensitivity = ISM330BX_GYRO_SENSITIVITY_FS_125DPS;
      break;

    case ISM330BX_250dps:
      *Sensitivity = ISM330BX_GYRO_SENSITIVITY_FS_250DPS;
      break;

    case ISM330BX_500dps:
      *Sensitivity = ISM330BX_GYRO_SENSITIVITY_FS_500DPS;
      break;

    case ISM330BX_1000dps:
      *Sensitivity = ISM330BX_GYRO_SENSITIVITY_FS_1000DPS;
      break;

    case ISM330BX_2000dps:
      *Sensitivity = ISM330BX_GYRO_SENSITIVITY_FS_2000DPS;
      break;

    case ISM330BX_4000dps:
      *Sensitivity = ISM330BX_GYRO_SENSITIVITY_FS_4000DPS;
      break;

    default:
      ret = ISM330BX_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Get the ISM330BX gyroscope sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr pointer where the output data rate is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_GYRO_GetOutputDataRate(ISM330BX_Object_t *pObj, float_t *Odr)
{
  int32_t ret = ISM330BX_OK;
  ism330bx_gy_data_rate_t odr_low_level;

  /* Get current output data rate. */
  if (ism330bx_gy_data_rate_get(&(pObj->Ctx), &odr_low_level) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  switch (odr_low_level)
  {
    case ISM330BX_GY_ODR_OFF:
      *Odr = 0.0f;
      break;

    case ISM330BX_GY_ODR_AT_7Hz5:
      *Odr = 7.5f;
      break;

    case ISM330BX_GY_ODR_AT_15Hz:
      *Odr = 15.0f;
      break;

    case ISM330BX_GY_ODR_AT_30Hz:
      *Odr = 30.0f;
      break;

    case ISM330BX_GY_ODR_AT_60Hz:
      *Odr = 60.0f;
      break;

    case ISM330BX_GY_ODR_AT_120Hz:
      *Odr = 120.0f;
      break;

    case ISM330BX_GY_ODR_AT_240Hz:
      *Odr = 240.0f;
      break;

    case ISM330BX_GY_ODR_AT_480Hz:
      *Odr = 480.0f;
      break;

    case ISM330BX_GY_ODR_AT_960Hz:
      *Odr = 960.0f;
      break;

    case ISM330BX_GY_ODR_AT_1920Hz:
      *Odr = 1920.0f;
      break;

    case ISM330BX_GY_ODR_AT_3840Hz:
      *Odr = 3840.0f;
      break;

    default:
      ret = ISM330BX_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the ISM330BX gyroscope sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_GYRO_SetOutputDataRate(ISM330BX_Object_t *pObj, float_t Odr)
{
  return ISM330BX_GYRO_SetOutputDataRate_With_Mode(pObj, Odr, ISM330BX_GYRO_HIGH_PERFORMANCE_MODE);
}

/**
 * @brief  Set the ISM330BX gyroscope sensor output data rate with operating mode
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @param  Mode the gyroscope operating mode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_GYRO_SetOutputDataRate_With_Mode(ISM330BX_Object_t *pObj, float_t Odr, ISM330BX_GYRO_Operating_Mode_t Mode)
{
  int32_t ret = ISM330BX_OK;
  float_t odr_local = 0.0f;

  switch (Mode)
  {
    case ISM330BX_GYRO_HIGH_PERFORMANCE_MODE:
      if (ism330bx_gy_mode_set(&(pObj->Ctx), ISM330BX_GY_HIGH_PERFORMANCE_MD) != ISM330BX_OK)
      {
        return ISM330BX_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 3.84kHz */
      odr_local = (Odr <    7.5f) ?    7.5f
    	          : (Odr > 3840.0f) ? 3840.0f
    	          :                       Odr;
      break;

    case ISM330BX_GYRO_LOW_POWER_MODE:
      if (ism330bx_gy_mode_set(&(pObj->Ctx), ISM330BX_GY_LOW_POWER_MD) != ISM330BX_OK)
      {
        return ISM330BX_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 240kHz */
      odr_local = (Odr <   7.5f) ?   7.5f
    	          : (Odr > 240.0f) ? 240.0f
    	          :                     Odr;
      break;

    default:
      ret = ISM330BX_ERROR;
      break;
  }

  if (ret != ISM330BX_OK)
  {
    return ret;
  }

  if (pObj->gyro_is_enabled == 1U)
  {
    return ISM330BX_GYRO_SetOutputDataRate_When_Enabled(pObj, odr_local);
  }
  else
  {
    return ISM330BX_GYRO_SetOutputDataRate_When_Disabled(pObj, odr_local);
  }
}

/**
 * @brief  Get the ISM330BX gyroscope sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale pointer where the full scale is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_GYRO_GetFullScale(ISM330BX_Object_t *pObj, int32_t  *FullScale)
{
  int32_t ret = ISM330BX_OK;
  ism330bx_gy_full_scale_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (ism330bx_gy_full_scale_get(&(pObj->Ctx), &fs_low_level) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  switch (fs_low_level)
  {
    case ISM330BX_125dps:
      *FullScale =  125;
      break;

    case ISM330BX_250dps:
      *FullScale =  250;
      break;

    case ISM330BX_500dps:
      *FullScale =  500;
      break;

    case ISM330BX_1000dps:
      *FullScale = 1000;
      break;

    case ISM330BX_2000dps:
      *FullScale = 2000;
      break;

    case ISM330BX_4000dps:
      *FullScale = 4000;
      break;

    default:
      ret = ISM330BX_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the ISM330BX gyroscope sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale the functional full scale to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_GYRO_SetFullScale(ISM330BX_Object_t *pObj, int32_t FullScale)
{
	ism330bx_gy_full_scale_t new_fs;

  new_fs = (FullScale <= 125)  ? ISM330BX_125dps
         : (FullScale <= 250)  ? ISM330BX_250dps
         : (FullScale <= 500)  ? ISM330BX_500dps
         : (FullScale <= 1000) ? ISM330BX_1000dps
         : (FullScale <= 2000) ? ISM330BX_2000dps
         :                       ISM330BX_4000dps;

  if (ism330bx_gy_full_scale_set(&(pObj->Ctx), new_fs) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  return ISM330BX_OK;
}

/**
 * @brief  Get the ISM330BX gyroscope sensor raw axes
 * @param  pObj the device pObj
 * @param  Value pointer where the raw values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_GYRO_GetAxesRaw(ISM330BX_Object_t *pObj, ISM330BX_AxesRaw_t *Value)
{
  ism330bx_axis3bit16_t data_raw;

  /* Read raw data values. */
  if (ism330bx_angular_rate_raw_get(&(pObj->Ctx), data_raw.i16bit) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return ISM330BX_OK;
}

/**
 * @brief  Get the ISM330BX gyroscope sensor axes
 * @param  pObj the device pObj
 * @param  AngularRate pointer where the values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_GYRO_GetAxes(ISM330BX_Object_t *pObj, ISM330BX_Axes_t *AngularRate)
{
  ism330bx_axis3bit16_t data_raw;
  float_t sensitivity = 0.0f;

  /* Read raw data values. */
  if (ism330bx_angular_rate_raw_get(&(pObj->Ctx), data_raw.i16bit) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  /* Get ISM330BX actual sensitivity. */
  if (ISM330BX_GYRO_GetSensitivity(pObj, &sensitivity) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  /* Calculate the data. */
  AngularRate->x = (int32_t)((float_t)((float_t)data_raw.i16bit[0] * sensitivity));
  AngularRate->y = (int32_t)((float_t)((float_t)data_raw.i16bit[1] * sensitivity));
  AngularRate->z = (int32_t)((float_t)((float_t)data_raw.i16bit[2] * sensitivity));

  return ISM330BX_OK;
}

/**
 * @brief  Get the ISM330BX register value
 * @param  pObj the device pObj
 * @param  Reg address to be read
 * @param  Data pointer where the value is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_Read_Reg(ISM330BX_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (ism330bx_read_reg(&(pObj->Ctx), Reg, Data, 1) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  return ISM330BX_OK;
}

/**
 * @brief  Set the ISM330BX register value
 * @param  pObj the device pObj
 * @param  Reg address to be written
 * @param  Data value to be written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_Write_Reg(ISM330BX_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (ism330bx_write_reg(&(pObj->Ctx), Reg, &Data, 1) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  return ISM330BX_OK;
}

/**
 * @brief  Get the ISM330BX ACC data ready bit value
 * @param  pObj the device pObj
 * @param  Status the status of data ready bit
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_ACC_Get_DRDY_Status(ISM330BX_Object_t *pObj, uint8_t *Status)
{
  ism330bx_all_sources_t val;

  if (ism330bx_all_sources_get(&(pObj->Ctx), &val) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  *Status = val.drdy_xl;
  return ISM330BX_OK;
}

/**
 * @brief  Get the ISM330BX GYRO data ready bit value
 * @param  pObj the device pObj
 * @param  Status the status of data ready bit
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_GYRO_Get_DRDY_Status(ISM330BX_Object_t *pObj, uint8_t *Status)
{
  ism330bx_all_sources_t val;

  if (ism330bx_all_sources_get(&(pObj->Ctx), &val) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  *Status = val.drdy_gy;
  return ISM330BX_OK;
}

/**
  * @brief  Set memory bank
  * @param  pObj the device pObj
  * @param  Val the value of memory bank in reg FUNC_CFG_ACCESS
  *         0 - ISM330BX_MAIN_MEM_BANK, 1 - ISM330BX_EMBED_FUNC_MEM_BANK
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM330BX_Set_Mem_Bank(ISM330BX_Object_t *pObj, uint8_t Val)
{
  int32_t ret = ISM330BX_OK;
  ism330bx_mem_bank_t reg;

  reg = (Val == 1U) ? ISM330BX_EMBED_FUNC_MEM_BANK
        :               ISM330BX_MAIN_MEM_BANK;

  if (ism330bx_mem_bank_set(&(pObj->Ctx), reg) != ISM330BX_OK)
  {
    ret = ISM330BX_ERROR;
  }

  return ret;
}

/**
 * @}
 */

/** @defgroup ISM330BX_Private_Functions ISM330BX Private Functions
 * @{
 */

/**
 * @brief  Set the ISM330BX accelerometer sensor output data rate when enabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t ISM330BX_ACC_SetOutputDataRate_When_Enabled(ISM330BX_Object_t *pObj, float_t Odr)
{
  ism330bx_xl_data_rate_t new_odr;

  new_odr = (Odr <=    1.875f) ? ISM330BX_XL_ODR_AT_1Hz875
          : (Odr <=    7.5f  ) ? ISM330BX_XL_ODR_AT_7Hz5
          : (Odr <=   15.0f  ) ? ISM330BX_XL_ODR_AT_15Hz
          : (Odr <=   30.0f  ) ? ISM330BX_XL_ODR_AT_30Hz
          : (Odr <=   60.0f  ) ? ISM330BX_XL_ODR_AT_60Hz
          : (Odr <=  120.0f  ) ? ISM330BX_XL_ODR_AT_120Hz
          : (Odr <=  240.0f  ) ? ISM330BX_XL_ODR_AT_240Hz
          : (Odr <=  480.0f  ) ? ISM330BX_XL_ODR_AT_480Hz
          : (Odr <=  960.0f  ) ? ISM330BX_XL_ODR_AT_960Hz
          : (Odr <= 1920.0f  ) ? ISM330BX_XL_ODR_AT_1920Hz
          :                      ISM330BX_XL_ODR_AT_3840Hz;

  /* Output data rate selection. */
  if (ism330bx_xl_data_rate_set(&(pObj->Ctx), new_odr) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  return ISM330BX_OK;
}

/**
 * @brief  Set the ISM330BX accelerometer sensor output data rate when disabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t ISM330BX_ACC_SetOutputDataRate_When_Disabled(ISM330BX_Object_t *pObj, float_t Odr)
{
  pObj->acc_odr = (Odr <=    1.875f) ? ISM330BX_XL_ODR_AT_1Hz875
  	            : (Odr <=    7.5f  ) ? ISM330BX_XL_ODR_AT_7Hz5
	            : (Odr <=   15.0f  ) ? ISM330BX_XL_ODR_AT_15Hz
	            : (Odr <=   30.0f  ) ? ISM330BX_XL_ODR_AT_30Hz
	            : (Odr <=   60.0f  ) ? ISM330BX_XL_ODR_AT_60Hz
	            : (Odr <=  120.0f  ) ? ISM330BX_XL_ODR_AT_120Hz
	            : (Odr <=  240.0f  ) ? ISM330BX_XL_ODR_AT_240Hz
	            : (Odr <=  480.0f  ) ? ISM330BX_XL_ODR_AT_480Hz
              : (Odr <=  960.0f  ) ? ISM330BX_XL_ODR_AT_960Hz
	            : (Odr <= 1920.0f  ) ? ISM330BX_XL_ODR_AT_1920Hz
              :                      ISM330BX_XL_ODR_AT_3840Hz;

  return ISM330BX_OK;
}

/**
 * @brief  Set the ISM330BX gyroscope sensor output data rate when enabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t ISM330BX_GYRO_SetOutputDataRate_When_Enabled(ISM330BX_Object_t *pObj, float_t Odr)
{
  ism330bx_gy_data_rate_t new_odr;

  new_odr = (Odr <=    7.5f) ? ISM330BX_GY_ODR_AT_7Hz5
          : (Odr <=   15.0f) ? ISM330BX_GY_ODR_AT_15Hz
          : (Odr <=   30.0f) ? ISM330BX_GY_ODR_AT_30Hz
          : (Odr <=   60.0f) ? ISM330BX_GY_ODR_AT_60Hz
          : (Odr <=  120.0f) ? ISM330BX_GY_ODR_AT_120Hz
          : (Odr <=  240.0f) ? ISM330BX_GY_ODR_AT_240Hz
          : (Odr <=  480.0f) ? ISM330BX_GY_ODR_AT_480Hz
          : (Odr <=  960.0f) ? ISM330BX_GY_ODR_AT_960Hz
          : (Odr <= 1920.0f) ? ISM330BX_GY_ODR_AT_1920Hz
	        :                    ISM330BX_GY_ODR_AT_3840Hz;

  /* Output data rate selection. */
  if (ism330bx_gy_data_rate_set(&(pObj->Ctx), new_odr) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  return ISM330BX_OK;
}

/**
 * @brief  Set the ISM330BX gyroscope sensor output data rate when disabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t ISM330BX_GYRO_SetOutputDataRate_When_Disabled(ISM330BX_Object_t *pObj, float_t Odr)
{
  pObj->gyro_odr = (Odr <=    7.5f) ? ISM330BX_GY_ODR_AT_7Hz5
	             : (Odr <=   15.0f) ? ISM330BX_GY_ODR_AT_15Hz
	             : (Odr <=   30.0f) ? ISM330BX_GY_ODR_AT_30Hz
	             : (Odr <=   60.0f) ? ISM330BX_GY_ODR_AT_60Hz
	             : (Odr <=  120.0f) ? ISM330BX_GY_ODR_AT_120Hz
	             : (Odr <=  240.0f) ? ISM330BX_GY_ODR_AT_240Hz
	             : (Odr <=  480.0f) ? ISM330BX_GY_ODR_AT_480Hz
	             : (Odr <=  960.0f) ? ISM330BX_GY_ODR_AT_960Hz
	             : (Odr <= 1920.0f) ? ISM330BX_GY_ODR_AT_1920Hz
	             :                    ISM330BX_GY_ODR_AT_3840Hz;

  return ISM330BX_OK;
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
  ISM330BX_Object_t *pObj = (ISM330BX_Object_t *)Handle;

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
  ISM330BX_Object_t *pObj = (ISM330BX_Object_t *)Handle;

  return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
}


/**
 * @brief  Set the ISM330BX accelerometer power mode
 * @param  pObj the device pObj
 * @param  PowerMode Value of the powerMode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_ACC_Set_Power_Mode(ISM330BX_Object_t *pObj, uint8_t PowerMode)
{
  if(ism330bx_xl_mode_set(&(pObj->Ctx), (ism330bx_xl_mode_t)PowerMode) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  return ISM330BX_OK;
}


/**
 * @brief  Set the ISM330BX gyroscope power mode
 * @param  pObj the device pObj
 * @param  PowerMode Value of the powerMode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_GYRO_Set_Power_Mode(ISM330BX_Object_t *pObj, uint8_t PowerMode)
{
  if (ism330bx_gy_mode_set(&(pObj->Ctx), (ism330bx_gy_mode_t)PowerMode) != ISM330BX_OK)
  {
    return ISM330BX_ERROR;
  }

  return ISM330BX_OK;
}


/**
 * @brief  Set the ISM330BX accelerometer filter mode
 * @param  pObj the device pObj
 * @param  LowHighPassFlag 0/1 for setting low-pass/high-pass filter mode
 * @param  FilterMode Value of the filter Mode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_ACC_Set_Filter_Mode(ISM330BX_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode)
{
  if (LowHighPassFlag == 0U)
  {
    /*Set accelerometer low_pass filter-mode*/

    /*Set to 1 LPF2 bit (CTRL8_XL)*/
    if (ism330bx_filt_xl_lp2_set(&(pObj->Ctx), 1) != ISM330BX_OK)
    {
      return ISM330BX_ERROR;
    }
    if (ism330bx_filt_xl_lp2_bandwidth_set(&(pObj->Ctx), (ism330bx_filt_xl_lp2_bandwidth_t)FilterMode) != ISM330BX_OK)
    {
      return ISM330BX_ERROR;
    }
  }
  else
  {
    if (ism330bx_filt_xl_lp2_set(&(pObj->Ctx), 0) != ISM330BX_OK)
    {
      return ISM330BX_ERROR;
    }
    /*Set accelerometer high_pass filter-mode*/
    if (ism330bx_filt_xl_lp2_bandwidth_set(&(pObj->Ctx), (ism330bx_filt_xl_lp2_bandwidth_t)FilterMode) != ISM330BX_OK)
    {
      return ISM330BX_ERROR;
    }
  }
  return ISM330BX_OK;
}


/**
 * @brief  Set the ISM330BX gyroscope filter mode
 * @param  pObj the device pObj
 * @param  LowHighPassFlag 0/1 for setting low-pass/high-pass filter mode
 * @param  FilterMode Value of the filter Mode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330BX_GYRO_Set_Filter_Mode(ISM330BX_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode)
{
  if (LowHighPassFlag == 0U)
  {
    /*Set gyroscope low_pass 1 filter-mode*/
    /* Enable low-pass filter */
    if (ism330bx_filt_gy_lp1_set(&(pObj->Ctx), 1) != ISM330BX_OK)
    {
      return ISM330BX_ERROR;
    }
    if (ism330bx_filt_gy_lp1_bandwidth_set(&(pObj->Ctx), (ism330bx_filt_gy_lp1_bandwidth_t)FilterMode) != ISM330BX_OK)
    {
      return ISM330BX_ERROR;
    }
  }
  else
  {
    /*Set gyroscope high_pass filter-mode*/
    /* Enable high-pass filter */
    if (ism330bx_filt_gy_lp1_set(&(pObj->Ctx), 0) != ISM330BX_OK)
    {
      return ISM330BX_ERROR;
    }
    if (ism330bx_filt_gy_lp1_bandwidth_set(&(pObj->Ctx), (ism330bx_filt_gy_lp1_bandwidth_t)FilterMode) != ISM330BX_OK)
    {
      return ISM330BX_ERROR;
    }
  }
  return ISM330BX_OK;
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
