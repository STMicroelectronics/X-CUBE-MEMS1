/**
  ******************************************************************************
  * @file    st1vafe3bx.c
  * @author  MEMS Software Solutions Team
  * @brief   ST1VAFE3BX driver file
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
#include "st1vafe3bx.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup ST1VAFE3BX ST1VAFE3BX
  * @{
  */

/** @defgroup ST1VAFE3BX_Exported_Variables ST1VAFE3BX Exported Variables
  * @{
  */

ST1VAFE3BX_CommonDrv_t ST1VAFE3BX_COMMON_Driver =
{
  ST1VAFE3BX_Init,
  ST1VAFE3BX_DeInit,
  ST1VAFE3BX_ReadID,
  ST1VAFE3BX_GetCapabilities,
};

ST1VAFE3BX_ACC_Drv_t ST1VAFE3BX_ACC_Driver =
{
  ST1VAFE3BX_ACC_Enable,
  ST1VAFE3BX_ACC_Disable,
  ST1VAFE3BX_ACC_GetSensitivity,
  ST1VAFE3BX_ACC_GetOutputDataRate,
  ST1VAFE3BX_ACC_SetOutputDataRate,
  ST1VAFE3BX_ACC_GetFullScale,
  ST1VAFE3BX_ACC_SetFullScale,
  ST1VAFE3BX_ACC_GetAxes,
  ST1VAFE3BX_ACC_GetAxesRaw,
};

/**
  * @}
  */

/** @defgroup ST1VAFE3BX_Private_Function_Prototypes ST1VAFE3BX Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t ST1VAFE3BX_ACC_SetOutputDataRate_When_Enabled(ST1VAFE3BX_Object_t *pObj, float_t Odr,
    ST1VAFE3BX_Power_Mode_t Power);
static int32_t ST1VAFE3BX_ACC_SetOutputDataRate_When_Disabled(ST1VAFE3BX_Object_t *pObj, float_t Odr,
    ST1VAFE3BX_Power_Mode_t Power);

/**
  * @}
  */

/** @defgroup ST1VAFE3BX_Exported_Functions ST1VAFE3BX Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_RegisterBusIO(ST1VAFE3BX_Object_t *pObj, ST1VAFE3BX_IO_t *pIO)
{
  int32_t ret = ST1VAFE3BX_OK;

  if (pObj == NULL)
  {
    ret = ST1VAFE3BX_ERROR;
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
      ret = ST1VAFE3BX_ERROR;
    }
    else if (pObj->IO.Init() != ST1VAFE3BX_OK)
    {
      ret = ST1VAFE3BX_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == ST1VAFE3BX_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Exit from deep power down only the first time in SPI mode */
          if (ST1VAFE3BX_ExitDeepPowerDownSPI(pObj) != ST1VAFE3BX_OK)
          {
            ret = ST1VAFE3BX_ERROR;
          }
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x50;

          if (ST1VAFE3BX_Write_Reg(pObj, ST1VAFE3BX_CTRL1, data) != ST1VAFE3BX_OK)
          {
            ret = ST1VAFE3BX_ERROR;
          }
        }
      }
      else if (pObj->IO.BusType == ST1VAFE3BX_SPI_4WIRES_BUS)
      {
        /* Exit from deep power down only the first time in SPI mode */
        if (pObj->is_initialized == 0U)
        {
          if (ST1VAFE3BX_ExitDeepPowerDownSPI(pObj) != ST1VAFE3BX_OK)
          {
            ret = ST1VAFE3BX_ERROR;
          }
        }
      }
      else if (pObj->IO.BusType == ST1VAFE3BX_I2C_BUS)
      {
        /* Exit from deep power down only the first time in I2C mode */
        if (pObj->is_initialized == 0U)
        {
          if (ST1VAFE3BX_ExitDeepPowerDownI2C(pObj) != ST1VAFE3BX_OK)
          {
            ret = ST1VAFE3BX_ERROR;
          }
        }
      }
      else
      {
        ret = ST1VAFE3BX_ERROR;
      }
    }
  }

  return ret;
}

