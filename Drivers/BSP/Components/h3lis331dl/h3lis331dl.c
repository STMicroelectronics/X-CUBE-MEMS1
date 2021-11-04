/**
 ******************************************************************************
 * @file    h3lis331dl.c
 * @author  MEMS Software Solutions Team
 * @brief   H3LIS331DL driver file
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "h3lis331dl.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @defgroup H3LIS331DL H3LIS331DL
 * @{
 */

/** @defgroup H3LIS331DL_Exported_Variables H3LIS331DL Exported Variables
 * @{
 */

H3LIS331DL_CommonDrv_t H3LIS331DL_COMMON_Driver =
{
  H3LIS331DL_Init,
  H3LIS331DL_DeInit,
  H3LIS331DL_ReadID,
  H3LIS331DL_GetCapabilities,
};

H3LIS331DL_ACC_Drv_t H3LIS331DL_ACC_Driver =
{
  H3LIS331DL_ACC_Enable,
  H3LIS331DL_ACC_Disable,
  H3LIS331DL_ACC_GetSensitivity,
  H3LIS331DL_ACC_GetOutputDataRate,
  H3LIS331DL_ACC_SetOutputDataRate,
  H3LIS331DL_ACC_GetFullScale,
  H3LIS331DL_ACC_SetFullScale,
  H3LIS331DL_ACC_GetAxes,
  H3LIS331DL_ACC_GetAxesRaw,
};

/**
 * @}
 */

/** @defgroup H3LIS331DL_Private_Function_Prototypes H3LIS331DL Private Function Prototypes
 * @{
 */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t H3LIS331DL_ACC_SetOutputDataRate_When_Enabled(H3LIS331DL_Object_t *pObj, float Odr);
static int32_t H3LIS331DL_ACC_SetOutputDataRate_When_Disabled(H3LIS331DL_Object_t *pObj, float Odr);

/**
 * @}
 */

/** @defgroup H3LIS331DL_Exported_Functions H3LIS331DL Exported Functions
 * @{
 */

/**
 * @brief  Register Component Bus IO operations
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t H3LIS331DL_RegisterBusIO(H3LIS331DL_Object_t *pObj, H3LIS331DL_IO_t *pIO)
{
  int32_t ret = H3LIS331DL_OK;

  if (pObj == NULL)
  {
    ret = H3LIS331DL_ERROR;
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
      ret = H3LIS331DL_ERROR;
    }
    else if (pObj->IO.Init() != H3LIS331DL_OK)
    {
      ret = H3LIS331DL_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == H3LIS331DL_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          if (h3lis331dl_spi_mode_set(&(pObj->Ctx), H3LIS331DL_SPI_3_WIRE) != H3LIS331DL_OK)
          {
            return H3LIS331DL_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
 * @brief  Initialize the H3LIS331DL sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t H3LIS331DL_Init(H3LIS331DL_Object_t *pObj)
{
  /* Enable BDU */
  if (h3lis331dl_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != H3LIS331DL_OK)
  {
    return H3LIS331DL_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = H3LIS331DL_ODR_100Hz;

  /* Output data rate selection - power down. */
  if (h3lis331dl_data_rate_set(&(pObj->Ctx), H3LIS331DL_ODR_OFF) != H3LIS331DL_OK)
  {
    return H3LIS331DL_ERROR;
  }

  /* Full scale selection. */
  if (h3lis331dl_full_scale_set(&(pObj->Ctx), H3LIS331DL_100g) != H3LIS331DL_OK)
  {
    return H3LIS331DL_ERROR;
  }

  pObj->is_initialized = 1;

  return H3LIS331DL_OK;
}

/**
 * @brief  Deinitialize the H3LIS331DL sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t H3LIS331DL_DeInit(H3LIS331DL_Object_t *pObj)
{
  /* Disable the component */
  if (H3LIS331DL_ACC_Disable(pObj) != H3LIS331DL_OK)
  {
    return H3LIS331DL_ERROR;
  }

  /* Reset output data rate. */
  pObj->acc_odr = H3LIS331DL_ODR_OFF;

  pObj->is_initialized = 0;

  return H3LIS331DL_OK;
}

/**
 * @brief  Read component ID
 * @param  pObj the device pObj
 * @param  Id the WHO_AM_I value
 * @retval 0 in case of success, an error code otherwise
 */
