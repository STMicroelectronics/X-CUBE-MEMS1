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
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x50;

          if (LIS2DUXS12_Write_Reg(pObj, LIS2DUXS12_CTRL1, data) != LIS2DUXS12_OK)
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
  * @brief  Disable I3C
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LIS2DUXS12_DisableI3C(LIS2DUXS12_Object_t *pObj)
{
  uint8_t val;

  /* Perform dummy read in order to disable I3C and enable I2C */
  (void)lis2duxs12_device_id_get(&(pObj->Ctx), &val);

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
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return LIS2DUXS12_OK;
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

    case LIS2DUXS12_1Hz5_ULP:
      *Odr = 1.5f;
      break;

    case LIS2DUXS12_3Hz_ULP:
      *Odr = 3.0f;
      break;

    case LIS2DUXS12_6Hz:
    case LIS2DUXS12_6Hz_HP:
      *Odr = 6.0f;
      break;

    case LIS2DUXS12_12Hz5:
    case LIS2DUXS12_12Hz5_HP:
      *Odr = 12.5f;
      break;

    case LIS2DUXS12_25Hz_ULP:
    case LIS2DUXS12_25Hz:
    case LIS2DUXS12_25Hz_HP:
      *Odr = 25.0f;
      break;

    case LIS2DUXS12_50Hz:
    case LIS2DUXS12_50Hz_HP:
      *Odr = 50.0f;
      break;

    case LIS2DUXS12_100Hz:
    case LIS2DUXS12_100Hz_HP:
      *Odr = 100.0f;
      break;

    case LIS2DUXS12_200Hz:
    case LIS2DUXS12_200Hz_HP:
      *Odr = 200.0f;
      break;

    case LIS2DUXS12_400Hz:
    case LIS2DUXS12_400Hz_HP:
      *Odr = 400.0f;
      break;

    case LIS2DUXS12_800Hz:
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
    mode.odr = (Odr <= 1.5f) ? LIS2DUXS12_1Hz5_ULP
               : (Odr <= 3.0f) ? LIS2DUXS12_3Hz_ULP
               :                 LIS2DUXS12_25Hz_ULP;
  }
  else if (Power == LIS2DUXS12_LOW_POWER)
  {
    mode.odr = (Odr <=   6.0f) ? LIS2DUXS12_6Hz
               : (Odr <=  12.5f) ? LIS2DUXS12_12Hz5
               : (Odr <=  25.0f) ? LIS2DUXS12_25Hz
               : (Odr <=  50.0f) ? LIS2DUXS12_50Hz
               : (Odr <= 100.0f) ? LIS2DUXS12_100Hz
               : (Odr <= 200.0f) ? LIS2DUXS12_200Hz
               : (Odr <= 400.0f) ? LIS2DUXS12_400Hz
               :                   LIS2DUXS12_800Hz;
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
  pObj->acc_odr = (mode.odr == LIS2DUXS12_1Hz5_ULP) ?   1.5f
                  : (mode.odr == LIS2DUXS12_3Hz_ULP)  ?   3.0f
                  : (mode.odr == LIS2DUXS12_6Hz)      ?   6.0f
                  : (mode.odr == LIS2DUXS12_6Hz_HP)   ?   6.0f
                  : (mode.odr == LIS2DUXS12_12Hz5)    ?  12.5f
                  : (mode.odr == LIS2DUXS12_12Hz5_HP) ?  12.5f
                  : (mode.odr == LIS2DUXS12_25Hz_ULP) ?  25.0f
                  : (mode.odr == LIS2DUXS12_25Hz)     ?  25.0f
                  : (mode.odr == LIS2DUXS12_25Hz_HP)  ?  25.0f
                  : (mode.odr == LIS2DUXS12_50Hz)     ?  50.0f
                  : (mode.odr == LIS2DUXS12_50Hz_HP)  ?  50.0f
                  : (mode.odr == LIS2DUXS12_100Hz)    ? 100.0f
                  : (mode.odr == LIS2DUXS12_100Hz_HP) ? 100.0f
                  : (mode.odr == LIS2DUXS12_200Hz)    ? 200.0f
                  : (mode.odr == LIS2DUXS12_200Hz_HP) ? 200.0f
                  : (mode.odr == LIS2DUXS12_400Hz)    ? 400.0f
                  : (mode.odr == LIS2DUXS12_400Hz_HP) ? 400.0f
                  : (mode.odr == LIS2DUXS12_800Hz)    ? 800.0f
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
