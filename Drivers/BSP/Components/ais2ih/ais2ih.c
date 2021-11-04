/**
 ******************************************************************************
 * @file    ais2ih.c
 * @author  MEMS Software Solutions Team
 * @brief   AIS2IH driver file
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "ais2ih.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @defgroup AIS2IH AIS2IH
 * @{
 */

/** @defgroup AIS2IH_Exported_Variables AIS2IH Exported Variables
 * @{
 */

AIS2IH_CommonDrv_t AIS2IH_COMMON_Driver =
{
  AIS2IH_Init,
  AIS2IH_DeInit,
  AIS2IH_ReadID,
  AIS2IH_GetCapabilities,
};

AIS2IH_ACC_Drv_t AIS2IH_ACC_Driver =
{
  AIS2IH_ACC_Enable,
  AIS2IH_ACC_Disable,
  AIS2IH_ACC_GetSensitivity,
  AIS2IH_ACC_GetOutputDataRate,
  AIS2IH_ACC_SetOutputDataRate,
  AIS2IH_ACC_GetFullScale,
  AIS2IH_ACC_SetFullScale,
  AIS2IH_ACC_GetAxes,
  AIS2IH_ACC_GetAxesRaw,
};

/**
 * @}
 */

/** @defgroup AIS2IH_Private_Function_Prototypes AIS2IH Private Function Prototypes
 * @{
 */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t AIS2IH_ACC_SetOutputDataRate_When_Enabled(AIS2IH_Object_t *pObj, float Odr, AIS2IH_Operating_Mode_t Mode, AIS2IH_Low_Noise_t Noise);
static int32_t AIS2IH_ACC_SetOutputDataRate_When_Disabled(AIS2IH_Object_t *pObj, float Odr, AIS2IH_Operating_Mode_t Mode, AIS2IH_Low_Noise_t Noise);

/**
 * @}
 */

/** @defgroup AIS2IH_Exported_Functions AIS2IH Exported Functions
 * @{
 */

