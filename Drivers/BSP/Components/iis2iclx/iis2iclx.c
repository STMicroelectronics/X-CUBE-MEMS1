/**
  ******************************************************************************
  * @file    iis2iclx.c
  * @author  MEMS Software Solutions Team
  * @brief   IIS2ICLX driver file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "iis2iclx.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup IIS2ICLX IIS2ICLX
  * @{
  */

/** @defgroup IIS2ICLX_Exported_Variables IIS2ICLX Exported Variables
  * @{
  */

IIS2ICLX_CommonDrv_t IIS2ICLX_COMMON_Driver =
{
  IIS2ICLX_Init,
  IIS2ICLX_DeInit,
  IIS2ICLX_ReadID,
  IIS2ICLX_GetCapabilities,
};

IIS2ICLX_ACC_Drv_t IIS2ICLX_ACC_Driver =
{
  IIS2ICLX_ACC_Enable,
  IIS2ICLX_ACC_Disable,
  IIS2ICLX_ACC_GetSensitivity,
  IIS2ICLX_ACC_GetOutputDataRate,
  IIS2ICLX_ACC_SetOutputDataRate,
  IIS2ICLX_ACC_GetFullScale,
  IIS2ICLX_ACC_SetFullScale,
  IIS2ICLX_ACC_GetAxes,
  IIS2ICLX_ACC_GetAxesRaw,
};

/**
  * @}
  */

/** @defgroup IIS2ICLX_Private_Function_Prototypes IIS2ICLX Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t IIS2ICLX_ACC_SetOutputDataRate_When_Enabled(IIS2ICLX_Object_t *pObj, float Odr);
static int32_t IIS2ICLX_ACC_SetOutputDataRate_When_Disabled(IIS2ICLX_Object_t *pObj, float Odr);

/**
  * @}
  */

