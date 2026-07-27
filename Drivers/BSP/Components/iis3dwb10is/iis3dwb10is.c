/**
  ******************************************************************************
  * @file    iis3dwb10is.c
  * @author  MEMS Software Solutions Team
  * @brief   IIS3DWB10IS driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "iis3dwb10is.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @defgroup IIS3DWB10IS IIS3DWB10IS
 * @{
 */

/** @defgroup IIS3DWB10IS_Exported_Variables IIS3DWB10IS Exported Variables
 * @{
 */

IIS3DWB10IS_CommonDrv_t IIS3DWB10IS_COMMON_Driver =
{
  IIS3DWB10IS_Init,
  IIS3DWB10IS_DeInit,
  IIS3DWB10IS_ReadID,
  IIS3DWB10IS_GetCapabilities,
};

IIS3DWB10IS_ACC_Drv_t IIS3DWB10IS_ACC_Driver =
{
  IIS3DWB10IS_ACC_Enable,
  IIS3DWB10IS_ACC_Disable,
  IIS3DWB10IS_ACC_GetSensitivity,
  IIS3DWB10IS_ACC_GetOutputDataRate,
  IIS3DWB10IS_ACC_SetOutputDataRate,
  IIS3DWB10IS_ACC_GetFullScale,
  IIS3DWB10IS_ACC_SetFullScale,
  IIS3DWB10IS_ACC_GetAxes,
  IIS3DWB10IS_ACC_GetAxesRaw,
};

/**
 * @}
 */

/** @defgroup IIS3DWB10IS_Private_Function_Prototypes IIS3DWB10IS Private Function Prototypes
 * @{
 */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t iis3dwb10is_auto_increment_set(const stmdev_ctx_t *ctx, uint8_t val);  // TODO: Move this function to PID driver


/**
 * @}
 */

/** @defgroup IIS3DWB10IS_Exported_Functions IIS3DWB10IS Exported Functions
 * @{
 */