int32_t H3LIS331DL_ReadID(H3LIS331DL_Object_t *pObj, uint8_t *Id)
{
  if (h3lis331dl_device_id_get(&(pObj->Ctx), Id) != H3LIS331DL_OK)
  {
    return H3LIS331DL_ERROR;
  }

  return H3LIS331DL_OK;
}

/**
 * @brief  Get H3LIS331DL sensor capabilities
 * @param  pObj Component object pointer
 * @param  Capabilities pointer to H3LIS331DL sensor capabilities
 * @retval 0 in case of success, an error code otherwise
 */
int32_t H3LIS331DL_GetCapabilities(H3LIS331DL_Object_t *pObj, H3LIS331DL_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Acc          = 1;
  Capabilities->Gyro         = 0;
  Capabilities->Magneto      = 0;
  Capabilities->LowPower     = 1;
  Capabilities->GyroMaxFS    = 0;
  Capabilities->AccMaxFS     = 400;
  Capabilities->MagMaxFS     = 0;
  Capabilities->GyroMaxOdr   = 0.0f;
  Capabilities->AccMaxOdr    = 1000.0f;
  Capabilities->MagMaxOdr    = 0.0f;
  return H3LIS331DL_OK;
}

/**
 * @brief  Enable the H3LIS331DL accelerometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t H3LIS331DL_ACC_Enable(H3LIS331DL_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return H3LIS331DL_OK;
  }

  /* Output data rate selection. */
  if (h3lis331dl_data_rate_set(&(pObj->Ctx), pObj->acc_odr) != H3LIS331DL_OK)
  {
    return H3LIS331DL_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return H3LIS331DL_OK;
}

/**
 * @brief  Disable the H3LIS331DL accelerometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t H3LIS331DL_ACC_Disable(H3LIS331DL_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return H3LIS331DL_OK;
  }

  /* Get current output data rate. */
  if (h3lis331dl_data_rate_get(&(pObj->Ctx), &pObj->acc_odr) != H3LIS331DL_OK)
  {
    return H3LIS331DL_ERROR;
  }

  /* Output data rate selection - power down. */
  if (h3lis331dl_data_rate_set(&(pObj->Ctx), H3LIS331DL_ODR_OFF) != H3LIS331DL_OK)
  {
    return H3LIS331DL_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return H3LIS331DL_OK;
}

/**
 * @brief  Get the H3LIS331DL accelerometer sensor sensitivity
 * @param  pObj the device pObj
 * @param  Sensitivity pointer
 * @retval 0 in case of success, an error code otherwise
 */
