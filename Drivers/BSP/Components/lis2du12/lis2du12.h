/**
  ******************************************************************************
  * @file    lis2du12.h
  * @author  MEMS Software Solutions Team
  * @brief   LIS2DU12 header driver file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#ifndef LIS2DU12_H
#define LIS2DU12_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "lis2du12_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup LIS2DU12 LIS2DU12
  * @{
  */

/** @defgroup LIS2DU12_Exported_Types LIS2DU12 Exported Types
  * @{
  */

typedef int32_t (*LIS2DU12_Init_Func)(void);
typedef int32_t (*LIS2DU12_DeInit_Func)(void);
typedef int32_t (*LIS2DU12_GetTick_Func)(void);
typedef int32_t (*LIS2DU12_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*LIS2DU12_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  LIS2DU12_INT1_PIN,
  LIS2DU12_INT2_PIN,
} LIS2DU12_SensorIntPin_t;

typedef struct
{
  LIS2DU12_Init_Func         Init;
  LIS2DU12_DeInit_Func       DeInit;
  uint32_t                   BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
  uint8_t                    Address;
  LIS2DU12_WriteReg_Func     WriteReg;
  LIS2DU12_ReadReg_Func      ReadReg;
  LIS2DU12_GetTick_Func      GetTick;
} LIS2DU12_IO_t;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} LIS2DU12_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} LIS2DU12_Axes_t;

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
} LIS2DU12_Event_Status_t;

typedef enum
{
  LIS2DU12_ULTRA_LOW_POWER_DISABLE,
  LIS2DU12_ULTRA_LOW_POWER_ENABLE
} LIS2DU12_Ultra_Low_Power_t;

typedef struct
{
  LIS2DU12_IO_t              IO;
  stmdev_ctx_t               Ctx;
  uint8_t                    is_initialized;
  uint8_t                    acc_is_enabled;
  float                      acc_odr;
  LIS2DU12_Ultra_Low_Power_t acc_ultra_low_power;
} LIS2DU12_Object_t;

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
} LIS2DU12_Capabilities_t;

typedef struct
{
  int32_t (*Init)(LIS2DU12_Object_t *);
  int32_t (*DeInit)(LIS2DU12_Object_t *);
  int32_t (*ReadID)(LIS2DU12_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(LIS2DU12_Object_t *, LIS2DU12_Capabilities_t *);
} LIS2DU12_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(LIS2DU12_Object_t *);
  int32_t (*Disable)(LIS2DU12_Object_t *);
  int32_t (*GetSensitivity)(LIS2DU12_Object_t *, float *);
  int32_t (*GetOutputDataRate)(LIS2DU12_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LIS2DU12_Object_t *, float);
  int32_t (*GetFullScale)(LIS2DU12_Object_t *, int32_t *);
  int32_t (*SetFullScale)(LIS2DU12_Object_t *, int32_t);
  int32_t (*GetAxes)(LIS2DU12_Object_t *, LIS2DU12_Axes_t *);
  int32_t (*GetAxesRaw)(LIS2DU12_Object_t *, LIS2DU12_AxesRaw_t *);
} LIS2DU12_ACC_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} lis2du12_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} lis2du12_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} lis2du12_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} lis2du12_axis1bit32_t;

/**
  * @}
  */

/** @defgroup LIS2DU12_Exported_Constants LIS2DU12 Exported Constants
  * @{
  */

#define LIS2DU12_OK                       0
#define LIS2DU12_ERROR                   -1

#define LIS2DU12_I2C_BUS                 0U
#define LIS2DU12_SPI_4WIRES_BUS          1U
#define LIS2DU12_SPI_3WIRES_BUS          2U