/**
  * @brief  Initialize the ST1VAFE3BX sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_Init(ST1VAFE3BX_Object_t *pObj)
{
  st1vafe3bx_i3c_cfg_t val;

  if(pObj->IO.BusType != ST1VAFE3BX_I3C_BUS)
  {
    /* Disable I3C */
    if (st1vafe3bx_i3c_configure_get(&(pObj->Ctx), &val) != ST1VAFE3BX_OK)
    {
      return ST1VAFE3BX_ERROR;
    }
    val.asf_on = PROPERTY_ENABLE;
    if (st1vafe3bx_i3c_configure_set(&(pObj->Ctx), &val) != ST1VAFE3BX_OK)
    {
      return ST1VAFE3BX_ERROR;
    }
  }

  /* Set main memory bank */
  if (ST1VAFE3BX_Set_Mem_Bank(pObj, (uint8_t)ST1VAFE3BX_MAIN_MEM_BANK) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }


  /* Enable register address automatically incremented during a multiple byte
  access with a serial interface. Enable BDU. */
  if (st1vafe3bx_init_set(&(pObj->Ctx), ST1VAFE3BX_SENSOR_ONLY_ON) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  /* FIFO mode selection */
  st1vafe3bx_fifo_mode_t fifo_mode =
  {
    .operation = ST1VAFE3BX_BYPASS_MODE,
    .store     = ST1VAFE3BX_FIFO_1X,
    .watermark = 0,
  };

  if (st1vafe3bx_fifo_mode_set(&(pObj->Ctx), fifo_mode) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = 100.0f;
  /* Select default ultra low power (disabled). */
  pObj->power_mode = ST1VAFE3BX_LOW_POWER;

  /* Output data rate: power down, full scale: 2g */
  st1vafe3bx_md_t mode =
  {
    .odr = ST1VAFE3BX_OFF,
    .fs  = ST1VAFE3BX_2g,
  };

  if (st1vafe3bx_mode_set(&(pObj->Ctx), &mode) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  pObj->is_initialized = 1;

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Deinitialize the ST1VAFE3BX sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_DeInit(ST1VAFE3BX_Object_t *pObj)
{
  /* Disable the component */
  if (ST1VAFE3BX_ACC_Disable(pObj) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  /* Reset output data rate. */
  pObj->acc_odr = 0.0f;
  /* Reset ultra low power to default value (disabled). */
  pObj->power_mode = ST1VAFE3BX_LOW_POWER;

  pObj->is_initialized = 0;

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Exit from deep power down in I2C
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ExitDeepPowerDownI2C(ST1VAFE3BX_Object_t *pObj)
{
  uint8_t val;

  /* Perform dummy read in order to exit from deep power down in I2C mode*/
  if (st1vafe3bx_device_id_get(&(pObj->Ctx), &val) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  /* Wait for 25 ms based on datasheet */
  pObj->Ctx.mdelay(25);

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Exit from deep power down in SPI
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ExitDeepPowerDownSPI(ST1VAFE3BX_Object_t *pObj)
{
  /* Write IF_WAKE_UP register to exit from deep power down in SPI mode*/
  if (st1vafe3bx_exit_deep_power_down(&(pObj->Ctx)) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  /* Wait for 25 ms based on datasheet */
  pObj->Ctx.mdelay(25);

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Read component ID
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ReadID(ST1VAFE3BX_Object_t *pObj, uint8_t *Id)
{
  uint8_t val;

  if (st1vafe3bx_device_id_get(&(pObj->Ctx), &val) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  *Id = val;

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Get ST1VAFE3BX sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to ST1VAFE3BX sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_GetCapabilities(ST1VAFE3BX_Object_t *pObj, ST1VAFE3BX_Capabilities_t *Capabilities)
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
  return ST1VAFE3BX_OK;
}

/**
  * @brief  Enable the ST1VAFE3BX accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_Enable(ST1VAFE3BX_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return ST1VAFE3BX_OK;
  }

  /* Output data rate selection. */
  if (ST1VAFE3BX_ACC_SetOutputDataRate_When_Enabled(pObj, pObj->acc_odr, pObj->power_mode) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Disable the ST1VAFE3BX accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_Disable(ST1VAFE3BX_Object_t *pObj)
{
  float_t Odr;
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return ST1VAFE3BX_OK;
  }

  if (ST1VAFE3BX_ACC_GetOutputDataRate(pObj, &Odr) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  if (Odr == 800.0f)
  {
    if (ST1VAFE3BX_ACC_SetOutputDataRate(pObj, 400.0f) != ST1VAFE3BX_OK)
    {
      return ST1VAFE3BX_ERROR;
    }

    /* Wait for 3 ms based on datasheet */
    pObj->Ctx.mdelay(3);
  }

  /* Output data rate selection - power down. */
  st1vafe3bx_md_t mode;

  if (st1vafe3bx_mode_get(&(pObj->Ctx), &mode) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  mode.odr = ST1VAFE3BX_OFF;

  if (st1vafe3bx_mode_set(&(pObj->Ctx), &mode) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Get the ST1VAFE3BX accelerometer sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_GetSensitivity(ST1VAFE3BX_Object_t *pObj, float_t *Sensitivity)
{
  int32_t ret = ST1VAFE3BX_OK;
  st1vafe3bx_md_t mode;

  if (st1vafe3bx_mode_get(&(pObj->Ctx), &mode) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  switch (mode.fs)
  {
    case ST1VAFE3BX_2g:
      *Sensitivity = ST1VAFE3BX_ACC_SENSITIVITY_FOR_FS_2G;
      break;

    case ST1VAFE3BX_4g:
      *Sensitivity = ST1VAFE3BX_ACC_SENSITIVITY_FOR_FS_4G;
      break;

    case ST1VAFE3BX_8g:
      *Sensitivity = ST1VAFE3BX_ACC_SENSITIVITY_FOR_FS_8G;
      break;

    case ST1VAFE3BX_16g:
      *Sensitivity = ST1VAFE3BX_ACC_SENSITIVITY_FOR_FS_16G;
      break;

    default:
      *Sensitivity = -1.0f;
      ret = ST1VAFE3BX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the ST1VAFE3BX accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_GetOutputDataRate(ST1VAFE3BX_Object_t *pObj, float_t *Odr)
{
  int32_t ret = ST1VAFE3BX_OK;
  st1vafe3bx_md_t mode;

  /* Read actual output data rate from sensor. */
  if (st1vafe3bx_mode_get(&(pObj->Ctx), &mode) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  switch (mode.odr)
  {
    case ST1VAFE3BX_OFF:
    case ST1VAFE3BX_TRIG_PIN:
    case ST1VAFE3BX_TRIG_SW:
      *Odr = 0.0f;
      break;

    case ST1VAFE3BX_1Hz6_ULP:
      *Odr = 1.6f;
      break;

    case ST1VAFE3BX_3Hz_ULP:
      *Odr = 3.0f;
      break;

    case ST1VAFE3BX_6Hz_LP:
    case ST1VAFE3BX_6Hz_HP:
      *Odr = 6.0f;
      break;

    case ST1VAFE3BX_12Hz5_LP:
    case ST1VAFE3BX_12Hz5_HP:
      *Odr = 12.5f;
      break;

    case ST1VAFE3BX_25Hz_ULP:
    case ST1VAFE3BX_25Hz_LP:
    case ST1VAFE3BX_25Hz_HP:
      *Odr = 25.0f;
      break;

    case ST1VAFE3BX_50Hz_LP:
    case ST1VAFE3BX_50Hz_HP:
      *Odr = 50.0f;
      break;

    case ST1VAFE3BX_100Hz_LP:
    case ST1VAFE3BX_100Hz_HP:
      *Odr = 100.0f;
      break;

    case ST1VAFE3BX_200Hz_LP:
    case ST1VAFE3BX_200Hz_HP:
      *Odr = 200.0f;
      break;

    case ST1VAFE3BX_400Hz_LP:
    case ST1VAFE3BX_400Hz_HP:
      *Odr = 400.0f;
      break;

    case ST1VAFE3BX_800Hz_LP:
    case ST1VAFE3BX_800Hz_HP:
      *Odr = 800.0f;
      break;

    case ST1VAFE3BX_3200Hz_VAFE_LP:
      *Odr = 3200.0f;
      break;

    case ST1VAFE3BX_800Hz_VAFE_HP:
      *Odr = 800.0f;
      break;

    default:
      *Odr = -1.0f;
      ret = ST1VAFE3BX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the ST1VAFE3BX accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_SetOutputDataRate(ST1VAFE3BX_Object_t *pObj, float_t Odr)
{
  /* By default we use Ultra Low Power disabled */
  return ST1VAFE3BX_ACC_SetOutputDataRate_With_Mode(pObj, Odr, ST1VAFE3BX_LOW_POWER);
}

/**
  * @brief  Set the ST1VAFE3BX accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @param  Power the ultra low power option
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_SetOutputDataRate_With_Mode(ST1VAFE3BX_Object_t *pObj, float_t Odr, ST1VAFE3BX_Power_Mode_t Power)
{
  /* Check if the component is enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return ST1VAFE3BX_ACC_SetOutputDataRate_When_Enabled(pObj, Odr, Power);
  }
  else
  {
    return ST1VAFE3BX_ACC_SetOutputDataRate_When_Disabled(pObj, Odr, Power);
  }
}

/**
  * @brief  Get the ST1VAFE3BX accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_GetFullScale(ST1VAFE3BX_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = ST1VAFE3BX_OK;
  st1vafe3bx_md_t mode;

  /* Read actual full scale selection from sensor. */
  if (st1vafe3bx_mode_get(&(pObj->Ctx), &mode) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  switch (mode.fs)
  {
    case ST1VAFE3BX_2g:
      *FullScale =  2;
      break;

    case ST1VAFE3BX_4g:
      *FullScale =  4;
      break;

    case ST1VAFE3BX_8g:
      *FullScale =  8;
      break;

    case ST1VAFE3BX_16g:
      *FullScale = 16;
      break;

    default:
      *FullScale = -1;
      ret = ST1VAFE3BX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the ST1VAFE3BX accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_SetFullScale(ST1VAFE3BX_Object_t *pObj, int32_t FullScale)
{
  st1vafe3bx_md_t mode;

  if (st1vafe3bx_mode_get(&(pObj->Ctx), &mode) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file statical analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  mode.fs = (FullScale <= 2) ? ST1VAFE3BX_2g
            : (FullScale <= 4) ? ST1VAFE3BX_4g
            : (FullScale <= 8) ? ST1VAFE3BX_8g
            :                    ST1VAFE3BX_16g;

  if (st1vafe3bx_mode_set(&(pObj->Ctx), &mode) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Get the ST1VAFE3BX accelerometer sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_GetAxesRaw(ST1VAFE3BX_Object_t *pObj, ST1VAFE3BX_AxesRaw_t *Value)
{
  st1vafe3bx_md_t mode;
  st1vafe3bx_xl_data_t data;

  if (st1vafe3bx_mode_get(&(pObj->Ctx), &mode) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  if (st1vafe3bx_xl_data_get(&(pObj->Ctx), &mode, &data) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  Value->x = data.raw[0];
  Value->y = data.raw[1];
  Value->z = data.raw[2];

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Get the ST1VAFE3BX accelerometer sensor axes
  * @param  pObj the device pObj
  * @param  Acceleration pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_GetAxes(ST1VAFE3BX_Object_t *pObj, ST1VAFE3BX_Axes_t *Acceleration)
{
  st1vafe3bx_md_t mode;
  st1vafe3bx_xl_data_t data;

  if (st1vafe3bx_mode_get(&(pObj->Ctx), &mode) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  if (st1vafe3bx_xl_data_get(&(pObj->Ctx), &mode, &data) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  Acceleration->x = (int32_t)data.mg[0];
  Acceleration->y = (int32_t)data.mg[1];
  Acceleration->z = (int32_t)data.mg[2];

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Get the ST1VAFE3BX register value
  * @param  pObj the device pObj
  * @param  Reg address to be read
  * @param  Data pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_Read_Reg(ST1VAFE3BX_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (st1vafe3bx_read_reg(&(pObj->Ctx), Reg, Data, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Set the ST1VAFE3BX register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_Write_Reg(ST1VAFE3BX_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (st1vafe3bx_write_reg(&(pObj->Ctx), Reg, &Data, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Get the ST1VAFE3BX ACC data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_Get_DRDY_Status(ST1VAFE3BX_Object_t *pObj, uint8_t *Status)
{
  st1vafe3bx_status_t device_status;

  if (st1vafe3bx_status_get(&(pObj->Ctx), &device_status) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  *Status = device_status.drdy;

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Get the ST1VAFE3BX ACC initialization status
  * @param  pObj the device pObj
  * @param  Status 1 if initialized, 0 otherwise
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_Get_Init_Status(ST1VAFE3BX_Object_t *pObj, uint8_t *Status)
{
  if (pObj == NULL)
  {
    return ST1VAFE3BX_ERROR;
  }

  *Status = pObj->is_initialized;

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Set memory bank
  * @param  pObj the device pObj
  * @param  Val the value of memory bank in reg FUNC_CFG_ACCESS
  *         0 - ST1VAFE3BX_MAIN_MEM_BANK, 1 - ST1VAFE3BX_EMBED_FUNC_MEM_BANK
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_Set_Mem_Bank(ST1VAFE3BX_Object_t *pObj, uint8_t Val)
{
  int32_t ret = ST1VAFE3BX_OK;
  st1vafe3bx_mem_bank_t reg;

  reg = (Val == 1U) ? ST1VAFE3BX_EMBED_FUNC_MEM_BANK
        :               ST1VAFE3BX_MAIN_MEM_BANK;

  if (st1vafe3bx_mem_bank_set(&(pObj->Ctx), reg) != ST1VAFE3BX_OK)
  {
    ret = ST1VAFE3BX_ERROR;
  }

  return ret;
}

/**
  * @}
  */

/** @defgroup ST1VAFE3BX_Private_Functions ST1VAFE3BX Private Functions
  * @{
  */

/**
  * @brief  Set the ST1VAFE3BX accelerometer sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @param  Power the ultra low power option
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t ST1VAFE3BX_ACC_SetOutputDataRate_When_Enabled(ST1VAFE3BX_Object_t *pObj, float_t Odr,
    ST1VAFE3BX_Power_Mode_t Power)
{
  st1vafe3bx_md_t mode;

  if (st1vafe3bx_mode_get(&(pObj->Ctx), &mode) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  if (Power == ST1VAFE3BX_ULTRA_LOW_POWER)
  {
    mode.odr = (Odr <= 1.6f) ? ST1VAFE3BX_1Hz6_ULP
               : (Odr <= 3.0f) ? ST1VAFE3BX_3Hz_ULP
               :                 ST1VAFE3BX_25Hz_ULP;
  }
  else if (Power == ST1VAFE3BX_LOW_POWER)
  {
    mode.odr = (Odr <=   6.0f) ? ST1VAFE3BX_6Hz_LP
               : (Odr <=  12.5f) ? ST1VAFE3BX_12Hz5_LP
               : (Odr <=  25.0f) ? ST1VAFE3BX_25Hz_LP
               : (Odr <=  50.0f) ? ST1VAFE3BX_50Hz_LP
               : (Odr <= 100.0f) ? ST1VAFE3BX_100Hz_LP
               : (Odr <= 200.0f) ? ST1VAFE3BX_200Hz_LP
               : (Odr <= 400.0f) ? ST1VAFE3BX_400Hz_LP
               :                   ST1VAFE3BX_800Hz_LP;
  }
  else if (Power == ST1VAFE3BX_HIGH_PERFORMANCE)
  {
    mode.odr = (Odr <=   6.0f) ? ST1VAFE3BX_6Hz_HP
               : (Odr <=  12.5f) ? ST1VAFE3BX_12Hz5_HP
               : (Odr <=  25.0f) ? ST1VAFE3BX_25Hz_HP
               : (Odr <=  50.0f) ? ST1VAFE3BX_50Hz_HP
               : (Odr <= 100.0f) ? ST1VAFE3BX_100Hz_HP
               : (Odr <= 200.0f) ? ST1VAFE3BX_200Hz_HP
               : (Odr <= 400.0f) ? ST1VAFE3BX_400Hz_HP
               :                   ST1VAFE3BX_800Hz_HP;
  }
  else
  {
    /* Do nothing */
  }

  if (st1vafe3bx_mode_set(&(pObj->Ctx), &mode) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  /* Store the current Odr value */
  pObj->acc_odr = (mode.odr == ST1VAFE3BX_1Hz6_ULP) ?   1.6f
                  : (mode.odr == ST1VAFE3BX_3Hz_ULP)  ?   3.0f
                  : (mode.odr == ST1VAFE3BX_6Hz_LP)   ?   6.0f
                  : (mode.odr == ST1VAFE3BX_6Hz_HP)   ?   6.0f
                  : (mode.odr == ST1VAFE3BX_12Hz5_LP) ?  12.5f
                  : (mode.odr == ST1VAFE3BX_12Hz5_HP) ?  12.5f
                  : (mode.odr == ST1VAFE3BX_25Hz_ULP) ?  25.0f
                  : (mode.odr == ST1VAFE3BX_25Hz_LP)  ?  25.0f
                  : (mode.odr == ST1VAFE3BX_25Hz_HP)  ?  25.0f
                  : (mode.odr == ST1VAFE3BX_50Hz_LP)  ?  50.0f
                  : (mode.odr == ST1VAFE3BX_50Hz_HP)  ?  50.0f
                  : (mode.odr == ST1VAFE3BX_100Hz_LP) ? 100.0f
                  : (mode.odr == ST1VAFE3BX_100Hz_HP) ? 100.0f
                  : (mode.odr == ST1VAFE3BX_200Hz_LP) ? 200.0f
                  : (mode.odr == ST1VAFE3BX_200Hz_HP) ? 200.0f
                  : (mode.odr == ST1VAFE3BX_400Hz_LP) ? 400.0f
                  : (mode.odr == ST1VAFE3BX_400Hz_HP) ? 400.0f
                  : (mode.odr == ST1VAFE3BX_800Hz_LP) ? 800.0f
                  : (mode.odr == ST1VAFE3BX_800Hz_HP) ? 800.0f
                  :                                     -1.0f;

  if (pObj->acc_odr == -1.0f)
  {
    return ST1VAFE3BX_ERROR;
  }

  /* Store the current Power value */
  pObj->power_mode = Power;

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Set the ST1VAFE3BX accelerometer sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @param  Power the ultra low power option
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t ST1VAFE3BX_ACC_SetOutputDataRate_When_Disabled(ST1VAFE3BX_Object_t *pObj, float_t Odr,
    ST1VAFE3BX_Power_Mode_t Power)
{
  /* Store the new Odr value */
  if (Power == ST1VAFE3BX_ULTRA_LOW_POWER)
  {
    pObj->acc_odr = (Odr <= 1.5f) ? 1.5f
                    : (Odr <= 3.0f) ? 3.0f
                    :                25.0f;
  }
  else if ((Power == ST1VAFE3BX_LOW_POWER) || (Power == ST1VAFE3BX_HIGH_PERFORMANCE))
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

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Get the status of all hardware events
  * @param  pObj the device pObj
  * @param  Status the status of all hardware events
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_Get_Event_Status(ST1VAFE3BX_Object_t *pObj, ST1VAFE3BX_Event_Status_t *Status)
{
  st1vafe3bx_all_int_src_t all_int_src_reg;
  st1vafe3bx_md1_cfg_t md1_cfg_reg;

  (void)memset((void *)Status, 0x0, sizeof(ST1VAFE3BX_Event_Status_t));

  if (st1vafe3bx_read_reg(&(pObj->Ctx), ST1VAFE3BX_ALL_INT_SRC, (uint8_t *)&all_int_src_reg, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  if (st1vafe3bx_read_reg(&(pObj->Ctx), ST1VAFE3BX_MD1_CFG, (uint8_t *)&md1_cfg_reg, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  if (md1_cfg_reg.int_wu == 1U)
  {
    if (all_int_src_reg.wu_ia_all == 1U)
    {
      Status->WakeUpStatus = 1;
    }
  }

  if (md1_cfg_reg.int_6d == 1U)
  {
    if (all_int_src_reg.d6d_ia_all == 1U)
    {
      Status->D6DOrientationStatus = 1;
    }
  }

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Enable wake up detection
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_Enable_Wake_Up_Detection(ST1VAFE3BX_Object_t *pObj, ST1VAFE3BX_SensorIntPin_t IntPin)
{
  int32_t ret = ST1VAFE3BX_OK;
  st1vafe3bx_md1_cfg_t val1;
  st1vafe3bx_interrupt_cfg_t interrupt_cfg;
  st1vafe3bx_ctrl1_t ctrl1;

  /* Output Data Rate selection */
  if (ST1VAFE3BX_ACC_SetOutputDataRate(pObj, 200.0f) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  /* Full scale selection */
  if (ST1VAFE3BX_ACC_SetFullScale(pObj, 2) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  /* Set wake-up threshold */
  if (ST1VAFE3BX_ACC_Set_Wake_Up_Threshold(pObj, 63) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  /* Set wake-up duration */
  if (ST1VAFE3BX_ACC_Set_Wake_Up_Duration(pObj, 0) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  /* Enable wake-up event on the 3-axis */
  if (st1vafe3bx_read_reg(&(pObj->Ctx), ST1VAFE3BX_CTRL1, (uint8_t *)&ctrl1, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  ctrl1.wu_x_en = PROPERTY_ENABLE;
  ctrl1.wu_y_en = PROPERTY_ENABLE;
  ctrl1.wu_z_en = PROPERTY_ENABLE;

  if (st1vafe3bx_write_reg(&(pObj->Ctx), ST1VAFE3BX_CTRL1, (uint8_t *)&ctrl1, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  /* Enable wake up event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case ST1VAFE3BX_INT1_PIN:
      if (st1vafe3bx_read_reg(&(pObj->Ctx), ST1VAFE3BX_MD1_CFG, (uint8_t *)&val1, 1) != ST1VAFE3BX_OK)
      {
        return ST1VAFE3BX_ERROR;
      }

      val1.int_wu = PROPERTY_ENABLE;

      if (st1vafe3bx_write_reg(&(pObj->Ctx), ST1VAFE3BX_MD1_CFG, (uint8_t *)&val1, 1) != ST1VAFE3BX_OK)
      {
        return ST1VAFE3BX_ERROR;
      }

      if (st1vafe3bx_read_reg(&(pObj->Ctx), ST1VAFE3BX_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != ST1VAFE3BX_OK)
      {
        return ST1VAFE3BX_ERROR;
      }

      interrupt_cfg.interrupts_enable = PROPERTY_ENABLE;

      if (st1vafe3bx_write_reg(&(pObj->Ctx), ST1VAFE3BX_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != ST1VAFE3BX_OK)
      {
        return ST1VAFE3BX_ERROR;
      }
      break;

    case ST1VAFE3BX_INT2_PIN:
    default:
      ret = ST1VAFE3BX_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Disable wake up detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_Disable_Wake_Up_Detection(ST1VAFE3BX_Object_t *pObj)
{
  st1vafe3bx_md1_cfg_t val1;
  st1vafe3bx_ctrl1_t ctrl1;

  /* Disable wake up event on both INT1 and INT2 pins */
  if (st1vafe3bx_read_reg(&(pObj->Ctx), ST1VAFE3BX_MD1_CFG, (uint8_t *)&val1, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  val1.int_wu = PROPERTY_DISABLE;

  if (st1vafe3bx_write_reg(&(pObj->Ctx), ST1VAFE3BX_MD1_CFG, (uint8_t *)&val1, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  /* Disable wake-up event on the 3-axis */
  if (st1vafe3bx_read_reg(&(pObj->Ctx), ST1VAFE3BX_CTRL1, (uint8_t *)&ctrl1, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  ctrl1.wu_x_en = PROPERTY_DISABLE;
  ctrl1.wu_y_en = PROPERTY_DISABLE;
  ctrl1.wu_z_en = PROPERTY_DISABLE;

  if (st1vafe3bx_write_reg(&(pObj->Ctx), ST1VAFE3BX_CTRL1, (uint8_t *)&ctrl1, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  /* Reset wake-up threshold */
  if (ST1VAFE3BX_ACC_Set_Wake_Up_Threshold(pObj, 0) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  /* Reset wake-up duration */
  if (ST1VAFE3BX_ACC_Set_Wake_Up_Duration(pObj, 0) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Set wake up threshold
  * @param  pObj the device pObj
  * @param  Threshold wake up detection threshold
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_Set_Wake_Up_Threshold(ST1VAFE3BX_Object_t *pObj, uint32_t Threshold)
{
  int32_t fs;
  int32_t ret = ST1VAFE3BX_OK;
  float_t tmp;
  st1vafe3bx_interrupt_cfg_t interrupt_cfg;
  st1vafe3bx_wake_up_ths_t wup_ths;

  if (ST1VAFE3BX_ACC_GetFullScale(pObj, &fs) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  if(st1vafe3bx_read_reg(&(pObj->Ctx), ST1VAFE3BX_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  if(st1vafe3bx_read_reg(&(pObj->Ctx), ST1VAFE3BX_WAKE_UP_THS, (uint8_t *)&wup_ths, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
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
      ret = ST1VAFE3BX_ERROR;
      break;
  }

  if(ret != ST1VAFE3BX_ERROR)
  {
    if(st1vafe3bx_write_reg(&(pObj->Ctx), ST1VAFE3BX_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != ST1VAFE3BX_OK)
    {
      return ST1VAFE3BX_ERROR;
    }

    if(st1vafe3bx_write_reg(&(pObj->Ctx), ST1VAFE3BX_WAKE_UP_THS, (uint8_t *)&wup_ths, 1) != ST1VAFE3BX_OK)
    {
      return ST1VAFE3BX_ERROR;
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
int32_t ST1VAFE3BX_ACC_Set_Wake_Up_Duration(ST1VAFE3BX_Object_t *pObj, uint8_t Duration)
{
  st1vafe3bx_wake_up_dur_t wup_dur;
  st1vafe3bx_wake_up_dur_ext_t wup_dur_ext;

  /* Check if the duration is one of the possible values */
  if((Duration != 0U) && (Duration != 1U) && (Duration != 2U) && (Duration != 3U) && (Duration != 7U) && (Duration != 11U) && (Duration != 15U))
  {
    return ST1VAFE3BX_ERROR;
  }

  if (st1vafe3bx_read_reg(&(pObj->Ctx), ST1VAFE3BX_WAKE_UP_DUR, (uint8_t *)&wup_dur, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  if (st1vafe3bx_read_reg(&(pObj->Ctx), ST1VAFE3BX_WAKE_UP_DUR_EXT, (uint8_t *)&wup_dur_ext, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
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
    else // Duration = 15U
    {
      wup_dur.wake_dur = 3;
    }
  }

  if (st1vafe3bx_write_reg(&(pObj->Ctx), ST1VAFE3BX_WAKE_UP_DUR, (uint8_t *)&wup_dur, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  if (st1vafe3bx_write_reg(&(pObj->Ctx), ST1VAFE3BX_WAKE_UP_DUR_EXT, (uint8_t *)&wup_dur_ext, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Enable 6d orientation
  * @param  pObj the device pObj
  * @param  IntPin interrupt pin line to be used
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_Enable_6D_Orientation(ST1VAFE3BX_Object_t *pObj, ST1VAFE3BX_SensorIntPin_t IntPin)
{
  int32_t ret = ST1VAFE3BX_OK;
  st1vafe3bx_md1_cfg_t val1;
  st1vafe3bx_interrupt_cfg_t interrupt_cfg;

  /* Output Data Rate selection */
  if (ST1VAFE3BX_ACC_SetOutputDataRate(pObj, 400.0f) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  /* Full scale selection */
  if (ST1VAFE3BX_ACC_SetFullScale(pObj, 2) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  /* Threshold selection*/
  if (ST1VAFE3BX_ACC_Set_6D_Orientation_Threshold(pObj, 2) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  /* Enable 6D orientation event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case ST1VAFE3BX_INT1_PIN:
      if (st1vafe3bx_read_reg(&(pObj->Ctx), ST1VAFE3BX_MD1_CFG, (uint8_t *)&val1, 1) != ST1VAFE3BX_OK)
      {
        return ST1VAFE3BX_ERROR;
      }

      val1.int_6d = PROPERTY_ENABLE;

      if (st1vafe3bx_write_reg(&(pObj->Ctx), ST1VAFE3BX_MD1_CFG, (uint8_t *)&val1, 1) != ST1VAFE3BX_OK)
      {
        return ST1VAFE3BX_ERROR;
      }

      if (st1vafe3bx_read_reg(&(pObj->Ctx), ST1VAFE3BX_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != ST1VAFE3BX_OK)
      {
        return ST1VAFE3BX_ERROR;
      }

      interrupt_cfg.interrupts_enable = PROPERTY_ENABLE;

      if (st1vafe3bx_write_reg(&(pObj->Ctx), ST1VAFE3BX_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != ST1VAFE3BX_OK)
      {
        return ST1VAFE3BX_ERROR;
      }

      if (st1vafe3bx_read_reg(&(pObj->Ctx), ST1VAFE3BX_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != ST1VAFE3BX_OK)
      {
        return ST1VAFE3BX_ERROR;
      }

      interrupt_cfg.interrupts_enable = PROPERTY_ENABLE;

      if (st1vafe3bx_write_reg(&(pObj->Ctx), ST1VAFE3BX_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1) != ST1VAFE3BX_OK)
      {
        return ST1VAFE3BX_ERROR;
      }
      break;

    case ST1VAFE3BX_INT2_PIN:
    default:
      ret = ST1VAFE3BX_ERROR;
      break;
  }

  return ret;

}


/**
  * @brief  Disable 6D orientation detection
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_Disable_6D_Orientation(ST1VAFE3BX_Object_t *pObj)
{
  st1vafe3bx_md1_cfg_t val1;

  /* Reset threshold */
  if (ST1VAFE3BX_ACC_Set_6D_Orientation_Threshold(pObj, 0) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  /* Disable 6D orientation event on INT1 pin */
  if (st1vafe3bx_read_reg(&(pObj->Ctx), ST1VAFE3BX_MD1_CFG, (uint8_t *)&val1, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  val1.int_6d = PROPERTY_DISABLE;

  if (st1vafe3bx_write_reg(&(pObj->Ctx), ST1VAFE3BX_MD1_CFG, (uint8_t *)&val1, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Set 6D orientation threshold
  * @param  pObj the device pObj
  * @param  Threshold free fall detection threshold
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_Set_6D_Orientation_Threshold(ST1VAFE3BX_Object_t *pObj, uint8_t Threshold)
{
  st1vafe3bx_sixd_t sixd;

  if (Threshold > 3U)
  {
    return ST1VAFE3BX_ERROR;
  }

  if (st1vafe3bx_read_reg(&(pObj->Ctx), ST1VAFE3BX_SIXD, (uint8_t *)&sixd, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  sixd.d6d_ths = Threshold;

  if (st1vafe3bx_write_reg(&(pObj->Ctx), ST1VAFE3BX_SIXD, (uint8_t *)&sixd, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  return ST1VAFE3BX_OK;

}

/**
  * @brief  Get the status of XLow orientation
  * @param  pObj the device pObj
  * @param  XLow the status of XLow orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_Get_6D_Orientation_XL(ST1VAFE3BX_Object_t *pObj, uint8_t *XLow)
{
  st1vafe3bx_sixd_src_t data;

  if (st1vafe3bx_read_reg(&(pObj->Ctx), ST1VAFE3BX_SIXD_SRC, (uint8_t *)&data, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  *XLow = data.xl;

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Get the status of XHigh orientation
  * @param  pObj the device pObj
  * @param  XHigh the status of XHigh orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_Get_6D_Orientation_XH(ST1VAFE3BX_Object_t *pObj, uint8_t *XHigh)
{
  st1vafe3bx_sixd_src_t data;

  if (st1vafe3bx_read_reg(&(pObj->Ctx), ST1VAFE3BX_SIXD_SRC, (uint8_t *)&data, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  *XHigh = data.xh;

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Get the status of YLow orientation
  * @param  pObj the device pObj
  * @param  YLow the status of YLow orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_Get_6D_Orientation_YL(ST1VAFE3BX_Object_t *pObj, uint8_t *YLow)
{
  st1vafe3bx_sixd_src_t data;

  if (st1vafe3bx_read_reg(&(pObj->Ctx), ST1VAFE3BX_SIXD_SRC, (uint8_t *)&data, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  *YLow = data.yl;

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Get the status of YHigh orientation
  * @param  pObj the device pObj
  * @param  YHigh the status of YHigh orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_Get_6D_Orientation_YH(ST1VAFE3BX_Object_t *pObj, uint8_t *YHigh)
{
  st1vafe3bx_sixd_src_t data;

  if (st1vafe3bx_read_reg(&(pObj->Ctx), ST1VAFE3BX_SIXD_SRC, (uint8_t *)&data, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  *YHigh = data.yh;

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Get the status of ZLow orientation
  * @param  pObj the device pObj
  * @param  ZLow the status of ZLow orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_Get_6D_Orientation_ZL(ST1VAFE3BX_Object_t *pObj, uint8_t *ZLow)
{
  st1vafe3bx_sixd_src_t data;

  if (st1vafe3bx_read_reg(&(pObj->Ctx), ST1VAFE3BX_SIXD_SRC, (uint8_t *)&data, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  *ZLow = data.zl;

  return ST1VAFE3BX_OK;
}

/**
  * @brief  Get the status of ZHigh orientation
  * @param  pObj the device pObj
  * @param  ZHigh the status of ZHigh orientation
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ST1VAFE3BX_ACC_Get_6D_Orientation_ZH(ST1VAFE3BX_Object_t *pObj, uint8_t *ZHigh)
{
  st1vafe3bx_sixd_src_t data;

  if (st1vafe3bx_read_reg(&(pObj->Ctx), ST1VAFE3BX_SIXD_SRC, (uint8_t *)&data, 1) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }

  *ZHigh = data.zh;

  return ST1VAFE3BX_OK;
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
  ST1VAFE3BX_Object_t *pObj = (ST1VAFE3BX_Object_t *)Handle;

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
  ST1VAFE3BX_Object_t *pObj = (ST1VAFE3BX_Object_t *)Handle;

  return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
}


/**
 * @brief  Get the ST1VAFE6AX vAFE data [LSB]
 * @param  pObj the device pObj
 * @param  vafe pointer where the vafe is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ST1VAFE3BX_BIO_GetVafe(ST1VAFE3BX_Object_t *pObj, int16_t  *vafe)
{
  int32_t ret = ST1VAFE3BX_OK;
  st1vafe3bx_ah_bio_data_t vafe_complete;


  /* Read actual full scale selection from sensor. */
  if (st1vafe3bx_ah_bio_data_get(&(pObj->Ctx), &vafe_complete) != ST1VAFE3BX_OK)
  {
    return ST1VAFE3BX_ERROR;
  }
  *vafe = vafe_complete.raw;
  return ret;
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
