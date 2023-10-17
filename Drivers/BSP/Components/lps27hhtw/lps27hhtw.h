/**
  ******************************************************************************
  * @file    lps27hhtw.h
  * @author  MEMS Software Solutions Team
  * @brief   LPS27HHTW header driver file
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
#ifndef LPS27HHTW_H
#define LPS27HHTW_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "lps27hhtw_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup LPS27HHTW LPS27HHTW
  * @{
  */

/** @defgroup LPS27HHTW_Exported_Types LPS27HHTW Exported Types
  * @{
  */

typedef int32_t (*LPS27HHTW_Init_Func)(void);
typedef int32_t (*LPS27HHTW_DeInit_Func)(void);
typedef int32_t (*LPS27HHTW_GetTick_Func)(void);
typedef void    (*LPS27HHTW_Delay_Func)(uint32_t);
typedef int32_t (*LPS27HHTW_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*LPS27HHTW_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef struct
{
  LPS27HHTW_Init_Func        Init;
  LPS27HHTW_DeInit_Func      DeInit;
  uint32_t                   BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires, 3 means I3C */
  uint8_t                    Address;
  LPS27HHTW_WriteReg_Func    WriteReg;
  LPS27HHTW_ReadReg_Func     ReadReg;
  LPS27HHTW_GetTick_Func     GetTick;
  LPS27HHTW_Delay_Func       Delay;
} LPS27HHTW_IO_t;

typedef struct
{
  LPS27HHTW_IO_t      IO;
  stmdev_ctx_t        Ctx;
  uint8_t             is_initialized;
  uint8_t             press_is_enabled;
  uint8_t             temp_is_enabled;
  lps27hhtw_odr_t     last_odr;
} LPS27HHTW_Object_t;

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
} LPS27HHTW_Capabilities_t;

typedef struct
{
  int32_t (*Init)(LPS27HHTW_Object_t *);
  int32_t (*DeInit)(LPS27HHTW_Object_t *);
  int32_t (*ReadID)(LPS27HHTW_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(LPS27HHTW_Object_t *, LPS27HHTW_Capabilities_t *);
} LPS27HHTW_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(LPS27HHTW_Object_t *);
  int32_t (*Disable)(LPS27HHTW_Object_t *);
  int32_t (*GetOutputDataRate)(LPS27HHTW_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LPS27HHTW_Object_t *, float);
  int32_t (*GetTemperature)(LPS27HHTW_Object_t *, float *);
} LPS27HHTW_TEMP_Drv_t;

typedef struct
{
  int32_t (*Enable)(LPS27HHTW_Object_t *);
  int32_t (*Disable)(LPS27HHTW_Object_t *);
  int32_t (*GetOutputDataRate)(LPS27HHTW_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LPS27HHTW_Object_t *, float);
  int32_t (*GetPressure)(LPS27HHTW_Object_t *, float *);
} LPS27HHTW_PRESS_Drv_t;

typedef enum
{
  LPS27HHTW_FIFO_BYPASS_MODE                    = (uint8_t)0x00,    /*!< The FIFO is disabled and empty. The pressure is read directly*/
  LPS27HHTW_FIFO_FIFO_MODE                      = (uint8_t)0x20,    /*!< Stops collecting data when full */
  LPS27HHTW_FIFO_STREAM_MODE                    = (uint8_t)0x40,    /*!< Keep the newest measurements in the FIFO*/
  LPS27HHTW_FIFO_TRIGGER_STREAMTOFIFO_MODE      = (uint8_t)0x60,    /*!< STREAM MODE until trigger deasserted, then change to FIFO MODE*/
  LPS27HHTW_FIFO_TRIGGER_BYPASSTOSTREAM_MODE    = (uint8_t)0x80,    /*!< BYPASS MODE until trigger deasserted, then STREAM MODE*/
  LPS27HHTW_FIFO_TRIGGER_BYPASSTOFIFO_MODE      = (uint8_t)0xE0     /*!< BYPASS mode until trigger deasserted, then FIFO MODE*/
} LPS27HHTW_FifoMode;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} lps27hhtw_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} lps27hhtw_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} lps27hhtw_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} lps27hhtw_axis1bit32_t;

/**
  * @}
  */

/** @defgroup LPS27HHTW_Exported_Constants LPS27HHTW Exported Constants
  * @{
  */

#define LPS27HHTW_OK                0
#define LPS27HHTW_ERROR            -1

#define LPS27HHTW_I2C_BUS          0U
#define LPS27HHTW_SPI_4WIRES_BUS   1U
#define LPS27HHTW_SPI_3WIRES_BUS   2U
#define LPS27HHTW_I3C_BUS          3U

#define LPS27HHTW_FIFO_FULL        (uint8_t)0x20

/** LPS27HHTW low noise mode  **/
#define LPS27HHTW_LOW_NOISE_DIS      0
#define LPS27HHTW_LOW_NOISE_EN       1

/**
  * @}
  */

/** @addtogroup LPS27HHTW_Exported_Functions LPS27HHTW Exported Functions
  * @{
  */

