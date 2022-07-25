/**
  ******************************************************************************
  * @file    lps22ch.h
  * @author  MEMS Software Solutions Team
  * @brief   LPS22CH header driver file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LPS22CH_H
#define LPS22CH_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "lps22ch_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup LPS22CH LPS22CH
  * @{
  */

/** @defgroup LPS22CH_Exported_Types LPS22CH Exported Types
  * @{
  */

typedef int32_t (*LPS22CH_Init_Func)(void);
typedef int32_t (*LPS22CH_DeInit_Func)(void);
typedef int32_t (*LPS22CH_GetTick_Func)(void);
typedef int32_t (*LPS22CH_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*LPS22CH_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef struct
{
  LPS22CH_Init_Func          Init;
  LPS22CH_DeInit_Func        DeInit;
  uint32_t                   BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
  uint8_t                    Address;
  LPS22CH_WriteReg_Func      WriteReg;
  LPS22CH_ReadReg_Func       ReadReg;
  LPS22CH_GetTick_Func       GetTick;
} LPS22CH_IO_t;

typedef struct
{
  LPS22CH_IO_t        IO;
  stmdev_ctx_t        Ctx;
  uint8_t             is_initialized;
  uint8_t             press_is_enabled;
  uint8_t             temp_is_enabled;
  lps22ch_odr_t       last_odr;
} LPS22CH_Object_t;

typedef struct
{
  uint8_t Temperature;
  uint8_t Pressure;
  uint8_t Humidity;
  uint8_t LowPower;
  float   HumMaxOdr;
  float   TempMaxOdr;
  float   PressMaxOdr;
} LPS22CH_Capabilities_t;

typedef struct
{
  int32_t (*Init)(LPS22CH_Object_t *);
  int32_t (*DeInit)(LPS22CH_Object_t *);
  int32_t (*ReadID)(LPS22CH_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(LPS22CH_Object_t *, LPS22CH_Capabilities_t *);
} LPS22CH_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(LPS22CH_Object_t *);
  int32_t (*Disable)(LPS22CH_Object_t *);
  int32_t (*GetOutputDataRate)(LPS22CH_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LPS22CH_Object_t *, float);
  int32_t (*GetTemperature)(LPS22CH_Object_t *, float *);
} LPS22CH_TEMP_Drv_t;

typedef struct
{
  int32_t (*Enable)(LPS22CH_Object_t *);
  int32_t (*Disable)(LPS22CH_Object_t *);
  int32_t (*GetOutputDataRate)(LPS22CH_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LPS22CH_Object_t *, float);
  int32_t (*GetPressure)(LPS22CH_Object_t *, float *);
} LPS22CH_PRESS_Drv_t;

typedef enum
{
  LPS22CH_FIFO_BYPASS_MODE                    = (uint8_t)0x00,    /*!< The FIFO is disabled and empty. The pressure is read directly*/
  LPS22CH_FIFO_FIFO_MODE                      = (uint8_t)0x20,    /*!< Stops collecting data when full */
  LPS22CH_FIFO_STREAM_MODE                    = (uint8_t)0x40,    /*!< Keep the newest measurements in the FIFO*/
  LPS22CH_FIFO_TRIGGER_STREAMTOFIFO_MODE      = (uint8_t)0x60,    /*!< STREAM MODE until trigger deasserted, then change to FIFO MODE*/
  LPS22CH_FIFO_TRIGGER_BYPASSTOSTREAM_MODE    = (uint8_t)0x80,    /*!< BYPASS MODE until trigger deasserted, then STREAM MODE*/
  LPS22CH_FIFO_TRIGGER_BYPASSTOFIFO_MODE      = (uint8_t)0xE0     /*!< BYPASS mode until trigger deasserted, then FIFO MODE*/
} LPS22CH_FifoMode;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} lps22ch_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} lps22ch_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} lps22ch_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} lps22ch_axis1bit32_t;

/**
  * @}
  */

/** @defgroup LPS22CH_Exported_Constants LPS22CH Exported Constants
  * @{
  */

#define LPS22CH_OK                0
#define LPS22CH_ERROR            -1

#define LPS22CH_I2C_BUS          0U
#define LPS22CH_SPI_4WIRES_BUS   1U
#define LPS22CH_SPI_3WIRES_BUS   2U

#define LPS22CH_FIFO_FULL        (uint8_t)0x20

/** LPS22CH low noise mode  **/
#define LPS22CH_LOW_NOISE_DIS      0
#define LPS22CH_LOW_NOISE_EN       1

/**
  * @}
  */

/** @addtogroup LPS22CH_Exported_Functions LPS22CH Exported Functions
  * @{
  */