/**
 * @brief  Register Component Bus IO operations
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t IIS3DWB10IS_RegisterBusIO(IIS3DWB10IS_Object_t *pObj, IIS3DWB10IS_IO_t *pIO)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = IIS3DWB10IS_ERROR;
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
    pObj->Ctx.handle    = pObj;

    if (pObj->IO.Init != NULL)
    {
      ret = pObj->IO.Init();
    }
    else
    {
      ret = IIS3DWB10IS_ERROR;
    }
  }

  return ret;
}

/**
 * @brief  Initialize the IIS3DWB10IS sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t IIS3DWB10IS_Init(IIS3DWB10IS_Object_t *pObj)
{
  /* Reset registers to their default value */
  if (iis3dwb10is_sw_reset(&(pObj->Ctx)) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  /* Enable register address automatically incremented during a multiple byte
     access with a serial interface */
  if (iis3dwb10is_auto_increment_set(&(pObj->Ctx), PROPERTY_ENABLE) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  /* Enable BDU */
  if (iis3dwb10is_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  /* FIFO mode selection */
  if (iis3dwb10is_fifo_mode_set(&(pObj->Ctx), IIS3DWB10IS_BYPASS_MODE) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  /* Select continuous mode and default output data rate */
  pObj->acc_data_rate = (iis3dwb10is_data_rate_t){
    .burst = IIS3DWB10IS_CONTINUOS_MODE,
    .odr   = IIS3DWB10IS_ODR_2KHz5
  };

  /* Output data rate selection - power down */
  iis3dwb10is_data_rate_t data_rate = {
    .burst = IIS3DWB10IS_CONTINUOS_MODE,
    .odr   = IIS3DWB10IS_ODR_IDLE
  };

  if (iis3dwb10is_xl_data_rate_set(&(pObj->Ctx), data_rate) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  /* Full scale selection. */
  if (iis3dwb10is_xl_full_scale_set(&(pObj->Ctx), IIS3DWB10IS_50g) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  /* Select 16-bit output data format */
  iis3dwb10is_xl_data_cfg_t data_config;

  if (iis3dwb10is_xl_data_config_get(&(pObj->Ctx), &data_config) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  data_config.rounding = IIS3DWB10IS_WRAPAROUND_2_EN;

  if (iis3dwb10is_xl_data_config_set(&(pObj->Ctx), data_config) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  pObj->is_initialized = 1;

  return IIS3DWB10IS_OK;
}

/**
 * @brief  Deinitialize the IIS3DWB10IS sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t IIS3DWB10IS_DeInit(IIS3DWB10IS_Object_t *pObj)
{
  /* Disable the component */
  if (IIS3DWB10IS_ACC_Disable(pObj) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  pObj->is_initialized = 0;

  return IIS3DWB10IS_OK;
}

/**
 * @brief  Read component ID
 * @param  pObj the device pObj
 * @param  Id the WHO_AM_I value
 * @retval 0 in case of success, an error code otherwise
 */
int32_t IIS3DWB10IS_ReadID(IIS3DWB10IS_Object_t *pObj, uint8_t *Id)
{
  if (iis3dwb10is_device_id_get(&(pObj->Ctx), Id) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  return IIS3DWB10IS_OK;
}

/**
 * @brief  Get IIS3DWB10IS sensor capabilities
 * @param  pObj Component object pointer
 * @param  Capabilities pointer to IIS3DWB10IS sensor capabilities
 * @retval 0 in case of success, an error code otherwise
 */
int32_t IIS3DWB10IS_GetCapabilities(IIS3DWB10IS_Object_t *pObj, IIS3DWB10IS_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Acc        = 1;
  Capabilities->Gyro       = 0;
  Capabilities->Magneto    = 0;
  Capabilities->LowPower   = 0;
  Capabilities->AccMaxFS   = 200;
  Capabilities->GyroMaxFS  = 0;
  Capabilities->MagMaxFS   = 0;
  Capabilities->AccMaxOdr  = 80000.0f;
  Capabilities->GyroMaxOdr = 0.0f;
  Capabilities->MagMaxOdr  = 0.0f;
  return IIS3DWB10IS_OK;
}

/**
 * @brief  Enable the IIS3DWB10IS accelerometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t IIS3DWB10IS_ACC_Enable(IIS3DWB10IS_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return IIS3DWB10IS_OK;
  }

  /* Output data rate selection. */
  if (iis3dwb10is_xl_data_rate_set(&(pObj->Ctx), pObj->acc_data_rate) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return IIS3DWB10IS_OK;
}

/**
 * @brief  Disable the IIS3DWB10IS accelerometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t IIS3DWB10IS_ACC_Disable(IIS3DWB10IS_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return IIS3DWB10IS_OK;
  }

  /* Get current output data rate. */
  if (iis3dwb10is_xl_data_rate_get(&(pObj->Ctx), &pObj->acc_data_rate) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  /* Output data rate selection - power down. */
  iis3dwb10is_data_rate_t data_rate = {
    .burst = IIS3DWB10IS_CONTINUOS_MODE,
    .odr   = IIS3DWB10IS_ODR_IDLE
  };

  if (iis3dwb10is_xl_data_rate_set(&(pObj->Ctx), data_rate) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return IIS3DWB10IS_OK;
}

/**
 * @brief  Get the IIS3DWB10IS accelerometer sensor sensitivity.
 * @note   This function is limited to 16-bit format only.
 * @param  pObj the device pObj
 * @param  Sensitivity pointer
 * @retval 0 in case of success, an error code otherwise
 */
int32_t IIS3DWB10IS_ACC_GetSensitivity(IIS3DWB10IS_Object_t *pObj, float_t *Sensitivity)
{
  int32_t ret = IIS3DWB10IS_OK;
  iis3dwb10is_xl_data_cfg_t xl_data_cfg;
  iis3dwb10is_fs_xl_t full_scale;

  /* Read actual accelerometer data configuration from sensor. */
  if (iis3dwb10is_xl_data_config_get(&(pObj->Ctx), &xl_data_cfg) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  /* Read actual full scale selection from sensor. */
  if (iis3dwb10is_xl_full_scale_get(&(pObj->Ctx), &full_scale) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  /* Note: Sensitivity intentionally restricted only to 16-bit format due to
   *       'IIS3DWB10IS_ACC_GetAxesRaw()' function limitation */
  xl_data_cfg.rounding = IIS3DWB10IS_WRAPAROUND_2_EN;

  if (xl_data_cfg.rounding == IIS3DWB10IS_WRAPAROUND_2_EN)
  {
    switch (full_scale)
    {
    case IIS3DWB10IS_50g:
      *Sensitivity = IIS3DWB10IS_ACC_SENSITIVITY_FOR_FS_50G_16_BIT_FORMAT;
      break;

    case IIS3DWB10IS_100g:
      *Sensitivity = IIS3DWB10IS_ACC_SENSITIVITY_FOR_FS_100G_16_BIT_FORMAT;
      break;

    case IIS3DWB10IS_200g:
      *Sensitivity = IIS3DWB10IS_ACC_SENSITIVITY_FOR_FS_200G_16_BIT_FORMAT;
      break;

    default:
      *Sensitivity = -1.0f;
      ret = IIS3DWB10IS_ERROR;
      break;
    }
  }
  else
  {
    switch (full_scale)
    {
    case IIS3DWB10IS_50g:
      *Sensitivity = IIS3DWB10IS_ACC_SENSITIVITY_FOR_FS_50G_20_BIT_FORMAT;
      break;

    case IIS3DWB10IS_100g:
      *Sensitivity = IIS3DWB10IS_ACC_SENSITIVITY_FOR_FS_100G_20_BIT_FORMAT;
      break;

    case IIS3DWB10IS_200g:
      *Sensitivity = IIS3DWB10IS_ACC_SENSITIVITY_FOR_FS_200G_20_BIT_FORMAT;
      break;

    default:
      *Sensitivity = -1.0f;
      ret = IIS3DWB10IS_ERROR;
      break;
    }
  }

  return ret;
}

/**
 * @brief  Get the IIS3DWB10IS accelerometer sensor output data rate
 * @param  pObj the device pObj
 * @param  odr pointer where the output data rate is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t IIS3DWB10IS_ACC_GetOutputDataRate(IIS3DWB10IS_Object_t *pObj, float_t *odr)
{
  int32_t ret = IIS3DWB10IS_OK;
  iis3dwb10is_data_rate_t data_rate;

  if (iis3dwb10is_xl_data_rate_get(&(pObj->Ctx), &data_rate) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  switch (data_rate.odr)
  {
  case IIS3DWB10IS_ODR_IDLE:
    *odr =  0.0f;
    break;

  case IIS3DWB10IS_ODR_2KHz5:
    *odr =  2500.0f;
    break;

  case IIS3DWB10IS_ODR_5KHz:
    *odr =  5000.0f;
    break;

  case IIS3DWB10IS_ODR_10KHz:
    *odr =  10000.0f;
    break;

  case IIS3DWB10IS_ODR_20KHz:
    *odr =  20000.0f;
    break;

  case IIS3DWB10IS_ODR_40KHz:
    *odr =  40000.0f;
    break;

  case IIS3DWB10IS_ODR_80KHz:
    *odr =  80000.0f;
    break;

  default:
    *odr = -1.0f;
    ret = IIS3DWB10IS_ERROR;
    break;
  }

  return ret;
}

/**
 * @brief  Set the IIS3DWB10IS accelerometer sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t IIS3DWB10IS_ACC_SetOutputDataRate(IIS3DWB10IS_Object_t *pObj, float_t Odr)
{
  iis3dwb10is_data_rate_t data_rate;

  /* Get current output data rate. */
  if (iis3dwb10is_xl_data_rate_get(&(pObj->Ctx), &data_rate) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  data_rate.odr = (Odr <=     0.0f) ? IIS3DWB10IS_ODR_IDLE
		        : (Odr <=  2500.0f) ? IIS3DWB10IS_ODR_2KHz5
		        : (Odr <=  5000.0f) ? IIS3DWB10IS_ODR_5KHz
		        : (Odr <= 10000.0f) ? IIS3DWB10IS_ODR_10KHz
		        : (Odr <= 20000.0f) ? IIS3DWB10IS_ODR_20KHz
		        : (Odr <= 40000.0f) ? IIS3DWB10IS_ODR_40KHz
		        :                     IIS3DWB10IS_ODR_80KHz;

  /* Output data rate selection. */
  if (iis3dwb10is_xl_data_rate_set(&(pObj->Ctx), data_rate) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  return IIS3DWB10IS_OK;
}

/**
 * @brief  Get the IIS3DWB10IS accelerometer sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale pointer where the full scale is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t IIS3DWB10IS_ACC_GetFullScale(IIS3DWB10IS_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = IIS3DWB10IS_OK;
  iis3dwb10is_fs_xl_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (iis3dwb10is_xl_full_scale_get(&(pObj->Ctx), &fs_low_level) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  switch (fs_low_level)
  {
    case IIS3DWB10IS_50g:
      *FullScale =  50;
      break;

    case IIS3DWB10IS_100g:
      *FullScale = 100;
      break;

    case IIS3DWB10IS_200g:
      *FullScale = 200;
      break;

    default:
      *FullScale = -1;
      ret = IIS3DWB10IS_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the IIS3DWB10IS accelerometer sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale the functional full scale to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t IIS3DWB10IS_ACC_SetFullScale(IIS3DWB10IS_Object_t *pObj, int32_t FullScale)
{
  iis3dwb10is_fs_xl_t new_fs;

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file
   * static analysis point of view because the parameter passed to the function
   * is not known at the moment of analysis */
  new_fs = (FullScale <=  50) ? IIS3DWB10IS_50g
         : (FullScale <= 100) ? IIS3DWB10IS_100g
         :                      IIS3DWB10IS_200g;

  if (iis3dwb10is_xl_full_scale_set(&(pObj->Ctx), new_fs) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  return IIS3DWB10IS_OK;
}

/**
 * @brief  Get the IIS3DWB10IS accelerometer sensor raw axes.
 * @note   This function is limited to 16-bit raw values only.
 * @param  pObj  pointer to the device object
 * @param  Value pointer to the raw axes output structure
 * @retval 0     on success
 * @retval Non-zero error code otherwise
 */
int32_t IIS3DWB10IS_ACC_GetAxesRaw(IIS3DWB10IS_Object_t *pObj, IIS3DWB10IS_AxesRaw16_t *Value)
{
  int32_t ret = IIS3DWB10IS_OK;
  iis3dwb10is_xl_data_cfg_t xl_data_cfg;
  uint32_t temp = 0;

  /* Read actual accelerometer data configuration from sensor. */
  if (iis3dwb10is_xl_data_config_get(&(pObj->Ctx), &xl_data_cfg) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  if (xl_data_cfg.rounding == IIS3DWB10IS_WRAPAROUND_2_EN)
  {
    /* Read raw data values - 16-bit format. */
    iis3dwb10is_axis3bit16_t data_raw;

    if (iis3dwb10is_acceleration_16b_raw_get(&(pObj->Ctx), data_raw.i16bit) != IIS3DWB10IS_OK)
    {
      return IIS3DWB10IS_ERROR;
    }

    /* Format the data. */
    Value->x = data_raw.i16bit[0];
    Value->y = data_raw.i16bit[1];
    Value->z = data_raw.i16bit[2];
  }
  else
  {
    /* Read raw data values - 20-bit format. */
    iis3dwb10is_axis3bit32_t data_raw;

    if (iis3dwb10is_acceleration_raw_get(&(pObj->Ctx), data_raw.i32bit) != IIS3DWB10IS_OK)
    {
      return IIS3DWB10IS_ERROR;
    }

    /* Format the data */
    /* Note: Precision intentionally decreased from 20-bit down to 16-bit due
     *       to function limitation */
    temp = (uint32_t)data_raw.i32bit[0] >> 4;
    Value->x = (int16_t)temp;
    temp = (uint32_t)data_raw.i32bit[1] >> 4;
    Value->y = (int16_t)temp;
    temp = (uint32_t)data_raw.i32bit[2] >> 4;
    Value->z = (int16_t)temp;
  }

  return ret;
}

/**
 * @brief  Get the IIS3DWB10IS accelerometer sensor axes
 * @param  pObj the device pObj
 * @param  Acceleration pointer where the values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t IIS3DWB10IS_ACC_GetAxes(IIS3DWB10IS_Object_t *pObj, IIS3DWB10IS_Axes_t *Acceleration)
{
  IIS3DWB10IS_AxesRaw16_t acc_raw;
  float_t temp = 0.0f;

  /* Read raw data values. */
  if (IIS3DWB10IS_ACC_GetAxesRaw(pObj, &acc_raw) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  float_t sensitivity = 0.0f;

  /* Get IIS3DWB10IS actual sensitivity. */
  if (IIS3DWB10IS_ACC_GetSensitivity(pObj, &sensitivity) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  /* Calculate the data. */
  temp = (float_t)acc_raw.x * sensitivity;
  Acceleration->x = (int32_t)temp;
  temp = (float_t)acc_raw.y * sensitivity;
  Acceleration->y = (int32_t)temp;
  temp = (float_t)acc_raw.z * sensitivity;
  Acceleration->z = (int32_t)temp;

  return IIS3DWB10IS_OK;
}

/**
 * @brief  Get the IIS3DWB10IS register value
 * @param  pObj the device pObj
 * @param  Reg address to be read
 * @param  Data pointer where the value is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t IIS3DWB10IS_Read_Reg(IIS3DWB10IS_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (iis3dwb10is_read_reg(&(pObj->Ctx), Reg, Data, 1) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  return IIS3DWB10IS_OK;
}

/**
 * @brief  Set the IIS3DWB10IS register value
 * @param  pObj the device pObj
 * @param  Reg address to be written
 * @param  Data value to be written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t IIS3DWB10IS_Write_Reg(IIS3DWB10IS_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (iis3dwb10is_write_reg(&(pObj->Ctx), Reg, &Data, 1) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  return IIS3DWB10IS_OK;
}

/**
 * @brief  Get the IIS3DWB10IS ACC data ready bit value
 * @param  pObj the device pObj
 * @param  Status the status of data ready bit
 * @retval 0 in case of success, an error code otherwise
 */
int32_t IIS3DWB10IS_ACC_Get_DRDY_Status(IIS3DWB10IS_Object_t *pObj, uint8_t *Status)
{
  iis3dwb10is_data_ready_t value;

  if (iis3dwb10is_data_ready_get(&(pObj->Ctx), &value) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  *Status = value.drdy_xl;
  return IIS3DWB10IS_OK;
}

/**
  * @brief  Reboot of the device
  * @param  Obj  the device Obj
  * @param  Timeout  max. expected Software reset duration [ms]
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS3DWB10IS_Boot(void *Obj, uint32_t Timeout)
{
  (void)Timeout;  /* Timeout implemented inside low level driver function */

  IIS3DWB10IS_Object_t *pObj = Obj;

  if (iis3dwb10is_reboot(&(pObj->Ctx)) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  return IIS3DWB10IS_OK;
}

/**
  * @brief  Power-on-reset of the device
  * @param  Obj the device Obj
  * @param  Timeout  max. expected Software reset duration [ms]
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS3DWB10IS_SW_Reset(void *Obj, uint32_t Timeout)
{
  (void)Timeout;  /* Timeout implemented inside low level driver function */

  IIS3DWB10IS_Object_t *pObj = Obj;

  if (iis3dwb10is_sw_por(&(pObj->Ctx)) != IIS3DWB10IS_OK)
  {
    return IIS3DWB10IS_ERROR;
  }

  return IIS3DWB10IS_OK;
}

/**
 * @}
 */

/** @defgroup IIS3DWB10IS_Private_Functions IIS3DWB10IS Private Functions
 * @{
 */


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
  IIS3DWB10IS_Object_t *pObj = (IIS3DWB10IS_Object_t *)Handle;

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
  IIS3DWB10IS_Object_t *pObj = (IIS3DWB10IS_Object_t *)Handle;

  return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
}

// TODO: Move this function to PID driver
/**
  * @brief  Register address automatically incremented during a multiple byte
  *         access with a serial interface.[set]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Change the values of if_inc in reg CTRL3
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
static int32_t iis3dwb10is_auto_increment_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  iis3dwb10is_ctrl3_t ctrl3;

  int32_t ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_CTRL3, (uint8_t *)&ctrl3, 1);

  if (ret == 0)
  {
    ctrl3.if_inc = (uint8_t)val;
    ret = iis3dwb10is_write_reg(ctx, IIS3DWB10IS_CTRL3, (uint8_t *)&ctrl3, 1);
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
