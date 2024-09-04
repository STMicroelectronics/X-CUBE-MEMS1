/**
  ******************************************************************************
  * @file    ais3624dq.c
  * @author  MEMS Software Solutions Team
  * @brief   AIS3624DQ driver file
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
#include "ais3624dq.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup AIS3624DQ AIS3624DQ
  * @{
  */

/** @defgroup AIS3624DQ_Exported_Variables AIS3624DQ Exported Variables
  * @{
  */

AIS3624DQ_CommonDrv_t AIS3624DQ_COMMON_Driver =
{
  AIS3624DQ_Init,
  AIS3624DQ_DeInit,
  AIS3624DQ_ReadID,
  AIS3624DQ_GetCapabilities,
};

AIS3624DQ_ACC_Drv_t AIS3624DQ_ACC_Driver =
{
  AIS3624DQ_ACC_Enable,
  AIS3624DQ_ACC_Disable,
  AIS3624DQ_ACC_GetSensitivity,
  AIS3624DQ_ACC_GetOutputDataRate,
  AIS3624DQ_ACC_SetOutputDataRate,
  AIS3624DQ_ACC_GetFullScale,
  AIS3624DQ_ACC_SetFullScale,
  AIS3624DQ_ACC_GetAxes,
  AIS3624DQ_ACC_GetAxesRaw,
};

/**
  * @}
  */

/** @defgroup AIS3624DQ_Private_Function_Prototypes AIS3624DQ Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t AIS3624DQ_ACC_SetOutputDataRate_When_Enabled(AIS3624DQ_Object_t *pObj, float Odr);
static int32_t AIS3624DQ_ACC_SetOutputDataRate_When_Disabled(AIS3624DQ_Object_t *pObj, float Odr);

/**
  * @}
  */

/** @defgroup AIS3624DQ_Exported_Functions AIS3624DQ Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS3624DQ_RegisterBusIO(AIS3624DQ_Object_t *pObj, AIS3624DQ_IO_t *pIO)
{
  int32_t ret = AIS3624DQ_OK;

  if (pObj == NULL)
  {
    ret = AIS3624DQ_ERROR;
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
      ret = AIS3624DQ_ERROR;
    }
    else if (pObj->IO.Init() != AIS3624DQ_OK)
    {
      ret = AIS3624DQ_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == AIS3624DQ_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          if (ais3624dq_spi_mode_set(&(pObj->Ctx), AIS3624DQ_SPI_3_WIRE) != AIS3624DQ_OK)
          {
            return AIS3624DQ_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  Initialize the AIS3624DQ sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS3624DQ_Init(AIS3624DQ_Object_t *pObj)
{
  /* Enable BDU */
  if (ais3624dq_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != AIS3624DQ_OK)
  {
    return AIS3624DQ_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = AIS3624DQ_ODR_100Hz;

  /* Output data rate selection - power down. */
  if (ais3624dq_data_rate_set(&(pObj->Ctx), AIS3624DQ_ODR_OFF) != AIS3624DQ_OK)
  {
    return AIS3624DQ_ERROR;
  }

  /* Full scale selection. */
  if (ais3624dq_full_scale_set(&(pObj->Ctx), AIS3624DQ_6g) != AIS3624DQ_OK)
  {
    return AIS3624DQ_ERROR;
  }

  pObj->is_initialized = 1;

  return AIS3624DQ_OK;
}

/**
  * @brief  Deinitialize the AIS3624DQ sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS3624DQ_DeInit(AIS3624DQ_Object_t *pObj)
{
  /* Disable the component */
  if (AIS3624DQ_ACC_Disable(pObj) != AIS3624DQ_OK)
  {
    return AIS3624DQ_ERROR;
  }

  /* Reset output data rate. */
  pObj->acc_odr = AIS3624DQ_ODR_OFF;

  pObj->is_initialized = 0;

  return AIS3624DQ_OK;
}

