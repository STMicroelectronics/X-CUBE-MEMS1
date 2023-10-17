/**
  ******************************************************************************
  * @file    sgp40.h
  * @author  MEMS Software Solutions Team
  * @brief   SGP40 header driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SGP40_H
#define SGP40_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "sgp40_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup SGP40 SGP40
  * @{
  */

/** @defgroup SGP40_Exported_Types SGP40 Exported Types
  * @{
  */

typedef int32_t (*SGP40_Init_Func)(void);
typedef int32_t (*SGP40_DeInit_Func)(void);
typedef int32_t (*SGP40_GetTick_Func)(void);
typedef void (*SGP40_Delay_Func)(uint32_t);
typedef int32_t (*SGP40_Write_Func)(uint16_t, uint8_t *, uint16_t);
typedef int32_t (*SGP40_Read_Func)(uint16_t, uint8_t *, uint16_t);

typedef struct
{
  SGP40_Init_Func    Init;
  SGP40_DeInit_Func  DeInit;
  uint32_t           BusType; /* 0 means I2C */
  uint8_t            Address;
  SGP40_Write_Func   Write;
  SGP40_Read_Func    Read;
  SGP40_GetTick_Func GetTick;
  SGP40_Delay_Func   Delay;
} SGP40_IO_t;

typedef struct
{
  SGP40_IO_t   IO;
  stmdev_ctx_t Ctx;
  uint8_t      is_initialized;
  uint8_t      gas_is_enabled;
  float        gas_odr;
  float        hum_value;
  float        temp_value;
  float        gas_value;
} SGP40_Object_t;

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
} SGP40_Capabilities_t;

typedef struct
{
  int32_t (*Init)(SGP40_Object_t *);
  int32_t (*DeInit)(SGP40_Object_t *);
  int32_t (*ReadID)(SGP40_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(SGP40_Object_t *, SGP40_Capabilities_t *);
} SGP40_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(SGP40_Object_t *);
  int32_t (*Disable)(SGP40_Object_t *);
  int32_t (*GetOutputDataRate)(SGP40_Object_t *, float *);
  int32_t (*SetOutputDataRate)(SGP40_Object_t *, float);
  int32_t (*GetValue)(SGP40_Object_t *, float *);
} SGP40_GAS_Drv_t;

/**
  * @}
  */

/** @defgroup SGP40_Exported_Constants SGP40 Exported Constants
  * @{
  */
#define SGP40_I2C_BUS           0U

/** SGP40 error codes  **/
#define SGP40_OK                 0
#define SGP40_ERROR             -1

/**
  * @}
  */

/** @addtogroup SGP40_Exported_Functions SGP40 Exported Functions
  * @{
  */

int32_t SGP40_RegisterBusIO(SGP40_Object_t *pObj, SGP40_IO_t *pIO);
int32_t SGP40_Init(SGP40_Object_t *pObj);
int32_t SGP40_DeInit(SGP40_Object_t *pObj);
int32_t SGP40_ReadID(SGP40_Object_t *pObj, uint8_t *Id);
int32_t SGP40_GetCapabilities(SGP40_Object_t *pObj, SGP40_Capabilities_t *Capabilities);
int32_t SGP40_Get_Init_Status(SGP40_Object_t *pObj, uint8_t *Status);

int32_t SGP40_GAS_Enable(SGP40_Object_t *pObj);
int32_t SGP40_GAS_Disable(SGP40_Object_t *pObj);
int32_t SGP40_GAS_GetOutputDataRate(SGP40_Object_t *pObj, float *Odr);
int32_t SGP40_GAS_SetOutputDataRate(SGP40_Object_t *pObj, float Odr);
int32_t SGP40_GAS_GetValue(SGP40_Object_t *pObj, float *Value);
int32_t SGP40_GAS_Get_DRDY_Status(SGP40_Object_t *pObj, uint8_t *Status);

int32_t SGP40_GAS_Set_Humidity(SGP40_Object_t *pObj, float Value);
int32_t SGP40_GAS_Set_Temperature(SGP40_Object_t *pObj, float Value);

/**
  * @}
  */

/** @addtogroup SGP40_Exported_Variables SGP40 Exported Variables
  * @{
  */

extern SGP40_CommonDrv_t SGP40_COMMON_Driver;
extern SGP40_GAS_Drv_t SGP40_GAS_Driver;

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
