/**
  ******************************************************************************
  * @file    asm330lhhx.h
  * @author  MEMS Software Solutions Team
  * @brief   ASM330LHHX header driver file
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
#ifndef ASM330LHHX_H
#define ASM330LHHX_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "asm330lhhx_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup ASM330LHHX ASM330LHHX
  * @{
  */

/** @defgroup ASM330LHHX_Exported_Types ASM330LHHX Exported Types
  * @{
  */

typedef int32_t (*ASM330LHHX_Init_Func)(void);
typedef int32_t (*ASM330LHHX_DeInit_Func)(void);
typedef int32_t (*ASM330LHHX_GetTick_Func)(void);
typedef void    (*ASM330LHHX_Delay_Func)(uint32_t);
typedef int32_t (*ASM330LHHX_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*ASM330LHHX_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  ASM330LHHX_INT1_PIN,
  ASM330LHHX_INT2_PIN,
} ASM330LHHX_SensorIntPin_t;

typedef struct
{
  ASM330LHHX_Init_Func       Init;
  ASM330LHHX_DeInit_Func     DeInit;
  uint32_t                   BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires, 3 means I3C */
  uint8_t                    Address;
  ASM330LHHX_WriteReg_Func   WriteReg;
  ASM330LHHX_ReadReg_Func    ReadReg;
  ASM330LHHX_GetTick_Func    GetTick;
  ASM330LHHX_Delay_Func      Delay;
} ASM330LHHX_IO_t;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} ASM330LHHX_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} ASM330LHHX_Axes_t;

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
} ASM330LHHX_Event_Status_t;

typedef struct
{
  ASM330LHHX_IO_t     IO;
  stmdev_ctx_t        Ctx;
  uint8_t             is_initialized;
  uint8_t             acc_is_enabled;
  uint8_t             gyro_is_enabled;
  asm330lhhx_odr_xl_t acc_odr;
  asm330lhhx_odr_g_t  gyro_odr;
} ASM330LHHX_Object_t;

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
} ASM330LHHX_Capabilities_t;

typedef struct
{
  int32_t (*Init)(ASM330LHHX_Object_t *);
  int32_t (*DeInit)(ASM330LHHX_Object_t *);
  int32_t (*ReadID)(ASM330LHHX_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(ASM330LHHX_Object_t *, ASM330LHHX_Capabilities_t *);
} ASM330LHHX_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(ASM330LHHX_Object_t *);
  int32_t (*Disable)(ASM330LHHX_Object_t *);
  int32_t (*GetSensitivity)(ASM330LHHX_Object_t *, float *);
  int32_t (*GetOutputDataRate)(ASM330LHHX_Object_t *, float *);
  int32_t (*SetOutputDataRate)(ASM330LHHX_Object_t *, float);
  int32_t (*GetFullScale)(ASM330LHHX_Object_t *, int32_t *);
  int32_t (*SetFullScale)(ASM330LHHX_Object_t *, int32_t);
  int32_t (*GetAxes)(ASM330LHHX_Object_t *, ASM330LHHX_Axes_t *);
  int32_t (*GetAxesRaw)(ASM330LHHX_Object_t *, ASM330LHHX_AxesRaw_t *);
} ASM330LHHX_ACC_Drv_t;

typedef struct
{
  int32_t (*Enable)(ASM330LHHX_Object_t *);
  int32_t (*Disable)(ASM330LHHX_Object_t *);
  int32_t (*GetSensitivity)(ASM330LHHX_Object_t *, float *);
  int32_t (*GetOutputDataRate)(ASM330LHHX_Object_t *, float *);
  int32_t (*SetOutputDataRate)(ASM330LHHX_Object_t *, float);
  int32_t (*GetFullScale)(ASM330LHHX_Object_t *, int32_t *);
  int32_t (*SetFullScale)(ASM330LHHX_Object_t *, int32_t);
  int32_t (*GetAxes)(ASM330LHHX_Object_t *, ASM330LHHX_Axes_t *);
  int32_t (*GetAxesRaw)(ASM330LHHX_Object_t *, ASM330LHHX_AxesRaw_t *);
} ASM330LHHX_GYRO_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} asm330lhhx_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} asm330lhhx_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} asm330lhhx_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} asm330lhhx_axis1bit32_t;

/**
  * @}
  */

/** @defgroup ASM330LHHX_Exported_Constants ASM330LHHX Exported Constants
  * @{
  */

#define ASM330LHHX_OK                       0
#define ASM330LHHX_ERROR                   -1

#define ASM330LHHX_I2C_BUS                 0U
#define ASM330LHHX_SPI_4WIRES_BUS          1U
#define ASM330LHHX_SPI_3WIRES_BUS          2U
#define ASM330LHHX_I3C_BUS                 3U

#define ASM330LHHX_ACC_SENSITIVITY_FS_2G   0.061f
#define ASM330LHHX_ACC_SENSITIVITY_FS_4G   0.122f
#define ASM330LHHX_ACC_SENSITIVITY_FS_8G   0.244f
#define ASM330LHHX_ACC_SENSITIVITY_FS_16G  0.488f

