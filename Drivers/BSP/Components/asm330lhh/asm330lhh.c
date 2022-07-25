/**
  ******************************************************************************
  * @file    asm330lhh.c
  * @author  MEMS Software Solutions Team
  * @brief   ASM330LHH driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "asm330lhh.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup ASM330LHH ASM330LHH
  * @{
  */

/** @defgroup ASM330LHH_Exported_Variables ASM330LHH Exported Variables
  * @{
  */

ASM330LHH_CommonDrv_t ASM330LHH_COMMON_Driver =
{
  ASM330LHH_Init,
  ASM330LHH_DeInit,
  ASM330LHH_ReadID,
  ASM330LHH_GetCapabilities,
};

ASM330LHH_ACC_Drv_t ASM330LHH_ACC_Driver =
{
  ASM330LHH_ACC_Enable,
  ASM330LHH_ACC_Disable,
  ASM330LHH_ACC_GetSensitivity,
  ASM330LHH_ACC_GetOutputDataRate,
  ASM330LHH_ACC_SetOutputDataRate,
  ASM330LHH_ACC_GetFullScale,
  ASM330LHH_ACC_SetFullScale,
  ASM330LHH_ACC_GetAxes,
  ASM330LHH_ACC_GetAxesRaw,
};

ASM330LHH_GYRO_Drv_t ASM330LHH_GYRO_Driver =
{
  ASM330LHH_GYRO_Enable,
  ASM330LHH_GYRO_Disable,
  ASM330LHH_GYRO_GetSensitivity,
  ASM330LHH_GYRO_GetOutputDataRate,
  ASM330LHH_GYRO_SetOutputDataRate,
  ASM330LHH_GYRO_GetFullScale,
  ASM330LHH_GYRO_SetFullScale,
  ASM330LHH_GYRO_GetAxes,
  ASM330LHH_GYRO_GetAxesRaw,
};

/**
  * @}
  */

/** @defgroup ASM330LHH_Private_Function_Prototypes ASM330LHH Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t ASM330LHH_ACC_SetOutputDataRate_When_Enabled(ASM330LHH_Object_t *pObj, float Odr);
static int32_t ASM330LHH_ACC_SetOutputDataRate_When_Disabled(ASM330LHH_Object_t *pObj, float Odr);
static int32_t ASM330LHH_GYRO_SetOutputDataRate_When_Enabled(ASM330LHH_Object_t *pObj, float Odr);
static int32_t ASM330LHH_GYRO_SetOutputDataRate_When_Disabled(ASM330LHH_Object_t *pObj, float Odr);

/**
  * @}
  */

