/**
  ******************************************************************************
  * @file    a3g4250d.c
  * @author  MEMS Software Solutions Team
  * @brief   A3G4250D driver file
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
#include "a3g4250d.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup A3G4250D A3G4250D
  * @{
  */

/** @defgroup A3G4250D_Exported_Variables A3G4250D Exported Variables
  * @{
  */

A3G4250D_CommonDrv_t A3G4250D_COMMON_Driver =
{
  A3G4250D_Init,
  A3G4250D_DeInit,
  A3G4250D_ReadID,
  A3G4250D_GetCapabilities,
};

A3G4250D_GYRO_Drv_t A3G4250D_GYRO_Driver =
{
  A3G4250D_GYRO_Enable,
  A3G4250D_GYRO_Disable,
  A3G4250D_GYRO_GetSensitivity,
  A3G4250D_GYRO_GetOutputDataRate,
  A3G4250D_GYRO_SetOutputDataRate,
  A3G4250D_GYRO_GetFullScale,
  A3G4250D_GYRO_SetFullScale,
  A3G4250D_GYRO_GetAxes,
  A3G4250D_GYRO_GetAxesRaw,
};

/**
  * @}
  */

/** @defgroup A3G4250D_Private_Function_Prototypes A3G4250D Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t A3G4250D_GYRO_SetOutputDataRate_When_Enabled(A3G4250D_Object_t *pObj, float_t Odr);
static int32_t A3G4250D_GYRO_SetOutputDataRate_When_Disabled(A3G4250D_Object_t *pObj, float_t Odr);

/**
  * @}
  */

/** @defgroup A3G4250D_Exported_Functions A3G4250D Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t A3G4250D_RegisterBusIO(A3G4250D_Object_t *pObj, A3G4250D_IO_t *pIO)
{
  int32_t ret = A3G4250D_OK;

  if (pObj == NULL)
  {
    ret = A3G4250D_ERROR;
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
      ret = A3G4250D_ERROR;
    }
    else if (pObj->IO.Init() != A3G4250D_OK)
    {
      ret = A3G4250D_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == A3G4250D_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x01;

          if (A3G4250D_Write_Reg(pObj, A3G4250D_CTRL_REG4, data) != A3G4250D_OK)
          {
            return A3G4250D_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  Initialize the A3G4250D sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t A3G4250D_Init(A3G4250D_Object_t *pObj)
{
  /* FIFO mode selection */
  if (a3g4250d_fifo_mode_set(&(pObj->Ctx), A3G4250D_FIFO_BYPASS_MODE) != A3G4250D_OK)
  {
    return A3G4250D_ERROR;
  }

  /* Select default output data rate. */
  pObj->gyro_odr = A3G4250D_ODR_100Hz;

  /* Output data rate selection - power down. */
  if (a3g4250d_data_rate_set(&(pObj->Ctx), A3G4250D_ODR_OFF) != A3G4250D_OK)
  {
    return A3G4250D_ERROR;
  }

  pObj->is_initialized = 1;

  return A3G4250D_OK;
}

/**
  * @brief  Deinitialize the A3G4250D sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t A3G4250D_DeInit(A3G4250D_Object_t *pObj)
{
  /* Disable the component */
  if (A3G4250D_GYRO_Disable(pObj) != A3G4250D_OK)
  {
    return A3G4250D_ERROR;
  }

  /* Reset output data rate. */
  pObj->gyro_odr = A3G4250D_ODR_OFF;

  pObj->is_initialized = 0;

  return A3G4250D_OK;
}

/**
  * @brief  Read component ID
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t A3G4250D_ReadID(A3G4250D_Object_t *pObj, uint8_t *Id)
{
  if (a3g4250d_device_id_get(&(pObj->Ctx), Id) != A3G4250D_OK)
  {
    return A3G4250D_ERROR;
  }

  return A3G4250D_OK;
}

/**
  * @brief  Get A3G4250D sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to A3G4250D sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t A3G4250D_GetCapabilities(A3G4250D_Object_t *pObj, A3G4250D_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Acc          = 0;
  Capabilities->Gyro         = 1;
  Capabilities->Magneto      = 0;
  Capabilities->LowPower     = 0;
  Capabilities->GyroMaxFS    = 245;
  Capabilities->AccMaxFS     = 0;
  Capabilities->MagMaxFS     = 0;
  Capabilities->GyroMaxOdr   = 840.0f;
  Capabilities->AccMaxOdr    = 0.0f;
  Capabilities->MagMaxOdr    = 0.0f;
  return A3G4250D_OK;
}

/**
  * @brief  Enable the A3G4250D gyroscope sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t A3G4250D_GYRO_Enable(A3G4250D_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->gyro_is_enabled == 1U)
  {
    return A3G4250D_OK;
  }

  /* Output data rate selection. */
  if (a3g4250d_data_rate_set(&(pObj->Ctx), pObj->gyro_odr) != A3G4250D_OK)
  {
    return A3G4250D_ERROR;
  }

  pObj->gyro_is_enabled = 1;

  return A3G4250D_OK;
}

