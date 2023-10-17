/**
  ******************************************************************************
  * @file    ilps28qsw.h
  * @author  MEMS Software Solutions Team
  * @brief   ILPS28QSW header driver file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ILPS28QSW_H
#define ILPS28QSW_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "ilps28qsw_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup ILPS28QSW ILPS28QSW
  * @{
  */

/** @defgroup ILPS28QSW_Exported_Types ILPS28QSW Exported Types
  * @{
  */

typedef int32_t (*ILPS28QSW_Init_Func)(void);
typedef int32_t (*ILPS28QSW_DeInit_Func)(void);
typedef int32_t (*ILPS28QSW_GetTick_Func)(void);
typedef void    (*ILPS28QSW_Delay_Func)(uint32_t);
typedef int32_t (*ILPS28QSW_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*ILPS28QSW_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef struct
{
  ILPS28QSW_Init_Func          Init;
  ILPS28QSW_DeInit_Func        DeInit;
  uint32_t                   BusType; /*0 means I2C, 1 means I3C */
  uint8_t                    Address;
  ILPS28QSW_WriteReg_Func      WriteReg;
  ILPS28QSW_ReadReg_Func       ReadReg;
  ILPS28QSW_GetTick_Func       GetTick;
  ILPS28QSW_Delay_Func         Delay;
} ILPS28QSW_IO_t;

typedef struct
{
  ILPS28QSW_IO_t        IO;
  stmdev_ctx_t        Ctx;
  uint8_t             is_initialized;
  uint8_t             press_is_enabled;
  uint8_t             temp_is_enabled;
  ilps28qsw_md_t        last_odr;
} ILPS28QSW_Object_t;

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
} ILPS28QSW_Capabilities_t;

typedef struct
{
  int32_t (*Init)(ILPS28QSW_Object_t *);
  int32_t (*DeInit)(ILPS28QSW_Object_t *);
  int32_t (*ReadID)(ILPS28QSW_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(ILPS28QSW_Object_t *, ILPS28QSW_Capabilities_t *);
} ILPS28QSW_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(ILPS28QSW_Object_t *);
  int32_t (*Disable)(ILPS28QSW_Object_t *);
  int32_t (*GetOutputDataRate)(ILPS28QSW_Object_t *, float *);
  int32_t (*SetOutputDataRate)(ILPS28QSW_Object_t *, float);
  int32_t (*GetTemperature)(ILPS28QSW_Object_t *, float *);
} ILPS28QSW_TEMP_Drv_t;

typedef struct
{
  int32_t (*Enable)(ILPS28QSW_Object_t *);
  int32_t (*Disable)(ILPS28QSW_Object_t *);
  int32_t (*GetOutputDataRate)(ILPS28QSW_Object_t *, float *);
  int32_t (*SetOutputDataRate)(ILPS28QSW_Object_t *, float);
  int32_t (*GetPressure)(ILPS28QSW_Object_t *, float *);
} ILPS28QSW_PRESS_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} ilps28qsw_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} ilps28qsw_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} ilps28qsw_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} ilps28qsw_axis1bit32_t;

/**
  * @}
  */

/** @defgroup ILPS28QSW_Exported_Constants ILPS28QSW Exported Constants
  * @{
  */

#define ILPS28QSW_OK                0
#define ILPS28QSW_ERROR            -1

#define ILPS28QSW_I2C_BUS          0U
#define ILPS28QSW_I3C_BUS          1U

/**
  * @}
  */

/** @addtogroup ILPS28QSW_Exported_Functions ILPS28QSW Exported Functions
  * @{
  */

int32_t ILPS28QSW_RegisterBusIO(ILPS28QSW_Object_t *pObj, ILPS28QSW_IO_t *pIO);
int32_t ILPS28QSW_Init(ILPS28QSW_Object_t *pObj);
int32_t ILPS28QSW_DeInit(ILPS28QSW_Object_t *pObj);
int32_t ILPS28QSW_ReadID(ILPS28QSW_Object_t *pObj, uint8_t *Id);
int32_t ILPS28QSW_GetCapabilities(ILPS28QSW_Object_t *pObj, ILPS28QSW_Capabilities_t *Capabilities);
int32_t ILPS28QSW_Get_Init_Status(ILPS28QSW_Object_t *pObj, uint8_t *Status);

int32_t ILPS28QSW_PRESS_Enable(ILPS28QSW_Object_t *pObj);
int32_t ILPS28QSW_PRESS_Disable(ILPS28QSW_Object_t *pObj);
int32_t ILPS28QSW_PRESS_GetOutputDataRate(ILPS28QSW_Object_t *pObj, float *Odr);
int32_t ILPS28QSW_PRESS_SetOutputDataRate(ILPS28QSW_Object_t *pObj, float Odr);
int32_t ILPS28QSW_PRESS_GetPressure(ILPS28QSW_Object_t *pObj, float *Value);
int32_t ILPS28QSW_PRESS_Get_DRDY_Status(ILPS28QSW_Object_t *pObj, uint8_t *Status);

int32_t ILPS28QSW_TEMP_Enable(ILPS28QSW_Object_t *pObj);
int32_t ILPS28QSW_TEMP_Disable(ILPS28QSW_Object_t *pObj);
int32_t ILPS28QSW_TEMP_GetOutputDataRate(ILPS28QSW_Object_t *pObj, float *Odr);
int32_t ILPS28QSW_TEMP_SetOutputDataRate(ILPS28QSW_Object_t *pObj, float Odr);
int32_t ILPS28QSW_TEMP_GetTemperature(ILPS28QSW_Object_t *pObj, float *Value);
int32_t ILPS28QSW_TEMP_Get_DRDY_Status(ILPS28QSW_Object_t *pObj, uint8_t *Status);

int32_t ILPS28QSW_Read_Reg(ILPS28QSW_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t ILPS28QSW_Write_Reg(ILPS28QSW_Object_t *pObj, uint8_t reg, uint8_t Data);

int32_t ILPS28QSW_Set_One_Shot(ILPS28QSW_Object_t *pObj);
int32_t ILPS28QSW_Get_One_Shot_Status(ILPS28QSW_Object_t *pObj, uint8_t *Status);

int32_t ILPS28QSW_Set_AVG(ILPS28QSW_Object_t *pObj, uint8_t avg);
int32_t ILPS28QSW_Set_LPF(ILPS28QSW_Object_t *pObj, uint8_t lpf);

/**
  * @}
  */

/** @addtogroup ILPS28QSW_Exported_Variables ILPS28QSW Exported Variables
  * @{
  */
extern ILPS28QSW_CommonDrv_t ILPS28QSW_COMMON_Driver;
extern ILPS28QSW_PRESS_Drv_t ILPS28QSW_PRESS_Driver;
extern ILPS28QSW_TEMP_Drv_t ILPS28QSW_TEMP_Driver;

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