#define LIS2DU12_ACC_SENSITIVITY_FOR_FS_2G   0.976f  /**< Sensitivity value for 2g full scale, Low-power1 mode [mg/LSB] */
#define LIS2DU12_ACC_SENSITIVITY_FOR_FS_4G   1.952f  /**< Sensitivity value for 4g full scale, Low-power1 mode [mg/LSB] */
#define LIS2DU12_ACC_SENSITIVITY_FOR_FS_8G   3.904f  /**< Sensitivity value for 8g full scale, Low-power1 mode [mg/LSB] */
#define LIS2DU12_ACC_SENSITIVITY_FOR_FS_16G  7.808f  /**< Sensitivity value for 16g full scale, Low-power1 mode [mg/LSB] */

/**
  * @}
  */

/** @addtogroup LIS2DU12_Exported_Functions LIS2DU12 Exported Functions
  * @{
  */

int32_t LIS2DU12_RegisterBusIO(LIS2DU12_Object_t *pObj, LIS2DU12_IO_t *pIO);
int32_t LIS2DU12_Init(LIS2DU12_Object_t *pObj);
int32_t LIS2DU12_DeInit(LIS2DU12_Object_t *pObj);
int32_t LIS2DU12_ReadID(LIS2DU12_Object_t *pObj, uint8_t *Id);
int32_t LIS2DU12_GetCapabilities(LIS2DU12_Object_t *pObj, LIS2DU12_Capabilities_t *Capabilities);

int32_t LIS2DU12_ACC_Enable(LIS2DU12_Object_t *pObj);
int32_t LIS2DU12_ACC_Disable(LIS2DU12_Object_t *pObj);
int32_t LIS2DU12_ACC_GetSensitivity(LIS2DU12_Object_t *pObj, float *Sensitivity);
int32_t LIS2DU12_ACC_GetOutputDataRate(LIS2DU12_Object_t *pObj, float *Odr);
int32_t LIS2DU12_ACC_SetOutputDataRate(LIS2DU12_Object_t *pObj, float Odr);
int32_t LIS2DU12_ACC_SetOutputDataRate_With_Mode(LIS2DU12_Object_t *pObj, float Odr, LIS2DU12_Ultra_Low_Power_t Power);
int32_t LIS2DU12_ACC_GetFullScale(LIS2DU12_Object_t *pObj, int32_t *FullScale);
int32_t LIS2DU12_ACC_SetFullScale(LIS2DU12_Object_t *pObj, int32_t FullScale);
int32_t LIS2DU12_ACC_GetAxesRaw(LIS2DU12_Object_t *pObj, LIS2DU12_AxesRaw_t *Value);
int32_t LIS2DU12_ACC_GetAxes(LIS2DU12_Object_t *pObj, LIS2DU12_Axes_t *Acceleration);

int32_t LIS2DU12_Read_Reg(LIS2DU12_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t LIS2DU12_Write_Reg(LIS2DU12_Object_t *pObj, uint8_t reg, uint8_t Data);
int32_t LIS2DU12_Set_Interrupt_Latch(LIS2DU12_Object_t *pObj, uint8_t Status);

int32_t LIS2DU12_ACC_Enable_DRDY_Interrupt(LIS2DU12_Object_t *pObj);
int32_t LIS2DU12_ACC_Disable_DRDY_Interrupt(LIS2DU12_Object_t *pObj);

int32_t LIS2DU12_ACC_Set_SelfTest(LIS2DU12_Object_t *pObj, uint8_t Val);
int32_t LIS2DU12_ACC_Get_DRDY_Status(LIS2DU12_Object_t *pObj, uint8_t *Status);
int32_t LIS2DU12_ACC_Get_Init_Status(LIS2DU12_Object_t *pObj, uint8_t *Status);

/**
  * @}
  */

/** @addtogroup LIS2DU12_Exported_Variables LIS2DU12 Exported Variables
  * @{
  */

extern LIS2DU12_CommonDrv_t LIS2DU12_COMMON_Driver;
extern LIS2DU12_ACC_Drv_t LIS2DU12_ACC_Driver;

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
