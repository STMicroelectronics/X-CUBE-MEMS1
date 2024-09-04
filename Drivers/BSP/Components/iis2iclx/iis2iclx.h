/**
  ******************************************************************************
  * @file    iis2iclx.h
  * @author  MEMS Software Solutions Team
  * @brief   IIS2ICLX header driver file
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
#ifndef IIS2ICLX_H
#define IIS2ICLX_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "iis2iclx_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup IIS2ICLX IIS2ICLX
  * @{
  */

/** @defgroup IIS2ICLX_Exported_Types IIS2ICLX Exported Types
  * @{
  */

typedef int32_t (*IIS2ICLX_Init_Func)(void);
typedef int32_t (*IIS2ICLX_DeInit_Func)(void);
typedef int32_t (*IIS2ICLX_GetTick_Func)(void);
typedef void    (*IIS2ICLX_Delay_Func)(uint32_t);
typedef int32_t (*IIS2ICLX_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*IIS2ICLX_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  IIS2ICLX_INT1_PIN,
  IIS2ICLX_INT2_PIN,
} IIS2ICLX_SensorIntPin_t;

typedef struct
{
  IIS2ICLX_Init_Func         Init;
  IIS2ICLX_DeInit_Func       DeInit;
  uint32_t                   BusType; /* 0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
  uint8_t                    Address;
  IIS2ICLX_WriteReg_Func     WriteReg;
  IIS2ICLX_ReadReg_Func      ReadReg;
  IIS2ICLX_GetTick_Func      GetTick;
  IIS2ICLX_Delay_Func        Delay;
} IIS2ICLX_IO_t;

typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} IIS2ICLX_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} IIS2ICLX_Axes_t;

typedef struct
{
  unsigned int FreeFallStatus : 1;
  unsigned int TapStatus : 1;
  unsigned int DoubleTapStatus : 1;
  unsigned int WakeUpStatus : 1;
  unsigned int StepStatus : 1;
  unsigned int TiltStatus : 1;
  unsigned int D6DOrientationStatus : 1;
  unsigned int SleepStatus : 1;
} IIS2ICLX_Event_Status_t;

typedef struct
{
  IIS2ICLX_IO_t       IO;
  stmdev_ctx_t        Ctx;
  uint8_t             is_initialized;
  uint8_t             acc_is_enabled;
  iis2iclx_odr_xl_t   acc_odr;
} IIS2ICLX_Object_t;

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
} IIS2ICLX_Capabilities_t;

typedef struct
{
  int32_t (*Init)(IIS2ICLX_Object_t *);
  int32_t (*DeInit)(IIS2ICLX_Object_t *);
  int32_t (*ReadID)(IIS2ICLX_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(IIS2ICLX_Object_t *, IIS2ICLX_Capabilities_t *);
} IIS2ICLX_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(IIS2ICLX_Object_t *);
  int32_t (*Disable)(IIS2ICLX_Object_t *);
  int32_t (*GetSensitivity)(IIS2ICLX_Object_t *, float *);
  int32_t (*GetOutputDataRate)(IIS2ICLX_Object_t *, float *);
  int32_t (*SetOutputDataRate)(IIS2ICLX_Object_t *, float);
  int32_t (*GetFullScale)(IIS2ICLX_Object_t *, int32_t *);
  int32_t (*SetFullScale)(IIS2ICLX_Object_t *, int32_t);
  int32_t (*GetAxes)(IIS2ICLX_Object_t *, IIS2ICLX_Axes_t *);
  int32_t (*GetAxesRaw)(IIS2ICLX_Object_t *, IIS2ICLX_AxesRaw_t *);
} IIS2ICLX_ACC_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} iis2iclx_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} iis2iclx_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} iis2iclx_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} iis2iclx_axis1bit32_t;

/**
  * @}
  */

/** @defgroup IIS2ICLX_Exported_Constants IIS2ICLX Exported Constants
  * @{
  */

#define IIS2ICLX_OK                       0
#define IIS2ICLX_ERROR                   -1

#define IIS2ICLX_I2C_BUS                 0U
#define IIS2ICLX_SPI_4WIRES_BUS          1U
#define IIS2ICLX_SPI_3WIRES_BUS          2U

#define IIS2ICLX_ACC_SENSITIVITY_FS_0G5  0.015f
#define IIS2ICLX_ACC_SENSITIVITY_FS_1G   0.031f
#define IIS2ICLX_ACC_SENSITIVITY_FS_2G   0.061f
#define IIS2ICLX_ACC_SENSITIVITY_FS_3G   0.122f

/**
  * @}
  */

/** @addtogroup IIS2ICLX_Exported_Functions IIS2ICLX Exported Functions
  * @{
  */

int32_t IIS2ICLX_RegisterBusIO(IIS2ICLX_Object_t *pObj, IIS2ICLX_IO_t *pIO);
int32_t IIS2ICLX_Init(IIS2ICLX_Object_t *pObj);
int32_t IIS2ICLX_DeInit(IIS2ICLX_Object_t *pObj);
int32_t IIS2ICLX_ReadID(IIS2ICLX_Object_t *pObj, uint8_t *Id);
int32_t IIS2ICLX_GetCapabilities(IIS2ICLX_Object_t *pObj, IIS2ICLX_Capabilities_t *Capabilities);

int32_t IIS2ICLX_ACC_Enable(IIS2ICLX_Object_t *pObj);
int32_t IIS2ICLX_ACC_Disable(IIS2ICLX_Object_t *pObj);
int32_t IIS2ICLX_ACC_GetSensitivity(IIS2ICLX_Object_t *pObj, float *Sensitivity);
int32_t IIS2ICLX_ACC_GetOutputDataRate(IIS2ICLX_Object_t *pObj, float *Odr);
int32_t IIS2ICLX_ACC_SetOutputDataRate(IIS2ICLX_Object_t *pObj, float Odr);
int32_t IIS2ICLX_ACC_GetFullScale(IIS2ICLX_Object_t *pObj, int32_t *FullScale);
int32_t IIS2ICLX_ACC_SetFullScale(IIS2ICLX_Object_t *pObj, int32_t FullScale);
int32_t IIS2ICLX_ACC_GetAxesRaw(IIS2ICLX_Object_t *pObj, IIS2ICLX_AxesRaw_t *Value);
int32_t IIS2ICLX_ACC_GetAxes(IIS2ICLX_Object_t *pObj, IIS2ICLX_Axes_t *Acceleration);

int32_t IIS2ICLX_Read_Reg(IIS2ICLX_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t IIS2ICLX_Write_Reg(IIS2ICLX_Object_t *pObj, uint8_t reg, uint8_t Data);
int32_t IIS2ICLX_Set_Interrupt_Latch(IIS2ICLX_Object_t *pObj, uint8_t Status);

int32_t IIS2ICLX_ACC_Enable_Wake_Up_Detection(IIS2ICLX_Object_t *pObj, IIS2ICLX_SensorIntPin_t IntPin);
int32_t IIS2ICLX_ACC_Disable_Wake_Up_Detection(IIS2ICLX_Object_t *pObj);
int32_t IIS2ICLX_ACC_Set_Wake_Up_Threshold(IIS2ICLX_Object_t *pObj, uint8_t Threshold);
int32_t IIS2ICLX_ACC_Set_Wake_Up_Duration(IIS2ICLX_Object_t *pObj, uint8_t Duration);

int32_t IIS2ICLX_ACC_Enable_Single_Tap_Detection(IIS2ICLX_Object_t *pObj, IIS2ICLX_SensorIntPin_t IntPin);
int32_t IIS2ICLX_ACC_Disable_Single_Tap_Detection(IIS2ICLX_Object_t *pObj);
int32_t IIS2ICLX_ACC_Enable_Double_Tap_Detection(IIS2ICLX_Object_t *pObj, IIS2ICLX_SensorIntPin_t IntPin);
int32_t IIS2ICLX_ACC_Disable_Double_Tap_Detection(IIS2ICLX_Object_t *pObj);
int32_t IIS2ICLX_ACC_Set_Tap_Threshold(IIS2ICLX_Object_t *pObj, uint8_t Threshold);
int32_t IIS2ICLX_ACC_Set_Tap_Shock_Time(IIS2ICLX_Object_t *pObj, uint8_t Time);
int32_t IIS2ICLX_ACC_Set_Tap_Quiet_Time(IIS2ICLX_Object_t *pObj, uint8_t Time);
int32_t IIS2ICLX_ACC_Set_Tap_Duration_Time(IIS2ICLX_Object_t *pObj, uint8_t Time);

int32_t IIS2ICLX_ACC_Get_DRDY_Status(IIS2ICLX_Object_t *pObj, uint8_t *Status);
int32_t IIS2ICLX_ACC_Get_Event_Status(IIS2ICLX_Object_t *pObj, IIS2ICLX_Event_Status_t *Status);
int32_t IIS2ICLX_ACC_Set_SelfTest(IIS2ICLX_Object_t *pObj, uint8_t Status);

int32_t IIS2ICLX_FIFO_Get_Num_Samples(IIS2ICLX_Object_t *pObj, uint16_t *NumSamples);
int32_t IIS2ICLX_FIFO_Get_Full_Status(IIS2ICLX_Object_t *pObj, uint8_t *Status);
int32_t IIS2ICLX_FIFO_Set_INT1_FIFO_Full(IIS2ICLX_Object_t *pObj, uint8_t Status);
int32_t IIS2ICLX_FIFO_Set_Watermark_Level(IIS2ICLX_Object_t *pObj, uint16_t Watermark);
int32_t IIS2ICLX_FIFO_Set_Stop_On_Fth(IIS2ICLX_Object_t *pObj, uint8_t Status);
int32_t IIS2ICLX_FIFO_Set_Mode(IIS2ICLX_Object_t *pObj, uint8_t Mode);
int32_t IIS2ICLX_FIFO_Get_Tag(IIS2ICLX_Object_t *pObj, uint8_t *Tag);
int32_t IIS2ICLX_FIFO_Get_Data(IIS2ICLX_Object_t *pObj, uint8_t *Data);
int32_t IIS2ICLX_FIFO_Get_Empty_Status(IIS2ICLX_Object_t *pObj, uint8_t *Status);
int32_t IIS2ICLX_FIFO_Get_Overrun_Status(IIS2ICLX_Object_t *pObj, uint8_t *Status);
int32_t IIS2ICLX_FIFO_ACC_Get_Axes(IIS2ICLX_Object_t *pObj, IIS2ICLX_Axes_t *Acceleration);
int32_t IIS2ICLX_FIFO_ACC_Set_BDR(IIS2ICLX_Object_t *pObj, float Bdr);

int32_t IIS2ICLX_ACC_Enable_DRDY_Interrupt(IIS2ICLX_Object_t *pObj);

int32_t IIS2ICLX_ACC_Set_Filter_Mode(IIS2ICLX_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode);
int32_t IIS2ICLX_ACC_Set_Sleep_Duration(IIS2ICLX_Object_t *pObj, uint8_t Duration);

int32_t IIS2ICLX_Set_Mem_Bank(IIS2ICLX_Object_t *pObj, uint8_t Val);

/**
  * @}
  */

/** @addtogroup IIS2ICLX_Exported_Variables IIS2ICLX Exported Variables
  * @{
  */

extern IIS2ICLX_CommonDrv_t IIS2ICLX_COMMON_Driver;
extern IIS2ICLX_ACC_Drv_t IIS2ICLX_ACC_Driver;

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
