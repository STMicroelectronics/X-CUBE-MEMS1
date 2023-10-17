/**
  ******************************************************************************
  * @file    lps22df.h
  * @author  MEMS Software Solutions Team
  * @brief   LPS22DF header driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LPS22DF_H
#define LPS22DF_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "lps22df_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup LPS22DF LPS22DF
  * @{
  */

/** @defgroup LPS22DF_Exported_Types LPS22DF Exported Types
  * @{
  */

typedef int32_t (*LPS22DF_Init_Func)(void);
typedef int32_t (*LPS22DF_DeInit_Func)(void);
typedef int32_t (*LPS22DF_GetTick_Func)(void);
typedef void    (*LPS22DF_Delay_Func)(uint32_t);
typedef int32_t (*LPS22DF_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*LPS22DF_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef struct
{
  LPS22DF_Init_Func          Init;
  LPS22DF_DeInit_Func        DeInit;
  uint32_t                   BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires, 3 means I3C */
  uint8_t                    Address;
  LPS22DF_WriteReg_Func      WriteReg;
  LPS22DF_ReadReg_Func       ReadReg;
  LPS22DF_GetTick_Func       GetTick;
  LPS22DF_Delay_Func         Delay;
} LPS22DF_IO_t;

typedef struct
{
  LPS22DF_IO_t        IO;
  stmdev_ctx_t        Ctx;
  uint8_t             is_initialized;
  uint8_t             press_is_enabled;
  uint8_t             temp_is_enabled;
  lps22df_md_t        last_odr;
} LPS22DF_Object_t;

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
} LPS22DF_Capabilities_t;

typedef struct
{
  int32_t (*Init)(LPS22DF_Object_t *);
  int32_t (*DeInit)(LPS22DF_Object_t *);
  int32_t (*ReadID)(LPS22DF_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(LPS22DF_Object_t *, LPS22DF_Capabilities_t *);
} LPS22DF_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(LPS22DF_Object_t *);
  int32_t (*Disable)(LPS22DF_Object_t *);
  int32_t (*GetOutputDataRate)(LPS22DF_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LPS22DF_Object_t *, float);
  int32_t (*GetTemperature)(LPS22DF_Object_t *, float *);
} LPS22DF_TEMP_Drv_t;

typedef struct
{
  int32_t (*Enable)(LPS22DF_Object_t *);
  int32_t (*Disable)(LPS22DF_Object_t *);
  int32_t (*GetOutputDataRate)(LPS22DF_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LPS22DF_Object_t *, float);
  int32_t (*GetPressure)(LPS22DF_Object_t *, float *);
} LPS22DF_PRESS_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} lps22df_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} lps22df_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} lps22df_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} lps22df_axis1bit32_t;

/**
  * @}
  */

/** @defgroup LPS22DF_Exported_Constants LPS22DF Exported Constants
  * @{
  */

#define LPS22DF_OK                0
#define LPS22DF_ERROR            -1

#define LPS22DF_I2C_BUS          0U
#define LPS22DF_SPI_4WIRES_BUS   1U
#define LPS22DF_SPI_3WIRES_BUS   2U
#define LPS22DF_I3C_BUS          3U

/**
  * @}
  */

/** @addtogroup LPS22DF_Exported_Functions LPS22DF Exported Functions
  * @{
  */

int32_t LPS22DF_RegisterBusIO(LPS22DF_Object_t *pObj, LPS22DF_IO_t *pIO);
int32_t LPS22DF_Init(LPS22DF_Object_t *pObj);
int32_t LPS22DF_DeInit(LPS22DF_Object_t *pObj);
int32_t LPS22DF_ReadID(LPS22DF_Object_t *pObj, uint8_t *Id);
int32_t LPS22DF_GetCapabilities(LPS22DF_Object_t *pObj, LPS22DF_Capabilities_t *Capabilities);
int32_t LPS22DF_Get_Init_Status(LPS22DF_Object_t *pObj, uint8_t *Status);

int32_t LPS22DF_PRESS_Enable(LPS22DF_Object_t *pObj);
int32_t LPS22DF_PRESS_Disable(LPS22DF_Object_t *pObj);
int32_t LPS22DF_PRESS_GetOutputDataRate(LPS22DF_Object_t *pObj, float *Odr);
int32_t LPS22DF_PRESS_SetOutputDataRate(LPS22DF_Object_t *pObj, float Odr);
int32_t LPS22DF_PRESS_GetPressure(LPS22DF_Object_t *pObj, float *Value);
int32_t LPS22DF_PRESS_Get_DRDY_Status(LPS22DF_Object_t *pObj, uint8_t *Status);

int32_t LPS22DF_TEMP_Enable(LPS22DF_Object_t *pObj);
int32_t LPS22DF_TEMP_Disable(LPS22DF_Object_t *pObj);
int32_t LPS22DF_TEMP_GetOutputDataRate(LPS22DF_Object_t *pObj, float *Odr);
int32_t LPS22DF_TEMP_SetOutputDataRate(LPS22DF_Object_t *pObj, float Odr);
int32_t LPS22DF_TEMP_GetTemperature(LPS22DF_Object_t *pObj, float *Value);
int32_t LPS22DF_TEMP_Get_DRDY_Status(LPS22DF_Object_t *pObj, uint8_t *Status);

int32_t LPS22DF_Read_Reg(LPS22DF_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t LPS22DF_Write_Reg(LPS22DF_Object_t *pObj, uint8_t reg, uint8_t Data);

int32_t LPS22DF_Set_One_Shot(LPS22DF_Object_t *pObj);
int32_t LPS22DF_Get_One_Shot_Status(LPS22DF_Object_t *pObj, uint8_t *Status);

int32_t LPS22DF_Enable_DRDY_Interrupt(LPS22DF_Object_t *pObj);
int32_t LPS22DF_Set_AVG(LPS22DF_Object_t *pObj, uint8_t avg);
int32_t LPS22DF_Set_LPF(LPS22DF_Object_t *pObj, uint8_t lpf);

/**
  * @}
  */

/** @addtogroup LPS22DF_Exported_Variables LPS22DF Exported Variables
  * @{
  */
extern LPS22DF_CommonDrv_t LPS22DF_COMMON_Driver;
extern LPS22DF_PRESS_Drv_t LPS22DF_PRESS_Driver;
extern LPS22DF_TEMP_Drv_t LPS22DF_TEMP_Driver;

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