#define ASM330LHHX_GYRO_SENSITIVITY_FS_125DPS     4.375f
#define ASM330LHHX_GYRO_SENSITIVITY_FS_250DPS     8.750f
#define ASM330LHHX_GYRO_SENSITIVITY_FS_500DPS    17.500f
#define ASM330LHHX_GYRO_SENSITIVITY_FS_1000DPS   35.000f
#define ASM330LHHX_GYRO_SENSITIVITY_FS_2000DPS   70.000f
#define ASM330LHHX_GYRO_SENSITIVITY_FS_4000DPS  140.000f

/**
  * @}
  */

/** @addtogroup ASM330LHHX_Exported_Functions ASM330LHHX Exported Functions
  * @{
  */

int32_t ASM330LHHX_RegisterBusIO(ASM330LHHX_Object_t *pObj, ASM330LHHX_IO_t *pIO);
int32_t ASM330LHHX_Init(ASM330LHHX_Object_t *pObj);
int32_t ASM330LHHX_DeInit(ASM330LHHX_Object_t *pObj);
int32_t ASM330LHHX_ReadID(ASM330LHHX_Object_t *pObj, uint8_t *Id);
int32_t ASM330LHHX_GetCapabilities(ASM330LHHX_Object_t *pObj, ASM330LHHX_Capabilities_t *Capabilities);

int32_t ASM330LHHX_ACC_Enable(ASM330LHHX_Object_t *pObj);
int32_t ASM330LHHX_ACC_Disable(ASM330LHHX_Object_t *pObj);
int32_t ASM330LHHX_ACC_GetSensitivity(ASM330LHHX_Object_t *pObj, float *Sensitivity);
int32_t ASM330LHHX_ACC_GetOutputDataRate(ASM330LHHX_Object_t *pObj, float *Odr);
int32_t ASM330LHHX_ACC_SetOutputDataRate(ASM330LHHX_Object_t *pObj, float Odr);
int32_t ASM330LHHX_ACC_GetFullScale(ASM330LHHX_Object_t *pObj, int32_t *FullScale);
int32_t ASM330LHHX_ACC_SetFullScale(ASM330LHHX_Object_t *pObj, int32_t FullScale);
int32_t ASM330LHHX_ACC_GetAxesRaw(ASM330LHHX_Object_t *pObj, ASM330LHHX_AxesRaw_t *Value);
int32_t ASM330LHHX_ACC_GetAxes(ASM330LHHX_Object_t *pObj, ASM330LHHX_Axes_t *Acceleration);

int32_t ASM330LHHX_GYRO_Enable(ASM330LHHX_Object_t *pObj);
int32_t ASM330LHHX_GYRO_Disable(ASM330LHHX_Object_t *pObj);
int32_t ASM330LHHX_GYRO_GetSensitivity(ASM330LHHX_Object_t *pObj, float *Sensitivity);
int32_t ASM330LHHX_GYRO_GetOutputDataRate(ASM330LHHX_Object_t *pObj, float *Odr);
int32_t ASM330LHHX_GYRO_SetOutputDataRate(ASM330LHHX_Object_t *pObj, float Odr);
int32_t ASM330LHHX_GYRO_GetFullScale(ASM330LHHX_Object_t *pObj, int32_t *FullScale);
int32_t ASM330LHHX_GYRO_SetFullScale(ASM330LHHX_Object_t *pObj, int32_t FullScale);
int32_t ASM330LHHX_GYRO_GetAxesRaw(ASM330LHHX_Object_t *pObj, ASM330LHHX_AxesRaw_t *Value);
int32_t ASM330LHHX_GYRO_GetAxes(ASM330LHHX_Object_t *pObj, ASM330LHHX_Axes_t *AngularRate);

