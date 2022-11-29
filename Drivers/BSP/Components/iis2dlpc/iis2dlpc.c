/**
  ******************************************************************************
  * @file    iis2dlpc.c
  * @author  MEMS Software Solutions Team
  * @brief   IIS2DLPC driver file
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
#include "iis2dlpc.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup IIS2DLPC IIS2DLPC
  * @{
  */

/** @defgroup IIS2DLPC_Exported_Variables IIS2DLPC Exported Variables
  * @{
  */

IIS2DLPC_CommonDrv_t IIS2DLPC_COMMON_Driver =
{
  IIS2DLPC_Init,
  IIS2DLPC_DeInit,
  IIS2DLPC_ReadID,
  IIS2DLPC_GetCapabilities,
};

IIS2DLPC_ACC_Drv_t IIS2DLPC_ACC_Driver =
{
  IIS2DLPC_ACC_Enable,
  IIS2DLPC_ACC_Disable,
  IIS2DLPC_ACC_GetSensitivity,
  IIS2DLPC_ACC_GetOutputDataRate,
  IIS2DLPC_ACC_SetOutputDataRate,
  IIS2DLPC_ACC_GetFullScale,
  IIS2DLPC_ACC_SetFullScale,
  IIS2DLPC_ACC_GetAxes,
  IIS2DLPC_ACC_GetAxesRaw,
};

/**
  * @}
  */

/** @defgroup IIS2DLPC_Private_Function_Prototypes IIS2DLPC Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t IIS2DLPC_ACC_SetOutputDataRate_When_Enabled(IIS2DLPC_Object_t *pObj, float Odr,
                                                           IIS2DLPC_Operating_Mode_t Mode, IIS2DLPC_Low_Noise_t Noise);
static int32_t IIS2DLPC_ACC_SetOutputDataRate_When_Disabled(IIS2DLPC_Object_t *pObj, float Odr,
                                                            IIS2DLPC_Operating_Mode_t Mode, IIS2DLPC_Low_Noise_t Noise);

/**
  * @}
  */

