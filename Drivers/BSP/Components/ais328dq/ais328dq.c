/**
  ******************************************************************************
  * @file    ais328dq.c
  * @author  MEMS Software Solutions Team
  * @brief   AIS328DQ driver file
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
#include "ais328dq.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup AIS328DQ AIS328DQ
  * @{
  */

/** @defgroup AIS328DQ_Exported_Variables AIS328DQ Exported Variables
  * @{
  */

AIS328DQ_CommonDrv_t AIS328DQ_COMMON_Driver =
{
  AIS328DQ_Init,
  AIS328DQ_DeInit,
  AIS328DQ_ReadID,
  AIS328DQ_GetCapabilities,
};

AIS328DQ_ACC_Drv_t AIS328DQ_ACC_Driver =
{
  AIS328DQ_ACC_Enable,
  AIS328DQ_ACC_Disable,
  AIS328DQ_ACC_GetSensitivity,
  AIS328DQ_ACC_GetOutputDataRate,
  AIS328DQ_ACC_SetOutputDataRate,
  AIS328DQ_ACC_GetFullScale,
  AIS328DQ_ACC_SetFullScale,
  AIS328DQ_ACC_GetAxes,
  AIS328DQ_ACC_GetAxesRaw,
};

/**
  * @}
  */

/** @defgroup AIS328DQ_Private_Function_Prototypes AIS328DQ Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t AIS328DQ_ACC_SetOutputDataRate_When_Enabled(AIS328DQ_Object_t *pObj, float Odr);
static int32_t AIS328DQ_ACC_SetOutputDataRate_When_Disabled(AIS328DQ_Object_t *pObj, float Odr);

/**
  * @}
  */

/** @defgroup AIS328DQ_Exported_Functions AIS328DQ Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS328DQ_RegisterBusIO(AIS328DQ_Object_t *pObj, AIS328DQ_IO_t *pIO)
{
  int32_t ret = AIS328DQ_OK;

  if (pObj == NULL)
  {
    ret = AIS328DQ_ERROR;
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

    if (pObj->IO.Init == NULL)
    {
      ret = AIS328DQ_ERROR;
    }
    else if (pObj->IO.Init() != AIS328DQ_OK)
    {
      ret = AIS328DQ_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == AIS328DQ_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          if (ais328dq_spi_mode_set(&(pObj->Ctx), AIS328DQ_SPI_3_WIRE) != AIS328DQ_OK)
          {
            return AIS328DQ_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  Initialize the AIS328DQ sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS328DQ_Init(AIS328DQ_Object_t *pObj)
{
  /* Enable BDU */
  if (ais328dq_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != AIS328DQ_OK)
  {
    return AIS328DQ_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = AIS328DQ_ODR_100Hz;

  /* Output data rate selection - power down. */
  if (ais328dq_data_rate_set(&(pObj->Ctx), AIS328DQ_ODR_OFF) != AIS328DQ_OK)
  {
    return AIS328DQ_ERROR;
  }

  /* Full scale selection. */
  if (ais328dq_full_scale_set(&(pObj->Ctx), AIS328DQ_2g) != AIS328DQ_OK)
  {
    return AIS328DQ_ERROR;
  }

  pObj->is_initialized = 1;

  return AIS328DQ_OK;
}

/**
  * @brief  Deinitialize the AIS328DQ sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS328DQ_DeInit(AIS328DQ_Object_t *pObj)
{
  /* Disable the component */
  if (AIS328DQ_ACC_Disable(pObj) != AIS328DQ_OK)
  {
    return AIS328DQ_ERROR;
  }

  /* Reset output data rate. */
  pObj->acc_odr = AIS328DQ_ODR_OFF;

  pObj->is_initialized = 0;

  return AIS328DQ_OK;
}

