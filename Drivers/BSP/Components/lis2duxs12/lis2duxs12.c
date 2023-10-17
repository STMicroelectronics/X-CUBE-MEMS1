/**
  ******************************************************************************
  * @file    lis2duxs12.c
  * @author  MEMS Software Solutions Team
  * @brief   LIS2DUXS12 driver file
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
#include "lis2duxs12.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup LIS2DUXS12 LIS2DUXS12
  * @{
  */

/** @defgroup LIS2DUXS12_Exported_Variables LIS2DUXS12 Exported Variables
  * @{
  */

LIS2DUXS12_CommonDrv_t LIS2DUXS12_COMMON_Driver =
{
  LIS2DUXS12_Init,
  LIS2DUXS12_DeInit,
  LIS2DUXS12_ReadID,
  LIS2DUXS12_GetCapabilities,
};

LIS2DUXS12_ACC_Drv_t LIS2DUXS12_ACC_Driver =
{
  LIS2DUXS12_ACC_Enable,
  LIS2DUXS12_ACC_Disable,
  LIS2DUXS12_ACC_GetSensitivity,
  LIS2DUXS12_ACC_GetOutputDataRate,
  LIS2DUXS12_ACC_SetOutputDataRate,
  LIS2DUXS12_ACC_GetFullScale,
  LIS2DUXS12_ACC_SetFullScale,
  LIS2DUXS12_ACC_GetAxes,
  LIS2DUXS12_ACC_GetAxesRaw,
};

/**
  * @}
  */

/** @defgroup LIS2DUXS12_Private_Function_Prototypes LIS2DUXS12 Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t LIS2DUXS12_ACC_SetOutputDataRate_When_Enabled(LIS2DUXS12_Object_t *pObj, float_t Odr,
    LIS2DUXS12_Power_Mode_t Power);
static int32_t LIS2DUXS12_ACC_SetOutputDataRate_When_Disabled(LIS2DUXS12_Object_t *pObj, float_t Odr,
    LIS2DUXS12_Power_Mode_t Power);

/**
  * @}
  */

