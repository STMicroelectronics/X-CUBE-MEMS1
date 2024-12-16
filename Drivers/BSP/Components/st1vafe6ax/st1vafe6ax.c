/**
 ******************************************************************************
 * @file    st1vafe6ax.c
 * @author  MEMS Software Solutions Team
 * @brief   ST1VAFE6AX driver file
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
#include "st1vafe6ax.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @defgroup ST1VAFE6AX ST1VAFE6AX
 * @{
 */

/** @defgroup ST1VAFE6AX_Exported_Variables ST1VAFE6AX Exported Variables
 * @{
 */

ST1VAFE6AX_CommonDrv_t ST1VAFE6AX_COMMON_Driver =
{
  ST1VAFE6AX_Init,
  ST1VAFE6AX_DeInit,
  ST1VAFE6AX_ReadID,
  ST1VAFE6AX_GetCapabilities,
};

ST1VAFE6AX_ACC_Drv_t ST1VAFE6AX_ACC_Driver =
{
  ST1VAFE6AX_ACC_Enable,
  ST1VAFE6AX_ACC_Disable,
  ST1VAFE6AX_ACC_GetSensitivity,
  ST1VAFE6AX_ACC_GetOutputDataRate,
  ST1VAFE6AX_ACC_SetOutputDataRate,
  ST1VAFE6AX_ACC_GetFullScale,
  ST1VAFE6AX_ACC_SetFullScale,
  ST1VAFE6AX_ACC_GetAxes,
  ST1VAFE6AX_ACC_GetAxesRaw,
};

ST1VAFE6AX_GYRO_Drv_t ST1VAFE6AX_GYRO_Driver =
{
  ST1VAFE6AX_GYRO_Enable,
  ST1VAFE6AX_GYRO_Disable,
  ST1VAFE6AX_GYRO_GetSensitivity,
  ST1VAFE6AX_GYRO_GetOutputDataRate,
  ST1VAFE6AX_GYRO_SetOutputDataRate,
  ST1VAFE6AX_GYRO_GetFullScale,
  ST1VAFE6AX_GYRO_SetFullScale,
  ST1VAFE6AX_GYRO_GetAxes,
  ST1VAFE6AX_GYRO_GetAxesRaw,
};

/**
 * @}
 */

/** @defgroup ST1VAFE6AX_Private_Function_Prototypes ST1VAFE6AX Private Function Prototypes
 * @{
 */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t ST1VAFE6AX_ACC_SetOutputDataRate_When_Enabled(ST1VAFE6AX_Object_t *pObj, float_t Odr);
static int32_t ST1VAFE6AX_ACC_SetOutputDataRate_When_Disabled(ST1VAFE6AX_Object_t *pObj, float_t Odr);
static int32_t ST1VAFE6AX_GYRO_SetOutputDataRate_When_Enabled(ST1VAFE6AX_Object_t *pObj, float_t Odr);
static int32_t ST1VAFE6AX_GYRO_SetOutputDataRate_When_Disabled(ST1VAFE6AX_Object_t *pObj, float_t Odr);

/**
 * @}
 */

/** @defgroup ST1VAFE6AX_Exported_Functions ST1VAFE6AX Exported Functions
 * @{
 */