/**
  * @brief  Read component ID
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS328DQ_ReadID(AIS328DQ_Object_t *pObj, uint8_t *Id)
{
  if (ais328dq_device_id_get(&(pObj->Ctx), Id) != AIS328DQ_OK)
  {
    return AIS328DQ_ERROR;
  }

  return AIS328DQ_OK;
}

/**
  * @brief  Get AIS328DQ sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to AIS328DQ sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS328DQ_GetCapabilities(AIS328DQ_Object_t *pObj, AIS328DQ_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Acc          = 1;
  Capabilities->Gyro         = 0;
  Capabilities->Magneto      = 0;
  Capabilities->LowPower     = 1;
  Capabilities->GyroMaxFS    = 0;
  Capabilities->AccMaxFS     = 8;
  Capabilities->MagMaxFS     = 0;
  Capabilities->GyroMaxOdr   = 0.0f;
  Capabilities->AccMaxOdr    = 1000.0f;
  Capabilities->MagMaxOdr    = 0.0f;
  return AIS328DQ_OK;
}

/**
  * @brief  Enable the AIS328DQ accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS328DQ_ACC_Enable(AIS328DQ_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return AIS328DQ_OK;
  }

  /* Output data rate selection. */
  if (ais328dq_data_rate_set(&(pObj->Ctx), pObj->acc_odr) != AIS328DQ_OK)
  {
    return AIS328DQ_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return AIS328DQ_OK;
}

/**
  * @brief  Disable the AIS328DQ accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS328DQ_ACC_Disable(AIS328DQ_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return AIS328DQ_OK;
  }

  /* Get current output data rate. */
  if (ais328dq_data_rate_get(&(pObj->Ctx), &pObj->acc_odr) != AIS328DQ_OK)
  {
    return AIS328DQ_ERROR;
  }

  /* Output data rate selection - power down. */
  if (ais328dq_data_rate_set(&(pObj->Ctx), AIS328DQ_ODR_OFF) != AIS328DQ_OK)
  {
    return AIS328DQ_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return AIS328DQ_OK;
}

