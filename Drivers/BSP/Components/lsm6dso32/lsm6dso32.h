/**
  ******************************************************************************
  * @file    lsm6dso32.h
  * @author  MEMS Software Solutions Team
  * @brief   LSM6DSO32 header driver file
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
#ifndef LSM6DSO32_H
#define LSM6DSO32_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "lsm6dso32_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup LSM6DSO32 LSM6DSO32
  * @{
  */

/** @defgroup LSM6DSO32_Exported_Types LSM6DSO32 Exported Types
  * @{
  */

typedef int32_t (*LSM6DSO32_Init_Func)(void);
typedef int32_t (*LSM6DSO32_DeInit_Func)(void);
typedef int32_t (*LSM6DSO32_GetTick_Func)(void);
typedef void    (*LSM6DSO32_Delay_Func)(uint32_t);
typedef int32_t (*LSM6DSO32_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*LSM6DSO32_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  LSM6DSO32_INT1_PIN,
  LSM6DSO32_INT2_PIN,
} LSM6DSO32_SensorIntPin_t;

typedef enum
{
  LSM6DSO32_ACC_HIGH_PERFORMANCE_MODE,
  LSM6DSO32_ACC_LOW_POWER_NORMAL_MODE,
  LSM6DSO32_ACC_ULTRA_LOW_POWER_MODE
} LSM6DSO32_ACC_Operating_Mode_t;

typedef enum
{
  LSM6DSO32_GYRO_HIGH_PERFORMANCE_MODE,
  LSM6DSO32_GYRO_LOW_POWER_NORMAL_MODE
} LSM6DSO32_GYRO_Operating_Mode_t;

typedef struct
{
  LSM6DSO32_Init_Func        Init;
  LSM6DSO32_DeInit_Func      DeInit;
  uint32_t                   BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
  uint8_t                    Address;
  LSM6DSO32_WriteReg_Func    WriteReg;
  LSM6DSO32_ReadReg_Func     ReadReg;
  LSM6DSO32_GetTick_Func     GetTick;
  LSM6DSO32_Delay_Func       Delay;
} LSM6DSO32_IO_t;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} LSM6DSO32_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} LSM6DSO32_Axes_t;

typedef struct
{
  unsigned int FreeFallStatus       : 1;
  unsigned int TapStatus            : 1;
  unsigned int DoubleTapStatus      : 1;
  unsigned int WakeUpStatus         : 1;
  unsigned int StepStatus           : 1;
  unsigned int TiltStatus           : 1;
  unsigned int D6DOrientationStatus : 1;
  unsigned int SleepStatus          : 1;
} LSM6DSO32_Event_Status_t;

typedef struct
{
  LSM6DSO32_IO_t      IO;
  stmdev_ctx_t        Ctx;
  uint8_t             is_initialized;
  uint8_t             acc_is_enabled;
  uint8_t             gyro_is_enabled;
  lsm6dso32_odr_xl_t  acc_odr;
  lsm6dso32_odr_g_t   gyro_odr;
} LSM6DSO32_Object_t;

typedef struct
{
  uint8_t   Acc;
  uint8_t   Gyro;
  uint8_t   Magneto;
  uint8_t   LowPower;
  uint32_t  GyroMaxFS;
  uint32_t  AccMaxFS;
  uint32_t  MagMaxFS;
  float_t   GyroMaxOdr;
  float_t   AccMaxOdr;
  float_t   MagMaxOdr;
} LSM6DSO32_Capabilities_t;

typedef struct
{
  int32_t (*Init)(LSM6DSO32_Object_t *);
  int32_t (*DeInit)(LSM6DSO32_Object_t *);
  int32_t (*ReadID)(LSM6DSO32_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(LSM6DSO32_Object_t *, LSM6DSO32_Capabilities_t *);
} LSM6DSO32_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(LSM6DSO32_Object_t *);
  int32_t (*Disable)(LSM6DSO32_Object_t *);
  int32_t (*GetSensitivity)(LSM6DSO32_Object_t *, float_t *);
  int32_t (*GetOutputDataRate)(LSM6DSO32_Object_t *, float_t *);
  int32_t (*SetOutputDataRate)(LSM6DSO32_Object_t *, float_t);
  int32_t (*GetFullScale)(LSM6DSO32_Object_t *, int32_t *);
  int32_t (*SetFullScale)(LSM6DSO32_Object_t *, int32_t);
  int32_t (*GetAxes)(LSM6DSO32_Object_t *, LSM6DSO32_Axes_t *);
  int32_t (*GetAxesRaw)(LSM6DSO32_Object_t *, LSM6DSO32_AxesRaw_t *);
} LSM6DSO32_ACC_Drv_t;

typedef struct
{
  int32_t (*Enable)(LSM6DSO32_Object_t *);
  int32_t (*Disable)(LSM6DSO32_Object_t *);
  int32_t (*GetSensitivity)(LSM6DSO32_Object_t *, float_t *);
  int32_t (*GetOutputDataRate)(LSM6DSO32_Object_t *, float_t *);
  int32_t (*SetOutputDataRate)(LSM6DSO32_Object_t *, float_t);
  int32_t (*GetFullScale)(LSM6DSO32_Object_t *, int32_t *);
  int32_t (*SetFullScale)(LSM6DSO32_Object_t *, int32_t);
  int32_t (*GetAxes)(LSM6DSO32_Object_t *, LSM6DSO32_Axes_t *);
  int32_t (*GetAxesRaw)(LSM6DSO32_Object_t *, LSM6DSO32_AxesRaw_t *);
} LSM6DSO32_GYRO_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} lsm6dso32_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} lsm6dso32_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} lsm6dso32_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} lsm6dso32_axis1bit32_t;

/**
  * @}
  */