int32_t LPS22CH_RegisterBusIO(LPS22CH_Object_t *pObj, LPS22CH_IO_t *pIO);
int32_t LPS22CH_Init(LPS22CH_Object_t *pObj);
int32_t LPS22CH_DeInit(LPS22CH_Object_t *pObj);
int32_t LPS22CH_ReadID(LPS22CH_Object_t *pObj, uint8_t *Id);
int32_t LPS22CH_GetCapabilities(LPS22CH_Object_t *pObj, LPS22CH_Capabilities_t *Capabilities);
int32_t LPS22CH_Get_Init_Status(LPS22CH_Object_t *pObj, uint8_t *Status);

int32_t LPS22CH_PRESS_Enable(LPS22CH_Object_t *pObj);
int32_t LPS22CH_PRESS_Disable(LPS22CH_Object_t *pObj);
int32_t LPS22CH_PRESS_GetOutputDataRate(LPS22CH_Object_t *pObj, float *Odr);
int32_t LPS22CH_PRESS_SetOutputDataRate(LPS22CH_Object_t *pObj, float Odr);
int32_t LPS22CH_PRESS_GetPressure(LPS22CH_Object_t *pObj, float *Value);
int32_t LPS22CH_PRESS_Get_DRDY_Status(LPS22CH_Object_t *pObj, uint8_t *Status);

int32_t LPS22CH_TEMP_Enable(LPS22CH_Object_t *pObj);
int32_t LPS22CH_TEMP_Disable(LPS22CH_Object_t *pObj);
int32_t LPS22CH_TEMP_GetOutputDataRate(LPS22CH_Object_t *pObj, float *Odr);
int32_t LPS22CH_TEMP_SetOutputDataRate(LPS22CH_Object_t *pObj, float Odr);
int32_t LPS22CH_TEMP_GetTemperature(LPS22CH_Object_t *pObj, float *Value);
int32_t LPS22CH_TEMP_Get_DRDY_Status(LPS22CH_Object_t *pObj, uint8_t *Status);

int32_t LPS22CH_Read_Reg(LPS22CH_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t LPS22CH_Write_Reg(LPS22CH_Object_t *pObj, uint8_t reg, uint8_t Data);

int32_t LPS22CH_Get_Press(LPS22CH_Object_t *pObj, float *Data);
int32_t LPS22CH_Get_Temp(LPS22CH_Object_t *pObj, float *Data);

int32_t LPS22CH_FIFO_Get_Data(LPS22CH_Object_t *pObj, float *Press, float *Temp);
int32_t LPS22CH_FIFO_Get_FTh_Status(LPS22CH_Object_t *pObj, uint8_t *Status);
int32_t LPS22CH_FIFO_Get_Full_Status(LPS22CH_Object_t *pObj, uint8_t *Status);
int32_t LPS22CH_FIFO_Get_Ovr_Status(LPS22CH_Object_t *pObj, uint8_t *Status);
int32_t LPS22CH_FIFO_Get_Level(LPS22CH_Object_t *pObj, uint8_t *Status);
int32_t LPS22CH_FIFO_Reset_Interrupt(LPS22CH_Object_t *pObj, uint8_t interrupt);
int32_t LPS22CH_FIFO_Set_Interrupt(LPS22CH_Object_t *pObj, uint8_t interrupt);
int32_t LPS22CH_FIFO_Set_Mode(LPS22CH_Object_t *pObj, uint8_t Mode);
int32_t LPS22CH_FIFO_Set_Watermark_Level(LPS22CH_Object_t *pObj, uint8_t Watermark);
int32_t LPS22CH_FIFO_Stop_On_Watermark(LPS22CH_Object_t *pObj, uint8_t Stop);

int32_t LPS22CH_Set_One_Shot(LPS22CH_Object_t *pObj);
int32_t LPS22CH_Get_One_Shot_Status(LPS22CH_Object_t *pObj, uint8_t *Status);

int32_t LPS22CH_Enable_DRDY_Interrupt(LPS22CH_Object_t *pObj);
int32_t LPS22CH_Set_Power_Mode(LPS22CH_Object_t *pObj, uint8_t powerMode);
int32_t LPS22CH_Set_Filter_Mode(LPS22CH_Object_t *pObj, uint8_t filterMode);

/**
  * @}
  */

/** @addtogroup LPS22CH_Exported_Variables LPS22CH Exported Variables
  * @{
  */
extern LPS22CH_CommonDrv_t LPS22CH_COMMON_Driver;
extern LPS22CH_PRESS_Drv_t LPS22CH_PRESS_Driver;
extern LPS22CH_TEMP_Drv_t LPS22CH_TEMP_Driver;

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
