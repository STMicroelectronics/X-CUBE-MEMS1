/**
  ******************************************************************************
  * @file    mis2du12.c
  * @author  MEMS Software Solutions Team
  * @brief   MIS2DU12 driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mis2du12.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup MIS2DU12 MIS2DU12
  * @{
  */

/** @defgroup MIS2DU12_Exported_Variables MIS2DU12 Exported Variables
  * @{
  */

MIS2DU12_CommonDrv_t MIS2DU12_COMMON_Driver =
{
  MIS2DU12_Init,
  MIS2DU12_DeInit,
  MIS2DU12_ReadID,
  MIS2DU12_GetCapabilities,
};

MIS2DU12_ACC_Drv_t MIS2DU12_ACC_Driver =
{
  MIS2DU12_ACC_Enable,
  MIS2DU12_ACC_Disable,
  MIS2DU12_ACC_GetSensitivity,
  MIS2DU12_ACC_GetOutputDataRate,
  MIS2DU12_ACC_SetOutputDataRate,
  MIS2DU12_ACC_GetFullScale,
  MIS2DU12_ACC_SetFullScale,
  MIS2DU12_ACC_GetAxes,
  MIS2DU12_ACC_GetAxesRaw,
};

/**
  * @}
  */

/** @defgroup MIS2DU12_Private_Function_Prototypes MIS2DU12 Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t MIS2DU12_ACC_SetOutputDataRate_When_Enabled(MIS2DU12_Object_t *pObj, float Odr,
                                                           MIS2DU12_Ultra_Low_Power_t Power);
static int32_t MIS2DU12_ACC_SetOutputDataRate_When_Disabled(MIS2DU12_Object_t *pObj, float Odr,
                                                            MIS2DU12_Ultra_Low_Power_t Power);

/**
  * @}
  */

