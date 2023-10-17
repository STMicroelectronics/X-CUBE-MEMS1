/**
  ******************************************************************************
  * @file    asm330lhhx.c
  * @author  MEMS Software Solutions Team
  * @brief   ASM330LHHX driver file
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
#include "asm330lhhx.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup ASM330LHHX ASM330LHHX
  * @{
  */

/** @defgroup ASM330LHHX_Exported_Variables ASM330LHHX Exported Variables
  * @{
  */

ASM330LHHX_CommonDrv_t ASM330LHHX_COMMON_Driver =
{
  ASM330LHHX_Init,
  ASM330LHHX_DeInit,
  ASM330LHHX_ReadID,
  ASM330LHHX_GetCapabilities,
};

ASM330LHHX_ACC_Drv_t ASM330LHHX_ACC_Driver =
{
  ASM330LHHX_ACC_Enable,
  ASM330LHHX_ACC_Disable,
  ASM330LHHX_ACC_GetSensitivity,
  ASM330LHHX_ACC_GetOutputDataRate,
  ASM330LHHX_ACC_SetOutputDataRate,
  ASM330LHHX_ACC_GetFullScale,
  ASM330LHHX_ACC_SetFullScale,
  ASM330LHHX_ACC_GetAxes,
  ASM330LHHX_ACC_GetAxesRaw,
};

ASM330LHHX_GYRO_Drv_t ASM330LHHX_GYRO_Driver =
{
  ASM330LHHX_GYRO_Enable,
  ASM330LHHX_GYRO_Disable,
  ASM330LHHX_GYRO_GetSensitivity,
  ASM330LHHX_GYRO_GetOutputDataRate,
  ASM330LHHX_GYRO_SetOutputDataRate,
  ASM330LHHX_GYRO_GetFullScale,
  ASM330LHHX_GYRO_SetFullScale,
  ASM330LHHX_GYRO_GetAxes,
  ASM330LHHX_GYRO_GetAxesRaw,
};

/**
  * @}
  */

/** @defgroup ASM330LHHX_Private_Function_Prototypes ASM330LHHX Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t ASM330LHHX_ACC_SetOutputDataRate_When_Enabled(ASM330LHHX_Object_t *pObj, float Odr);
static int32_t ASM330LHHX_ACC_SetOutputDataRate_When_Disabled(ASM330LHHX_Object_t *pObj, float Odr);
static int32_t ASM330LHHX_GYRO_SetOutputDataRate_When_Enabled(ASM330LHHX_Object_t *pObj, float Odr);
static int32_t ASM330LHHX_GYRO_SetOutputDataRate_When_Disabled(ASM330LHHX_Object_t *pObj, float Odr);

/**
  * @}
  */

