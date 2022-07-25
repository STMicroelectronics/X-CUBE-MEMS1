/**
  ******************************************************************************
  * @file    ais2dw12.c
  * @author  MEMS Software Solutions Team
  * @brief   LIS2DW12 driver file
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
#include "ais2dw12.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup AIS2DW12 AIS2DW12
  * @{
  */

/** @defgroup AIS2DW12_Exported_Variables AIS2DW12 Exported Variables
  * @{
  */

AIS2DW12_CommonDrv_t AIS2DW12_COMMON_Driver =
{
  AIS2DW12_Init,
  AIS2DW12_DeInit,
  AIS2DW12_ReadID,
  AIS2DW12_GetCapabilities,
};

AIS2DW12_ACC_Drv_t AIS2DW12_ACC_Driver =
{
  AIS2DW12_ACC_Enable,
  AIS2DW12_ACC_Disable,
  AIS2DW12_ACC_GetSensitivity,
  AIS2DW12_ACC_GetOutputDataRate,
  AIS2DW12_ACC_SetOutputDataRate,
  AIS2DW12_ACC_GetFullScale,
  AIS2DW12_ACC_SetFullScale,
  AIS2DW12_ACC_GetAxes,
  AIS2DW12_ACC_GetAxesRaw,
};

/**
  * @}
  */

/** @defgroup AIS2DW12_Private_Function_Prototypes AIS2DW12 Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t AIS2DW12_ACC_SetOutputDataRate_When_Enabled(AIS2DW12_Object_t *pObj, float Odr,
                                                           AIS2DW12_Operating_Mode_t Mode);
static int32_t AIS2DW12_ACC_SetOutputDataRate_When_Disabled(AIS2DW12_Object_t *pObj, float Odr,
                                                            AIS2DW12_Operating_Mode_t Mode);

/**
  * @}
  */