/**
 * @brief  Register Component Bus IO operations
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_RegisterBusIO(AIS2IH_Object_t *pObj, AIS2IH_IO_t *pIO)
{
  int32_t ret = AIS2IH_OK;

  if (pObj == NULL)
  {
    ret = AIS2IH_ERROR;
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
      ret = AIS2IH_ERROR;
    }
    else if (pObj->IO.Init() != AIS2IH_OK)
    {
      ret = AIS2IH_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == AIS2IH_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x05;

          if (AIS2IH_Write_Reg(pObj, AIS2IH_CTRL2, data) != AIS2IH_OK)
          {
            ret = AIS2IH_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
 * @brief  Initialize the AIS2IH sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_Init(AIS2IH_Object_t *pObj)
{
  /* Enable register address automatically incremented during a multiple byte
  access with a serial interface. */
  if (ais2ih_auto_increment_set(&(pObj->Ctx), PROPERTY_ENABLE) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* Enable BDU */
  if (ais2ih_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* FIFO mode selection */
  if (ais2ih_fifo_mode_set(&(pObj->Ctx), AIS2IH_BYPASS_MODE) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* Power mode selection */
  if (ais2ih_power_mode_set(&(pObj->Ctx), AIS2IH_HIGH_PERFORMANCE) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = 100.0f;
  /* Select default operating mode. */
  pObj->acc_operating_mode = AIS2IH_HIGH_PERFORMANCE_MODE;
  /* Select default low noise (disabled). */
  pObj->acc_low_noise = AIS2IH_LOW_NOISE_DISABLE;

  /* Output data rate selection - power down. */
  if (ais2ih_data_rate_set(&(pObj->Ctx), AIS2IH_XL_ODR_OFF) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* Full scale selection. */
  if (ais2ih_full_scale_set(&(pObj->Ctx), AIS2IH_2g) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  pObj->is_initialized = 1;

  return AIS2IH_OK;
}

/**
 * @brief  Deinitialize the AIS2IH sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_DeInit(AIS2IH_Object_t *pObj)
{
  /* Disable the component */
  if (AIS2IH_ACC_Disable(pObj) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* Reset output data rate. */
  pObj->acc_odr = 0.0f;
  /* Reset operating mode to default value. */
  pObj->acc_operating_mode = AIS2IH_HIGH_PERFORMANCE_MODE;
  /* Reset low noise to default value (disabled). */
  pObj->acc_low_noise = AIS2IH_LOW_NOISE_DISABLE;

  pObj->is_initialized = 0;

  return AIS2IH_OK;
}

/**
 * @brief  Read component ID
 * @param  pObj the device pObj
 * @param  Id the WHO_AM_I value
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ReadID(AIS2IH_Object_t *pObj, uint8_t *Id)
{
  if (ais2ih_device_id_get(&(pObj->Ctx), Id) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  return AIS2IH_OK;
}

/**
 * @brief  Get AIS2IH sensor capabilities
 * @param  pObj Component object pointer
 * @param  Capabilities pointer to AIS2IH sensor capabilities
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_GetCapabilities(AIS2IH_Object_t *pObj, AIS2IH_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Acc          = 1;
  Capabilities->Gyro         = 0;
  Capabilities->Magneto      = 0;
  Capabilities->LowPower     = 1;
  Capabilities->GyroMaxFS    = 0;
  Capabilities->AccMaxFS     = 16;
  Capabilities->MagMaxFS     = 0;
  Capabilities->GyroMaxOdr   = 0.0f;
  Capabilities->AccMaxOdr    = 1600.0f;
  Capabilities->MagMaxOdr    = 0.0f;
  return AIS2IH_OK;
}

/**
 * @brief  Enable the AIS2IH accelerometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Enable(AIS2IH_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return AIS2IH_OK;
  }

  /* Output data rate selection. */
  if (AIS2IH_ACC_SetOutputDataRate_When_Enabled(pObj, pObj->acc_odr, pObj->acc_operating_mode, pObj->acc_low_noise) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return AIS2IH_OK;
}

/**
 * @brief  Disable the AIS2IH accelerometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Disable(AIS2IH_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return AIS2IH_OK;
  }

  /* Output data rate selection - power down. */
  if (ais2ih_data_rate_set(&(pObj->Ctx), AIS2IH_XL_ODR_OFF) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return AIS2IH_OK;
}

/**
 * @brief  Get the AIS2IH accelerometer sensor sensitivity
 * @param  pObj the device pObj
 * @param  Sensitivity pointer
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_GetSensitivity(AIS2IH_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = AIS2IH_OK;
  ais2ih_fs_t full_scale;
  ais2ih_mode_t mode;

  /* Read actual full scale selection from sensor. */
  if (ais2ih_full_scale_get(&(pObj->Ctx), &full_scale) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* Read actual power mode selection from sensor. */
  if (ais2ih_power_mode_get(&(pObj->Ctx), &mode) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  switch(mode)
  {
    case AIS2IH_CONT_LOW_PWR_12bit:
    case AIS2IH_SINGLE_LOW_PWR_12bit:
    case AIS2IH_CONT_LOW_PWR_LOW_NOISE_12bit:
    case AIS2IH_SINGLE_LOW_LOW_NOISE_PWR_12bit:
      switch (full_scale)
      {
        case AIS2IH_2g:
          *Sensitivity = AIS2IH_ACC_SENSITIVITY_FOR_FS_2G_LOPOW1_MODE;
           break;

        case AIS2IH_4g:
          *Sensitivity = AIS2IH_ACC_SENSITIVITY_FOR_FS_4G_LOPOW1_MODE;
          break;

        case AIS2IH_8g:
          *Sensitivity = AIS2IH_ACC_SENSITIVITY_FOR_FS_8G_LOPOW1_MODE;
           break;

        case AIS2IH_16g:
          *Sensitivity = AIS2IH_ACC_SENSITIVITY_FOR_FS_16G_LOPOW1_MODE;
          break;

        default:
          *Sensitivity = -1.0f;
          ret = AIS2IH_ERROR;
          break;
      }
      break;

    case AIS2IH_HIGH_PERFORMANCE:
    case AIS2IH_CONT_LOW_PWR_4:
    case AIS2IH_CONT_LOW_PWR_3:
    case AIS2IH_CONT_LOW_PWR_2:
    case AIS2IH_SINGLE_LOW_PWR_4:
    case AIS2IH_SINGLE_LOW_PWR_3:
    case AIS2IH_SINGLE_LOW_PWR_2:
    case AIS2IH_HIGH_PERFORMANCE_LOW_NOISE:
    case AIS2IH_CONT_LOW_PWR_LOW_NOISE_4:
    case AIS2IH_CONT_LOW_PWR_LOW_NOISE_3:
    case AIS2IH_CONT_LOW_PWR_LOW_NOISE_2:
    case AIS2IH_SINGLE_LOW_PWR_LOW_NOISE_4:
    case AIS2IH_SINGLE_LOW_PWR_LOW_NOISE_3:
    case AIS2IH_SINGLE_LOW_PWR_LOW_NOISE_2:
      switch (full_scale)
      {
        case AIS2IH_2g:
          *Sensitivity = AIS2IH_ACC_SENSITIVITY_FOR_FS_2G_OTHER_MODES;
           break;

        case AIS2IH_4g:
          *Sensitivity = AIS2IH_ACC_SENSITIVITY_FOR_FS_4G_OTHER_MODES;
          break;

        case AIS2IH_8g:
          *Sensitivity = AIS2IH_ACC_SENSITIVITY_FOR_FS_8G_OTHER_MODES;
           break;

        case AIS2IH_16g:
          *Sensitivity = AIS2IH_ACC_SENSITIVITY_FOR_FS_16G_OTHER_MODES;
          break;

        default:
          *Sensitivity = -1.0f;
          ret = AIS2IH_ERROR;
          break;
      }
      break;

    default:
      *Sensitivity = -1.0f;
      ret = AIS2IH_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Get the AIS2IH accelerometer sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr pointer where the output data rate is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_GetOutputDataRate(AIS2IH_Object_t *pObj, float *Odr)
{
  int32_t ret = AIS2IH_OK;
  ais2ih_odr_t odr_low_level;
  ais2ih_mode_t mode;

  /* Get current output data rate. */
  if (ais2ih_data_rate_get(&(pObj->Ctx), &odr_low_level) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* Read actual power mode selection from sensor. */
  if (ais2ih_power_mode_get(&(pObj->Ctx), &mode) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  switch (odr_low_level)
  {
    case AIS2IH_XL_ODR_OFF:
    case AIS2IH_XL_SET_SW_TRIG:
    case AIS2IH_XL_SET_PIN_TRIG:
      *Odr = 0.0f;
      break;

    case AIS2IH_XL_ODR_1Hz6_LP_ONLY:
      switch (mode)
      {
        case AIS2IH_HIGH_PERFORMANCE:
        case AIS2IH_HIGH_PERFORMANCE_LOW_NOISE:
          *Odr = 12.5f;
           break;

        case AIS2IH_CONT_LOW_PWR_4:
        case AIS2IH_CONT_LOW_PWR_3:
        case AIS2IH_CONT_LOW_PWR_2:
        case AIS2IH_CONT_LOW_PWR_12bit:
        case AIS2IH_SINGLE_LOW_PWR_4:
        case AIS2IH_SINGLE_LOW_PWR_3:
        case AIS2IH_SINGLE_LOW_PWR_2:
        case AIS2IH_SINGLE_LOW_PWR_12bit:
        case AIS2IH_CONT_LOW_PWR_LOW_NOISE_4:
        case AIS2IH_CONT_LOW_PWR_LOW_NOISE_3:
        case AIS2IH_CONT_LOW_PWR_LOW_NOISE_2:
        case AIS2IH_CONT_LOW_PWR_LOW_NOISE_12bit:
        case AIS2IH_SINGLE_LOW_PWR_LOW_NOISE_4:
        case AIS2IH_SINGLE_LOW_PWR_LOW_NOISE_3:
        case AIS2IH_SINGLE_LOW_PWR_LOW_NOISE_2:
        case AIS2IH_SINGLE_LOW_LOW_NOISE_PWR_12bit:
          *Odr = 1.6f;
          break;

        default:
          *Odr = -1.0f;
          ret = AIS2IH_ERROR;
          break;
      }
      break;

    case AIS2IH_XL_ODR_12Hz5:
      *Odr = 12.5f;
      break;

    case AIS2IH_XL_ODR_25Hz:
      *Odr = 25.0f;
      break;

    case AIS2IH_XL_ODR_50Hz:
      *Odr = 50.0f;
      break;

    case AIS2IH_XL_ODR_100Hz:
      *Odr = 100.0f;
      break;

    case AIS2IH_XL_ODR_200Hz:
      *Odr = 200.0f;
      break;

    case AIS2IH_XL_ODR_400Hz:
      switch (mode)
      {
        case AIS2IH_HIGH_PERFORMANCE:
        case AIS2IH_HIGH_PERFORMANCE_LOW_NOISE:
          *Odr = 400.0f;
           break;

        case AIS2IH_CONT_LOW_PWR_4:
        case AIS2IH_CONT_LOW_PWR_3:
        case AIS2IH_CONT_LOW_PWR_2:
        case AIS2IH_CONT_LOW_PWR_12bit:
        case AIS2IH_SINGLE_LOW_PWR_4:
        case AIS2IH_SINGLE_LOW_PWR_3:
        case AIS2IH_SINGLE_LOW_PWR_2:
        case AIS2IH_SINGLE_LOW_PWR_12bit:
        case AIS2IH_CONT_LOW_PWR_LOW_NOISE_4:
        case AIS2IH_CONT_LOW_PWR_LOW_NOISE_3:
        case AIS2IH_CONT_LOW_PWR_LOW_NOISE_2:
        case AIS2IH_CONT_LOW_PWR_LOW_NOISE_12bit:
        case AIS2IH_SINGLE_LOW_PWR_LOW_NOISE_4:
        case AIS2IH_SINGLE_LOW_PWR_LOW_NOISE_3:
        case AIS2IH_SINGLE_LOW_PWR_LOW_NOISE_2:
        case AIS2IH_SINGLE_LOW_LOW_NOISE_PWR_12bit:
          *Odr = 200.0f;
          break;

        default:
          *Odr = -1.0f;
          ret = AIS2IH_ERROR;
          break;
      }
      break;

    case AIS2IH_XL_ODR_800Hz:
      switch (mode)
      {
        case AIS2IH_HIGH_PERFORMANCE:
        case AIS2IH_HIGH_PERFORMANCE_LOW_NOISE:
          *Odr = 800.0f;
           break;

        case AIS2IH_CONT_LOW_PWR_4:
        case AIS2IH_CONT_LOW_PWR_3:
        case AIS2IH_CONT_LOW_PWR_2:
        case AIS2IH_CONT_LOW_PWR_12bit:
        case AIS2IH_SINGLE_LOW_PWR_4:
        case AIS2IH_SINGLE_LOW_PWR_3:
        case AIS2IH_SINGLE_LOW_PWR_2:
        case AIS2IH_SINGLE_LOW_PWR_12bit:
        case AIS2IH_CONT_LOW_PWR_LOW_NOISE_4:
        case AIS2IH_CONT_LOW_PWR_LOW_NOISE_3:
        case AIS2IH_CONT_LOW_PWR_LOW_NOISE_2:
        case AIS2IH_CONT_LOW_PWR_LOW_NOISE_12bit:
        case AIS2IH_SINGLE_LOW_PWR_LOW_NOISE_4:
        case AIS2IH_SINGLE_LOW_PWR_LOW_NOISE_3:
        case AIS2IH_SINGLE_LOW_PWR_LOW_NOISE_2:
        case AIS2IH_SINGLE_LOW_LOW_NOISE_PWR_12bit:
          *Odr = 200.0f;
          break;

        default:
          *Odr = -1.0f;
          ret = AIS2IH_ERROR;
          break;
      }
      break;

    case AIS2IH_XL_ODR_1k6Hz:
      switch (mode)
      {
        case AIS2IH_HIGH_PERFORMANCE:
        case AIS2IH_HIGH_PERFORMANCE_LOW_NOISE:
          *Odr = 1600.0f;
           break;

        case AIS2IH_CONT_LOW_PWR_4:
        case AIS2IH_CONT_LOW_PWR_3:
        case AIS2IH_CONT_LOW_PWR_2:
        case AIS2IH_CONT_LOW_PWR_12bit:
        case AIS2IH_SINGLE_LOW_PWR_4:
        case AIS2IH_SINGLE_LOW_PWR_3:
        case AIS2IH_SINGLE_LOW_PWR_2:
        case AIS2IH_SINGLE_LOW_PWR_12bit:
        case AIS2IH_CONT_LOW_PWR_LOW_NOISE_4:
        case AIS2IH_CONT_LOW_PWR_LOW_NOISE_3:
        case AIS2IH_CONT_LOW_PWR_LOW_NOISE_2:
        case AIS2IH_CONT_LOW_PWR_LOW_NOISE_12bit:
        case AIS2IH_SINGLE_LOW_PWR_LOW_NOISE_4:
        case AIS2IH_SINGLE_LOW_PWR_LOW_NOISE_3:
        case AIS2IH_SINGLE_LOW_PWR_LOW_NOISE_2:
        case AIS2IH_SINGLE_LOW_LOW_NOISE_PWR_12bit:
          *Odr = 200.0f;
          break;

        default:
          *Odr = -1.0f;
          ret = AIS2IH_ERROR;
          break;
      }
      break;

    default:
      *Odr = -1.0f;
      ret = AIS2IH_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the AIS2IH accelerometer sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_SetOutputDataRate(AIS2IH_Object_t *pObj, float Odr)
{
  /* By default we use High Performance mode and Low Noise disabled */
  return AIS2IH_ACC_SetOutputDataRate_With_Mode(pObj, Odr, AIS2IH_HIGH_PERFORMANCE_MODE, AIS2IH_LOW_NOISE_DISABLE);
}

/**
 * @brief  Set the AIS2IH accelerometer sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @param  Mode the operating mode to be used
 * @param  Noise the low noise option
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_SetOutputDataRate_With_Mode(AIS2IH_Object_t *pObj, float Odr, AIS2IH_Operating_Mode_t Mode, AIS2IH_Low_Noise_t Noise)
{
  /* Check if the component is enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return AIS2IH_ACC_SetOutputDataRate_When_Enabled(pObj, Odr, Mode, Noise);
  }
  else
  {
    return AIS2IH_ACC_SetOutputDataRate_When_Disabled(pObj, Odr, Mode, Noise);
  }
}

/**
 * @brief  Get the AIS2IH accelerometer sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale pointer where the full scale is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_GetFullScale(AIS2IH_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = AIS2IH_OK;
  ais2ih_fs_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (ais2ih_full_scale_get(&(pObj->Ctx), &fs_low_level) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  switch (fs_low_level)
  {
    case AIS2IH_2g:
      *FullScale =  2;
      break;

    case AIS2IH_4g:
      *FullScale =  4;
      break;

    case AIS2IH_8g:
      *FullScale =  8;
      break;

    case AIS2IH_16g:
      *FullScale = 16;
      break;

    default:
      *FullScale = -1;
      ret = AIS2IH_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the AIS2IH accelerometer sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale the functional full scale to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_SetFullScale(AIS2IH_Object_t *pObj, int32_t FullScale)
{
  ais2ih_fs_t new_fs;

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file statical analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  new_fs = (FullScale <= 2) ? AIS2IH_2g
         : (FullScale <= 4) ? AIS2IH_4g
         : (FullScale <= 8) ? AIS2IH_8g
         :                    AIS2IH_16g;

  if (ais2ih_full_scale_set(&(pObj->Ctx), new_fs) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  return AIS2IH_OK;
}

/**
 * @brief  Get the AIS2IH accelerometer sensor raw axes
 * @param  pObj the device pObj
 * @param  Value pointer where the raw values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_GetAxesRaw(AIS2IH_Object_t *pObj, AIS2IH_AxesRaw_t *Value)
{
  ais2ih_axis3bit16_t data_raw;
  ais2ih_mode_t mode;
  int32_t ret = AIS2IH_OK;

  /* Read actual power mode selection from sensor. */
  if (ais2ih_power_mode_get(&(pObj->Ctx), &mode) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* Read raw data values. */
  if (ais2ih_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  switch(mode)
  {
    case AIS2IH_CONT_LOW_PWR_12bit:
    case AIS2IH_SINGLE_LOW_PWR_12bit:
    case AIS2IH_CONT_LOW_PWR_LOW_NOISE_12bit:
    case AIS2IH_SINGLE_LOW_LOW_NOISE_PWR_12bit:
      /* Data format 12 bits. */
      Value->x = (data_raw.i16bit[0] / 16);
      Value->y = (data_raw.i16bit[1] / 16);
      Value->z = (data_raw.i16bit[2] / 16);
      break;

    case AIS2IH_HIGH_PERFORMANCE:
    case AIS2IH_CONT_LOW_PWR_4:
    case AIS2IH_CONT_LOW_PWR_3:
    case AIS2IH_CONT_LOW_PWR_2:
    case AIS2IH_SINGLE_LOW_PWR_4:
    case AIS2IH_SINGLE_LOW_PWR_3:
    case AIS2IH_SINGLE_LOW_PWR_2:
    case AIS2IH_HIGH_PERFORMANCE_LOW_NOISE:
    case AIS2IH_CONT_LOW_PWR_LOW_NOISE_4:
    case AIS2IH_CONT_LOW_PWR_LOW_NOISE_3:
    case AIS2IH_CONT_LOW_PWR_LOW_NOISE_2:
    case AIS2IH_SINGLE_LOW_PWR_LOW_NOISE_4:
    case AIS2IH_SINGLE_LOW_PWR_LOW_NOISE_3:
    case AIS2IH_SINGLE_LOW_PWR_LOW_NOISE_2:
      /* Data format 14 bits. */
      Value->x = (data_raw.i16bit[0] / 4);
      Value->y = (data_raw.i16bit[1] / 4);
      Value->z = (data_raw.i16bit[2] / 4);
      break;

    default:
      ret = AIS2IH_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Get the AIS2IH accelerometer sensor axes
 * @param  pObj the device pObj
 * @param  Acceleration pointer where the values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_GetAxes(AIS2IH_Object_t *pObj, AIS2IH_Axes_t *Acceleration)
{
  AIS2IH_AxesRaw_t data_raw;
  float sensitivity = 0.0f;

  /* Read raw data values. */
  if (AIS2IH_ACC_GetAxesRaw(pObj, &data_raw) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* Get AIS2IH actual sensitivity. */
  if (AIS2IH_ACC_GetSensitivity(pObj, &sensitivity) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* Calculate the data. */
  Acceleration->x = (int32_t)((float)((float)data_raw.x * sensitivity));
  Acceleration->y = (int32_t)((float)((float)data_raw.y * sensitivity));
  Acceleration->z = (int32_t)((float)((float)data_raw.z * sensitivity));

  return AIS2IH_OK;
}

/**
 * @brief  Get the AIS2IH register value
 * @param  pObj the device pObj
 * @param  Reg address to be read
 * @param  Data pointer where the value is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_Read_Reg(AIS2IH_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (ais2ih_read_reg(&(pObj->Ctx), Reg, Data, 1) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  return AIS2IH_OK;
}

/**
 * @brief  Set the AIS2IH register value
 * @param  pObj the device pObj
 * @param  Reg address to be written
 * @param  Data value to be written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_Write_Reg(AIS2IH_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (ais2ih_write_reg(&(pObj->Ctx), Reg, &Data, 1) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  return AIS2IH_OK;
}

/**
 * @brief  Set the interrupt latch
 * @param  pObj the device pObj
 * @param  Status value to be written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_Set_Interrupt_Latch(AIS2IH_Object_t *pObj, uint8_t Status)
{
  if (Status > 1U)
  {
    return AIS2IH_ERROR;
  }

  if (ais2ih_int_notification_set(&(pObj->Ctx), (ais2ih_lir_t)Status) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  return AIS2IH_OK;
}

/**
 * @brief  Enable DRDY interrupt mode
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Enable_DRDY_Interrupt(AIS2IH_Object_t *pObj)
{
  ais2ih_ctrl5_int2_pad_ctrl_t int2_pad_ctrl;

  /* Enable DRDY interrupts on INT1 */
  if (ais2ih_data_ready_mode_set(&(pObj->Ctx), AIS2IH_DRDY_PULSED) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }
  if (ais2ih_pin_int2_route_get(&(pObj->Ctx), &int2_pad_ctrl) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }
  int2_pad_ctrl.int2_drdy = 1;
  if (ais2ih_pin_int2_route_set(&(pObj->Ctx), &int2_pad_ctrl) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  return AIS2IH_OK;
}

/**
 * @brief  Disable DRDY interrupt mode
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Disable_DRDY_Interrupt(AIS2IH_Object_t *pObj)
{
  ais2ih_ctrl5_int2_pad_ctrl_t int2_pad_ctrl;

  if (ais2ih_pin_int2_route_get(&(pObj->Ctx), &int2_pad_ctrl) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }
  int2_pad_ctrl.int2_drdy = 0;
  if (ais2ih_pin_int2_route_set(&(pObj->Ctx), &int2_pad_ctrl) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  return AIS2IH_OK;
}

/**
 * @brief  Set the filterMode value
 * @param  pObj the device pObj
 * @param  filterMode value to be written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Set_Filter_Mode(AIS2IH_Object_t *pObj, uint8_t filterMode)
{
  if(ais2ih_filter_bandwidth_set(&(pObj->Ctx), (ais2ih_bw_filt_t)filterMode) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  return AIS2IH_OK;
}

/**
 * @brief  Enable wake up detection
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Enable_Wake_Up_Detection(AIS2IH_Object_t *pObj)
{
  int32_t ret = AIS2IH_OK;
  ais2ih_ctrl4_int1_pad_ctrl_t val;

  /* Output Data Rate selection */
  if (AIS2IH_ACC_SetOutputDataRate(pObj, 200.0f) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* Full scale selection */
  if (AIS2IH_ACC_SetFullScale(pObj, 2) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* WAKE_DUR setting */
  if (ais2ih_wkup_dur_set(&(pObj->Ctx), 0x00) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* Set wake up threshold. */
  if (ais2ih_wkup_threshold_set(&(pObj->Ctx), 0x02) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  if (ais2ih_pin_int1_route_get(&(pObj->Ctx), &val) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  val.int1_wu = PROPERTY_ENABLE;

  if (ais2ih_pin_int1_route_set(&(pObj->Ctx), &val) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  return ret;
}

/**
 * @brief  Disable wake up detection
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Disable_Wake_Up_Detection(AIS2IH_Object_t *pObj)
{
  ais2ih_ctrl4_int1_pad_ctrl_t ctrl4_int1_reg;
  ais2ih_ctrl5_int2_pad_ctrl_t ctrl5_int2_reg;
  ais2ih_ctrl_reg7_t ctrl_reg7;

  /* Disable wake up event on INT1 pin. */
  if (ais2ih_read_reg(&(pObj->Ctx), AIS2IH_CTRL4_INT1_PAD_CTRL, (uint8_t *)&ctrl4_int1_reg, 1) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  ctrl4_int1_reg.int1_wu = PROPERTY_DISABLE;

  if (ais2ih_write_reg(&(pObj->Ctx), AIS2IH_CTRL4_INT1_PAD_CTRL, (uint8_t *)&ctrl4_int1_reg, 1) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* Read INT2 Sleep Change */
  if (ais2ih_read_reg(&(pObj->Ctx), AIS2IH_CTRL5_INT2_PAD_CTRL, (uint8_t *)&ctrl5_int2_reg, 1) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /*Disable Interrupts bit if none event is still enabled */
  if(ctrl5_int2_reg.int2_sleep_chg == 0 && ctrl4_int1_reg.int1_wu == 0 && ctrl4_int1_reg.int1_6d == 0)
  {
    if (ais2ih_read_reg(&(pObj->Ctx), AIS2IH_CTRL_REG7, (uint8_t *)&ctrl_reg7, 1) != AIS2IH_OK)
    {
      return AIS2IH_ERROR;
    }

    ctrl_reg7.interrupts_enable = PROPERTY_DISABLE;

    if (ais2ih_write_reg(&(pObj->Ctx), AIS2IH_CTRL_REG7, (uint8_t *)&ctrl_reg7, 1) != AIS2IH_OK)
    {
      return AIS2IH_ERROR;
    }
  }

  /* Reset wake up threshold. */
  if (ais2ih_wkup_threshold_set(&(pObj->Ctx), 0x00) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* WAKE_DUR setting */
  if (ais2ih_wkup_dur_set(&(pObj->Ctx), 0x00) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  return AIS2IH_OK;
}

/**
 * @brief  Set wake up threshold
 * @param  pObj the device pObj
 * @param  Threshold wake up detection threshold
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Set_Wake_Up_Threshold(AIS2IH_Object_t *pObj, uint8_t Threshold)
{
  /* Set wake up threshold. */
  if (ais2ih_wkup_threshold_set(&(pObj->Ctx), Threshold) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  return AIS2IH_OK;
}

/**
 * @brief  Set wake up duration
 * @param  pObj the device pObj
 * @param  Duration wake up detection duration
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Set_Wake_Up_Duration(AIS2IH_Object_t *pObj, uint8_t Duration)
{
  /* Set wake up duration. */
  if (ais2ih_wkup_dur_set(&(pObj->Ctx), Duration) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  return AIS2IH_OK;
}

/**
 * @brief  Enable inactivity detection
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Enable_Inactivity_Detection(AIS2IH_Object_t *pObj)
{
  int32_t ret = AIS2IH_OK;
  ais2ih_ctrl5_int2_pad_ctrl_t val;

  /* Output Data Rate selection */
  if (AIS2IH_ACC_SetOutputDataRate(pObj, 50.0f) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* Full scale selection */
  if (AIS2IH_ACC_SetFullScale(pObj, 2) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* SLEEP_DUR setting */
  if (ais2ih_act_sleep_dur_set(&(pObj->Ctx), 0x01) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* Enable inactivity detection. */
  if (ais2ih_act_mode_set(&(pObj->Ctx), AIS2IH_DETECT_ACT_INACT) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  if (ais2ih_pin_int2_route_get(&(pObj->Ctx), &val) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  val.int2_sleep_chg = PROPERTY_ENABLE;

  if (ais2ih_pin_int2_route_set(&(pObj->Ctx), &val) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  return ret;
}

/**
 * @brief  Disable inactivity detection
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Disable_Inactivity_Detection(AIS2IH_Object_t *pObj)
{
  ais2ih_ctrl4_int1_pad_ctrl_t ctrl4_int1_reg;
  ais2ih_ctrl5_int2_pad_ctrl_t ctrl5_int2_reg;
  ais2ih_ctrl_reg7_t ctrl_reg7;

  /* Disable inactivity event on INT2 pin */
  if (ais2ih_read_reg(&(pObj->Ctx), AIS2IH_CTRL5_INT2_PAD_CTRL, (uint8_t *)&ctrl5_int2_reg, 1) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  ctrl5_int2_reg.int2_sleep_chg = PROPERTY_DISABLE;

  if (ais2ih_write_reg(&(pObj->Ctx), AIS2IH_CTRL5_INT2_PAD_CTRL, (uint8_t *)&ctrl5_int2_reg, 1) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* Read INT1 Wake Up event and INT1 6D Orientation event */
  if (ais2ih_read_reg(&(pObj->Ctx), AIS2IH_CTRL4_INT1_PAD_CTRL, (uint8_t *)&ctrl4_int1_reg, 1) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /*Disable Interrupts bit if none event is still enabled */
  if(ctrl5_int2_reg.int2_sleep_chg == 0 && ctrl4_int1_reg.int1_wu == 0 && ctrl4_int1_reg.int1_6d == 0)
  {
    if (ais2ih_read_reg(&(pObj->Ctx), AIS2IH_CTRL_REG7, (uint8_t *)&ctrl_reg7, 1) != AIS2IH_OK)
    {
      return AIS2IH_ERROR;
    }

    ctrl_reg7.interrupts_enable = PROPERTY_DISABLE;

    if (ais2ih_write_reg(&(pObj->Ctx), AIS2IH_CTRL_REG7, (uint8_t *)&ctrl_reg7, 1) != AIS2IH_OK)
    {
      return AIS2IH_ERROR;
    }
  }

  /* Disable inactivity detection. */
  if (ais2ih_act_mode_set(&(pObj->Ctx), AIS2IH_NO_DETECTION) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* SLEEP_DUR setting */
  if (ais2ih_act_sleep_dur_set(&(pObj->Ctx), 0x00) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  return AIS2IH_OK;
}

/**
 * @brief  Set sleep duration
 * @param  pObj the device pObj
 * @param  Duration wake up detection duration
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Set_Sleep_Duration(AIS2IH_Object_t *pObj, uint8_t Duration)
{
  /* Set sleep duration. */
  if (ais2ih_act_sleep_dur_set(&(pObj->Ctx), Duration) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  return AIS2IH_OK;
}

/**
 * @brief  Enable 6D orientation detection
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Enable_6D_Orientation(AIS2IH_Object_t *pObj)
{
  int32_t ret = AIS2IH_OK;
  ais2ih_ctrl4_int1_pad_ctrl_t val;

  /* Output Data Rate selection */
  if (AIS2IH_ACC_SetOutputDataRate(pObj, 200.0f) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* Full scale selection */
  if (AIS2IH_ACC_SetFullScale(pObj, 2) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* 6D orientation threshold. */
  if (ais2ih_6d_threshold_set(&(pObj->Ctx), 2) != AIS2IH_OK) /* 60 degrees */
  {
    return AIS2IH_ERROR;
  }

  /* Enable 6D orientation event on INT1 pin */
  if (ais2ih_pin_int1_route_get(&(pObj->Ctx), &val) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  val.int1_6d = PROPERTY_ENABLE;

  if (ais2ih_pin_int1_route_set(&(pObj->Ctx), &val) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  return ret;
}

/**
 * @brief  Disable 6D orientation detection
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Disable_6D_Orientation(AIS2IH_Object_t *pObj)
{
  ais2ih_ctrl4_int1_pad_ctrl_t ctrl4_int1_reg;
  ais2ih_ctrl5_int2_pad_ctrl_t ctrl5_int2_reg;
  ais2ih_ctrl_reg7_t ctrl_reg7;

  /* Disable 6D orientation event on INT1 pin */
  if (ais2ih_read_reg(&(pObj->Ctx), AIS2IH_CTRL4_INT1_PAD_CTRL, (uint8_t *)&ctrl4_int1_reg, 1) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  ctrl4_int1_reg.int1_6d = PROPERTY_DISABLE;

  if (ais2ih_write_reg(&(pObj->Ctx), AIS2IH_CTRL4_INT1_PAD_CTRL, (uint8_t *)&ctrl4_int1_reg, 1) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* Read INT2 Sleep Change */
  if (ais2ih_read_reg(&(pObj->Ctx), AIS2IH_CTRL5_INT2_PAD_CTRL, (uint8_t *)&ctrl5_int2_reg, 1) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /*Disable Interrupts bit if none event is still enabled */
  if(ctrl5_int2_reg.int2_sleep_chg == 0 && ctrl4_int1_reg.int1_wu == 0 && ctrl4_int1_reg.int1_6d == 0)
  {
    if (ais2ih_read_reg(&(pObj->Ctx), AIS2IH_CTRL_REG7, (uint8_t *)&ctrl_reg7, 1) != AIS2IH_OK)
    {
      return AIS2IH_ERROR;
    }

    ctrl_reg7.interrupts_enable = PROPERTY_DISABLE;

    if (ais2ih_write_reg(&(pObj->Ctx), AIS2IH_CTRL_REG7, (uint8_t *)&ctrl_reg7, 1) != AIS2IH_OK)
    {
      return AIS2IH_ERROR;
    }
  }

  /* Reset 6D orientation threshold. */
  if (ais2ih_6d_threshold_set(&(pObj->Ctx), 0) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  return AIS2IH_OK;
}

/**
 * @brief  Set 6D orientation threshold
 * @param  pObj the device pObj
 * @param  Threshold 6D orientation detection threshold
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Set_6D_Orientation_Threshold(AIS2IH_Object_t *pObj, uint8_t Threshold)
{
  if(Threshold > 3)
  {
    return AIS2IH_ERROR;
  }

  if (ais2ih_6d_threshold_set(&(pObj->Ctx), Threshold) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  return AIS2IH_OK;
}

/**
 * @brief  Get the status of XLow orientation
 * @param  pObj the device pObj
 * @param  XLow the status of XLow orientation
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Get_6D_Orientation_XL(AIS2IH_Object_t *pObj, uint8_t *XLow)
{
  ais2ih_sixd_src_t data;

  if (ais2ih_read_reg(&(pObj->Ctx), AIS2IH_SIXD_SRC, (uint8_t *)&data, 1) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  *XLow = data.xl;

  return AIS2IH_OK;
}

/**
 * @brief  Get the status of XHigh orientation
 * @param  pObj the device pObj
 * @param  XHigh the status of XHigh orientation
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Get_6D_Orientation_XH(AIS2IH_Object_t *pObj, uint8_t *XHigh)
{
  ais2ih_sixd_src_t data;

  if (ais2ih_read_reg(&(pObj->Ctx), AIS2IH_SIXD_SRC, (uint8_t *)&data, 1) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  *XHigh = data.xh;

  return AIS2IH_OK;
}

/**
 * @brief  Get the status of YLow orientation
 * @param  pObj the device pObj
 * @param  YLow the status of YLow orientation
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Get_6D_Orientation_YL(AIS2IH_Object_t *pObj, uint8_t *YLow)
{
  ais2ih_sixd_src_t data;

  if (ais2ih_read_reg(&(pObj->Ctx), AIS2IH_SIXD_SRC, (uint8_t *)&data, 1) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  *YLow = data.yl;

  return AIS2IH_OK;
}

/**
 * @brief  Get the status of YHigh orientation
 * @param  pObj the device pObj
 * @param  YHigh the status of YHigh orientation
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Get_6D_Orientation_YH(AIS2IH_Object_t *pObj, uint8_t *YHigh)
{
  ais2ih_sixd_src_t data;

  if (ais2ih_read_reg(&(pObj->Ctx), AIS2IH_SIXD_SRC, (uint8_t *)&data, 1) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  *YHigh = data.yh;

  return AIS2IH_OK;
}

/**
 * @brief  Get the status of ZLow orientation
 * @param  pObj the device pObj
 * @param  ZLow the status of ZLow orientation
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Get_6D_Orientation_ZL(AIS2IH_Object_t *pObj, uint8_t *ZLow)
{
  ais2ih_sixd_src_t data;

  if (ais2ih_read_reg(&(pObj->Ctx), AIS2IH_SIXD_SRC, (uint8_t *)&data, 1) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  *ZLow = data.zl;

  return AIS2IH_OK;
}

/**
 * @brief  Get the status of ZHigh orientation
 * @param  pObj the device pObj
 * @param  ZHigh the status of ZHigh orientation
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Get_6D_Orientation_ZH(AIS2IH_Object_t *pObj, uint8_t *ZHigh)
{
  ais2ih_sixd_src_t data;

  if (ais2ih_read_reg(&(pObj->Ctx), AIS2IH_SIXD_SRC, (uint8_t *)&data, 1) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  *ZHigh = data.zh;

  return AIS2IH_OK;
}

/**
 * @brief  Get the status of all hardware events
 * @param  pObj the device pObj
 * @param  Status the status of all hardware events
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Get_Event_Status(AIS2IH_Object_t *pObj, AIS2IH_Event_Status_t *Status)
{
  ais2ih_status_t status_reg;
  ais2ih_ctrl4_int1_pad_ctrl_t ctrl4_int1_reg;
  ais2ih_ctrl5_int2_pad_ctrl_t ctrl5_int2_reg;

  (void)memset((void *)Status, 0x0, sizeof(AIS2IH_Event_Status_t));

  if (ais2ih_read_reg(&(pObj->Ctx), AIS2IH_STATUS, (uint8_t *)&status_reg, 1) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  if (ais2ih_read_reg(&(pObj->Ctx), AIS2IH_CTRL4_INT1_PAD_CTRL, (uint8_t *)&ctrl4_int1_reg, 1) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  if (ais2ih_read_reg(&(pObj->Ctx), AIS2IH_CTRL5_INT2_PAD_CTRL, (uint8_t *)&ctrl5_int2_reg, 1) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  if (ctrl4_int1_reg.int1_wu == 1U)
  {
    if (status_reg.wu_ia == 1U)
    {
      Status->WakeUpStatus = 1;
    }
  }

  if (ctrl4_int1_reg.int1_6d == 1U)
  {
    if (status_reg._6d_ia == 1U)
    {
      Status->D6DOrientationStatus = 1;
    }
  }

  if (ctrl5_int2_reg.int2_sleep_chg == 1U)
  {
    if (status_reg.sleep_state == 1U)
    {
      Status->SleepStatus = 1;
    }
  }

  return AIS2IH_OK;
}

/**
 * @brief  Set self test
 * @param  pObj the device pObj
 * @param  Val the value of ST in reg CTRL3
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Set_SelfTest(AIS2IH_Object_t *pObj, uint8_t Val)
{
  ais2ih_st_t reg;

  if(Val > 2U)
  {
    return AIS2IH_ERROR;
  }

  reg = (Val == 0U)  ? AIS2IH_XL_ST_DISABLE
      : (Val == 1U)  ? AIS2IH_XL_ST_POSITIVE
      :                AIS2IH_XL_ST_NEGATIVE;

  if (ais2ih_self_test_set(&(pObj->Ctx), reg) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  return AIS2IH_OK;
}

/**
 * @brief  Get the AIS2IH ACC data ready bit value
 * @param  pObj the device pObj
 * @param  Status the status of data ready bit
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Get_DRDY_Status(AIS2IH_Object_t *pObj, uint8_t *Status)
{
  if (ais2ih_flag_data_ready_get(&(pObj->Ctx), Status) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  return AIS2IH_OK;
}

/**
 * @brief  Get the AIS2IH ACC initialization status
 * @param  pObj the device pObj
 * @param  Status 1 if initialized, 0 otherwise
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_ACC_Get_Init_Status(AIS2IH_Object_t *pObj, uint8_t *Status)
{
  if (pObj == NULL)
  {
    return AIS2IH_ERROR;
  }

  *Status = pObj->is_initialized;

  return AIS2IH_OK;
}

/**
 * @brief  Get the number of samples contained into the FIFO
 * @param  pObj the device pObj
 * @param  NumSamples the number of samples contained into the FIFO
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_FIFO_Get_Num_Samples(AIS2IH_Object_t *pObj, uint16_t *NumSamples)
{
  ais2ih_fifo_samples_t FIFO_Samples;

  if (ais2ih_read_reg(&(pObj->Ctx), AIS2IH_FIFO_SAMPLES, (uint8_t *)&FIFO_Samples, 1) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  if (FIFO_Samples.diff == 0x20)
  {
    *NumSamples = 32;
  }
  else
  {
    *NumSamples = FIFO_Samples.diff;
  }

  return AIS2IH_OK;
}

/**
 * @brief  Set the FIFO mode
 * @param  pObj the device pObj
 * @param  Mode FIFO mode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t AIS2IH_FIFO_Set_Mode(AIS2IH_Object_t *pObj, uint8_t Mode)
{
  int32_t ret = AIS2IH_OK;

  /* Verify that the passed parameter contains one of the valid values. */
  switch ((ais2ih_fmode_t)Mode)
  {
    case AIS2IH_BYPASS_MODE:
    case AIS2IH_FIFO_MODE:
    case AIS2IH_STREAM_TO_FIFO_MODE:
    case AIS2IH_BYPASS_TO_STREAM_MODE:
    case AIS2IH_STREAM_MODE:
      break;

    default:
      ret = AIS2IH_ERROR;
      break;
  }

  if (ret == AIS2IH_ERROR)
  {
    return ret;
  }

  if (ais2ih_fifo_mode_set(&(pObj->Ctx), (ais2ih_fmode_t)Mode) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  return ret;
}

/**
 * @}
 */

/** @defgroup AIS2IH_Private_Functions AIS2IH Private Functions
 * @{
 */

/**
 * @brief  Set the AIS2IH accelerometer sensor output data rate when enabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @param  Mode the operating mode to be used
 * @param  Noise the low noise option
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t AIS2IH_ACC_SetOutputDataRate_When_Enabled(AIS2IH_Object_t *pObj, float Odr, AIS2IH_Operating_Mode_t Mode, AIS2IH_Low_Noise_t Noise)
{
  ais2ih_odr_t new_odr;
  ais2ih_mode_t new_power_mode;

  switch (Mode)
  {
    case AIS2IH_HIGH_PERFORMANCE_MODE:
    default:
      switch (Noise)
      {
        case AIS2IH_LOW_NOISE_DISABLE:
        default:
          new_power_mode = AIS2IH_HIGH_PERFORMANCE;
          break;
        case AIS2IH_LOW_NOISE_ENABLE:
          new_power_mode = AIS2IH_HIGH_PERFORMANCE_LOW_NOISE;
          break;
      }

      /* If High Performance mode minimum ODR is 12.5Hz */
      if (Odr < 12.5f)
      {
        Odr = 12.5f;
      }
      break;

    case AIS2IH_LOW_POWER_MODE4:
      switch (Noise)
      {
        case AIS2IH_LOW_NOISE_DISABLE:
        default:
          new_power_mode = AIS2IH_CONT_LOW_PWR_4;
          break;
        case AIS2IH_LOW_NOISE_ENABLE:
          new_power_mode = AIS2IH_CONT_LOW_PWR_LOW_NOISE_4;
          break;
      }

      /* If Low Power mode maximum ODR is 200Hz */
      if (Odr > 200.0f)
      {
        Odr = 200.0f;
      }
      break;

    case AIS2IH_LOW_POWER_MODE3:
      switch (Noise)
      {
        case AIS2IH_LOW_NOISE_DISABLE:
        default:
          new_power_mode = AIS2IH_CONT_LOW_PWR_3;
          break;
        case AIS2IH_LOW_NOISE_ENABLE:
          new_power_mode = AIS2IH_CONT_LOW_PWR_LOW_NOISE_3;
          break;
      }

      /* If Low Power mode maximum ODR is 200Hz */
      if (Odr > 200.0f)
      {
        Odr = 200.0f;
      }
      break;

    case AIS2IH_LOW_POWER_MODE2:
      switch (Noise)
      {
        case AIS2IH_LOW_NOISE_DISABLE:
        default:
          new_power_mode = AIS2IH_CONT_LOW_PWR_2;
          break;
        case AIS2IH_LOW_NOISE_ENABLE:
          new_power_mode = AIS2IH_CONT_LOW_PWR_LOW_NOISE_2;
          break;
      }

      /* If Low Power mode maximum ODR is 200Hz */
      if (Odr > 200.0f)
      {
        Odr = 200.0f;
      }
      break;

    case AIS2IH_LOW_POWER_MODE1:
      switch (Noise)
      {
        case AIS2IH_LOW_NOISE_DISABLE:
        default:
          new_power_mode = AIS2IH_CONT_LOW_PWR_12bit;
          break;
        case AIS2IH_LOW_NOISE_ENABLE:
          new_power_mode = AIS2IH_CONT_LOW_PWR_LOW_NOISE_12bit;
          break;
      }

      /* If Low Power mode maximum ODR is 200Hz */
      if (Odr > 200.0f)
      {
        Odr = 200.0f;
      }
      break;
  }

  new_odr = (Odr <=    1.6f) ? AIS2IH_XL_ODR_1Hz6_LP_ONLY
          : (Odr <=   12.5f) ? AIS2IH_XL_ODR_12Hz5
          : (Odr <=   25.0f) ? AIS2IH_XL_ODR_25Hz
          : (Odr <=   50.0f) ? AIS2IH_XL_ODR_50Hz
          : (Odr <=  100.0f) ? AIS2IH_XL_ODR_100Hz
          : (Odr <=  200.0f) ? AIS2IH_XL_ODR_200Hz
          : (Odr <=  400.0f) ? AIS2IH_XL_ODR_400Hz
          : (Odr <=  800.0f) ? AIS2IH_XL_ODR_800Hz
          :                    AIS2IH_XL_ODR_1k6Hz;

  /* Output data rate selection. */
  if (ais2ih_data_rate_set(&(pObj->Ctx), new_odr) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* Power mode selection. */
  if (ais2ih_power_mode_set(&(pObj->Ctx), new_power_mode) != AIS2IH_OK)
  {
    return AIS2IH_ERROR;
  }

  /* Store the current Odr, Mode and Noise values */
  pObj->acc_odr = Odr;
  pObj->acc_operating_mode = Mode;
  pObj->acc_low_noise = Noise;

  return AIS2IH_OK;
}

/**
 * @brief  Set the AIS2IH accelerometer sensor output data rate when disabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @param  Mode the operating mode to be used
 * @param  Noise the low noise option
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t AIS2IH_ACC_SetOutputDataRate_When_Disabled(AIS2IH_Object_t *pObj, float Odr, AIS2IH_Operating_Mode_t Mode, AIS2IH_Low_Noise_t Noise)
{
  /* Store the new Odr, Mode and Noise values */
  pObj->acc_operating_mode = Mode;
  pObj->acc_low_noise = Noise;

  pObj->acc_odr = (Odr <=    1.6f) ?    1.6f
                : (Odr <=   12.5f) ?   12.5f
                : (Odr <=   25.0f) ?   25.0f
                : (Odr <=   50.0f) ?   50.0f
                : (Odr <=  100.0f) ?  100.0f
                : (Odr <=  200.0f) ?  200.0f
                : (Odr <=  400.0f) ?  400.0f
                : (Odr <=  800.0f) ?  800.0f
                :                    1600.0f;

  return AIS2IH_OK;
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
  AIS2IH_Object_t *pObj = (AIS2IH_Object_t *)Handle;

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
  AIS2IH_Object_t *pObj = (AIS2IH_Object_t *)Handle;

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
