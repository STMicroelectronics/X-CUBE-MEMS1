/**
  ******************************************************************************
  * @file    a3g4250d.h
  * @author  MEMS Software Solutions Team
  * @brief   A3G4250D header driver file
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
#ifndef A3G4250D_H
#define A3G4250D_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "a3g4250d_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup A3G4250D A3G4250D
  * @{
  */

/** @defgroup A3G4250D_Exported_Types A3G4250D Exported Types
  * @{
  */

typedef int32_t (*A3G4250D_Init_Func)(void);
typedef int32_t (*A3G4250D_DeInit_Func)(void);
typedef int32_t (*A3G4250D_GetTick_Func)(void);
typedef void    (*A3G4250D_Delay_Func)(uint32_t);
typedef int32_t (*A3G4250D_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*A3G4250D_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  A3G4250D_INT1_PIN,
  A3G4250D_INT2_PIN,
} A3G4250D_SensorIntPin_t;

typedef struct
{
  A3G4250D_Init_Func        Init;
  A3G4250D_DeInit_Func      DeInit;
  uint32_t                  BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
  uint8_t                   Address;
  A3G4250D_WriteReg_Func    WriteReg;
  A3G4250D_ReadReg_Func     ReadReg;
  A3G4250D_GetTick_Func     GetTick;
  A3G4250D_Delay_Func       Delay;
} A3G4250D_IO_t;

typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} A3G4250D_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} A3G4250D_Axes_t;

typedef struct
{
  A3G4250D_IO_t      IO;
  stmdev_ctx_t       Ctx;
  uint8_t            is_initialized;
  uint8_t            gyro_is_enabled;
  a3g4250d_dr_t      gyro_odr;
} A3G4250D_Object_t;

typedef struct
{
  uint8_t   Acc;
  uint8_t   Gyro;
  uint8_t   Magneto;
  uint8_t   LowPower;
  uint32_t  GyroMaxFS;
  uint32_t  AccMaxFS;
  uint32_t  MagMaxFS;
  float_t     GyroMaxOdr;
  float_t     AccMaxOdr;
  float_t     MagMaxOdr;
} A3G4250D_Capabilities_t;

typedef struct
{
  int32_t (*Init)(A3G4250D_Object_t *);
  int32_t (*DeInit)(A3G4250D_Object_t *);
  int32_t (*ReadID)(A3G4250D_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(A3G4250D_Object_t *, A3G4250D_Capabilities_t *);
} A3G4250D_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(A3G4250D_Object_t *);
  int32_t (*Disable)(A3G4250D_Object_t *);
  int32_t (*GetSensitivity)(A3G4250D_Object_t *, float_t *);
  int32_t (*GetOutputDataRate)(A3G4250D_Object_t *, float_t *);
  int32_t (*SetOutputDataRate)(A3G4250D_Object_t *, float_t);
  int32_t (*GetFullScale)(A3G4250D_Object_t *, int32_t *);
  int32_t (*SetFullScale)(A3G4250D_Object_t *, int32_t);
  int32_t (*GetAxes)(A3G4250D_Object_t *, A3G4250D_Axes_t *);
  int32_t (*GetAxesRaw)(A3G4250D_Object_t *, A3G4250D_AxesRaw_t *);
} A3G4250D_GYRO_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} a3g4250d_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} a3g4250d_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} a3g4250d_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} a3g4250d_axis1bit32_t;

/**
  * @}
  */

/** @defgroup A3G4250D_Exported_Constants A3G4250D Exported Constants
  * @{
  */

#define A3G4250D_OK                       0
#define A3G4250D_ERROR                   -1

#define A3G4250D_I2C_BUS                 0U
#define A3G4250D_SPI_4WIRES_BUS          1U
#define A3G4250D_SPI_3WIRES_BUS          2U

#define A3G4250D_GYRO_SENSITIVITY    8.750f

/**
  * @}
  */

/** @addtogroup A3G4250D_Exported_Functions A3G4250D Exported Functions
  * @{
  */

int32_t A3G4250D_RegisterBusIO(A3G4250D_Object_t *pObj, A3G4250D_IO_t *pIO);
int32_t A3G4250D_Init(A3G4250D_Object_t *pObj);
int32_t A3G4250D_DeInit(A3G4250D_Object_t *pObj);
int32_t A3G4250D_ReadID(A3G4250D_Object_t *pObj, uint8_t *Id);
int32_t A3G4250D_GetCapabilities(A3G4250D_Object_t *pObj, A3G4250D_Capabilities_t *Capabilities);

int32_t A3G4250D_GYRO_Enable(A3G4250D_Object_t *pObj);
int32_t A3G4250D_GYRO_Disable(A3G4250D_Object_t *pObj);
int32_t A3G4250D_GYRO_GetSensitivity(A3G4250D_Object_t *pObj, float_t *Sensitivity);
int32_t A3G4250D_GYRO_GetOutputDataRate(A3G4250D_Object_t *pObj, float_t *Odr);
int32_t A3G4250D_GYRO_SetOutputDataRate(A3G4250D_Object_t *pObj, float_t Odr);
int32_t A3G4250D_GYRO_GetFullScale(A3G4250D_Object_t *pObj, int32_t *FullScale);
int32_t A3G4250D_GYRO_SetFullScale(A3G4250D_Object_t *pObj, int32_t FullScale);
int32_t A3G4250D_GYRO_GetAxesRaw(A3G4250D_Object_t *pObj, A3G4250D_AxesRaw_t *Value);
int32_t A3G4250D_GYRO_GetAxes(A3G4250D_Object_t *pObj, A3G4250D_Axes_t *AngularRate);

int32_t A3G4250D_Read_Reg(A3G4250D_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t A3G4250D_Write_Reg(A3G4250D_Object_t *pObj, uint8_t reg, uint8_t Data);

int32_t A3G4250D_GYRO_Get_DRDY_Status(A3G4250D_Object_t *pObj, uint8_t *Status);

/**
  * @}
  */

/** @addtogroup A3G4250D_Exported_Variables A3G4250D Exported Variables
  * @{
  */

extern A3G4250D_CommonDrv_t A3G4250D_COMMON_Driver;
extern A3G4250D_GYRO_Drv_t A3G4250D_GYRO_Driver;

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
