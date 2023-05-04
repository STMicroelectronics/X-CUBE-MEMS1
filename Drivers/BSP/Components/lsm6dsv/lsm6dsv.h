/**
 ******************************************************************************
 * @file    lsm6dsv.h
 * @author  MEMS Software Solutions Team
 * @brief   LSM6DSV header driver file
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
#ifndef LSM6DSV_H
#define LSM6DSV_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "lsm6dsv_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @addtogroup LSM6DSV LSM6DSV
 * @{
 */

/** @defgroup LSM6DSV_Exported_Types LSM6DSV Exported Types
 * @{
 */

typedef int32_t (*LSM6DSV_Init_Func)(void);
typedef int32_t (*LSM6DSV_DeInit_Func)(void);
typedef int32_t (*LSM6DSV_GetTick_Func)(void);
typedef void (*LSM6DSV_Delay_Func)(uint32_t);
typedef int32_t (*LSM6DSV_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*LSM6DSV_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  LSM6DSV_INT1_PIN,
  LSM6DSV_INT2_PIN,
} LSM6DSV_SensorIntPin_t;

typedef enum
{
  LSM6DSV_ACC_HIGH_PERFORMANCE_MODE,
  LSM6DSV_ACC_HIGH_ACCURACY_MODE,
  LSM6DSV_ACC_NORMAL_MODE,
  LSM6DSV_ACC_LOW_POWER_MODE1,
  LSM6DSV_ACC_LOW_POWER_MODE2,
  LSM6DSV_ACC_LOW_POWER_MODE3
} LSM6DSV_ACC_Operating_Mode_t;

typedef enum
{
  LSM6DSV_GYRO_HIGH_PERFORMANCE_MODE,
  LSM6DSV_GYRO_HIGH_ACCURACY_MODE,
  LSM6DSV_GYRO_SLEEP_MODE,
  LSM6DSV_GYRO_LOW_POWER_MODE
} LSM6DSV_GYRO_Operating_Mode_t;

typedef struct
{
  LSM6DSV_Init_Func       Init;
  LSM6DSV_DeInit_Func     DeInit;
  uint32_t                BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
  uint8_t                 Address;
  LSM6DSV_WriteReg_Func   WriteReg;
  LSM6DSV_ReadReg_Func    ReadReg;
  LSM6DSV_GetTick_Func    GetTick;
  LSM6DSV_Delay_Func      Delay;
} LSM6DSV_IO_t;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} LSM6DSV_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} LSM6DSV_Axes_t;

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
} LSM6DSV_Event_Status_t;

typedef struct
{
  LSM6DSV_IO_t              IO;
  stmdev_ctx_t              Ctx;
  uint8_t                   is_initialized;
  uint8_t                   acc_is_enabled;
  uint8_t                   gyro_is_enabled;
  lsm6dsv_data_rate_t       acc_odr;
  lsm6dsv_data_rate_t       gyro_odr;
} LSM6DSV_Object_t;

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
} LSM6DSV_Capabilities_t;

typedef struct
{
  int32_t (*Init)(LSM6DSV_Object_t *);
  int32_t (*DeInit)(LSM6DSV_Object_t *);
  int32_t (*ReadID)(LSM6DSV_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(LSM6DSV_Object_t *, LSM6DSV_Capabilities_t *);
} LSM6DSV_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(LSM6DSV_Object_t *);
  int32_t (*Disable)(LSM6DSV_Object_t *);
  int32_t (*GetSensitivity)(LSM6DSV_Object_t *, float *);
  int32_t (*GetOutputDataRate)(LSM6DSV_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LSM6DSV_Object_t *, float);
  int32_t (*GetFullScale)(LSM6DSV_Object_t *, int32_t *);
  int32_t (*SetFullScale)(LSM6DSV_Object_t *, int32_t);
  int32_t (*GetAxes)(LSM6DSV_Object_t *, LSM6DSV_Axes_t *);
  int32_t (*GetAxesRaw)(LSM6DSV_Object_t *, LSM6DSV_AxesRaw_t *);
} LSM6DSV_ACC_Drv_t;

typedef struct
{
  int32_t (*Enable)(LSM6DSV_Object_t *);
  int32_t (*Disable)(LSM6DSV_Object_t *);
  int32_t (*GetSensitivity)(LSM6DSV_Object_t *, float *);
  int32_t (*GetOutputDataRate)(LSM6DSV_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LSM6DSV_Object_t *, float);
  int32_t (*GetFullScale)(LSM6DSV_Object_t *, int32_t *);
  int32_t (*SetFullScale)(LSM6DSV_Object_t *, int32_t);
  int32_t (*GetAxes)(LSM6DSV_Object_t *, LSM6DSV_Axes_t *);
  int32_t (*GetAxesRaw)(LSM6DSV_Object_t *, LSM6DSV_AxesRaw_t *);
} LSM6DSV_GYRO_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} lsm6dsv_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} lsm6dsv_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} lsm6dsv_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} lsm6dsv_axis1bit32_t;

