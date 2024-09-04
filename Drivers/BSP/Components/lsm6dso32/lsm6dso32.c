/**
  ******************************************************************************
  * @file    lsm6dso32.c
  * @author  MEMS Software Solutions Team
  * @brief   LSM6DSO32 driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lsm6dso32.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup LSM6DSO32 LSM6DSO32
  * @{
  */

/** @defgroup LSM6DSO32_Exported_Variables LSM6DSO32 Exported Variables
  * @{
  */

LSM6DSO32_CommonDrv_t LSM6DSO32_COMMON_Driver =
{
  LSM6DSO32_Init,
  LSM6DSO32_DeInit,
  LSM6DSO32_ReadID,
  LSM6DSO32_GetCapabilities,
};

LSM6DSO32_ACC_Drv_t LSM6DSO32_ACC_Driver =
{
  LSM6DSO32_ACC_Enable,
  LSM6DSO32_ACC_Disable,
  LSM6DSO32_ACC_GetSensitivity,
  LSM6DSO32_ACC_GetOutputDataRate,
  LSM6DSO32_ACC_SetOutputDataRate,
  LSM6DSO32_ACC_GetFullScale,
  LSM6DSO32_ACC_SetFullScale,
  LSM6DSO32_ACC_GetAxes,
  LSM6DSO32_ACC_GetAxesRaw,
};

LSM6DSO32_GYRO_Drv_t LSM6DSO32_GYRO_Driver =
{
  LSM6DSO32_GYRO_Enable,
  LSM6DSO32_GYRO_Disable,
  LSM6DSO32_GYRO_GetSensitivity,
  LSM6DSO32_GYRO_GetOutputDataRate,
  LSM6DSO32_GYRO_SetOutputDataRate,
  LSM6DSO32_GYRO_GetFullScale,
  LSM6DSO32_GYRO_SetFullScale,
  LSM6DSO32_GYRO_GetAxes,
  LSM6DSO32_GYRO_GetAxesRaw,
};

/**
  * @}
  */

/** @defgroup LSM6DSO32_Private_Function_Prototypes LSM6DSO32 Private Function Prototypes
  * @{
  */

static int32_t LSM6DSO32_ACC_SetOutputDataRate_When_Enabled(LSM6DSO32_Object_t *pObj, float_t Odr);
static int32_t LSM6DSO32_ACC_SetOutputDataRate_When_Disabled(LSM6DSO32_Object_t *pObj, float_t Odr);
static int32_t LSM6DSO32_GYRO_SetOutputDataRate_When_Enabled(LSM6DSO32_Object_t *pObj, float_t Odr);
static int32_t LSM6DSO32_GYRO_SetOutputDataRate_When_Disabled(LSM6DSO32_Object_t *pObj, float_t Odr);
static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);

/**
  * @}
  */