/** @defgroup LIS2DUXS12_Exported_Functions LIS2DUXS12 Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_RegisterBusIO(LIS2DUXS12_Object_t *pObj, LIS2DUXS12_IO_t *pIO)
{
  int32_t ret = LIS2DUXS12_OK;

  if (pObj == NULL)
  {
    ret = LIS2DUXS12_ERROR;
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
      ret = LIS2DUXS12_ERROR;
    }
    else if (pObj->IO.Init() != LIS2DUXS12_OK)
    {
      ret = LIS2DUXS12_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == LIS2DUXS12_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Exit from deep power down only the first time in SPI mode */
          if (LIS2DUXS12_ExitDeepPowerDownSPI(pObj) != LIS2DUXS12_OK)
          {
            ret = LIS2DUXS12_ERROR;
          }
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x50;

          if (LIS2DUXS12_Write_Reg(pObj, LIS2DUXS12_CTRL1, data) != LIS2DUXS12_OK)
          {
            ret = LIS2DUXS12_ERROR;
          }
        }
      }
      else if (pObj->IO.BusType == LIS2DUXS12_SPI_4WIRES_BUS)
      {
        /* Exit from deep power down only the first time in SPI mode */
        if (pObj->is_initialized == 0U)
        {
          if (LIS2DUXS12_ExitDeepPowerDownSPI(pObj) != LIS2DUXS12_OK)
          {
            ret = LIS2DUXS12_ERROR;
          }
        }
      }
      else if (pObj->IO.BusType == LIS2DUXS12_I2C_BUS)
      {
        /* Exit from deep power down only the first time in I2C mode */
        if (pObj->is_initialized == 0U)
        {
          if (LIS2DUXS12_ExitDeepPowerDownI2C(pObj) != LIS2DUXS12_OK)
          {
            ret = LIS2DUXS12_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  Initialize the LIS2DUXS12 sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_Init(LIS2DUXS12_Object_t *pObj)
{
  lis2duxs12_i3c_cfg_t val;

  if(pObj->IO.BusType != LIS2DUXS12_I3C_BUS)
  {
    /* Disable I3C */
    if (lis2duxs12_i3c_configure_get(&(pObj->Ctx), &val) != LIS2DUXS12_OK)
    {
      return LIS2DUXS12_ERROR;
    }
    val.asf_on = PROPERTY_ENABLE;
    if (lis2duxs12_i3c_configure_set(&(pObj->Ctx), &val) != LIS2DUXS12_OK)
    {
      return LIS2DUXS12_ERROR;
    }
  }

  /* Enable register address automatically incremented during a multiple byte
  access with a serial interface. Enable BDU. */
  if (lis2duxs12_init_set(&(pObj->Ctx), LIS2DUXS12_SENSOR_ONLY_ON) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  /* FIFO mode selection */
  lis2duxs12_fifo_mode_t fifo_mode =
  {
    .operation = LIS2DUXS12_BYPASS_MODE,
    .store     = LIS2DUXS12_FIFO_1X,
    .watermark = 0,
  };

  if (lis2duxs12_fifo_mode_set(&(pObj->Ctx), fifo_mode) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = 100.0f;
  /* Select default ultra low power (disabled). */
  pObj->power_mode = LIS2DUXS12_LOW_POWER;

  /* Output data rate: power down, full scale: 2g */
  lis2duxs12_md_t mode =
  {
    .odr = LIS2DUXS12_OFF,
    .fs  = LIS2DUXS12_2g,
  };

  if (lis2duxs12_mode_set(&(pObj->Ctx), &mode) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  pObj->is_initialized = 1;

  return LIS2DUXS12_OK;
}

/**
  * @brief  Deinitialize the LIS2DUXS12 sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_DeInit(LIS2DUXS12_Object_t *pObj)
{
  /* Disable the component */
  if (LIS2DUXS12_ACC_Disable(pObj) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  /* Reset output data rate. */
  pObj->acc_odr = 0.0f;
  /* Reset ultra low power to default value (disabled). */
  pObj->power_mode = LIS2DUXS12_LOW_POWER;

  pObj->is_initialized = 0;

  return LIS2DUXS12_OK;
}

/**
  * @brief  Exit from deep power down in I2C
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ExitDeepPowerDownI2C(LIS2DUXS12_Object_t *pObj)
{
  uint8_t val;

  /* Perform dummy read in order to exit from deep power down in I2C mode*/
  (void)lis2duxs12_device_id_get(&(pObj->Ctx), &val);

  /* Wait for 25 ms based on datasheet */
  pObj->Ctx.mdelay(25);

  return LIS2DUXS12_OK;
}

/**
  * @brief  Exit from deep power down in SPI
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ExitDeepPowerDownSPI(LIS2DUXS12_Object_t *pObj)
{
  /* Write IF_WAKE_UP register to exit from deep power down in SPI mode*/
  (void)lis2duxs12_exit_deep_power_down(&(pObj->Ctx));

  /* Wait for 25 ms based on datasheet */
  pObj->Ctx.mdelay(25);

  return LIS2DUXS12_OK;
}

/**
  * @brief  Read component ID
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ReadID(LIS2DUXS12_Object_t *pObj, uint8_t *Id)
{
  uint8_t val;

  if (lis2duxs12_device_id_get(&(pObj->Ctx), &val) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  *Id = val;

  return LIS2DUXS12_OK;
}

/**
  * @brief  Get LIS2DUXS12 sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to LIS2DUXS12 sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_GetCapabilities(LIS2DUXS12_Object_t *pObj, LIS2DUXS12_Capabilities_t *Capabilities)
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
  return LIS2DUXS12_OK;
}

/**
  * @brief  Enable the LIS2DUXS12 accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_Enable(LIS2DUXS12_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return LIS2DUXS12_OK;
  }

  /* Output data rate selection. */
  if (LIS2DUXS12_ACC_SetOutputDataRate_When_Enabled(pObj, pObj->acc_odr, pObj->power_mode) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return LIS2DUXS12_OK;
}

/**
  * @brief  Disable the LIS2DUXS12 accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_Disable(LIS2DUXS12_Object_t *pObj)
{
  float_t Odr;
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return LIS2DUXS12_OK;
  }

  if (LIS2DUXS12_ACC_GetOutputDataRate(pObj, &Odr) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  if (Odr == 800.0f)
  {
    if (LIS2DUXS12_ACC_SetOutputDataRate(pObj, 400.0f) != LIS2DUXS12_OK)
    {
      return LIS2DUXS12_ERROR;
    }

    /* Wait for 3 ms based on datasheet */
    pObj->Ctx.mdelay(3);
  }

  /* Output data rate selection - power down. */
  lis2duxs12_md_t mode;

  if (lis2duxs12_mode_get(&(pObj->Ctx), &mode) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  mode.odr = LIS2DUXS12_OFF;

  if (lis2duxs12_mode_set(&(pObj->Ctx), &mode) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return LIS2DUXS12_OK;
}

/**
  * @brief  Get the LIS2DUXS12 accelerometer sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_GetSensitivity(LIS2DUXS12_Object_t *pObj, float_t *Sensitivity)
{
  int32_t ret = LIS2DUXS12_OK;
  lis2duxs12_md_t mode;

  if (lis2duxs12_mode_get(&(pObj->Ctx), &mode) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  switch (mode.fs)
  {
    case LIS2DUXS12_2g:
      *Sensitivity = LIS2DUXS12_ACC_SENSITIVITY_FOR_FS_2G;
      break;

    case LIS2DUXS12_4g:
      *Sensitivity = LIS2DUXS12_ACC_SENSITIVITY_FOR_FS_4G;
      break;

    case LIS2DUXS12_8g:
      *Sensitivity = LIS2DUXS12_ACC_SENSITIVITY_FOR_FS_8G;
      break;

    case LIS2DUXS12_16g:
      *Sensitivity = LIS2DUXS12_ACC_SENSITIVITY_FOR_FS_16G;
      break;

    default:
      *Sensitivity = -1.0f;
      ret = LIS2DUXS12_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the LIS2DUXS12 accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_GetOutputDataRate(LIS2DUXS12_Object_t *pObj, float_t *Odr)
{
  int32_t ret = LIS2DUXS12_OK;
  lis2duxs12_md_t mode;

  /* Read actual output data rate from sensor. */
  if (lis2duxs12_mode_get(&(pObj->Ctx), &mode) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  switch (mode.odr)
  {
    case LIS2DUXS12_OFF:
    case LIS2DUXS12_TRIG_PIN:
    case LIS2DUXS12_TRIG_SW:
      *Odr = 0.0f;
      break;

    case LIS2DUXS12_1Hz6_ULP:
      *Odr = 1.6f;
      break;

    case LIS2DUXS12_3Hz_ULP:
      *Odr = 3.0f;
      break;

    case LIS2DUXS12_6Hz_LP:
    case LIS2DUXS12_6Hz_HP:
      *Odr = 6.0f;
      break;

    case LIS2DUXS12_12Hz5_LP:
    case LIS2DUXS12_12Hz5_HP:
      *Odr = 12.5f;
      break;

    case LIS2DUXS12_25Hz_ULP:
    case LIS2DUXS12_25Hz_LP:
    case LIS2DUXS12_25Hz_HP:
      *Odr = 25.0f;
      break;

    case LIS2DUXS12_50Hz_LP:
    case LIS2DUXS12_50Hz_HP:
      *Odr = 50.0f;
      break;

    case LIS2DUXS12_100Hz_LP:
    case LIS2DUXS12_100Hz_HP:
      *Odr = 100.0f;
      break;

    case LIS2DUXS12_200Hz_LP:
    case LIS2DUXS12_200Hz_HP:
      *Odr = 200.0f;
      break;

    case LIS2DUXS12_400Hz_LP:
    case LIS2DUXS12_400Hz_HP:
      *Odr = 400.0f;
      break;

    case LIS2DUXS12_800Hz_LP:
    case LIS2DUXS12_800Hz_HP:
      *Odr = 800.0f;
      break;

    default:
      *Odr = -1.0f;
      ret = LIS2DUXS12_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the LIS2DUXS12 accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_SetOutputDataRate(LIS2DUXS12_Object_t *pObj, float_t Odr)
{
  /* By default we use Ultra Low Power disabled */
  return LIS2DUXS12_ACC_SetOutputDataRate_With_Mode(pObj, Odr, LIS2DUXS12_LOW_POWER);
}

/**
  * @brief  Set the LIS2DUXS12 accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @param  Power the ultra low power option
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_SetOutputDataRate_With_Mode(LIS2DUXS12_Object_t *pObj, float_t Odr, LIS2DUXS12_Power_Mode_t Power)
{
  /* Check if the component is enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return LIS2DUXS12_ACC_SetOutputDataRate_When_Enabled(pObj, Odr, Power);
  }
  else
  {
    return LIS2DUXS12_ACC_SetOutputDataRate_When_Disabled(pObj, Odr, Power);
  }
}

/**
  * @brief  Get the LIS2DUXS12 accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_GetFullScale(LIS2DUXS12_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = LIS2DUXS12_OK;
  lis2duxs12_md_t mode;

  /* Read actual full scale selection from sensor. */
  if (lis2duxs12_mode_get(&(pObj->Ctx), &mode) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  switch (mode.fs)
  {
    case LIS2DUXS12_2g:
      *FullScale =  2;
      break;

    case LIS2DUXS12_4g:
      *FullScale =  4;
      break;

    case LIS2DUXS12_8g:
      *FullScale =  8;
      break;

    case LIS2DUXS12_16g:
      *FullScale = 16;
      break;

    default:
      *FullScale = -1;
      ret = LIS2DUXS12_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the LIS2DUXS12 accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_SetFullScale(LIS2DUXS12_Object_t *pObj, int32_t FullScale)
{
  lis2duxs12_md_t mode;

  if (lis2duxs12_mode_get(&(pObj->Ctx), &mode) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file statical analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  mode.fs = (FullScale <= 2) ? LIS2DUXS12_2g
            : (FullScale <= 4) ? LIS2DUXS12_4g
            : (FullScale <= 8) ? LIS2DUXS12_8g
            :                    LIS2DUXS12_16g;

  if (lis2duxs12_mode_set(&(pObj->Ctx), &mode) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  return LIS2DUXS12_OK;
}

/**
  * @brief  Get the LIS2DUXS12 accelerometer sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_GetAxesRaw(LIS2DUXS12_Object_t *pObj, LIS2DUXS12_AxesRaw_t *Value)
{
  lis2duxs12_md_t mode;
  lis2duxs12_xl_data_t data;

  if (lis2duxs12_mode_get(&(pObj->Ctx), &mode) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  if (lis2duxs12_xl_data_get(&(pObj->Ctx), &mode, &data) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  Value->x = data.raw[0];
  Value->y = data.raw[1];
  Value->z = data.raw[2];

  return LIS2DUXS12_OK;
}

/**
  * @brief  Get the LIS2DUXS12 accelerometer sensor axes
  * @param  pObj the device pObj
  * @param  Acceleration pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_GetAxes(LIS2DUXS12_Object_t *pObj, LIS2DUXS12_Axes_t *Acceleration)
{
  lis2duxs12_md_t mode;
  lis2duxs12_xl_data_t data;

  if (lis2duxs12_mode_get(&(pObj->Ctx), &mode) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  if (lis2duxs12_xl_data_get(&(pObj->Ctx), &mode, &data) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  Acceleration->x = (int32_t)data.mg[0];
  Acceleration->y = (int32_t)data.mg[1];
  Acceleration->z = (int32_t)data.mg[2];

  return LIS2DUXS12_OK;
}

/**
  * @brief  Get the LIS2DUXS12 register value
  * @param  pObj the device pObj
  * @param  Reg address to be read
  * @param  Data pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_Read_Reg(LIS2DUXS12_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (lis2duxs12_read_reg(&(pObj->Ctx), Reg, Data, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  return LIS2DUXS12_OK;
}

/**
  * @brief  Set the LIS2DUXS12 register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_Write_Reg(LIS2DUXS12_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (lis2duxs12_write_reg(&(pObj->Ctx), Reg, &Data, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  return LIS2DUXS12_OK;
}

/**
  * @brief  Get the LIS2DUXS12 ACC data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_Get_DRDY_Status(LIS2DUXS12_Object_t *pObj, uint8_t *Status)
{
  lis2duxs12_status_t device_status;

  if (lis2duxs12_status_get(&(pObj->Ctx), &device_status) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  *Status = device_status.drdy;

  return LIS2DUXS12_OK;
}

/**
  * @brief  Get the LIS2DUXS12 ACC initialization status
  * @param  pObj the device pObj
  * @param  Status 1 if initialized, 0 otherwise
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_Get_Init_Status(LIS2DUXS12_Object_t *pObj, uint8_t *Status)
{
  if (pObj == NULL)
  {
    return LIS2DUXS12_ERROR;
  }

  *Status = pObj->is_initialized;

  return LIS2DUXS12_OK;
}

/**
  * @}
  */

/** @defgroup LIS2DUXS12_Private_Functions LIS2DUXS12 Private Functions
  * @{
  */

/**
  * @brief  Set the LIS2DUXS12 accelerometer sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @param  Power the ultra low power option
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LIS2DUXS12_ACC_SetOutputDataRate_When_Enabled(LIS2DUXS12_Object_t *pObj, float_t Odr,
    LIS2DUXS12_Power_Mode_t Power)
{
  lis2duxs12_md_t mode;

  if (lis2duxs12_mode_get(&(pObj->Ctx), &mode) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  if (Power == LIS2DUXS12_ULTRA_LOW_POWER)
  {
    mode.odr = (Odr <= 1.6f) ? LIS2DUXS12_1Hz6_ULP
               : (Odr <= 3.0f) ? LIS2DUXS12_3Hz_ULP
               :                 LIS2DUXS12_25Hz_ULP;
  }
  else if (Power == LIS2DUXS12_LOW_POWER)
  {
    mode.odr = (Odr <=   6.0f) ? LIS2DUXS12_6Hz_LP
               : (Odr <=  12.5f) ? LIS2DUXS12_12Hz5_LP
               : (Odr <=  25.0f) ? LIS2DUXS12_25Hz_LP
               : (Odr <=  50.0f) ? LIS2DUXS12_50Hz_LP
               : (Odr <= 100.0f) ? LIS2DUXS12_100Hz_LP
               : (Odr <= 200.0f) ? LIS2DUXS12_200Hz_LP
               : (Odr <= 400.0f) ? LIS2DUXS12_400Hz_LP
               :                   LIS2DUXS12_800Hz_LP;
  }
  else if (Power == LIS2DUXS12_HIGH_PERFORMANCE)
  {
    mode.odr = (Odr <=   6.0f) ? LIS2DUXS12_6Hz_HP
               : (Odr <=  12.5f) ? LIS2DUXS12_12Hz5_HP
               : (Odr <=  25.0f) ? LIS2DUXS12_25Hz_HP
               : (Odr <=  50.0f) ? LIS2DUXS12_50Hz_HP
               : (Odr <= 100.0f) ? LIS2DUXS12_100Hz_HP
               : (Odr <= 200.0f) ? LIS2DUXS12_200Hz_HP
               : (Odr <= 400.0f) ? LIS2DUXS12_400Hz_HP
               :                   LIS2DUXS12_800Hz_HP;
  }
  else
  {
    /* Do nothing */
  }

  if (lis2duxs12_mode_set(&(pObj->Ctx), &mode) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  /* Store the current Odr value */
  pObj->acc_odr = (mode.odr == LIS2DUXS12_1Hz6_ULP) ?   1.6f
                  : (mode.odr == LIS2DUXS12_3Hz_ULP)  ?   3.0f
                  : (mode.odr == LIS2DUXS12_6Hz_LP)   ?   6.0f
                  : (mode.odr == LIS2DUXS12_6Hz_HP)   ?   6.0f
                  : (mode.odr == LIS2DUXS12_12Hz5_LP) ?  12.5f
                  : (mode.odr == LIS2DUXS12_12Hz5_HP) ?  12.5f
                  : (mode.odr == LIS2DUXS12_25Hz_ULP) ?  25.0f
                  : (mode.odr == LIS2DUXS12_25Hz_LP)  ?  25.0f
                  : (mode.odr == LIS2DUXS12_25Hz_HP)  ?  25.0f
                  : (mode.odr == LIS2DUXS12_50Hz_LP)  ?  50.0f
                  : (mode.odr == LIS2DUXS12_50Hz_HP)  ?  50.0f
                  : (mode.odr == LIS2DUXS12_100Hz_LP) ? 100.0f
                  : (mode.odr == LIS2DUXS12_100Hz_HP) ? 100.0f
                  : (mode.odr == LIS2DUXS12_200Hz_LP) ? 200.0f
                  : (mode.odr == LIS2DUXS12_200Hz_HP) ? 200.0f
                  : (mode.odr == LIS2DUXS12_400Hz_LP) ? 400.0f
                  : (mode.odr == LIS2DUXS12_400Hz_HP) ? 400.0f
                  : (mode.odr == LIS2DUXS12_800Hz_LP) ? 800.0f
                  : (mode.odr == LIS2DUXS12_800Hz_HP) ? 800.0f
                  :                                     -1.0f;

  if (pObj->acc_odr == -1.0f)
  {
    return LIS2DUXS12_ERROR;
  }

  /* Store the current Power value */
  pObj->power_mode = Power;

  return LIS2DUXS12_OK;
}

/**
  * @brief  Set the LIS2DUXS12 accelerometer sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @param  Power the ultra low power option
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LIS2DUXS12_ACC_SetOutputDataRate_When_Disabled(LIS2DUXS12_Object_t *pObj, float_t Odr,
    LIS2DUXS12_Power_Mode_t Power)
{
  /* Store the new Odr value */
  if (Power == LIS2DUXS12_ULTRA_LOW_POWER)
  {
    pObj->acc_odr = (Odr <= 1.5f) ? 1.5f
                    : (Odr <= 3.0f) ? 3.0f
                    :                25.0f;
  }
  else if ((Power == LIS2DUXS12_LOW_POWER) || (Power == LIS2DUXS12_HIGH_PERFORMANCE))
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

  return LIS2DUXS12_OK;
}

/**
  * @brief  Get the status of all hardware events
  * @param  pObj the device pObj
  * @param  Status the status of all hardware events
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_Get_Event_Status(LIS2DUXS12_Object_t *pObj, LIS2DUXS12_Event_Status_t *Status)
{
  lis2duxs12_all_int_src_t all_int_src_reg;
  lis2duxs12_md1_cfg_t md1_cfg_reg;
  lis2duxs12_md2_cfg_t md2_cfg_reg;

  (void)memset((void *)Status, 0x0, sizeof(LIS2DUXS12_Event_Status_t));

  if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_ALL_INT_SRC, (uint8_t *)&all_int_src_reg, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_MD1_CFG, (uint8_t *)&md1_cfg_reg, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_MD2_CFG, (uint8_t *)&md2_cfg_reg, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
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

  return LIS2DUXS12_OK;
}

/**
  * @brief  Enable wake up detection
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_Enable_Wake_Up_Detection(LIS2DUXS12_Object_t *pObj, LIS2DUXS12_SensorIntPin_t IntPin)
{
  int32_t ret = LIS2DUXS12_OK;
  lis2duxs12_md1_cfg_t val1;
  lis2duxs12_md2_cfg_t val2;
  lis2duxs12_interrupt_cfg_t interrupt_cfg;
  lis2duxs12_ctrl1_t ctrl1;

  /* Output Data Rate selection */
  if (LIS2DUXS12_ACC_SetOutputDataRate(pObj, 200.0f) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  /* Full scale selection */
  if (LIS2DUXS12_ACC_SetFullScale(pObj, 2) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  /* Set wake-up threshold */
  if (LIS2DUXS12_ACC_Set_Wake_Up_Threshold(pObj, 63) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  /* Set wake-up duration */
  if (LIS2DUXS12_ACC_Set_Wake_Up_Duration(pObj, 0) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  /* Enable wake-up event on the 3-axis */
  if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_CTRL1, (uint8_t *)&ctrl1, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  ctrl1.wu_x_en = PROPERTY_ENABLE;
  ctrl1.wu_y_en = PROPERTY_ENABLE;
  ctrl1.wu_z_en = PROPERTY_ENABLE;

  if (lis2duxs12_write_reg(&(pObj->Ctx), LIS2DUXS12_CTRL1, (uint8_t *)&ctrl1, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  /* Enable wake up event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case LIS2DUXS12_INT1_PIN:
      if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_MD1_CFG, (uint8_t *)&val1, 1) != LIS2DUXS12_OK)
      {
        return LIS2DUXS12_ERROR;
      }

      val1.int1_wu = PROPERTY_ENABLE;

      if (lis2duxs12_write_reg(&(pObj->Ctx), LIS2DUXS12_MD1_CFG, (uint8_t *)&val1, 1) != LIS2DUXS12_OK)
      {
        return LIS2DUXS12_ERROR;
      }

      if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != LIS2DUXS12_OK)
      {
        return LIS2DUXS12_ERROR;
      }

      interrupt_cfg.interrupts_enable = PROPERTY_ENABLE;

      if (lis2duxs12_write_reg(&(pObj->Ctx), LIS2DUXS12_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != LIS2DUXS12_OK)
      {
        return LIS2DUXS12_ERROR;
      }
      break;

    case LIS2DUXS12_INT2_PIN:
      if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_MD2_CFG, (uint8_t *)&val2, 1) != LIS2DUXS12_OK)
      {
        return LIS2DUXS12_ERROR;
      }

      val2.int2_wu = PROPERTY_ENABLE;

      if (lis2duxs12_write_reg(&(pObj->Ctx), LIS2DUXS12_MD2_CFG, (uint8_t *)&val2, 1) != LIS2DUXS12_OK)
      {
        return LIS2DUXS12_ERROR;
      }

      if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != LIS2DUXS12_OK)
      {
        return LIS2DUXS12_ERROR;
      }

      interrupt_cfg.interrupts_enable = PROPERTY_ENABLE;

      if (lis2duxs12_write_reg(&(pObj->Ctx), LIS2DUXS12_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != LIS2DUXS12_OK)
      {
        return LIS2DUXS12_ERROR;
      }
      break;

    default:
      ret = LIS2DUXS12_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Disable wake up detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_Disable_Wake_Up_Detection(LIS2DUXS12_Object_t *pObj)
{
  lis2duxs12_md1_cfg_t val1;
  lis2duxs12_md2_cfg_t val2;
  lis2duxs12_ctrl1_t ctrl1;

  /* Disable wake up event on both INT1 and INT2 pins */
  if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_MD1_CFG, (uint8_t *)&val1, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  val1.int1_wu = PROPERTY_DISABLE;

  if (lis2duxs12_write_reg(&(pObj->Ctx), LIS2DUXS12_MD1_CFG, (uint8_t *)&val1, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_MD2_CFG, (uint8_t *)&val2, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  val2.int2_wu = PROPERTY_DISABLE;

  if (lis2duxs12_write_reg(&(pObj->Ctx), LIS2DUXS12_MD2_CFG, (uint8_t *)&val2, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  /* Disable wake-up event on the 3-axis */
  if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_CTRL1, (uint8_t *)&ctrl1, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  ctrl1.wu_x_en = PROPERTY_DISABLE;
  ctrl1.wu_y_en = PROPERTY_DISABLE;
  ctrl1.wu_z_en = PROPERTY_DISABLE;

  if (lis2duxs12_write_reg(&(pObj->Ctx), LIS2DUXS12_CTRL1, (uint8_t *)&ctrl1, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  /* Reset wake-up threshold */
  if (LIS2DUXS12_ACC_Set_Wake_Up_Threshold(pObj, 0) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  /* Reset wake-up duration */
  if (LIS2DUXS12_ACC_Set_Wake_Up_Duration(pObj, 0) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  return LIS2DUXS12_OK;
}

/**
  * @brief  Set wake up threshold
  * @param  pObj the device pObj
  * @param  Threshold wake up detection threshold
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_Set_Wake_Up_Threshold(LIS2DUXS12_Object_t *pObj, uint32_t Threshold)
{
  int32_t fs;
  int32_t ret = LIS2DUXS12_OK;
  float_t tmp;
  lis2duxs12_interrupt_cfg_t interrupt_cfg;
  lis2duxs12_wake_up_ths_t wup_ths;

  if (LIS2DUXS12_ACC_GetFullScale(pObj, &fs) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  if(lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  if(lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_WAKE_UP_THS, (uint8_t *)&wup_ths, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
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
      ret = LIS2DUXS12_ERROR;
      break;
  }

  if(ret != LIS2DUXS12_ERROR)
  {
    if(lis2duxs12_write_reg(&(pObj->Ctx), LIS2DUXS12_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != LIS2DUXS12_OK)
    {
      return LIS2DUXS12_ERROR;
    }

    if(lis2duxs12_write_reg(&(pObj->Ctx), LIS2DUXS12_WAKE_UP_THS, (uint8_t *)&wup_ths, 1) != LIS2DUXS12_OK)
    {
      return LIS2DUXS12_ERROR;
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
int32_t LIS2DUXS12_ACC_Set_Wake_Up_Duration(LIS2DUXS12_Object_t *pObj, uint8_t Duration)
{
  lis2duxs12_wake_up_dur_t wup_dur;
  lis2duxs12_wake_up_dur_ext_t wup_dur_ext;

  /* Check if the duration is one of the possible values */
  if(Duration != 0 && Duration != 1 && Duration != 2 && Duration != 3 && Duration != 7 && Duration != 11 && Duration != 15)
  {
    return LIS2DUXS12_ERROR;
  }

  if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_WAKE_UP_DUR, (uint8_t *)&wup_dur, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_WAKE_UP_DUR_EXT, (uint8_t *)&wup_dur_ext, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  if (Duration == 0 || Duration == 1 || Duration == 2)
  {
    wup_dur_ext.wu_dur_extended = 0;
    wup_dur.wake_dur = Duration;
  }
  else
  {
    wup_dur_ext.wu_dur_extended = 1;
    if (Duration == 3)
    {
      wup_dur.wake_dur = 0;
    }
    else if (Duration == 7)
    {
      wup_dur.wake_dur = 1;
    }
    else if (Duration == 11)
    {
      wup_dur.wake_dur = 2;
    }
    else // Duration = 15
    {
      wup_dur.wake_dur = 3;
    }
  }

  if (lis2duxs12_write_reg(&(pObj->Ctx), LIS2DUXS12_WAKE_UP_DUR, (uint8_t *)&wup_dur, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  if (lis2duxs12_write_reg(&(pObj->Ctx), LIS2DUXS12_WAKE_UP_DUR_EXT, (uint8_t *)&wup_dur_ext, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  return LIS2DUXS12_OK;
}

/**
  * @brief  Enable 6d orientation
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_Enable_6D_Orientation(LIS2DUXS12_Object_t *pObj, LIS2DUXS12_SensorIntPin_t IntPin)
{
  int32_t ret = LIS2DUXS12_OK;
  lis2duxs12_md1_cfg_t val1;
  lis2duxs12_md2_cfg_t val2;
  lis2duxs12_interrupt_cfg_t interrupt_cfg;

  /* Output Data Rate selection */
  if (LIS2DUXS12_ACC_SetOutputDataRate(pObj, 400.0f) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  /* Full scale selection */
  if (LIS2DUXS12_ACC_SetFullScale(pObj, 2) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  /* Threshold selection*/
  if (LIS2DUXS12_ACC_Set_6D_Orientation_Threshold(pObj, 2) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  /* Enable 6D orientation event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case LIS2DUXS12_INT1_PIN:
      if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_MD1_CFG, (uint8_t *)&val1, 1) != LIS2DUXS12_OK)
      {
        return LIS2DUXS12_ERROR;
      }

      val1.int1_6d = PROPERTY_ENABLE;

      if (lis2duxs12_write_reg(&(pObj->Ctx), LIS2DUXS12_MD1_CFG, (uint8_t *)&val1, 1) != LIS2DUXS12_OK)
      {
        return LIS2DUXS12_ERROR;
      }

      if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != LIS2DUXS12_OK)
      {
        return LIS2DUXS12_ERROR;
      }

      interrupt_cfg.interrupts_enable = PROPERTY_ENABLE;

      if (lis2duxs12_write_reg(&(pObj->Ctx), LIS2DUXS12_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != LIS2DUXS12_OK)
      {
        return LIS2DUXS12_ERROR;
      }
      break;

    case LIS2DUXS12_INT2_PIN:
      if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_MD2_CFG, (uint8_t *)&val2, 1) != LIS2DUXS12_OK)
      {
        return LIS2DUXS12_ERROR;
      }

      val2.int2_6d = PROPERTY_ENABLE;

      if (lis2duxs12_write_reg(&(pObj->Ctx), LIS2DUXS12_MD2_CFG, (uint8_t *)&val2, 1) != LIS2DUXS12_OK)
      {
        return LIS2DUXS12_ERROR;
      }

      if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != LIS2DUXS12_OK)
      {
        return LIS2DUXS12_ERROR;
      }

      interrupt_cfg.interrupts_enable = PROPERTY_ENABLE;

      if (lis2duxs12_write_reg(&(pObj->Ctx), LIS2DUXS12_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != LIS2DUXS12_OK)
      {
        return LIS2DUXS12_ERROR;
      }
      break;

    default:
      ret = LIS2DUXS12_ERROR;
      break;
  }

  return ret;

}


/**
  * @brief  Disable 6D orientation detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_Disable_6D_Orientation(LIS2DUXS12_Object_t *pObj)
{
  lis2duxs12_md1_cfg_t val1;
  lis2duxs12_md2_cfg_t val2;

  /* Reset threshold */
  if (LIS2DUXS12_ACC_Set_6D_Orientation_Threshold(pObj, 0) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  /* Disable 6D orientation event on both INT1 and INT2 pins */
  if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_MD1_CFG, (uint8_t *)&val1, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  val1.int1_6d = PROPERTY_DISABLE;

  if (lis2duxs12_write_reg(&(pObj->Ctx), LIS2DUXS12_MD1_CFG, (uint8_t *)&val1, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_MD2_CFG, (uint8_t *)&val2, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  val2.int2_6d = PROPERTY_DISABLE;

  if (lis2duxs12_write_reg(&(pObj->Ctx), LIS2DUXS12_MD2_CFG, (uint8_t *)&val2, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  return LIS2DUXS12_OK;
}

/**
  * @brief  Set 6D orientation threshold
  * @param  pObj the device pObj
  * @param  Threshold free fall detection threshold
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_Set_6D_Orientation_Threshold(LIS2DUXS12_Object_t *pObj, uint8_t Threshold)
{
  lis2duxs12_sixd_t sixd;

  if (Threshold > 3)
  {
    return LIS2DUXS12_ERROR;
  }

  if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_SIXD, (uint8_t *)&sixd, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  sixd.d6d_ths = Threshold;

  if (lis2duxs12_write_reg(&(pObj->Ctx), LIS2DUXS12_SIXD, (uint8_t *)&sixd, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  return LIS2DUXS12_OK;

}

/**
  * @brief  Get the status of XLow orientation
  * @param  pObj the device pObj
  * @param  XLow the status of XLow orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_Get_6D_Orientation_XL(LIS2DUXS12_Object_t *pObj, uint8_t *XLow)
{
  lis2duxs12_sixd_src_t data;

  if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_SIXD_SRC, (uint8_t *)&data, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  *XLow = data.xl;

  return LIS2DUXS12_OK;
}

/**
  * @brief  Get the status of XHigh orientation
  * @param  pObj the device pObj
  * @param  XHigh the status of XHigh orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_Get_6D_Orientation_XH(LIS2DUXS12_Object_t *pObj, uint8_t *XHigh)
{
  lis2duxs12_sixd_src_t data;

  if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_SIXD_SRC, (uint8_t *)&data, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  *XHigh = data.xh;

  return LIS2DUXS12_OK;
}

/**
  * @brief  Get the status of YLow orientation
  * @param  pObj the device pObj
  * @param  YLow the status of YLow orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_Get_6D_Orientation_YL(LIS2DUXS12_Object_t *pObj, uint8_t *YLow)
{
  lis2duxs12_sixd_src_t data;

  if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_SIXD_SRC, (uint8_t *)&data, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  *YLow = data.yl;

  return LIS2DUXS12_OK;
}

/**
  * @brief  Get the status of YHigh orientation
  * @param  pObj the device pObj
  * @param  YHigh the status of YHigh orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_Get_6D_Orientation_YH(LIS2DUXS12_Object_t *pObj, uint8_t *YHigh)
{
  lis2duxs12_sixd_src_t data;

  if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_SIXD_SRC, (uint8_t *)&data, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  *YHigh = data.yh;

  return LIS2DUXS12_OK;
}

/**
  * @brief  Get the status of ZLow orientation
  * @param  pObj the device pObj
  * @param  ZLow the status of ZLow orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_Get_6D_Orientation_ZL(LIS2DUXS12_Object_t *pObj, uint8_t *ZLow)
{
  lis2duxs12_sixd_src_t data;

  if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_SIXD_SRC, (uint8_t *)&data, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  *ZLow = data.zl;

  return LIS2DUXS12_OK;
}

/**
  * @brief  Get the status of ZHigh orientation
  * @param  pObj the device pObj
  * @param  ZHigh the status of ZHigh orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_ACC_Get_6D_Orientation_ZH(LIS2DUXS12_Object_t *pObj, uint8_t *ZHigh)
{
  lis2duxs12_sixd_src_t data;

  if (lis2duxs12_read_reg(&(pObj->Ctx), LIS2DUXS12_SIXD_SRC, (uint8_t *)&data, 1) != LIS2DUXS12_OK)
  {
    return LIS2DUXS12_ERROR;
  }

  *ZHigh = data.zh;

  return LIS2DUXS12_OK;
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
  LIS2DUXS12_Object_t *pObj = (LIS2DUXS12_Object_t *)Handle;

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
  LIS2DUXS12_Object_t *pObj = (LIS2DUXS12_Object_t *)Handle;

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