/** @defgroup AIS2DW12_Exported_Functions AIS2DW12 Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_RegisterBusIO(AIS2DW12_Object_t *pObj, AIS2DW12_IO_t *pIO)
{
  int32_t ret = AIS2DW12_OK;

  if (pObj == NULL)
  {
    ret = AIS2DW12_ERROR;
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
      ret = AIS2DW12_ERROR;
    }
    else if (pObj->IO.Init() != AIS2DW12_OK)
    {
      ret = AIS2DW12_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == AIS2DW12_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x05;

          if (AIS2DW12_Write_Reg(pObj, AIS2DW12_CTRL2, data) != AIS2DW12_OK)
          {
            ret = AIS2DW12_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  Initialize the AIS2DW12 sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_Init(AIS2DW12_Object_t *pObj)
{
  /* Enable register address automatically incremented during a multiple byte
  access with a serial interface. */
  if (ais2dw12_auto_increment_set(&(pObj->Ctx), PROPERTY_ENABLE) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* Enable BDU */
  if (ais2dw12_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* FIFO mode selection */
  if (ais2dw12_fifo_mode_set(&(pObj->Ctx), AIS2DW12_BYPASS_MODE) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* Power mode selection */
  if (ais2dw12_power_mode_set(&(pObj->Ctx), AIS2DW12_PWR_MD_4) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = 100.0f;
  /* Select default operating mode. */
  pObj->acc_operating_mode = AIS2DW12_POWER_MODE4;

  /* Output data rate selection - power down. */
  if (ais2dw12_data_rate_set(&(pObj->Ctx), AIS2DW12_XL_ODR_OFF) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* Full scale selection. */
  if (ais2dw12_full_scale_set(&(pObj->Ctx), AIS2DW12_2g) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  pObj->is_initialized = 1;

  return AIS2DW12_OK;
}

/**
  * @brief  Deinitialize the AIS2DW12 sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_DeInit(AIS2DW12_Object_t *pObj)
{
  /* Disable the component */
  if (AIS2DW12_ACC_Disable(pObj) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* Reset output data rate. */
  pObj->acc_odr = 0.0f;
  /* Reset operating mode to default value. */
  pObj->acc_operating_mode = AIS2DW12_POWER_MODE4;

  pObj->is_initialized = 0;

  return AIS2DW12_OK;
}

/**
  * @brief  Read component ID
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ReadID(AIS2DW12_Object_t *pObj, uint8_t *Id)
{
  if (ais2dw12_device_id_get(&(pObj->Ctx), Id) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  return AIS2DW12_OK;
}

/**
  * @brief  Get AIS2DW12 sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to AIS2DW12 sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_GetCapabilities(AIS2DW12_Object_t *pObj, AIS2DW12_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Acc          = 1;
  Capabilities->Gyro         = 0;
  Capabilities->Magneto      = 0;
  Capabilities->LowPower     = 0;
  Capabilities->GyroMaxFS    = 0;
  Capabilities->AccMaxFS     = 4;
  Capabilities->MagMaxFS     = 0;
  Capabilities->GyroMaxOdr   = 0.0f;
  Capabilities->AccMaxOdr    = 100.0f;
  Capabilities->MagMaxOdr    = 0.0f;
  return AIS2DW12_OK;
}

/**
  * @brief  Enable the AIS2DW12 accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Enable(AIS2DW12_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return AIS2DW12_OK;
  }

  /* Output data rate selection. */
  if (AIS2DW12_ACC_SetOutputDataRate_When_Enabled(pObj, pObj->acc_odr, pObj->acc_operating_mode) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return AIS2DW12_OK;
}

/**
  * @brief  Disable the AIS2DW12 accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Disable(AIS2DW12_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return AIS2DW12_OK;
  }

  /* Output data rate selection - power down. */
  if (ais2dw12_data_rate_set(&(pObj->Ctx), AIS2DW12_XL_ODR_OFF) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return AIS2DW12_OK;
}

/**
  * @brief  Get the AIS2DW12 accelerometer sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_GetSensitivity(AIS2DW12_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = AIS2DW12_OK;
  ais2dw12_fs_t full_scale;
  ais2dw12_mode_t mode;

  /* Read actual full scale selection from sensor. */
  if (ais2dw12_full_scale_get(&(pObj->Ctx), &full_scale) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* Read actual power mode selection from sensor. */
  if (ais2dw12_power_mode_get(&(pObj->Ctx), &mode) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  switch (mode)
  {
    case AIS2DW12_PWR_MD_12bit:
    case AIS2DW12_SINGLE_PWR_MD_12bit:
      switch (full_scale)
      {
        case AIS2DW12_2g:
          *Sensitivity = AIS2DW12_ACC_SENSITIVITY_FOR_FS_2G_LOPOW1_MODE;
          break;

        case AIS2DW12_4g:
          *Sensitivity = AIS2DW12_ACC_SENSITIVITY_FOR_FS_4G_LOPOW1_MODE;
          break;

        default:
          *Sensitivity = -1.0f;
          ret = AIS2DW12_ERROR;
          break;
      }
      break;

    case AIS2DW12_PWR_MD_4:
    case AIS2DW12_PWR_MD_3:
    case AIS2DW12_PWR_MD_2:
    case AIS2DW12_SINGLE_PWR_MD_4:
    case AIS2DW12_SINGLE_PWR_MD_3:
    case AIS2DW12_SINGLE_PWR_MD_2:
      switch (full_scale)
      {
        case AIS2DW12_2g:
          *Sensitivity = AIS2DW12_ACC_SENSITIVITY_FOR_FS_2G_OTHER_MODES;
          break;

        case AIS2DW12_4g:
          *Sensitivity = AIS2DW12_ACC_SENSITIVITY_FOR_FS_4G_OTHER_MODES;
          break;

        default:
          *Sensitivity = -1.0f;
          ret = AIS2DW12_ERROR;
          break;
      }
      break;

    default:
      *Sensitivity = -1.0f;
      ret = AIS2DW12_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the AIS2DW12 accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_GetOutputDataRate(AIS2DW12_Object_t *pObj, float *Odr)
{
  int32_t ret = AIS2DW12_OK;
  ais2dw12_odr_t odr_low_level;
  ais2dw12_mode_t mode;

  /* Get current output data rate. */
  if (ais2dw12_data_rate_get(&(pObj->Ctx), &odr_low_level) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* Read actual power mode selection from sensor. */
  if (ais2dw12_power_mode_get(&(pObj->Ctx), &mode) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  switch (odr_low_level)
  {
    case AIS2DW12_XL_ODR_OFF:
    case AIS2DW12_XL_SET_SW_TRIG:
    case AIS2DW12_XL_SET_PIN_TRIG:
      *Odr = 0.0f;
      break;

    case AIS2DW12_XL_ODR_1Hz6:
      *Odr = 1.6f;
      break;

    case AIS2DW12_XL_ODR_12Hz5:
      *Odr = 12.5f;
      break;

    case AIS2DW12_XL_ODR_25Hz:
      *Odr = 25.0f;
      break;

    case AIS2DW12_XL_ODR_50Hz:
      *Odr = 50.0f;
      break;

    case AIS2DW12_XL_ODR_100Hz:
      *Odr = 100.0f;
      break;

    default:
      *Odr = -1.0f;
      ret = AIS2DW12_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the AIS2DW12 accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_SetOutputDataRate(AIS2DW12_Object_t *pObj, float Odr)
{
  /* By default we use High Performance mode and Low Noise disabled */
  return AIS2DW12_ACC_SetOutputDataRate_With_Mode(pObj, Odr, AIS2DW12_POWER_MODE4);
}

/**
  * @brief  Set the AIS2DW12 accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @param  Mode the operating mode to be used
  * @param  Noise the low noise option
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_SetOutputDataRate_With_Mode(AIS2DW12_Object_t *pObj, float Odr, AIS2DW12_Operating_Mode_t Mode)
{
  /* Check if the component is enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return AIS2DW12_ACC_SetOutputDataRate_When_Enabled(pObj, Odr, Mode);
  }
  else
  {
    return AIS2DW12_ACC_SetOutputDataRate_When_Disabled(pObj, Odr, Mode);
  }
}

/**
  * @brief  Get the AIS2DW12 accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_GetFullScale(AIS2DW12_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = AIS2DW12_OK;
  ais2dw12_fs_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (ais2dw12_full_scale_get(&(pObj->Ctx), &fs_low_level) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  switch (fs_low_level)
  {
    case AIS2DW12_2g:
      *FullScale =  2;
      break;

    case AIS2DW12_4g:
      *FullScale =  4;
      break;

    default:
      *FullScale = -1;
      ret = AIS2DW12_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the AIS2DW12 accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_SetFullScale(AIS2DW12_Object_t *pObj, int32_t FullScale)
{
  ais2dw12_fs_t new_fs;

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file statical analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  new_fs = (FullScale <= 2) ? AIS2DW12_2g
           :                    AIS2DW12_4g;

  if (ais2dw12_full_scale_set(&(pObj->Ctx), new_fs) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  return AIS2DW12_OK;
}

/**
  * @brief  Get the AIS2DW12 accelerometer sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_GetAxesRaw(AIS2DW12_Object_t *pObj, AIS2DW12_AxesRaw_t *Value)
{
  ais2dw12_axis3bit16_t data_raw;
  ais2dw12_mode_t mode;
  int32_t ret = AIS2DW12_OK;

  /* Read actual power mode selection from sensor. */
  if (ais2dw12_power_mode_get(&(pObj->Ctx), &mode) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* Read raw data values. */
  if (ais2dw12_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  switch (mode)
  {
    case AIS2DW12_PWR_MD_12bit:
    case AIS2DW12_SINGLE_PWR_MD_12bit:
      /* Data format 12 bits. */
      Value->x = (data_raw.i16bit[0] / 16);
      Value->y = (data_raw.i16bit[1] / 16);
      Value->z = (data_raw.i16bit[2] / 16);
      break;

    case AIS2DW12_PWR_MD_4:
    case AIS2DW12_PWR_MD_3:
    case AIS2DW12_PWR_MD_2:
    case AIS2DW12_SINGLE_PWR_MD_4:
    case AIS2DW12_SINGLE_PWR_MD_3:
    case AIS2DW12_SINGLE_PWR_MD_2:
      /* Data format 14 bits. */
      Value->x = (data_raw.i16bit[0] / 4);
      Value->y = (data_raw.i16bit[1] / 4);
      Value->z = (data_raw.i16bit[2] / 4);
      break;

    default:
      ret = AIS2DW12_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the AIS2DW12 accelerometer sensor axes
  * @param  pObj the device pObj
  * @param  Acceleration pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_GetAxes(AIS2DW12_Object_t *pObj, AIS2DW12_Axes_t *Acceleration)
{
  AIS2DW12_AxesRaw_t data_raw;
  float sensitivity = 0.0f;

  /* Read raw data values. */
  if (AIS2DW12_ACC_GetAxesRaw(pObj, &data_raw) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* Get AIS2DW12 actual sensitivity. */
  if (AIS2DW12_ACC_GetSensitivity(pObj, &sensitivity) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* Calculate the data. */
  Acceleration->x = (int32_t)((float)((float)data_raw.x * sensitivity));
  Acceleration->y = (int32_t)((float)((float)data_raw.y * sensitivity));
  Acceleration->z = (int32_t)((float)((float)data_raw.z * sensitivity));

  return AIS2DW12_OK;
}

/**
  * @brief  Get the AIS2DW12 register value
  * @param  pObj the device pObj
  * @param  Reg address to be read
  * @param  Data pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_Read_Reg(AIS2DW12_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (ais2dw12_read_reg(&(pObj->Ctx), Reg, Data, 1) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  return AIS2DW12_OK;
}

/**
  * @brief  Set the AIS2DW12 register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_Write_Reg(AIS2DW12_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (ais2dw12_write_reg(&(pObj->Ctx), Reg, &Data, 1) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  return AIS2DW12_OK;
}

/**
  * @brief  Set the interrupt latch
  * @param  pObj the device pObj
  * @param  Status value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_Set_Interrupt_Latch(AIS2DW12_Object_t *pObj, uint8_t Status)
{
  if (Status > 1U)
  {
    return AIS2DW12_ERROR;
  }

  if (ais2dw12_int_notification_set(&(pObj->Ctx), (ais2dw12_lir_t)Status) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  return AIS2DW12_OK;
}

/**
  * @brief  Enable DRDY interrupt mode
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Enable_DRDY_Interrupt(AIS2DW12_Object_t *pObj)
{
  ais2dw12_ctrl5_int2_t int2_pad_ctrl;

  /* Enable DRDY interrupts on INT1 */
  if (ais2dw12_data_ready_mode_set(&(pObj->Ctx), AIS2DW12_DRDY_PULSED) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }
  if (ais2dw12_pin_int2_route_get(&(pObj->Ctx), &int2_pad_ctrl) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }
  int2_pad_ctrl.int2_drdy = 1;
  if (ais2dw12_pin_int2_route_set(&(pObj->Ctx), &int2_pad_ctrl) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  return AIS2DW12_OK;
}

/**
  * @brief  Disable DRDY interrupt mode
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Disable_DRDY_Interrupt(AIS2DW12_Object_t *pObj)
{
  ais2dw12_ctrl5_int2_t int2_pad_ctrl;

  if (ais2dw12_pin_int2_route_get(&(pObj->Ctx), &int2_pad_ctrl) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }
  int2_pad_ctrl.int2_drdy = 0;
  if (ais2dw12_pin_int2_route_set(&(pObj->Ctx), &int2_pad_ctrl) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  return AIS2DW12_OK;
}

/**
  * @brief  Set the filterMode value
  * @param  pObj the device pObj
  * @param  filterMode value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Set_Filter_Mode(AIS2DW12_Object_t *pObj, uint8_t filterMode)
{
  if (ais2dw12_filter_bandwidth_set(&(pObj->Ctx), (ais2dw12_bw_filt_t)filterMode) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  return AIS2DW12_OK;
}

/**
  * @brief  Enable wake up detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Enable_Wake_Up_Detection(AIS2DW12_Object_t *pObj)
{
  int32_t ret = AIS2DW12_OK;
  ais2dw12_ctrl4_int1_t val;

  /* Output Data Rate selection */
  if (AIS2DW12_ACC_SetOutputDataRate(pObj, 200.0f) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* Full scale selection */
  if (AIS2DW12_ACC_SetFullScale(pObj, 2) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* WAKE_DUR setting */
  if (ais2dw12_wkup_dur_set(&(pObj->Ctx), 0x00) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* Set wake up threshold. */
  if (ais2dw12_wkup_threshold_set(&(pObj->Ctx), 0x02) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  if (ais2dw12_pin_int1_route_get(&(pObj->Ctx), &val) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  val.int1_wu = PROPERTY_ENABLE;

  if (ais2dw12_pin_int1_route_set(&(pObj->Ctx), &val) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  return ret;
}

/**
  * @brief  Disable wake up detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Disable_Wake_Up_Detection(AIS2DW12_Object_t *pObj)
{
  ais2dw12_ctrl4_int1_t ctrl4_int1_reg;
  ais2dw12_ctrl5_int2_t ctrl5_int2_reg;
  ais2dw12_ctrl7_t ctrl_reg7;

  /* Disable wake up event on INT1 pin. */
  if (ais2dw12_read_reg(&(pObj->Ctx), AIS2DW12_CTRL4_INT1, (uint8_t *)&ctrl4_int1_reg, 1) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  ctrl4_int1_reg.int1_wu = PROPERTY_DISABLE;

  if (ais2dw12_write_reg(&(pObj->Ctx), AIS2DW12_CTRL4_INT1, (uint8_t *)&ctrl4_int1_reg, 1) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* Read INT2 Sleep Change */
  if (ais2dw12_read_reg(&(pObj->Ctx), AIS2DW12_CTRL5_INT2, (uint8_t *)&ctrl5_int2_reg, 1) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /*Disable Interrupts bit if none event is still enabled */
  if (ctrl5_int2_reg.int2_sleep_chg == 0 && ctrl4_int1_reg.int1_wu == 0 && ctrl4_int1_reg.int1_6d == 0)
  {
    if (ais2dw12_read_reg(&(pObj->Ctx), AIS2DW12_CTRL7, (uint8_t *)&ctrl_reg7, 1) != AIS2DW12_OK)
    {
      return AIS2DW12_ERROR;
    }

    ctrl_reg7.interrupts_enable = PROPERTY_DISABLE;

    if (ais2dw12_write_reg(&(pObj->Ctx), AIS2DW12_CTRL7, (uint8_t *)&ctrl_reg7, 1) != AIS2DW12_OK)
    {
      return AIS2DW12_ERROR;
    }
  }

  /* Reset wake up threshold. */
  if (ais2dw12_wkup_threshold_set(&(pObj->Ctx), 0x00) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* WAKE_DUR setting */
  if (ais2dw12_wkup_dur_set(&(pObj->Ctx), 0x00) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  return AIS2DW12_OK;
}

/**
  * @brief  Set wake up threshold
  * @param  pObj the device pObj
  * @param  Threshold wake up detection threshold
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Set_Wake_Up_Threshold(AIS2DW12_Object_t *pObj, uint8_t Threshold)
{
  /* Set wake up threshold. */
  if (ais2dw12_wkup_threshold_set(&(pObj->Ctx), Threshold) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  return AIS2DW12_OK;
}

/**
  * @brief  Set wake up duration
  * @param  pObj the device pObj
  * @param  Duration wake up detection duration
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Set_Wake_Up_Duration(AIS2DW12_Object_t *pObj, uint8_t Duration)
{
  /* Set wake up duration. */
  if (ais2dw12_wkup_dur_set(&(pObj->Ctx), Duration) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  return AIS2DW12_OK;
}

/**
  * @brief  Enable inactivity detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Enable_Inactivity_Detection(AIS2DW12_Object_t *pObj)
{
  int32_t ret = AIS2DW12_OK;
  ais2dw12_ctrl5_int2_t val;

  /* Output Data Rate selection */
  if (AIS2DW12_ACC_SetOutputDataRate(pObj, 50.0f) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* Full scale selection */
  if (AIS2DW12_ACC_SetFullScale(pObj, 2) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* SLEEP_DUR setting */
  if (ais2dw12_act_sleep_dur_set(&(pObj->Ctx), 0x01) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* Enable inactivity detection. */
  if (ais2dw12_act_mode_set(&(pObj->Ctx), AIS2DW12_DETECT_ACT_INACT) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  if (ais2dw12_pin_int2_route_get(&(pObj->Ctx), &val) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  val.int2_sleep_chg = PROPERTY_ENABLE;

  if (ais2dw12_pin_int2_route_set(&(pObj->Ctx), &val) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  return ret;
}

/**
  * @brief  Disable inactivity detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Disable_Inactivity_Detection(AIS2DW12_Object_t *pObj)
{
  ais2dw12_ctrl4_int1_t ctrl4_int1_reg;
  ais2dw12_ctrl5_int2_t ctrl5_int2_reg;
  ais2dw12_ctrl7_t ctrl_reg7;

  /* Disable inactivity event on INT2 pin */
  if (ais2dw12_read_reg(&(pObj->Ctx), AIS2DW12_CTRL5_INT2, (uint8_t *)&ctrl5_int2_reg, 1) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  ctrl5_int2_reg.int2_sleep_chg = PROPERTY_DISABLE;

  if (ais2dw12_write_reg(&(pObj->Ctx), AIS2DW12_CTRL5_INT2, (uint8_t *)&ctrl5_int2_reg, 1) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* Read INT1 Wake Up event and INT1 6D Orientation event */
  if (ais2dw12_read_reg(&(pObj->Ctx), AIS2DW12_CTRL4_INT1, (uint8_t *)&ctrl4_int1_reg, 1) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /*Disable Interrupts bit if none event is still enabled */
  if (ctrl5_int2_reg.int2_sleep_chg == 0 && ctrl4_int1_reg.int1_wu == 0 && ctrl4_int1_reg.int1_6d == 0)
  {
    if (ais2dw12_read_reg(&(pObj->Ctx), AIS2DW12_CTRL7, (uint8_t *)&ctrl_reg7, 1) != AIS2DW12_OK)
    {
      return AIS2DW12_ERROR;
    }

    ctrl_reg7.interrupts_enable = PROPERTY_DISABLE;

    if (ais2dw12_write_reg(&(pObj->Ctx), AIS2DW12_CTRL7, (uint8_t *)&ctrl_reg7, 1) != AIS2DW12_OK)
    {
      return AIS2DW12_ERROR;
    }
  }

  /* Disable inactivity detection. */
  if (ais2dw12_act_mode_set(&(pObj->Ctx), AIS2DW12_NO_DETECTION) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* SLEEP_DUR setting */
  if (ais2dw12_act_sleep_dur_set(&(pObj->Ctx), 0x00) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  return AIS2DW12_OK;
}

/**
  * @brief  Set sleep duration
  * @param  pObj the device pObj
  * @param  Duration wake up detection duration
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Set_Sleep_Duration(AIS2DW12_Object_t *pObj, uint8_t Duration)
{
  /* Set sleep duration. */
  if (ais2dw12_act_sleep_dur_set(&(pObj->Ctx), Duration) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  return AIS2DW12_OK;
}

/**
  * @brief  Enable 6D orientation detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Enable_6D_Orientation(AIS2DW12_Object_t *pObj)
{
  int32_t ret = AIS2DW12_OK;
  ais2dw12_ctrl4_int1_t val;

  /* Output Data Rate selection */
  if (AIS2DW12_ACC_SetOutputDataRate(pObj, 200.0f) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* Full scale selection */
  if (AIS2DW12_ACC_SetFullScale(pObj, 2) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* 6D orientation threshold. */
  if (ais2dw12_6d_threshold_set(&(pObj->Ctx), 2) != AIS2DW12_OK) /* 60 degrees */
  {
    return AIS2DW12_ERROR;
  }

  /* Enable 6D orientation event on INT1 pin */
  if (ais2dw12_pin_int1_route_get(&(pObj->Ctx), &val) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  val.int1_6d = PROPERTY_ENABLE;

  if (ais2dw12_pin_int1_route_set(&(pObj->Ctx), &val) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  return ret;
}

/**
  * @brief  Disable 6D orientation detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Disable_6D_Orientation(AIS2DW12_Object_t *pObj)
{
  ais2dw12_ctrl4_int1_t ctrl4_int1_reg;
  ais2dw12_ctrl5_int2_t ctrl5_int2_reg;
  ais2dw12_ctrl7_t ctrl_reg7;

  /* Disable 6D orientation event on INT1 pin */
  if (ais2dw12_read_reg(&(pObj->Ctx), AIS2DW12_CTRL4_INT1, (uint8_t *)&ctrl4_int1_reg, 1) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  ctrl4_int1_reg.int1_6d = PROPERTY_DISABLE;

  if (ais2dw12_write_reg(&(pObj->Ctx), AIS2DW12_CTRL4_INT1, (uint8_t *)&ctrl4_int1_reg, 1) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* Read INT2 Sleep Change */
  if (ais2dw12_read_reg(&(pObj->Ctx), AIS2DW12_CTRL5_INT2, (uint8_t *)&ctrl5_int2_reg, 1) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /*Disable Interrupts bit if none event is still enabled */
  if (ctrl5_int2_reg.int2_sleep_chg == 0 && ctrl4_int1_reg.int1_wu == 0 && ctrl4_int1_reg.int1_6d == 0)
  {
    if (ais2dw12_read_reg(&(pObj->Ctx), AIS2DW12_CTRL7, (uint8_t *)&ctrl_reg7, 1) != AIS2DW12_OK)
    {
      return AIS2DW12_ERROR;
    }

    ctrl_reg7.interrupts_enable = PROPERTY_DISABLE;

    if (ais2dw12_write_reg(&(pObj->Ctx), AIS2DW12_CTRL7, (uint8_t *)&ctrl_reg7, 1) != AIS2DW12_OK)
    {
      return AIS2DW12_ERROR;
    }
  }

  /* Reset 6D orientation threshold. */
  if (ais2dw12_6d_threshold_set(&(pObj->Ctx), 0) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  return AIS2DW12_OK;
}

/**
  * @brief  Set 6D orientation threshold
  * @param  pObj the device pObj
  * @param  Threshold 6D orientation detection threshold
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Set_6D_Orientation_Threshold(AIS2DW12_Object_t *pObj, uint8_t Threshold)
{
  if (Threshold > 3)
  {
    return AIS2DW12_ERROR;
  }

  if (ais2dw12_6d_threshold_set(&(pObj->Ctx), Threshold) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  return AIS2DW12_OK;
}

/**
  * @brief  Get the status of XLow orientation
  * @param  pObj the device pObj
  * @param  XLow the status of XLow orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Get_6D_Orientation_XL(AIS2DW12_Object_t *pObj, uint8_t *XLow)
{
  ais2dw12_sixd_src_t data;

  if (ais2dw12_read_reg(&(pObj->Ctx), AIS2DW12_SIXD_SRC, (uint8_t *)&data, 1) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  *XLow = data.xl;

  return AIS2DW12_OK;
}

/**
  * @brief  Get the status of XHigh orientation
  * @param  pObj the device pObj
  * @param  XHigh the status of XHigh orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Get_6D_Orientation_XH(AIS2DW12_Object_t *pObj, uint8_t *XHigh)
{
  ais2dw12_sixd_src_t data;

  if (ais2dw12_read_reg(&(pObj->Ctx), AIS2DW12_SIXD_SRC, (uint8_t *)&data, 1) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  *XHigh = data.xh;

  return AIS2DW12_OK;
}

/**
  * @brief  Get the status of YLow orientation
  * @param  pObj the device pObj
  * @param  YLow the status of YLow orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Get_6D_Orientation_YL(AIS2DW12_Object_t *pObj, uint8_t *YLow)
{
  ais2dw12_sixd_src_t data;

  if (ais2dw12_read_reg(&(pObj->Ctx), AIS2DW12_SIXD_SRC, (uint8_t *)&data, 1) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  *YLow = data.yl;

  return AIS2DW12_OK;
}

/**
  * @brief  Get the status of YHigh orientation
  * @param  pObj the device pObj
  * @param  YHigh the status of YHigh orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Get_6D_Orientation_YH(AIS2DW12_Object_t *pObj, uint8_t *YHigh)
{
  ais2dw12_sixd_src_t data;

  if (ais2dw12_read_reg(&(pObj->Ctx), AIS2DW12_SIXD_SRC, (uint8_t *)&data, 1) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  *YHigh = data.yh;

  return AIS2DW12_OK;
}

/**
  * @brief  Get the status of ZLow orientation
  * @param  pObj the device pObj
  * @param  ZLow the status of ZLow orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Get_6D_Orientation_ZL(AIS2DW12_Object_t *pObj, uint8_t *ZLow)
{
  ais2dw12_sixd_src_t data;

  if (ais2dw12_read_reg(&(pObj->Ctx), AIS2DW12_SIXD_SRC, (uint8_t *)&data, 1) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  *ZLow = data.zl;

  return AIS2DW12_OK;
}

/**
  * @brief  Get the status of ZHigh orientation
  * @param  pObj the device pObj
  * @param  ZHigh the status of ZHigh orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Get_6D_Orientation_ZH(AIS2DW12_Object_t *pObj, uint8_t *ZHigh)
{
  ais2dw12_sixd_src_t data;

  if (ais2dw12_read_reg(&(pObj->Ctx), AIS2DW12_SIXD_SRC, (uint8_t *)&data, 1) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  *ZHigh = data.zh;

  return AIS2DW12_OK;
}

/**
  * @brief  Get the status of all hardware events
  * @param  pObj the device pObj
  * @param  Status the status of all hardware events
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Get_Event_Status(AIS2DW12_Object_t *pObj, AIS2DW12_Event_Status_t *Status)
{
  ais2dw12_status_t status_reg;
  ais2dw12_ctrl4_int1_t ctrl4_int1_reg;
  ais2dw12_ctrl5_int2_t ctrl5_int2_reg;

  (void)memset((void *)Status, 0x0, sizeof(AIS2DW12_Event_Status_t));

  if (ais2dw12_read_reg(&(pObj->Ctx), AIS2DW12_STATUS, (uint8_t *)&status_reg, 1) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  if (ais2dw12_read_reg(&(pObj->Ctx), AIS2DW12_CTRL4_INT1, (uint8_t *)&ctrl4_int1_reg, 1) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  if (ais2dw12_read_reg(&(pObj->Ctx), AIS2DW12_CTRL5_INT2, (uint8_t *)&ctrl5_int2_reg, 1) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
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

  return AIS2DW12_OK;
}

/**
  * @brief  Set self test
  * @param  pObj the device pObj
  * @param  Val the value of ST in reg CTRL3
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Set_SelfTest(AIS2DW12_Object_t *pObj, uint8_t Val)
{
  ais2dw12_st_t reg;

  if (Val > 2U)
  {
    return AIS2DW12_ERROR;
  }

  reg = (Val == 0U)  ? AIS2DW12_XL_ST_DISABLE
        : (Val == 1U)  ? AIS2DW12_XL_ST_POSITIVE
        :                AIS2DW12_XL_ST_NEGATIVE;

  if (ais2dw12_self_test_set(&(pObj->Ctx), reg) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  return AIS2DW12_OK;
}

/**
  * @brief  Get the AIS2DW12 ACC data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Get_DRDY_Status(AIS2DW12_Object_t *pObj, uint8_t *Status)
{
  if (ais2dw12_flag_data_ready_get(&(pObj->Ctx), Status) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  return AIS2DW12_OK;
}

/**
  * @brief  Get the AIS2DW12 ACC initialization status
  * @param  pObj the device pObj
  * @param  Status 1 if initialized, 0 otherwise
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_ACC_Get_Init_Status(AIS2DW12_Object_t *pObj, uint8_t *Status)
{
  if (pObj == NULL)
  {
    return AIS2DW12_ERROR;
  }

  *Status = pObj->is_initialized;

  return AIS2DW12_OK;
}

/**
  * @brief  Get the number of samples contained into the FIFO
  * @param  pObj the device pObj
  * @param  NumSamples the number of samples contained into the FIFO
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_FIFO_Get_Num_Samples(AIS2DW12_Object_t *pObj, uint16_t *NumSamples)
{
  ais2dw12_fifo_samples_t FIFO_Samples;

  if (ais2dw12_read_reg(&(pObj->Ctx), AIS2DW12_FIFO_SAMPLES, (uint8_t *)&FIFO_Samples, 1) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  if (FIFO_Samples.diff == 0x20)
  {
    *NumSamples = 32;
  }
  else
  {
    *NumSamples = FIFO_Samples.diff;
  }

  return AIS2DW12_OK;
}

/**
  * @brief  Set the FIFO mode
  * @param  pObj the device pObj
  * @param  Mode FIFO mode
  * @retval 0 in case of success, an error code otherwise
  */
int32_t AIS2DW12_FIFO_Set_Mode(AIS2DW12_Object_t *pObj, uint8_t Mode)
{
  int32_t ret = AIS2DW12_OK;

  /* Verify that the passed parameter contains one of the valid values. */
  switch ((ais2dw12_fmode_t)Mode)
  {
    case AIS2DW12_BYPASS_MODE:
    case AIS2DW12_FIFO_MODE:
    case AIS2DW12_STREAM_TO_FIFO_MODE:
    case AIS2DW12_BYPASS_TO_STREAM_MODE:
    case AIS2DW12_STREAM_MODE:
      break;

    default:
      ret = AIS2DW12_ERROR;
      break;
  }

  if (ret == AIS2DW12_ERROR)
  {
    return ret;
  }

  if (ais2dw12_fifo_mode_set(&(pObj->Ctx), (ais2dw12_fmode_t)Mode) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  return ret;
}

/**
  * @}
  */

/** @defgroup AIS2DW12_Private_Functions AIS2DW12 Private Functions
  * @{
  */

/**
  * @brief  Set the AIS2DW12 accelerometer sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @param  Mode the operating mode to be used
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t AIS2DW12_ACC_SetOutputDataRate_When_Enabled(AIS2DW12_Object_t *pObj, float Odr,
                                                           AIS2DW12_Operating_Mode_t Mode)
{
  ais2dw12_odr_t new_odr;
  ais2dw12_mode_t new_power_mode;

  switch (Mode)
  {
    case AIS2DW12_POWER_MODE4:
    default:
      new_power_mode = AIS2DW12_PWR_MD_4;
      break;

    case AIS2DW12_POWER_MODE3:
      new_power_mode = AIS2DW12_PWR_MD_3;
      break;

    case AIS2DW12_POWER_MODE2:
      new_power_mode = AIS2DW12_PWR_MD_2;
      break;

    case AIS2DW12_POWER_MODE1:
      new_power_mode = AIS2DW12_PWR_MD_12bit;
      break;
  }

  new_odr = (Odr <=    1.6f) ? AIS2DW12_XL_ODR_1Hz6
            : (Odr <=   12.5f) ? AIS2DW12_XL_ODR_12Hz5
            : (Odr <=   25.0f) ? AIS2DW12_XL_ODR_25Hz
            : (Odr <=   50.0f) ? AIS2DW12_XL_ODR_50Hz
            :                    AIS2DW12_XL_ODR_100Hz;

  /* Output data rate selection. */
  if (ais2dw12_data_rate_set(&(pObj->Ctx), new_odr) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* Power mode selection. */
  if (ais2dw12_power_mode_set(&(pObj->Ctx), new_power_mode) != AIS2DW12_OK)
  {
    return AIS2DW12_ERROR;
  }

  /* Store the current Odr, Mode and Noise values */
  pObj->acc_odr = Odr;
  pObj->acc_operating_mode = Mode;

  return AIS2DW12_OK;
}

/**
  * @brief  Set the AIS2DW12 accelerometer sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @param  Mode the operating mode to be used
  * @param  Noise the low noise option
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t AIS2DW12_ACC_SetOutputDataRate_When_Disabled(AIS2DW12_Object_t *pObj, float Odr,
                                                            AIS2DW12_Operating_Mode_t Mode)
{
  /* Store the new Odr, Mode and Noise values */
  pObj->acc_operating_mode = Mode;

  pObj->acc_odr = (Odr <=    1.6f) ?    1.6f
                  : (Odr <=   12.5f) ?   12.5f
                  : (Odr <=   25.0f) ?   25.0f
                  : (Odr <=   50.0f) ?   50.0f
                  :                     100.0f;

  return AIS2DW12_OK;
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
  AIS2DW12_Object_t *pObj = (AIS2DW12_Object_t *)Handle;

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
  AIS2DW12_Object_t *pObj = (AIS2DW12_Object_t *)Handle;

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