/**
  * @brief  Get the AIS328DQ accelerometer sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS328DQ_ACC_GetSensitivity(AIS328DQ_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = AIS328DQ_OK;
  ais328dq_fs_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (ais328dq_full_scale_get(&(pObj->Ctx), &full_scale) != AIS328DQ_OK)
  {
    return AIS328DQ_ERROR;
  }

  switch (full_scale)
  {
    case AIS328DQ_2g:
      *Sensitivity = AIS328DQ_ACC_SENSITIVITY_FOR_FS_2G;
      break;

    case AIS328DQ_4g:
      *Sensitivity = AIS328DQ_ACC_SENSITIVITY_FOR_FS_4G;
      break;

    case AIS328DQ_8g:
      *Sensitivity = AIS328DQ_ACC_SENSITIVITY_FOR_FS_8G;
      break;

    default:
      *Sensitivity = -1.0f;
      ret = AIS328DQ_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the AIS328DQ accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS328DQ_ACC_GetOutputDataRate(AIS328DQ_Object_t *pObj, float *Odr)
{
  int32_t ret = AIS328DQ_OK;
  ais328dq_dr_t odr_low_level;

  /* Get current output data rate. */
  if (ais328dq_data_rate_get(&(pObj->Ctx), &odr_low_level) != AIS328DQ_OK)
  {
    return AIS328DQ_ERROR;
  }

  switch (odr_low_level)
  {
    case AIS328DQ_ODR_OFF:
      *Odr = 0.0f;
      break;

    case AIS328DQ_ODR_Hz5:
      *Odr = 0.5f;
      break;

    case AIS328DQ_ODR_1Hz:
      *Odr = 1.0f;
      break;

    case AIS328DQ_ODR_2Hz:
      *Odr = 2.0f;
      break;

    case AIS328DQ_ODR_5Hz:
      *Odr = 5.0f;
      break;

    case AIS328DQ_ODR_10Hz:
      *Odr = 10.0f;
      break;

    case AIS328DQ_ODR_50Hz:
      *Odr = 50.0f;
      break;

    case AIS328DQ_ODR_100Hz:
      *Odr = 100.0f;
      break;

    case AIS328DQ_ODR_400Hz:
      *Odr = 400.0f;
      break;

    case AIS328DQ_ODR_1kHz:
      *Odr = 1000.0f;
      break;

    default:
      *Odr = -1.0f;
      ret = AIS328DQ_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the AIS328DQ accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS328DQ_ACC_SetOutputDataRate(AIS328DQ_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return AIS328DQ_ACC_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return AIS328DQ_ACC_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the AIS328DQ accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS328DQ_ACC_GetFullScale(AIS328DQ_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = AIS328DQ_OK;
  ais328dq_fs_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (ais328dq_full_scale_get(&(pObj->Ctx), &fs_low_level) != AIS328DQ_OK)
  {
    return AIS328DQ_ERROR;
  }

  switch (fs_low_level)
  {
    case AIS328DQ_2g:
      *FullScale =  2;
      break;

    case AIS328DQ_4g:
      *FullScale =  4;
      break;

    case AIS328DQ_8g:
      *FullScale =  8;
      break;

    default:
      *FullScale = -1;
      ret = AIS328DQ_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the AIS328DQ accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS328DQ_ACC_SetFullScale(AIS328DQ_Object_t *pObj, int32_t FullScale)
{
  ais328dq_fs_t new_fs;

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file statical analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  new_fs = (FullScale <= 2) ? AIS328DQ_2g
           : (FullScale <= 4) ? AIS328DQ_4g
           :                    AIS328DQ_8g;

  if (ais328dq_full_scale_set(&(pObj->Ctx), new_fs) != AIS328DQ_OK)
  {
    return AIS328DQ_ERROR;
  }

  return AIS328DQ_OK;
}

/**
  * @brief  Get the AIS328DQ accelerometer sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS328DQ_ACC_GetAxesRaw(AIS328DQ_Object_t *pObj, AIS328DQ_AxesRaw_t *Value)
{
  ais328dq_axis3bit16_t data_raw;
  uint8_t multi_read;

  /* Read raw data values. */
  multi_read = (pObj->IO.BusType == AIS328DQ_I2C_BUS)        ? 0x80
               : (pObj->IO.BusType == AIS328DQ_SPI_4WIRES_BUS) ? 0x40
               : (pObj->IO.BusType == AIS328DQ_SPI_3WIRES_BUS) ? 0x40
               :                                                 0x00;

  if (ais328dq_read_reg(&(pObj->Ctx), AIS328DQ_OUT_X_L | multi_read, data_raw.u8bit, 6) != AIS328DQ_OK)
  {
    return AIS328DQ_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0] >> 4;
  Value->y = data_raw.i16bit[1] >> 4;
  Value->z = data_raw.i16bit[2] >> 4;

  return AIS328DQ_OK;
}

/**
  * @brief  Get the AIS328DQ accelerometer sensor axes
  * @param  pObj the device pObj
  * @param  Acceleration pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS328DQ_ACC_GetAxes(AIS328DQ_Object_t *pObj, AIS328DQ_Axes_t *Acceleration)
{
  AIS328DQ_AxesRaw_t data_raw;
  float sensitivity = 0.0f;

  /* Read raw data values. */
  if (AIS328DQ_ACC_GetAxesRaw(pObj, &data_raw) != AIS328DQ_OK)
  {
    return AIS328DQ_ERROR;
  }

  /* Get AIS328DQ actual sensitivity. */
  if (AIS328DQ_ACC_GetSensitivity(pObj, &sensitivity) != AIS328DQ_OK)
  {
    return AIS328DQ_ERROR;
  }

  /* Calculate the data. */
  Acceleration->x = (int32_t)((float)((float)data_raw.x * sensitivity));
  Acceleration->y = (int32_t)((float)((float)data_raw.y * sensitivity));
  Acceleration->z = (int32_t)((float)((float)data_raw.z * sensitivity));

  return AIS328DQ_OK;
}

/**
  * @brief  Get the AIS328DQ register value
  * @param  pObj the device pObj
  * @param  Reg address to be read
  * @param  Data pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS328DQ_Read_Reg(AIS328DQ_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (ais328dq_read_reg(&(pObj->Ctx), Reg, Data, 1) != AIS328DQ_OK)
  {
    return AIS328DQ_ERROR;
  }

  return AIS328DQ_OK;
}

/**
  * @brief  Set the AIS328DQ register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS328DQ_Write_Reg(AIS328DQ_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (ais328dq_write_reg(&(pObj->Ctx), Reg, &Data, 1) != AIS328DQ_OK)
  {
    return AIS328DQ_ERROR;
  }

  return AIS328DQ_OK;
}

/**
  * @brief  Get the AIS328DQ ACC data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS328DQ_ACC_Get_DRDY_Status(AIS328DQ_Object_t *pObj, uint8_t *Status)
{
  if (ais328dq_flag_data_ready_get(&(pObj->Ctx), Status) != AIS328DQ_OK)
  {
    return AIS328DQ_ERROR;
  }

  return AIS328DQ_OK;
}

/**
  * @}
  */

/** @defgroup AIS328DQ_Private_Functions AIS328DQ Private Functions
  * @{
  */

/**
  * @brief  Set the AIS328DQ accelerometer sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t AIS328DQ_ACC_SetOutputDataRate_When_Enabled(AIS328DQ_Object_t *pObj, float Odr)
{
  ais328dq_dr_t new_odr;

  new_odr = (Odr <=   0.5f) ? AIS328DQ_ODR_Hz5
            : (Odr <=   1.0f) ? AIS328DQ_ODR_1Hz
            : (Odr <=   2.0f) ? AIS328DQ_ODR_2Hz
            : (Odr <=   5.0f) ? AIS328DQ_ODR_5Hz
            : (Odr <=  10.0f) ? AIS328DQ_ODR_10Hz
            : (Odr <=  50.0f) ? AIS328DQ_ODR_50Hz
            : (Odr <= 100.0f) ? AIS328DQ_ODR_100Hz
            : (Odr <= 400.0f) ? AIS328DQ_ODR_400Hz
            :                   AIS328DQ_ODR_1kHz;

  /* Output data rate selection. */
  if (ais328dq_data_rate_set(&(pObj->Ctx), new_odr) != AIS328DQ_OK)
  {
    return AIS328DQ_ERROR;
  }

  /* Store the current output data rate value */
  pObj->acc_odr = new_odr;

  return AIS328DQ_OK;
}

/**
  * @brief  Set the AIS328DQ accelerometer sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t AIS328DQ_ACC_SetOutputDataRate_When_Disabled(AIS328DQ_Object_t *pObj, float Odr)
{
  /* Store the new output data rate value */
  pObj->acc_odr = (Odr <=   0.5f) ? AIS328DQ_ODR_Hz5
                  : (Odr <=   1.0f) ? AIS328DQ_ODR_1Hz
                  : (Odr <=   2.0f) ? AIS328DQ_ODR_2Hz
                  : (Odr <=   5.0f) ? AIS328DQ_ODR_5Hz
                  : (Odr <=  10.0f) ? AIS328DQ_ODR_10Hz
                  : (Odr <=  50.0f) ? AIS328DQ_ODR_50Hz
                  : (Odr <= 100.0f) ? AIS328DQ_ODR_100Hz
                  : (Odr <= 400.0f) ? AIS328DQ_ODR_400Hz
                  :                   AIS328DQ_ODR_1kHz;

  return AIS328DQ_OK;
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
  AIS328DQ_Object_t *pObj = (AIS328DQ_Object_t *)Handle;

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
  AIS328DQ_Object_t *pObj = (AIS328DQ_Object_t *)Handle;

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