/**
 * @brief  Register Component Bus IO operations
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_RegisterBusIO(ST1VAFE6AX_Object_t *pObj, ST1VAFE6AX_IO_t *pIO)
{
  int32_t ret = ST1VAFE6AX_OK;

  if (pObj == NULL)
  {
    ret = ST1VAFE6AX_ERROR;
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
      ret = ST1VAFE6AX_ERROR;
    }
    else if (pObj->IO.Init() != ST1VAFE6AX_OK)
    {
      ret = ST1VAFE6AX_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == ST1VAFE6AX_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x04;

          if (ST1VAFE6AX_Write_Reg(pObj, ST1VAFE6AX_CTRL3, data) != ST1VAFE6AX_OK)
          {
            ret = ST1VAFE6AX_ERROR;
          }

          if (ST1VAFE6AX_Write_Reg(pObj, ST1VAFE6AX_IF_CFG, data) != ST1VAFE6AX_OK)
          {
            ret = ST1VAFE6AX_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
 * @brief  Initialize the ST1VAFE6AX sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_Init(ST1VAFE6AX_Object_t *pObj)
{
  /* Enable register address automatically incremented during a multiple byte
  access with a serial interface. */
  if (st1vafe6ax_auto_increment_set(&(pObj->Ctx), PROPERTY_ENABLE) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  /* Set main memory bank */
  if (ST1VAFE6AX_Set_Mem_Bank(pObj, (uint8_t)ST1VAFE6AX_MAIN_MEM_BANK) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  /* Enable BDU */
  if (st1vafe6ax_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  /* FIFO mode selection */
  if (st1vafe6ax_fifo_mode_set(&(pObj->Ctx), ST1VAFE6AX_BYPASS_MODE) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = ST1VAFE6AX_XL_ODR_AT_120Hz;

  /* Output data rate selection - power down. */
  if (st1vafe6ax_xl_data_rate_set(&(pObj->Ctx), ST1VAFE6AX_XL_ODR_OFF) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  /* Full scale selection. */
  if (st1vafe6ax_xl_full_scale_set(&(pObj->Ctx), ST1VAFE6AX_2g) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  /* Select default output data rate. */
  pObj->gyro_odr = ST1VAFE6AX_GY_ODR_AT_120Hz;

  /* Output data rate selection - power down. */
  if (st1vafe6ax_gy_data_rate_set(&(pObj->Ctx), ST1VAFE6AX_GY_ODR_OFF) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  /* Full scale selection. */
  if (st1vafe6ax_gy_full_scale_set(&(pObj->Ctx), ST1VAFE6AX_2000dps) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  /* Enable bio functionality */
  st1vafe6ax_ah_bio_mode_t mode;

  mode.ah_bio1_en = 1;
  mode.ah_bio2_en = 0;
  mode.swaps       = 0;

  if (st1vafe6ax_ah_bio_mode_set(&(pObj->Ctx), mode) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  pObj->is_initialized = 1;

  return ST1VAFE6AX_OK;
}

/**
 * @brief  Deinitialize the ST1VAFE6AX sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_DeInit(ST1VAFE6AX_Object_t *pObj)
{
  /* Disable the component */
  if (ST1VAFE6AX_ACC_Disable(pObj) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  if (ST1VAFE6AX_GYRO_Disable(pObj) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  /* Reset output data rate. */
  pObj->acc_odr = ST1VAFE6AX_XL_ODR_OFF;
  pObj->gyro_odr = ST1VAFE6AX_GY_ODR_OFF;

  /* Disable bio functionality */
  st1vafe6ax_ah_bio_mode_t mode;

  mode.ah_bio1_en = 0;
  mode.ah_bio2_en = 0;
  mode.swaps       = 0;

  if (st1vafe6ax_ah_bio_mode_set(&(pObj->Ctx), mode) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  pObj->is_initialized = 0;

  return ST1VAFE6AX_OK;
}

/**
 * @brief  Read component ID
 * @param  pObj the device pObj
 * @param  Id the WHO_AM_I value
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_ReadID(ST1VAFE6AX_Object_t *pObj, uint8_t *Id)
{
  if (st1vafe6ax_device_id_get(&(pObj->Ctx), Id) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  return ST1VAFE6AX_OK;
}

/**
 * @brief  Get ST1VAFE6AX sensor capabilities
 * @param  pObj Component object pointer
 * @param  Capabilities pointer to ST1VAFE6AX sensor capabilities
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_GetCapabilities(ST1VAFE6AX_Object_t *pObj, ST1VAFE6AX_Capabilities_t *Capabilities)
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
  return ST1VAFE6AX_OK;
}

/**
 * @brief  Enable the ST1VAFE6AX accelerometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_ACC_Enable(ST1VAFE6AX_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return ST1VAFE6AX_OK;
  }

  /* Output data rate selection. */
  if (st1vafe6ax_xl_data_rate_set(&(pObj->Ctx), pObj->acc_odr) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return ST1VAFE6AX_OK;
}

/**
 * @brief  Disable the ST1VAFE6AX accelerometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_ACC_Disable(ST1VAFE6AX_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return ST1VAFE6AX_OK;
  }

  /* Get current output data rate. */
  if (st1vafe6ax_xl_data_rate_get(&(pObj->Ctx), &pObj->acc_odr) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  /* Output data rate selection - power down. */
  if (st1vafe6ax_xl_data_rate_set(&(pObj->Ctx), ST1VAFE6AX_XL_ODR_OFF) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return ST1VAFE6AX_OK;
}

/**
 * @brief  Get the ST1VAFE6AX accelerometer sensor sensitivity
 * @param  pObj the device pObj
 * @param  Sensitivity pointer
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_ACC_GetSensitivity(ST1VAFE6AX_Object_t *pObj, float_t *Sensitivity)
{
  int32_t ret = ST1VAFE6AX_OK;
  st1vafe6ax_xl_full_scale_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (st1vafe6ax_xl_full_scale_get(&(pObj->Ctx), &full_scale) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  /* Store the Sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case ST1VAFE6AX_2g:
      *Sensitivity = ST1VAFE6AX_ACC_SENSITIVITY_FS_2G;
      break;

    case ST1VAFE6AX_4g:
      *Sensitivity = ST1VAFE6AX_ACC_SENSITIVITY_FS_4G;
      break;

    case ST1VAFE6AX_8g:
      *Sensitivity = ST1VAFE6AX_ACC_SENSITIVITY_FS_8G;
      break;

    default:
      ret = ST1VAFE6AX_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Get the ST1VAFE6AX accelerometer sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr pointer where the output data rate is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_ACC_GetOutputDataRate(ST1VAFE6AX_Object_t *pObj, float_t *Odr)
{
  int32_t ret = ST1VAFE6AX_OK;
  st1vafe6ax_xl_data_rate_t odr_low_level;

  /* Get current output data rate. */
  if (st1vafe6ax_xl_data_rate_get(&(pObj->Ctx), &odr_low_level) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  switch (odr_low_level)
  {
    case ST1VAFE6AX_XL_ODR_OFF:
      *Odr = 0.0f;
      break;

    case ST1VAFE6AX_XL_ODR_AT_1Hz875:
      *Odr = 1.875f;
      break;

    case ST1VAFE6AX_XL_ODR_AT_7Hz5:
      *Odr = 7.5f;
      break;

    case ST1VAFE6AX_XL_ODR_AT_15Hz:
      *Odr = 15.0f;
      break;

    case ST1VAFE6AX_XL_ODR_AT_30Hz:
      *Odr = 30.0f;
      break;

    case ST1VAFE6AX_XL_ODR_AT_60Hz:
      *Odr = 60.0f;
      break;

    case ST1VAFE6AX_XL_ODR_AT_120Hz:
      *Odr = 120.0f;
      break;

    case ST1VAFE6AX_XL_ODR_AT_240Hz:
      *Odr = 240.0f;
      break;

    case ST1VAFE6AX_XL_ODR_AT_480Hz:
      *Odr = 480.0f;
      break;

    case ST1VAFE6AX_XL_ODR_AT_960Hz:
      *Odr = 960.0f;
      break;

    case ST1VAFE6AX_XL_ODR_AT_1920Hz:
      *Odr = 1920.0f;
      break;

    case ST1VAFE6AX_XL_ODR_AT_3840Hz:
      *Odr = 3840.0f;
      break;

    default:
      ret = ST1VAFE6AX_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the ST1VAFE6AX accelerometer sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_ACC_SetOutputDataRate(ST1VAFE6AX_Object_t *pObj, float_t Odr)
{
  return ST1VAFE6AX_ACC_SetOutputDataRate_With_Mode(pObj, Odr, ST1VAFE6AX_ACC_HIGH_PERFORMANCE_MODE);
}

/**
 * @brief  Set the ST1VAFE6AX accelerometer sensor output data rate with operating mode
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @param  Mode the accelerometer operating mode
 * @note   This function switches off the gyroscope if Ultra Low Power Mode is set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_ACC_SetOutputDataRate_With_Mode(ST1VAFE6AX_Object_t *pObj, float_t Odr, ST1VAFE6AX_ACC_Operating_Mode_t Mode)
{
  int32_t ret = ST1VAFE6AX_OK;
  float_t odr_local = 0.0f;

  switch (Mode)
  {
    case ST1VAFE6AX_ACC_HIGH_PERFORMANCE_MODE:
      if (st1vafe6ax_xl_mode_set(&(pObj->Ctx), ST1VAFE6AX_XL_HIGH_PERFORMANCE_MD) != ST1VAFE6AX_OK)
      {
        return ST1VAFE6AX_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 7.68kHz */
      odr_local = (Odr <    7.5f) ?    7.5f
    	  : (Odr > 7680.0f) ? 7680.0f
    	  :                       Odr;
      break;

    case ST1VAFE6AX_ACC_LOW_POWER_MODE1:
      if (st1vafe6ax_xl_mode_set(&(pObj->Ctx), ST1VAFE6AX_XL_LOW_POWER_2_AVG_MD) != ST1VAFE6AX_OK)
      {
        return ST1VAFE6AX_ERROR;
      }

      /* Valid ODR: 1.875Hz;  15Hz <= Odr <= 240kHz */
      odr_local = (Odr ==   1.875f) ?    Odr
    	  : (Odr <   15.000f) ?  15.0f
    	  : (Odr >  240.000f) ? 240.0f
    	  :                        Odr;
      break;

    case ST1VAFE6AX_ACC_LOW_POWER_MODE2:
      if (st1vafe6ax_xl_mode_set(&(pObj->Ctx), ST1VAFE6AX_XL_LOW_POWER_4_AVG_MD) != ST1VAFE6AX_OK)
      {
        return ST1VAFE6AX_ERROR;
      }

      /* Valid ODR: 1.875Hz;  15Hz <= Odr <= 240kHz */
      odr_local = (Odr ==   1.875f) ?    Odr
    	  : (Odr <   15.000f) ?  15.0f
    	  : (Odr >  240.000f) ? 240.0f
    	  :                        Odr;
      break;

    case ST1VAFE6AX_ACC_LOW_POWER_MODE3:
      if (st1vafe6ax_xl_mode_set(&(pObj->Ctx), ST1VAFE6AX_XL_LOW_POWER_8_AVG_MD) != ST1VAFE6AX_OK)
      {
        return ST1VAFE6AX_ERROR;
      }

      /* Valid ODR: 1.875Hz;  15Hz <= Odr <= 240kHz */
      odr_local = (Odr ==   1.875f) ?    Odr
    	  : (Odr <   15.000f) ?  15.0f
    	  : (Odr >  240.000f) ? 240.0f
    	  :                        Odr;
      break;

    case ST1VAFE6AX_ACC_HIGH_ACCURACY_MODE:
    default:
      ret = ST1VAFE6AX_ERROR;
      break;
  }

  if (ret != ST1VAFE6AX_OK)
  {
    return ret;
  }

  if (pObj->acc_is_enabled == 1U)
  {
    return ST1VAFE6AX_ACC_SetOutputDataRate_When_Enabled(pObj, odr_local);
  }
  else
  {
    return ST1VAFE6AX_ACC_SetOutputDataRate_When_Disabled(pObj, odr_local);
  }
}

/**
 * @brief  Get the ST1VAFE6AX accelerometer sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale pointer where the full scale is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_ACC_GetFullScale(ST1VAFE6AX_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = ST1VAFE6AX_OK;
  st1vafe6ax_xl_full_scale_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (st1vafe6ax_xl_full_scale_get(&(pObj->Ctx), &fs_low_level) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  switch (fs_low_level)
  {
    case ST1VAFE6AX_2g:
      *FullScale =  2;
      break;

    case ST1VAFE6AX_4g:
      *FullScale =  4;
      break;

    case ST1VAFE6AX_8g:
      *FullScale =  8;
      break;

    default:
      ret = ST1VAFE6AX_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the ST1VAFE6AX accelerometer sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale the functional full scale to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_ACC_SetFullScale(ST1VAFE6AX_Object_t *pObj, int32_t FullScale)
{
  st1vafe6ax_xl_full_scale_t new_fs;

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file statical analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  new_fs = (FullScale <= 2) ? ST1VAFE6AX_2g
         : (FullScale <= 4) ? ST1VAFE6AX_4g
         :                    ST1VAFE6AX_8g;

  if (st1vafe6ax_xl_full_scale_set(&(pObj->Ctx), new_fs) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  return ST1VAFE6AX_OK;
}

/**
 * @brief  Get the ST1VAFE6AX accelerometer sensor raw axes
 * @param  pObj the device pObj
 * @param  Value pointer where the raw values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_ACC_GetAxesRaw(ST1VAFE6AX_Object_t *pObj, ST1VAFE6AX_AxesRaw_t *Value)
{
  st1vafe6ax_axis3bit16_t data_raw;

  /* Read raw data values. */
  if (st1vafe6ax_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return ST1VAFE6AX_OK;
}

/**
 * @brief  Get the ST1VAFE6AX accelerometer sensor axes
 * @param  pObj the device pObj
 * @param  Acceleration pointer where the values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_ACC_GetAxes(ST1VAFE6AX_Object_t *pObj, ST1VAFE6AX_Axes_t *Acceleration)
{
  st1vafe6ax_axis3bit16_t data_raw;
  float_t sensitivity = 0.0f;

  /* Read raw data values. */
  if (st1vafe6ax_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  /* Get ST1VAFE6AX actual sensitivity. */
  if (ST1VAFE6AX_ACC_GetSensitivity(pObj, &sensitivity) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  /* Calculate the data. */
  Acceleration->x = (int32_t)((float_t)((float_t)data_raw.i16bit[0] * sensitivity));
  Acceleration->y = (int32_t)((float_t)((float_t)data_raw.i16bit[1] * sensitivity));
  Acceleration->z = (int32_t)((float_t)((float_t)data_raw.i16bit[2] * sensitivity));

  return ST1VAFE6AX_OK;
}

/**
 * @brief  Enable the ST1VAFE6AX gyroscope sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_GYRO_Enable(ST1VAFE6AX_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->gyro_is_enabled == 1U)
  {
    return ST1VAFE6AX_OK;
  }

  /* Output data rate selection. */
  if (st1vafe6ax_gy_data_rate_set(&(pObj->Ctx), pObj->gyro_odr) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  pObj->gyro_is_enabled = 1;

  return ST1VAFE6AX_OK;
}

/**
 * @brief  Disable the ST1VAFE6AX gyroscope sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_GYRO_Disable(ST1VAFE6AX_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->gyro_is_enabled == 0U)
  {
    return ST1VAFE6AX_OK;
  }

  /* Get current output data rate. */
  if (st1vafe6ax_gy_data_rate_get(&(pObj->Ctx), &pObj->gyro_odr) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  /* Output data rate selection - power down. */
  if (st1vafe6ax_gy_data_rate_set(&(pObj->Ctx), ST1VAFE6AX_GY_ODR_OFF) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  pObj->gyro_is_enabled = 0;

  return ST1VAFE6AX_OK;
}

/**
 * @brief  Get the ST1VAFE6AX gyroscope sensor sensitivity
 * @param  pObj the device pObj
 * @param  Sensitivity pointer
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_GYRO_GetSensitivity(ST1VAFE6AX_Object_t *pObj, float_t *Sensitivity)
{
  int32_t ret = ST1VAFE6AX_OK;
  st1vafe6ax_gy_full_scale_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (st1vafe6ax_gy_full_scale_get(&(pObj->Ctx), &full_scale) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  /* Store the sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case ST1VAFE6AX_125dps:
      *Sensitivity = ST1VAFE6AX_GYRO_SENSITIVITY_FS_125DPS;
      break;

    case ST1VAFE6AX_250dps:
      *Sensitivity = ST1VAFE6AX_GYRO_SENSITIVITY_FS_250DPS;
      break;

    case ST1VAFE6AX_500dps:
      *Sensitivity = ST1VAFE6AX_GYRO_SENSITIVITY_FS_500DPS;
      break;

    case ST1VAFE6AX_1000dps:
      *Sensitivity = ST1VAFE6AX_GYRO_SENSITIVITY_FS_1000DPS;
      break;

    case ST1VAFE6AX_2000dps:
      *Sensitivity = ST1VAFE6AX_GYRO_SENSITIVITY_FS_2000DPS;
      break;

    case ST1VAFE6AX_4000dps:
      *Sensitivity = ST1VAFE6AX_GYRO_SENSITIVITY_FS_4000DPS;
      break;

    default:
      ret = ST1VAFE6AX_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Get the ST1VAFE6AX gyroscope sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr pointer where the output data rate is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_GYRO_GetOutputDataRate(ST1VAFE6AX_Object_t *pObj, float_t *Odr)
{
  int32_t ret = ST1VAFE6AX_OK;
  st1vafe6ax_gy_data_rate_t odr_low_level;

  /* Get current output data rate. */
  if (st1vafe6ax_gy_data_rate_get(&(pObj->Ctx), &odr_low_level) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  switch (odr_low_level)
  {
    case ST1VAFE6AX_GY_ODR_OFF:
      *Odr = 0.0f;
      break;

    case ST1VAFE6AX_GY_ODR_AT_7Hz5:
      *Odr = 7.5f;
      break;

    case ST1VAFE6AX_GY_ODR_AT_15Hz:
      *Odr = 15.0f;
      break;

    case ST1VAFE6AX_GY_ODR_AT_30Hz:
      *Odr = 30.0f;
      break;

    case ST1VAFE6AX_GY_ODR_AT_60Hz:
      *Odr = 60.0f;
      break;

    case ST1VAFE6AX_GY_ODR_AT_120Hz:
      *Odr = 120.0f;
      break;

    case ST1VAFE6AX_GY_ODR_AT_240Hz:
      *Odr = 240.0f;
      break;

    case ST1VAFE6AX_GY_ODR_AT_480Hz:
      *Odr = 480.0f;
      break;

    case ST1VAFE6AX_GY_ODR_AT_960Hz:
      *Odr = 960.0f;
      break;

    case ST1VAFE6AX_GY_ODR_AT_1920Hz:
      *Odr = 1920.0f;
      break;

    case ST1VAFE6AX_GY_ODR_AT_3840Hz:
      *Odr = 3840.0f;
      break;

    default:
      ret = ST1VAFE6AX_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the ST1VAFE6AX gyroscope sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_GYRO_SetOutputDataRate(ST1VAFE6AX_Object_t *pObj, float_t Odr)
{
  return ST1VAFE6AX_GYRO_SetOutputDataRate_With_Mode(pObj, Odr, ST1VAFE6AX_GYRO_HIGH_PERFORMANCE_MODE);
}

/**
 * @brief  Set the ST1VAFE6AX gyroscope sensor output data rate with operating mode
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @param  Mode the gyroscope operating mode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_GYRO_SetOutputDataRate_With_Mode(ST1VAFE6AX_Object_t *pObj, float_t Odr, ST1VAFE6AX_GYRO_Operating_Mode_t Mode)
{
  int32_t ret = ST1VAFE6AX_OK;
  float_t odr_local = 0.0f;

  switch (Mode)
  {
    case ST1VAFE6AX_GYRO_HIGH_PERFORMANCE_MODE:
      if (st1vafe6ax_gy_mode_set(&(pObj->Ctx), ST1VAFE6AX_GY_HIGH_PERFORMANCE_MD) != ST1VAFE6AX_OK)
      {
        return ST1VAFE6AX_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 7.68kHz */
      odr_local = (Odr <    7.5f) ?    7.5f
    	  : (Odr > 7680.0f) ? 7680.0f
    	  :                       Odr;
      break;

    case ST1VAFE6AX_GYRO_LOW_POWER_MODE:
      if (st1vafe6ax_gy_mode_set(&(pObj->Ctx), ST1VAFE6AX_GY_LOW_POWER_MD) != ST1VAFE6AX_OK)
      {
        return ST1VAFE6AX_ERROR;
      }

      /* Valid ODR: 7.5Hz <= Odr <= 240kHz */
      odr_local = (Odr <   7.5f) ?   7.5f
    	  : (Odr > 240.0f) ? 240.0f
    	  :                     Odr;
      break;

    case ST1VAFE6AX_GYRO_HIGH_ACCURACY_MODE:
    case ST1VAFE6AX_GYRO_SLEEP_MODE:
    default:
      ret = ST1VAFE6AX_ERROR;
      break;
  }

  if (ret != ST1VAFE6AX_OK)
  {
    return ret;
  }

  if (pObj->gyro_is_enabled == 1U)
  {
    return ST1VAFE6AX_GYRO_SetOutputDataRate_When_Enabled(pObj, odr_local);
  }
  else
  {
    return ST1VAFE6AX_GYRO_SetOutputDataRate_When_Disabled(pObj, odr_local);
  }
}

/**
 * @brief  Get the ST1VAFE6AX gyroscope sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale pointer where the full scale is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_GYRO_GetFullScale(ST1VAFE6AX_Object_t *pObj, int32_t  *FullScale)
{
  int32_t ret = ST1VAFE6AX_OK;
  st1vafe6ax_gy_full_scale_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (st1vafe6ax_gy_full_scale_get(&(pObj->Ctx), &fs_low_level) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  switch (fs_low_level)
  {
    case ST1VAFE6AX_125dps:
      *FullScale =  125;
      break;

    case ST1VAFE6AX_250dps:
      *FullScale =  250;
      break;

    case ST1VAFE6AX_500dps:
      *FullScale =  500;
      break;

    case ST1VAFE6AX_1000dps:
      *FullScale = 1000;
      break;

    case ST1VAFE6AX_2000dps:
      *FullScale = 2000;
      break;

    case ST1VAFE6AX_4000dps:
      *FullScale = 4000;
      break;

    default:
      ret = ST1VAFE6AX_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the ST1VAFE6AX gyroscope sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale the functional full scale to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_GYRO_SetFullScale(ST1VAFE6AX_Object_t *pObj, int32_t FullScale)
{
	st1vafe6ax_gy_full_scale_t new_fs;

  new_fs = (FullScale <= 125)  ? ST1VAFE6AX_125dps
         : (FullScale <= 250)  ? ST1VAFE6AX_250dps
         : (FullScale <= 500)  ? ST1VAFE6AX_500dps
         : (FullScale <= 1000) ? ST1VAFE6AX_1000dps
         : (FullScale <= 2000) ? ST1VAFE6AX_2000dps
         :                       ST1VAFE6AX_4000dps;

  if (st1vafe6ax_gy_full_scale_set(&(pObj->Ctx), new_fs) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  return ST1VAFE6AX_OK;
}

/**
 * @brief  Get the ST1VAFE6AX gyroscope sensor raw axes
 * @param  pObj the device pObj
 * @param  Value pointer where the raw values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_GYRO_GetAxesRaw(ST1VAFE6AX_Object_t *pObj, ST1VAFE6AX_AxesRaw_t *Value)
{
  st1vafe6ax_axis3bit16_t data_raw;

  /* Read raw data values. */
  if (st1vafe6ax_angular_rate_raw_get(&(pObj->Ctx), data_raw.i16bit) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return ST1VAFE6AX_OK;
}

/**
 * @brief  Get the ST1VAFE6AX gyroscope sensor axes
 * @param  pObj the device pObj
 * @param  AngularRate pointer where the values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_GYRO_GetAxes(ST1VAFE6AX_Object_t *pObj, ST1VAFE6AX_Axes_t *AngularRate)
{
  st1vafe6ax_axis3bit16_t data_raw;
  float_t sensitivity = 0.0f;

  /* Read raw data values. */
  if (st1vafe6ax_angular_rate_raw_get(&(pObj->Ctx), data_raw.i16bit) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  /* Get ST1VAFE6AX actual sensitivity. */
  if (ST1VAFE6AX_GYRO_GetSensitivity(pObj, &sensitivity) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  /* Calculate the data. */
  AngularRate->x = (int32_t)((float_t)((float_t)data_raw.i16bit[0] * sensitivity));
  AngularRate->y = (int32_t)((float_t)((float_t)data_raw.i16bit[1] * sensitivity));
  AngularRate->z = (int32_t)((float_t)((float_t)data_raw.i16bit[2] * sensitivity));

  return ST1VAFE6AX_OK;
}

/**
 * @brief  Get the ST1VAFE6AX register value
 * @param  pObj the device pObj
 * @param  Reg address to be read
 * @param  Data pointer where the value is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_Read_Reg(ST1VAFE6AX_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (st1vafe6ax_read_reg(&(pObj->Ctx), Reg, Data, 1) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  return ST1VAFE6AX_OK;
}

/**
 * @brief  Set the ST1VAFE6AX register value
 * @param  pObj the device pObj
 * @param  Reg address to be written
 * @param  Data value to be written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_Write_Reg(ST1VAFE6AX_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (st1vafe6ax_write_reg(&(pObj->Ctx), Reg, &Data, 1) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  return ST1VAFE6AX_OK;
}

/**
 * @brief  Get the ST1VAFE6AX ACC data ready bit value
 * @param  pObj the device pObj
 * @param  Status the status of data ready bit
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_ACC_Get_DRDY_Status(ST1VAFE6AX_Object_t *pObj, uint8_t *Status)
{
  st1vafe6ax_all_sources_t val;

  if (st1vafe6ax_all_sources_get(&(pObj->Ctx), &val) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  *Status = val.drdy_xl;
  return ST1VAFE6AX_OK;
}

/**
 * @brief  Get the ST1VAFE6AX GYRO data ready bit value
 * @param  pObj the device pObj
 * @param  Status the status of data ready bit
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_GYRO_Get_DRDY_Status(ST1VAFE6AX_Object_t *pObj, uint8_t *Status)
{
  st1vafe6ax_all_sources_t val;

  if (st1vafe6ax_all_sources_get(&(pObj->Ctx), &val) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  *Status = val.drdy_gy;
  return ST1VAFE6AX_OK;
}

/**
  * @brief  Set memory bank
  * @param  pObj the device pObj
  * @param  Val the value of memory bank in reg FUNC_CFG_ACCESS
  *         0 - ST1VAFE6AX_MAIN_MEM_BANK, 1 - ST1VAFE6AX_EMBED_FUNC_MEM_BANK
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE6AX_Set_Mem_Bank(ST1VAFE6AX_Object_t *pObj, uint8_t Val)
{
  int32_t ret = ST1VAFE6AX_OK;
  st1vafe6ax_mem_bank_t reg;

  reg = (Val == 1U) ? ST1VAFE6AX_EMBED_FUNC_MEM_BANK
        :               ST1VAFE6AX_MAIN_MEM_BANK;

  if (st1vafe6ax_mem_bank_set(&(pObj->Ctx), reg) != ST1VAFE6AX_OK)
  {
    ret = ST1VAFE6AX_ERROR;
  }

  return ret;
}

/**
 * @}
 */

/** @defgroup ST1VAFE6AX_Private_Functions ST1VAFE6AX Private Functions
 * @{
 */

/**
 * @brief  Set the ST1VAFE6AX accelerometer sensor output data rate when enabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t ST1VAFE6AX_ACC_SetOutputDataRate_When_Enabled(ST1VAFE6AX_Object_t *pObj, float_t Odr)
{
  st1vafe6ax_xl_data_rate_t new_odr;

  new_odr = (Odr <=    1.875f) ? ST1VAFE6AX_XL_ODR_AT_1Hz875
          : (Odr <=    7.5f  ) ? ST1VAFE6AX_XL_ODR_AT_7Hz5
          : (Odr <=   15.0f  ) ? ST1VAFE6AX_XL_ODR_AT_15Hz
          : (Odr <=   30.0f  ) ? ST1VAFE6AX_XL_ODR_AT_30Hz
          : (Odr <=   60.0f  ) ? ST1VAFE6AX_XL_ODR_AT_60Hz
          : (Odr <=  120.0f  ) ? ST1VAFE6AX_XL_ODR_AT_120Hz
          : (Odr <=  240.0f  ) ? ST1VAFE6AX_XL_ODR_AT_240Hz
          : (Odr <=  480.0f  ) ? ST1VAFE6AX_XL_ODR_AT_480Hz
          : (Odr <=  960.0f  ) ? ST1VAFE6AX_XL_ODR_AT_960Hz
          : (Odr <= 1920.0f  ) ? ST1VAFE6AX_XL_ODR_AT_1920Hz
          :                      ST1VAFE6AX_XL_ODR_AT_3840Hz;

  /* Output data rate selection. */
  if (st1vafe6ax_xl_data_rate_set(&(pObj->Ctx), new_odr) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  return ST1VAFE6AX_OK;
}

/**
 * @brief  Set the ST1VAFE6AX accelerometer sensor output data rate when disabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t ST1VAFE6AX_ACC_SetOutputDataRate_When_Disabled(ST1VAFE6AX_Object_t *pObj, float_t Odr)
{
  pObj->acc_odr = (Odr <=    1.875f) ? ST1VAFE6AX_XL_ODR_AT_1Hz875
  	            : (Odr <=    7.5f  ) ? ST1VAFE6AX_XL_ODR_AT_7Hz5
	            : (Odr <=   15.0f  ) ? ST1VAFE6AX_XL_ODR_AT_15Hz
	            : (Odr <=   30.0f  ) ? ST1VAFE6AX_XL_ODR_AT_30Hz
	            : (Odr <=   60.0f  ) ? ST1VAFE6AX_XL_ODR_AT_60Hz
	            : (Odr <=  120.0f  ) ? ST1VAFE6AX_XL_ODR_AT_120Hz
	            : (Odr <=  240.0f  ) ? ST1VAFE6AX_XL_ODR_AT_240Hz
	            : (Odr <=  480.0f  ) ? ST1VAFE6AX_XL_ODR_AT_480Hz
  	            : (Odr <=  960.0f  ) ? ST1VAFE6AX_XL_ODR_AT_960Hz
	            : (Odr <= 1920.0f  ) ? ST1VAFE6AX_XL_ODR_AT_1920Hz
	            :                      ST1VAFE6AX_XL_ODR_AT_3840Hz;

  return ST1VAFE6AX_OK;
}

/**
 * @brief  Set the ST1VAFE6AX gyroscope sensor output data rate when enabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t ST1VAFE6AX_GYRO_SetOutputDataRate_When_Enabled(ST1VAFE6AX_Object_t *pObj, float_t Odr)
{
  st1vafe6ax_gy_data_rate_t new_odr;

  new_odr = (Odr <=    7.5f) ? ST1VAFE6AX_GY_ODR_AT_7Hz5
          : (Odr <=   15.0f) ? ST1VAFE6AX_GY_ODR_AT_15Hz
          : (Odr <=   30.0f) ? ST1VAFE6AX_GY_ODR_AT_30Hz
          : (Odr <=   60.0f) ? ST1VAFE6AX_GY_ODR_AT_60Hz
          : (Odr <=  120.0f) ? ST1VAFE6AX_GY_ODR_AT_120Hz
          : (Odr <=  240.0f) ? ST1VAFE6AX_GY_ODR_AT_240Hz
          : (Odr <=  480.0f) ? ST1VAFE6AX_GY_ODR_AT_480Hz
          : (Odr <=  960.0f) ? ST1VAFE6AX_GY_ODR_AT_960Hz
          : (Odr <= 1920.0f) ? ST1VAFE6AX_GY_ODR_AT_1920Hz
          :                    ST1VAFE6AX_GY_ODR_AT_3840Hz;

  /* Output data rate selection. */
  if (st1vafe6ax_gy_data_rate_set(&(pObj->Ctx), new_odr) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  return ST1VAFE6AX_OK;
}

/**
 * @brief  Set the ST1VAFE6AX gyroscope sensor output data rate when disabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t ST1VAFE6AX_GYRO_SetOutputDataRate_When_Disabled(ST1VAFE6AX_Object_t *pObj, float_t Odr)
{
  pObj->gyro_odr = (Odr <=    7.5f) ? ST1VAFE6AX_GY_ODR_AT_7Hz5
	             : (Odr <=   15.0f) ? ST1VAFE6AX_GY_ODR_AT_15Hz
	             : (Odr <=   30.0f) ? ST1VAFE6AX_GY_ODR_AT_30Hz
	             : (Odr <=   60.0f) ? ST1VAFE6AX_GY_ODR_AT_60Hz
	             : (Odr <=  120.0f) ? ST1VAFE6AX_GY_ODR_AT_120Hz
	             : (Odr <=  240.0f) ? ST1VAFE6AX_GY_ODR_AT_240Hz
	             : (Odr <=  480.0f) ? ST1VAFE6AX_GY_ODR_AT_480Hz
	             : (Odr <=  960.0f) ? ST1VAFE6AX_GY_ODR_AT_960Hz
	             : (Odr <= 1920.0f) ? ST1VAFE6AX_GY_ODR_AT_1920Hz
	             :                    ST1VAFE6AX_GY_ODR_AT_3840Hz;

  return ST1VAFE6AX_OK;
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
  ST1VAFE6AX_Object_t *pObj = (ST1VAFE6AX_Object_t *)Handle;

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
  ST1VAFE6AX_Object_t *pObj = (ST1VAFE6AX_Object_t *)Handle;

  return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
}


/**
 * @brief  Set the ST1VAFE6AX accelerometer power mode
 * @param  pObj the device pObj
 * @param  PowerMode Value of the powerMode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_ACC_Set_Power_Mode(ST1VAFE6AX_Object_t *pObj, uint8_t PowerMode)
{
  if(st1vafe6ax_xl_mode_set(&(pObj->Ctx), (st1vafe6ax_xl_mode_t)PowerMode) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  return ST1VAFE6AX_OK;
}


/**
 * @brief  Set the ST1VAFE6AX gyroscope power mode
 * @param  pObj the device pObj
 * @param  PowerMode Value of the powerMode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_GYRO_Set_Power_Mode(ST1VAFE6AX_Object_t *pObj, uint8_t PowerMode)
{
  if(st1vafe6ax_gy_mode_set(&(pObj->Ctx), (st1vafe6ax_gy_mode_t)PowerMode) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }

  return ST1VAFE6AX_OK;
}


/**
 * @brief  Set the ST1VAFE6AX accelerometer filter mode
 * @param  pObj the device pObj
 * @param  LowHighPassFlag 0/1 for setting low-pass/high-pass filter mode
 * @param  FilterMode Value of the filter Mode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_ACC_Set_Filter_Mode(ST1VAFE6AX_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode)
{
  if(LowHighPassFlag == 0U)
  {
    /*Set accelerometer low_pass filter-mode*/

    /*Set to 1 LPF2 bit (CTRL8_XL)*/
    if(st1vafe6ax_filt_xl_lp2_set(&(pObj->Ctx), 1) != ST1VAFE6AX_OK)
    {
      return ST1VAFE6AX_ERROR;
    }
    if(st1vafe6ax_filt_xl_lp2_bandwidth_set(&(pObj->Ctx), (st1vafe6ax_filt_xl_lp2_bandwidth_t)FilterMode) != ST1VAFE6AX_OK)
    {
      return ST1VAFE6AX_ERROR;
    }
  }
  else
  {
    if(st1vafe6ax_filt_xl_lp2_set(&(pObj->Ctx), 0U) != ST1VAFE6AX_OK)
    {
      return ST1VAFE6AX_ERROR;
    }
    /*Set accelerometer high_pass filter-mode*/
    if(st1vafe6ax_filt_xl_lp2_bandwidth_set(&(pObj->Ctx), (st1vafe6ax_filt_xl_lp2_bandwidth_t)FilterMode) != ST1VAFE6AX_OK)
    {
      return ST1VAFE6AX_ERROR;
    }
  }
  return ST1VAFE6AX_OK;
}


/**
 * @brief  Set the ST1VAFE6AX gyroscope filter mode
 * @param  pObj the device pObj
 * @param  LowHighPassFlag 0/1 for setting low-pass/high-pass filter mode
 * @param  FilterMode Value of the filter Mode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_GYRO_Set_Filter_Mode(ST1VAFE6AX_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode)
{
  if(LowHighPassFlag == 0U)
  {
    /*Set gyroscope low_pass 1 filter-mode*/
    /* Enable low-pass filter */
    if(st1vafe6ax_filt_gy_lp1_set(&(pObj->Ctx), 1) != ST1VAFE6AX_OK)
    {
      return ST1VAFE6AX_ERROR;
    }
    if(st1vafe6ax_filt_gy_lp1_bandwidth_set(&(pObj->Ctx), (st1vafe6ax_filt_gy_lp1_bandwidth_t)FilterMode) != ST1VAFE6AX_OK)
    {
      return ST1VAFE6AX_ERROR;
    }
  }
  else
  {
    /*Set gyroscope high_pass filter-mode*/
    /* Enable high-pass filter */
    if(st1vafe6ax_filt_gy_lp1_set(&(pObj->Ctx), 0U) != ST1VAFE6AX_OK)
    {
      return ST1VAFE6AX_ERROR;
    }
    if(st1vafe6ax_filt_gy_lp1_bandwidth_set(&(pObj->Ctx), (st1vafe6ax_filt_gy_lp1_bandwidth_t)FilterMode) != ST1VAFE6AX_OK)
    {
      return ST1VAFE6AX_ERROR;
    }
  }
  return ST1VAFE6AX_OK;
}


/**
 * @brief  Get the ST1VAFE6AX vAFE data [LSB]
 * @param  pObj the device pObj
 * @param  vafe pointer where the vafe is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE6AX_BIO_GetVafe(ST1VAFE6AX_Object_t *pObj, int16_t  *vafe)
{
  int32_t ret = ST1VAFE6AX_OK;

  /* Read actual full scale selection from sensor. */
  if (st1vafe6ax_ah_bio_raw_get(&(pObj->Ctx), vafe) != ST1VAFE6AX_OK)
  {
    return ST1VAFE6AX_ERROR;
  }
  return ret;
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
