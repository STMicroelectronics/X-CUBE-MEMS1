/**
  ******************************************************************************
  * @file    lis2dh12.h
  * @author  MEMS Software Solutions Team
  * @brief   LIS2DH12 header driver file
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
#ifndef LIS2DH12_H
#define LIS2DH12_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "lis2dh12_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup LIS2DH12 LIS2DH12
  * @{
  */

/** @defgroup LIS2DH12_Exported_Types LIS2DH12 Exported Types
  * @{
  */

typedef int32_t (*LIS2DH12_Init_Func)(void);
typedef int32_t (*LIS2DH12_DeInit_Func)(void);
typedef int32_t (*LIS2DH12_GetTick_Func)(void);
typedef void    (*LIS2DH12_Delay_Func)(uint32_t);
typedef int32_t (*LIS2DH12_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*LIS2DH12_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef struct
{
  LIS2DH12_Init_Func           Init;
  LIS2DH12_DeInit_Func         DeInit;
  uint32_t                     BusType; /*0 means I2C, 1 means SPI-3-Wires */
  uint8_t                      Address;
  LIS2DH12_WriteReg_Func       WriteReg;
  LIS2DH12_ReadReg_Func        ReadReg;
  LIS2DH12_GetTick_Func        GetTick;
  LIS2DH12_Delay_Func          Delay;
} LIS2DH12_IO_t;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} LIS2DH12_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} LIS2DH12_Axes_t;

typedef struct
{
  LIS2DH12_IO_t         IO;
  stmdev_ctx_t          Ctx;
  uint8_t               is_initialized;
  uint8_t               acc_is_enabled;
  lis2dh12_odr_t        acc_odr;
} LIS2DH12_Object_t;

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
} LIS2DH12_Capabilities_t;

typedef struct
{
  int32_t (*Init)(LIS2DH12_Object_t *);
  int32_t (*DeInit)(LIS2DH12_Object_t *);
  int32_t (*ReadID)(LIS2DH12_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(LIS2DH12_Object_t *, LIS2DH12_Capabilities_t *);
} LIS2DH12_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(LIS2DH12_Object_t *);
  int32_t (*Disable)(LIS2DH12_Object_t *);
  int32_t (*GetSensitivity)(LIS2DH12_Object_t *, float *);
  int32_t (*GetOutputDataRate)(LIS2DH12_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LIS2DH12_Object_t *, float);
  int32_t (*GetFullScale)(LIS2DH12_Object_t *, int32_t *);
  int32_t (*SetFullScale)(LIS2DH12_Object_t *, int32_t);
  int32_t (*GetAxes)(LIS2DH12_Object_t *, LIS2DH12_Axes_t *);
  int32_t (*GetAxesRaw)(LIS2DH12_Object_t *, LIS2DH12_AxesRaw_t *);
} LIS2DH12_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} lis2dh12_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} lis2dh12_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} lis2dh12_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} lis2dh12_axis1bit32_t;

/**
  * @}
  */

/** @defgroup LIS2DH12_Exported_Constants LIS2DH12 Exported Constants
  * @{
  */

#define LIS2DH12_OK                     0
#define LIS2DH12_ERROR                 -1

#define LIS2DH12_I2C_BUS               0U
#define LIS2DH12_SPI_4WIRES_BUS        1U
#define LIS2DH12_SPI_3WIRES_BUS        2U

