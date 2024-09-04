/**
 ******************************************************************************
 * @file    ism330bx.h
 * @author  MEMS Software Solutions Team
 * @brief   ISM330BX header driver file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ISM330BX_H
#define ISM330BX_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "ism330bx_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @addtogroup ISM330BX ISM330BX
 * @{
 */

/** @defgroup ISM330BX_Exported_Types ISM330BX Exported Types
 * @{
 */

typedef int32_t (*ISM330BX_Init_Func)(void);
typedef int32_t (*ISM330BX_DeInit_Func)(void);
typedef int32_t (*ISM330BX_GetTick_Func)(void);
typedef void    (*ISM330BX_Delay_Func)(uint32_t);
typedef int32_t (*ISM330BX_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*ISM330BX_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  ISM330BX_INT1_PIN,
  ISM330BX_INT2_PIN,
} ISM330BX_SensorIntPin_t;

typedef enum
{
  ISM330BX_ACC_HIGH_PERFORMANCE_MODE,
  ISM330BX_ACC_HIGH_PERFORMANCE_TDM_MODE,
  ISM330BX_ACC_LOW_POWER_MODE1,
  ISM330BX_ACC_LOW_POWER_MODE2,
  ISM330BX_ACC_LOW_POWER_MODE3
} ISM330BX_ACC_Operating_Mode_t;

typedef enum
{
  ISM330BX_GYRO_HIGH_PERFORMANCE_MODE,
  ISM330BX_GYRO_SLEEP_MODE,
  ISM330BX_GYRO_LOW_POWER_MODE
} ISM330BX_GYRO_Operating_Mode_t;

typedef struct
{
  ISM330BX_Init_Func       Init;
  ISM330BX_DeInit_Func     DeInit;
  uint32_t                 BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires, 3 means I3C */
  uint8_t                  Address;
  ISM330BX_WriteReg_Func   WriteReg;
  ISM330BX_ReadReg_Func    ReadReg;
  ISM330BX_GetTick_Func    GetTick;
  ISM330BX_Delay_Func      Delay;
} ISM330BX_IO_t;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} ISM330BX_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} ISM330BX_Axes_t;

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
} ISM330BX_Event_Status_t;

typedef struct
{
  ISM330BX_IO_t           IO;
  stmdev_ctx_t            Ctx;
  uint8_t                 is_initialized;
  uint8_t                 acc_is_enabled;
  uint8_t                 gyro_is_enabled;
  ism330bx_xl_data_rate_t acc_odr;
  ism330bx_gy_data_rate_t gyro_odr;
} ISM330BX_Object_t;

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
} ISM330BX_Capabilities_t;

typedef struct
{
  int32_t (*Init)(ISM330BX_Object_t *);
  int32_t (*DeInit)(ISM330BX_Object_t *);
  int32_t (*ReadID)(ISM330BX_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(ISM330BX_Object_t *, ISM330BX_Capabilities_t *);
} ISM330BX_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(ISM330BX_Object_t *);
  int32_t (*Disable)(ISM330BX_Object_t *);
  int32_t (*GetSensitivity)(ISM330BX_Object_t *, float *);
  int32_t (*GetOutputDataRate)(ISM330BX_Object_t *, float *);
  int32_t (*SetOutputDataRate)(ISM330BX_Object_t *, float);
  int32_t (*GetFullScale)(ISM330BX_Object_t *, int32_t *);
  int32_t (*SetFullScale)(ISM330BX_Object_t *, int32_t);
  int32_t (*GetAxes)(ISM330BX_Object_t *, ISM330BX_Axes_t *);
  int32_t (*GetAxesRaw)(ISM330BX_Object_t *, ISM330BX_AxesRaw_t *);
} ISM330BX_ACC_Drv_t;

typedef struct
{
  int32_t (*Enable)(ISM330BX_Object_t *);
  int32_t (*Disable)(ISM330BX_Object_t *);
  int32_t (*GetSensitivity)(ISM330BX_Object_t *, float *);
  int32_t (*GetOutputDataRate)(ISM330BX_Object_t *, float *);
  int32_t (*SetOutputDataRate)(ISM330BX_Object_t *, float);
  int32_t (*GetFullScale)(ISM330BX_Object_t *, int32_t *);
  int32_t (*SetFullScale)(ISM330BX_Object_t *, int32_t);
  int32_t (*GetAxes)(ISM330BX_Object_t *, ISM330BX_Axes_t *);
  int32_t (*GetAxesRaw)(ISM330BX_Object_t *, ISM330BX_AxesRaw_t *);
} ISM330BX_GYRO_Drv_t;

typedef union{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} ism330bx_axis3bit16_t;