/** @defgroup LSM6DSO32_Exported_Functions LSM6DSO32 Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_RegisterBusIO(LSM6DSO32_Object_t *pObj, LSM6DSO32_IO_t *pIO)
{
  int32_t ret = LSM6DSO32_OK;

  if (pObj == NULL)
  {
    ret = LSM6DSO32_ERROR;
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
      ret = LSM6DSO32_ERROR;
    }
    else if (pObj->IO.Init() != LSM6DSO32_OK)
    {
      ret = LSM6DSO32_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == LSM6DSO32_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x0C;

          if (LSM6DSO32_Write_Reg(pObj, LSM6DSO32_CTRL3_C, data) != LSM6DSO32_OK)
          {
            ret = LSM6DSO32_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  Initialize the LSM6DSO32 sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_Init(LSM6DSO32_Object_t *pObj)
{
  if(pObj->IO.BusType != LSM6DSO32_I3C_BUS)
  {
    /* Disable I3C */
    if (lsm6dso32_i3c_disable_set(&(pObj->Ctx), LSM6DSO32_I3C_DISABLE) != LSM6DSO32_OK)
    {
      return LSM6DSO32_ERROR;
    }
  }

  /* Set main memory bank */
  if (LSM6DSO32_Set_Mem_Bank(pObj, (uint8_t)LSM6DSO32_USER_BANK) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  /* Enable register address automatically incremented during a multiple byte
  access with a serial interface. */
  if (lsm6dso32_auto_increment_set(&(pObj->Ctx), PROPERTY_ENABLE) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  /* Enable BDU */
  if (lsm6dso32_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  /* FIFO mode selection */
  if (lsm6dso32_fifo_mode_set(&(pObj->Ctx), LSM6DSO32_BYPASS_MODE) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = LSM6DSO32_XL_ODR_104Hz_HIGH_PERF;

  /* Output data rate selection - power down. */
  if (lsm6dso32_xl_data_rate_set(&(pObj->Ctx), LSM6DSO32_XL_ODR_OFF) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  /* Full scale selection. */
  if (lsm6dso32_xl_full_scale_set(&(pObj->Ctx), LSM6DSO32_4g) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  /* Select default output data rate. */
  pObj->gyro_odr = LSM6DSO32_GY_ODR_104Hz_HIGH_PERF;

  /* Output data rate selection - power down. */
  if (lsm6dso32_gy_data_rate_set(&(pObj->Ctx), LSM6DSO32_GY_ODR_OFF) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  /* Full scale selection. */
  if (lsm6dso32_gy_full_scale_set(&(pObj->Ctx), LSM6DSO32_2000dps) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  pObj->is_initialized = 1;

  return LSM6DSO32_OK;
}

/**
  * @brief  Deinitialize the LSM6DSO32 sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_DeInit(LSM6DSO32_Object_t *pObj)
{
  /* Disable the component */
  if (LSM6DSO32_ACC_Disable(pObj) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  if (LSM6DSO32_GYRO_Disable(pObj) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  /* Reset output data rate. */
  pObj->acc_odr = LSM6DSO32_XL_ODR_OFF;
  pObj->gyro_odr = LSM6DSO32_GY_ODR_OFF;

  pObj->is_initialized = 0;

  return LSM6DSO32_OK;
}

/**
  * @brief  Read component ID
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_ReadID(LSM6DSO32_Object_t *pObj, uint8_t *Id)
{
  if (lsm6dso32_device_id_get(&(pObj->Ctx), Id) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  return LSM6DSO32_OK;
}

/**
  * @brief  Get LSM6DSO32 sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to LSM6DSO32 sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_GetCapabilities(LSM6DSO32_Object_t *pObj, LSM6DSO32_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Acc          = 1;
  Capabilities->Gyro         = 1;
  Capabilities->Magneto      = 0;
  Capabilities->LowPower     = 1;
  Capabilities->GyroMaxFS    = 2000;
  Capabilities->AccMaxFS     = 32;
  Capabilities->MagMaxFS     = 0;
  Capabilities->GyroMaxOdr   = 6660.0f;
  Capabilities->AccMaxOdr    = 6660.0f;
  Capabilities->MagMaxOdr    = 0.0f;
  return LSM6DSO32_OK;
}

/**
  * @brief  Enable the LSM6DSO32 accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_ACC_Enable(LSM6DSO32_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return LSM6DSO32_OK;
  }

  /* Output data rate selection. */
  if (lsm6dso32_xl_data_rate_set(&(pObj->Ctx), pObj->acc_odr) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return LSM6DSO32_OK;
}

/**
  * @brief  Disable the LSM6DSO32 accelerometer sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_ACC_Disable(LSM6DSO32_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return LSM6DSO32_OK;
  }

  /* Get current output data rate. */
  if (lsm6dso32_xl_data_rate_get(&(pObj->Ctx), &pObj->acc_odr) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  /* Output data rate selection - power down. */
  if (lsm6dso32_xl_data_rate_set(&(pObj->Ctx), LSM6DSO32_XL_ODR_OFF) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return LSM6DSO32_OK;
}

/**
  * @brief  Get the LSM6DSO32 accelerometer sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_ACC_GetSensitivity(LSM6DSO32_Object_t *pObj, float_t *Sensitivity)
{
  int32_t ret = LSM6DSO32_OK;
  lsm6dso32_fs_xl_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (lsm6dso32_xl_full_scale_get(&(pObj->Ctx), &full_scale) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  /* Store the Sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case LSM6DSO32_4g:
      *Sensitivity = LSM6DSO32_ACC_SENSITIVITY_FS_4G;
      break;

    case LSM6DSO32_8g:
      *Sensitivity = LSM6DSO32_ACC_SENSITIVITY_FS_8G;
      break;

    case LSM6DSO32_16g:
      *Sensitivity = LSM6DSO32_ACC_SENSITIVITY_FS_16G;
      break;

    case LSM6DSO32_32g:
      *Sensitivity = LSM6DSO32_ACC_SENSITIVITY_FS_32G;
      break;

    default:
      ret = LSM6DSO32_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the LSM6DSO32 accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_ACC_GetOutputDataRate(LSM6DSO32_Object_t *pObj, float_t *Odr)
{
  int32_t ret = LSM6DSO32_OK;
  lsm6dso32_odr_xl_t odr_low_level;

  /* Get current output data rate. */
  if (lsm6dso32_xl_data_rate_get(&(pObj->Ctx), &odr_low_level) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  switch (odr_low_level)
  {
    case LSM6DSO32_XL_ODR_OFF:
      *Odr = 0.0f;
      break;

    case LSM6DSO32_XL_ODR_6Hz5_ULTRA_LOW_PW:
    case LSM6DSO32_XL_ODR_6Hz5_LOW_PW:
      *Odr = 6.5f;
      break;

    case LSM6DSO32_XL_ODR_12Hz5_ULTRA_LOW_PW:
    case LSM6DSO32_XL_ODR_12Hz5_LOW_PW:
    case LSM6DSO32_XL_ODR_12Hz5_HIGH_PERF:
      *Odr = 12.5f;
      break;

    case LSM6DSO32_XL_ODR_26Hz_ULTRA_LOW_PW:
    case LSM6DSO32_XL_ODR_26Hz_LOW_PW:
    case LSM6DSO32_XL_ODR_26Hz_HIGH_PERF:
      *Odr = 26.0f;
      break;

    case LSM6DSO32_XL_ODR_52Hz_ULTRA_LOW_PW:
    case LSM6DSO32_XL_ODR_52Hz_LOW_PW:
    case LSM6DSO32_XL_ODR_52Hz_HIGH_PERF:
      *Odr = 52.0f;
      break;

    case LSM6DSO32_XL_ODR_104Hz_ULTRA_LOW_PW:
    case LSM6DSO32_XL_ODR_104Hz_NORMAL_MD:
    case LSM6DSO32_XL_ODR_104Hz_HIGH_PERF:
      *Odr = 104.0f;
      break;

    case LSM6DSO32_XL_ODR_208Hz_ULTRA_LOW_PW:
    case LSM6DSO32_XL_ODR_208Hz_NORMAL_MD:
    case LSM6DSO32_XL_ODR_208Hz_HIGH_PERF:
      *Odr = 208.0f;
      break;

    case LSM6DSO32_XL_ODR_417Hz_HIGH_PERF:
      *Odr = 417.0f;
      break;

    case LSM6DSO32_XL_ODR_833Hz_HIGH_PERF:
      *Odr = 833.0f;
      break;

    case LSM6DSO32_XL_ODR_1667Hz_HIGH_PERF:
      *Odr = 1667.0f;
      break;

    case LSM6DSO32_XL_ODR_3333Hz_HIGH_PERF:
      *Odr = 3333.0f;
      break;

    case LSM6DSO32_XL_ODR_6667Hz_HIGH_PERF:
      *Odr = 6667.0f;
      break;

    default:
      ret = LSM6DSO32_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the LSM6DSO32 accelerometer sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_ACC_SetOutputDataRate(LSM6DSO32_Object_t *pObj, float_t Odr)
{
  return LSM6DSO32_ACC_SetOutputDataRate_With_Mode(pObj, Odr, LSM6DSO32_ACC_HIGH_PERFORMANCE_MODE);
}

/**
  * @brief  Set the LSM6DSO32 accelerometer sensor output data rate with operating mode
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @param  Mode the accelerometer operating mode
  * @note   This function switches off the gyroscope if Ultra Low Power Mode is set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_ACC_SetOutputDataRate_With_Mode(LSM6DSO32_Object_t *pObj, float_t Odr,
                                                  LSM6DSO32_ACC_Operating_Mode_t Mode)
{
  int32_t ret = LSM6DSO32_OK;
  float_t newOdr = Odr;

  switch (Mode)
  {
    case LSM6DSO32_ACC_HIGH_PERFORMANCE_MODE:
    {
      /* We must uncheck Low Power and Ultra Low Power bits if they are enabled */
      lsm6dso32_ctrl5_c_t val1;
      lsm6dso32_ctrl6_c_t val2;

      if (lsm6dso32_read_reg(&(pObj->Ctx), LSM6DSO32_CTRL5_C, (uint8_t *)&val1, 1) != LSM6DSO32_OK)
      {
        return LSM6DSO32_ERROR;
      }

      if (val1.xl_ulp_en != 0U)
      {
        /* Power off the accelerometer */
        if (pObj->acc_is_enabled == 1U)
        {
          if (lsm6dso32_xl_data_rate_set(&(pObj->Ctx), LSM6DSO32_XL_ODR_OFF) != LSM6DSO32_OK)
          {
            return LSM6DSO32_ERROR;
          }
        }

        val1.xl_ulp_en = 0;
        if (lsm6dso32_write_reg(&(pObj->Ctx), LSM6DSO32_CTRL5_C, (uint8_t *)&val1, 1) != LSM6DSO32_OK)
        {
          return LSM6DSO32_ERROR;
        }
      }

      if (lsm6dso32_read_reg(&(pObj->Ctx), LSM6DSO32_CTRL6_C, (uint8_t *)&val2, 1) != LSM6DSO32_OK)
      {
        return LSM6DSO32_ERROR;
      }

      if (val2.xl_hm_mode != 0U)
      {
        val2.xl_hm_mode = 0U;
        if (lsm6dso32_write_reg(&(pObj->Ctx), LSM6DSO32_CTRL6_C, (uint8_t *)&val2, 1) != LSM6DSO32_OK)
        {
          return LSM6DSO32_ERROR;
        }
      }

      /* ODR should be at least 12.5Hz */
      if (newOdr < 12.5f)
      {
        newOdr = 12.5f;
      }
      break;
    }

    case LSM6DSO32_ACC_LOW_POWER_NORMAL_MODE:
    {
      /* We must uncheck Ultra Low Power bit if it is enabled */
      /* and check the Low Power bit if it is unchecked       */
      lsm6dso32_ctrl5_c_t val1;
      lsm6dso32_ctrl6_c_t val2;

      if (lsm6dso32_read_reg(&(pObj->Ctx), LSM6DSO32_CTRL5_C, (uint8_t *)&val1, 1) != LSM6DSO32_OK)
      {
        return LSM6DSO32_ERROR;
      }

      if (val1.xl_ulp_en != 0U)
      {
        /* Power off the accelerometer */
        if (pObj->acc_is_enabled == 1U)
        {
          if (lsm6dso32_xl_data_rate_set(&(pObj->Ctx), LSM6DSO32_XL_ODR_OFF) != LSM6DSO32_OK)
          {
            return LSM6DSO32_ERROR;
          }
        }

        val1.xl_ulp_en = 0;
        if (lsm6dso32_write_reg(&(pObj->Ctx), LSM6DSO32_CTRL5_C, (uint8_t *)&val1, 1) != LSM6DSO32_OK)
        {
          return LSM6DSO32_ERROR;
        }
      }

      if (lsm6dso32_read_reg(&(pObj->Ctx), LSM6DSO32_CTRL6_C, (uint8_t *)&val2, 1) != LSM6DSO32_OK)
      {
        return LSM6DSO32_ERROR;
      }

      if (val2.xl_hm_mode == 0U)
      {
        val2.xl_hm_mode = 1U;
        if (lsm6dso32_write_reg(&(pObj->Ctx), LSM6DSO32_CTRL6_C, (uint8_t *)&val2, 1) != LSM6DSO32_OK)
        {
          return LSM6DSO32_ERROR;
        }
      }

      /* Now we need to limit the ODR to 208 Hz if it is higher */
      if (newOdr > 208.0f)
      {
        newOdr = 208.0f;
      }
      break;
    }

    case LSM6DSO32_ACC_ULTRA_LOW_POWER_MODE:
    {
      /* We must uncheck Low Power bit if it is enabled                   */
      /* and check the Ultra Low Power bit if it is unchecked             */
      /* We must switch off gyro otherwise Ultra Low Power does not work  */
      lsm6dso32_ctrl5_c_t val1;
      lsm6dso32_ctrl6_c_t val2;

      if (lsm6dso32_read_reg(&(pObj->Ctx), LSM6DSO32_CTRL6_C, (uint8_t *)&val2, 1) != LSM6DSO32_OK)
      {
        return LSM6DSO32_ERROR;
      }

      if (val2.xl_hm_mode != 0U)
      {
        val2.xl_hm_mode = 0U;
        if (lsm6dso32_write_reg(&(pObj->Ctx), LSM6DSO32_CTRL6_C, (uint8_t *)&val2, 1) != LSM6DSO32_OK)
        {
          return LSM6DSO32_ERROR;
        }
      }

      /* Disable Gyro */
      if (pObj->gyro_is_enabled == 1U)
      {
        if (LSM6DSO32_GYRO_Disable(pObj) != LSM6DSO32_OK)
        {
          return LSM6DSO32_ERROR;
        }
      }

      if (lsm6dso32_read_reg(&(pObj->Ctx), LSM6DSO32_CTRL5_C, (uint8_t *)&val1, 1) != LSM6DSO32_OK)
      {
        return LSM6DSO32_ERROR;
      }

      if (val1.xl_ulp_en == 0U)
      {
        /* Power off the accelerometer */
        if (pObj->acc_is_enabled == 1U)
        {
          if (lsm6dso32_xl_data_rate_set(&(pObj->Ctx), LSM6DSO32_XL_ODR_OFF) != LSM6DSO32_OK)
          {
            return LSM6DSO32_ERROR;
          }
        }

        val1.xl_ulp_en = 1U;
        if (lsm6dso32_write_reg(&(pObj->Ctx), LSM6DSO32_CTRL5_C, (uint8_t *)&val1, 1) != LSM6DSO32_OK)
        {
          return LSM6DSO32_ERROR;
        }
      }

      /* Now we need to limit the ODR to 208 Hz if it is higher */
      if (newOdr > 208.0f)
      {
        newOdr = 208.0f;
      }
      break;
    }

    default:
      ret = LSM6DSO32_ERROR;
      break;
  }

  if (ret == LSM6DSO32_ERROR)
  {
    return LSM6DSO32_ERROR;
  }

  if (pObj->acc_is_enabled == 1U)
  {
    ret = LSM6DSO32_ACC_SetOutputDataRate_When_Enabled(pObj, newOdr);
  }
  else
  {
    ret = LSM6DSO32_ACC_SetOutputDataRate_When_Disabled(pObj, newOdr);
  }

  return ret;
}

/**
  * @brief  Get the LSM6DSO32 accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_ACC_GetFullScale(LSM6DSO32_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = LSM6DSO32_OK;
  lsm6dso32_fs_xl_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (lsm6dso32_xl_full_scale_get(&(pObj->Ctx), &fs_low_level) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  switch (fs_low_level)
  {
    case LSM6DSO32_4g:
      *FullScale =  4;
      break;

    case LSM6DSO32_8g:
      *FullScale =  8;
      break;

    case LSM6DSO32_16g:
      *FullScale = 16;
      break;

    case LSM6DSO32_32g:
      *FullScale = 32;
      break;

    default:
      ret = LSM6DSO32_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the LSM6DSO32 accelerometer sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_ACC_SetFullScale(LSM6DSO32_Object_t *pObj, int32_t FullScale)
{
  lsm6dso32_fs_xl_t new_fs;

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file statical analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  new_fs = (FullScale <=  4) ? LSM6DSO32_4g
           : (FullScale <=  8) ? LSM6DSO32_8g
           : (FullScale <= 16) ? LSM6DSO32_16g
           :                     LSM6DSO32_32g;

  if (lsm6dso32_xl_full_scale_set(&(pObj->Ctx), new_fs) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  return LSM6DSO32_OK;
}

/**
  * @brief  Get the LSM6DSO32 accelerometer sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_ACC_GetAxesRaw(LSM6DSO32_Object_t *pObj, LSM6DSO32_AxesRaw_t *Value)
{
  lsm6dso32_axis3bit16_t data_raw;

  /* Read raw data values. */
  if (lsm6dso32_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return LSM6DSO32_OK;
}

/**
  * @brief  Get the LSM6DSO32 accelerometer sensor axes
  * @param  pObj the device pObj
  * @param  Acceleration pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_ACC_GetAxes(LSM6DSO32_Object_t *pObj, LSM6DSO32_Axes_t *Acceleration)
{
  lsm6dso32_axis3bit16_t data_raw;
  float_t sensitivity = 0.0f;

  /* Read raw data values. */
  if (lsm6dso32_acceleration_raw_get(&(pObj->Ctx), data_raw.i16bit) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  /* Get LSM6DSO32 actual sensitivity. */
  if (LSM6DSO32_ACC_GetSensitivity(pObj, &sensitivity) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  /* Calculate the data. */
  Acceleration->x = (int32_t)((float_t)((float_t)data_raw.i16bit[0] * sensitivity));
  Acceleration->y = (int32_t)((float_t)((float_t)data_raw.i16bit[1] * sensitivity));
  Acceleration->z = (int32_t)((float_t)((float_t)data_raw.i16bit[2] * sensitivity));

  return LSM6DSO32_OK;
}

/**
  * @brief  Enable the LSM6DSO32 gyroscope sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_GYRO_Enable(LSM6DSO32_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->gyro_is_enabled == 1U)
  {
    return LSM6DSO32_OK;
  }

  /* Output data rate selection. */
  if (lsm6dso32_gy_data_rate_set(&(pObj->Ctx), pObj->gyro_odr) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  pObj->gyro_is_enabled = 1;

  return LSM6DSO32_OK;
}

/**
  * @brief  Disable the LSM6DSO32 gyroscope sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_GYRO_Disable(LSM6DSO32_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->gyro_is_enabled == 0U)
  {
    return LSM6DSO32_OK;
  }

  /* Get current output data rate. */
  if (lsm6dso32_gy_data_rate_get(&(pObj->Ctx), &pObj->gyro_odr) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  /* Output data rate selection - power down. */
  if (lsm6dso32_gy_data_rate_set(&(pObj->Ctx), LSM6DSO32_GY_ODR_OFF) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  pObj->gyro_is_enabled = 0;

  return LSM6DSO32_OK;
}

/**
  * @brief  Get the LSM6DSO32 gyroscope sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_GYRO_GetSensitivity(LSM6DSO32_Object_t *pObj, float_t *Sensitivity)
{
  int32_t ret = LSM6DSO32_OK;
  lsm6dso32_fs_g_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (lsm6dso32_gy_full_scale_get(&(pObj->Ctx), &full_scale) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  /* Store the sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case LSM6DSO32_125dps:
      *Sensitivity = LSM6DSO32_GYRO_SENSITIVITY_FS_125DPS;
      break;

    case LSM6DSO32_250dps:
      *Sensitivity = LSM6DSO32_GYRO_SENSITIVITY_FS_250DPS;
      break;

    case LSM6DSO32_500dps:
      *Sensitivity = LSM6DSO32_GYRO_SENSITIVITY_FS_500DPS;
      break;

    case LSM6DSO32_1000dps:
      *Sensitivity = LSM6DSO32_GYRO_SENSITIVITY_FS_1000DPS;
      break;

    case LSM6DSO32_2000dps:
      *Sensitivity = LSM6DSO32_GYRO_SENSITIVITY_FS_2000DPS;
      break;

    default:
      ret = LSM6DSO32_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Get the LSM6DSO32 gyroscope sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_GYRO_GetOutputDataRate(LSM6DSO32_Object_t *pObj, float_t *Odr)
{
  int32_t ret = LSM6DSO32_OK;
  lsm6dso32_odr_g_t odr_low_level;

  /* Get current output data rate. */
  if (lsm6dso32_gy_data_rate_get(&(pObj->Ctx), &odr_low_level) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  switch (odr_low_level)
  {
    case LSM6DSO32_GY_ODR_OFF:
      *Odr = 0.0f;
      break;

    case LSM6DSO32_GY_ODR_12Hz5_LOW_PW:
    case LSM6DSO32_GY_ODR_12Hz5_HIGH_PERF:
      *Odr = 12.5f;
      break;

    case LSM6DSO32_GY_ODR_26Hz_LOW_PW:
    case LSM6DSO32_GY_ODR_26Hz_HIGH_PERF:
      *Odr = 26.0f;
      break;

    case LSM6DSO32_GY_ODR_52Hz_LOW_PW:
    case LSM6DSO32_GY_ODR_52Hz_HIGH_PERF:
      *Odr = 52.0f;
      break;

    case LSM6DSO32_GY_ODR_104Hz_NORMAL_MD:
    case LSM6DSO32_GY_ODR_104Hz_HIGH_PERF:
      *Odr = 104.0f;
      break;

    case LSM6DSO32_GY_ODR_208Hz_NORMAL_MD:
    case LSM6DSO32_GY_ODR_208Hz_HIGH_PERF:
      *Odr = 208.0f;
      break;

    case LSM6DSO32_GY_ODR_417Hz_HIGH_PERF:
      *Odr = 417.0f;
      break;

    case LSM6DSO32_GY_ODR_833Hz_HIGH_PERF:
      *Odr = 833.0f;
      break;

    case LSM6DSO32_GY_ODR_1667Hz_HIGH_PERF:
      *Odr = 1667.0f;
      break;

    case LSM6DSO32_GY_ODR_3333Hz_HIGH_PERF:
      *Odr = 3333.0f;
      break;

    case LSM6DSO32_GY_ODR_6667Hz_HIGH_PERF:
      *Odr = 6667.0f;
      break;

    default:
      ret = LSM6DSO32_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the LSM6DSO32 gyroscope sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_GYRO_SetOutputDataRate(LSM6DSO32_Object_t *pObj, float_t Odr)
{
  return LSM6DSO32_GYRO_SetOutputDataRate_With_Mode(pObj, Odr, LSM6DSO32_GYRO_HIGH_PERFORMANCE_MODE);
}

/**
  * @brief  Set the LSM6DSO32 gyroscope sensor output data rate with operating mode
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @param  Mode the gyroscope operating mode
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_GYRO_SetOutputDataRate_With_Mode(LSM6DSO32_Object_t *pObj, float_t Odr,
                                                   LSM6DSO32_GYRO_Operating_Mode_t Mode)
{
  int32_t ret = LSM6DSO32_OK;
  float_t newOdr = Odr;

  switch (Mode)
  {
    case LSM6DSO32_GYRO_HIGH_PERFORMANCE_MODE:
    {
      /* We must uncheck Low Power bit if it is enabled */
      lsm6dso32_ctrl7_g_t val1;

      if (lsm6dso32_read_reg(&(pObj->Ctx), LSM6DSO32_CTRL7_G, (uint8_t *)&val1, 1) != LSM6DSO32_OK)
      {
        return LSM6DSO32_ERROR;
      }

      if (val1.g_hm_mode != 0U)
      {
        val1.g_hm_mode = 0U;
        if (lsm6dso32_write_reg(&(pObj->Ctx), LSM6DSO32_CTRL7_G, (uint8_t *)&val1, 1) != LSM6DSO32_OK)
        {
          return LSM6DSO32_ERROR;
        }
      }
      break;
    }

    case LSM6DSO32_GYRO_LOW_POWER_NORMAL_MODE:
    {
      /* We must check the Low Power bit if it is unchecked */
      lsm6dso32_ctrl7_g_t val1;

      if (lsm6dso32_read_reg(&(pObj->Ctx), LSM6DSO32_CTRL7_G, (uint8_t *)&val1, 1) != LSM6DSO32_OK)
      {
        return LSM6DSO32_ERROR;
      }

      if (val1.g_hm_mode == 0U)
      {
        val1.g_hm_mode = 1U;
        if (lsm6dso32_write_reg(&(pObj->Ctx), LSM6DSO32_CTRL7_G, (uint8_t *)&val1, 1) != LSM6DSO32_OK)
        {
          return LSM6DSO32_ERROR;
        }
      }

      /* Now we need to limit the ODR to 208 Hz if it is higher */
      if (newOdr > 208.0f)
      {
        newOdr = 208.0f;
      }
      break;
    }

    default:
      ret = LSM6DSO32_ERROR;
      break;
  }

  if (ret == LSM6DSO32_ERROR)
  {
    return LSM6DSO32_ERROR;
  }

  if (pObj->gyro_is_enabled == 1U)
  {
    ret = LSM6DSO32_GYRO_SetOutputDataRate_When_Enabled(pObj, newOdr);
  }
  else
  {
    ret = LSM6DSO32_GYRO_SetOutputDataRate_When_Disabled(pObj, newOdr);
  }

  return ret;
}

/**
  * @brief  Get the LSM6DSO32 gyroscope sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_GYRO_GetFullScale(LSM6DSO32_Object_t *pObj, int32_t  *FullScale)
{
  int32_t ret = LSM6DSO32_OK;
  lsm6dso32_fs_g_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (lsm6dso32_gy_full_scale_get(&(pObj->Ctx), &fs_low_level) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  switch (fs_low_level)
  {
    case LSM6DSO32_125dps:
      *FullScale =  125;
      break;

    case LSM6DSO32_250dps:
      *FullScale =  250;
      break;

    case LSM6DSO32_500dps:
      *FullScale =  500;
      break;

    case LSM6DSO32_1000dps:
      *FullScale = 1000;
      break;

    case LSM6DSO32_2000dps:
      *FullScale = 2000;
      break;

    default:
      ret = LSM6DSO32_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the LSM6DSO32 gyroscope sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_GYRO_SetFullScale(LSM6DSO32_Object_t *pObj, int32_t FullScale)
{
  lsm6dso32_fs_g_t new_fs;

  new_fs = (FullScale <= 125)  ? LSM6DSO32_125dps
           : (FullScale <= 250)  ? LSM6DSO32_250dps
           : (FullScale <= 500)  ? LSM6DSO32_500dps
           : (FullScale <= 1000) ? LSM6DSO32_1000dps
           :                       LSM6DSO32_2000dps;

  if (lsm6dso32_gy_full_scale_set(&(pObj->Ctx), new_fs) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  return LSM6DSO32_OK;
}

/**
  * @brief  Get the LSM6DSO32 gyroscope sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_GYRO_GetAxesRaw(LSM6DSO32_Object_t *pObj, LSM6DSO32_AxesRaw_t *Value)
{
  lsm6dso32_axis3bit16_t data_raw;

  /* Read raw data values. */
  if (lsm6dso32_angular_rate_raw_get(&(pObj->Ctx), data_raw.i16bit) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return LSM6DSO32_OK;
}

/**
  * @brief  Get the LSM6DSO32 gyroscope sensor axes
  * @param  pObj the device pObj
  * @param  AngularRate pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_GYRO_GetAxes(LSM6DSO32_Object_t *pObj, LSM6DSO32_Axes_t *AngularRate)
{
  lsm6dso32_axis3bit16_t data_raw;
  float_t sensitivity = 0.0f;

  /* Read raw data values. */
  if (lsm6dso32_angular_rate_raw_get(&(pObj->Ctx), data_raw.i16bit) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  /* Get LSM6DSO32 actual sensitivity. */
  if (LSM6DSO32_GYRO_GetSensitivity(pObj, &sensitivity) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  /* Calculate the data. */
  AngularRate->x = (int32_t)((float_t)((float_t)data_raw.i16bit[0] * sensitivity));
  AngularRate->y = (int32_t)((float_t)((float_t)data_raw.i16bit[1] * sensitivity));
  AngularRate->z = (int32_t)((float_t)((float_t)data_raw.i16bit[2] * sensitivity));

  return LSM6DSO32_OK;
}

/**
  * @brief  Get the LSM6DSO32 register value
  * @param  pObj the device pObj
  * @param  Reg address to be read
  * @param  Data pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_Read_Reg(LSM6DSO32_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (lsm6dso32_read_reg(&(pObj->Ctx), Reg, Data, 1) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  return LSM6DSO32_OK;
}

/**
  * @brief  Set the LSM6DSO32 register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_Write_Reg(LSM6DSO32_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (lsm6dso32_write_reg(&(pObj->Ctx), Reg, &Data, 1) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  return LSM6DSO32_OK;
}

/**
  * @brief  Get the LSM6DSO32 ACC data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_ACC_Get_DRDY_Status(LSM6DSO32_Object_t *pObj, uint8_t *Status)
{
  if (lsm6dso32_xl_flag_data_ready_get(&(pObj->Ctx), Status) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  return LSM6DSO32_OK;
}

/**
  * @brief  Get the LSM6DSO32 GYRO data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_GYRO_Get_DRDY_Status(LSM6DSO32_Object_t *pObj, uint8_t *Status)
{
  if (lsm6dso32_gy_flag_data_ready_get(&(pObj->Ctx), Status) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  return LSM6DSO32_OK;
}

/**
  * @brief  Set memory bank
  * @param  pObj the device pObj
  * @param  Val the value of memory bank in reg FUNC_CFG_ACCESS
  *         0 - LSM6DSO32_USER_BANK, 1 - LSM6DSO32_SENSOR_HUB_BANK, 2 - LSM6DSO32_EMBEDDED_FUNC_BANK
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSO32_Set_Mem_Bank(LSM6DSO32_Object_t *pObj, uint8_t Val)
{
  int32_t ret = LSM6DSO32_OK;
  lsm6dso32_reg_access_t reg;

  reg = (Val == 1U) ? LSM6DSO32_SENSOR_HUB_BANK
        : (Val == 2U) ? LSM6DSO32_EMBEDDED_FUNC_BANK
        :               LSM6DSO32_USER_BANK;

  if (lsm6dso32_mem_bank_set(&(pObj->Ctx), reg) != LSM6DSO32_OK)
  {
    ret = LSM6DSO32_ERROR;
  }

  return ret;
}

/**
  * @}
  */

/** @defgroup LSM6DSO32_Private_Functions LSM6DSO32 Private Functions
  * @{
  */

/**
  * @brief  Set the LSM6DSO32 accelerometer sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LSM6DSO32_ACC_SetOutputDataRate_When_Enabled(LSM6DSO32_Object_t *pObj, float_t Odr)
{
  lsm6dso32_odr_xl_t new_odr;

  new_odr = (Odr <=   12.5f) ? LSM6DSO32_XL_ODR_12Hz5_HIGH_PERF
            : (Odr <=   26.0f) ? LSM6DSO32_XL_ODR_26Hz_HIGH_PERF
            : (Odr <=   52.0f) ? LSM6DSO32_XL_ODR_52Hz_HIGH_PERF
            : (Odr <=  104.0f) ? LSM6DSO32_XL_ODR_104Hz_HIGH_PERF
            : (Odr <=  208.0f) ? LSM6DSO32_XL_ODR_208Hz_HIGH_PERF
            : (Odr <=  417.0f) ? LSM6DSO32_XL_ODR_417Hz_HIGH_PERF
            : (Odr <=  833.0f) ? LSM6DSO32_XL_ODR_833Hz_HIGH_PERF
            : (Odr <= 1667.0f) ? LSM6DSO32_XL_ODR_1667Hz_HIGH_PERF
            : (Odr <= 3333.0f) ? LSM6DSO32_XL_ODR_3333Hz_HIGH_PERF
            :                    LSM6DSO32_XL_ODR_6667Hz_HIGH_PERF;

  /* Output data rate selection. */
  if (lsm6dso32_xl_data_rate_set(&(pObj->Ctx), new_odr) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  return LSM6DSO32_OK;
}

/**
  * @brief  Set the LSM6DSO32 accelerometer sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LSM6DSO32_ACC_SetOutputDataRate_When_Disabled(LSM6DSO32_Object_t *pObj, float_t Odr)
{
  pObj->acc_odr = (Odr <=   12.5f) ? LSM6DSO32_XL_ODR_12Hz5_HIGH_PERF
                  : (Odr <=   26.0f) ? LSM6DSO32_XL_ODR_26Hz_HIGH_PERF
                  : (Odr <=   52.0f) ? LSM6DSO32_XL_ODR_52Hz_HIGH_PERF
                  : (Odr <=  104.0f) ? LSM6DSO32_XL_ODR_104Hz_HIGH_PERF
                  : (Odr <=  208.0f) ? LSM6DSO32_XL_ODR_208Hz_HIGH_PERF
                  : (Odr <=  417.0f) ? LSM6DSO32_XL_ODR_417Hz_HIGH_PERF
                  : (Odr <=  833.0f) ? LSM6DSO32_XL_ODR_833Hz_HIGH_PERF
                  : (Odr <= 1667.0f) ? LSM6DSO32_XL_ODR_1667Hz_HIGH_PERF
                  : (Odr <= 3333.0f) ? LSM6DSO32_XL_ODR_3333Hz_HIGH_PERF
                  :                    LSM6DSO32_XL_ODR_6667Hz_HIGH_PERF;

  return LSM6DSO32_OK;
}

/**
  * @brief  Set the LSM6DSO32 gyroscope sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LSM6DSO32_GYRO_SetOutputDataRate_When_Enabled(LSM6DSO32_Object_t *pObj, float_t Odr)
{
  lsm6dso32_odr_g_t new_odr;

  new_odr = (Odr <=   12.5f) ? LSM6DSO32_GY_ODR_12Hz5_HIGH_PERF
            : (Odr <=   26.0f) ? LSM6DSO32_GY_ODR_26Hz_HIGH_PERF
            : (Odr <=   52.0f) ? LSM6DSO32_GY_ODR_52Hz_HIGH_PERF
            : (Odr <=  104.0f) ? LSM6DSO32_GY_ODR_104Hz_HIGH_PERF
            : (Odr <=  208.0f) ? LSM6DSO32_GY_ODR_208Hz_HIGH_PERF
            : (Odr <=  417.0f) ? LSM6DSO32_GY_ODR_417Hz_HIGH_PERF
            : (Odr <=  833.0f) ? LSM6DSO32_GY_ODR_833Hz_HIGH_PERF
            : (Odr <= 1667.0f) ? LSM6DSO32_GY_ODR_1667Hz_HIGH_PERF
            : (Odr <= 3333.0f) ? LSM6DSO32_GY_ODR_3333Hz_HIGH_PERF
            :                    LSM6DSO32_GY_ODR_6667Hz_HIGH_PERF;

  /* Output data rate selection. */
  if (lsm6dso32_gy_data_rate_set(&(pObj->Ctx), new_odr) != LSM6DSO32_OK)
  {
    return LSM6DSO32_ERROR;
  }

  return LSM6DSO32_OK;
}

/**
  * @brief  Set the LSM6DSO32 gyroscope sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t LSM6DSO32_GYRO_SetOutputDataRate_When_Disabled(LSM6DSO32_Object_t *pObj, float_t Odr)
{
  pObj->gyro_odr = (Odr <=   12.5f) ? LSM6DSO32_GY_ODR_12Hz5_HIGH_PERF
                   : (Odr <=   26.0f) ? LSM6DSO32_GY_ODR_26Hz_HIGH_PERF
                   : (Odr <=   52.0f) ? LSM6DSO32_GY_ODR_52Hz_HIGH_PERF
                   : (Odr <=  104.0f) ? LSM6DSO32_GY_ODR_104Hz_HIGH_PERF
                   : (Odr <=  208.0f) ? LSM6DSO32_GY_ODR_208Hz_HIGH_PERF
                   : (Odr <=  417.0f) ? LSM6DSO32_GY_ODR_417Hz_HIGH_PERF
                   : (Odr <=  833.0f) ? LSM6DSO32_GY_ODR_833Hz_HIGH_PERF
                   : (Odr <= 1667.0f) ? LSM6DSO32_GY_ODR_1667Hz_HIGH_PERF
                   : (Odr <= 3333.0f) ? LSM6DSO32_GY_ODR_3333Hz_HIGH_PERF
                   :                    LSM6DSO32_GY_ODR_6667Hz_HIGH_PERF;

  return LSM6DSO32_OK;
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
  LSM6DSO32_Object_t *pObj = (LSM6DSO32_Object_t *)Handle;

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
  LSM6DSO32_Object_t *pObj = (LSM6DSO32_Object_t *)Handle;

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