/** @defgroup ASM330LHH_Exported_Functions ASM330LHH Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_RegisterBusIO(ASM330LHH_Object_t *pObj, ASM330LHH_IO_t *pIO)
{
  int32_t ret = ASM330LHH_OK;

  if (pObj == NULL)
  {
    ret = ASM330LHH_ERROR;
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
      ret = ASM330LHH_ERROR;
    }
    else if (pObj->IO.Init() != ASM330LHH_OK)
    {
      ret = ASM330LHH_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == ASM330LHH_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x0C;

          if (ASM330LHH_Write_Reg(pObj, ASM330LHH_CTRL3_C, data) != ASM330LHH_OK)
          {
            ret = ASM330LHH_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  Initialize the ASM330LHH sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_Init(ASM330LHH_Object_t *pObj)
{
  /* Set DEVICE_CONF bit */
  if (asm330lhh_device_conf_set(&(pObj->Ctx), PROPERTY_ENABLE) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  /* Enable register address automatically incremented during a multiple byte
  access with a serial interface. */
  if (asm330lhh_auto_increment_set(&(pObj->Ctx), PROPERTY_ENABLE) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  /* Enable BDU */
  if (asm330lhh_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  /* FIFO mode selection */
  if (asm330lhh_fifo_mode_set(&(pObj->Ctx), ASM330LHH_BYPASS_MODE) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = ASM330LHH_XL_ODR_104Hz;

  /* Output data rate selection - power down. */
  if (asm330lhh_xl_data_rate_set(&(pObj->Ctx), ASM330LHH_XL_ODR_OFF) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  /* Full scale selection. */
  if (asm330lhh_xl_full_scale_set(&(pObj->Ctx), ASM330LHH_2g) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  /* Select default output data rate. */
  pObj->gyro_odr = ASM330LHH_GY_ODR_104Hz;

  /* Output data rate selection - power down. */
  if (asm330lhh_gy_data_rate_set(&(pObj->Ctx), ASM330LHH_GY_ODR_OFF) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  /* Full scale selection. */
  if (asm330lhh_gy_full_scale_set(&(pObj->Ctx), ASM330LHH_2000dps) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  pObj->is_initialized = 1;

  return ASM330LHH_OK;
}

/**
  * @brief  Deinitialize the ASM330LHH sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_DeInit(ASM330LHH_Object_t *pObj)
{
  /* Disable the component */
  if (ASM330LHH_ACC_Disable(pObj) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  if (ASM330LHH_GYRO_Disable(pObj) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  /* Reset output data rate. */
  pObj->acc_odr = ASM330LHH_XL_ODR_OFF;
  pObj->gyro_odr = ASM330LHH_GY_ODR_OFF;

  pObj->is_initialized = 0;

  return ASM330LHH_OK;
}

/**
  * @brief  Read component ID
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_ReadID(ASM330LHH_Object_t *pObj, uint8_t *Id)
{
  if (asm330lhh_device_id_get(&(pObj->Ctx), Id) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  return ASM330LHH_OK;
}

/**
  * @brief  Get ASM330LHH sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to ASM330LHH sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_GetCapabilities(ASM330LHH_Object_t *pObj, ASM330LHH_Capabilities_t *Capabilities)
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
  Capabilities->GyroMaxOdr   = 6660.0f;
  Capabilities->AccMaxOdr    = 6660.0f;
  Capabilities->MagMaxOdr    = 0.0f;
  return ASM330LHH_OK;
}

/**
  * @brief  Enable the ASM330LHH accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_ACC_Enable(ASM330LHH_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return ASM330LHH_OK;
  }

  /* Output data rate selection. */
  if (asm330lhh_xl_data_rate_set(&(pObj->Ctx), pObj->acc_odr) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return ASM330LHH_OK;
}

/**
  * @brief  Disable the ASM330LHH accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_ACC_Disable(ASM330LHH_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return ASM330LHH_OK;
  }

  /* Get current output data rate. */
  if (asm330lhh_xl_data_rate_get(&(pObj->Ctx), &pObj->acc_odr) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  /* Output data rate selection - power down. */
  if (asm330lhh_xl_data_rate_set(&(pObj->Ctx), ASM330LHH_XL_ODR_OFF) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return ASM330LHH_OK;
}

/**
  * @brief  Get the ASM330LHH accelerometer sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_ACC_GetSensitivity(ASM330LHH_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = ASM330LHH_OK;
  asm330lhh_fs_xl_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (asm330lhh_xl_full_scale_get(&(pObj->Ctx), &full_scale) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  /* Store the Sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case ASM330LHH_2g:
      *Sensitivity = ASM330LHH_ACC_SENSITIVITY_FS_2G;
      break;

    case ASM330LHH_4g:
      *Sensitivity = ASM330LHH_ACC_SENSITIVITY_FS_4G;
      break;

    case ASM330LHH_8g:
      *Sensitivity = ASM330LHH_ACC_SENSITIVITY_FS_8G;
      break;

    case ASM330LHH_16g:
      *Sensitivity = ASM330LHH_ACC_SENSITIVITY_FS_16G;
      break;

    default:
      ret = ASM330LHH_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the ASM330LHH accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_ACC_GetOutputDataRate(ASM330LHH_Object_t *pObj, float *Odr)
{
  int32_t ret = ASM330LHH_OK;
  asm330lhh_odr_xl_t odr_low_level;

  /* Get current output data rate. */
  if (asm330lhh_xl_data_rate_get(&(pObj->Ctx), &odr_low_level) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  switch (odr_low_level)
  {
    case ASM330LHH_XL_ODR_OFF:
      *Odr = 0.0f;
      break;

    case ASM330LHH_XL_ODR_12Hz5:
      *Odr = 12.5f;
      break;

    case ASM330LHH_XL_ODR_26Hz:
      *Odr = 26.0f;
      break;

    case ASM330LHH_XL_ODR_52Hz:
      *Odr = 52.0f;
      break;

    case ASM330LHH_XL_ODR_104Hz:
      *Odr = 104.0f;
      break;

    case ASM330LHH_XL_ODR_208Hz:
      *Odr = 208.0f;
      break;

    case ASM330LHH_XL_ODR_416Hz:
      *Odr = 416.0f;
      break;

    case ASM330LHH_XL_ODR_833Hz:
      *Odr = 833.0f;
      break;

    case ASM330LHH_XL_ODR_1667Hz:
      *Odr = 1667.0f;
      break;

    case ASM330LHH_XL_ODR_3333Hz:
      *Odr = 3333.0f;
      break;

    case ASM330LHH_XL_ODR_6667Hz:
      *Odr = 6667.0f;
      break;

    default:
      ret = ASM330LHH_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the ASM330LHH accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_ACC_SetOutputDataRate(ASM330LHH_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return ASM330LHH_ACC_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return ASM330LHH_ACC_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the ASM330LHH accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_ACC_GetFullScale(ASM330LHH_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = ASM330LHH_OK;
  asm330lhh_fs_xl_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (asm330lhh_xl_full_scale_get(&(pObj->Ctx), &fs_low_level) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  switch (fs_low_level)
  {
    case ASM330LHH_2g:
      *FullScale =  2;
      break;

    case ASM330LHH_4g:
      *FullScale =  4;
      break;

    case ASM330LHH_8g:
      *FullScale =  8;
      break;

    case ASM330LHH_16g:
      *FullScale = 16;
      break;

    default:
      ret = ASM330LHH_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the ASM330LHH accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_ACC_SetFullScale(ASM330LHH_Object_t *pObj, int32_t FullScale)
{
  asm330lhh_fs_xl_t new_fs;

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file statical analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  new_fs = (FullScale <= 2) ? ASM330LHH_2g
           : (FullScale <= 4) ? ASM330LHH_4g
           : (FullScale <= 8) ? ASM330LHH_8g
           :                    ASM330LHH_16g;

  if (asm330lhh_xl_full_scale_set(&(pObj->Ctx), new_fs) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  return ASM330LHH_OK;
}

/**
  * @brief  Get the ASM330LHH accelerometer sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_ACC_GetAxesRaw(ASM330LHH_Object_t *pObj, ASM330LHH_AxesRaw_t *Value)
{
  asm330lhh_axis3bit16_t data_raw;

  /* Read raw data values. */
  if (asm330lhh_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return ASM330LHH_OK;
}

/**
  * @brief  Get the ASM330LHH accelerometer sensor axes
  * @param  pObj the device pObj
  * @param  Acceleration pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_ACC_GetAxes(ASM330LHH_Object_t *pObj, ASM330LHH_Axes_t *Acceleration)
{
  asm330lhh_axis3bit16_t data_raw;
  float sensitivity = 0.0f;

  /* Read raw data values. */
  if (asm330lhh_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  /* Get ASM330LHH actual sensitivity. */
  if (ASM330LHH_ACC_GetSensitivity(pObj, &sensitivity) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  /* Calculate the data. */
  Acceleration->x = (int32_t)((float)((float)data_raw.i16bit[0] * sensitivity));
  Acceleration->y = (int32_t)((float)((float)data_raw.i16bit[1] * sensitivity));
  Acceleration->z = (int32_t)((float)((float)data_raw.i16bit[2] * sensitivity));

  return ASM330LHH_OK;
}

/**
  * @brief  Enable the ASM330LHH gyroscope sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_GYRO_Enable(ASM330LHH_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->gyro_is_enabled == 1U)
  {
    return ASM330LHH_OK;
  }

  /* Output data rate selection. */
  if (asm330lhh_gy_data_rate_set(&(pObj->Ctx), pObj->gyro_odr) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  pObj->gyro_is_enabled = 1;

  return ASM330LHH_OK;
}

/**
  * @brief  Disable the ASM330LHH gyroscope sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_GYRO_Disable(ASM330LHH_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->gyro_is_enabled == 0U)
  {
    return ASM330LHH_OK;
  }

  /* Get current output data rate. */
  if (asm330lhh_gy_data_rate_get(&(pObj->Ctx), &pObj->gyro_odr) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  /* Output data rate selection - power down. */
  if (asm330lhh_gy_data_rate_set(&(pObj->Ctx), ASM330LHH_GY_ODR_OFF) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  pObj->gyro_is_enabled = 0;

  return ASM330LHH_OK;
}

/**
  * @brief  Get the ASM330LHH gyroscope sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_GYRO_GetSensitivity(ASM330LHH_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = ASM330LHH_OK;
  asm330lhh_fs_g_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (asm330lhh_gy_full_scale_get(&(pObj->Ctx), &full_scale) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  /* Store the sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case ASM330LHH_125dps:
      *Sensitivity = ASM330LHH_GYRO_SENSITIVITY_FS_125DPS;
      break;

    case ASM330LHH_250dps:
      *Sensitivity = ASM330LHH_GYRO_SENSITIVITY_FS_250DPS;
      break;

    case ASM330LHH_500dps:
      *Sensitivity = ASM330LHH_GYRO_SENSITIVITY_FS_500DPS;
      break;

    case ASM330LHH_1000dps:
      *Sensitivity = ASM330LHH_GYRO_SENSITIVITY_FS_1000DPS;
      break;

    case ASM330LHH_2000dps:
      *Sensitivity = ASM330LHH_GYRO_SENSITIVITY_FS_2000DPS;
      break;

    default:
      ret = ASM330LHH_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the ASM330LHH gyroscope sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_GYRO_GetOutputDataRate(ASM330LHH_Object_t *pObj, float *Odr)
{
  int32_t ret = ASM330LHH_OK;
  asm330lhh_odr_g_t odr_low_level;

  /* Get current output data rate. */
  if (asm330lhh_gy_data_rate_get(&(pObj->Ctx), &odr_low_level) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  switch (odr_low_level)
  {
    case ASM330LHH_GY_ODR_OFF:
      *Odr = 0.0f;
      break;

    case ASM330LHH_GY_ODR_12Hz5:
      *Odr = 12.5f;
      break;

    case ASM330LHH_GY_ODR_26Hz:
      *Odr = 26.0f;
      break;

    case ASM330LHH_GY_ODR_52Hz:
      *Odr = 52.0f;
      break;

    case ASM330LHH_GY_ODR_104Hz:
      *Odr = 104.0f;
      break;

    case ASM330LHH_GY_ODR_208Hz:
      *Odr = 208.0f;
      break;

    case ASM330LHH_GY_ODR_416Hz:
      *Odr = 416.0f;
      break;

    case ASM330LHH_GY_ODR_833Hz:
      *Odr = 833.0f;
      break;

    case ASM330LHH_GY_ODR_1667Hz:
      *Odr =  1667.0f;
      break;

    case ASM330LHH_GY_ODR_3333Hz:
      *Odr =  3333.0f;
      break;

    case ASM330LHH_GY_ODR_6667Hz:
      *Odr =  6667.0f;
      break;

    default:
      ret = ASM330LHH_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the ASM330LHH gyroscope sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_GYRO_SetOutputDataRate(ASM330LHH_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->gyro_is_enabled == 1U)
  {
    return ASM330LHH_GYRO_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return ASM330LHH_GYRO_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the ASM330LHH gyroscope sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_GYRO_GetFullScale(ASM330LHH_Object_t *pObj, int32_t  *FullScale)
{
  int32_t ret = ASM330LHH_OK;
  asm330lhh_fs_g_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (asm330lhh_gy_full_scale_get(&(pObj->Ctx), &fs_low_level) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  switch (fs_low_level)
  {
    case ASM330LHH_125dps:
      *FullScale =  125;
      break;

    case ASM330LHH_250dps:
      *FullScale =  250;
      break;

    case ASM330LHH_500dps:
      *FullScale =  500;
      break;

    case ASM330LHH_1000dps:
      *FullScale = 1000;
      break;

    case ASM330LHH_2000dps:
      *FullScale = 2000;
      break;

    case ASM330LHH_4000dps:
      *FullScale = 4000;
      break;

    default:
      ret = ASM330LHH_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the ASM330LHH gyroscope sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_GYRO_SetFullScale(ASM330LHH_Object_t *pObj, int32_t FullScale)
{
  asm330lhh_fs_g_t new_fs;

  new_fs = (FullScale <= 125)  ? ASM330LHH_125dps
           : (FullScale <= 250)  ? ASM330LHH_250dps
           : (FullScale <= 500)  ? ASM330LHH_500dps
           : (FullScale <= 1000) ? ASM330LHH_1000dps
           : (FullScale <= 2000) ? ASM330LHH_2000dps
           :                       ASM330LHH_4000dps;

  if (asm330lhh_gy_full_scale_set(&(pObj->Ctx), new_fs) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  return ASM330LHH_OK;
}

/**
  * @brief  Get the ASM330LHH gyroscope sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_GYRO_GetAxesRaw(ASM330LHH_Object_t *pObj, ASM330LHH_AxesRaw_t *Value)
{
  asm330lhh_axis3bit16_t data_raw;

  /* Read raw data values. */
  if (asm330lhh_angular_rate_raw_get(&(pObj->Ctx), data_raw.i16bit) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return ASM330LHH_OK;
}

/**
  * @brief  Get the ASM330LHH gyroscope sensor axes
  * @param  pObj the device pObj
  * @param  AngularRate pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_GYRO_GetAxes(ASM330LHH_Object_t *pObj, ASM330LHH_Axes_t *AngularRate)
{
  asm330lhh_axis3bit16_t data_raw;
  float sensitivity;

  /* Read raw data values. */
  if (asm330lhh_angular_rate_raw_get(&(pObj->Ctx), data_raw.i16bit) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  /* Get ASM330LHH actual sensitivity. */
  if (ASM330LHH_GYRO_GetSensitivity(pObj, &sensitivity) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  /* Calculate the data. */
  AngularRate->x = (int32_t)((float)((float)data_raw.i16bit[0] * sensitivity));
  AngularRate->y = (int32_t)((float)((float)data_raw.i16bit[1] * sensitivity));
  AngularRate->z = (int32_t)((float)((float)data_raw.i16bit[2] * sensitivity));

  return ASM330LHH_OK;
}

/**
  * @brief  Get the ASM330LHH register value
  * @param  pObj the device pObj
  * @param  Reg address to be read
  * @param  Data pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_Read_Reg(ASM330LHH_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (asm330lhh_read_reg(&(pObj->Ctx), Reg, Data, 1) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  return ASM330LHH_OK;
}

/**
  * @brief  Set the ASM330LHH register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_Write_Reg(ASM330LHH_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (asm330lhh_write_reg(&(pObj->Ctx), Reg, &Data, 1) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  return ASM330LHH_OK;
}

/**
  * @brief  Get the ASM330LHH ACC data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_ACC_Get_DRDY_Status(ASM330LHH_Object_t *pObj, uint8_t *Status)
{
  if (asm330lhh_xl_flag_data_ready_get(&(pObj->Ctx), Status) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  return ASM330LHH_OK;
}

/**
  * @brief  Get the ASM330LHH GYRO data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ASM330LHH_GYRO_Get_DRDY_Status(ASM330LHH_Object_t *pObj, uint8_t *Status)
{
  if (asm330lhh_gy_flag_data_ready_get(&(pObj->Ctx), Status) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  return ASM330LHH_OK;
}

/**
  * @}
  */

/** @defgroup ASM330LHH_Private_Functions ASM330LHH Private Functions
  * @{
  */

/**
  * @brief  Set the ASM330LHH accelerometer sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t ASM330LHH_ACC_SetOutputDataRate_When_Enabled(ASM330LHH_Object_t *pObj, float Odr)
{
  asm330lhh_odr_xl_t new_odr;

  new_odr = (Odr <=   12.5f) ? ASM330LHH_XL_ODR_12Hz5
            : (Odr <=   26.0f) ? ASM330LHH_XL_ODR_26Hz
            : (Odr <=   52.0f) ? ASM330LHH_XL_ODR_52Hz
            : (Odr <=  104.0f) ? ASM330LHH_XL_ODR_104Hz
            : (Odr <=  208.0f) ? ASM330LHH_XL_ODR_208Hz
            : (Odr <=  416.0f) ? ASM330LHH_XL_ODR_416Hz
            : (Odr <=  833.0f) ? ASM330LHH_XL_ODR_833Hz
            : (Odr <= 1667.0f) ? ASM330LHH_XL_ODR_1667Hz
            : (Odr <= 3333.0f) ? ASM330LHH_XL_ODR_3333Hz
            :                    ASM330LHH_XL_ODR_6667Hz;

  /* Output data rate selection. */
  if (asm330lhh_xl_data_rate_set(&(pObj->Ctx), new_odr) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  return ASM330LHH_OK;
}

/**
  * @brief  Set the ASM330LHH accelerometer sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t ASM330LHH_ACC_SetOutputDataRate_When_Disabled(ASM330LHH_Object_t *pObj, float Odr)
{
  pObj->acc_odr = (Odr <=   12.5f) ? ASM330LHH_XL_ODR_12Hz5
                  : (Odr <=   26.0f) ? ASM330LHH_XL_ODR_26Hz
                  : (Odr <=   52.0f) ? ASM330LHH_XL_ODR_52Hz
                  : (Odr <=  104.0f) ? ASM330LHH_XL_ODR_104Hz
                  : (Odr <=  208.0f) ? ASM330LHH_XL_ODR_208Hz
                  : (Odr <=  416.0f) ? ASM330LHH_XL_ODR_416Hz
                  : (Odr <=  833.0f) ? ASM330LHH_XL_ODR_833Hz
                  : (Odr <= 1667.0f) ? ASM330LHH_XL_ODR_1667Hz
                  : (Odr <= 3333.0f) ? ASM330LHH_XL_ODR_3333Hz
                  :                    ASM330LHH_XL_ODR_6667Hz;

  return ASM330LHH_OK;
}

/**
  * @brief  Set the ASM330LHH gyroscope sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t ASM330LHH_GYRO_SetOutputDataRate_When_Enabled(ASM330LHH_Object_t *pObj, float Odr)
{
  asm330lhh_odr_g_t new_odr;

  new_odr = (Odr <=   12.5f) ? ASM330LHH_GY_ODR_12Hz5
            : (Odr <=   26.0f) ? ASM330LHH_GY_ODR_26Hz
            : (Odr <=   52.0f) ? ASM330LHH_GY_ODR_52Hz
            : (Odr <=  104.0f) ? ASM330LHH_GY_ODR_104Hz
            : (Odr <=  208.0f) ? ASM330LHH_GY_ODR_208Hz
            : (Odr <=  416.0f) ? ASM330LHH_GY_ODR_416Hz
            : (Odr <=  833.0f) ? ASM330LHH_GY_ODR_833Hz
            : (Odr <= 1667.0f) ? ASM330LHH_GY_ODR_1667Hz
            : (Odr <= 3333.0f) ? ASM330LHH_GY_ODR_3333Hz
            :                    ASM330LHH_GY_ODR_6667Hz;

  /* Output data rate selection. */
  if (asm330lhh_gy_data_rate_set(&(pObj->Ctx), new_odr) != ASM330LHH_OK)
  {
    return ASM330LHH_ERROR;
  }

  return ASM330LHH_OK;
}

/**
  * @brief  Set the ASM330LHH gyroscope sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t ASM330LHH_GYRO_SetOutputDataRate_When_Disabled(ASM330LHH_Object_t *pObj, float Odr)
{
  pObj->gyro_odr = (Odr <=   12.5f) ? ASM330LHH_GY_ODR_12Hz5
                   : (Odr <=   26.0f) ? ASM330LHH_GY_ODR_26Hz
                   : (Odr <=   52.0f) ? ASM330LHH_GY_ODR_52Hz
                   : (Odr <=  104.0f) ? ASM330LHH_GY_ODR_104Hz
                   : (Odr <=  208.0f) ? ASM330LHH_GY_ODR_208Hz
                   : (Odr <=  416.0f) ? ASM330LHH_GY_ODR_416Hz
                   : (Odr <=  833.0f) ? ASM330LHH_GY_ODR_833Hz
                   : (Odr <= 1667.0f) ? ASM330LHH_GY_ODR_1667Hz
                   : (Odr <= 3333.0f) ? ASM330LHH_GY_ODR_3333Hz
                   :                    ASM330LHH_GY_ODR_6667Hz;

  return ASM330LHH_OK;
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
  ASM330LHH_Object_t *pObj = (ASM330LHH_Object_t *)Handle;

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
  ASM330LHH_Object_t *pObj = (ASM330LHH_Object_t *)Handle;

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