/**
  * @brief  Read component ID
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS3624DQ_ReadID(AIS3624DQ_Object_t *pObj, uint8_t *Id)
{
  if (ais3624dq_device_id_get(&(pObj->Ctx), Id) != AIS3624DQ_OK)
  {
    return AIS3624DQ_ERROR;
  }

  return AIS3624DQ_OK;
}

/**
  * @brief  Get AIS3624DQ sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to AIS3624DQ sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS3624DQ_GetCapabilities(AIS3624DQ_Object_t *pObj, AIS3624DQ_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Acc          = 1;
  Capabilities->Gyro         = 0;
  Capabilities->Magneto      = 0;
  Capabilities->LowPower     = 1;
  Capabilities->GyroMaxFS    = 0;
  Capabilities->AccMaxFS     = 24;
  Capabilities->MagMaxFS     = 0;
  Capabilities->GyroMaxOdr   = 0.0f;
  Capabilities->AccMaxOdr    = 1000.0f;
  Capabilities->MagMaxOdr    = 0.0f;
  return AIS3624DQ_OK;
}

/**
  * @brief  Enable the AIS3624DQ accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS3624DQ_ACC_Enable(AIS3624DQ_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return AIS3624DQ_OK;
  }

  /* Output data rate selection. */
  if (ais3624dq_data_rate_set(&(pObj->Ctx), pObj->acc_odr) != AIS3624DQ_OK)
  {
    return AIS3624DQ_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return AIS3624DQ_OK;
}

/**
  * @brief  Disable the AIS3624DQ accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS3624DQ_ACC_Disable(AIS3624DQ_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return AIS3624DQ_OK;
  }

  /* Get current output data rate. */
  if (ais3624dq_data_rate_get(&(pObj->Ctx), &pObj->acc_odr) != AIS3624DQ_OK)
  {
    return AIS3624DQ_ERROR;
  }

  /* Output data rate selection - power down. */
  if (ais3624dq_data_rate_set(&(pObj->Ctx), AIS3624DQ_ODR_OFF) != AIS3624DQ_OK)
  {
    return AIS3624DQ_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return AIS3624DQ_OK;
}

