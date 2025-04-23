/**
  ******************************************************************************
  * @file    iis2dulpx.c
  * @author  MEMS Software Solutions Team
  * @brief   IIS2DULPX driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "iis2dulpx.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup IIS2DULPX IIS2DULPX
  * @{
  */

/** @defgroup IIS2DULPX_Exported_Variables IIS2DULPX Exported Variables
  * @{
  */

IIS2DULPX_CommonDrv_t IIS2DULPX_COMMON_Driver =
{
  IIS2DULPX_Init,
  IIS2DULPX_DeInit,
  IIS2DULPX_ReadID,
  IIS2DULPX_GetCapabilities,
};

IIS2DULPX_ACC_Drv_t IIS2DULPX_ACC_Driver =
{
  IIS2DULPX_ACC_Enable,
  IIS2DULPX_ACC_Disable,
  IIS2DULPX_ACC_GetSensitivity,
  IIS2DULPX_ACC_GetOutputDataRate,
  IIS2DULPX_ACC_SetOutputDataRate,
  IIS2DULPX_ACC_GetFullScale,
  IIS2DULPX_ACC_SetFullScale,
  IIS2DULPX_ACC_GetAxes,
  IIS2DULPX_ACC_GetAxesRaw,
};

/**
  * @}
  */

/** @defgroup IIS2DULPX_Private_Function_Prototypes IIS2DULPX Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t IIS2DULPX_ACC_SetOutputDataRate_When_Enabled(IIS2DULPX_Object_t *pObj, float_t Odr,
    IIS2DULPX_Power_Mode_t Power);
static int32_t IIS2DULPX_ACC_SetOutputDataRate_When_Disabled(IIS2DULPX_Object_t *pObj, float_t Odr,
    IIS2DULPX_Power_Mode_t Power);

/**
  * @}
  */

/** @defgroup IIS2DULPX_Exported_Functions IIS2DULPX Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_RegisterBusIO(IIS2DULPX_Object_t *pObj, IIS2DULPX_IO_t *pIO)
{
  int32_t ret = IIS2DULPX_OK;

  if (pObj == NULL)
  {
    ret = IIS2DULPX_ERROR;
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
    pObj->IO.Delay     = pIO->Delay;

    pObj->Ctx.read_reg  = ReadRegWrap;
    pObj->Ctx.write_reg = WriteRegWrap;
    pObj->Ctx.mdelay    = pIO->Delay;
    pObj->Ctx.handle    = pObj;

    if (pObj->IO.Init == NULL)
    {
      ret = IIS2DULPX_ERROR;
    }
    else if (pObj->IO.Init() != IIS2DULPX_OK)
    {
      ret = IIS2DULPX_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == IIS2DULPX_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Exit from deep power down only the first time in SPI mode */
          if (IIS2DULPX_ExitDeepPowerDownSPI(pObj) != IIS2DULPX_OK)
          {
            /* Forced OK because of an expected failure during the wake-up sequence */
            ret = IIS2DULPX_OK;
          }
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x50;

          if (IIS2DULPX_Write_Reg(pObj, IIS2DULPX_CTRL1, data) != IIS2DULPX_OK)
          {
            ret = IIS2DULPX_ERROR;
          }
        }
      }
      else if (pObj->IO.BusType == IIS2DULPX_SPI_4WIRES_BUS)
      {
        /* Exit from deep power down only the first time in SPI mode */
        if (pObj->is_initialized == 0U)
        {
          if (IIS2DULPX_ExitDeepPowerDownSPI(pObj) != IIS2DULPX_OK)
          {
            /* Forced OK because of an expected failure during the wake-up sequence */
            ret = IIS2DULPX_OK;
          }
        }
      }
      else if (pObj->IO.BusType == IIS2DULPX_I2C_BUS)
      {
        /* Exit from deep power down only the first time in I2C mode */
        if (pObj->is_initialized == 0U)
        {
          if (IIS2DULPX_ExitDeepPowerDownI2C(pObj) != IIS2DULPX_OK)
          {
            pObj->Ctx.mdelay(100);
            
            /* Forced OK because of an expected failure during the wake-up sequence */
            ret = IIS2DULPX_OK;
          }
        }
      }
      else
      {
        /* Do nothing */
      }
    }
  }

  return ret;
}