/** @defgroup ASM330LHHX_Exported_Functions ASM330LHHX Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_RegisterBusIO(ASM330LHHX_Object_t *pObj, ASM330LHHX_IO_t *pIO)
{
  int32_t ret = ASM330LHHX_OK;

  if (pObj == NULL)
  {
    ret = ASM330LHHX_ERROR;
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
      ret = ASM330LHHX_ERROR;
    }
    else if (pObj->IO.Init() != ASM330LHHX_OK)
    {
      ret = ASM330LHHX_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == ASM330LHHX_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x0C;

          if (ASM330LHHX_Write_Reg(pObj, ASM330LHHX_CTRL3_C, data) != ASM330LHHX_OK)
          {
            ret = ASM330LHHX_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  Initialize the ASM330LHHX sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_Init(ASM330LHHX_Object_t *pObj)
{
  if(pObj->IO.BusType != ASM330LHHX_I3C_BUS)
  {
    /* Disable I3C */
    if (asm330lhhx_i3c_disable_set(&(pObj->Ctx), ASM330LHHX_I3C_DISABLE) != ASM330LHHX_OK)
    {
      return ASM330LHHX_ERROR;
    }
  }

  /* Enable register address automatically incremented during a multiple byte
  access with a serial interface. */
  if (asm330lhhx_auto_increment_set(&(pObj->Ctx), PROPERTY_ENABLE) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Enable BDU */
  if (asm330lhhx_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* FIFO mode selection */
  if (asm330lhhx_fifo_mode_set(&(pObj->Ctx), ASM330LHHX_BYPASS_MODE) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = ASM330LHHX_XL_ODR_104Hz;

  /* Output data rate selection - power down. */
  if (asm330lhhx_xl_data_rate_set(&(pObj->Ctx), ASM330LHHX_XL_ODR_OFF) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Full scale selection. */
  if (asm330lhhx_xl_full_scale_set(&(pObj->Ctx), ASM330LHHX_2g) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Select default output data rate. */
  pObj->gyro_odr = ASM330LHHX_GY_ODR_104Hz;

  /* Output data rate selection - power down. */
  if (asm330lhhx_gy_data_rate_set(&(pObj->Ctx), ASM330LHHX_GY_ODR_OFF) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Full scale selection. */
  if (asm330lhhx_gy_full_scale_set(&(pObj->Ctx), ASM330LHHX_2000dps) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  pObj->is_initialized = 1;

  return ASM330LHHX_OK;
}

/**
  * @brief  Deinitialize the ASM330LHHX sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_DeInit(ASM330LHHX_Object_t *pObj)
{
  /* Disable the component */
  if (ASM330LHHX_ACC_Disable(pObj) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  if (ASM330LHHX_GYRO_Disable(pObj) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Reset output data rate. */
  pObj->acc_odr = ASM330LHHX_XL_ODR_OFF;
  pObj->gyro_odr = ASM330LHHX_GY_ODR_OFF;

  pObj->is_initialized = 0;

  return ASM330LHHX_OK;
}

/**
  * @brief  Read component ID
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ReadID(ASM330LHHX_Object_t *pObj, uint8_t *Id)
{
  if (asm330lhhx_device_id_get(&(pObj->Ctx), Id) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Get ASM330LHHX sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to ASM330LHHX sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_GetCapabilities(ASM330LHHX_Object_t *pObj, ASM330LHHX_Capabilities_t *Capabilities)
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
  Capabilities->GyroMaxOdr   = 6667.0f;
  Capabilities->AccMaxOdr    = 6667.0f;
  Capabilities->MagMaxOdr    = 0.0f;
  return ASM330LHHX_OK;
}

/**
  * @brief  Enable the ASM330LHHX accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_Enable(ASM330LHHX_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return ASM330LHHX_OK;
  }

  /* Output data rate selection. */
  if (asm330lhhx_xl_data_rate_set(&(pObj->Ctx), pObj->acc_odr) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return ASM330LHHX_OK;
}

/**
  * @brief  Disable the ASM330LHHX accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_Disable(ASM330LHHX_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return ASM330LHHX_OK;
  }

  /* Get current output data rate. */
  if (asm330lhhx_xl_data_rate_get(&(pObj->Ctx), &pObj->acc_odr) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Output data rate selection - power down. */
  if (asm330lhhx_xl_data_rate_set(&(pObj->Ctx), ASM330LHHX_XL_ODR_OFF) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return ASM330LHHX_OK;
}

/**
  * @brief  Get the ASM330LHHX accelerometer sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_GetSensitivity(ASM330LHHX_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = ASM330LHHX_OK;
  asm330lhhx_fs_xl_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (asm330lhhx_xl_full_scale_get(&(pObj->Ctx), &full_scale) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Store the Sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case ASM330LHHX_2g:
      *Sensitivity = ASM330LHHX_ACC_SENSITIVITY_FS_2G;
      break;

    case ASM330LHHX_4g:
      *Sensitivity = ASM330LHHX_ACC_SENSITIVITY_FS_4G;
      break;

    case ASM330LHHX_8g:
      *Sensitivity = ASM330LHHX_ACC_SENSITIVITY_FS_8G;
      break;

    case ASM330LHHX_16g:
      *Sensitivity = ASM330LHHX_ACC_SENSITIVITY_FS_16G;
      break;

    default:
      ret = ASM330LHHX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the ASM330LHHX accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_GetOutputDataRate(ASM330LHHX_Object_t *pObj, float *Odr)
{
  int32_t ret = ASM330LHHX_OK;
  asm330lhhx_odr_xl_t odr_low_level;

  /* Get current output data rate. */
  if (asm330lhhx_xl_data_rate_get(&(pObj->Ctx), &odr_low_level) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  switch (odr_low_level)
  {
    case ASM330LHHX_XL_ODR_OFF:
      *Odr = 0.0f;
      break;

    case ASM330LHHX_XL_ODR_12Hz5:
      *Odr = 12.5f;
      break;

    case ASM330LHHX_XL_ODR_26Hz:
      *Odr = 26.0f;
      break;

    case ASM330LHHX_XL_ODR_52Hz:
      *Odr = 52.0f;
      break;

    case ASM330LHHX_XL_ODR_104Hz:
      *Odr = 104.0f;
      break;

    case ASM330LHHX_XL_ODR_208Hz:
      *Odr = 208.0f;
      break;

    case ASM330LHHX_XL_ODR_417Hz:
      *Odr = 416.0f;
      break;

    case ASM330LHHX_XL_ODR_833Hz:
      *Odr = 833.0f;
      break;

    case ASM330LHHX_XL_ODR_1667Hz:
      *Odr = 1667.0f;
      break;

    case ASM330LHHX_XL_ODR_3333Hz:
      *Odr = 3333.0f;
      break;

    case ASM330LHHX_XL_ODR_6667Hz:
      *Odr = 6667.0f;
      break;

    default:
      ret = ASM330LHHX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the ASM330LHHX accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_SetOutputDataRate(ASM330LHHX_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return ASM330LHHX_ACC_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return ASM330LHHX_ACC_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the ASM330LHHX accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_GetFullScale(ASM330LHHX_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = ASM330LHHX_OK;
  asm330lhhx_fs_xl_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (asm330lhhx_xl_full_scale_get(&(pObj->Ctx), &fs_low_level) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  switch (fs_low_level)
  {
    case ASM330LHHX_2g:
      *FullScale =  2;
      break;

    case ASM330LHHX_4g:
      *FullScale =  4;
      break;

    case ASM330LHHX_8g:
      *FullScale =  8;
      break;

    case ASM330LHHX_16g:
      *FullScale = 16;
      break;

    default:
      ret = ASM330LHHX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the ASM330LHHX accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_SetFullScale(ASM330LHHX_Object_t *pObj, int32_t FullScale)
{
  asm330lhhx_fs_xl_t new_fs;

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file statical analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  new_fs = (FullScale <= 2) ? ASM330LHHX_2g
           : (FullScale <= 4) ? ASM330LHHX_4g
           : (FullScale <= 8) ? ASM330LHHX_8g
           :                    ASM330LHHX_16g;

  if (asm330lhhx_xl_full_scale_set(&(pObj->Ctx), new_fs) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Get the ASM330LHHX accelerometer sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_GetAxesRaw(ASM330LHHX_Object_t *pObj, ASM330LHHX_AxesRaw_t *Value)
{
  asm330lhhx_axis3bit16_t data_raw;

  /* Read raw data values. */
  if (asm330lhhx_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return ASM330LHHX_OK;
}

/**
  * @brief  Get the ASM330LHHX accelerometer sensor axes
  * @param  pObj the device pObj
  * @param  Acceleration pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_GetAxes(ASM330LHHX_Object_t *pObj, ASM330LHHX_Axes_t *Acceleration)
{
  asm330lhhx_axis3bit16_t data_raw;
  float sensitivity = 0.0f;

  /* Read raw data values. */
  if (asm330lhhx_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Get ASM330LHHX actual sensitivity. */
  if (ASM330LHHX_ACC_GetSensitivity(pObj, &sensitivity) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Calculate the data. */
  Acceleration->x = (int32_t)((float)((float)data_raw.i16bit[0] * sensitivity));
  Acceleration->y = (int32_t)((float)((float)data_raw.i16bit[1] * sensitivity));
  Acceleration->z = (int32_t)((float)((float)data_raw.i16bit[2] * sensitivity));

  return ASM330LHHX_OK;
}

/**
  * @brief  Enable the ASM330LHHX gyroscope sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_GYRO_Enable(ASM330LHHX_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->gyro_is_enabled == 1U)
  {
    return ASM330LHHX_OK;
  }

  /* Output data rate selection. */
  if (asm330lhhx_gy_data_rate_set(&(pObj->Ctx), pObj->gyro_odr) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  pObj->gyro_is_enabled = 1;

  return ASM330LHHX_OK;
}

/**
  * @brief  Disable the ASM330LHHX gyroscope sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_GYRO_Disable(ASM330LHHX_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->gyro_is_enabled == 0U)
  {
    return ASM330LHHX_OK;
  }

  /* Get current output data rate. */
  if (asm330lhhx_gy_data_rate_get(&(pObj->Ctx), &pObj->gyro_odr) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Output data rate selection - power down. */
  if (asm330lhhx_gy_data_rate_set(&(pObj->Ctx), ASM330LHHX_GY_ODR_OFF) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  pObj->gyro_is_enabled = 0;

  return ASM330LHHX_OK;
}

/**
  * @brief  Get the ASM330LHHX gyroscope sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_GYRO_GetSensitivity(ASM330LHHX_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = ASM330LHHX_OK;
  asm330lhhx_fs_g_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (asm330lhhx_gy_full_scale_get(&(pObj->Ctx), &full_scale) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Store the sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case ASM330LHHX_125dps:
      *Sensitivity = ASM330LHHX_GYRO_SENSITIVITY_FS_125DPS;
      break;

    case ASM330LHHX_250dps:
      *Sensitivity = ASM330LHHX_GYRO_SENSITIVITY_FS_250DPS;
      break;

    case ASM330LHHX_500dps:
      *Sensitivity = ASM330LHHX_GYRO_SENSITIVITY_FS_500DPS;
      break;

    case ASM330LHHX_1000dps:
      *Sensitivity = ASM330LHHX_GYRO_SENSITIVITY_FS_1000DPS;
      break;

    case ASM330LHHX_2000dps:
      *Sensitivity = ASM330LHHX_GYRO_SENSITIVITY_FS_2000DPS;
      break;

    case ASM330LHHX_4000dps:
      *Sensitivity = ASM330LHHX_GYRO_SENSITIVITY_FS_4000DPS;
      break;

    default:
      ret = ASM330LHHX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the ASM330LHHX gyroscope sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_GYRO_GetOutputDataRate(ASM330LHHX_Object_t *pObj, float *Odr)
{
  int32_t ret = ASM330LHHX_OK;
  asm330lhhx_odr_g_t odr_low_level;

  /* Get current output data rate. */
  if (asm330lhhx_gy_data_rate_get(&(pObj->Ctx), &odr_low_level) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  switch (odr_low_level)
  {
    case ASM330LHHX_GY_ODR_OFF:
      *Odr = 0.0f;
      break;

    case ASM330LHHX_GY_ODR_12Hz5:
      *Odr = 12.5f;
      break;

    case ASM330LHHX_GY_ODR_26Hz:
      *Odr = 26.0f;
      break;

    case ASM330LHHX_GY_ODR_52Hz:
      *Odr = 52.0f;
      break;

    case ASM330LHHX_GY_ODR_104Hz:
      *Odr = 104.0f;
      break;

    case ASM330LHHX_GY_ODR_208Hz:
      *Odr = 208.0f;
      break;

    case ASM330LHHX_GY_ODR_417Hz:
      *Odr = 416.0f;
      break;

    case ASM330LHHX_GY_ODR_833Hz:
      *Odr = 833.0f;
      break;

    case ASM330LHHX_GY_ODR_1667Hz:
      *Odr =  1667.0f;
      break;

    case ASM330LHHX_GY_ODR_3333Hz:
      *Odr =  3333.0f;
      break;

    case ASM330LHHX_GY_ODR_6667Hz:
      *Odr =  6667.0f;
      break;

    default:
      ret = ASM330LHHX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the ASM330LHHX gyroscope sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_GYRO_SetOutputDataRate(ASM330LHHX_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->gyro_is_enabled == 1U)
  {
    return ASM330LHHX_GYRO_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return ASM330LHHX_GYRO_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the ASM330LHHX gyroscope sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_GYRO_GetFullScale(ASM330LHHX_Object_t *pObj, int32_t  *FullScale)
{
  int32_t ret = ASM330LHHX_OK;
  asm330lhhx_fs_g_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (asm330lhhx_gy_full_scale_get(&(pObj->Ctx), &fs_low_level) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  switch (fs_low_level)
  {
    case ASM330LHHX_125dps:
      *FullScale =  125;
      break;

    case ASM330LHHX_250dps:
      *FullScale =  250;
      break;

    case ASM330LHHX_500dps:
      *FullScale =  500;
      break;

    case ASM330LHHX_1000dps:
      *FullScale = 1000;
      break;

    case ASM330LHHX_2000dps:
      *FullScale = 2000;
      break;

    case ASM330LHHX_4000dps:
      *FullScale = 4000;
      break;

    default:
      ret = ASM330LHHX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the ASM330LHHX gyroscope sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_GYRO_SetFullScale(ASM330LHHX_Object_t *pObj, int32_t FullScale)
{
  asm330lhhx_fs_g_t new_fs;

  new_fs = (FullScale <= 125)  ? ASM330LHHX_125dps
           : (FullScale <= 250)  ? ASM330LHHX_250dps
           : (FullScale <= 500)  ? ASM330LHHX_500dps
           : (FullScale <= 1000) ? ASM330LHHX_1000dps
           : (FullScale <= 2000) ? ASM330LHHX_2000dps
           :                       ASM330LHHX_4000dps;

  if (asm330lhhx_gy_full_scale_set(&(pObj->Ctx), new_fs) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Get the ASM330LHHX gyroscope sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_GYRO_GetAxesRaw(ASM330LHHX_Object_t *pObj, ASM330LHHX_AxesRaw_t *Value)
{
  asm330lhhx_axis3bit16_t data_raw;

  /* Read raw data values. */
  if (asm330lhhx_angular_rate_raw_get(&(pObj->Ctx), data_raw.i16bit) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return ASM330LHHX_OK;
}

/**
  * @brief  Get the ASM330LHHX gyroscope sensor axes
  * @param  pObj the device pObj
  * @param  AngularRate pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_GYRO_GetAxes(ASM330LHHX_Object_t *pObj, ASM330LHHX_Axes_t *AngularRate)
{
  asm330lhhx_axis3bit16_t data_raw;
  float sensitivity;

  /* Read raw data values. */
  if (asm330lhhx_angular_rate_raw_get(&(pObj->Ctx), data_raw.i16bit) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Get ASM330LHHX actual sensitivity. */
  if (ASM330LHHX_GYRO_GetSensitivity(pObj, &sensitivity) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Calculate the data. */
  AngularRate->x = (int32_t)((float)((float)data_raw.i16bit[0] * sensitivity));
  AngularRate->y = (int32_t)((float)((float)data_raw.i16bit[1] * sensitivity));
  AngularRate->z = (int32_t)((float)((float)data_raw.i16bit[2] * sensitivity));

  return ASM330LHHX_OK;
}

/**
  * @brief  Get the ASM330LHHX register value
  * @param  pObj the device pObj
  * @param  Reg address to be read
  * @param  Data pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_Read_Reg(ASM330LHHX_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (asm330lhhx_read_reg(&(pObj->Ctx), Reg, Data, 1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Set the ASM330LHHX register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_Write_Reg(ASM330LHHX_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (asm330lhhx_write_reg(&(pObj->Ctx), Reg, &Data, 1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Set the interrupt latch
  * @param  pObj the device pObj
  * @param  Status value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_Set_Interrupt_Latch(ASM330LHHX_Object_t *pObj, uint8_t Status)
{
  if (Status > 1U)
  {
    return ASM330LHHX_ERROR;
  }

  if (asm330lhhx_int_notification_set(&(pObj->Ctx), (asm330lhhx_lir_t)Status) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Enable free fall detection
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_Enable_Free_Fall_Detection(ASM330LHHX_Object_t *pObj, ASM330LHHX_SensorIntPin_t IntPin)
{
  int32_t ret = ASM330LHHX_OK;
  asm330lhhx_pin_int1_route_t val1;
  asm330lhhx_pin_int2_route_t val2;

  /* Output Data Rate selection */
  if (ASM330LHHX_ACC_SetOutputDataRate(pObj, 416.0f) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Full scale selection */
  if (ASM330LHHX_ACC_SetFullScale(pObj, 2) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* FF_DUR setting */
  if (asm330lhhx_ff_dur_set(&(pObj->Ctx), 0x06) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* WAKE_DUR setting */
  if (asm330lhhx_wkup_dur_set(&(pObj->Ctx), 0x00) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* SLEEP_DUR setting */
  if (asm330lhhx_act_sleep_dur_set(&(pObj->Ctx), 0x00) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* FF_THS setting */
  if (asm330lhhx_ff_threshold_set(&(pObj->Ctx), ASM330LHHX_FF_TSH_312mg) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Enable free fall event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case ASM330LHHX_INT1_PIN:
      if (asm330lhhx_pin_int1_route_get(&(pObj->Ctx), &val1) != ASM330LHHX_OK)
      {
        return ASM330LHHX_ERROR;
      }

      val1.md1_cfg.int1_ff = PROPERTY_ENABLE;

      if (asm330lhhx_pin_int1_route_set(&(pObj->Ctx), &val1) != ASM330LHHX_OK)
      {
        return ASM330LHHX_ERROR;
      }
      break;

    case ASM330LHHX_INT2_PIN:
      if (asm330lhhx_pin_int2_route_get(&(pObj->Ctx), &val2) != ASM330LHHX_OK)
      {
        return ASM330LHHX_ERROR;
      }

      val2.md2_cfg.int2_ff = PROPERTY_ENABLE;

      if (asm330lhhx_pin_int2_route_set(&(pObj->Ctx), &val2) != ASM330LHHX_OK)
      {
        return ASM330LHHX_ERROR;
      }
      break;

    default:
      ret = ASM330LHHX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Disable free fall detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_Disable_Free_Fall_Detection(ASM330LHHX_Object_t *pObj)
{
  asm330lhhx_pin_int1_route_t val1;
  asm330lhhx_pin_int2_route_t val2;

  /* Disable free fall event on both INT1 and INT2 pins */
  if (asm330lhhx_pin_int1_route_get(&(pObj->Ctx), &val1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  val1.md1_cfg.int1_ff = PROPERTY_DISABLE;

  if (asm330lhhx_pin_int1_route_set(&(pObj->Ctx), &val1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  if (asm330lhhx_pin_int2_route_get(&(pObj->Ctx), &val2) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  val2.md2_cfg.int2_ff = PROPERTY_DISABLE;

  if (asm330lhhx_pin_int2_route_set(&(pObj->Ctx), &val2) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* FF_DUR setting */
  if (asm330lhhx_ff_dur_set(&(pObj->Ctx), 0x00) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* FF_THS setting */
  if (asm330lhhx_ff_threshold_set(&(pObj->Ctx), ASM330LHHX_FF_TSH_156mg) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Set free fall threshold
  * @param  pObj the device pObj
  * @param  Threshold free fall detection threshold
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_Set_Free_Fall_Threshold(ASM330LHHX_Object_t *pObj, uint8_t Threshold)
{
  if (asm330lhhx_ff_threshold_set(&(pObj->Ctx), (asm330lhhx_ff_ths_t)Threshold) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Set free fall duration
  * @param  pObj the device pObj
  * @param  Duration free fall detection duration
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_Set_Free_Fall_Duration(ASM330LHHX_Object_t *pObj, uint8_t Duration)
{
  if (asm330lhhx_ff_dur_set(&(pObj->Ctx), Duration) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Enable wake up detection
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_Enable_Wake_Up_Detection(ASM330LHHX_Object_t *pObj, ASM330LHHX_SensorIntPin_t IntPin)
{
  int32_t ret = ASM330LHHX_OK;
  asm330lhhx_pin_int1_route_t val1;
  asm330lhhx_pin_int2_route_t val2;

  /* Output Data Rate selection */
  if (ASM330LHHX_ACC_SetOutputDataRate(pObj, 416.0f) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Full scale selection */
  if (ASM330LHHX_ACC_SetFullScale(pObj, 2) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* WAKE_DUR setting */
  if (asm330lhhx_wkup_dur_set(&(pObj->Ctx), 0x00) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Set wake up threshold. */
  if (asm330lhhx_wkup_threshold_set(&(pObj->Ctx), 0x02) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Enable wake up event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case ASM330LHHX_INT1_PIN:
      if (asm330lhhx_pin_int1_route_get(&(pObj->Ctx), &val1) != ASM330LHHX_OK)
      {
        return ASM330LHHX_ERROR;
      }

      val1.md1_cfg.int1_wu = PROPERTY_ENABLE;

      if (asm330lhhx_pin_int1_route_set(&(pObj->Ctx), &val1) != ASM330LHHX_OK)
      {
        return ASM330LHHX_ERROR;
      }
      break;

    case ASM330LHHX_INT2_PIN:
      if (asm330lhhx_pin_int2_route_get(&(pObj->Ctx), &val2) != ASM330LHHX_OK)
      {
        return ASM330LHHX_ERROR;
      }

      val2.md2_cfg.int2_wu = PROPERTY_ENABLE;

      if (asm330lhhx_pin_int2_route_set(&(pObj->Ctx), &val2) != ASM330LHHX_OK)
      {
        return ASM330LHHX_ERROR;
      }
      break;

    default:
      ret = ASM330LHHX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Disable wake up detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_Disable_Wake_Up_Detection(ASM330LHHX_Object_t *pObj)
{
  asm330lhhx_pin_int1_route_t val1;
  asm330lhhx_pin_int2_route_t val2;

  /* Disable wake up event on both INT1 and INT2 pins */
  if (asm330lhhx_pin_int1_route_get(&(pObj->Ctx), &val1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  val1.md1_cfg.int1_wu = PROPERTY_DISABLE;

  if (asm330lhhx_pin_int1_route_set(&(pObj->Ctx), &val1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  if (asm330lhhx_pin_int2_route_get(&(pObj->Ctx), &val2) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  val2.md2_cfg.int2_wu = PROPERTY_DISABLE;

  if (asm330lhhx_pin_int2_route_set(&(pObj->Ctx), &val2) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Reset wake up threshold. */
  if (asm330lhhx_wkup_threshold_set(&(pObj->Ctx), 0x00) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* WAKE_DUR setting */
  if (asm330lhhx_wkup_dur_set(&(pObj->Ctx), 0x00) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Set wake up threshold
  * @param  pObj the device pObj
  * @param  Threshold wake up detection threshold
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_Set_Wake_Up_Threshold(ASM330LHHX_Object_t *pObj, uint8_t Threshold)
{
  /* Set wake up threshold. */
  if (asm330lhhx_wkup_threshold_set(&(pObj->Ctx), Threshold) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Set wake up duration
  * @param  pObj the device pObj
  * @param  Duration wake up detection duration
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_Set_Wake_Up_Duration(ASM330LHHX_Object_t *pObj, uint8_t Duration)
{
  /* Set wake up duration. */
  if (asm330lhhx_wkup_dur_set(&(pObj->Ctx), Duration) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Enable 6D orientation detection
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_Enable_6D_Orientation(ASM330LHHX_Object_t *pObj, ASM330LHHX_SensorIntPin_t IntPin)
{
  int32_t ret = ASM330LHHX_OK;
  asm330lhhx_pin_int1_route_t val1;
  asm330lhhx_pin_int2_route_t val2;

  /* Output Data Rate selection */
  if (ASM330LHHX_ACC_SetOutputDataRate(pObj, 416.0f) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Full scale selection */
  if (ASM330LHHX_ACC_SetFullScale(pObj, 2) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* 6D orientation enabled. */
  if (asm330lhhx_6d_threshold_set(&(pObj->Ctx), ASM330LHHX_DEG_60) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Enable 6D orientation event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case ASM330LHHX_INT1_PIN:
      if (asm330lhhx_pin_int1_route_get(&(pObj->Ctx), &val1) != ASM330LHHX_OK)
      {
        return ASM330LHHX_ERROR;
      }

      val1.md1_cfg.int1_6d = PROPERTY_ENABLE;

      if (asm330lhhx_pin_int1_route_set(&(pObj->Ctx), &val1) != ASM330LHHX_OK)
      {
        return ASM330LHHX_ERROR;
      }
      break;

    case ASM330LHHX_INT2_PIN:
      if (asm330lhhx_pin_int2_route_get(&(pObj->Ctx), &val2) != ASM330LHHX_OK)
      {
        return ASM330LHHX_ERROR;
      }

      val2.md2_cfg.int2_6d = PROPERTY_ENABLE;

      if (asm330lhhx_pin_int2_route_set(&(pObj->Ctx), &val2) != ASM330LHHX_OK)
      {
        return ASM330LHHX_ERROR;
      }
      break;

    default:
      ret = ASM330LHHX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Disable 6D orientation detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_Disable_6D_Orientation(ASM330LHHX_Object_t *pObj)
{
  asm330lhhx_pin_int1_route_t val1;
  asm330lhhx_pin_int2_route_t val2;

  /* Disable 6D orientation event on both INT1 and INT2 pins */
  if (asm330lhhx_pin_int1_route_get(&(pObj->Ctx), &val1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  val1.md1_cfg.int1_6d = PROPERTY_DISABLE;

  if (asm330lhhx_pin_int1_route_set(&(pObj->Ctx), &val1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  if (asm330lhhx_pin_int2_route_get(&(pObj->Ctx), &val2) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  val2.md2_cfg.int2_6d = PROPERTY_DISABLE;

  if (asm330lhhx_pin_int2_route_set(&(pObj->Ctx), &val2) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  /* Reset 6D orientation. */
  if (asm330lhhx_6d_threshold_set(&(pObj->Ctx), ASM330LHHX_DEG_80) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Set 6D orientation threshold
  * @param  pObj the device pObj
  * @param  Threshold 6D Orientation detection threshold
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_Set_6D_Orientation_Threshold(ASM330LHHX_Object_t *pObj, uint8_t Threshold)
{
  if (asm330lhhx_6d_threshold_set(&(pObj->Ctx), (asm330lhhx_sixd_ths_t)Threshold) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Get the status of XLow orientation
  * @param  pObj the device pObj
  * @param  XLow the status of XLow orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_Get_6D_Orientation_XL(ASM330LHHX_Object_t *pObj, uint8_t *XLow)
{
  asm330lhhx_d6d_src_t data;

  if (asm330lhhx_read_reg(&(pObj->Ctx), ASM330LHHX_D6D_SRC, (uint8_t *)&data, 1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  *XLow = data.xl;

  return ASM330LHHX_OK;
}

/**
  * @brief  Get the status of XHigh orientation
  * @param  pObj the device pObj
  * @param  XHigh the status of XHigh orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_Get_6D_Orientation_XH(ASM330LHHX_Object_t *pObj, uint8_t *XHigh)
{
  asm330lhhx_d6d_src_t data;

  if (asm330lhhx_read_reg(&(pObj->Ctx), ASM330LHHX_D6D_SRC, (uint8_t *)&data, 1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  *XHigh = data.xh;

  return ASM330LHHX_OK;
}

/**
  * @brief  Get the status of YLow orientation
  * @param  pObj the device pObj
  * @param  YLow the status of YLow orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_Get_6D_Orientation_YL(ASM330LHHX_Object_t *pObj, uint8_t *YLow)
{
  asm330lhhx_d6d_src_t data;

  if (asm330lhhx_read_reg(&(pObj->Ctx), ASM330LHHX_D6D_SRC, (uint8_t *)&data, 1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  *YLow = data.yl;

  return ASM330LHHX_OK;
}

/**
  * @brief  Get the status of YHigh orientation
  * @param  pObj the device pObj
  * @param  YHigh the status of YHigh orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_Get_6D_Orientation_YH(ASM330LHHX_Object_t *pObj, uint8_t *YHigh)
{
  asm330lhhx_d6d_src_t data;

  if (asm330lhhx_read_reg(&(pObj->Ctx), ASM330LHHX_D6D_SRC, (uint8_t *)&data, 1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  *YHigh = data.yh;

  return ASM330LHHX_OK;
}

/**
  * @brief  Get the status of ZLow orientation
  * @param  pObj the device pObj
  * @param  ZLow the status of ZLow orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_Get_6D_Orientation_ZL(ASM330LHHX_Object_t *pObj, uint8_t *ZLow)
{
  asm330lhhx_d6d_src_t data;

  if (asm330lhhx_read_reg(&(pObj->Ctx), ASM330LHHX_D6D_SRC, (uint8_t *)&data, 1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  *ZLow = data.zl;

  return ASM330LHHX_OK;
}

/**
  * @brief  Get the status of ZHigh orientation
  * @param  pObj the device pObj
  * @param  ZHigh the status of ZHigh orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_Get_6D_Orientation_ZH(ASM330LHHX_Object_t *pObj, uint8_t *ZHigh)
{
  asm330lhhx_d6d_src_t data;

  if (asm330lhhx_read_reg(&(pObj->Ctx), ASM330LHHX_D6D_SRC, (uint8_t *)&data, 1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  *ZHigh = data.zh;

  return ASM330LHHX_OK;
}

/**
  * @brief  Get the ASM330LHHX ACC data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_Get_DRDY_Status(ASM330LHHX_Object_t *pObj, uint8_t *Status)
{
  if (asm330lhhx_xl_flag_data_ready_get(&(pObj->Ctx), Status) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Get the status of all hardware events
  * @param  pObj the device pObj
  * @param  Status the status of all hardware events
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_Get_Event_Status(ASM330LHHX_Object_t *pObj, ASM330LHHX_Event_Status_t *Status)
{
  asm330lhhx_wake_up_src_t wake_up_src;
  asm330lhhx_d6d_src_t d6d_src;
  asm330lhhx_md1_cfg_t md1_cfg;
  asm330lhhx_md2_cfg_t md2_cfg;
  asm330lhhx_emb_func_int1_t int1_ctrl;
  asm330lhhx_emb_func_int2_t int2_ctrl;

  (void)memset((void *)Status, 0x0, sizeof(ASM330LHHX_Event_Status_t));

  if (asm330lhhx_read_reg(&(pObj->Ctx), ASM330LHHX_WAKE_UP_SRC, (uint8_t *)&wake_up_src, 1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  if (asm330lhhx_read_reg(&(pObj->Ctx), ASM330LHHX_D6D_SRC, (uint8_t *)&d6d_src, 1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  if (asm330lhhx_mem_bank_set(&(pObj->Ctx), ASM330LHHX_EMBEDDED_FUNC_BANK) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  if (asm330lhhx_read_reg(&(pObj->Ctx), ASM330LHHX_EMB_FUNC_INT1, (uint8_t *)&int1_ctrl, 1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  if (asm330lhhx_read_reg(&(pObj->Ctx), ASM330LHHX_EMB_FUNC_INT2, (uint8_t *)&int2_ctrl, 1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  if (asm330lhhx_mem_bank_set(&(pObj->Ctx), ASM330LHHX_USER_BANK) != 0)
  {
    return ASM330LHHX_ERROR;
  }

  if (asm330lhhx_read_reg(&(pObj->Ctx), ASM330LHHX_MD1_CFG, (uint8_t *)&md1_cfg, 1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  if (asm330lhhx_read_reg(&(pObj->Ctx), ASM330LHHX_MD2_CFG, (uint8_t *)&md2_cfg, 1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
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

  if ((md1_cfg.int1_6d == 1U) || (md2_cfg.int2_6d == 1U))
  {
    if (d6d_src.d6d_ia == 1U)
    {
      Status->D6DOrientationStatus = 1;
    }
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Set self test
  * @param  pObj the device pObj
  * @param  val the value of st_xl in reg CTRL5_C
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_ACC_Set_SelfTest(ASM330LHHX_Object_t *pObj, uint8_t val)
{
  asm330lhhx_st_xl_t reg;

  reg = (val == 0U)  ? ASM330LHHX_XL_ST_DISABLE
        : (val == 1U)  ? ASM330LHHX_XL_ST_POSITIVE
        : (val == 2U)  ? ASM330LHHX_XL_ST_NEGATIVE
        :                ASM330LHHX_XL_ST_DISABLE;

  if (asm330lhhx_xl_self_test_set(&(pObj->Ctx), reg) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Get the ASM330LHHX GYRO data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_GYRO_Get_DRDY_Status(ASM330LHHX_Object_t *pObj, uint8_t *Status)
{
  if (asm330lhhx_gy_flag_data_ready_get(&(pObj->Ctx), Status) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Set self test
  * @param  pObj the device pObj
  * @param  val the value of st_xl in reg CTRL5_C
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_GYRO_Set_SelfTest(ASM330LHHX_Object_t *pObj, uint8_t val)
{
  asm330lhhx_st_g_t reg;

  reg = (val == 0U)  ? ASM330LHHX_GY_ST_DISABLE
        : (val == 1U)  ? ASM330LHHX_GY_ST_POSITIVE
        : (val == 2U)  ? ASM330LHHX_GY_ST_NEGATIVE
        :                ASM330LHHX_GY_ST_DISABLE;


  if (asm330lhhx_gy_self_test_set(&(pObj->Ctx), reg) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Get the ASM330LHHX FIFO number of samples
  * @param  pObj the device pObj
  * @param  NumSamples number of samples
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_FIFO_Get_Num_Samples(ASM330LHHX_Object_t *pObj, uint16_t *NumSamples)
{
  if (asm330lhhx_fifo_data_level_get(&(pObj->Ctx), NumSamples) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Get the ASM330LHHX FIFO full status
  * @param  pObj the device pObj
  * @param  Status FIFO full status
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_FIFO_Get_Full_Status(ASM330LHHX_Object_t *pObj, uint8_t *Status)
{
  asm330lhhx_reg_t reg;

  if (asm330lhhx_read_reg(&(pObj->Ctx), ASM330LHHX_FIFO_STATUS2, &reg.byte, 1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  *Status = reg.fifo_status2.fifo_full_ia;

  return ASM330LHHX_OK;
}

/**
  * @brief  Set the ASM330LHHX FIFO full interrupt on INT1 pin
  * @param  pObj the device pObj
  * @param  Status FIFO full interrupt on INT1 pin status
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_FIFO_Set_INT1_FIFO_Full(ASM330LHHX_Object_t *pObj, uint8_t Status)
{
  asm330lhhx_reg_t reg;

  if (asm330lhhx_read_reg(&(pObj->Ctx), ASM330LHHX_INT1_CTRL, &reg.byte, 1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  reg.int1_ctrl.int1_fifo_full = Status;

  if (asm330lhhx_write_reg(&(pObj->Ctx), ASM330LHHX_INT1_CTRL, &reg.byte, 1) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Set the ASM330LHHX FIFO watermark level
  * @param  pObj the device pObj
  * @param  Watermark FIFO watermark level
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_FIFO_Set_Watermark_Level(ASM330LHHX_Object_t *pObj, uint16_t Watermark)
{
  if (asm330lhhx_fifo_watermark_set(&(pObj->Ctx), Watermark) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Set the ASM330LHHX FIFO stop on watermark
  * @param  pObj the device pObj
  * @param  Status FIFO stop on watermark status
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_FIFO_Set_Stop_On_Fth(ASM330LHHX_Object_t *pObj, uint8_t Status)
{
  if (asm330lhhx_fifo_stop_on_wtm_set(&(pObj->Ctx), Status) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Set the ASM330LHHX FIFO mode
  * @param  pObj the device pObj
  * @param  Mode FIFO mode
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_FIFO_Set_Mode(ASM330LHHX_Object_t *pObj, uint8_t Mode)
{
  int32_t ret = ASM330LHHX_OK;

  /* Verify that the passed parameter contains one of the valid values. */
  switch ((asm330lhhx_fifo_mode_t)Mode)
  {
    case ASM330LHHX_BYPASS_MODE:
    case ASM330LHHX_FIFO_MODE:
    case ASM330LHHX_STREAM_TO_FIFO_MODE:
    case ASM330LHHX_BYPASS_TO_STREAM_MODE:
    case ASM330LHHX_STREAM_MODE:
      break;

    default:
      ret = ASM330LHHX_ERROR;
      break;
  }

  if (ret == ASM330LHHX_ERROR)
  {
    return ret;
  }

  if (asm330lhhx_fifo_mode_set(&(pObj->Ctx), (asm330lhhx_fifo_mode_t)Mode) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ret;
}

/**
  * @brief  Get the ASM330LHHX FIFO tag
  * @param  pObj the device pObj
  * @param  Tag FIFO tag
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_FIFO_Get_Tag(ASM330LHHX_Object_t *pObj, uint8_t *Tag)
{
  asm330lhhx_fifo_tag_t tag_local;

  if (asm330lhhx_fifo_sensor_tag_get(&(pObj->Ctx), &tag_local) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  *Tag = (uint8_t)tag_local;

  return ASM330LHHX_OK;
}

/**
  * @brief  Get the ASM330LHHX FIFO raw data
  * @param  pObj the device pObj
  * @param  Data FIFO raw data array [6]
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_FIFO_Get_Data(ASM330LHHX_Object_t *pObj, uint8_t *Data)
{
  if (asm330lhhx_read_reg(&(pObj->Ctx), ASM330LHHX_FIFO_DATA_OUT_X_L, Data, 6) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Get the ASM330LHHX FIFO accelero single sample (16-bit data per 3 axes) and calculate acceleration [mg]
  * @param  pObj the device pObj
  * @param  Acceleration FIFO accelero axes [mg]
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_FIFO_ACC_Get_Axes(ASM330LHHX_Object_t *pObj, ASM330LHHX_Axes_t *Acceleration)
{
  uint8_t data[6];
  int16_t data_raw[3];
  float sensitivity = 0.0f;
  float acceleration_float[3];

  if (ASM330LHHX_FIFO_Get_Data(pObj, data) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  data_raw[0] = ((int16_t)data[1] << 8) | data[0];
  data_raw[1] = ((int16_t)data[3] << 8) | data[2];
  data_raw[2] = ((int16_t)data[5] << 8) | data[4];

  if (ASM330LHHX_ACC_GetSensitivity(pObj, &sensitivity) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  acceleration_float[0] = (float)data_raw[0] * sensitivity;
  acceleration_float[1] = (float)data_raw[1] * sensitivity;
  acceleration_float[2] = (float)data_raw[2] * sensitivity;

  Acceleration->x = (int32_t)acceleration_float[0];
  Acceleration->y = (int32_t)acceleration_float[1];
  Acceleration->z = (int32_t)acceleration_float[2];

  return ASM330LHHX_OK;
}

/**
  * @brief  Set the ASM330LHHX FIFO accelero BDR value
  * @param  pObj the device pObj
  * @param  Bdr FIFO accelero BDR value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_FIFO_ACC_Set_BDR(ASM330LHHX_Object_t *pObj, float Bdr)
{
  asm330lhhx_bdr_xl_t new_bdr;

  new_bdr = (Bdr <=    0.0f) ? ASM330LHHX_XL_NOT_BATCHED
            : (Bdr <=   12.5f) ? ASM330LHHX_XL_BATCHED_AT_12Hz5
            : (Bdr <=   26.0f) ? ASM330LHHX_XL_BATCHED_AT_26Hz
            : (Bdr <=   52.0f) ? ASM330LHHX_XL_BATCHED_AT_52Hz
            : (Bdr <=  104.0f) ? ASM330LHHX_XL_BATCHED_AT_104Hz
            : (Bdr <=  208.0f) ? ASM330LHHX_XL_BATCHED_AT_208Hz
            : (Bdr <=  416.0f) ? ASM330LHHX_XL_BATCHED_AT_417Hz
            : (Bdr <=  833.0f) ? ASM330LHHX_XL_BATCHED_AT_833Hz
            : (Bdr <= 1660.0f) ? ASM330LHHX_XL_BATCHED_AT_1667Hz
            : (Bdr <= 3330.0f) ? ASM330LHHX_XL_BATCHED_AT_3333Hz
            :                    ASM330LHHX_XL_BATCHED_AT_6667Hz;

  if (asm330lhhx_fifo_xl_batch_set(&(pObj->Ctx), new_bdr) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Get the ASM330LHHX FIFO gyro single sample (16-bit data per 3 axes) and calculate angular velocity [mDPS]
  * @param  pObj the device pObj
  * @param  AngularVelocity FIFO gyro axes [mDPS]
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_FIFO_GYRO_Get_Axes(ASM330LHHX_Object_t *pObj, ASM330LHHX_Axes_t *AngularVelocity)
{
  uint8_t data[6];
  int16_t data_raw[3];
  float sensitivity = 0.0f;
  float angular_velocity_float[3];

  if (ASM330LHHX_FIFO_Get_Data(pObj, data) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  data_raw[0] = ((int16_t)data[1] << 8) | data[0];
  data_raw[1] = ((int16_t)data[3] << 8) | data[2];
  data_raw[2] = ((int16_t)data[5] << 8) | data[4];

  if (ASM330LHHX_GYRO_GetSensitivity(pObj, &sensitivity) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  angular_velocity_float[0] = (float)data_raw[0] * sensitivity;
  angular_velocity_float[1] = (float)data_raw[1] * sensitivity;
  angular_velocity_float[2] = (float)data_raw[2] * sensitivity;

  AngularVelocity->x = (int32_t)angular_velocity_float[0];
  AngularVelocity->y = (int32_t)angular_velocity_float[1];
  AngularVelocity->z = (int32_t)angular_velocity_float[2];

  return ASM330LHHX_OK;
}

/**
  * @brief  Set the ASM330LHHX FIFO gyro BDR value
  * @param  pObj the device pObj
  * @param  Bdr FIFO gyro BDR value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHHX_FIFO_GYRO_Set_BDR(ASM330LHHX_Object_t *pObj, float Bdr)
{
  asm330lhhx_bdr_gy_t new_bdr;

  new_bdr = (Bdr <=    0.0f) ? ASM330LHHX_GY_NOT_BATCHED
            : (Bdr <=   12.5f) ? ASM330LHHX_GY_BATCHED_AT_12Hz5
            : (Bdr <=   26.0f) ? ASM330LHHX_GY_BATCHED_AT_26Hz
            : (Bdr <=   52.0f) ? ASM330LHHX_GY_BATCHED_AT_52Hz
            : (Bdr <=  104.0f) ? ASM330LHHX_GY_BATCHED_AT_104Hz
            : (Bdr <=  208.0f) ? ASM330LHHX_GY_BATCHED_AT_208Hz
            : (Bdr <=  416.0f) ? ASM330LHHX_GY_BATCHED_AT_417Hz
            : (Bdr <=  833.0f) ? ASM330LHHX_GY_BATCHED_AT_833Hz
            : (Bdr <= 1660.0f) ? ASM330LHHX_GY_BATCHED_AT_1667Hz
            : (Bdr <= 3330.0f) ? ASM330LHHX_GY_BATCHED_AT_3333Hz
            :                    ASM330LHHX_GY_BATCHED_AT_6667Hz;

  if (asm330lhhx_fifo_gy_batch_set(&(pObj->Ctx), new_bdr) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @}
  */

/** @defgroup ASM330LHHX_Private_Functions ASM330LHHX Private Functions
  * @{
  */

/**
  * @brief  Set the ASM330LHHX accelerometer sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t ASM330LHHX_ACC_SetOutputDataRate_When_Enabled(ASM330LHHX_Object_t *pObj, float Odr)
{
  asm330lhhx_odr_xl_t new_odr;

  new_odr = (Odr <=   12.5f) ? ASM330LHHX_XL_ODR_12Hz5
            : (Odr <=   26.0f) ? ASM330LHHX_XL_ODR_26Hz
            : (Odr <=   52.0f) ? ASM330LHHX_XL_ODR_52Hz
            : (Odr <=  104.0f) ? ASM330LHHX_XL_ODR_104Hz
            : (Odr <=  208.0f) ? ASM330LHHX_XL_ODR_208Hz
            : (Odr <=  416.0f) ? ASM330LHHX_XL_ODR_417Hz
            : (Odr <=  833.0f) ? ASM330LHHX_XL_ODR_833Hz
            : (Odr <= 1666.0f) ? ASM330LHHX_XL_ODR_1667Hz
            : (Odr <= 3332.0f) ? ASM330LHHX_XL_ODR_3333Hz
            :                    ASM330LHHX_XL_ODR_6667Hz;

  /* Output data rate selection. */
  if (asm330lhhx_xl_data_rate_set(&(pObj->Ctx), new_odr) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Set the ASM330LHHX accelerometer sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t ASM330LHHX_ACC_SetOutputDataRate_When_Disabled(ASM330LHHX_Object_t *pObj, float Odr)
{
  pObj->acc_odr = (Odr <=   12.5f) ? ASM330LHHX_XL_ODR_12Hz5
                  : (Odr <=   26.0f) ? ASM330LHHX_XL_ODR_26Hz
                  : (Odr <=   52.0f) ? ASM330LHHX_XL_ODR_52Hz
                  : (Odr <=  104.0f) ? ASM330LHHX_XL_ODR_104Hz
                  : (Odr <=  208.0f) ? ASM330LHHX_XL_ODR_208Hz
                  : (Odr <=  416.0f) ? ASM330LHHX_XL_ODR_417Hz
                  : (Odr <=  833.0f) ? ASM330LHHX_XL_ODR_833Hz
                  : (Odr <= 1666.0f) ? ASM330LHHX_XL_ODR_1667Hz
                  : (Odr <= 3332.0f) ? ASM330LHHX_XL_ODR_3333Hz
                  :                    ASM330LHHX_XL_ODR_6667Hz;

  return ASM330LHHX_OK;
}

/**
  * @brief  Set the ASM330LHHX gyroscope sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t ASM330LHHX_GYRO_SetOutputDataRate_When_Enabled(ASM330LHHX_Object_t *pObj, float Odr)
{
  asm330lhhx_odr_g_t new_odr;

  new_odr = (Odr <=   12.5f) ? ASM330LHHX_GY_ODR_12Hz5
            : (Odr <=   26.0f) ? ASM330LHHX_GY_ODR_26Hz
            : (Odr <=   52.0f) ? ASM330LHHX_GY_ODR_52Hz
            : (Odr <=  104.0f) ? ASM330LHHX_GY_ODR_104Hz
            : (Odr <=  208.0f) ? ASM330LHHX_GY_ODR_208Hz
            : (Odr <=  416.0f) ? ASM330LHHX_GY_ODR_417Hz
            : (Odr <=  833.0f) ? ASM330LHHX_GY_ODR_833Hz
            : (Odr <= 1666.0f) ? ASM330LHHX_GY_ODR_1667Hz
            : (Odr <= 3332.0f) ? ASM330LHHX_GY_ODR_3333Hz
            :                    ASM330LHHX_GY_ODR_6667Hz;

  /* Output data rate selection. */
  if (asm330lhhx_gy_data_rate_set(&(pObj->Ctx), new_odr) != ASM330LHHX_OK)
  {
    return ASM330LHHX_ERROR;
  }

  return ASM330LHHX_OK;
}

/**
  * @brief  Set the ASM330LHHX gyroscope sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t ASM330LHHX_GYRO_SetOutputDataRate_When_Disabled(ASM330LHHX_Object_t *pObj, float Odr)
{
  pObj->gyro_odr = (Odr <=   12.5f) ? ASM330LHHX_GY_ODR_12Hz5
                   : (Odr <=   26.0f) ? ASM330LHHX_GY_ODR_26Hz
                   : (Odr <=   52.0f) ? ASM330LHHX_GY_ODR_52Hz
                   : (Odr <=  104.0f) ? ASM330LHHX_GY_ODR_104Hz
                   : (Odr <=  208.0f) ? ASM330LHHX_GY_ODR_208Hz
                   : (Odr <=  416.0f) ? ASM330LHHX_GY_ODR_417Hz
                   : (Odr <=  833.0f) ? ASM330LHHX_GY_ODR_833Hz
                   : (Odr <= 1666.0f) ? ASM330LHHX_GY_ODR_1667Hz
                   : (Odr <= 3332.0f) ? ASM330LHHX_GY_ODR_3333Hz
                   :                    ASM330LHHX_GY_ODR_6667Hz;

  return ASM330LHHX_OK;
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
  ASM330LHHX_Object_t *pObj = (ASM330LHHX_Object_t *)Handle;

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
  ASM330LHHX_Object_t *pObj = (ASM330LHHX_Object_t *)Handle;

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