#define LIS2DH12_SENSITIVITY_FS_2G_NORMAL_MODE               3.900f  /**< Sensitivity value for 2 g full scale and normal mode [mg/LSB] */
#define LIS2DH12_SENSITIVITY_FS_2G_HIGH_RESOLUTION_MODE      0.980f  /**< Sensitivity value for 2 g full scale and high resolution mode [mg/LSB] */
#define LIS2DH12_SENSITIVITY_FS_2G_LOW_POWER_MODE           15.630f  /**< Sensitivity value for 2 g full scale and low power mode [mg/LSB] */
#define LIS2DH12_SENSITIVITY_FS_4G_NORMAL_MODE               7.820f  /**< Sensitivity value for 4 g full scale and normal mode [mg/LSB] */
#define LIS2DH12_SENSITIVITY_FS_4G_HIGH_RESOLUTION_MODE      1.950f  /**< Sensitivity value for 4 g full scale and high resolution mode [mg/LSB] */
#define LIS2DH12_SENSITIVITY_FS_4G_LOW_POWER_MODE           31.260f  /**< Sensitivity value for 4 g full scale and low power mode [mg/LSB] */
#define LIS2DH12_SENSITIVITY_FS_8G_NORMAL_MODE              15.630f  /**< Sensitivity value for 8 g full scale and normal mode [mg/LSB] */
#define LIS2DH12_SENSITIVITY_FS_8G_HIGH_RESOLUTION_MODE      3.900f  /**< Sensitivity value for 8 g full scale and high resolution mode [mg/LSB] */
#define LIS2DH12_SENSITIVITY_FS_8G_LOW_POWER_MODE           62.520f  /**< Sensitivity value for 8 g full scale and low power mode [mg/LSB] */
#define LIS2DH12_SENSITIVITY_FS_16G_NORMAL_MODE             46.900f  /**< Sensitivity value for 16 g full scale and normal mode [mg/LSB] */
#define LIS2DH12_SENSITIVITY_FS_16G_HIGH_RESOLUTION_MODE    11.720f  /**< Sensitivity value for 16 g full scale and high resolution mode [mg/LSB] */
#define LIS2DH12_SENSITIVITY_FS_16G_LOW_POWER_MODE         187.580f  /**< Sensitivity value for 16 g full scale and low power mode [mg/LSB] */

#define LIS2DH12_MAG_SENSITIVITY_FS_50GAUSS  1.500f  /**< Sensitivity value for 50 gauss full scale [mgauss/LSB] */

/**
  * @}
  */

/** @defgroup LIS2DH12_Exported_Functions LIS2DH12 Exported Functions
  * @{
  */

int32_t LIS2DH12_RegisterBusIO(LIS2DH12_Object_t *pObj, LIS2DH12_IO_t *pIO);
int32_t LIS2DH12_Init(LIS2DH12_Object_t *pObj);
int32_t LIS2DH12_DeInit(LIS2DH12_Object_t *pObj);
int32_t LIS2DH12_ReadID(LIS2DH12_Object_t *pObj, uint8_t *Id);
int32_t LIS2DH12_GetCapabilities(LIS2DH12_Object_t *pObj, LIS2DH12_Capabilities_t *Capabilities);

int32_t LIS2DH12_Enable(LIS2DH12_Object_t *pObj);
int32_t LIS2DH12_Disable(LIS2DH12_Object_t *pObj);
int32_t LIS2DH12_GetSensitivity(LIS2DH12_Object_t *pObj, float *sensitivity);
int32_t LIS2DH12_GetOutputDataRate(LIS2DH12_Object_t *pObj, float *odr);
int32_t LIS2DH12_SetOutputDataRate(LIS2DH12_Object_t *pObj, float odr);
int32_t LIS2DH12_GetFullScale(LIS2DH12_Object_t *pObj, int32_t *fullscale);
int32_t LIS2DH12_SetFullScale(LIS2DH12_Object_t *pObj, int32_t fullscale);
int32_t LIS2DH12_GetAxes(LIS2DH12_Object_t *pObj, LIS2DH12_Axes_t *acceleration);
int32_t LIS2DH12_GetAxesRaw(LIS2DH12_Object_t *pObj, LIS2DH12_AxesRaw_t *value);

int32_t LIS2DH12_Read_Reg(LIS2DH12_Object_t *pObj, uint8_t reg, uint8_t *data);
int32_t LIS2DH12_Write_Reg(LIS2DH12_Object_t *pObj, uint8_t reg, uint8_t data);

int32_t LIS2DH12_Get_DRDY_Status(LIS2DH12_Object_t *pObj, uint8_t *status);
int32_t LIS2DH12_Get_Init_Status(LIS2DH12_Object_t *pObj, uint8_t *status);

/**
  * @}
  */

/** @addtogroup LIS2DH12_Exported_Variables LIS2DH12 Exported Variables
  * @{
  */

extern LIS2DH12_CommonDrv_t LIS2DH12_COMMON_Driver;
extern LIS2DH12_Drv_t LIS2DH12_Driver;

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