/**
  * @brief  Initialize the IIS2DULPX sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_Init(IIS2DULPX_Object_t *pObj)
{
  iis2dulpx_i3c_cfg_t val;

  if(pObj->IO.BusType != IIS2DULPX_I3C_BUS)
  {
    /* Disable I3C */
    if (iis2dulpx_i3c_configure_get(&(pObj->Ctx), &val) != IIS2DULPX_OK)
    {
      return IIS2DULPX_ERROR;
    }
    val.asf_on = PROPERTY_ENABLE;
    if (iis2dulpx_i3c_configure_set(&(pObj->Ctx), &val) != IIS2DULPX_OK)
    {
      return IIS2DULPX_ERROR;
    }
  }

  /* Set main memory bank */
  if (IIS2DULPX_Set_Mem_Bank(pObj, (uint8_t)IIS2DULPX_MAIN_MEM_BANK) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  /* Enable register address automatically incremented during a multiple byte
  access with a serial interface. Enable BDU. */
  if (iis2dulpx_init_set(&(pObj->Ctx), IIS2DULPX_SENSOR_ONLY_ON) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  /* FIFO mode selection */
  iis2dulpx_fifo_mode_t fifo_mode =
  {
    .operation = IIS2DULPX_BYPASS_MODE,
    .store     = IIS2DULPX_FIFO_1X,
    .watermark = 0,
  };

  if (iis2dulpx_fifo_mode_set(&(pObj->Ctx), fifo_mode) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  /* Select default output data rate */
  pObj->acc_odr = 100.0f;

  /* Select default high performance mode (when disabled) */
  pObj->power_mode = IIS2DULPX_HIGH_PERFORMANCE;

  /* Output data rate: power down, full scale: 2g */
  iis2dulpx_md_t mode =
  {
    .odr = IIS2DULPX_OFF,
    .fs  = IIS2DULPX_2g,
  };

  if (iis2dulpx_mode_set(&(pObj->Ctx), &mode) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  pObj->is_initialized = 1;

  return IIS2DULPX_OK;
}

/**
  * @brief  Deinitialize the IIS2DULPX sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_DeInit(IIS2DULPX_Object_t *pObj)
{
  /* Disable the component */
  if (IIS2DULPX_ACC_Disable(pObj) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  /* Reset output data rate */
  pObj->acc_odr = 0.0f;

  /* Set low power mode (when disabled) */
  pObj->power_mode = IIS2DULPX_LOW_POWER;

  pObj->is_initialized = 0;

  return IIS2DULPX_OK;
}

/**
  * @brief  Exit from deep power down in I2C
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ExitDeepPowerDownI2C(IIS2DULPX_Object_t *pObj)
{
  uint8_t val;

  /* Perform dummy read in order to exit from deep power down in I2C mode*/
  if (iis2dulpx_device_id_get(&(pObj->Ctx), &val) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  /* Wait for 25 ms based on datasheet */
  pObj->Ctx.mdelay(25);

  return IIS2DULPX_OK;
}

/**
  * @brief  Exit from deep power down in SPI
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ExitDeepPowerDownSPI(IIS2DULPX_Object_t *pObj)
{
  /* Write IF_WAKE_UP register to exit from deep power down in SPI mode*/
  if (iis2dulpx_exit_deep_power_down(&(pObj->Ctx)) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  /* Wait for 25 ms based on datasheet */
  pObj->Ctx.mdelay(25);

  return IIS2DULPX_OK;
}

/**
  * @brief  Read component ID
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ReadID(IIS2DULPX_Object_t *pObj, uint8_t *Id)
{
  uint8_t val;

  if (iis2dulpx_device_id_get(&(pObj->Ctx), &val) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  *Id = val;

  return IIS2DULPX_OK;
}

/**
  * @brief  Get IIS2DULPX sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to IIS2DULPX sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_GetCapabilities(IIS2DULPX_Object_t *pObj, IIS2DULPX_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Acc        = 1;
  Capabilities->Gyro       = 0;
  Capabilities->Magneto    = 0;
  Capabilities->LowPower   = 1;
  Capabilities->GyroMaxFS  = 0;
  Capabilities->AccMaxFS   = 16;
  Capabilities->MagMaxFS   = 0;
  Capabilities->GyroMaxOdr = 0.0f;
  Capabilities->AccMaxOdr  = 800.0f;
  Capabilities->MagMaxOdr  = 0.0f;
  return IIS2DULPX_OK;
}

/**
  * @brief  Enable the IIS2DULPX accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_Enable(IIS2DULPX_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return IIS2DULPX_OK;
  }

  /* Output data rate selection. */
  if (IIS2DULPX_ACC_SetOutputDataRate_When_Enabled(pObj, pObj->acc_odr, pObj->power_mode) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return IIS2DULPX_OK;
}

/**
  * @brief  Disable the IIS2DULPX accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_Disable(IIS2DULPX_Object_t *pObj)
{
  float_t Odr;
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return IIS2DULPX_OK;
  }

  if (IIS2DULPX_ACC_GetOutputDataRate(pObj, &Odr) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  if (Odr > 400.0f)
  {
    if (IIS2DULPX_ACC_SetOutputDataRate(pObj, 400.0f) != IIS2DULPX_OK)
    {
      return IIS2DULPX_ERROR;
    }

    /* Wait for 3 ms based on datasheet */
    pObj->Ctx.mdelay(3);
  }

  /* Output data rate selection - power down. */
  iis2dulpx_md_t mode;

  if (iis2dulpx_mode_get(&(pObj->Ctx), &mode) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  mode.odr = IIS2DULPX_OFF;

  if (iis2dulpx_mode_set(&(pObj->Ctx), &mode) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return IIS2DULPX_OK;
}

/**
  * @brief  Get the IIS2DULPX accelerometer sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_GetSensitivity(IIS2DULPX_Object_t *pObj, float_t *Sensitivity)
{
  int32_t ret = IIS2DULPX_OK;
  iis2dulpx_md_t mode;

  if (iis2dulpx_mode_get(&(pObj->Ctx), &mode) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  switch (mode.fs)
  {
    case IIS2DULPX_2g:
      *Sensitivity = IIS2DULPX_ACC_SENSITIVITY_FOR_FS_2G;
      break;

    case IIS2DULPX_4g:
      *Sensitivity = IIS2DULPX_ACC_SENSITIVITY_FOR_FS_4G;
      break;

    case IIS2DULPX_8g:
      *Sensitivity = IIS2DULPX_ACC_SENSITIVITY_FOR_FS_8G;
      break;

    case IIS2DULPX_16g:
      *Sensitivity = IIS2DULPX_ACC_SENSITIVITY_FOR_FS_16G;
      break;

    default:
      *Sensitivity = -1.0f;
      ret = IIS2DULPX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the IIS2DULPX accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_GetOutputDataRate(IIS2DULPX_Object_t *pObj, float_t *Odr)
{
  int32_t ret = IIS2DULPX_OK;
  iis2dulpx_md_t mode;

  /* Read actual output data rate from sensor. */
  if (iis2dulpx_mode_get(&(pObj->Ctx), &mode) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  switch (mode.odr)
  {
    case IIS2DULPX_OFF:
    case IIS2DULPX_TRIG_PIN:
    case IIS2DULPX_TRIG_SW:
      *Odr = 0.0f;
      break;

    case IIS2DULPX_1Hz6_ULP:
      *Odr = 1.6f;
      break;

    case IIS2DULPX_3Hz_ULP:
      *Odr = 3.0f;
      break;

    case IIS2DULPX_6Hz_LP:
    case IIS2DULPX_6Hz_HP:
      *Odr = 6.0f;
      break;

    case IIS2DULPX_12Hz5_LP:
    case IIS2DULPX_12Hz5_HP:
      *Odr = 12.5f;
      break;

    case IIS2DULPX_25Hz_ULP:
    case IIS2DULPX_25Hz_LP:
    case IIS2DULPX_25Hz_HP:
      *Odr = 25.0f;
      break;

    case IIS2DULPX_50Hz_LP:
    case IIS2DULPX_50Hz_HP:
      *Odr = 50.0f;
      break;

    case IIS2DULPX_100Hz_LP:
    case IIS2DULPX_100Hz_HP:
      *Odr = 100.0f;
      break;

    case IIS2DULPX_200Hz_LP:
    case IIS2DULPX_200Hz_HP:
      *Odr = 200.0f;
      break;

    case IIS2DULPX_400Hz_LP:
    case IIS2DULPX_400Hz_HP:
      *Odr = 400.0f;
      break;

    case IIS2DULPX_800Hz_LP:
    case IIS2DULPX_800Hz_HP:
      *Odr = 800.0f;
      break;

    default:
      *Odr = -1.0f;
      ret = IIS2DULPX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the IIS2DULPX accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_SetOutputDataRate(IIS2DULPX_Object_t *pObj, float_t Odr)
{
  /* By default we use high performance mode */
  return IIS2DULPX_ACC_SetOutputDataRate_With_Mode(pObj, Odr, IIS2DULPX_HIGH_PERFORMANCE);
}

/**
  * @brief  Set the IIS2DULPX accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @param  Power the ultra low power option
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_SetOutputDataRate_With_Mode(IIS2DULPX_Object_t *pObj, float_t Odr, IIS2DULPX_Power_Mode_t Power)
{
  /* Check if the component is enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return IIS2DULPX_ACC_SetOutputDataRate_When_Enabled(pObj, Odr, Power);
  }
  else
  {
    return IIS2DULPX_ACC_SetOutputDataRate_When_Disabled(pObj, Odr, Power);
  }
}

/**
  * @brief  Get the IIS2DULPX accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_GetFullScale(IIS2DULPX_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = IIS2DULPX_OK;
  iis2dulpx_md_t mode;

  /* Read actual full scale selection from sensor. */
  if (iis2dulpx_mode_get(&(pObj->Ctx), &mode) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  switch (mode.fs)
  {
    case IIS2DULPX_2g:
      *FullScale =  2;
      break;

    case IIS2DULPX_4g:
      *FullScale =  4;
      break;

    case IIS2DULPX_8g:
      *FullScale =  8;
      break;

    case IIS2DULPX_16g:
      *FullScale = 16;
      break;

    default:
      *FullScale = -1;
      ret = IIS2DULPX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the IIS2DULPX accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_SetFullScale(IIS2DULPX_Object_t *pObj, int32_t FullScale)
{
  iis2dulpx_md_t mode;

  if (iis2dulpx_mode_get(&(pObj->Ctx), &mode) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file statical analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  mode.fs = (FullScale <= 2) ? IIS2DULPX_2g
            : (FullScale <= 4) ? IIS2DULPX_4g
            : (FullScale <= 8) ? IIS2DULPX_8g
            :                    IIS2DULPX_16g;

  if (iis2dulpx_mode_set(&(pObj->Ctx), &mode) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  return IIS2DULPX_OK;
}

/**
  * @brief  Get the IIS2DULPX accelerometer sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_GetAxesRaw(IIS2DULPX_Object_t *pObj, IIS2DULPX_AxesRaw_t *Value)
{
  iis2dulpx_md_t mode;
  iis2dulpx_xl_data_t data;

  if (iis2dulpx_mode_get(&(pObj->Ctx), &mode) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  if (iis2dulpx_xl_data_get(&(pObj->Ctx), &mode, &data) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  Value->x = data.raw[0];
  Value->y = data.raw[1];
  Value->z = data.raw[2];

  return IIS2DULPX_OK;
}

/**
  * @brief  Get the IIS2DULPX accelerometer sensor axes
  * @param  pObj the device pObj
  * @param  Acceleration pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_GetAxes(IIS2DULPX_Object_t *pObj, IIS2DULPX_Axes_t *Acceleration)
{
  iis2dulpx_md_t mode;
  iis2dulpx_xl_data_t data;

  if (iis2dulpx_mode_get(&(pObj->Ctx), &mode) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  if (iis2dulpx_xl_data_get(&(pObj->Ctx), &mode, &data) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  Acceleration->x = (int32_t)data.mg[0];
  Acceleration->y = (int32_t)data.mg[1];
  Acceleration->z = (int32_t)data.mg[2];

  return IIS2DULPX_OK;
}

/**
  * @brief  Get the IIS2DULPX register value
  * @param  pObj the device pObj
  * @param  Reg address to be read
  * @param  Data pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_Read_Reg(IIS2DULPX_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (iis2dulpx_read_reg(&(pObj->Ctx), Reg, Data, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  return IIS2DULPX_OK;
}

/**
  * @brief  Set the IIS2DULPX register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_Write_Reg(IIS2DULPX_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (iis2dulpx_write_reg(&(pObj->Ctx), Reg, &Data, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  return IIS2DULPX_OK;
}

/**
  * @brief  Get the IIS2DULPX ACC data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_Get_DRDY_Status(IIS2DULPX_Object_t *pObj, uint8_t *Status)
{
  iis2dulpx_status_t device_status;

  if (iis2dulpx_status_get(&(pObj->Ctx), &device_status) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  *Status = device_status.drdy;

  return IIS2DULPX_OK;
}

/**
  * @brief  Get the IIS2DULPX ACC initialization status
  * @param  pObj the device pObj
  * @param  Status 1 if initialized, 0 otherwise
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_Get_Init_Status(IIS2DULPX_Object_t *pObj, uint8_t *Status)
{
  if (pObj == NULL)
  {
    return IIS2DULPX_ERROR;
  }

  *Status = pObj->is_initialized;

  return IIS2DULPX_OK;
}

/**
  * @brief  Set memory bank
  * @param  pObj the device pObj
  * @param  Val the value of memory bank in reg FUNC_CFG_ACCESS
  *         0 - IIS2DULPX_MAIN_MEM_BANK, 1 - IIS2DULPX_EMBED_FUNC_MEM_BANK
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_Set_Mem_Bank(IIS2DULPX_Object_t *pObj, uint8_t Val)
{
  int32_t ret = IIS2DULPX_OK;
  iis2dulpx_mem_bank_t reg;

  reg = (Val == 1U) ? IIS2DULPX_EMBED_FUNC_MEM_BANK
        :               IIS2DULPX_MAIN_MEM_BANK;

  if (iis2dulpx_mem_bank_set(&(pObj->Ctx), reg) != IIS2DULPX_OK)
  {
    ret = IIS2DULPX_ERROR;
  }

  return ret;
}

/**
  * @}
  */

/** @defgroup IIS2DULPX_Private_Functions IIS2DULPX Private Functions
  * @{
  */

/**
  * @brief  Set the IIS2DULPX accelerometer sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @param  Power the ultra low power option
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t IIS2DULPX_ACC_SetOutputDataRate_When_Enabled(IIS2DULPX_Object_t *pObj, float_t Odr,
    IIS2DULPX_Power_Mode_t Power)
{
  iis2dulpx_md_t mode;

  if (iis2dulpx_mode_get(&(pObj->Ctx), &mode) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  if (Power == IIS2DULPX_ULTRA_LOW_POWER)
  {
    mode.odr = (Odr <= 1.6f) ? IIS2DULPX_1Hz6_ULP
               : (Odr <= 3.0f) ? IIS2DULPX_3Hz_ULP
               :                 IIS2DULPX_25Hz_ULP;
  }
  else if (Power == IIS2DULPX_LOW_POWER)
  {
    mode.odr = (Odr <=   6.0f) ? IIS2DULPX_6Hz_LP
               : (Odr <=  12.5f) ? IIS2DULPX_12Hz5_LP
               : (Odr <=  25.0f) ? IIS2DULPX_25Hz_LP
               : (Odr <=  50.0f) ? IIS2DULPX_50Hz_LP
               : (Odr <= 100.0f) ? IIS2DULPX_100Hz_LP
               : (Odr <= 200.0f) ? IIS2DULPX_200Hz_LP
               : (Odr <= 400.0f) ? IIS2DULPX_400Hz_LP
               :                   IIS2DULPX_800Hz_LP;
  }
  else if (Power == IIS2DULPX_HIGH_PERFORMANCE)
  {
    mode.odr = (Odr <=   6.0f) ? IIS2DULPX_6Hz_HP
               : (Odr <=  12.5f) ? IIS2DULPX_12Hz5_HP
               : (Odr <=  25.0f) ? IIS2DULPX_25Hz_HP
               : (Odr <=  50.0f) ? IIS2DULPX_50Hz_HP
               : (Odr <= 100.0f) ? IIS2DULPX_100Hz_HP
               : (Odr <= 200.0f) ? IIS2DULPX_200Hz_HP
               : (Odr <= 400.0f) ? IIS2DULPX_400Hz_HP
               :                   IIS2DULPX_800Hz_HP;
  }
  else
  {
    /* Do nothing */
  }

  if (iis2dulpx_mode_set(&(pObj->Ctx), &mode) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  /* Store the current Odr value */
  pObj->acc_odr = (mode.odr == IIS2DULPX_1Hz6_ULP) ?   1.6f
                  : (mode.odr == IIS2DULPX_3Hz_ULP)  ?   3.0f
                  : (mode.odr == IIS2DULPX_6Hz_LP)   ?   6.0f
                  : (mode.odr == IIS2DULPX_6Hz_HP)   ?   6.0f
                  : (mode.odr == IIS2DULPX_12Hz5_LP) ?  12.5f
                  : (mode.odr == IIS2DULPX_12Hz5_HP) ?  12.5f
                  : (mode.odr == IIS2DULPX_25Hz_ULP) ?  25.0f
                  : (mode.odr == IIS2DULPX_25Hz_LP)  ?  25.0f
                  : (mode.odr == IIS2DULPX_25Hz_HP)  ?  25.0f
                  : (mode.odr == IIS2DULPX_50Hz_LP)  ?  50.0f
                  : (mode.odr == IIS2DULPX_50Hz_HP)  ?  50.0f
                  : (mode.odr == IIS2DULPX_100Hz_LP) ? 100.0f
                  : (mode.odr == IIS2DULPX_100Hz_HP) ? 100.0f
                  : (mode.odr == IIS2DULPX_200Hz_LP) ? 200.0f
                  : (mode.odr == IIS2DULPX_200Hz_HP) ? 200.0f
                  : (mode.odr == IIS2DULPX_400Hz_LP) ? 400.0f
                  : (mode.odr == IIS2DULPX_400Hz_HP) ? 400.0f
                  : (mode.odr == IIS2DULPX_800Hz_LP) ? 800.0f
                  : (mode.odr == IIS2DULPX_800Hz_HP) ? 800.0f
                  :                                     -1.0f;

  if (pObj->acc_odr < 0.0f)
  {
    return IIS2DULPX_ERROR;
  }

  /* Store the current Power value */
  pObj->power_mode = Power;

  return IIS2DULPX_OK;
}

/**
  * @brief  Set the IIS2DULPX accelerometer sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @param  Power the ultra low power option
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t IIS2DULPX_ACC_SetOutputDataRate_When_Disabled(IIS2DULPX_Object_t *pObj, float_t Odr,
    IIS2DULPX_Power_Mode_t Power)
{
  /* Store the new Odr value */
  if (Power == IIS2DULPX_ULTRA_LOW_POWER)
  {
    pObj->acc_odr = (Odr <= 1.5f) ? 1.5f
                    : (Odr <= 3.0f) ? 3.0f
                    :                25.0f;
  }
  else if ((Power == IIS2DULPX_LOW_POWER) || (Power == IIS2DULPX_HIGH_PERFORMANCE))
  {
    pObj->acc_odr = (Odr <=   6.0f) ?   6.0f
                    : (Odr <=  12.5f) ?  12.5f
                    : (Odr <=  25.0f) ?  25.0f
                    : (Odr <=  50.0f) ?  50.0f
                    : (Odr <= 100.0f) ? 100.0f
                    : (Odr <= 200.0f) ? 200.0f
                    : (Odr <= 400.0f) ? 400.0f
                    :                   800.0f;
  }
  else
  {
    /* Do nothing */
  }

  /* Store the new Power value */
  pObj->power_mode = Power;

  return IIS2DULPX_OK;
}

/**
  * @brief  Get the status of all hardware events
  * @param  pObj the device pObj
  * @param  Status the status of all hardware events
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_Get_Event_Status(IIS2DULPX_Object_t *pObj, IIS2DULPX_Event_Status_t *Status)
{
  iis2dulpx_all_int_src_t all_int_src_reg;
  iis2dulpx_md1_cfg_t md1_cfg_reg;
  iis2dulpx_md2_cfg_t md2_cfg_reg;

  (void)memset((void *)Status, 0x0, sizeof(IIS2DULPX_Event_Status_t));

  if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_ALL_INT_SRC, (uint8_t *)&all_int_src_reg, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_MD1_CFG, (uint8_t *)&md1_cfg_reg, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_MD2_CFG, (uint8_t *)&md2_cfg_reg, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  if ((md1_cfg_reg.int1_wu == 1U) || (md2_cfg_reg.int2_wu == 1U))
  {
    if (all_int_src_reg.wu_ia_all == 1U)
    {
      Status->WakeUpStatus = 1;
    }
  }

  if ((md1_cfg_reg.int1_6d == 1U) || (md2_cfg_reg.int2_6d == 1U))
  {
    if (all_int_src_reg.d6d_ia_all == 1U)
    {
      Status->D6DOrientationStatus = 1;
    }
  }

  return IIS2DULPX_OK;
}

/**
  * @brief  Enable wake up detection
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_Enable_Wake_Up_Detection(IIS2DULPX_Object_t *pObj, IIS2DULPX_SensorIntPin_t IntPin)
{
  int32_t ret = IIS2DULPX_OK;
  iis2dulpx_md1_cfg_t val1;
  iis2dulpx_md2_cfg_t val2;
  iis2dulpx_interrupt_cfg_t interrupt_cfg;
  iis2dulpx_ctrl1_t ctrl1;

  /* Output Data Rate selection */
  if (IIS2DULPX_ACC_SetOutputDataRate(pObj, 200.0f) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  /* Full scale selection */
  if (IIS2DULPX_ACC_SetFullScale(pObj, 2) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  /* Set wake-up threshold */
  if (IIS2DULPX_ACC_Set_Wake_Up_Threshold(pObj, 63) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  /* Set wake-up duration */
  if (IIS2DULPX_ACC_Set_Wake_Up_Duration(pObj, 0) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  /* Enable wake-up event on the 3-axis */
  if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_CTRL1, (uint8_t *)&ctrl1, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  ctrl1.wu_x_en = PROPERTY_ENABLE;
  ctrl1.wu_y_en = PROPERTY_ENABLE;
  ctrl1.wu_z_en = PROPERTY_ENABLE;

  if (iis2dulpx_write_reg(&(pObj->Ctx), IIS2DULPX_CTRL1, (uint8_t *)&ctrl1, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  /* Enable wake up event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case IIS2DULPX_INT1_PIN:
      if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_MD1_CFG, (uint8_t *)&val1, 1) != IIS2DULPX_OK)
      {
        return IIS2DULPX_ERROR;
      }

      val1.int1_wu = PROPERTY_ENABLE;

      if (iis2dulpx_write_reg(&(pObj->Ctx), IIS2DULPX_MD1_CFG, (uint8_t *)&val1, 1) != IIS2DULPX_OK)
      {
        return IIS2DULPX_ERROR;
      }

      if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != IIS2DULPX_OK)
      {
        return IIS2DULPX_ERROR;
      }

      interrupt_cfg.interrupts_enable = PROPERTY_ENABLE;

      if (iis2dulpx_write_reg(&(pObj->Ctx), IIS2DULPX_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != IIS2DULPX_OK)
      {
        return IIS2DULPX_ERROR;
      }
      break;

    case IIS2DULPX_INT2_PIN:
      if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_MD2_CFG, (uint8_t *)&val2, 1) != IIS2DULPX_OK)
      {
        return IIS2DULPX_ERROR;
      }

      val2.int2_wu = PROPERTY_ENABLE;

      if (iis2dulpx_write_reg(&(pObj->Ctx), IIS2DULPX_MD2_CFG, (uint8_t *)&val2, 1) != IIS2DULPX_OK)
      {
        return IIS2DULPX_ERROR;
      }

      if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != IIS2DULPX_OK)
      {
        return IIS2DULPX_ERROR;
      }

      interrupt_cfg.interrupts_enable = PROPERTY_ENABLE;

      if (iis2dulpx_write_reg(&(pObj->Ctx), IIS2DULPX_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != IIS2DULPX_OK)
      {
        return IIS2DULPX_ERROR;
      }
      break;

    default:
      ret = IIS2DULPX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Disable wake up detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_Disable_Wake_Up_Detection(IIS2DULPX_Object_t *pObj)
{
  iis2dulpx_md1_cfg_t val1;
  iis2dulpx_md2_cfg_t val2;
  iis2dulpx_ctrl1_t ctrl1;

  /* Disable wake up event on both INT1 and INT2 pins */
  if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_MD1_CFG, (uint8_t *)&val1, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  val1.int1_wu = PROPERTY_DISABLE;

  if (iis2dulpx_write_reg(&(pObj->Ctx), IIS2DULPX_MD1_CFG, (uint8_t *)&val1, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_MD2_CFG, (uint8_t *)&val2, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  val2.int2_wu = PROPERTY_DISABLE;

  if (iis2dulpx_write_reg(&(pObj->Ctx), IIS2DULPX_MD2_CFG, (uint8_t *)&val2, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  /* Disable wake-up event on the 3-axis */
  if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_CTRL1, (uint8_t *)&ctrl1, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  ctrl1.wu_x_en = PROPERTY_DISABLE;
  ctrl1.wu_y_en = PROPERTY_DISABLE;
  ctrl1.wu_z_en = PROPERTY_DISABLE;

  if (iis2dulpx_write_reg(&(pObj->Ctx), IIS2DULPX_CTRL1, (uint8_t *)&ctrl1, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  /* Reset wake-up threshold */
  if (IIS2DULPX_ACC_Set_Wake_Up_Threshold(pObj, 0) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  /* Reset wake-up duration */
  if (IIS2DULPX_ACC_Set_Wake_Up_Duration(pObj, 0) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  return IIS2DULPX_OK;
}

/**
  * @brief  Set wake up threshold
  * @param  pObj the device pObj
  * @param  Threshold wake up detection threshold
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_Set_Wake_Up_Threshold(IIS2DULPX_Object_t *pObj, uint32_t Threshold)
{
  int32_t fs;
  int32_t ret = IIS2DULPX_OK;
  float_t tmp;
  iis2dulpx_interrupt_cfg_t interrupt_cfg;
  iis2dulpx_wake_up_ths_t wup_ths;

  if (IIS2DULPX_ACC_GetFullScale(pObj, &fs) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  if(iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  if(iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_WAKE_UP_THS, (uint8_t *)&wup_ths, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  switch(fs)
  {
    case 2:
      if(Threshold < (uint32_t)(7.8125f * 63.0f))
      {
        interrupt_cfg.wake_ths_w = 1;
        tmp = (float_t)Threshold / 7.8125f;
        wup_ths.wk_ths = (uint8_t)tmp;
      }
      else if (Threshold < (uint32_t)(31.25f * 63.0f))
      {
        interrupt_cfg.wake_ths_w = 0;
        tmp = (float_t)Threshold / 31.25f;
        wup_ths.wk_ths = (uint8_t)tmp;
      }
      else // Out of limit, we set it to the maximum possible value
      {
        interrupt_cfg.wake_ths_w = 0;
        wup_ths.wk_ths = 63;
      }
      break;

    case 4:
      if(Threshold < (uint32_t)(15.625f * 63.0f))
      {
        interrupt_cfg.wake_ths_w = 1;
        tmp = (float_t)Threshold / 15.625f;
        wup_ths.wk_ths = (uint8_t)tmp;
      }
      else if (Threshold < (uint32_t)(62.5f * 63.0f))
      {
        interrupt_cfg.wake_ths_w = 0;
        tmp = (float_t)Threshold / 62.5f;
        wup_ths.wk_ths = (uint8_t)tmp;
      }
      else // Out of limit, we set it to the maximum possible value
      {
        interrupt_cfg.wake_ths_w = 0;
        wup_ths.wk_ths = 63;
      }
      break;

    case 8:
      if(Threshold < (uint32_t)(31.25f * 63.0f))
      {
        interrupt_cfg.wake_ths_w = 1;
        tmp = (float_t)Threshold / 31.25f;
        wup_ths.wk_ths = (uint8_t)tmp;
      }
      else if (Threshold < (uint32_t)(125.0f * 63.0f))
      {
        interrupt_cfg.wake_ths_w = 0;
        tmp = (float_t)Threshold / 125.0f;
        wup_ths.wk_ths = (uint8_t)tmp;
      }
      else // Out of limit, we set it to the maximum possible value
      {
        interrupt_cfg.wake_ths_w = 0;
        wup_ths.wk_ths = 63;
      }
      break;

    case 16:
      if(Threshold < (uint32_t)(62.5f * 63.0f))
      {
        interrupt_cfg.wake_ths_w = 1;
        tmp = (float_t)Threshold / 62.5f;
        wup_ths.wk_ths = (uint8_t)tmp;
      }
      else if (Threshold < (uint32_t)(250.0f * 63.0f))
      {
        interrupt_cfg.wake_ths_w = 0;
        tmp = (float_t)Threshold / 250.0f;
        wup_ths.wk_ths = (uint8_t)tmp;
      }
      else // Out of limit, we set it to the maximum possible value
      {
        interrupt_cfg.wake_ths_w = 0;
        wup_ths.wk_ths = 63;
      }
      break;

    default:
      ret = IIS2DULPX_ERROR;
      break;
  }

  if(ret != IIS2DULPX_ERROR)
  {
    if(iis2dulpx_write_reg(&(pObj->Ctx), IIS2DULPX_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != IIS2DULPX_OK)
    {
      return IIS2DULPX_ERROR;
    }

    if(iis2dulpx_write_reg(&(pObj->Ctx), IIS2DULPX_WAKE_UP_THS, (uint8_t *)&wup_ths, 1) != IIS2DULPX_OK)
    {
      return IIS2DULPX_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Set wake up duration
  * @param  pObj the device pObj
  * @param  Duration wake up detection duration
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_Set_Wake_Up_Duration(IIS2DULPX_Object_t *pObj, uint8_t Duration)
{
  iis2dulpx_wake_up_dur_t wup_dur;
  iis2dulpx_wake_up_dur_ext_t wup_dur_ext;

  /* Check if the duration is one of the possible values */
  if((Duration != 0U) && (Duration != 1U) && (Duration != 2U) && (Duration != 3U) && (Duration != 7U) && (Duration != 11U) && (Duration != 15U))
  {
    return IIS2DULPX_ERROR;
  }

  if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_WAKE_UP_DUR, (uint8_t *)&wup_dur, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_WAKE_UP_DUR_EXT, (uint8_t *)&wup_dur_ext, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  if ((Duration == 0U) || (Duration == 1U) || (Duration == 2U))
  {
    wup_dur_ext.wu_dur_extended = 0;
    wup_dur.wake_dur = Duration;
  }
  else
  {
    wup_dur_ext.wu_dur_extended = 1;
    if (Duration == 3U)
    {
      wup_dur.wake_dur = 0;
    }
    else if (Duration == 7U)
    {
      wup_dur.wake_dur = 1;
    }
    else if (Duration == 11U)
    {
      wup_dur.wake_dur = 2;
    }
    else // Duration = 15
    {
      wup_dur.wake_dur = 3;
    }
  }

  if (iis2dulpx_write_reg(&(pObj->Ctx), IIS2DULPX_WAKE_UP_DUR, (uint8_t *)&wup_dur, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  if (iis2dulpx_write_reg(&(pObj->Ctx), IIS2DULPX_WAKE_UP_DUR_EXT, (uint8_t *)&wup_dur_ext, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  return IIS2DULPX_OK;
}

/**
  * @brief  Enable 6d orientation
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_Enable_6D_Orientation(IIS2DULPX_Object_t *pObj, IIS2DULPX_SensorIntPin_t IntPin)
{
  int32_t ret = IIS2DULPX_OK;
  iis2dulpx_md1_cfg_t val1;
  iis2dulpx_md2_cfg_t val2;
  iis2dulpx_interrupt_cfg_t interrupt_cfg;

  /* Output Data Rate selection */
  if (IIS2DULPX_ACC_SetOutputDataRate(pObj, 400.0f) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  /* Full scale selection */
  if (IIS2DULPX_ACC_SetFullScale(pObj, 2) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  /* Threshold selection*/
  if (IIS2DULPX_ACC_Set_6D_Orientation_Threshold(pObj, 2) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  /* Enable 6D orientation event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case IIS2DULPX_INT1_PIN:
      if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_MD1_CFG, (uint8_t *)&val1, 1) != IIS2DULPX_OK)
      {
        return IIS2DULPX_ERROR;
      }

      val1.int1_6d = PROPERTY_ENABLE;

      if (iis2dulpx_write_reg(&(pObj->Ctx), IIS2DULPX_MD1_CFG, (uint8_t *)&val1, 1) != IIS2DULPX_OK)
      {
        return IIS2DULPX_ERROR;
      }

      if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != IIS2DULPX_OK)
      {
        return IIS2DULPX_ERROR;
      }

      interrupt_cfg.interrupts_enable = PROPERTY_ENABLE;

      if (iis2dulpx_write_reg(&(pObj->Ctx), IIS2DULPX_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != IIS2DULPX_OK)
      {
        return IIS2DULPX_ERROR;
      }
      break;

    case IIS2DULPX_INT2_PIN:
      if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_MD2_CFG, (uint8_t *)&val2, 1) != IIS2DULPX_OK)
      {
        return IIS2DULPX_ERROR;
      }

      val2.int2_6d = PROPERTY_ENABLE;

      if (iis2dulpx_write_reg(&(pObj->Ctx), IIS2DULPX_MD2_CFG, (uint8_t *)&val2, 1) != IIS2DULPX_OK)
      {
        return IIS2DULPX_ERROR;
      }

      if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != IIS2DULPX_OK)
      {
        return IIS2DULPX_ERROR;
      }

      interrupt_cfg.interrupts_enable = PROPERTY_ENABLE;

      if (iis2dulpx_write_reg(&(pObj->Ctx), IIS2DULPX_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != IIS2DULPX_OK)
      {
        return IIS2DULPX_ERROR;
      }
      break;

    default:
      ret = IIS2DULPX_ERROR;
      break;
  }

  return ret;

}


/**
  * @brief  Disable 6D orientation detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_Disable_6D_Orientation(IIS2DULPX_Object_t *pObj)
{
  iis2dulpx_md1_cfg_t val1;
  iis2dulpx_md2_cfg_t val2;

  /* Reset threshold */
  if (IIS2DULPX_ACC_Set_6D_Orientation_Threshold(pObj, 0) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  /* Disable 6D orientation event on both INT1 and INT2 pins */
  if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_MD1_CFG, (uint8_t *)&val1, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  val1.int1_6d = PROPERTY_DISABLE;

  if (iis2dulpx_write_reg(&(pObj->Ctx), IIS2DULPX_MD1_CFG, (uint8_t *)&val1, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_MD2_CFG, (uint8_t *)&val2, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  val2.int2_6d = PROPERTY_DISABLE;

  if (iis2dulpx_write_reg(&(pObj->Ctx), IIS2DULPX_MD2_CFG, (uint8_t *)&val2, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  return IIS2DULPX_OK;
}

/**
  * @brief  Set 6D orientation threshold
  * @param  pObj the device pObj
  * @param  Threshold free fall detection threshold
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_Set_6D_Orientation_Threshold(IIS2DULPX_Object_t *pObj, uint8_t Threshold)
{
  iis2dulpx_sixd_t sixd;

  if (Threshold > 3U)
  {
    return IIS2DULPX_ERROR;
  }

  if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_SIXD, (uint8_t *)&sixd, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  sixd.d6d_ths = Threshold;

  if (iis2dulpx_write_reg(&(pObj->Ctx), IIS2DULPX_SIXD, (uint8_t *)&sixd, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  return IIS2DULPX_OK;

}

/**
  * @brief  Get the status of XLow orientation
  * @param  pObj the device pObj
  * @param  XLow the status of XLow orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_Get_6D_Orientation_XL(IIS2DULPX_Object_t *pObj, uint8_t *XLow)
{
  iis2dulpx_sixd_src_t data;

  if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_SIXD_SRC, (uint8_t *)&data, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  *XLow = data.xl;

  return IIS2DULPX_OK;
}

/**
  * @brief  Get the status of XHigh orientation
  * @param  pObj the device pObj
  * @param  XHigh the status of XHigh orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_Get_6D_Orientation_XH(IIS2DULPX_Object_t *pObj, uint8_t *XHigh)
{
  iis2dulpx_sixd_src_t data;

  if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_SIXD_SRC, (uint8_t *)&data, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  *XHigh = data.xh;

  return IIS2DULPX_OK;
}

/**
  * @brief  Get the status of YLow orientation
  * @param  pObj the device pObj
  * @param  YLow the status of YLow orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_Get_6D_Orientation_YL(IIS2DULPX_Object_t *pObj, uint8_t *YLow)
{
  iis2dulpx_sixd_src_t data;

  if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_SIXD_SRC, (uint8_t *)&data, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  *YLow = data.yl;

  return IIS2DULPX_OK;
}

/**
  * @brief  Get the status of YHigh orientation
  * @param  pObj the device pObj
  * @param  YHigh the status of YHigh orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_Get_6D_Orientation_YH(IIS2DULPX_Object_t *pObj, uint8_t *YHigh)
{
  iis2dulpx_sixd_src_t data;

  if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_SIXD_SRC, (uint8_t *)&data, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  *YHigh = data.yh;

  return IIS2DULPX_OK;
}

/**
  * @brief  Get the status of ZLow orientation
  * @param  pObj the device pObj
  * @param  ZLow the status of ZLow orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_Get_6D_Orientation_ZL(IIS2DULPX_Object_t *pObj, uint8_t *ZLow)
{
  iis2dulpx_sixd_src_t data;

  if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_SIXD_SRC, (uint8_t *)&data, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  *ZLow = data.zl;

  return IIS2DULPX_OK;
}

/**
  * @brief  Get the status of ZHigh orientation
  * @param  pObj the device pObj
  * @param  ZHigh the status of ZHigh orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t IIS2DULPX_ACC_Get_6D_Orientation_ZH(IIS2DULPX_Object_t *pObj, uint8_t *ZHigh)
{
  iis2dulpx_sixd_src_t data;

  if (iis2dulpx_read_reg(&(pObj->Ctx), IIS2DULPX_SIXD_SRC, (uint8_t *)&data, 1) != IIS2DULPX_OK)
  {
    return IIS2DULPX_ERROR;
  }

  *ZHigh = data.zh;

  return IIS2DULPX_OK;
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
  IIS2DULPX_Object_t *pObj = (IIS2DULPX_Object_t *)Handle;

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
  IIS2DULPX_Object_t *pObj = (IIS2DULPX_Object_t *)Handle;

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