/** @defgroup MIS2DU12_Exported_Functions MIS2DU12 Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_RegisterBusIO(MIS2DU12_Object_t *pObj, MIS2DU12_IO_t *pIO)
{
  int32_t ret = MIS2DU12_OK;

  if (pObj == NULL)
  {
    ret = MIS2DU12_ERROR;
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
      ret = MIS2DU12_ERROR;
    }
    else if (pObj->IO.Init() != MIS2DU12_OK)
    {
      ret = MIS2DU12_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == MIS2DU12_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x50;

          if (MIS2DU12_Write_Reg(pObj, MIS2DU12_CTRL1, data) != MIS2DU12_OK)
          {
            ret = MIS2DU12_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  Initialize the MIS2DU12 sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_Init(MIS2DU12_Object_t *pObj)
{
  if(pObj->IO.BusType != MIS2DU12_I3C_BUS)
  {
    /* Disable I3C */
    if (mis2du12_bus_mode_set(&(pObj->Ctx), MIS2DU12_I3C_DISABLE) != MIS2DU12_OK)
    {
      return MIS2DU12_ERROR;
    }
  }

  /* Enable register address automatically incremented during a multiple byte
  access with a serial interface. Enable BDU. */
  if (mis2du12_init_set(&(pObj->Ctx), MIS2DU12_DRV_RDY) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  /* FIFO mode selection */
  mis2du12_fifo_md_t fifo_mode =
  {
    .operation = MIS2DU12_BYPASS,
    .store     = MIS2DU12_16_BIT,
    .watermark = 0,
  };

  if (mis2du12_fifo_mode_set(&(pObj->Ctx), &fifo_mode) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = 100.0f;
  /* Select default ultra low power (disabled). */
  pObj->acc_ultra_low_power = MIS2DU12_ULTRA_LOW_POWER_DISABLE;

  /* Output data rate: power down, full scale: 2g */
  mis2du12_md_t mode =
  {
    .odr = MIS2DU12_OFF,
    .fs  = MIS2DU12_2g,
  };

  if (mis2du12_mode_set(&(pObj->Ctx), &mode) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  pObj->is_initialized = 1;

  return MIS2DU12_OK;
}

/**
  * @brief  Deinitialize the MIS2DU12 sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_DeInit(MIS2DU12_Object_t *pObj)
{
  /* Disable the component */
  if (MIS2DU12_ACC_Disable(pObj) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  /* Reset output data rate. */
  pObj->acc_odr = 0.0f;
  /* Reset ultra low power to default value (disabled). */
  pObj->acc_ultra_low_power = MIS2DU12_ULTRA_LOW_POWER_DISABLE;

  pObj->is_initialized = 0;

  return MIS2DU12_OK;
}

/**
  * @brief  Read component ID
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_ReadID(MIS2DU12_Object_t *pObj, uint8_t *Id)
{
  mis2du12_id_t val;

  if (mis2du12_id_get(&(pObj->Ctx), &val) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  *Id = val.whoami;

  return MIS2DU12_OK;
}

/**
  * @brief  Get MIS2DU12 sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to MIS2DU12 sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_GetCapabilities(MIS2DU12_Object_t *pObj, MIS2DU12_Capabilities_t *Capabilities)
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
  return MIS2DU12_OK;
}

/**
  * @brief  Enable the MIS2DU12 accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_ACC_Enable(MIS2DU12_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return MIS2DU12_OK;
  }

  /* Output data rate selection. */
  if (MIS2DU12_ACC_SetOutputDataRate_When_Enabled(pObj, pObj->acc_odr, pObj->acc_ultra_low_power) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return MIS2DU12_OK;
}

/**
  * @brief  Disable the MIS2DU12 accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_ACC_Disable(MIS2DU12_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return MIS2DU12_OK;
  }

  /* Output data rate selection - power down. */
  mis2du12_md_t mode;

  if (mis2du12_mode_get(&(pObj->Ctx), &mode) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  mode.odr = MIS2DU12_OFF;

  if (mis2du12_mode_set(&(pObj->Ctx), &mode) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return MIS2DU12_OK;
}

/**
  * @brief  Get the MIS2DU12 accelerometer sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_ACC_GetSensitivity(MIS2DU12_Object_t *pObj, float *Sensitivity)
{
  mis2du12_md_t mode;

  if (mis2du12_mode_get(&(pObj->Ctx), &mode) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  switch (mode.fs)
  {
    case MIS2DU12_2g:
      *Sensitivity = MIS2DU12_ACC_SENSITIVITY_FOR_FS_2G;
      break;

    case MIS2DU12_4g:
      *Sensitivity = MIS2DU12_ACC_SENSITIVITY_FOR_FS_4G;
      break;

    case MIS2DU12_8g:
      *Sensitivity = MIS2DU12_ACC_SENSITIVITY_FOR_FS_8G;
      break;

    case MIS2DU12_16g:
      *Sensitivity = MIS2DU12_ACC_SENSITIVITY_FOR_FS_16G;
      break;

    default:
      *Sensitivity = -1.0f;
      return MIS2DU12_ERROR;
  }

  return MIS2DU12_OK;
}

/**
  * @brief  Get the MIS2DU12 accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_ACC_GetOutputDataRate(MIS2DU12_Object_t *pObj, float *Odr)
{
  mis2du12_md_t mode;

  /* Read actual output data rate from sensor. */
  if (mis2du12_mode_get(&(pObj->Ctx), &mode) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  switch (mode.odr)
  {
    case MIS2DU12_OFF:
    case MIS2DU12_TRIG_PIN:
    case MIS2DU12_TRIG_SW:
      *Odr = 0.0f;
      break;

    case MIS2DU12_1Hz6_ULP:
      *Odr = 1.6f;
      break;

    case MIS2DU12_3Hz_ULP:
      *Odr = 3.0f;
      break;

    case MIS2DU12_6Hz_ULP:
    case MIS2DU12_6Hz:
      *Odr = 6.0f;
      break;

    case MIS2DU12_12Hz5:
      *Odr = 12.5f;
      break;

    case MIS2DU12_25Hz:
      *Odr = 25.0f;
      break;

    case MIS2DU12_50Hz:
      *Odr = 50.0f;
      break;

    case MIS2DU12_100Hz:
      *Odr = 100.0f;
      break;

    case MIS2DU12_200Hz:
      *Odr = 200.0f;
      break;

    case MIS2DU12_400Hz:
      *Odr = 400.0f;
      break;

    case MIS2DU12_800Hz:
      *Odr = 800.0f;
      break;

    default:
      *Odr = -1.0f;
      return MIS2DU12_ERROR;
  }

  return MIS2DU12_OK;
}

/**
  * @brief  Set the MIS2DU12 accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_ACC_SetOutputDataRate(MIS2DU12_Object_t *pObj, float Odr)
{
  /* By default we use Ultra Low Power disabled */
  return MIS2DU12_ACC_SetOutputDataRate_With_Mode(pObj, Odr, MIS2DU12_ULTRA_LOW_POWER_DISABLE);
}

/**
  * @brief  Set the MIS2DU12 accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @param  Power the ultra low power option
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_ACC_SetOutputDataRate_With_Mode(MIS2DU12_Object_t *pObj, float Odr, MIS2DU12_Ultra_Low_Power_t Power)
{
  /* Check if the component is enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return MIS2DU12_ACC_SetOutputDataRate_When_Enabled(pObj, Odr, Power);
  }
  else
  {
    return MIS2DU12_ACC_SetOutputDataRate_When_Disabled(pObj, Odr, Power);
  }
}

/**
  * @brief  Get the MIS2DU12 accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_ACC_GetFullScale(MIS2DU12_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = MIS2DU12_OK;
  mis2du12_md_t mode;

  /* Read actual full scale selection from sensor. */
  if (mis2du12_mode_get(&(pObj->Ctx), &mode) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  switch (mode.fs)
  {
    case MIS2DU12_2g:
      *FullScale =  2;
      break;

    case MIS2DU12_4g:
      *FullScale =  4;
      break;

    case MIS2DU12_8g:
      *FullScale =  8;
      break;

    case MIS2DU12_16g:
      *FullScale = 16;
      break;

    default:
      *FullScale = -1;
      ret = MIS2DU12_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the MIS2DU12 accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_ACC_SetFullScale(MIS2DU12_Object_t *pObj, int32_t FullScale)
{
  mis2du12_md_t mode;

  if (mis2du12_mode_get(&(pObj->Ctx), &mode) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file statical analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  mode.fs = (FullScale <= 2) ? MIS2DU12_2g
            : (FullScale <= 4) ? MIS2DU12_4g
            : (FullScale <= 8) ? MIS2DU12_8g
            :                    MIS2DU12_16g;

  if (mis2du12_mode_set(&(pObj->Ctx), &mode) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  return MIS2DU12_OK;
}

/**
  * @brief  Get the MIS2DU12 accelerometer sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_ACC_GetAxesRaw(MIS2DU12_Object_t *pObj, MIS2DU12_AxesRaw_t *Value)
{
  mis2du12_md_t mode;
  mis2du12_data_t data;

  if (mis2du12_mode_get(&(pObj->Ctx), &mode) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  if (mis2du12_data_get(&(pObj->Ctx), &mode, &data) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  Value->x = data.xl.raw[0];
  Value->y = data.xl.raw[1];
  Value->z = data.xl.raw[2];

  return MIS2DU12_OK;
}

/**
  * @brief  Get the MIS2DU12 accelerometer sensor axes
  * @param  pObj the device pObj
  * @param  Acceleration pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_ACC_GetAxes(MIS2DU12_Object_t *pObj, MIS2DU12_Axes_t *Acceleration)
{
  mis2du12_md_t mode;
  mis2du12_data_t data;

  if (mis2du12_mode_get(&(pObj->Ctx), &mode) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  if (mis2du12_data_get(&(pObj->Ctx), &mode, &data) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  Acceleration->x = (int32_t)data.xl.mg[0];
  Acceleration->y = (int32_t)data.xl.mg[1];
  Acceleration->z = (int32_t)data.xl.mg[2];

  return MIS2DU12_OK;
}

/**
  * @brief  Get the MIS2DU12 register value
  * @param  pObj the device pObj
  * @param  Reg address to be read
  * @param  Data pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_Read_Reg(MIS2DU12_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (mis2du12_read_reg(&(pObj->Ctx), Reg, Data, 1) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  return MIS2DU12_OK;
}

/**
  * @brief  Set the MIS2DU12 register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_Write_Reg(MIS2DU12_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (mis2du12_write_reg(&(pObj->Ctx), Reg, &Data, 1) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  return MIS2DU12_OK;
}

/**
  * @brief  Set the interrupt latch
  * @param  pObj the device pObj
  * @param  Status value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_Set_Interrupt_Latch(MIS2DU12_Object_t *pObj, uint8_t Status)
{
  mis2du12_int_mode_t mode;

  if (mis2du12_interrupt_mode_get(&(pObj->Ctx), &mode) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  switch (Status)
  {
    case 0:
      mode.base_sig = MIS2DU12_INT_LEVEL;
      break;

    case 3:
      mode.base_sig = MIS2DU12_INT_LATCHED;
      break;

    default:
      return MIS2DU12_ERROR;
  }

  if (mis2du12_interrupt_mode_set(&(pObj->Ctx), &mode) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  return MIS2DU12_OK;
}

/**
  * @brief  Enable DRDY interrupt mode
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_ACC_Enable_DRDY_Interrupt(MIS2DU12_Object_t *pObj)
{
  mis2du12_int_mode_t mode;

  if (mis2du12_interrupt_mode_get(&(pObj->Ctx), &mode) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  mode.drdy_latched = PROPERTY_DISABLE;

  if (mis2du12_interrupt_mode_set(&(pObj->Ctx), &mode) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  mis2du12_pin_int_route_t int2_route;

  if (mis2du12_pin_int2_route_get(&(pObj->Ctx), &int2_route) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  int2_route.drdy_xl = PROPERTY_ENABLE;

  if (mis2du12_pin_int2_route_set(&(pObj->Ctx), &int2_route) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  return MIS2DU12_OK;
}

/**
  * @brief  Disable DRDY interrupt mode
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_ACC_Disable_DRDY_Interrupt(MIS2DU12_Object_t *pObj)
{
  mis2du12_pin_int_route_t int2_route;

  if (mis2du12_pin_int2_route_get(&(pObj->Ctx), &int2_route) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  int2_route.drdy_xl = PROPERTY_DISABLE;

  if (mis2du12_pin_int2_route_set(&(pObj->Ctx), &int2_route) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  return MIS2DU12_OK;
}

/**
  * @brief  Set self test
  * @param  pObj the device pObj
  * @param  Val the value of ST in reg CTRL3
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_ACC_Set_SelfTest(MIS2DU12_Object_t *pObj, uint8_t Val)
{
  mis2du12_st_t reg;

  switch (Val)
  {
    case 0:
      reg = MIS2DU12_ST_DISABLE;
      break;

    case 1:
      reg = MIS2DU12_ST_POSITIVE;
      break;

    case 2:
      reg = MIS2DU12_ST_NEGATIVE;
      break;

    default:
      return MIS2DU12_ERROR;
  }

  if (mis2du12_self_test_sign_set(&(pObj->Ctx), reg) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  return MIS2DU12_OK;
}

/**
  * @brief  Get the MIS2DU12 ACC data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_ACC_Get_DRDY_Status(MIS2DU12_Object_t *pObj, uint8_t *Status)
{
  mis2du12_status_t device_status;

  if (mis2du12_status_get(&(pObj->Ctx), &device_status) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  *Status = device_status.drdy_xl;

  return MIS2DU12_OK;
}

/**
  * @brief  Get the MIS2DU12 ACC initialization status
  * @param  pObj the device pObj
  * @param  Status 1 if initialized, 0 otherwise
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_ACC_Get_Init_Status(MIS2DU12_Object_t *pObj, uint8_t *Status)
{
  if (pObj == NULL)
  {
    return MIS2DU12_ERROR;
  }

  *Status = pObj->is_initialized;

  return MIS2DU12_OK;
}

/**
  * @}
  */

/** @defgroup MIS2DU12_Private_Functions MIS2DU12 Private Functions
  * @{
  */

/**
  * @brief  Set the MIS2DU12 accelerometer sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @param  Power the ultra low power option
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t MIS2DU12_ACC_SetOutputDataRate_When_Enabled(MIS2DU12_Object_t *pObj, float Odr,
                                                           MIS2DU12_Ultra_Low_Power_t Power)
{
  mis2du12_md_t mode;

  if (mis2du12_mode_get(&(pObj->Ctx), &mode) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  if (Power == MIS2DU12_ULTRA_LOW_POWER_ENABLE)
  {
    mode.odr = (Odr <= 1.6f) ? MIS2DU12_1Hz6_ULP
               : (Odr <= 3.0f) ? MIS2DU12_3Hz_ULP
               :                 MIS2DU12_6Hz_ULP;
  }
  else
  {
    mode.odr = (Odr <=   6.0f) ? MIS2DU12_6Hz
               : (Odr <=  12.5f) ? MIS2DU12_12Hz5
               : (Odr <=  25.0f) ? MIS2DU12_25Hz
               : (Odr <=  50.0f) ? MIS2DU12_50Hz
               : (Odr <= 100.0f) ? MIS2DU12_100Hz
               : (Odr <= 200.0f) ? MIS2DU12_200Hz
               : (Odr <= 400.0f) ? MIS2DU12_400Hz
               :                   MIS2DU12_800Hz;
  }

  if (mis2du12_mode_set(&(pObj->Ctx), &mode) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }

  /* Store the current Odr value */
  pObj->acc_odr = (mode.odr == MIS2DU12_1Hz6_ULP) ?   1.6f
                  : (mode.odr == MIS2DU12_3Hz_ULP)  ?   3.0f
                  : (mode.odr == MIS2DU12_6Hz_ULP)  ?   6.0f
                  : (mode.odr == MIS2DU12_6Hz)      ?   6.0f
                  : (mode.odr == MIS2DU12_12Hz5)    ?  12.5f
                  : (mode.odr == MIS2DU12_25Hz)     ?  25.0f
                  : (mode.odr == MIS2DU12_50Hz)     ?  50.0f
                  : (mode.odr == MIS2DU12_100Hz)    ? 100.0f
                  : (mode.odr == MIS2DU12_200Hz)    ? 200.0f
                  : (mode.odr == MIS2DU12_400Hz)    ? 400.0f
                  : (mode.odr == MIS2DU12_800Hz)    ? 800.0f
                  :                                    -1.0f;

  if (pObj->acc_odr == -1.0f)
  {
    return MIS2DU12_ERROR;
  }

  /* Store the current Power value */
  pObj->acc_ultra_low_power = Power;

  return MIS2DU12_OK;
}

/**
  * @brief  Set the MIS2DU12 accelerometer sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @param  Power the ultra low power option
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t MIS2DU12_ACC_SetOutputDataRate_When_Disabled(MIS2DU12_Object_t *pObj, float Odr,
                                                            MIS2DU12_Ultra_Low_Power_t Power)
{
  /* Store the new Odr value */
  if (Power == MIS2DU12_ULTRA_LOW_POWER_ENABLE)
  {
    pObj->acc_odr = (Odr <= 1.5f) ? 1.5f
                    : (Odr <= 3.0f) ? 3.0f
                    :                 6.0f;
  }
  else
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

  /* Store the new Power value */
  pObj->acc_ultra_low_power = Power;

  return MIS2DU12_OK;
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
  MIS2DU12_Object_t *pObj = (MIS2DU12_Object_t *)Handle;

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
  MIS2DU12_Object_t *pObj = (MIS2DU12_Object_t *)Handle;

  return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
}



/**
  * @brief  Set the MIS2DU12 accelerometer filter mode
  * @param  pObj the device pObj
  * @param  LowHighPassFlag 0/1 for setting low-pass/high-pass filter mode
  * @param  FilterMode Value of the filter Mode
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MIS2DU12_ACC_Set_Filter_Mode(MIS2DU12_Object_t *pObj, uint8_t FilterMode)
{
  mis2du12_md_t mode;

  if (mis2du12_mode_get(&(pObj->Ctx), &mode) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }
  mode.bw = (FilterMode == 0) ? MIS2DU12_ODR_div_2
             : (FilterMode == 1) ? MIS2DU12_ODR_div_4
             : (FilterMode == 2) ? MIS2DU12_ODR_div_8
             :                     MIS2DU12_ODR_div_16;

  if (mis2du12_mode_set(&(pObj->Ctx), &mode) != MIS2DU12_OK)
  {
    return MIS2DU12_ERROR;
  }
  return MIS2DU12_OK;
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