int32_t LPS27HHTW_RegisterBusIO(LPS27HHTW_Object_t *pObj, LPS27HHTW_IO_t *pIO);
int32_t LPS27HHTW_Init(LPS27HHTW_Object_t *pObj);
int32_t LPS27HHTW_DeInit(LPS27HHTW_Object_t *pObj);
int32_t LPS27HHTW_ReadID(LPS27HHTW_Object_t *pObj, uint8_t *Id);
int32_t LPS27HHTW_GetCapabilities(LPS27HHTW_Object_t *pObj, LPS27HHTW_Capabilities_t *Capabilities);
int32_t LPS27HHTW_Get_Init_Status(LPS27HHTW_Object_t *pObj, uint8_t *Status);

int32_t LPS27HHTW_PRESS_Enable(LPS27HHTW_Object_t *pObj);
int32_t LPS27HHTW_PRESS_Disable(LPS27HHTW_Object_t *pObj);
int32_t LPS27HHTW_PRESS_GetOutputDataRate(LPS27HHTW_Object_t *pObj, float *Odr);
int32_t LPS27HHTW_PRESS_SetOutputDataRate(LPS27HHTW_Object_t *pObj, float Odr);
int32_t LPS27HHTW_PRESS_GetPressure(LPS27HHTW_Object_t *pObj, float *Value);
int32_t LPS27HHTW_PRESS_Get_DRDY_Status(LPS27HHTW_Object_t *pObj, uint8_t *Status);

int32_t LPS27HHTW_TEMP_Enable(LPS27HHTW_Object_t *pObj);
int32_t LPS27HHTW_TEMP_Disable(LPS27HHTW_Object_t *pObj);
int32_t LPS27HHTW_TEMP_GetOutputDataRate(LPS27HHTW_Object_t *pObj, float *Odr);
int32_t LPS27HHTW_TEMP_SetOutputDataRate(LPS27HHTW_Object_t *pObj, float Odr);
int32_t LPS27HHTW_TEMP_GetTemperature(LPS27HHTW_Object_t *pObj, float *Value);
int32_t LPS27HHTW_TEMP_Get_DRDY_Status(LPS27HHTW_Object_t *pObj, uint8_t *Status);

int32_t LPS27HHTW_Read_Reg(LPS27HHTW_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t LPS27HHTW_Write_Reg(LPS27HHTW_Object_t *pObj, uint8_t reg, uint8_t Data);

int32_t LPS27HHTW_Get_Press(LPS27HHTW_Object_t *pObj, float *Data);
int32_t LPS27HHTW_Get_Temp(LPS27HHTW_Object_t *pObj, float *Data);

int32_t LPS27HHTW_FIFO_Get_Data(LPS27HHTW_Object_t *pObj, float *Press, float *Temp);
int32_t LPS27HHTW_FIFO_Get_FTh_Status(LPS27HHTW_Object_t *pObj, uint8_t *Status);
int32_t LPS27HHTW_FIFO_Get_Full_Status(LPS27HHTW_Object_t *pObj, uint8_t *Status);
int32_t LPS27HHTW_FIFO_Get_Ovr_Status(LPS27HHTW_Object_t *pObj, uint8_t *Status);
int32_t LPS27HHTW_FIFO_Get_Level(LPS27HHTW_Object_t *pObj, uint8_t *Status);
int32_t LPS27HHTW_FIFO_Reset_Interrupt(LPS27HHTW_Object_t *pObj, uint8_t interrupt);
int32_t LPS27HHTW_FIFO_Set_Interrupt(LPS27HHTW_Object_t *pObj, uint8_t interrupt);
int32_t LPS27HHTW_FIFO_Set_Mode(LPS27HHTW_Object_t *pObj, uint8_t Mode);
int32_t LPS27HHTW_FIFO_Set_Watermark_Level(LPS27HHTW_Object_t *pObj, uint8_t Watermark);
int32_t LPS27HHTW_FIFO_Stop_On_Watermark(LPS27HHTW_Object_t *pObj, uint8_t Stop);

int32_t LPS27HHTW_Set_One_Shot(LPS27HHTW_Object_t *pObj);
int32_t LPS27HHTW_Get_One_Shot_Status(LPS27HHTW_Object_t *pObj, uint8_t *Status);

int32_t LPS27HHTW_Enable_DRDY_Interrupt(LPS27HHTW_Object_t *pObj);
int32_t LPS27HHTW_Set_Power_Mode(LPS27HHTW_Object_t *pObj, uint8_t powerMode);
int32_t LPS27HHTW_Set_Filter_Mode(LPS27HHTW_Object_t *pObj, uint8_t filterMode);

/**
  * @}
  */

/** @addtogroup LPS27HHTW_Exported_Variables LPS27HHTW Exported Variables
  * @{
  */
extern LPS27HHTW_CommonDrv_t LPS27HHTW_COMMON_Driver;
extern LPS27HHTW_PRESS_Drv_t LPS27HHTW_PRESS_Driver;
extern LPS27HHTW_TEMP_Drv_t LPS27HHTW_TEMP_Driver;

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
