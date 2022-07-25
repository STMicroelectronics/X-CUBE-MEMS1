/**
  ******************************************************************************
  * @file    asm330lhh.h
  * @author  MEMS Software Solutions Team
  * @brief   ASM330LHH header driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ASM330LHH_H
#define ASM330LHH_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "asm330lhh_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup ASM330LHH ASM330LHH
  * @{
  */

/** @defgroup ASM330LHH_Exported_Types ASM330LHH Exported Types
  * @{
  */

typedef int32_t (*ASM330LHH_Init_Func)(void);
typedef int32_t (*ASM330LHH_DeInit_Func)(void);
typedef int32_t (*ASM330LHH_GetTick_Func)(void);
typedef int32_t (*ASM330LHH_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*ASM330LHH_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  ASM330LHH_INT1_PIN,
  ASM330LHH_INT2_PIN,
} ASM330LHH_SensorIntPin_t;

typedef struct
{
  ASM330LHH_Init_Func        Init;
  ASM330LHH_DeInit_Func      DeInit;
  uint32_t                   BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
  uint8_t                    Address;
  ASM330LHH_WriteReg_Func    WriteReg;
  ASM330LHH_ReadReg_Func     ReadReg;
  ASM330LHH_GetTick_Func     GetTick;
} ASM330LHH_IO_t;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} ASM330LHH_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} ASM330LHH_Axes_t;

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
} ASM330LHH_Event_Status_t;

typedef struct
{
  ASM330LHH_IO_t      IO;
  stmdev_ctx_t        Ctx;
  uint8_t             is_initialized;
  uint8_t             acc_is_enabled;
  uint8_t             gyro_is_enabled;
  asm330lhh_odr_xl_t  acc_odr;
  asm330lhh_odr_g_t   gyro_odr;
} ASM330LHH_Object_t;

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
} ASM330LHH_Capabilities_t;

typedef struct
{
  int32_t (*Init)(ASM330LHH_Object_t *);
  int32_t (*DeInit)(ASM330LHH_Object_t *);
  int32_t (*ReadID)(ASM330LHH_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(ASM330LHH_Object_t *, ASM330LHH_Capabilities_t *);
} ASM330LHH_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(ASM330LHH_Object_t *);
  int32_t (*Disable)(ASM330LHH_Object_t *);
  int32_t (*GetSensitivity)(ASM330LHH_Object_t *, float *);
  int32_t (*GetOutputDataRate)(ASM330LHH_Object_t *, float *);
  int32_t (*SetOutputDataRate)(ASM330LHH_Object_t *, float);
  int32_t (*GetFullScale)(ASM330LHH_Object_t *, int32_t *);
  int32_t (*SetFullScale)(ASM330LHH_Object_t *, int32_t);
  int32_t (*GetAxes)(ASM330LHH_Object_t *, ASM330LHH_Axes_t *);
  int32_t (*GetAxesRaw)(ASM330LHH_Object_t *, ASM330LHH_AxesRaw_t *);
} ASM330LHH_ACC_Drv_t;

typedef struct
{
  int32_t (*Enable)(ASM330LHH_Object_t *);
  int32_t (*Disable)(ASM330LHH_Object_t *);
  int32_t (*GetSensitivity)(ASM330LHH_Object_t *, float *);
  int32_t (*GetOutputDataRate)(ASM330LHH_Object_t *, float *);
  int32_t (*SetOutputDataRate)(ASM330LHH_Object_t *, float);
  int32_t (*GetFullScale)(ASM330LHH_Object_t *, int32_t *);
  int32_t (*SetFullScale)(ASM330LHH_Object_t *, int32_t);
  int32_t (*GetAxes)(ASM330LHH_Object_t *, ASM330LHH_Axes_t *);
  int32_t (*GetAxesRaw)(ASM330LHH_Object_t *, ASM330LHH_AxesRaw_t *);
} ASM330LHH_GYRO_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} asm330lhh_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} asm330lhh_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} asm330lhh_axis1bit32_t;

/**
  * @}
  */

/** @defgroup ASM330LHH_Exported_Constants ASM330LHH Exported Constants
  * @{
  */

#define ASM330LHH_OK                       0
#define ASM330LHH_ERROR                   -1

#define ASM330LHH_I2C_BUS                 0U
#define ASM330LHH_SPI_4WIRES_BUS          1U
#define ASM330LHH_SPI_3WIRES_BUS          2U

