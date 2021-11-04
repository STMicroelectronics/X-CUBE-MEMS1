/**
 ******************************************************************************
 * @file    lps33k.h
 * @author  MEMS Software Solutions Team
 * @brief   LPS33K header driver file
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LPS33K_H
#define LPS33K_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "lps33k_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @addtogroup LPS33K LPS33K
 * @{
 */

/** @defgroup LPS33K_Exported_Types LPS33K Exported Types
 * @{
 */

typedef int32_t (*LPS33K_Init_Func)(void);
typedef int32_t (*LPS33K_DeInit_Func)(void);
typedef int32_t (*LPS33K_GetTick_Func)(void);
typedef int32_t (*LPS33K_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*LPS33K_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef struct
{
  LPS33K_Init_Func           Init;
  LPS33K_DeInit_Func         DeInit;
  uint32_t                   BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
  uint8_t                    Address;
  LPS33K_WriteReg_Func       WriteReg;
  LPS33K_ReadReg_Func        ReadReg;
  LPS33K_GetTick_Func        GetTick;
} LPS33K_IO_t;

typedef struct
{
  LPS33K_IO_t         IO;
  stmdev_ctx_t        Ctx;
  uint8_t             is_initialized;
  uint8_t             press_is_enabled;
  uint8_t             temp_is_enabled;
  lps33k_odr_t        last_odr;
} LPS33K_Object_t;

typedef struct
{
  uint8_t Temperature;
  uint8_t Pressure;
  uint8_t Humidity;
  uint8_t LowPower;
  float   HumMaxOdr;
  float   TempMaxOdr;
  float   PressMaxOdr;
} LPS33K_Capabilities_t;

typedef struct
{
  int32_t (*Init)(LPS33K_Object_t *);
  int32_t (*DeInit)(LPS33K_Object_t *);
  int32_t (*ReadID)(LPS33K_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(LPS33K_Object_t *, LPS33K_Capabilities_t *);
} LPS33K_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(LPS33K_Object_t *);
  int32_t (*Disable)(LPS33K_Object_t *);
  int32_t (*GetOutputDataRate)(LPS33K_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LPS33K_Object_t *, float);
  int32_t (*GetTemperature)(LPS33K_Object_t *, float *);
} LPS33K_TEMP_Drv_t;

typedef struct
{
  int32_t (*Enable)(LPS33K_Object_t *);
  int32_t (*Disable)(LPS33K_Object_t *);
  int32_t (*GetOutputDataRate)(LPS33K_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LPS33K_Object_t *, float);
  int32_t (*GetPressure)(LPS33K_Object_t *, float *);
} LPS33K_PRESS_Drv_t;

typedef union{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} lps33k_axis3bit16_t;

typedef union{
  int16_t i16bit;
  uint8_t u8bit[2];
} lps33k_axis1bit16_t;

typedef union{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} lps33k_axis3bit32_t;

typedef union{
  int32_t i32bit;
  uint8_t u8bit[4];
} lps33k_axis1bit32_t;

/**
 * @}
 */

/** @defgroup LPS33K_Exported_Constants LPS33K Exported Constants
 * @{
 */

#define LPS33K_OK                0
#define LPS33K_ERROR            -1

#define LPS33K_I2C_BUS          0U

/**
 * @}
 */

/** @addtogroup LPS33K_Exported_Functions LPS33K Exported Functions
 * @{
 */

int32_t LPS33K_RegisterBusIO(LPS33K_Object_t *pObj, LPS33K_IO_t *pIO);
int32_t LPS33K_Init(LPS33K_Object_t *pObj);
int32_t LPS33K_DeInit(LPS33K_Object_t *pObj);
int32_t LPS33K_ReadID(LPS33K_Object_t *pObj, uint8_t *Id);
int32_t LPS33K_GetCapabilities(LPS33K_Object_t *pObj, LPS33K_Capabilities_t *Capabilities);
int32_t LPS33K_Get_Init_Status(LPS33K_Object_t *pObj, uint8_t *Status);

int32_t LPS33K_PRESS_Enable(LPS33K_Object_t *pObj);
int32_t LPS33K_PRESS_Disable(LPS33K_Object_t *pObj);
int32_t LPS33K_PRESS_GetOutputDataRate(LPS33K_Object_t *pObj, float *Odr);
int32_t LPS33K_PRESS_SetOutputDataRate(LPS33K_Object_t *pObj, float Odr);
int32_t LPS33K_PRESS_GetPressure(LPS33K_Object_t *pObj, float *Value);
int32_t LPS33K_PRESS_Get_DRDY_Status(LPS33K_Object_t *pObj, uint8_t *Status);

int32_t LPS33K_TEMP_Enable(LPS33K_Object_t *pObj);
int32_t LPS33K_TEMP_Disable(LPS33K_Object_t *pObj);
int32_t LPS33K_TEMP_GetOutputDataRate(LPS33K_Object_t *pObj, float *Odr);
int32_t LPS33K_TEMP_SetOutputDataRate(LPS33K_Object_t *pObj, float Odr);
int32_t LPS33K_TEMP_GetTemperature(LPS33K_Object_t *pObj, float *Value);
int32_t LPS33K_TEMP_Get_DRDY_Status(LPS33K_Object_t *pObj, uint8_t *Status);

int32_t LPS33K_Read_Reg(LPS33K_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t LPS33K_Write_Reg(LPS33K_Object_t *pObj, uint8_t reg, uint8_t Data);

int32_t LPS33K_Get_Press(LPS33K_Object_t *pObj, float *Data);
int32_t LPS33K_Get_Temp(LPS33K_Object_t *pObj, float *Data);

int32_t LPS33K_Set_One_Shot(LPS33K_Object_t *pObj);
int32_t LPS33K_Get_One_Shot_Status(LPS33K_Object_t *pObj, uint8_t *Status);

/**
 * @}
 */

/** @addtogroup LPS33K_Exported_Variables LPS33K Exported Variables
 * @{
 */
extern LPS33K_CommonDrv_t LPS33K_COMMON_Driver;
extern LPS33K_PRESS_Drv_t LPS33K_PRESS_Driver;
extern LPS33K_TEMP_Drv_t LPS33K_TEMP_Driver;

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