/**
  * @brief  Get the AIS3624DQ accelerometer sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS3624DQ_ACC_GetSensitivity(AIS3624DQ_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = AIS3624DQ_OK;
  ais3624dq_fs_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (ais3624dq_full_scale_get(&(pObj->Ctx), &full_scale) != AIS3624DQ_OK)
  {
    return AIS3624DQ_ERROR;
  }

  switch (full_scale)
  {
    case AIS3624DQ_6g:
      *Sensitivity = AIS3624DQ_ACC_SENSITIVITY_FOR_FS_6G;
      break;

    case AIS3624DQ_12g:
      *Sensitivity = AIS3624DQ_ACC_SENSITIVITY_FOR_FS_12G;
      break;

    case AIS3624DQ_24g:
      *Sensitivity = AIS3624DQ_ACC_SENSITIVITY_FOR_FS_24G;
      break;

    default:
      *Sensitivity = -1.0f;
      ret = AIS3624DQ_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the AIS3624DQ accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS3624DQ_ACC_GetOutputDataRate(AIS3624DQ_Object_t *pObj, float *Odr)
{
  int32_t ret = AIS3624DQ_OK;
  ais3624dq_dr_t odr_low_level;

  /* Get current output data rate. */
  if (ais3624dq_data_rate_get(&(pObj->Ctx), &odr_low_level) != AIS3624DQ_OK)
  {
    return AIS3624DQ_ERROR;
  }

  switch (odr_low_level)
  {
    case AIS3624DQ_ODR_OFF:
      *Odr = 0.0f;
      break;

    case AIS3624DQ_ODR_Hz5:
      *Odr = 0.5f;
      break;

    case AIS3624DQ_ODR_1Hz:
      *Odr = 1.0f;
      break;

    case AIS3624DQ_ODR_5Hz2:
      *Odr = 2.0f;
      break;

    case AIS3624DQ_ODR_5Hz:
      *Odr = 5.0f;
      break;

    case AIS3624DQ_ODR_10Hz:
      *Odr = 10.0f;
      break;

    case AIS3624DQ_ODR_50Hz:
      *Odr = 50.0f;
      break;

    case AIS3624DQ_ODR_100Hz:
      *Odr = 100.0f;
      break;

    case AIS3624DQ_ODR_400Hz:
      *Odr = 400.0f;
      break;

    case AIS3624DQ_ODR_1kHz:
      *Odr = 1000.0f;
      break;

    default:
      *Odr = -1.0f;
      ret = AIS3624DQ_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the AIS3624DQ accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS3624DQ_ACC_SetOutputDataRate(AIS3624DQ_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return AIS3624DQ_ACC_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return AIS3624DQ_ACC_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the AIS3624DQ accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS3624DQ_ACC_GetFullScale(AIS3624DQ_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = AIS3624DQ_OK;
  ais3624dq_fs_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (ais3624dq_full_scale_get(&(pObj->Ctx), &fs_low_level) != AIS3624DQ_OK)
  {
    return AIS3624DQ_ERROR;
  }

  switch (fs_low_level)
  {
    case AIS3624DQ_6g:
      *FullScale = 6;
      break;

    case AIS3624DQ_12g:
      *FullScale = 12;
      break;

    case AIS3624DQ_24g:
      *FullScale = 24;
      break;

    default:
      *FullScale = -1;
      ret = AIS3624DQ_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the AIS3624DQ accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS3624DQ_ACC_SetFullScale(AIS3624DQ_Object_t *pObj, int32_t FullScale)
{
  ais3624dq_fs_t new_fs;

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file statical analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  new_fs = (FullScale <=  6) ? AIS3624DQ_6g
           : (FullScale <= 12) ? AIS3624DQ_12g
           :                     AIS3624DQ_24g;

  if (ais3624dq_full_scale_set(&(pObj->Ctx), new_fs) != AIS3624DQ_OK)
  {
    return AIS3624DQ_ERROR;
  }

  return AIS3624DQ_OK;
}

/**
  * @brief  Get the AIS3624DQ accelerometer sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS3624DQ_ACC_GetAxesRaw(AIS3624DQ_Object_t *pObj, AIS3624DQ_AxesRaw_t *Value)
{
  ais3624dq_axis3bit16_t data_raw;
  uint8_t multi_read;

  /* Read raw data values. */
  multi_read = (pObj->IO.BusType == AIS3624DQ_I2C_BUS)        ? 0x80
               : (pObj->IO.BusType == AIS3624DQ_SPI_4WIRES_BUS) ? 0x40
               : (pObj->IO.BusType == AIS3624DQ_SPI_3WIRES_BUS) ? 0x40
               :                                                  0x00;

  if (ais3624dq_read_reg(&(pObj->Ctx), AIS3624DQ_OUT_X_L | multi_read, data_raw.u8bit, 6) != AIS3624DQ_OK)
  {
    return AIS3624DQ_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0] >> 4;
  Value->y = data_raw.i16bit[1] >> 4;
  Value->z = data_raw.i16bit[2] >> 4;

  return AIS3624DQ_OK;
}

/**
  * @brief  Get the AIS3624DQ accelerometer sensor axes
  * @param  pObj the device pObj
  * @param  Acceleration pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS3624DQ_ACC_GetAxes(AIS3624DQ_Object_t *pObj, AIS3624DQ_Axes_t *Acceleration)
{
  AIS3624DQ_AxesRaw_t data_raw;
  float sensitivity = 0.0f;

  /* Read raw data values. */
  if (AIS3624DQ_ACC_GetAxesRaw(pObj, &data_raw) != AIS3624DQ_OK)
  {
    return AIS3624DQ_ERROR;
  }

  /* Get AIS3624DQ actual sensitivity. */
  if (AIS3624DQ_ACC_GetSensitivity(pObj, &sensitivity) != AIS3624DQ_OK)
  {
    return AIS3624DQ_ERROR;
  }

  /* Calculate the data. */
  Acceleration->x = (int32_t)((float)((float)data_raw.x * sensitivity));
  Acceleration->y = (int32_t)((float)((float)data_raw.y * sensitivity));
  Acceleration->z = (int32_t)((float)((float)data_raw.z * sensitivity));

  return AIS3624DQ_OK;
}

/**
  * @brief  Get the AIS3624DQ register value
  * @param  pObj the device pObj
  * @param  Reg address to be read
  * @param  Data pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS3624DQ_Read_Reg(AIS3624DQ_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (ais3624dq_read_reg(&(pObj->Ctx), Reg, Data, 1) != AIS3624DQ_OK)
  {
    return AIS3624DQ_ERROR;
  }

  return AIS3624DQ_OK;
}

/**
  * @brief  Set the AIS3624DQ register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS3624DQ_Write_Reg(AIS3624DQ_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (ais3624dq_write_reg(&(pObj->Ctx), Reg, &Data, 1) != AIS3624DQ_OK)
  {
    return AIS3624DQ_ERROR;
  }

  return AIS3624DQ_OK;
}

/**
  * @brief  Get the AIS3624DQ ACC data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS3624DQ_ACC_Get_DRDY_Status(AIS3624DQ_Object_t *pObj, uint8_t *Status)
{
  if (ais3624dq_flag_data_ready_get(&(pObj->Ctx), Status) != AIS3624DQ_OK)
  {
    return AIS3624DQ_ERROR;
  }

  return AIS3624DQ_OK;
}

/**
  * @}
  */

/** @defgroup AIS3624DQ_Private_Functions AIS3624DQ Private Functions
  * @{
  */

/**
  * @brief  Set the AIS3624DQ accelerometer sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t AIS3624DQ_ACC_SetOutputDataRate_When_Enabled(AIS3624DQ_Object_t *pObj, float Odr)
{
  ais3624dq_dr_t new_odr;

  new_odr = (Odr <=   0.5f) ? AIS3624DQ_ODR_Hz5
            : (Odr <=   1.0f) ? AIS3624DQ_ODR_1Hz
            : (Odr <=   2.0f) ? AIS3624DQ_ODR_5Hz2
            : (Odr <=   5.0f) ? AIS3624DQ_ODR_5Hz
            : (Odr <=  10.0f) ? AIS3624DQ_ODR_10Hz
            : (Odr <=  50.0f) ? AIS3624DQ_ODR_50Hz
            : (Odr <= 100.0f) ? AIS3624DQ_ODR_100Hz
            : (Odr <= 400.0f) ? AIS3624DQ_ODR_400Hz
            :                   AIS3624DQ_ODR_1kHz;

  /* Output data rate selection. */
  if (ais3624dq_data_rate_set(&(pObj->Ctx), new_odr) != AIS3624DQ_OK)
  {
    return AIS3624DQ_ERROR;
  }

  /* Store the current output data rate value */
  pObj->acc_odr = new_odr;

  return AIS3624DQ_OK;
}

/**
  * @brief  Set the AIS3624DQ accelerometer sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t AIS3624DQ_ACC_SetOutputDataRate_When_Disabled(AIS3624DQ_Object_t *pObj, float Odr)
{
  /* Store the new output data rate value */
  pObj->acc_odr = (Odr <=   0.5f) ? AIS3624DQ_ODR_Hz5
                  : (Odr <=   1.0f) ? AIS3624DQ_ODR_1Hz
                  : (Odr <=   2.0f) ? AIS3624DQ_ODR_5Hz2
                  : (Odr <=   5.0f) ? AIS3624DQ_ODR_5Hz
                  : (Odr <=  10.0f) ? AIS3624DQ_ODR_10Hz
                  : (Odr <=  50.0f) ? AIS3624DQ_ODR_50Hz
                  : (Odr <= 100.0f) ? AIS3624DQ_ODR_100Hz
                  : (Odr <= 400.0f) ? AIS3624DQ_ODR_400Hz
                  :                   AIS3624DQ_ODR_1kHz;

  return AIS3624DQ_OK;
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
  AIS3624DQ_Object_t *pObj = (AIS3624DQ_Object_t *)Handle;

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
  AIS3624DQ_Object_t *pObj = (AIS3624DQ_Object_t *)Handle;

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