/**
  * @brief  Disable the A3G4250D gyroscope sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t A3G4250D_GYRO_Disable(A3G4250D_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->gyro_is_enabled == 0U)
  {
    return A3G4250D_OK;
  }

  /* Get current output data rate. */
  if (a3g4250d_data_rate_get(&(pObj->Ctx), &pObj->gyro_odr) != A3G4250D_OK)
  {
    return A3G4250D_ERROR;
  }

  /* Output data rate selection - power down. */
  if (a3g4250d_data_rate_set(&(pObj->Ctx), A3G4250D_ODR_OFF) != A3G4250D_OK)
  {
    return A3G4250D_ERROR;
  }

  pObj->gyro_is_enabled = 0;

  return A3G4250D_OK;
}

/**
  * @brief  Get the A3G4250D gyroscope sensor sensitivity
  * @param  pObj the device pObj
  * @param  Sensitivity pointer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t A3G4250D_GYRO_GetSensitivity(A3G4250D_Object_t *pObj, float_t *Sensitivity)
{
  (void)pObj;
  *Sensitivity = A3G4250D_GYRO_SENSITIVITY;

  return A3G4250D_OK;
}

/**
  * @brief  Get the A3G4250D gyroscope sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t A3G4250D_GYRO_GetOutputDataRate(A3G4250D_Object_t *pObj, float_t *Odr)
{
  int32_t ret = A3G4250D_OK;
  a3g4250d_dr_t odr_low_level;

  /* Get current output data rate. */
  if (a3g4250d_data_rate_get(&(pObj->Ctx), &odr_low_level) != A3G4250D_OK)
  {
    return A3G4250D_ERROR;
  }

  switch (odr_low_level)
  {
    case A3G4250D_ODR_OFF:
      *Odr = 0.0f;
      break;

    case A3G4250D_ODR_SLEEP:
      *Odr = 0.0f;
      break;

    case A3G4250D_ODR_100Hz:
      *Odr = 105.0f;
      break;

    case A3G4250D_ODR_200Hz:
      *Odr = 208.0f;
      break;

    case A3G4250D_ODR_400Hz:
      *Odr = 420.0f;
      break;

    case A3G4250D_ODR_800Hz:
      *Odr = 840.0f;
      break;

    default:
      *Odr = -1.0f;
      ret = A3G4250D_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the A3G4250D gyroscope sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t A3G4250D_GYRO_SetOutputDataRate(A3G4250D_Object_t *pObj, float_t Odr)
{
  /* Check if the component is enabled */
  if (pObj->gyro_is_enabled == 1U)
  {
    return A3G4250D_GYRO_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return A3G4250D_GYRO_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  Get the A3G4250D gyroscope sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale pointer where the full scale is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t A3G4250D_GYRO_GetFullScale(A3G4250D_Object_t *pObj, int32_t *FullScale)
{
  (void)pObj;
  *FullScale = 245;

  return A3G4250D_OK;
}

/**
  * @brief  Set the A3G4250D gyroscope sensor full scale
  * @param  pObj the device pObj
  * @param  FullScale the functional full scale to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t A3G4250D_GYRO_SetFullScale(A3G4250D_Object_t *pObj, int32_t FullScale)
{
  (void)pObj;
  (void)FullScale;

  /* This device has fixed Full Scale */
  return A3G4250D_OK;
}

/**
  * @brief  Get the A3G4250D gyroscope sensor raw axes
  * @param  pObj the device pObj
  * @param  Value pointer where the raw values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t A3G4250D_GYRO_GetAxesRaw(A3G4250D_Object_t *pObj, A3G4250D_AxesRaw_t *Value)
{
  a3g4250d_axis3bit16_t data_raw;
  a3g4250d_status_reg_t status_reg;

  /* Read raw data values. */
  if (a3g4250d_read_reg(&(pObj->Ctx), A3G4250D_OUT_X_L, data_raw.u8bit, 6) != A3G4250D_OK)
  {
    return A3G4250D_ERROR;
  }

  /* Check data overrun */
  if (a3g4250d_read_reg(&(pObj->Ctx), A3G4250D_STATUS_REG, (uint8_t *)&status_reg, 1) != A3G4250D_OK)
  {
    return A3G4250D_ERROR;
  }

  if (status_reg.zyxor == 1)
  {
    return A3G4250D_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return A3G4250D_OK;
}

/**
  * @brief  Get the A3G4250D gyroscope sensor axes
  * @param  pObj the device pObj
  * @param  AngularRate pointer where the values of the axes are written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t A3G4250D_GYRO_GetAxes(A3G4250D_Object_t *pObj, A3G4250D_Axes_t *AngularRate)
{
  A3G4250D_AxesRaw_t data_raw;
  float_t sensitivity;

  /* Read raw data values. */
  if (A3G4250D_GYRO_GetAxesRaw(pObj, &data_raw) != A3G4250D_OK)
  {
    return A3G4250D_ERROR;
  }

  /* Get A3G4250D actual sensitivity. */
  if (A3G4250D_GYRO_GetSensitivity(pObj, &sensitivity) != A3G4250D_OK)
  {
    return A3G4250D_ERROR;
  }

  /* Calculate the data. */
  AngularRate->x = (int32_t)((float_t)((float_t)data_raw.x * sensitivity));
  AngularRate->y = (int32_t)((float_t)((float_t)data_raw.y * sensitivity));
  AngularRate->z = (int32_t)((float_t)((float_t)data_raw.z * sensitivity));

  return A3G4250D_OK;
}

/**
  * @brief  Get the A3G4250D register value
  * @param  pObj the device pObj
  * @param  Reg address to be read
  * @param  Data pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t A3G4250D_Read_Reg(A3G4250D_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (a3g4250d_read_reg(&(pObj->Ctx), Reg, Data, 1) != A3G4250D_OK)
  {
    return A3G4250D_ERROR;
  }

  return A3G4250D_OK;
}

/**
  * @brief  Set the A3G4250D register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t A3G4250D_Write_Reg(A3G4250D_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (a3g4250d_write_reg(&(pObj->Ctx), Reg, &Data, 1) != A3G4250D_OK)
  {
    return A3G4250D_ERROR;
  }

  return A3G4250D_OK;
}

/**
  * @brief  Get the A3G4250D GYRO data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t A3G4250D_GYRO_Get_DRDY_Status(A3G4250D_Object_t *pObj, uint8_t *Status)
{
  if (a3g4250d_flag_data_ready_get(&(pObj->Ctx), Status) != A3G4250D_OK)
  {
    return A3G4250D_ERROR;
  }

  return A3G4250D_OK;
}

/**
  * @}
  */

/** @defgroup A3G4250D_Private_Functions A3G4250D Private Functions
  * @{
  */

/**
  * @brief  Set the A3G4250D gyroscope sensor output data rate when enabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t A3G4250D_GYRO_SetOutputDataRate_When_Enabled(A3G4250D_Object_t *pObj, float_t Odr)
{
  a3g4250d_dr_t new_odr;

  new_odr = (Odr <= 105.0f) ? A3G4250D_ODR_100Hz
            : (Odr <= 208.0f) ? A3G4250D_ODR_200Hz
            : (Odr <= 420.0f) ? A3G4250D_ODR_400Hz
            :                   A3G4250D_ODR_800Hz;

  /* Output data rate selection. */
  if (a3g4250d_data_rate_set(&(pObj->Ctx), new_odr) != A3G4250D_OK)
  {
    return A3G4250D_ERROR;
  }

  /* Store the current output data rate value */
  pObj->gyro_odr = new_odr;

  return A3G4250D_OK;
}