int32_t H3LIS331DL_ACC_GetSensitivity(H3LIS331DL_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = H3LIS331DL_OK;
  h3lis331dl_fs_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (h3lis331dl_full_scale_get(&(pObj->Ctx), &full_scale) != H3LIS331DL_OK)
  {
    return H3LIS331DL_ERROR;
  }

  switch (full_scale)
  {
    case H3LIS331DL_100g:
      *Sensitivity = H3LIS331DL_ACC_SENSITIVITY_FOR_FS_100G;
       break;

    case H3LIS331DL_200g:
      *Sensitivity = H3LIS331DL_ACC_SENSITIVITY_FOR_FS_200G;
      break;

    case H3LIS331DL_400g:
      *Sensitivity = H3LIS331DL_ACC_SENSITIVITY_FOR_FS_400G;
      break;

    default:
      *Sensitivity = -1.0f;
      ret = H3LIS331DL_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Get the H3LIS331DL accelerometer sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr pointer where the output data rate is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t H3LIS331DL_ACC_GetOutputDataRate(H3LIS331DL_Object_t *pObj, float *Odr)
{
  int32_t ret = H3LIS331DL_OK;
  h3lis331dl_dr_t odr_low_level;

  /* Get current output data rate. */
  if (h3lis331dl_data_rate_get(&(pObj->Ctx), &odr_low_level) != H3LIS331DL_OK)
  {
    return H3LIS331DL_ERROR;
  }

  switch (odr_low_level)
  {
    case H3LIS331DL_ODR_OFF:
      *Odr = 0.0f;
      break;

    case H3LIS331DL_ODR_Hz5:
      *Odr = 0.5f;
      break;

    case H3LIS331DL_ODR_1Hz:
      *Odr = 1.0f;
      break;

    case H3LIS331DL_ODR_5Hz2:
      *Odr = 2.0f;
      break;

    case H3LIS331DL_ODR_5Hz:
      *Odr = 5.0f;
      break;

    case H3LIS331DL_ODR_10Hz:
      *Odr = 10.0f;
      break;

    case H3LIS331DL_ODR_50Hz:
      *Odr = 50.0f;
      break;

    case H3LIS331DL_ODR_100Hz:
      *Odr = 100.0f;
      break;

    case H3LIS331DL_ODR_400Hz:
      *Odr = 400.0f;
      break;

    case H3LIS331DL_ODR_1kHz:
      *Odr = 1000.0f;
      break;

    default:
      *Odr = -1.0f;
      ret = H3LIS331DL_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the H3LIS331DL accelerometer sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t H3LIS331DL_ACC_SetOutputDataRate(H3LIS331DL_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return H3LIS331DL_ACC_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return H3LIS331DL_ACC_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
 * @brief  Get the H3LIS331DL accelerometer sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale pointer where the full scale is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t H3LIS331DL_ACC_GetFullScale(H3LIS331DL_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = H3LIS331DL_OK;
  h3lis331dl_fs_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (h3lis331dl_full_scale_get(&(pObj->Ctx), &fs_low_level) != H3LIS331DL_OK)
  {
    return H3LIS331DL_ERROR;
  }

  switch (fs_low_level)
  {
    case H3LIS331DL_100g:
      *FullScale = 100;
      break;

    case H3LIS331DL_200g:
      *FullScale = 200;
      break;

    case H3LIS331DL_400g:
      *FullScale = 400;
      break;

    default:
      *FullScale = -1;
      ret = H3LIS331DL_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the H3LIS331DL accelerometer sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale the functional full scale to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t H3LIS331DL_ACC_SetFullScale(H3LIS331DL_Object_t *pObj, int32_t FullScale)
{
  h3lis331dl_fs_t new_fs;

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file statical analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  new_fs = (FullScale <= 100) ? H3LIS331DL_100g
         : (FullScale <= 200) ? H3LIS331DL_200g
         :                      H3LIS331DL_400g;

  if (h3lis331dl_full_scale_set(&(pObj->Ctx), new_fs) != H3LIS331DL_OK)
  {
    return H3LIS331DL_ERROR;
  }

  return H3LIS331DL_OK;
}

/**
 * @brief  Get the H3LIS331DL accelerometer sensor raw axes
 * @param  pObj the device pObj
 * @param  Value pointer where the raw values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t H3LIS331DL_ACC_GetAxesRaw(H3LIS331DL_Object_t *pObj, H3LIS331DL_AxesRaw_t *Value)
{
  h3lis331dl_axis3bit16_t data_raw;
  uint8_t multi_read;

  /* Read raw data values. */
  multi_read = (pObj->IO.BusType == H3LIS331DL_I2C_BUS)        ? 0x80
             : (pObj->IO.BusType == H3LIS331DL_SPI_4WIRES_BUS) ? 0x40
             : (pObj->IO.BusType == H3LIS331DL_SPI_3WIRES_BUS) ? 0x40
             :                                                   0x00;

  if (h3lis331dl_read_reg(&(pObj->Ctx), H3LIS331DL_OUT_X_L | multi_read, data_raw.u8bit, 6) != H3LIS331DL_OK)
  {
    return H3LIS331DL_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0] >> 4;
  Value->y = data_raw.i16bit[1] >> 4;
  Value->z = data_raw.i16bit[2] >> 4;

  return H3LIS331DL_OK;
}

/**
 * @brief  Get the H3LIS331DL accelerometer sensor axes
 * @param  pObj the device pObj
 * @param  Acceleration pointer where the values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t H3LIS331DL_ACC_GetAxes(H3LIS331DL_Object_t *pObj, H3LIS331DL_Axes_t *Acceleration)
{
  H3LIS331DL_AxesRaw_t data_raw;
  float sensitivity;

  /* Read raw data values. */
  if (H3LIS331DL_ACC_GetAxesRaw(pObj, &data_raw) != H3LIS331DL_OK)
  {
    return H3LIS331DL_ERROR;
  }

  /* Get H3LIS331DL actual sensitivity. */
  if (H3LIS331DL_ACC_GetSensitivity(pObj, &sensitivity) != H3LIS331DL_OK)
  {
    return H3LIS331DL_ERROR;
  }

  /* Calculate the data. */
  Acceleration->x = (int32_t)((float)((float)data_raw.x * sensitivity));
  Acceleration->y = (int32_t)((float)((float)data_raw.y * sensitivity));
  Acceleration->z = (int32_t)((float)((float)data_raw.z * sensitivity));

  return H3LIS331DL_OK;
}

/**
 * @brief  Get the H3LIS331DL register value
 * @param  pObj the device pObj
 * @param  Reg address to be read
 * @param  Data pointer where the value is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t H3LIS331DL_Read_Reg(H3LIS331DL_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (h3lis331dl_read_reg(&(pObj->Ctx), Reg, Data, 1) != H3LIS331DL_OK)
  {
    return H3LIS331DL_ERROR;
  }

  return H3LIS331DL_OK;
}

/**
 * @brief  Set the H3LIS331DL register value
 * @param  pObj the device pObj
 * @param  Reg address to be written
 * @param  Data value to be written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t H3LIS331DL_Write_Reg(H3LIS331DL_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (h3lis331dl_write_reg(&(pObj->Ctx), Reg, &Data, 1) != H3LIS331DL_OK)
  {
    return H3LIS331DL_ERROR;
  }

  return H3LIS331DL_OK;
}

/**
 * @brief  Get the H3LIS331DL ACC data ready bit value
 * @param  pObj the device pObj
 * @param  Status the status of data ready bit
 * @retval 0 in case of success, an error code otherwise
 */
int32_t H3LIS331DL_ACC_Get_DRDY_Status(H3LIS331DL_Object_t *pObj, uint8_t *Status)
{
  if (h3lis331dl_flag_data_ready_get(&(pObj->Ctx), Status) != H3LIS331DL_OK)
  {
    return H3LIS331DL_ERROR;
  }

  return H3LIS331DL_OK;
}

/**
 * @}
 */

/** @defgroup H3LIS331DL_Private_Functions H3LIS331DL Private Functions
 * @{
 */

/**
 * @brief  Set the H3LIS331DL accelerometer sensor output data rate when enabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t H3LIS331DL_ACC_SetOutputDataRate_When_Enabled(H3LIS331DL_Object_t *pObj, float Odr)
{
  h3lis331dl_dr_t new_odr;

  new_odr = (Odr <=   0.5f) ? H3LIS331DL_ODR_Hz5
          : (Odr <=   1.0f) ? H3LIS331DL_ODR_1Hz
          : (Odr <=   2.0f) ? H3LIS331DL_ODR_5Hz2
          : (Odr <=   5.0f) ? H3LIS331DL_ODR_5Hz
          : (Odr <=  10.0f) ? H3LIS331DL_ODR_10Hz
          : (Odr <=  50.0f) ? H3LIS331DL_ODR_50Hz
          : (Odr <= 100.0f) ? H3LIS331DL_ODR_100Hz
          : (Odr <= 400.0f) ? H3LIS331DL_ODR_400Hz
          :                   H3LIS331DL_ODR_1kHz;

  /* Output data rate selection. */
  if (h3lis331dl_data_rate_set(&(pObj->Ctx), new_odr) != H3LIS331DL_OK)
  {
    return H3LIS331DL_ERROR;
  }

  /* Store the current output data rate value */
  pObj->acc_odr = new_odr;

  return H3LIS331DL_OK;
}

/**
 * @brief  Set the H3LIS331DL accelerometer sensor output data rate when disabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t H3LIS331DL_ACC_SetOutputDataRate_When_Disabled(H3LIS331DL_Object_t *pObj, float Odr)
{
  /* Store the new output data rate value */
  pObj->acc_odr = (Odr <=   0.5f) ? H3LIS331DL_ODR_Hz5
                : (Odr <=   1.0f) ? H3LIS331DL_ODR_1Hz
                : (Odr <=   2.0f) ? H3LIS331DL_ODR_5Hz2
                : (Odr <=   5.0f) ? H3LIS331DL_ODR_5Hz
                : (Odr <=  10.0f) ? H3LIS331DL_ODR_10Hz
                : (Odr <=  50.0f) ? H3LIS331DL_ODR_50Hz
                : (Odr <= 100.0f) ? H3LIS331DL_ODR_100Hz
                : (Odr <= 400.0f) ? H3LIS331DL_ODR_400Hz
                :                   H3LIS331DL_ODR_1kHz;

  return H3LIS331DL_OK;
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
  H3LIS331DL_Object_t *pObj = (H3LIS331DL_Object_t *)Handle;

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
  H3LIS331DL_Object_t *pObj = (H3LIS331DL_Object_t *)Handle;

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