int32_t ASM330LHHX_Read_Reg(ASM330LHHX_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t ASM330LHHX_Write_Reg(ASM330LHHX_Object_t *pObj, uint8_t reg, uint8_t Data);
int32_t ASM330LHHX_Set_Interrupt_Latch(ASM330LHHX_Object_t *pObj, uint8_t Status);

int32_t ASM330LHHX_ACC_Enable_Free_Fall_Detection(ASM330LHHX_Object_t *pObj, ASM330LHHX_SensorIntPin_t IntPin);
int32_t ASM330LHHX_ACC_Disable_Free_Fall_Detection(ASM330LHHX_Object_t *pObj);
int32_t ASM330LHHX_ACC_Set_Free_Fall_Threshold(ASM330LHHX_Object_t *pObj, uint8_t Threshold);
int32_t ASM330LHHX_ACC_Set_Free_Fall_Duration(ASM330LHHX_Object_t *pObj, uint8_t Duration);

int32_t ASM330LHHX_ACC_Enable_Wake_Up_Detection(ASM330LHHX_Object_t *pObj, ASM330LHHX_SensorIntPin_t IntPin);
int32_t ASM330LHHX_ACC_Disable_Wake_Up_Detection(ASM330LHHX_Object_t *pObj);
int32_t ASM330LHHX_ACC_Set_Wake_Up_Threshold(ASM330LHHX_Object_t *pObj, uint8_t Threshold);
int32_t ASM330LHHX_ACC_Set_Wake_Up_Duration(ASM330LHHX_Object_t *pObj, uint8_t Duration);

int32_t ASM330LHHX_ACC_Enable_6D_Orientation(ASM330LHHX_Object_t *pObj, ASM330LHHX_SensorIntPin_t IntPin);
int32_t ASM330LHHX_ACC_Disable_6D_Orientation(ASM330LHHX_Object_t *pObj);
int32_t ASM330LHHX_ACC_Set_6D_Orientation_Threshold(ASM330LHHX_Object_t *pObj, uint8_t Threshold);
int32_t ASM330LHHX_ACC_Get_6D_Orientation_XL(ASM330LHHX_Object_t *pObj, uint8_t *XLow);
int32_t ASM330LHHX_ACC_Get_6D_Orientation_XH(ASM330LHHX_Object_t *pObj, uint8_t *XHigh);
int32_t ASM330LHHX_ACC_Get_6D_Orientation_YL(ASM330LHHX_Object_t *pObj, uint8_t *YLow);
int32_t ASM330LHHX_ACC_Get_6D_Orientation_YH(ASM330LHHX_Object_t *pObj, uint8_t *YHigh);
int32_t ASM330LHHX_ACC_Get_6D_Orientation_ZL(ASM330LHHX_Object_t *pObj, uint8_t *ZLow);
int32_t ASM330LHHX_ACC_Get_6D_Orientation_ZH(ASM330LHHX_Object_t *pObj, uint8_t *ZHigh);

int32_t ASM330LHHX_ACC_Get_DRDY_Status(ASM330LHHX_Object_t *pObj, uint8_t *Status);
int32_t ASM330LHHX_ACC_Get_Event_Status(ASM330LHHX_Object_t *pObj, ASM330LHHX_Event_Status_t *Status);
int32_t ASM330LHHX_ACC_Set_SelfTest(ASM330LHHX_Object_t *pObj, uint8_t Status);

int32_t ASM330LHHX_GYRO_Get_DRDY_Status(ASM330LHHX_Object_t *pObj, uint8_t *Status);
int32_t ASM330LHHX_GYRO_Set_SelfTest(ASM330LHHX_Object_t *pObj, uint8_t Status);

int32_t ASM330LHHX_FIFO_Get_Num_Samples(ASM330LHHX_Object_t *pObj, uint16_t *NumSamples);
int32_t ASM330LHHX_FIFO_Get_Full_Status(ASM330LHHX_Object_t *pObj, uint8_t *Status);
int32_t ASM330LHHX_FIFO_Set_INT1_FIFO_Full(ASM330LHHX_Object_t *pObj, uint8_t Status);
int32_t ASM330LHHX_FIFO_Set_Watermark_Level(ASM330LHHX_Object_t *pObj, uint16_t Watermark);
int32_t ASM330LHHX_FIFO_Set_Stop_On_Fth(ASM330LHHX_Object_t *pObj, uint8_t Status);
int32_t ASM330LHHX_FIFO_Set_Mode(ASM330LHHX_Object_t *pObj, uint8_t Mode);
int32_t ASM330LHHX_FIFO_Get_Tag(ASM330LHHX_Object_t *pObj, uint8_t *Tag);
int32_t ASM330LHHX_FIFO_Get_Data(ASM330LHHX_Object_t *pObj, uint8_t *Data);
int32_t ASM330LHHX_FIFO_Get_Empty_Status(ASM330LHHX_Object_t *pObj, uint8_t *Status);
int32_t ASM330LHHX_FIFO_Get_Overrun_Status(ASM330LHHX_Object_t *pObj, uint8_t *Status);
int32_t ASM330LHHX_FIFO_ACC_Get_Axes(ASM330LHHX_Object_t *pObj, ASM330LHHX_Axes_t *Acceleration);
int32_t ASM330LHHX_FIFO_ACC_Set_BDR(ASM330LHHX_Object_t *pObj, float Bdr);
int32_t ASM330LHHX_FIFO_GYRO_Get_Axes(ASM330LHHX_Object_t *pObj, ASM330LHHX_Axes_t *AngularVelocity);
int32_t ASM330LHHX_FIFO_GYRO_Set_BDR(ASM330LHHX_Object_t *pObj, float Bdr);

/**
  * @}
  */

/** @addtogroup ASM330LHHX_Exported_Variables ASM330LHHX Exported Variables
  * @{
  */

extern ASM330LHHX_CommonDrv_t ASM330LHHX_COMMON_Driver;
extern ASM330LHHX_ACC_Drv_t ASM330LHHX_ACC_Driver;
extern ASM330LHHX_GYRO_Drv_t ASM330LHHX_GYRO_Driver;

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