/** @defgroup LSM6DSO32_Exported_Constants LSM6DSO32 Exported Constants
  * @{
  */

#define LSM6DSO32_OK                       0
#define LSM6DSO32_ERROR                   -1

#define LSM6DSO32_I2C_BUS                 0U
#define LSM6DSO32_SPI_4WIRES_BUS          1U
#define LSM6DSO32_SPI_3WIRES_BUS          2U

#define LSM6DSO32_ACC_SENSITIVITY_FS_4G   0.122f
#define LSM6DSO32_ACC_SENSITIVITY_FS_8G   0.244f
#define LSM6DSO32_ACC_SENSITIVITY_FS_16G  0.488f
#define LSM6DSO32_ACC_SENSITIVITY_FS_32G  0.976f

#define LSM6DSO32_GYRO_SENSITIVITY_FS_125DPS    4.375f
#define LSM6DSO32_GYRO_SENSITIVITY_FS_250DPS    8.750f
#define LSM6DSO32_GYRO_SENSITIVITY_FS_500DPS   17.500f
#define LSM6DSO32_GYRO_SENSITIVITY_FS_1000DPS  35.000f
#define LSM6DSO32_GYRO_SENSITIVITY_FS_2000DPS  70.000f

/**
  * @}
  */

/** @addtogroup LSM6DSO32_Exported_Functions LSM6DSO32 Exported Functions
  * @{
  */

int32_t LSM6DSO32_RegisterBusIO(LSM6DSO32_Object_t *pObj, LSM6DSO32_IO_t *pIO);
int32_t LSM6DSO32_Init(LSM6DSO32_Object_t *pObj);
int32_t LSM6DSO32_DeInit(LSM6DSO32_Object_t *pObj);
int32_t LSM6DSO32_ReadID(LSM6DSO32_Object_t *pObj, uint8_t *Id);
int32_t LSM6DSO32_GetCapabilities(LSM6DSO32_Object_t *pObj, LSM6DSO32_Capabilities_t *Capabilities);

int32_t LSM6DSO32_ACC_Enable(LSM6DSO32_Object_t *pObj);
int32_t LSM6DSO32_ACC_Disable(LSM6DSO32_Object_t *pObj);
int32_t LSM6DSO32_ACC_GetSensitivity(LSM6DSO32_Object_t *pObj, float_t *Sensitivity);
int32_t LSM6DSO32_ACC_GetOutputDataRate(LSM6DSO32_Object_t *pObj, float_t *Odr);
int32_t LSM6DSO32_ACC_SetOutputDataRate(LSM6DSO32_Object_t *pObj, float_t Odr);
int32_t LSM6DSO32_ACC_SetOutputDataRate_With_Mode(LSM6DSO32_Object_t *pObj, float_t Odr,
                                                  LSM6DSO32_ACC_Operating_Mode_t Mode);
int32_t LSM6DSO32_ACC_GetFullScale(LSM6DSO32_Object_t *pObj, int32_t *FullScale);
int32_t LSM6DSO32_ACC_SetFullScale(LSM6DSO32_Object_t *pObj, int32_t FullScale);
int32_t LSM6DSO32_ACC_GetAxesRaw(LSM6DSO32_Object_t *pObj, LSM6DSO32_AxesRaw_t *Value);
int32_t LSM6DSO32_ACC_GetAxes(LSM6DSO32_Object_t *pObj, LSM6DSO32_Axes_t *Acceleration);

int32_t LSM6DSO32_GYRO_Enable(LSM6DSO32_Object_t *pObj);
int32_t LSM6DSO32_GYRO_Disable(LSM6DSO32_Object_t *pObj);
int32_t LSM6DSO32_GYRO_GetSensitivity(LSM6DSO32_Object_t *pObj, float_t *Sensitivity);
int32_t LSM6DSO32_GYRO_GetOutputDataRate(LSM6DSO32_Object_t *pObj, float_t *Odr);
int32_t LSM6DSO32_GYRO_SetOutputDataRate(LSM6DSO32_Object_t *pObj, float_t Odr);
int32_t LSM6DSO32_GYRO_SetOutputDataRate_With_Mode(LSM6DSO32_Object_t *pObj, float_t Odr,
                                                   LSM6DSO32_GYRO_Operating_Mode_t Mode);
int32_t LSM6DSO32_GYRO_GetFullScale(LSM6DSO32_Object_t *pObj, int32_t *FullScale);
int32_t LSM6DSO32_GYRO_SetFullScale(LSM6DSO32_Object_t *pObj, int32_t FullScale);
int32_t LSM6DSO32_GYRO_GetAxesRaw(LSM6DSO32_Object_t *pObj, LSM6DSO32_AxesRaw_t *Value);
int32_t LSM6DSO32_GYRO_GetAxes(LSM6DSO32_Object_t *pObj, LSM6DSO32_Axes_t *AngularRate);

int32_t LSM6DSO32_Read_Reg(LSM6DSO32_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t LSM6DSO32_Write_Reg(LSM6DSO32_Object_t *pObj, uint8_t reg, uint8_t Data);

int32_t LSM6DSO32_ACC_Get_DRDY_Status(LSM6DSO32_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSO32_GYRO_Get_DRDY_Status(LSM6DSO32_Object_t *pObj, uint8_t *Status);

/**
  * @}
  */

/** @addtogroup LSM6DSO32_Exported_Variables LSM6DSO32 Exported Variables
  * @{
  */

extern LSM6DSO32_CommonDrv_t LSM6DSO32_COMMON_Driver;
extern LSM6DSO32_ACC_Drv_t LSM6DSO32_ACC_Driver;
extern LSM6DSO32_GYRO_Drv_t LSM6DSO32_GYRO_Driver;

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
