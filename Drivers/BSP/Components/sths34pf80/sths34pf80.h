/**
  ******************************************************************************
  * @file    sths34pf80.h
  * @author  MEMS Software Solutions Team
  * @brief   STHS34PF80 header driver file
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
#ifndef STHS34PF80_H
#define STHS34PF80_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "sths34pf80_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup STHS34PF80 STHS34PF80
  * @{
  */

/** @defgroup STHS34PF80_Exported_Types STHS34PF80 Exported Types
  * @{
  */

typedef int32_t (*STHS34PF80_Init_Func)(void);
typedef int32_t (*STHS34PF80_DeInit_Func)(void);
typedef int32_t (*STHS34PF80_GetTick_Func)(void);
typedef void (*STHS34PF80_Delay_Func)(uint32_t);
typedef int32_t (*STHS34PF80_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*STHS34PF80_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef struct
{
  STHS34PF80_Init_Func          Init;
  STHS34PF80_DeInit_Func        DeInit;
  uint32_t                      BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
  uint8_t                       Address;
  STHS34PF80_WriteReg_Func      WriteReg;
  STHS34PF80_ReadReg_Func       ReadReg;
  STHS34PF80_GetTick_Func       GetTick;
  STHS34PF80_Delay_Func         Delay;
} STHS34PF80_IO_t;

typedef struct
{
  STHS34PF80_IO_t    IO;
  stmdev_ctx_t       Ctx;
  uint8_t            is_initialized;
  uint8_t            temp_is_enabled;
  float_t              temp_odr;
} STHS34PF80_Object_t;

typedef struct
{
  uint8_t Temperature;
  uint8_t Pressure;
  uint8_t Humidity;
  uint8_t Gas;
  uint8_t LowPower;
  float_t   HumMaxOdr;
  float_t   TempMaxOdr;
  float_t   PressMaxOdr;
  float_t   GasMaxOdr;
} STHS34PF80_Capabilities_t;

typedef struct
{
  int32_t (*Init)(STHS34PF80_Object_t *);
  int32_t (*DeInit)(STHS34PF80_Object_t *);
  int32_t (*ReadID)(STHS34PF80_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(STHS34PF80_Object_t *, STHS34PF80_Capabilities_t *);
} STHS34PF80_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(STHS34PF80_Object_t *);
  int32_t (*Disable)(STHS34PF80_Object_t *);
  int32_t (*GetOutputDataRate)(STHS34PF80_Object_t *, float_t *);
  int32_t (*SetOutputDataRate)(STHS34PF80_Object_t *, float_t);
  int32_t (*GetTemperature)(STHS34PF80_Object_t *, float_t *);
} STHS34PF80_TEMP_Drv_t;

/**
  * @}
  */

/** @defgroup STHS34PF80_Exported_Constants STHS34PF80 Exported Constants
  * @{
  */
#define STHS34PF80_I2C_BUS           0U
#define STHS34PF80_SPI_4WIRES_BUS    1U
#define STHS34PF80_SPI_3WIRES_BUS    2U

/** STHS34PF80 error codes  **/
#define STHS34PF80_OK                 0
#define STHS34PF80_ERROR             -1

/**
  * @}
  */

/** @addtogroup STHS34PF80_Exported_Functions STHS34PF80 Exported Functions
  * @{
  */

int32_t STHS34PF80_RegisterBusIO(STHS34PF80_Object_t *pObj, STHS34PF80_IO_t *pIO);
int32_t STHS34PF80_Init(STHS34PF80_Object_t *pObj);
int32_t STHS34PF80_DeInit(STHS34PF80_Object_t *pObj);
int32_t STHS34PF80_ReadID(STHS34PF80_Object_t *pObj, uint8_t *Id);
int32_t STHS34PF80_GetCapabilities(STHS34PF80_Object_t *pObj, STHS34PF80_Capabilities_t *Capabilities);
int32_t STHS34PF80_Get_Init_Status(STHS34PF80_Object_t *pObj, uint8_t *Status);

int32_t STHS34PF80_TEMP_Enable(STHS34PF80_Object_t *pObj);
int32_t STHS34PF80_TEMP_Disable(STHS34PF80_Object_t *pObj);
int32_t STHS34PF80_TEMP_GetOutputDataRate(STHS34PF80_Object_t *pObj, float_t *Odr);
int32_t STHS34PF80_TEMP_SetOutputDataRate(STHS34PF80_Object_t *pObj, float_t Odr);
int32_t STHS34PF80_TEMP_GetTemperature(STHS34PF80_Object_t *pObj, float_t *Value);

int32_t STHS34PF80_TEMP_Get_DRDY_Status(STHS34PF80_Object_t *pObj, uint8_t *Status);

int32_t STHS34PF80_Read_Reg(STHS34PF80_Object_t *pObj, uint8_t Reg, uint8_t *Data);
int32_t STHS34PF80_Write_Reg(STHS34PF80_Object_t *pObj, uint8_t Reg, uint8_t Data);

int32_t STHS34PF80_GetObjectTemperatureRaw(STHS34PF80_Object_t *pObj, int16_t *Value);
int32_t STHS34PF80_GetAmbientTemperatureRaw(STHS34PF80_Object_t *pObj, int16_t *Value);
int32_t STHS34PF80_GetObjectTemperature(STHS34PF80_Object_t *pObj, float_t *Value);
int32_t STHS34PF80_GetAmbientTemperature(STHS34PF80_Object_t *pObj, float_t *Value);
int32_t STHS34PF80_GetPresenceData(STHS34PF80_Object_t *pObj, int16_t *Value);
int32_t STHS34PF80_GetPresenceFlag(STHS34PF80_Object_t *pObj, uint8_t *Value);
int32_t STHS34PF80_GetMotionData(STHS34PF80_Object_t *pObj, int16_t *Value);
int32_t STHS34PF80_GetMotionFlag(STHS34PF80_Object_t *pObj, uint8_t *Value);

int32_t STHS34PF80_GetAvgTmos(STHS34PF80_Object_t *pObj, uint16_t *Value);
int32_t STHS34PF80_SetAvgTmos(STHS34PF80_Object_t *pObj, uint16_t Value);
int32_t STHS34PF80_GetAvgT(STHS34PF80_Object_t *pObj, uint8_t *Value);
int32_t STHS34PF80_GetGainFactor(STHS34PF80_Object_t *pObj, uint8_t *Value);
int32_t STHS34PF80_GetSensData(STHS34PF80_Object_t *pObj, uint8_t *Value);
int32_t STHS34PF80_GetSensitivity(STHS34PF80_Object_t *pObj, uint16_t *Value);

/**
  * @}
  */

/** @addtogroup STHS34PF80_Exported_Variables STHS34PF80 Exported Variables
  * @{
  */

extern STHS34PF80_CommonDrv_t STHS34PF80_COMMON_Driver;
extern STHS34PF80_TEMP_Drv_t STHS34PF80_TEMP_Driver;

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
