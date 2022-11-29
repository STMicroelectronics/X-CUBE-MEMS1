/**
  ******************************************************************************
  * @file    lis2mdl.h
  * @author  MEMS Software Solutions Team
  * @brief   LIS2MDL header driver file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LIS2MDL_H
#define LIS2MDL_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "lis2mdl_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup LIS2MDL LIS2MDL
  * @{
  */

/** @defgroup LIS2MDL_Exported_Types LIS2MDL Exported Types
  * @{
  */

typedef int32_t (*LIS2MDL_Init_Func)(void);
typedef int32_t (*LIS2MDL_DeInit_Func)(void);
typedef int32_t (*LIS2MDL_GetTick_Func)(void);
typedef void    (*LIS2MDL_Delay_Func)(uint32_t);
typedef int32_t (*LIS2MDL_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*LIS2MDL_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef struct
{
  LIS2MDL_Init_Func          Init;
  LIS2MDL_DeInit_Func        DeInit;
  uint32_t                   BusType; /* 0 means I2C, 1 means SPI-3-Wires */
  uint8_t                    Address;
  LIS2MDL_WriteReg_Func      WriteReg;
  LIS2MDL_ReadReg_Func       ReadReg;
  LIS2MDL_GetTick_Func       GetTick;
  LIS2MDL_Delay_Func         Delay;
} LIS2MDL_IO_t;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} LIS2MDL_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} LIS2MDL_Axes_t;

typedef struct
{
  LIS2MDL_IO_t        IO;
  stmdev_ctx_t        Ctx;
  uint8_t             is_initialized;
  uint8_t             mag_is_enabled;
} LIS2MDL_Object_t;

typedef struct
{
  uint8_t   Acc;
  uint8_t   Gyro;
  uint8_t   Magneto;
  uint8_t   LowPower;
  uint32_t  GyroMaxFS;
  uint32_t  AccMaxFS;
  uint32_t  MagMaxFS;
  float     GyroMaxOdr;
  float     AccMaxOdr;
  float     MagMaxOdr;
} LIS2MDL_Capabilities_t;

typedef struct
{
  int32_t (*Init)(LIS2MDL_Object_t *);
  int32_t (*DeInit)(LIS2MDL_Object_t *);
  int32_t (*ReadID)(LIS2MDL_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(LIS2MDL_Object_t *, LIS2MDL_Capabilities_t *);
} LIS2MDL_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(LIS2MDL_Object_t *);
  int32_t (*Disable)(LIS2MDL_Object_t *);
  int32_t (*GetSensitivity)(LIS2MDL_Object_t *, float *);
  int32_t (*GetOutputDataRate)(LIS2MDL_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LIS2MDL_Object_t *, float);
  int32_t (*GetFullScale)(LIS2MDL_Object_t *, int32_t *);
  int32_t (*SetFullScale)(LIS2MDL_Object_t *, int32_t);
  int32_t (*GetAxes)(LIS2MDL_Object_t *, LIS2MDL_Axes_t *);
  int32_t (*GetAxesRaw)(LIS2MDL_Object_t *, LIS2MDL_AxesRaw_t *);
} LIS2MDL_MAG_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} lis2mdl_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} lis2mdl_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} lis2mdl_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} lis2mdl_axis1bit32_t;

/**
  * @}
  */

/** @defgroup LIS2MDL_Exported_Constants LIS2MDL Exported Constants
  * @{
  */

#define LIS2MDL_OK                           0
#define LIS2MDL_ERROR                       -1

#define LIS2MDL_I2C_BUS                     0U
#define LIS2MDL_SPI_4WIRES_BUS              1U /* NOTE: if you use the component in SPI 4-Wires mode, you lose the DRDY pin */
#define LIS2MDL_SPI_3WIRES_BUS              2U
#define LSM6DSOX_SENSORHUB_LIS2MDL_I2C_BUS  3U /* LSM6DSOX SensorHub with LIS2MDL example */

#define LIS2MDL_MAG_SENSITIVITY_FS_50GAUSS  1.500f  /**< Sensitivity value for 50 gauss full scale [mgauss/LSB] */

/**
  * @}
  */

/** @defgroup LIS2MDL_Exported_Functions LIS2MDL Exported Functions
  * @{
  */

int32_t LIS2MDL_RegisterBusIO(LIS2MDL_Object_t *pObj, LIS2MDL_IO_t *pIO);
int32_t LIS2MDL_Init(LIS2MDL_Object_t *pObj);
int32_t LIS2MDL_DeInit(LIS2MDL_Object_t *pObj);
int32_t LIS2MDL_ReadID(LIS2MDL_Object_t *pObj, uint8_t *id);
int32_t LIS2MDL_GetCapabilities(LIS2MDL_Object_t *pObj, LIS2MDL_Capabilities_t *Capabilities);

int32_t LIS2MDL_MAG_Enable(LIS2MDL_Object_t *pObj);
int32_t LIS2MDL_MAG_Disable(LIS2MDL_Object_t *pObj);
int32_t LIS2MDL_MAG_GetSensitivity(LIS2MDL_Object_t *pObj, float *sensitivity);
int32_t LIS2MDL_MAG_GetOutputDataRate(LIS2MDL_Object_t *pObj, float *odr);
int32_t LIS2MDL_MAG_SetOutputDataRate(LIS2MDL_Object_t *pObj, float odr);
int32_t LIS2MDL_MAG_GetFullScale(LIS2MDL_Object_t *pObj, int32_t *fullscale);
int32_t LIS2MDL_MAG_SetFullScale(LIS2MDL_Object_t *pObj, int32_t fullscale);
int32_t LIS2MDL_MAG_GetAxes(LIS2MDL_Object_t *pObj, LIS2MDL_Axes_t *magnetic_field);
int32_t LIS2MDL_MAG_GetAxesRaw(LIS2MDL_Object_t *pObj, LIS2MDL_AxesRaw_t *value);

int32_t LIS2MDL_Read_Reg(LIS2MDL_Object_t *pObj, uint8_t reg, uint8_t *data);
int32_t LIS2MDL_Write_Reg(LIS2MDL_Object_t *pObj, uint8_t reg, uint8_t data);

int32_t LIS2MDL_MAG_Set_SelfTest(LIS2MDL_Object_t *pObj, uint8_t val);
int32_t LIS2MDL_MAG_Get_DRDY_Status(LIS2MDL_Object_t *pObj, uint8_t *status);
int32_t LIS2MDL_MAG_Get_Init_Status(LIS2MDL_Object_t *pObj, uint8_t *status);

int32_t LIS2MDL_MAG_Set_Filter_Mode(LIS2MDL_Object_t *pObj, uint8_t FilterMode);
int32_t LIS2MDL_MAG_Set_Power_Mode(LIS2MDL_Object_t *pObj, uint8_t PowerMode);

/**
  * @}
  */

/** @addtogroup LIS2MDL_Exported_Variables LIS2MDL Exported Variables
  * @{
  */

extern LIS2MDL_CommonDrv_t LIS2MDL_COMMON_Driver;
extern LIS2MDL_MAG_Drv_t LIS2MDL_MAG_Driver;

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