/**
  * @brief  Set the A3G4250D gyroscope sensor output data rate when disabled
  * @param  pObj the device pObj
  * @param  Odr the functional output data rate to be set
  * @retval 0 in case of success, an error code otherwise
  */
static int32_t A3G4250D_GYRO_SetOutputDataRate_When_Disabled(A3G4250D_Object_t *pObj, float_t Odr)
{
  /* Store the new output data rate value */
  pObj->gyro_odr = (Odr <= 105.0f) ? A3G4250D_ODR_100Hz
                   : (Odr <= 208.0f) ? A3G4250D_ODR_200Hz
                   : (Odr <= 420.0f) ? A3G4250D_ODR_400Hz
                   :                   A3G4250D_ODR_800Hz;

  return A3G4250D_OK;
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
  A3G4250D_Object_t *pObj = (A3G4250D_Object_t *)Handle;

  if (pObj->IO.BusType == A3G4250D_I2C_BUS) /* I2C */
  {
    /* Enable Multi-byte read */
    return pObj->IO.ReadReg(pObj->IO.Address, (Reg | 0x80U), pData, Length);
  }
  else   /* SPI 3-Wires */
  {
    /* Enable Multi-byte read */
    return pObj->IO.ReadReg(pObj->IO.Address, (Reg | 0x40U), pData, Length);
  }
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
  A3G4250D_Object_t *pObj = (A3G4250D_Object_t *)Handle;

  if (pObj->IO.BusType == A3G4250D_I2C_BUS) /* I2C */
  {
    /* Enable Multi-byte write */
    return pObj->IO.WriteReg(pObj->IO.Address, (Reg | 0x80U), pData, Length);
  }
  else   /* SPI 3-Wires */
  {
    /* Enable Multi-byte write */
    return pObj->IO.WriteReg(pObj->IO.Address, (Reg | 0x40U), pData, Length);
  }
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