/** @defgroup IIS2DLPC_Exported_Functions IIS2DLPC Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DLPC_RegisterBusIO(IIS2DLPC_Object_t *pObj, IIS2DLPC_IO_t *pIO)
{
  int32_t ret = IIS2DLPC_OK;

  if (pObj == NULL)
  {
    ret = IIS2DLPC_ERROR;
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
      ret = IIS2DLPC_ERROR;
    }
    else if (pObj->IO.Init() != IIS2DLPC_OK)
    {
      ret = IIS2DLPC_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == IIS2DLPC_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x05;

          if (IIS2DLPC_Write_Reg(pObj, IIS2DLPC_CTRL2, data) != IIS2DLPC_OK)
          {
            ret = IIS2DLPC_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  Initialize the IIS2DLPC sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DLPC_Init(IIS2DLPC_Object_t *pObj)
{
  /* Enable register address automatically incremented during a multiple byte
  access with a serial interface. */
  if (iis2dlpc_auto_increment_set(&(pObj->Ctx), PROPERTY_ENABLE) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  /* Enable BDU */
  if (iis2dlpc_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  /* FIFO mode selection */
  if (iis2dlpc_fifo_mode_set(&(pObj->Ctx), IIS2DLPC_BYPASS_MODE) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  /* Power mode selection */
  if (iis2dlpc_power_mode_set(&(pObj->Ctx), IIS2DLPC_HIGH_PERFORMANCE) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = 100.0f;
  /* Select default operating mode. */
  pObj->acc_operating_mode = IIS2DLPC_HIGH_PERFORMANCE_MODE;
  /* Select default low noise (disabled). */
  pObj->acc_low_noise = IIS2DLPC_LOW_NOISE_DISABLE;

  /* Output data rate selection - power down. */
  if (iis2dlpc_data_rate_set(&(pObj->Ctx), IIS2DLPC_XL_ODR_OFF) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  /* Full scale selection. */
  if (iis2dlpc_full_scale_set(&(pObj->Ctx), IIS2DLPC_2g) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  pObj->is_initialized = 1;

  return IIS2DLPC_OK;
}

/**
  * @brief  Deinitialize the IIS2DLPC sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DLPC_DeInit(IIS2DLPC_Object_t *pObj)
{
  /* Disable the component */
  if (IIS2DLPC_ACC_Disable(pObj) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  /* Reset output data rate. */
  pObj->acc_odr = 0.0f;
  /* Reset operating mode to default value. */
  pObj->acc_operating_mode = IIS2DLPC_HIGH_PERFORMANCE_MODE;
  /* Reset low noise to default value (disabled). */
  pObj->acc_low_noise = IIS2DLPC_LOW_NOISE_DISABLE;

  pObj->is_initialized = 0;

  return IIS2DLPC_OK;
}

/**
  * @brief  Read component ID
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DLPC_ReadID(IIS2DLPC_Object_t *pObj, uint8_t *Id)
{
  if (iis2dlpc_device_id_get(&(pObj->Ctx), Id) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  return IIS2DLPC_OK;
}

/**
  * @brief  Get IIS2DLPC sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to IIS2DLPC sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DLPC_GetCapabilities(IIS2DLPC_Object_t *pObj, IIS2DLPC_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Acc          = 1;
  Capabilities->Gyro         = 0;
  Capabilities->Magneto      = 0;
  Capabilities->LowPower     = 0;
  Capabilities->GyroMaxFS    = 0;
  Capabilities->AccMaxFS     = 16;
  Capabilities->MagMaxFS     = 0;
  Capabilities->GyroMaxOdr   = 0.0f;
  Capabilities->AccMaxOdr    = 1600.0f;
  Capabilities->MagMaxOdr    = 0.0f;
  return IIS2DLPC_OK;
}

/**
  * @brief  Enable the IIS2DLPC accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DLPC_ACC_Enable(IIS2DLPC_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return IIS2DLPC_OK;
  }

  /* Output data rate selection. */
  if (IIS2DLPC_ACC_SetOutputDataRate_When_Enabled(pObj, pObj->acc_odr, pObj->acc_operating_mode,
                                                  pObj->acc_low_noise) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return IIS2DLPC_OK;
}

/**
  * @brief  Disable the IIS2DLPC accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DLPC_ACC_Disable(IIS2DLPC_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return IIS2DLPC_OK;
  }

  /* Output data rate selection - power down. */
  if (iis2dlpc_data_rate_set(&(pObj->Ctx), IIS2DLPC_XL_ODR_OFF) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return IIS2DLPC_OK;
}

/**
  * @brief  Get the IIS2DLPC accelerometer sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DLPC_ACC_GetSensitivity(IIS2DLPC_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = IIS2DLPC_OK;
  iis2dlpc_fs_t full_scale;
  iis2dlpc_mode_t mode;

  /* Read actual full scale selection from sensor. */
  if (iis2dlpc_full_scale_get(&(pObj->Ctx), &full_scale) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  /* Read actual power mode selection from sensor. */
  if (iis2dlpc_power_mode_get(&(pObj->Ctx), &mode) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  switch (mode)
  {
    case IIS2DLPC_CONT_LOW_PWR_12bit:
    case IIS2DLPC_SINGLE_LOW_PWR_12bit:
    case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_12bit:
    case IIS2DLPC_SINGLE_LOW_LOW_NOISE_PWR_12bit:
      switch (full_scale)
      {
        case IIS2DLPC_2g:
          *Sensitivity = IIS2DLPC_ACC_SENSITIVITY_FOR_FS_2G_LOPOW1_MODE;
          break;

        case IIS2DLPC_4g:
          *Sensitivity = IIS2DLPC_ACC_SENSITIVITY_FOR_FS_4G_LOPOW1_MODE;
          break;

        case IIS2DLPC_8g:
          *Sensitivity = IIS2DLPC_ACC_SENSITIVITY_FOR_FS_8G_LOPOW1_MODE;
          break;

        case IIS2DLPC_16g:
          *Sensitivity = IIS2DLPC_ACC_SENSITIVITY_FOR_FS_16G_LOPOW1_MODE;
          break;

        default:
          *Sensitivity = -1.0f;
          ret = IIS2DLPC_ERROR;
          break;
      }
      break;

    case IIS2DLPC_HIGH_PERFORMANCE:
    case IIS2DLPC_CONT_LOW_PWR_4:
    case IIS2DLPC_CONT_LOW_PWR_3:
    case IIS2DLPC_CONT_LOW_PWR_2:
    case IIS2DLPC_SINGLE_LOW_PWR_4:
    case IIS2DLPC_SINGLE_LOW_PWR_3:
    case IIS2DLPC_SINGLE_LOW_PWR_2:
    case IIS2DLPC_HIGH_PERFORMANCE_LOW_NOISE:
    case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_4:
    case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_3:
    case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_2:
    case IIS2DLPC_SINGLE_LOW_PWR_LOW_NOISE_4:
    case IIS2DLPC_SINGLE_LOW_PWR_LOW_NOISE_3:
    case IIS2DLPC_SINGLE_LOW_PWR_LOW_NOISE_2:
      switch (full_scale)
      {
        case IIS2DLPC_2g:
          *Sensitivity = IIS2DLPC_ACC_SENSITIVITY_FOR_FS_2G_OTHER_MODES;
          break;

        case IIS2DLPC_4g:
          *Sensitivity = IIS2DLPC_ACC_SENSITIVITY_FOR_FS_4G_OTHER_MODES;
          break;

        case IIS2DLPC_8g:
          *Sensitivity = IIS2DLPC_ACC_SENSITIVITY_FOR_FS_8G_OTHER_MODES;
          break;

        case IIS2DLPC_16g:
          *Sensitivity = IIS2DLPC_ACC_SENSITIVITY_FOR_FS_16G_OTHER_MODES;
          break;

        default:
          *Sensitivity = -1.0f;
          ret = IIS2DLPC_ERROR;
          break;
      }
      break;

    default:
      *Sensitivity = -1.0f;
      ret = IIS2DLPC_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the IIS2DLPC accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DLPC_ACC_GetOutputDataRate(IIS2DLPC_Object_t *pObj, float *Odr)
{
  int32_t ret = IIS2DLPC_OK;
  iis2dlpc_odr_t odr_low_level;
  iis2dlpc_mode_t mode;

  /* Get current output data rate. */
  if (iis2dlpc_data_rate_get(&(pObj->Ctx), &odr_low_level) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  /* Read actual power mode selection from sensor. */
  if (iis2dlpc_power_mode_get(&(pObj->Ctx), &mode) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  switch (odr_low_level)
  {
    case IIS2DLPC_XL_ODR_OFF:
    case IIS2DLPC_XL_SET_SW_TRIG:
    case IIS2DLPC_XL_SET_PIN_TRIG:
      *Odr = 0.0f;
      break;

    case IIS2DLPC_XL_ODR_1Hz6_LP_ONLY:
      switch (mode)
      {
        case IIS2DLPC_HIGH_PERFORMANCE:
        case IIS2DLPC_HIGH_PERFORMANCE_LOW_NOISE:
          *Odr = 12.5f;
          break;

        case IIS2DLPC_CONT_LOW_PWR_4:
        case IIS2DLPC_CONT_LOW_PWR_3:
        case IIS2DLPC_CONT_LOW_PWR_2:
        case IIS2DLPC_CONT_LOW_PWR_12bit:
        case IIS2DLPC_SINGLE_LOW_PWR_4:
        case IIS2DLPC_SINGLE_LOW_PWR_3:
        case IIS2DLPC_SINGLE_LOW_PWR_2:
        case IIS2DLPC_SINGLE_LOW_PWR_12bit:
        case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_4:
        case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_3:
        case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_2:
        case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_12bit:
        case IIS2DLPC_SINGLE_LOW_PWR_LOW_NOISE_4:
        case IIS2DLPC_SINGLE_LOW_PWR_LOW_NOISE_3:
        case IIS2DLPC_SINGLE_LOW_PWR_LOW_NOISE_2:
        case IIS2DLPC_SINGLE_LOW_LOW_NOISE_PWR_12bit:
          *Odr = 1.6f;
          break;

        default:
          *Odr = -1.0f;
          ret = IIS2DLPC_ERROR;
          break;
      }
      break;

    case IIS2DLPC_XL_ODR_12Hz5:
      *Odr = 12.5f;
      break;

    case IIS2DLPC_XL_ODR_25Hz:
      *Odr = 25.0f;
      break;

    case IIS2DLPC_XL_ODR_50Hz:
      *Odr = 50.0f;
      break;

    case IIS2DLPC_XL_ODR_100Hz:
      *Odr = 100.0f;
      break;

    case IIS2DLPC_XL_ODR_200Hz:
      *Odr = 200.0f;
      break;

    case IIS2DLPC_XL_ODR_400Hz:
      switch (mode)
      {
        case IIS2DLPC_HIGH_PERFORMANCE:
        case IIS2DLPC_HIGH_PERFORMANCE_LOW_NOISE:
          *Odr = 400.0f;
          break;

        case IIS2DLPC_CONT_LOW_PWR_4:
        case IIS2DLPC_CONT_LOW_PWR_3:
        case IIS2DLPC_CONT_LOW_PWR_2:
        case IIS2DLPC_CONT_LOW_PWR_12bit:
        case IIS2DLPC_SINGLE_LOW_PWR_4:
        case IIS2DLPC_SINGLE_LOW_PWR_3:
        case IIS2DLPC_SINGLE_LOW_PWR_2:
        case IIS2DLPC_SINGLE_LOW_PWR_12bit:
        case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_4:
        case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_3:
        case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_2:
        case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_12bit:
        case IIS2DLPC_SINGLE_LOW_PWR_LOW_NOISE_4:
        case IIS2DLPC_SINGLE_LOW_PWR_LOW_NOISE_3:
        case IIS2DLPC_SINGLE_LOW_PWR_LOW_NOISE_2:
        case IIS2DLPC_SINGLE_LOW_LOW_NOISE_PWR_12bit:
          *Odr = 200.0f;
          break;

        default:
          *Odr = -1.0f;
          ret = IIS2DLPC_ERROR;
          break;
      }
      break;

    case IIS2DLPC_XL_ODR_800Hz:
      switch (mode)
      {
        case IIS2DLPC_HIGH_PERFORMANCE:
        case IIS2DLPC_HIGH_PERFORMANCE_LOW_NOISE:
          *Odr = 800.0f;
          break;

        case IIS2DLPC_CONT_LOW_PWR_4:
        case IIS2DLPC_CONT_LOW_PWR_3:
        case IIS2DLPC_CONT_LOW_PWR_2:
        case IIS2DLPC_CONT_LOW_PWR_12bit:
        case IIS2DLPC_SINGLE_LOW_PWR_4:
        case IIS2DLPC_SINGLE_LOW_PWR_3:
        case IIS2DLPC_SINGLE_LOW_PWR_2:
        case IIS2DLPC_SINGLE_LOW_PWR_12bit:
        case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_4:
        case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_3:
        case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_2:
        case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_12bit:
        case IIS2DLPC_SINGLE_LOW_PWR_LOW_NOISE_4:
        case IIS2DLPC_SINGLE_LOW_PWR_LOW_NOISE_3:
        case IIS2DLPC_SINGLE_LOW_PWR_LOW_NOISE_2:
        case IIS2DLPC_SINGLE_LOW_LOW_NOISE_PWR_12bit:
          *Odr = 200.0f;
          break;

        default:
          *Odr = -1.0f;
          ret = IIS2DLPC_ERROR;
          break;
      }
      break;

    case IIS2DLPC_XL_ODR_1k6Hz:
      switch (mode)
      {
        case IIS2DLPC_HIGH_PERFORMANCE:
        case IIS2DLPC_HIGH_PERFORMANCE_LOW_NOISE:
          *Odr = 1600.0f;
          break;

        case IIS2DLPC_CONT_LOW_PWR_4:
        case IIS2DLPC_CONT_LOW_PWR_3:
        case IIS2DLPC_CONT_LOW_PWR_2:
        case IIS2DLPC_CONT_LOW_PWR_12bit:
        case IIS2DLPC_SINGLE_LOW_PWR_4:
        case IIS2DLPC_SINGLE_LOW_PWR_3:
        case IIS2DLPC_SINGLE_LOW_PWR_2:
        case IIS2DLPC_SINGLE_LOW_PWR_12bit:
        case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_4:
        case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_3:
        case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_2:
        case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_12bit:
        case IIS2DLPC_SINGLE_LOW_PWR_LOW_NOISE_4:
        case IIS2DLPC_SINGLE_LOW_PWR_LOW_NOISE_3:
        case IIS2DLPC_SINGLE_LOW_PWR_LOW_NOISE_2:
        case IIS2DLPC_SINGLE_LOW_LOW_NOISE_PWR_12bit:
          *Odr = 200.0f;
          break;

        default:
          *Odr = -1.0f;
          ret = IIS2DLPC_ERROR;
          break;
      }
      break;

    default:
      *Odr = -1.0f;
      ret = IIS2DLPC_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the IIS2DLPC accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DLPC_ACC_SetOutputDataRate(IIS2DLPC_Object_t *pObj, float Odr)
{
  /* By default we use High Performance mode and Low Noise disabled */
  return IIS2DLPC_ACC_SetOutputDataRate_With_Mode(pObj, Odr, IIS2DLPC_HIGH_PERFORMANCE_MODE, IIS2DLPC_LOW_NOISE_DISABLE);
}

/**
  * @brief  Set the IIS2DLPC accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @param  Mode the operating mode to be used
  * @param  Noise the low noise option
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DLPC_ACC_SetOutputDataRate_With_Mode(IIS2DLPC_Object_t *pObj, float Odr, IIS2DLPC_Operating_Mode_t Mode,
                                                 IIS2DLPC_Low_Noise_t Noise)
{
  /* Check if the component is enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return IIS2DLPC_ACC_SetOutputDataRate_When_Enabled(pObj, Odr, Mode, Noise);
  }
  else
  {
    return IIS2DLPC_ACC_SetOutputDataRate_When_Disabled(pObj, Odr, Mode, Noise);
  }
}

/**
  * @brief  Get the IIS2DLPC accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DLPC_ACC_GetFullScale(IIS2DLPC_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = IIS2DLPC_OK;
  iis2dlpc_fs_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (iis2dlpc_full_scale_get(&(pObj->Ctx), &fs_low_level) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  switch (fs_low_level)
  {
    case IIS2DLPC_2g:
      *FullScale =  2;
      break;

    case IIS2DLPC_4g:
      *FullScale =  4;
      break;

    case IIS2DLPC_8g:
      *FullScale =  8;
      break;

    case IIS2DLPC_16g:
      *FullScale = 16;
      break;

    default:
      *FullScale = -1;
      ret = IIS2DLPC_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the IIS2DLPC accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DLPC_ACC_SetFullScale(IIS2DLPC_Object_t *pObj, int32_t FullScale)
{
  iis2dlpc_fs_t new_fs;

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file statical analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  new_fs = (FullScale <= 2) ? IIS2DLPC_2g
           : (FullScale <= 4) ? IIS2DLPC_4g
           : (FullScale <= 8) ? IIS2DLPC_8g
           :                    IIS2DLPC_16g;

  if (iis2dlpc_full_scale_set(&(pObj->Ctx), new_fs) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  return IIS2DLPC_OK;
}

/**
  * @brief  Get the IIS2DLPC accelerometer sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DLPC_ACC_GetAxesRaw(IIS2DLPC_Object_t *pObj, IIS2DLPC_AxesRaw_t *Value)
{
  iis2dlpc_axis3bit16_t data_raw;
  iis2dlpc_mode_t mode;
  int32_t ret = IIS2DLPC_OK;

  /* Read actual power mode selection from sensor. */
  if (iis2dlpc_power_mode_get(&(pObj->Ctx), &mode) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  /* Read raw data values. */
  if (iis2dlpc_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  switch (mode)
  {
    case IIS2DLPC_CONT_LOW_PWR_12bit:
    case IIS2DLPC_SINGLE_LOW_PWR_12bit:
    case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_12bit:
    case IIS2DLPC_SINGLE_LOW_LOW_NOISE_PWR_12bit:
      /* Data format 12 bits. */
      Value->x = (data_raw.i16bit[0] / 16);
      Value->y = (data_raw.i16bit[1] / 16);
      Value->z = (data_raw.i16bit[2] / 16);
      break;

    case IIS2DLPC_HIGH_PERFORMANCE:
    case IIS2DLPC_CONT_LOW_PWR_4:
    case IIS2DLPC_CONT_LOW_PWR_3:
    case IIS2DLPC_CONT_LOW_PWR_2:
    case IIS2DLPC_SINGLE_LOW_PWR_4:
    case IIS2DLPC_SINGLE_LOW_PWR_3:
    case IIS2DLPC_SINGLE_LOW_PWR_2:
    case IIS2DLPC_HIGH_PERFORMANCE_LOW_NOISE:
    case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_4:
    case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_3:
    case IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_2:
    case IIS2DLPC_SINGLE_LOW_PWR_LOW_NOISE_4:
    case IIS2DLPC_SINGLE_LOW_PWR_LOW_NOISE_3:
    case IIS2DLPC_SINGLE_LOW_PWR_LOW_NOISE_2:
      /* Data format 14 bits. */
      Value->x = (data_raw.i16bit[0] / 4);
      Value->y = (data_raw.i16bit[1] / 4);
      Value->z = (data_raw.i16bit[2] / 4);
      break;

    default:
      ret = IIS2DLPC_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the IIS2DLPC accelerometer sensor axes
  * @param  pObj the device pObj
  * @param  Acceleration pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DLPC_ACC_GetAxes(IIS2DLPC_Object_t *pObj, IIS2DLPC_Axes_t *Acceleration)
{
  IIS2DLPC_AxesRaw_t data_raw;
  float sensitivity = 0.0f;

  /* Read raw data values. */
  if (IIS2DLPC_ACC_GetAxesRaw(pObj, &data_raw) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  /* Get IIS2DLPC actual sensitivity. */
  if (IIS2DLPC_ACC_GetSensitivity(pObj, &sensitivity) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  /* Calculate the data. */
  Acceleration->x = (int32_t)((float)((float)data_raw.x * sensitivity));
  Acceleration->y = (int32_t)((float)((float)data_raw.y * sensitivity));
  Acceleration->z = (int32_t)((float)((float)data_raw.z * sensitivity));

  return IIS2DLPC_OK;
}

/**
  * @brief  Get the IIS2DLPC register value
  * @param  pObj the device pObj
  * @param  Reg address to be read
  * @param  Data pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DLPC_Read_Reg(IIS2DLPC_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (iis2dlpc_read_reg(&(pObj->Ctx), Reg, Data, 1) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  return IIS2DLPC_OK;
}

/**
  * @brief  Set the IIS2DLPC register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DLPC_Write_Reg(IIS2DLPC_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (iis2dlpc_write_reg(&(pObj->Ctx), Reg, &Data, 1) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  return IIS2DLPC_OK;
}

/**
  * @brief  Get the IIS2DLPC ACC data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DLPC_ACC_Get_DRDY_Status(IIS2DLPC_Object_t *pObj, uint8_t *Status)
{
  if (iis2dlpc_flag_data_ready_get(&(pObj->Ctx), Status) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  return IIS2DLPC_OK;
}

/**
  * @}
  */

/** @defgroup IIS2DLPC_Private_Functions IIS2DLPC Private Functions
  * @{
  */

/**
  * @brief  Set the IIS2DLPC accelerometer sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @param  Mode the operating mode to be used
  * @param  Noise the low noise option
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t IIS2DLPC_ACC_SetOutputDataRate_When_Enabled(IIS2DLPC_Object_t *pObj, float Odr,
                                                           IIS2DLPC_Operating_Mode_t Mode, IIS2DLPC_Low_Noise_t Noise)
{
  iis2dlpc_odr_t new_odr;
  iis2dlpc_mode_t new_power_mode;

  switch (Mode)
  {
    case IIS2DLPC_HIGH_PERFORMANCE_MODE:
    default:
      switch (Noise)
      {
        case IIS2DLPC_LOW_NOISE_DISABLE:
        default:
          new_power_mode = IIS2DLPC_HIGH_PERFORMANCE;
          break;
        case IIS2DLPC_LOW_NOISE_ENABLE:
          new_power_mode = IIS2DLPC_HIGH_PERFORMANCE_LOW_NOISE;
          break;
      }

      /* If High Performance mode minimum ODR is 12.5Hz */
      if (Odr < 12.5f)
      {
        Odr = 12.5f;
      }
      break;
    case IIS2DLPC_LOW_POWER_MODE4:
      switch (Noise)
      {
        case IIS2DLPC_LOW_NOISE_DISABLE:
        default:
          new_power_mode = IIS2DLPC_CONT_LOW_PWR_4;
          break;
        case IIS2DLPC_LOW_NOISE_ENABLE:
          new_power_mode = IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_4;
          break;
      }

      /* If Low Power mode maximum ODR is 200Hz */
      if (Odr > 200.0f)
      {
        Odr = 200.0f;
      }
      break;
    case IIS2DLPC_LOW_POWER_MODE3:
      switch (Noise)
      {
        case IIS2DLPC_LOW_NOISE_DISABLE:
        default:
          new_power_mode = IIS2DLPC_CONT_LOW_PWR_3;
          break;
        case IIS2DLPC_LOW_NOISE_ENABLE:
          new_power_mode = IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_3;
          break;
      }

      /* If Low Power mode maximum ODR is 200Hz */
      if (Odr > 200.0f)
      {
        Odr = 200.0f;
      }
      break;
    case IIS2DLPC_LOW_POWER_MODE2:
      switch (Noise)
      {
        case IIS2DLPC_LOW_NOISE_DISABLE:
        default:
          new_power_mode = IIS2DLPC_CONT_LOW_PWR_2;
          break;
        case IIS2DLPC_LOW_NOISE_ENABLE:
          new_power_mode = IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_2;
          break;
      }

      /* If Low Power mode maximum ODR is 200Hz */
      if (Odr > 200.0f)
      {
        Odr = 200.0f;
      }
      break;
    case IIS2DLPC_LOW_POWER_MODE1:
      switch (Noise)
      {
        case IIS2DLPC_LOW_NOISE_DISABLE:
        default:
          new_power_mode = IIS2DLPC_CONT_LOW_PWR_12bit;
          break;
        case IIS2DLPC_LOW_NOISE_ENABLE:
          new_power_mode = IIS2DLPC_CONT_LOW_PWR_LOW_NOISE_12bit;
          break;
      }

      /* If Low Power mode maximum ODR is 200Hz */
      if (Odr > 200.0f)
      {
        Odr = 200.0f;
      }
      break;
  }

  new_odr = (Odr <=    1.6f) ? IIS2DLPC_XL_ODR_1Hz6_LP_ONLY
            : (Odr <=   12.5f) ? IIS2DLPC_XL_ODR_12Hz5
            : (Odr <=   25.0f) ? IIS2DLPC_XL_ODR_25Hz
            : (Odr <=   50.0f) ? IIS2DLPC_XL_ODR_50Hz
            : (Odr <=  100.0f) ? IIS2DLPC_XL_ODR_100Hz
            : (Odr <=  200.0f) ? IIS2DLPC_XL_ODR_200Hz
            : (Odr <=  400.0f) ? IIS2DLPC_XL_ODR_400Hz
            : (Odr <=  800.0f) ? IIS2DLPC_XL_ODR_800Hz
            :                    IIS2DLPC_XL_ODR_1k6Hz;

  /* Output data rate selection. */
  if (iis2dlpc_data_rate_set(&(pObj->Ctx), new_odr) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  /* Power mode selection. */
  if (iis2dlpc_power_mode_set(&(pObj->Ctx), new_power_mode) != IIS2DLPC_OK)
  {
    return IIS2DLPC_ERROR;
  }

  /* Store the current Odr, Mode and Noise values */
  pObj->acc_odr = Odr;
  pObj->acc_operating_mode = Mode;
  pObj->acc_low_noise = Noise;

  return IIS2DLPC_OK;
}

/**
  * @brief  Set the IIS2DLPC accelerometer sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @param  Mode the operating mode to be used
  * @param  Noise the low noise option
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t IIS2DLPC_ACC_SetOutputDataRate_When_Disabled(IIS2DLPC_Object_t *pObj, float Odr,
                                                            IIS2DLPC_Operating_Mode_t Mode, IIS2DLPC_Low_Noise_t Noise)
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

  return IIS2DLPC_OK;
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
  IIS2DLPC_Object_t *pObj = (IIS2DLPC_Object_t *)Handle;

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
  IIS2DLPC_Object_t *pObj = (IIS2DLPC_Object_t *)Handle;

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