/**
 * @}
 */

/** @defgroup LSM6DSV_Exported_Constants LSM6DSV Exported Constants
 * @{
 */

#define LSM6DSV_OK                       0
#define LSM6DSV_ERROR                   -1

#define LSM6DSV_I2C_BUS                 0U
#define LSM6DSV_SPI_4WIRES_BUS          1U
#define LSM6DSV_SPI_3WIRES_BUS          2U

#define LSM6DSV_ACC_SENSITIVITY_FS_2G   0.061f
#define LSM6DSV_ACC_SENSITIVITY_FS_4G   0.122f
#define LSM6DSV_ACC_SENSITIVITY_FS_8G   0.244f
#define LSM6DSV_ACC_SENSITIVITY_FS_16G  0.488f

#define LSM6DSV_GYRO_SENSITIVITY_FS_125DPS     4.375f
#define LSM6DSV_GYRO_SENSITIVITY_FS_250DPS     8.750f
#define LSM6DSV_GYRO_SENSITIVITY_FS_500DPS    17.500f
#define LSM6DSV_GYRO_SENSITIVITY_FS_1000DPS   35.000f
#define LSM6DSV_GYRO_SENSITIVITY_FS_2000DPS   70.000f
#define LSM6DSV_GYRO_SENSITIVITY_FS_4000DPS  140.000f

/**
 * @}
 */

/** @addtogroup LSM6DSV_Exported_Functions LSM6DSV Exported Functions
 * @{
 */

int32_t LSM6DSV_RegisterBusIO(LSM6DSV_Object_t *pObj, LSM6DSV_IO_t *pIO);
int32_t LSM6DSV_Init(LSM6DSV_Object_t *pObj);
int32_t LSM6DSV_DeInit(LSM6DSV_Object_t *pObj);
int32_t LSM6DSV_ReadID(LSM6DSV_Object_t *pObj, uint8_t *Id);
int32_t LSM6DSV_GetCapabilities(LSM6DSV_Object_t *pObj, LSM6DSV_Capabilities_t *Capabilities);

int32_t LSM6DSV_ACC_Enable(LSM6DSV_Object_t *pObj);
int32_t LSM6DSV_ACC_Disable(LSM6DSV_Object_t *pObj);
int32_t LSM6DSV_ACC_GetSensitivity(LSM6DSV_Object_t *pObj, float *Sensitivity);
int32_t LSM6DSV_ACC_GetOutputDataRate(LSM6DSV_Object_t *pObj, float *Odr);
int32_t LSM6DSV_ACC_SetOutputDataRate(LSM6DSV_Object_t *pObj, float Odr);
int32_t LSM6DSV_ACC_SetOutputDataRate_With_Mode(LSM6DSV_Object_t *pObj, float Odr, LSM6DSV_ACC_Operating_Mode_t Mode);
int32_t LSM6DSV_ACC_GetFullScale(LSM6DSV_Object_t *pObj, int32_t *FullScale);
int32_t LSM6DSV_ACC_SetFullScale(LSM6DSV_Object_t *pObj, int32_t FullScale);
int32_t LSM6DSV_ACC_GetAxesRaw(LSM6DSV_Object_t *pObj, LSM6DSV_AxesRaw_t *Value);
int32_t LSM6DSV_ACC_GetAxes(LSM6DSV_Object_t *pObj, LSM6DSV_Axes_t *Acceleration);

int32_t LSM6DSV_GYRO_Enable(LSM6DSV_Object_t *pObj);
int32_t LSM6DSV_GYRO_Disable(LSM6DSV_Object_t *pObj);
int32_t LSM6DSV_GYRO_GetSensitivity(LSM6DSV_Object_t *pObj, float *Sensitivity);
int32_t LSM6DSV_GYRO_GetOutputDataRate(LSM6DSV_Object_t *pObj, float *Odr);
int32_t LSM6DSV_GYRO_SetOutputDataRate(LSM6DSV_Object_t *pObj, float Odr);
int32_t LSM6DSV_GYRO_SetOutputDataRate_With_Mode(LSM6DSV_Object_t *pObj, float Odr, LSM6DSV_GYRO_Operating_Mode_t Mode);
int32_t LSM6DSV_GYRO_GetFullScale(LSM6DSV_Object_t *pObj, int32_t *FullScale);
int32_t LSM6DSV_GYRO_SetFullScale(LSM6DSV_Object_t *pObj, int32_t FullScale);
int32_t LSM6DSV_GYRO_GetAxesRaw(LSM6DSV_Object_t *pObj, LSM6DSV_AxesRaw_t *Value);
int32_t LSM6DSV_GYRO_GetAxes(LSM6DSV_Object_t *pObj, LSM6DSV_Axes_t *AngularRate);