typedef union{
  int16_t i16bit;
  uint8_t u8bit[2];
} ism330bx_axis1bit16_t;

typedef union{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} ism330bx_axis3bit32_t;

typedef union{
  int32_t i32bit;
  uint8_t u8bit[4];
} ism330bx_axis1bit32_t;

/**
 * @}
 */

/** @defgroup ISM330BX_Exported_Constants ISM330BX Exported Constants
 * @{
 */

#define ISM330BX_OK                       0
#define ISM330BX_ERROR                   -1

#define ISM330BX_I2C_BUS                 0U
#define ISM330BX_SPI_4WIRES_BUS          1U
#define ISM330BX_SPI_3WIRES_BUS          2U
#define ISM330BX_I3C_BUS                 3U

#define ISM330BX_ACC_SENSITIVITY_FS_2G   0.061f
#define ISM330BX_ACC_SENSITIVITY_FS_4G   0.122f
#define ISM330BX_ACC_SENSITIVITY_FS_8G   0.244f
#define ISM330BX_ACC_SENSITIVITY_FS_16G  0.488f

#define ISM330BX_GYRO_SENSITIVITY_FS_125DPS     4.375f
#define ISM330BX_GYRO_SENSITIVITY_FS_250DPS     8.750f
#define ISM330BX_GYRO_SENSITIVITY_FS_500DPS    17.500f
#define ISM330BX_GYRO_SENSITIVITY_FS_1000DPS   35.000f
#define ISM330BX_GYRO_SENSITIVITY_FS_2000DPS   70.000f
#define ISM330BX_GYRO_SENSITIVITY_FS_4000DPS  140.000f

#define ISM330BX_QVAR_GAIN  78.000f

/**
 * @}
 */

/** @addtogroup ISM330BX_Exported_Functions ISM330BX Exported Functions
 * @{
 */

int32_t ISM330BX_RegisterBusIO(ISM330BX_Object_t *pObj, ISM330BX_IO_t *pIO);
int32_t ISM330BX_Init(ISM330BX_Object_t *pObj);
int32_t ISM330BX_DeInit(ISM330BX_Object_t *pObj);
int32_t ISM330BX_ReadID(ISM330BX_Object_t *pObj, uint8_t *Id);
int32_t ISM330BX_GetCapabilities(ISM330BX_Object_t *pObj, ISM330BX_Capabilities_t *Capabilities);

int32_t ISM330BX_ACC_Enable(ISM330BX_Object_t *pObj);
int32_t ISM330BX_ACC_Disable(ISM330BX_Object_t *pObj);
int32_t ISM330BX_ACC_GetSensitivity(ISM330BX_Object_t *pObj, float *Sensitivity);
int32_t ISM330BX_ACC_GetOutputDataRate(ISM330BX_Object_t *pObj, float *Odr);
int32_t ISM330BX_ACC_SetOutputDataRate(ISM330BX_Object_t *pObj, float Odr);
int32_t ISM330BX_ACC_SetOutputDataRate_With_Mode(ISM330BX_Object_t *pObj, float Odr, ISM330BX_ACC_Operating_Mode_t Mode);
int32_t ISM330BX_ACC_GetFullScale(ISM330BX_Object_t *pObj, int32_t *FullScale);
int32_t ISM330BX_ACC_SetFullScale(ISM330BX_Object_t *pObj, int32_t FullScale);
int32_t ISM330BX_ACC_GetAxesRaw(ISM330BX_Object_t *pObj, ISM330BX_AxesRaw_t *Value);
int32_t ISM330BX_ACC_GetAxes(ISM330BX_Object_t *pObj, ISM330BX_Axes_t *Acceleration);

int32_t ISM330BX_GYRO_Enable(ISM330BX_Object_t *pObj);
int32_t ISM330BX_GYRO_Disable(ISM330BX_Object_t *pObj);
int32_t ISM330BX_GYRO_GetSensitivity(ISM330BX_Object_t *pObj, float *Sensitivity);
int32_t ISM330BX_GYRO_GetOutputDataRate(ISM330BX_Object_t *pObj, float *Odr);
int32_t ISM330BX_GYRO_SetOutputDataRate(ISM330BX_Object_t *pObj, float Odr);
int32_t ISM330BX_GYRO_SetOutputDataRate_With_Mode(ISM330BX_Object_t *pObj, float Odr, ISM330BX_GYRO_Operating_Mode_t Mode);
int32_t ISM330BX_GYRO_GetFullScale(ISM330BX_Object_t *pObj, int32_t *FullScale);
int32_t ISM330BX_GYRO_SetFullScale(ISM330BX_Object_t *pObj, int32_t FullScale);
int32_t ISM330BX_GYRO_GetAxesRaw(ISM330BX_Object_t *pObj, ISM330BX_AxesRaw_t *Value);
int32_t ISM330BX_GYRO_GetAxes(ISM330BX_Object_t *pObj, ISM330BX_Axes_t *AngularRate);