#define ASM330LHH_ACC_SENSITIVITY_FS_2G   0.061f
#define ASM330LHH_ACC_SENSITIVITY_FS_4G   0.122f
#define ASM330LHH_ACC_SENSITIVITY_FS_8G   0.244f
#define ASM330LHH_ACC_SENSITIVITY_FS_16G  0.488f

#define ASM330LHH_GYRO_SENSITIVITY_FS_125DPS    4.375f
#define ASM330LHH_GYRO_SENSITIVITY_FS_250DPS    8.750f
#define ASM330LHH_GYRO_SENSITIVITY_FS_500DPS   17.500f
#define ASM330LHH_GYRO_SENSITIVITY_FS_1000DPS  35.000f
#define ASM330LHH_GYRO_SENSITIVITY_FS_2000DPS  70.000f

/**
  * @}
  */

/** @addtogroup ASM330LHH_Exported_Functions ASM330LHH Exported Functions
  * @{
  */

int32_t ASM330LHH_RegisterBusIO(ASM330LHH_Object_t *pObj, ASM330LHH_IO_t *pIO);
int32_t ASM330LHH_Init(ASM330LHH_Object_t *pObj);
int32_t ASM330LHH_DeInit(ASM330LHH_Object_t *pObj);
int32_t ASM330LHH_ReadID(ASM330LHH_Object_t *pObj, uint8_t *Id);
int32_t ASM330LHH_GetCapabilities(ASM330LHH_Object_t *pObj, ASM330LHH_Capabilities_t *Capabilities);

int32_t ASM330LHH_ACC_Enable(ASM330LHH_Object_t *pObj);
int32_t ASM330LHH_ACC_Disable(ASM330LHH_Object_t *pObj);
int32_t ASM330LHH_ACC_GetSensitivity(ASM330LHH_Object_t *pObj, float *Sensitivity);
int32_t ASM330LHH_ACC_GetOutputDataRate(ASM330LHH_Object_t *pObj, float *Odr);
int32_t ASM330LHH_ACC_SetOutputDataRate(ASM330LHH_Object_t *pObj, float Odr);
int32_t ASM330LHH_ACC_GetFullScale(ASM330LHH_Object_t *pObj, int32_t *FullScale);
int32_t ASM330LHH_ACC_SetFullScale(ASM330LHH_Object_t *pObj, int32_t FullScale);
int32_t ASM330LHH_ACC_GetAxesRaw(ASM330LHH_Object_t *pObj, ASM330LHH_AxesRaw_t *Value);
int32_t ASM330LHH_ACC_GetAxes(ASM330LHH_Object_t *pObj, ASM330LHH_Axes_t *Acceleration);

int32_t ASM330LHH_GYRO_Enable(ASM330LHH_Object_t *pObj);
int32_t ASM330LHH_GYRO_Disable(ASM330LHH_Object_t *pObj);
int32_t ASM330LHH_GYRO_GetSensitivity(ASM330LHH_Object_t *pObj, float *Sensitivity);
int32_t ASM330LHH_GYRO_GetOutputDataRate(ASM330LHH_Object_t *pObj, float *Odr);
int32_t ASM330LHH_GYRO_SetOutputDataRate(ASM330LHH_Object_t *pObj, float Odr);
int32_t ASM330LHH_GYRO_GetFullScale(ASM330LHH_Object_t *pObj, int32_t *FullScale);
int32_t ASM330LHH_GYRO_SetFullScale(ASM330LHH_Object_t *pObj, int32_t FullScale);
int32_t ASM330LHH_GYRO_GetAxesRaw(ASM330LHH_Object_t *pObj, ASM330LHH_AxesRaw_t *Value);
int32_t ASM330LHH_GYRO_GetAxes(ASM330LHH_Object_t *pObj, ASM330LHH_Axes_t *AngularRate);

int32_t ASM330LHH_Read_Reg(ASM330LHH_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t ASM330LHH_Write_Reg(ASM330LHH_Object_t *pObj, uint8_t reg, uint8_t Data);

int32_t ASM330LHH_ACC_Get_DRDY_Status(ASM330LHH_Object_t *pObj, uint8_t *Status);

int32_t ASM330LHH_GYRO_Get_DRDY_Status(ASM330LHH_Object_t *pObj, uint8_t *Status);

/**
  * @}
  */

/** @addtogroup ASM330LHH_Exported_Variables ASM330LHH Exported Variables
  * @{
  */

extern ASM330LHH_CommonDrv_t ASM330LHH_COMMON_Driver;
extern ASM330LHH_ACC_Drv_t ASM330LHH_ACC_Driver;
extern ASM330LHH_GYRO_Drv_t ASM330LHH_GYRO_Driver;

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