/** @defgroup IIS2ICLX_Exported_Functions IIS2ICLX Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_RegisterBusIO(IIS2ICLX_Object_t *pObj, IIS2ICLX_IO_t *pIO)
{
  int32_t ret = IIS2ICLX_OK;

  if (pObj == NULL)
  {
    ret = IIS2ICLX_ERROR;
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
      ret = IIS2ICLX_ERROR;
    }
    else if (pObj->IO.Init() != IIS2ICLX_OK)
    {
      ret = IIS2ICLX_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == IIS2ICLX_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x0C;

          if (IIS2ICLX_Write_Reg(pObj, IIS2ICLX_CTRL3_C, data) != IIS2ICLX_OK)
          {
            ret = IIS2ICLX_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  Initialize the IIS2ICLX sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_Init(IIS2ICLX_Object_t *pObj)
{
  /* Set DEVICE_CONF bit */
  if (iis2iclx_device_conf_set(&(pObj->Ctx), PROPERTY_ENABLE) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Enable register address automatically incremented during a multiple byte
  access with a serial interface. */
  if (iis2iclx_auto_increment_set(&(pObj->Ctx), PROPERTY_ENABLE) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Enable BDU */
  if (iis2iclx_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* FIFO mode selection */
  if (iis2iclx_fifo_mode_set(&(pObj->Ctx), IIS2ICLX_BYPASS_MODE) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = IIS2ICLX_XL_ODR_104Hz;

  /* Output data rate selection - power down. */
  if (iis2iclx_xl_data_rate_set(&(pObj->Ctx), IIS2ICLX_XL_ODR_OFF) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Full scale selection. */
  if (iis2iclx_xl_full_scale_set(&(pObj->Ctx), IIS2ICLX_2g) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  pObj->is_initialized = 1;

  return IIS2ICLX_OK;
}

/**
  * @brief  Deinitialize the IIS2ICLX sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_DeInit(IIS2ICLX_Object_t *pObj)
{
  /* Disable the component */
  if (IIS2ICLX_ACC_Disable(pObj) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Reset output data rate. */
  pObj->acc_odr = IIS2ICLX_XL_ODR_OFF;

  pObj->is_initialized = 0;

  return IIS2ICLX_OK;
}

/**
  * @brief  Read component ID
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ReadID(IIS2ICLX_Object_t *pObj, uint8_t *Id)
{
  if (iis2iclx_device_id_get(&(pObj->Ctx), Id) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Get IIS2ICLX sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to IIS2ICLX sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_GetCapabilities(IIS2ICLX_Object_t *pObj, IIS2ICLX_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Acc          = 1;
  Capabilities->Gyro         = 0;
  Capabilities->Magneto      = 0;
  Capabilities->LowPower     = 0;
  Capabilities->GyroMaxFS    = 0;
  Capabilities->AccMaxFS     = 3;
  Capabilities->MagMaxFS     = 0;
  Capabilities->GyroMaxOdr   = 0.0f;
  Capabilities->AccMaxOdr    = 833.0f;
  Capabilities->MagMaxOdr    = 0.0f;
  return IIS2ICLX_OK;
}

/**
  * @brief  Enable the IIS2ICLX accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_Enable(IIS2ICLX_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return IIS2ICLX_OK;
  }

  /* Output data rate selection. */
  if (iis2iclx_xl_data_rate_set(&(pObj->Ctx), pObj->acc_odr) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return IIS2ICLX_OK;
}

/**
  * @brief  Disable the IIS2ICLX accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_Disable(IIS2ICLX_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return IIS2ICLX_OK;
  }

  /* Get current output data rate. */
  if (iis2iclx_xl_data_rate_get(&(pObj->Ctx), &pObj->acc_odr) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Output data rate selection - power down. */
  if (iis2iclx_xl_data_rate_set(&(pObj->Ctx), IIS2ICLX_XL_ODR_OFF) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return IIS2ICLX_OK;
}

/**
  * @brief  Get the IIS2ICLX accelerometer sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_GetSensitivity(IIS2ICLX_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = IIS2ICLX_OK;
  iis2iclx_fs_xl_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (iis2iclx_xl_full_scale_get(&(pObj->Ctx), &full_scale) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Store the Sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case IIS2ICLX_500mg:
      *Sensitivity = IIS2ICLX_ACC_SENSITIVITY_FS_0G5;
      break;

    case IIS2ICLX_1g:
      *Sensitivity = IIS2ICLX_ACC_SENSITIVITY_FS_1G;
      break;

    case IIS2ICLX_2g:
      *Sensitivity = IIS2ICLX_ACC_SENSITIVITY_FS_2G;
      break;

    case IIS2ICLX_3g:
      *Sensitivity = IIS2ICLX_ACC_SENSITIVITY_FS_3G;
      break;

    default:
      ret = IIS2ICLX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the IIS2ICLX accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_GetOutputDataRate(IIS2ICLX_Object_t *pObj, float *Odr)
{
  int32_t ret = IIS2ICLX_OK;
  iis2iclx_odr_xl_t odr_low_level;

  /* Get current output data rate. */
  if (iis2iclx_xl_data_rate_get(&(pObj->Ctx), &odr_low_level) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  switch (odr_low_level)
  {
    case IIS2ICLX_XL_ODR_OFF:
      *Odr = 0.0f;
      break;

    case IIS2ICLX_XL_ODR_12Hz5:
      *Odr = 12.5f;
      break;

    case IIS2ICLX_XL_ODR_26Hz:
      *Odr = 26.0f;
      break;

    case IIS2ICLX_XL_ODR_52Hz:
      *Odr = 52.0f;
      break;

    case IIS2ICLX_XL_ODR_104Hz:
      *Odr = 104.0f;
      break;

    case IIS2ICLX_XL_ODR_208Hz:
      *Odr = 208.0f;
      break;

    case IIS2ICLX_XL_ODR_416Hz:
      *Odr = 416.0f;
      break;

    case IIS2ICLX_XL_ODR_833Hz:
      *Odr = 833.0f;
      break;

    default:
      ret = IIS2ICLX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the IIS2ICLX accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_SetOutputDataRate(IIS2ICLX_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return IIS2ICLX_ACC_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return IIS2ICLX_ACC_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the IIS2ICLX accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_GetFullScale(IIS2ICLX_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = IIS2ICLX_OK;
  iis2iclx_fs_xl_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (iis2iclx_xl_full_scale_get(&(pObj->Ctx), &fs_low_level) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  switch (fs_low_level)
  {
    case IIS2ICLX_500mg:
      *FullScale =  500; /* [mg] */
      break;

    case IIS2ICLX_1g:
      *FullScale = 1000; /* [mg] */
      break;

    case IIS2ICLX_2g:
      *FullScale = 2000; /* [mg] */
      break;

    case IIS2ICLX_3g:
      *FullScale = 3000; /* [mg] */
      break;

    default:
      ret = IIS2ICLX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the IIS2ICLX accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_SetFullScale(IIS2ICLX_Object_t *pObj, int32_t FullScale)
{
  iis2iclx_fs_xl_t new_fs;

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file statical analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  new_fs = (FullScale <=  500 /* [mg] */) ? IIS2ICLX_500mg
           : (FullScale <= 1000 /* [mg] */) ? IIS2ICLX_1g
           : (FullScale <= 2000 /* [mg] */) ? IIS2ICLX_2g
           :                                  IIS2ICLX_3g;

  if (iis2iclx_xl_full_scale_set(&(pObj->Ctx), new_fs) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Get the IIS2ICLX accelerometer sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_GetAxesRaw(IIS2ICLX_Object_t *pObj, IIS2ICLX_AxesRaw_t *Value)
{
  iis2iclx_axis3bit16_t data_raw;

  /* Read raw data values. */
  if (iis2iclx_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = 0;

  return IIS2ICLX_OK;
}

/**
  * @brief  Get the IIS2ICLX accelerometer sensor axes
  * @param  pObj the device pObj
  * @param  Acceleration pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_GetAxes(IIS2ICLX_Object_t *pObj, IIS2ICLX_Axes_t *Acceleration)
{
  iis2iclx_axis3bit16_t data_raw;
  float sensitivity = 0.0f;

  /* Read raw data values. */
  if (iis2iclx_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Get IIS2ICLX actual sensitivity. */
  if (IIS2ICLX_ACC_GetSensitivity(pObj, &sensitivity) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Calculate the data. */
  Acceleration->x = (int32_t)((float)((float)data_raw.i16bit[0] * sensitivity));
  Acceleration->y = (int32_t)((float)((float)data_raw.i16bit[1] * sensitivity));
  Acceleration->z = 0;

  return IIS2ICLX_OK;
}

/**
  * @brief  Get the IIS2ICLX register value
  * @param  pObj the device pObj
  * @param  Reg address to be read
  * @param  Data pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_Read_Reg(IIS2ICLX_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (iis2iclx_read_reg(&(pObj->Ctx), Reg, Data, 1) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Set the IIS2ICLX register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_Write_Reg(IIS2ICLX_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (iis2iclx_write_reg(&(pObj->Ctx), Reg, &Data, 1) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Set the interrupt latch
  * @param  pObj the device pObj
  * @param  Status value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_Set_Interrupt_Latch(IIS2ICLX_Object_t *pObj, uint8_t Status)
{
  if (Status > 1U)
  {
    return IIS2ICLX_ERROR;
  }

  if (iis2iclx_int_notification_set(&(pObj->Ctx), (iis2iclx_lir_t)Status) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Enable wake up detection
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_Enable_Wake_Up_Detection(IIS2ICLX_Object_t *pObj, IIS2ICLX_SensorIntPin_t IntPin)
{
  int32_t ret = IIS2ICLX_OK;
  iis2iclx_pin_int1_route_t val1;
  iis2iclx_pin_int2_route_t val2;

  /* Output Data Rate selection */
  if (IIS2ICLX_ACC_SetOutputDataRate(pObj, 416.0f) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Full scale selection */
  if (IIS2ICLX_ACC_SetFullScale(pObj, 2000 /* [mg] */) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* WAKE_DUR setting */
  if (iis2iclx_wkup_dur_set(&(pObj->Ctx), 0x00) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Set wake up threshold. */
  if (iis2iclx_wkup_threshold_set(&(pObj->Ctx), 0x02) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Enable wake up event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case IIS2ICLX_INT1_PIN:
      if (iis2iclx_pin_int1_route_get(&(pObj->Ctx), &val1) != IIS2ICLX_OK)
      {
        return IIS2ICLX_ERROR;
      }

      val1.md1_cfg.int1_wu = PROPERTY_ENABLE;

      if (iis2iclx_pin_int1_route_set(&(pObj->Ctx), &val1) != IIS2ICLX_OK)
      {
        return IIS2ICLX_ERROR;
      }
      break;

    case IIS2ICLX_INT2_PIN:
      if (iis2iclx_pin_int2_route_get(&(pObj->Ctx), &val2) != IIS2ICLX_OK)
      {
        return IIS2ICLX_ERROR;
      }

      val2.md2_cfg.int2_wu = PROPERTY_ENABLE;

      if (iis2iclx_pin_int2_route_set(&(pObj->Ctx), &val2) != IIS2ICLX_OK)
      {
        return IIS2ICLX_ERROR;
      }
      break;

    default:
      ret = IIS2ICLX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Disable wake up detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_Disable_Wake_Up_Detection(IIS2ICLX_Object_t *pObj)
{
  iis2iclx_pin_int1_route_t val1;
  iis2iclx_pin_int2_route_t val2;

  /* Disable wake up event on both INT1 and INT2 pins */
  if (iis2iclx_pin_int1_route_get(&(pObj->Ctx), &val1) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  val1.md1_cfg.int1_wu = PROPERTY_DISABLE;

  if (iis2iclx_pin_int1_route_set(&(pObj->Ctx), &val1) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  if (iis2iclx_pin_int2_route_get(&(pObj->Ctx), &val2) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  val2.md2_cfg.int2_wu = PROPERTY_DISABLE;

  if (iis2iclx_pin_int2_route_set(&(pObj->Ctx), &val2) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Reset wake up threshold. */
  if (iis2iclx_wkup_threshold_set(&(pObj->Ctx), 0x00) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* WAKE_DUR setting */
  if (iis2iclx_wkup_dur_set(&(pObj->Ctx), 0x00) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Set wake up threshold
  * @param  pObj the device pObj
  * @param  Threshold wake up detection threshold
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_Set_Wake_Up_Threshold(IIS2ICLX_Object_t *pObj, uint8_t Threshold)
{
  /* Set wake up threshold. */
  if (iis2iclx_wkup_threshold_set(&(pObj->Ctx), Threshold) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Set wake up duration
  * @param  pObj the device pObj
  * @param  Duration wake up detection duration
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_Set_Wake_Up_Duration(IIS2ICLX_Object_t *pObj, uint8_t Duration)
{
  /* Set wake up duration. */
  if (iis2iclx_wkup_dur_set(&(pObj->Ctx), Duration) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Enable single tap detection
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_Enable_Single_Tap_Detection(IIS2ICLX_Object_t *pObj, IIS2ICLX_SensorIntPin_t IntPin)
{
  int32_t ret = IIS2ICLX_OK;
  iis2iclx_pin_int1_route_t val1;
  iis2iclx_pin_int2_route_t val2;

  /* Output Data Rate selection */
  if (IIS2ICLX_ACC_SetOutputDataRate(pObj, 416.0f) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Full scale selection */
  if (IIS2ICLX_ACC_SetFullScale(pObj, 2000 /* [mg] */) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Enable X direction in tap recognition. */
  if (iis2iclx_tap_detection_on_x_set(&(pObj->Ctx), PROPERTY_ENABLE) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Enable Y direction in tap recognition. */
  if (iis2iclx_tap_detection_on_y_set(&(pObj->Ctx), PROPERTY_ENABLE) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Set tap threshold. */
  if (iis2iclx_tap_threshold_x_set(&(pObj->Ctx), 0x08) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Set tap shock time window. */
  if (iis2iclx_tap_shock_set(&(pObj->Ctx), 0x02) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Set tap quiet time window. */
  if (iis2iclx_tap_quiet_set(&(pObj->Ctx), 0x01) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* _NOTE_: Tap duration time window - don't care for single tap. */

  /* _NOTE_: Single/Double Tap event - don't care of this flag for single tap. */

  /* Enable single tap event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case IIS2ICLX_INT1_PIN:
      if (iis2iclx_pin_int1_route_get(&(pObj->Ctx), &val1) != IIS2ICLX_OK)
      {
        return IIS2ICLX_ERROR;
      }

      val1.md1_cfg.int1_single_tap = PROPERTY_ENABLE;

      if (iis2iclx_pin_int1_route_set(&(pObj->Ctx), &val1) != IIS2ICLX_OK)
      {
        return IIS2ICLX_ERROR;
      }
      break;

    case IIS2ICLX_INT2_PIN:
      if (iis2iclx_pin_int2_route_get(&(pObj->Ctx), &val2) != IIS2ICLX_OK)
      {
        return IIS2ICLX_ERROR;
      }

      val2.md2_cfg.int2_single_tap = PROPERTY_ENABLE;

      if (iis2iclx_pin_int2_route_set(&(pObj->Ctx), &val2) != IIS2ICLX_OK)
      {
        return IIS2ICLX_ERROR;
      }
      break;

    default:
      ret = IIS2ICLX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Disable single tap detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_Disable_Single_Tap_Detection(IIS2ICLX_Object_t *pObj)
{
  iis2iclx_pin_int1_route_t val1;
  iis2iclx_pin_int2_route_t val2;

  /* Disable single tap event on both INT1 and INT2 pins */
  if (iis2iclx_pin_int1_route_get(&(pObj->Ctx), &val1) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  val1.md1_cfg.int1_single_tap = PROPERTY_DISABLE;

  if (iis2iclx_pin_int1_route_set(&(pObj->Ctx), &val1) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  if (iis2iclx_pin_int2_route_get(&(pObj->Ctx), &val2) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  val2.md2_cfg.int2_single_tap = PROPERTY_DISABLE;

  if (iis2iclx_pin_int2_route_set(&(pObj->Ctx), &val2) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Reset tap quiet time window. */
  if (iis2iclx_tap_quiet_set(&(pObj->Ctx), 0x00) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Reset tap shock time window. */
  if (iis2iclx_tap_shock_set(&(pObj->Ctx), 0x00) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Reset tap threshold. */
  if (iis2iclx_tap_threshold_x_set(&(pObj->Ctx), 0x00) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Disable Y direction in tap recognition. */
  if (iis2iclx_tap_detection_on_y_set(&(pObj->Ctx), PROPERTY_DISABLE) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Disable X direction in tap recognition. */
  if (iis2iclx_tap_detection_on_x_set(&(pObj->Ctx), PROPERTY_DISABLE) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Enable double tap detection
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_Enable_Double_Tap_Detection(IIS2ICLX_Object_t *pObj, IIS2ICLX_SensorIntPin_t IntPin)
{
  int32_t ret = IIS2ICLX_OK;
  iis2iclx_pin_int1_route_t val1;
  iis2iclx_pin_int2_route_t val2;

  /* Output Data Rate selection */
  if (IIS2ICLX_ACC_SetOutputDataRate(pObj, 416.0f) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Full scale selection */
  if (IIS2ICLX_ACC_SetFullScale(pObj, 2000 /* [mg] */) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Enable X direction in tap recognition. */
  if (iis2iclx_tap_detection_on_x_set(&(pObj->Ctx), PROPERTY_ENABLE) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Enable Y direction in tap recognition. */
  if (iis2iclx_tap_detection_on_y_set(&(pObj->Ctx), PROPERTY_ENABLE) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Set tap threshold. */
  if (iis2iclx_tap_threshold_x_set(&(pObj->Ctx), 0x08) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Set tap shock time window. */
  if (iis2iclx_tap_shock_set(&(pObj->Ctx), 0x03) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Set tap quiet time window. */
  if (iis2iclx_tap_quiet_set(&(pObj->Ctx), 0x03) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Set tap duration time window. */
  if (iis2iclx_tap_dur_set(&(pObj->Ctx), 0x08) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Single and double tap enabled. */
  if (iis2iclx_tap_mode_set(&(pObj->Ctx), IIS2ICLX_BOTH_SINGLE_DOUBLE) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Enable double tap event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case IIS2ICLX_INT1_PIN:
      if (iis2iclx_pin_int1_route_get(&(pObj->Ctx), &val1) != IIS2ICLX_OK)
      {
        return IIS2ICLX_ERROR;
      }

      val1.md1_cfg.int1_double_tap = PROPERTY_ENABLE;

      if (iis2iclx_pin_int1_route_set(&(pObj->Ctx), &val1) != IIS2ICLX_OK)
      {
        return IIS2ICLX_ERROR;
      }
      break;

    case IIS2ICLX_INT2_PIN:
      if (iis2iclx_pin_int2_route_get(&(pObj->Ctx), &val2) != IIS2ICLX_OK)
      {
        return IIS2ICLX_ERROR;
      }

      val2.md2_cfg.int2_double_tap = PROPERTY_ENABLE;

      if (iis2iclx_pin_int2_route_set(&(pObj->Ctx), &val2) != IIS2ICLX_OK)
      {
        return IIS2ICLX_ERROR;
      }
      break;

    default:
      ret = IIS2ICLX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Disable double tap detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_Disable_Double_Tap_Detection(IIS2ICLX_Object_t *pObj)
{
  iis2iclx_pin_int1_route_t val1;
  iis2iclx_pin_int2_route_t val2;

  /* Disable double tap event on both INT1 and INT2 pins */
  if (iis2iclx_pin_int1_route_get(&(pObj->Ctx), &val1) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  val1.md1_cfg.int1_double_tap = PROPERTY_DISABLE;

  if (iis2iclx_pin_int1_route_set(&(pObj->Ctx), &val1) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  if (iis2iclx_pin_int2_route_get(&(pObj->Ctx), &val2) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  val2.md2_cfg.int2_double_tap = PROPERTY_DISABLE;

  if (iis2iclx_pin_int2_route_set(&(pObj->Ctx), &val2) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Only single tap enabled. */
  if (iis2iclx_tap_mode_set(&(pObj->Ctx), IIS2ICLX_ONLY_SINGLE) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Reset tap duration time window. */
  if (iis2iclx_tap_dur_set(&(pObj->Ctx), 0x00) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Reset tap quiet time window. */
  if (iis2iclx_tap_quiet_set(&(pObj->Ctx), 0x00) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Reset tap shock time window. */
  if (iis2iclx_tap_shock_set(&(pObj->Ctx), 0x00) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Reset tap threshold. */
  if (iis2iclx_tap_threshold_x_set(&(pObj->Ctx), 0x00) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Disable Y direction in tap recognition. */
  if (iis2iclx_tap_detection_on_y_set(&(pObj->Ctx), PROPERTY_DISABLE) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  /* Disable X direction in tap recognition. */
  if (iis2iclx_tap_detection_on_x_set(&(pObj->Ctx), PROPERTY_DISABLE) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Set tap threshold
  * @param  pObj the device pObj
  * @param  Threshold tap threshold
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_Set_Tap_Threshold(IIS2ICLX_Object_t *pObj, uint8_t Threshold)
{
  /* Set tap threshold. */
  if (iis2iclx_tap_threshold_x_set(&(pObj->Ctx), Threshold) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Set tap shock time
  * @param  pObj the device pObj
  * @param  Time tap shock time
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_Set_Tap_Shock_Time(IIS2ICLX_Object_t *pObj, uint8_t Time)
{
  /* Set tap shock time window. */
  if (iis2iclx_tap_shock_set(&(pObj->Ctx), Time) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Set tap quiet time
  * @param  pObj the device pObj
  * @param  Time tap quiet time
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_Set_Tap_Quiet_Time(IIS2ICLX_Object_t *pObj, uint8_t Time)
{
  /* Set tap quiet time window. */
  if (iis2iclx_tap_quiet_set(&(pObj->Ctx), Time) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Set tap duration time
  * @param  pObj the device pObj
  * @param  Time tap duration time
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_Set_Tap_Duration_Time(IIS2ICLX_Object_t *pObj, uint8_t Time)
{
  /* Set tap duration time window. */
  if (iis2iclx_tap_dur_set(&(pObj->Ctx), Time) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Get the IIS2ICLX ACC data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_Get_DRDY_Status(IIS2ICLX_Object_t *pObj, uint8_t *Status)
{
  if (iis2iclx_xl_flag_data_ready_get(&(pObj->Ctx), Status) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Get the status of all hardware events
  * @param  pObj the device pObj
  * @param  Status the status of all hardware events
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_Get_Event_Status(IIS2ICLX_Object_t *pObj, IIS2ICLX_Event_Status_t *Status)
{
  iis2iclx_wake_up_src_t wake_up_src;
  iis2iclx_tap_src_t tap_src;
  iis2iclx_md1_cfg_t md1_cfg;
  iis2iclx_md2_cfg_t md2_cfg;
  iis2iclx_emb_func_int1_t int1_ctrl;
  iis2iclx_emb_func_int2_t int2_ctrl;

  (void)memset((void *)Status, 0x0, sizeof(IIS2ICLX_Event_Status_t));

  if (iis2iclx_read_reg(&(pObj->Ctx), IIS2ICLX_WAKE_UP_SRC, (uint8_t *)&wake_up_src, 1) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  if (iis2iclx_read_reg(&(pObj->Ctx), IIS2ICLX_TAP_SRC, (uint8_t *)&tap_src, 1) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  if (iis2iclx_mem_bank_set(&(pObj->Ctx), IIS2ICLX_EMBEDDED_FUNC_BANK) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  if (iis2iclx_read_reg(&(pObj->Ctx), IIS2ICLX_EMB_FUNC_INT1, (uint8_t *)&int1_ctrl, 1) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  if (iis2iclx_read_reg(&(pObj->Ctx), IIS2ICLX_EMB_FUNC_INT2, (uint8_t *)&int2_ctrl, 1) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  if (iis2iclx_mem_bank_set(&(pObj->Ctx), IIS2ICLX_USER_BANK) != 0)
  {
    return IIS2ICLX_ERROR;
  }

  if (iis2iclx_read_reg(&(pObj->Ctx), IIS2ICLX_MD1_CFG, (uint8_t *)&md1_cfg, 1) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  if (iis2iclx_read_reg(&(pObj->Ctx), IIS2ICLX_MD2_CFG, (uint8_t *)&md2_cfg, 1) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  if ((md1_cfg.int1_wu == 1U) || (md2_cfg.int2_wu == 1U))
  {
    if (wake_up_src.wu_ia == 1U)
    {
      Status->WakeUpStatus = 1;
    }
  }

  if ((md1_cfg.int1_single_tap == 1U) || (md2_cfg.int2_single_tap == 1U))
  {
    if (tap_src.single_tap == 1U)
    {
      Status->TapStatus = 1;
    }
  }

  if ((md1_cfg.int1_double_tap == 1U) || (md2_cfg.int2_double_tap == 1U))
  {
    if (tap_src.double_tap == 1U)
    {
      Status->DoubleTapStatus = 1;
    }
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Set self test
  * @param  pObj the device pObj
  * @param  val the value of st_xl in reg CTRL5_C
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_Set_SelfTest(IIS2ICLX_Object_t *pObj, uint8_t val)
{
  iis2iclx_st_xl_t reg;

  reg = (val == 0U)  ? IIS2ICLX_XL_ST_DISABLE
        : (val == 1U)  ? IIS2ICLX_XL_ST_POSITIVE
        : (val == 2U)  ? IIS2ICLX_XL_ST_NEGATIVE
        :                IIS2ICLX_XL_ST_DISABLE;

  if (iis2iclx_xl_self_test_set(&(pObj->Ctx), reg) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Get the IIS2ICLX FIFO number of samples
  * @param  pObj the device pObj
  * @param  NumSamples number of samples
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_FIFO_Get_Num_Samples(IIS2ICLX_Object_t *pObj, uint16_t *NumSamples)
{
  if (iis2iclx_fifo_data_level_get(&(pObj->Ctx), NumSamples) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Get the IIS2ICLX FIFO full status
  * @param  pObj the device pObj
  * @param  Status FIFO full status
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_FIFO_Get_Full_Status(IIS2ICLX_Object_t *pObj, uint8_t *Status)
{
  iis2iclx_reg_t reg;

  if (iis2iclx_read_reg(&(pObj->Ctx), IIS2ICLX_FIFO_STATUS2, &reg.byte, 1) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  *Status = reg.fifo_status2.fifo_full_ia;

  return IIS2ICLX_OK;
}

/**
  * @brief  Set the IIS2ICLX FIFO full interrupt on INT1 pin
  * @param  pObj the device pObj
  * @param  Status FIFO full interrupt on INT1 pin status
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_FIFO_Set_INT1_FIFO_Full(IIS2ICLX_Object_t *pObj, uint8_t Status)
{
  iis2iclx_reg_t reg;

  if (iis2iclx_read_reg(&(pObj->Ctx), IIS2ICLX_INT1_CTRL, &reg.byte, 1) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  reg.int1_ctrl.int1_fifo_full = Status;

  if (iis2iclx_write_reg(&(pObj->Ctx), IIS2ICLX_INT1_CTRL, &reg.byte, 1) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Set the IIS2ICLX FIFO watermark level
  * @param  pObj the device pObj
  * @param  Watermark FIFO watermark level
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_FIFO_Set_Watermark_Level(IIS2ICLX_Object_t *pObj, uint16_t Watermark)
{
  if (iis2iclx_fifo_watermark_set(&(pObj->Ctx), Watermark) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Set the IIS2ICLX FIFO stop on watermark
  * @param  pObj the device pObj
  * @param  Status FIFO stop on watermark status
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_FIFO_Set_Stop_On_Fth(IIS2ICLX_Object_t *pObj, uint8_t Status)
{
  if (iis2iclx_fifo_stop_on_wtm_set(&(pObj->Ctx), Status) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Set the IIS2ICLX FIFO mode
  * @param  pObj the device pObj
  * @param  Mode FIFO mode
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_FIFO_Set_Mode(IIS2ICLX_Object_t *pObj, uint8_t Mode)
{
  int32_t ret = IIS2ICLX_OK;

  /* Verify that the passed parameter contains one of the valid values. */
  switch ((iis2iclx_fifo_mode_t)Mode)
  {
    case IIS2ICLX_BYPASS_MODE:
    case IIS2ICLX_FIFO_MODE:
    case IIS2ICLX_STREAM_TO_FIFO_MODE:
    case IIS2ICLX_BYPASS_TO_STREAM_MODE:
    case IIS2ICLX_STREAM_MODE:
      break;

    default:
      ret = IIS2ICLX_ERROR;
      break;
  }

  if (ret == IIS2ICLX_ERROR)
  {
    return ret;
  }

  if (iis2iclx_fifo_mode_set(&(pObj->Ctx), (iis2iclx_fifo_mode_t)Mode) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return ret;
}

/**
  * @brief  Get the IIS2ICLX FIFO tag
  * @param  pObj the device pObj
  * @param  Tag FIFO tag
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_FIFO_Get_Tag(IIS2ICLX_Object_t *pObj, uint8_t *Tag)
{
  iis2iclx_fifo_tag_t tag_local;

  if (iis2iclx_fifo_sensor_tag_get(&(pObj->Ctx), &tag_local) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  *Tag = (uint8_t)tag_local;

  return IIS2ICLX_OK;
}

/**
  * @brief  Get the IIS2ICLX FIFO raw data
  * @param  pObj the device pObj
  * @param  Data FIFO raw data array [6]
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_FIFO_Get_Data(IIS2ICLX_Object_t *pObj, uint8_t *Data)
{
  if (iis2iclx_read_reg(&(pObj->Ctx), IIS2ICLX_FIFO_DATA_OUT_X_L, Data, 6) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Get the IIS2ICLX FIFO accelero single sample (16-bit data per 2 axes) and calculate acceleration [mg]
  * @param  pObj the device pObj
  * @param  Acceleration FIFO accelero axes [mg]
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_FIFO_ACC_Get_Axes(IIS2ICLX_Object_t *pObj, IIS2ICLX_Axes_t *Acceleration)
{
  uint8_t data[6];
  int16_t data_raw[3];
  float sensitivity = 0.0f;
  float acceleration_float[3];

  if (IIS2ICLX_FIFO_Get_Data(pObj, data) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  data_raw[0] = ((int16_t)data[1] << 8) | data[0];
  data_raw[1] = ((int16_t)data[3] << 8) | data[2];
  data_raw[2] = 0;

  if (IIS2ICLX_ACC_GetSensitivity(pObj, &sensitivity) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  acceleration_float[0] = (float)data_raw[0] * sensitivity;
  acceleration_float[1] = (float)data_raw[1] * sensitivity;
  acceleration_float[2] = (float)data_raw[2] * sensitivity;

  Acceleration->x = (int32_t)acceleration_float[0];
  Acceleration->y = (int32_t)acceleration_float[1];
  Acceleration->z = (int32_t)acceleration_float[2];

  return IIS2ICLX_OK;
}

/**
  * @brief  Set the IIS2ICLX FIFO accelero BDR value
  * @param  pObj the device pObj
  * @param  Bdr FIFO accelero BDR value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_FIFO_ACC_Set_BDR(IIS2ICLX_Object_t *pObj, float Bdr)
{
  iis2iclx_bdr_xl_t new_bdr;

  new_bdr = (Bdr <=    0.0f) ? IIS2ICLX_XL_NOT_BATCHED
            : (Bdr <=   12.5f) ? IIS2ICLX_XL_BATCHED_AT_12Hz5
            : (Bdr <=   26.0f) ? IIS2ICLX_XL_BATCHED_AT_26Hz
            : (Bdr <=   52.0f) ? IIS2ICLX_XL_BATCHED_AT_52Hz
            : (Bdr <=  104.0f) ? IIS2ICLX_XL_BATCHED_AT_104Hz
            : (Bdr <=  208.0f) ? IIS2ICLX_XL_BATCHED_AT_208Hz
            : (Bdr <=  417.0f) ? IIS2ICLX_XL_BATCHED_AT_417Hz
            :                    IIS2ICLX_XL_BATCHED_AT_833Hz;

  if (iis2iclx_fifo_xl_batch_set(&(pObj->Ctx), new_bdr) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @}
  */

/** @defgroup IIS2ICLX_Private_Functions IIS2ICLX Private Functions
  * @{
  */

/**
  * @brief  Set the IIS2ICLX accelerometer sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t IIS2ICLX_ACC_SetOutputDataRate_When_Enabled(IIS2ICLX_Object_t *pObj, float Odr)
{
  iis2iclx_odr_xl_t new_odr;

  new_odr = (Odr <=   12.5f) ? IIS2ICLX_XL_ODR_12Hz5
            : (Odr <=   26.0f) ? IIS2ICLX_XL_ODR_26Hz
            : (Odr <=   52.0f) ? IIS2ICLX_XL_ODR_52Hz
            : (Odr <=  104.0f) ? IIS2ICLX_XL_ODR_104Hz
            : (Odr <=  208.0f) ? IIS2ICLX_XL_ODR_208Hz
            : (Odr <=  416.0f) ? IIS2ICLX_XL_ODR_416Hz
            :                    IIS2ICLX_XL_ODR_833Hz;

  /* Output data rate selection. */
  if (iis2iclx_xl_data_rate_set(&(pObj->Ctx), new_odr) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Set the IIS2ICLX accelerometer sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t IIS2ICLX_ACC_SetOutputDataRate_When_Disabled(IIS2ICLX_Object_t *pObj, float Odr)
{
  pObj->acc_odr = (Odr <=   12.5f) ? IIS2ICLX_XL_ODR_12Hz5
                  : (Odr <=   26.0f) ? IIS2ICLX_XL_ODR_26Hz
                  : (Odr <=   52.0f) ? IIS2ICLX_XL_ODR_52Hz
                  : (Odr <=  104.0f) ? IIS2ICLX_XL_ODR_104Hz
                  : (Odr <=  208.0f) ? IIS2ICLX_XL_ODR_208Hz
                  : (Odr <=  416.0f) ? IIS2ICLX_XL_ODR_416Hz
                  :                    IIS2ICLX_XL_ODR_833Hz;

  return IIS2ICLX_OK;
}

/**
  * @brief  Enable IIS2ICLX accelerometer DRDY interrupt mode
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_Enable_DRDY_Interrupt(IIS2ICLX_Object_t *pObj)
{
  iis2iclx_pin_int1_route_t pin_int1_route;

  /* Enable accelerometer DRDY Interrupt on INT1 */
  if (iis2iclx_pin_int1_route_get(&(pObj->Ctx), &pin_int1_route) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }
  pin_int1_route.int1_ctrl.int1_drdy_xl = 1;
  if (iis2iclx_pin_int1_route_set(&(pObj->Ctx), &pin_int1_route) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
}

/**
  * @brief  Set the IIS2ICLX accelerometer filter mode
  * @param  pObj the device pObj
  * @param  LowHighPassFlag 0/1 for setting low-pass/high-pass filter mode
  * @param  FilterMode Value of the filter Mode
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_Set_Filter_Mode(IIS2ICLX_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode)
{
  if (LowHighPassFlag == 0)
  {
    /*Set accelerometer low_pass filter-mode*/

    /*Set to 1 LPF2 bit (CTRL8_XL)*/
    if (iis2iclx_xl_filter_lp2_set(&(pObj->Ctx), 1) != IIS2ICLX_OK)
    {
      return IIS2ICLX_ERROR;
    }
    if (iis2iclx_xl_hp_path_on_out_set(&(pObj->Ctx), (iis2iclx_hp_slope_xl_en_t)FilterMode) != IIS2ICLX_OK)
    {
      return IIS2ICLX_ERROR;
    }
  }
  else
  {
    /*Set accelerometer high_pass filter-mode*/
    if (iis2iclx_xl_hp_path_on_out_set(&(pObj->Ctx), (iis2iclx_hp_slope_xl_en_t)FilterMode) != IIS2ICLX_OK)
    {
      return IIS2ICLX_ERROR;
    }
  }
  return IIS2ICLX_OK;
}

/**
  * @brief  Set LSM6DSO accelerometer sleep duration
  * @param  pObj the device pObj
  * @param  Duration wake up detection duration
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2ICLX_ACC_Set_Sleep_Duration(IIS2ICLX_Object_t *pObj, uint8_t Duration)
{
  /* Set wake up duration. */
  if (iis2iclx_act_sleep_dur_set(&(pObj->Ctx), Duration) != IIS2ICLX_OK)
  {
    return IIS2ICLX_ERROR;
  }

  return IIS2ICLX_OK;
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
  IIS2ICLX_Object_t *pObj = (IIS2ICLX_Object_t *)Handle;

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
  IIS2ICLX_Object_t *pObj = (IIS2ICLX_Object_t *)Handle;

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