int32_t ISM330BX_Read_Reg(ISM330BX_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t ISM330BX_Write_Reg(ISM330BX_Object_t *pObj, uint8_t reg, uint8_t Data);

int32_t ISM330BX_ACC_Get_DRDY_Status(ISM330BX_Object_t *pObj, uint8_t *Status);
int32_t ISM330BX_GYRO_Get_DRDY_Status(ISM330BX_Object_t *pObj, uint8_t *Status);

int32_t ISM330BX_ACC_Set_Power_Mode(ISM330BX_Object_t *pObj, uint8_t PowerMode);
int32_t ISM330BX_GYRO_Set_Power_Mode(ISM330BX_Object_t *pObj, uint8_t PowerMode);
int32_t ISM330BX_ACC_Set_Filter_Mode(ISM330BX_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode);
int32_t ISM330BX_GYRO_Set_Filter_Mode(ISM330BX_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode);

int32_t ISM330BX_Set_Mem_Bank(ISM330BX_Object_t *pObj, uint8_t Val);

/**
 * @}
 */

/** @addtogroup ISM330BX_Exported_Variables ISM330BX Exported Variables
 * @{
 */

extern ISM330BX_CommonDrv_t ISM330BX_COMMON_Driver;
extern ISM330BX_ACC_Drv_t ISM330BX_ACC_Driver;
extern ISM330BX_GYRO_Drv_t ISM330BX_GYRO_Driver;

/**
 * @}
 */


typedef enum {
  ISM330BX_ULTRA_LIGHT  = 0,
  ISM330BX_VERY_LIGHT   = 1,
  ISM330BX_LIGHT        = 2,
  ISM330BX_MEDIUM       = 3,
  ISM330BX_STRONG       = 4,
  ISM330BX_VERY_STRONG  = 5,
  ISM330BX_AGGRESSIVE   = 6,
  ISM330BX_XTREME       = 7,
} ism330bx_ftype_t;

typedef enum {
  ISM330BX_HP_FILTER_NONE     = 0x00,
  ISM330BX_HP_FILTER_16mHz    = 0x80,
  ISM330BX_HP_FILTER_65mHz    = 0x81,
  ISM330BX_HP_FILTER_260mHz   = 0x82,
  ISM330BX_HP_FILTER_1Hz04    = 0x83,
} ism330bx_hpm_g_t;

typedef enum {
  ISM330BX_HP_PATH_DISABLE_ON_OUT    = 0x00,
  ISM330BX_SLOPE_ODR_DIV_4           = ISM330BX_GY_ULTRA_LIGHT,
  ISM330BX_HP_ODR_DIV_10             = ISM330BX_GY_VERY_LIGHT,
  ISM330BX_HP_ODR_DIV_20             = ISM330BX_GY_LIGHT,
  ISM330BX_HP_ODR_DIV_45             = ISM330BX_GY_MEDIUM,
  ISM330BX_HP_ODR_DIV_100            = ISM330BX_GY_STRONG,
  ISM330BX_HP_ODR_DIV_200            = ISM330BX_GY_VERY_STRONG,
  ISM330BX_HP_ODR_DIV_400            = ISM330BX_GY_AGGRESSIVE,
  ISM330BX_HP_ODR_DIV_800            = ISM330BX_GY_XTREME,

  ISM330BX_HP_REF_MD_ODR_DIV_10      = 0x31,
  ISM330BX_HP_REF_MD_ODR_DIV_20      = 0x32,
  ISM330BX_HP_REF_MD_ODR_DIV_45      = 0x33,
  ISM330BX_HP_REF_MD_ODR_DIV_100     = 0x34,
  ISM330BX_HP_REF_MD_ODR_DIV_200     = 0x35,
  ISM330BX_HP_REF_MD_ODR_DIV_400     = 0x36,
  ISM330BX_HP_REF_MD_ODR_DIV_800     = 0x37,
  ISM330BX_LP_ODR_DIV_10             = 0x01,
  ISM330BX_LP_ODR_DIV_20             = 0x02,
  ISM330BX_LP_ODR_DIV_45             = 0x03,
  ISM330BX_LP_ODR_DIV_100            = 0x04,
  ISM330BX_LP_ODR_DIV_200            = 0x05,
  ISM330BX_LP_ODR_DIV_400            = 0x06,
  ISM330BX_LP_ODR_DIV_800            = 0x07,
} ism330bx_hp_slope_xl_en_t;

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
