/**
  ******************************************************************************
  * @file    sht40ad1b.h
  * @author  MEMS Software Solutions Team
  * @brief   SHT40AD1B header driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014-2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SHT40AD1B_H
#define SHT40AD1B_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "sht40ad1b_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup SHT40AD1B SHT40AD1B
  * @{
  */

/** @defgroup SHT40AD1B_Exported_Types SHT40AD1B Exported Types
  * @{
  */

typedef int32_t (*SHT40AD1B_Init_Func)(void);
typedef int32_t (*SHT40AD1B_DeInit_Func)(void);
typedef int32_t (*SHT40AD1B_GetTick_Func)(void);
typedef void (*SHT40AD1B_Delay_Func)(uint32_t);
typedef int32_t (*SHT40AD1B_Write_Func)(uint16_t, uint8_t *, uint16_t);
typedef int32_t (*SHT40AD1B_Read_Func)(uint16_t, uint8_t *, uint16_t);

typedef struct
{
  SHT40AD1B_Init_Func    Init;
  SHT40AD1B_DeInit_Func  DeInit;
  uint32_t               BusType; /* 0 means I2C */
  uint8_t                Address;
  SHT40AD1B_Write_Func   Write;
  SHT40AD1B_Read_Func    Read;
  SHT40AD1B_GetTick_Func GetTick;
  SHT40AD1B_Delay_Func   Delay;
} SHT40AD1B_IO_t;

typedef struct
{
  SHT40AD1B_IO_t  IO;
  stmdev_ctx_t    Ctx;
  uint8_t         is_initialized;
  uint8_t         hum_is_enabled;
  uint8_t         temp_is_enabled;
  float           hum_odr;
  float           temp_odr;
  float           hum_value;
  float           temp_value;
} SHT40AD1B_Object_t;

typedef struct
{
  uint8_t Temperature;
  uint8_t Pressure;
  uint8_t Humidity;
  uint8_t Gas;
  uint8_t LowPower;
  float   HumMaxOdr;
  float   TempMaxOdr;
  float   PressMaxOdr;
  float   GasMaxOdr;
} SHT40AD1B_Capabilities_t;

typedef struct
{
  int32_t (*Init)(SHT40AD1B_Object_t *);
  int32_t (*DeInit)(SHT40AD1B_Object_t *);
  int32_t (*ReadID)(SHT40AD1B_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(SHT40AD1B_Object_t *, SHT40AD1B_Capabilities_t *);
} SHT40AD1B_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(SHT40AD1B_Object_t *);
  int32_t (*Disable)(SHT40AD1B_Object_t *);
  int32_t (*GetOutputDataRate)(SHT40AD1B_Object_t *, float *);
  int32_t (*SetOutputDataRate)(SHT40AD1B_Object_t *, float);
  int32_t (*GetHumidity)(SHT40AD1B_Object_t *, float *);
} SHT40AD1B_HUM_Drv_t;

typedef struct
{
  int32_t (*Enable)(SHT40AD1B_Object_t *);
  int32_t (*Disable)(SHT40AD1B_Object_t *);
  int32_t (*GetOutputDataRate)(SHT40AD1B_Object_t *, float *);
  int32_t (*SetOutputDataRate)(SHT40AD1B_Object_t *, float);
  int32_t (*GetTemperature)(SHT40AD1B_Object_t *, float *);
} SHT40AD1B_TEMP_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} sht40ad1b_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} sht40ad1b_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} sht40ad1b_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} sht40ad1b_axis1bit32_t;

/**
  * @}
  */

/** @defgroup SHT40AD1B_Exported_Constants SHT40AD1B Exported Constants
  * @{
  */
#define SHT40AD1B_I2C_BUS           0U

/** SHT40AD1B error codes  **/
#define SHT40AD1B_OK                 0
#define SHT40AD1B_ERROR             -1

/**
  * @}
  */

/** @addtogroup SHT40AD1B_Exported_Functions SHT40AD1B Exported Functions
  * @{
  */

int32_t SHT40AD1B_RegisterBusIO(SHT40AD1B_Object_t *pObj, SHT40AD1B_IO_t *pIO);
int32_t SHT40AD1B_Init(SHT40AD1B_Object_t *pObj);
int32_t SHT40AD1B_DeInit(SHT40AD1B_Object_t *pObj);
int32_t SHT40AD1B_ReadID(SHT40AD1B_Object_t *pObj, uint8_t *Id);
int32_t SHT40AD1B_GetCapabilities(SHT40AD1B_Object_t *pObj, SHT40AD1B_Capabilities_t *Capabilities);
int32_t SHT40AD1B_Get_Init_Status(SHT40AD1B_Object_t *pObj, uint8_t *Status);

int32_t SHT40AD1B_HUM_Enable(SHT40AD1B_Object_t *pObj);
int32_t SHT40AD1B_HUM_Disable(SHT40AD1B_Object_t *pObj);
int32_t SHT40AD1B_HUM_GetOutputDataRate(SHT40AD1B_Object_t *pObj, float *Odr);
int32_t SHT40AD1B_HUM_SetOutputDataRate(SHT40AD1B_Object_t *pObj, float Odr);
int32_t SHT40AD1B_HUM_GetHumidity(SHT40AD1B_Object_t *pObj, float *Value);
int32_t SHT40AD1B_HUM_Get_DRDY_Status(SHT40AD1B_Object_t *pObj, uint8_t *Status);

int32_t SHT40AD1B_TEMP_Enable(SHT40AD1B_Object_t *pObj);
int32_t SHT40AD1B_TEMP_Disable(SHT40AD1B_Object_t *pObj);
int32_t SHT40AD1B_TEMP_GetOutputDataRate(SHT40AD1B_Object_t *pObj, float *Odr);
int32_t SHT40AD1B_TEMP_SetOutputDataRate(SHT40AD1B_Object_t *pObj, float Odr);
int32_t SHT40AD1B_TEMP_GetTemperature(SHT40AD1B_Object_t *pObj, float *Value);
int32_t SHT40AD1B_TEMP_Get_DRDY_Status(SHT40AD1B_Object_t *pObj, uint8_t *Status);

/**
  * @}
  */

/** @addtogroup SHT40AD1B_Exported_Variables SHT40AD1B Exported Variables
  * @{
  */

extern SHT40AD1B_CommonDrv_t SHT40AD1B_COMMON_Driver;
extern SHT40AD1B_HUM_Drv_t SHT40AD1B_HUM_Driver;
extern SHT40AD1B_TEMP_Drv_t SHT40AD1B_TEMP_Driver;

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