int32_t LSM6DSV_Read_Reg(LSM6DSV_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t LSM6DSV_Write_Reg(LSM6DSV_Object_t *pObj, uint8_t reg, uint8_t Data);

int32_t LSM6DSV_ACC_Get_DRDY_Status(LSM6DSV_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSV_GYRO_Get_DRDY_Status(LSM6DSV_Object_t *pObj, uint8_t *Status);

int32_t LSM6DSV_ACC_Set_Power_Mode(LSM6DSV_Object_t *pObj, uint8_t PowerMode);
int32_t LSM6DSV_GYRO_Set_Power_Mode(LSM6DSV_Object_t *pObj, uint8_t PowerMode);
int32_t LSM6DSV_ACC_Set_Filter_Mode(LSM6DSV_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode);
int32_t LSM6DSV_GYRO_Set_Filter_Mode(LSM6DSV_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode);



/**
 * @}
 */

/** @addtogroup LSM6DSV_Exported_Variables LSM6DSV Exported Variables
 * @{
 */

extern LSM6DSV_CommonDrv_t LSM6DSV_COMMON_Driver;
extern LSM6DSV_ACC_Drv_t LSM6DSV_ACC_Driver;
extern LSM6DSV_GYRO_Drv_t LSM6DSV_GYRO_Driver;

/**
 * @}
 */


typedef enum
{
  LSM6DSV_ULTRA_LIGHT  = 0,
  LSM6DSV_VERY_LIGHT   = 1,
  LSM6DSV_LIGHT        = 2,
  LSM6DSV_MEDIUM       = 3,
  LSM6DSV_STRONG       = 4,
  LSM6DSV_VERY_STRONG  = 5,
  LSM6DSV_AGGRESSIVE   = 6,
  LSM6DSV_XTREME       = 7,
} lsm6dsv_ftype_t;

typedef enum
{
  LSM6DSV_HP_FILTER_NONE     = 0x00,
  LSM6DSV_HP_FILTER_16mHz    = 0x80,
  LSM6DSV_HP_FILTER_65mHz    = 0x81,
  LSM6DSV_HP_FILTER_260mHz   = 0x82,
  LSM6DSV_HP_FILTER_1Hz04    = 0x83,
} lsm6dsv_hpm_g_t;

typedef enum
{
  LSM6DSV_HP_PATH_DISABLE_ON_OUT    = 0x00,
  LSM6DSV_SLOPE_ODR_DIV_4           = LSM6DSV_GY_ULTRA_LIGHT,
  LSM6DSV_HP_ODR_DIV_10             = LSM6DSV_GY_VERY_LIGHT,
  LSM6DSV_HP_ODR_DIV_20             = LSM6DSV_GY_LIGHT,
  LSM6DSV_HP_ODR_DIV_45             = LSM6DSV_GY_MEDIUM,
  LSM6DSV_HP_ODR_DIV_100            = LSM6DSV_GY_STRONG,
  LSM6DSV_HP_ODR_DIV_200            = LSM6DSV_GY_VERY_STRONG,
  LSM6DSV_HP_ODR_DIV_400            = LSM6DSV_GY_AGGRESSIVE,
  LSM6DSV_HP_ODR_DIV_800            = LSM6DSV_GY_XTREME,

  LSM6DSV_HP_REF_MD_ODR_DIV_10      = 0x31,
  LSM6DSV_HP_REF_MD_ODR_DIV_20      = 0x32,
  LSM6DSV_HP_REF_MD_ODR_DIV_45      = 0x33,
  LSM6DSV_HP_REF_MD_ODR_DIV_100     = 0x34,
  LSM6DSV_HP_REF_MD_ODR_DIV_200     = 0x35,
  LSM6DSV_HP_REF_MD_ODR_DIV_400     = 0x36,
  LSM6DSV_HP_REF_MD_ODR_DIV_800     = 0x37,
  LSM6DSV_LP_ODR_DIV_10             = 0x01,
  LSM6DSV_LP_ODR_DIV_20             = 0x02,
  LSM6DSV_LP_ODR_DIV_45             = 0x03,
  LSM6DSV_LP_ODR_DIV_100            = 0x04,
  LSM6DSV_LP_ODR_DIV_200            = 0x05,
  LSM6DSV_LP_ODR_DIV_400            = 0x06,
  LSM6DSV_LP_ODR_DIV_800            = 0x07,
} lsm6dsv_hp_slope_xl_en_t;

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
