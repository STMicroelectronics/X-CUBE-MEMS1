/**
 ******************************************************************************
 * @file    st1vafe6ax.h
 * @author  MEMS Software Solutions Team
 * @brief   ST1VAFE6AX header driver file
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
#ifndef ST1VAFE6AX_H
#define ST1VAFE6AX_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "st1vafe6ax_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @addtogroup ST1VAFE6AX ST1VAFE6AX
 * @{
 */

/** @defgroup ST1VAFE6AX_Exported_Types ST1VAFE6AX Exported Types
 * @{
 */

typedef int32_t (*ST1VAFE6AX_Init_Func)(void);
typedef int32_t (*ST1VAFE6AX_DeInit_Func)(void);
typedef int32_t (*ST1VAFE6AX_GetTick_Func)(void);
typedef void    (*ST1VAFE6AX_Delay_Func)(uint32_t);
typedef int32_t (*ST1VAFE6AX_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*ST1VAFE6AX_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  ST1VAFE6AX_INT1_PIN,
  ST1VAFE6AX_INT2_PIN,
} ST1VAFE6AX_SensorIntPin_t;

typedef enum
{
  ST1VAFE6AX_ACC_HIGH_PERFORMANCE_MODE,
  ST1VAFE6AX_ACC_HIGH_ACCURACY_MODE,
  ST1VAFE6AX_ACC_NORMAL_MODE,
  ST1VAFE6AX_ACC_LOW_POWER_MODE1,
  ST1VAFE6AX_ACC_LOW_POWER_MODE2,
  ST1VAFE6AX_ACC_LOW_POWER_MODE3
} ST1VAFE6AX_ACC_Operating_Mode_t;

typedef enum
{
  ST1VAFE6AX_GYRO_HIGH_PERFORMANCE_MODE,
  ST1VAFE6AX_GYRO_HIGH_ACCURACY_MODE,
  ST1VAFE6AX_GYRO_SLEEP_MODE,
  ST1VAFE6AX_GYRO_LOW_POWER_MODE
} ST1VAFE6AX_GYRO_Operating_Mode_t;

typedef struct
{
  ST1VAFE6AX_Init_Func       Init;
  ST1VAFE6AX_DeInit_Func     DeInit;
  uint32_t                    BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires, 3 means I3C */
  uint8_t                     Address;
  ST1VAFE6AX_WriteReg_Func   WriteReg;
  ST1VAFE6AX_ReadReg_Func    ReadReg;
  ST1VAFE6AX_GetTick_Func    GetTick;
  ST1VAFE6AX_Delay_Func      Delay;
} ST1VAFE6AX_IO_t;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} ST1VAFE6AX_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} ST1VAFE6AX_Axes_t;

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
} ST1VAFE6AX_Event_Status_t;

typedef struct
{
  ST1VAFE6AX_IO_t           IO;
  stmdev_ctx_t              Ctx;
  uint8_t                   is_initialized;
  uint8_t                   acc_is_enabled;
  uint8_t                   gyro_is_enabled;
  st1vafe6ax_xl_data_rate_t acc_odr;
  st1vafe6ax_gy_data_rate_t gyro_odr;
} ST1VAFE6AX_Object_t;

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
} ST1VAFE6AX_Capabilities_t;

typedef struct
{
  int32_t (*Init)(ST1VAFE6AX_Object_t *);
  int32_t (*DeInit)(ST1VAFE6AX_Object_t *);
  int32_t (*ReadID)(ST1VAFE6AX_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(ST1VAFE6AX_Object_t *, ST1VAFE6AX_Capabilities_t *);
} ST1VAFE6AX_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(ST1VAFE6AX_Object_t *);
  int32_t (*Disable)(ST1VAFE6AX_Object_t *);
  int32_t (*GetSensitivity)(ST1VAFE6AX_Object_t *, float *);
  int32_t (*GetOutputDataRate)(ST1VAFE6AX_Object_t *, float *);
  int32_t (*SetOutputDataRate)(ST1VAFE6AX_Object_t *, float);
  int32_t (*GetFullScale)(ST1VAFE6AX_Object_t *, int32_t *);
  int32_t (*SetFullScale)(ST1VAFE6AX_Object_t *, int32_t);
  int32_t (*GetAxes)(ST1VAFE6AX_Object_t *, ST1VAFE6AX_Axes_t *);
  int32_t (*GetAxesRaw)(ST1VAFE6AX_Object_t *, ST1VAFE6AX_AxesRaw_t *);
} ST1VAFE6AX_ACC_Drv_t;

typedef struct
{
  int32_t (*Enable)(ST1VAFE6AX_Object_t *);
  int32_t (*Disable)(ST1VAFE6AX_Object_t *);
  int32_t (*GetSensitivity)(ST1VAFE6AX_Object_t *, float *);
  int32_t (*GetOutputDataRate)(ST1VAFE6AX_Object_t *, float *);
  int32_t (*SetOutputDataRate)(ST1VAFE6AX_Object_t *, float);
  int32_t (*GetFullScale)(ST1VAFE6AX_Object_t *, int32_t *);
  int32_t (*SetFullScale)(ST1VAFE6AX_Object_t *, int32_t);
  int32_t (*GetAxes)(ST1VAFE6AX_Object_t *, ST1VAFE6AX_Axes_t *);
  int32_t (*GetAxesRaw)(ST1VAFE6AX_Object_t *, ST1VAFE6AX_AxesRaw_t *);
} ST1VAFE6AX_GYRO_Drv_t;

typedef union{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} st1vafe6ax_axis3bit16_t;

typedef union{
  int16_t i16bit;
  uint8_t u8bit[2];
} st1vafe6ax_axis1bit16_t;

typedef union{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} st1vafe6ax_axis3bit32_t;

typedef union{
  int32_t i32bit;
  uint8_t u8bit[4];
} st1vafe6ax_axis1bit32_t;

/**
 * @}
 */

/** @defgroup ST1VAFE6AX_Exported_Constants ST1VAFE6AX Exported Constants
 * @{
 */

#define ST1VAFE6AX_OK                       0
#define ST1VAFE6AX_ERROR                   -1

#define ST1VAFE6AX_I2C_BUS                 0U
#define ST1VAFE6AX_SPI_4WIRES_BUS          1U
#define ST1VAFE6AX_SPI_3WIRES_BUS          2U
#define ST1VAFE6AX_I3C_BUS                 3U

#define ST1VAFE6AX_ACC_SENSITIVITY_FS_2G   0.061f
#define ST1VAFE6AX_ACC_SENSITIVITY_FS_4G   0.122f
#define ST1VAFE6AX_ACC_SENSITIVITY_FS_8G   0.244f
#define ST1VAFE6AX_ACC_SENSITIVITY_FS_16G  0.488f

#define ST1VAFE6AX_GYRO_SENSITIVITY_FS_125DPS     4.375f
#define ST1VAFE6AX_GYRO_SENSITIVITY_FS_250DPS     8.750f
#define ST1VAFE6AX_GYRO_SENSITIVITY_FS_500DPS    17.500f
#define ST1VAFE6AX_GYRO_SENSITIVITY_FS_1000DPS   35.000f
#define ST1VAFE6AX_GYRO_SENSITIVITY_FS_2000DPS   70.000f
#define ST1VAFE6AX_GYRO_SENSITIVITY_FS_4000DPS  140.000f

#define ST1VAFE6AX_BIO_GAIN  78.000f

/**
 * @}
 */

/** @addtogroup ST1VAFE6AX_Exported_Functions ST1VAFE6AX Exported Functions
 * @{
 */

int32_t ST1VAFE6AX_RegisterBusIO(ST1VAFE6AX_Object_t *pObj, ST1VAFE6AX_IO_t *pIO);
int32_t ST1VAFE6AX_Init(ST1VAFE6AX_Object_t *pObj);
int32_t ST1VAFE6AX_DeInit(ST1VAFE6AX_Object_t *pObj);
int32_t ST1VAFE6AX_ReadID(ST1VAFE6AX_Object_t *pObj, uint8_t *Id);
int32_t ST1VAFE6AX_GetCapabilities(ST1VAFE6AX_Object_t *pObj, ST1VAFE6AX_Capabilities_t *Capabilities);

int32_t ST1VAFE6AX_ACC_Enable(ST1VAFE6AX_Object_t *pObj);
int32_t ST1VAFE6AX_ACC_Disable(ST1VAFE6AX_Object_t *pObj);
int32_t ST1VAFE6AX_ACC_GetSensitivity(ST1VAFE6AX_Object_t *pObj, float *Sensitivity);
int32_t ST1VAFE6AX_ACC_GetOutputDataRate(ST1VAFE6AX_Object_t *pObj, float *Odr);
int32_t ST1VAFE6AX_ACC_SetOutputDataRate(ST1VAFE6AX_Object_t *pObj, float Odr);
int32_t ST1VAFE6AX_ACC_SetOutputDataRate_With_Mode(ST1VAFE6AX_Object_t *pObj, float Odr, ST1VAFE6AX_ACC_Operating_Mode_t Mode);
int32_t ST1VAFE6AX_ACC_GetFullScale(ST1VAFE6AX_Object_t *pObj, int32_t *FullScale);
int32_t ST1VAFE6AX_ACC_SetFullScale(ST1VAFE6AX_Object_t *pObj, int32_t FullScale);
int32_t ST1VAFE6AX_ACC_GetAxesRaw(ST1VAFE6AX_Object_t *pObj, ST1VAFE6AX_AxesRaw_t *Value);
int32_t ST1VAFE6AX_ACC_GetAxes(ST1VAFE6AX_Object_t *pObj, ST1VAFE6AX_Axes_t *Acceleration);

int32_t ST1VAFE6AX_GYRO_Enable(ST1VAFE6AX_Object_t *pObj);
int32_t ST1VAFE6AX_GYRO_Disable(ST1VAFE6AX_Object_t *pObj);
int32_t ST1VAFE6AX_GYRO_GetSensitivity(ST1VAFE6AX_Object_t *pObj, float *Sensitivity);
int32_t ST1VAFE6AX_GYRO_GetOutputDataRate(ST1VAFE6AX_Object_t *pObj, float *Odr);
int32_t ST1VAFE6AX_GYRO_SetOutputDataRate(ST1VAFE6AX_Object_t *pObj, float Odr);
int32_t ST1VAFE6AX_GYRO_SetOutputDataRate_With_Mode(ST1VAFE6AX_Object_t *pObj, float Odr, ST1VAFE6AX_GYRO_Operating_Mode_t Mode);
int32_t ST1VAFE6AX_GYRO_GetFullScale(ST1VAFE6AX_Object_t *pObj, int32_t *FullScale);
int32_t ST1VAFE6AX_GYRO_SetFullScale(ST1VAFE6AX_Object_t *pObj, int32_t FullScale);
int32_t ST1VAFE6AX_GYRO_GetAxesRaw(ST1VAFE6AX_Object_t *pObj, ST1VAFE6AX_AxesRaw_t *Value);
int32_t ST1VAFE6AX_GYRO_GetAxes(ST1VAFE6AX_Object_t *pObj, ST1VAFE6AX_Axes_t *AngularRate);

int32_t ST1VAFE6AX_Read_Reg(ST1VAFE6AX_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t ST1VAFE6AX_Write_Reg(ST1VAFE6AX_Object_t *pObj, uint8_t reg, uint8_t Data);

int32_t ST1VAFE6AX_ACC_Get_DRDY_Status(ST1VAFE6AX_Object_t *pObj, uint8_t *Status);
int32_t ST1VAFE6AX_GYRO_Get_DRDY_Status(ST1VAFE6AX_Object_t *pObj, uint8_t *Status);

int32_t ST1VAFE6AX_ACC_Set_Power_Mode(ST1VAFE6AX_Object_t *pObj, uint8_t PowerMode);
int32_t ST1VAFE6AX_GYRO_Set_Power_Mode(ST1VAFE6AX_Object_t *pObj, uint8_t PowerMode);
int32_t ST1VAFE6AX_ACC_Set_Filter_Mode(ST1VAFE6AX_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode);
int32_t ST1VAFE6AX_GYRO_Set_Filter_Mode(ST1VAFE6AX_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode);

int32_t ST1VAFE6AX_Set_Mem_Bank(ST1VAFE6AX_Object_t *pObj, uint8_t Val);
int32_t ST1VAFE6AX_BIO_GetVafe(ST1VAFE6AX_Object_t *pObj, int16_t  *vafe);





/**
 * @}
 */

/** @addtogroup ST1VAFE6AX_Exported_Variables ST1VAFE6AX Exported Variables
 * @{
 */

extern ST1VAFE6AX_CommonDrv_t ST1VAFE6AX_COMMON_Driver;
extern ST1VAFE6AX_ACC_Drv_t ST1VAFE6AX_ACC_Driver;
extern ST1VAFE6AX_GYRO_Drv_t ST1VAFE6AX_GYRO_Driver;

/**
 * @}
 */


typedef enum {
  ST1VAFE6AX_ULTRA_LIGHT  = 0,
  ST1VAFE6AX_VERY_LIGHT   = 1,
  ST1VAFE6AX_LIGHT        = 2,
  ST1VAFE6AX_MEDIUM       = 3,
  ST1VAFE6AX_STRONG       = 4,
  ST1VAFE6AX_VERY_STRONG  = 5,
  ST1VAFE6AX_AGGRESSIVE   = 6,
  ST1VAFE6AX_XTREME       = 7,
} st1vafe6ax_ftype_t;

typedef enum {
  ST1VAFE6AX_HP_FILTER_NONE     = 0x00,
  ST1VAFE6AX_HP_FILTER_16mHz    = 0x80,
  ST1VAFE6AX_HP_FILTER_65mHz    = 0x81,
  ST1VAFE6AX_HP_FILTER_260mHz   = 0x82,
  ST1VAFE6AX_HP_FILTER_1Hz04    = 0x83,
} st1vafe6ax_hpm_g_t;

typedef enum {
  ST1VAFE6AX_HP_PATH_DISABLE_ON_OUT    = 0x00,
  ST1VAFE6AX_SLOPE_ODR_DIV_4           = ST1VAFE6AX_GY_ULTRA_LIGHT,
  ST1VAFE6AX_HP_ODR_DIV_10             = ST1VAFE6AX_GY_VERY_LIGHT,
  ST1VAFE6AX_HP_ODR_DIV_20             = ST1VAFE6AX_GY_LIGHT,
  ST1VAFE6AX_HP_ODR_DIV_45             = ST1VAFE6AX_GY_MEDIUM,
  ST1VAFE6AX_HP_ODR_DIV_100            = ST1VAFE6AX_GY_STRONG,
  ST1VAFE6AX_HP_ODR_DIV_200            = ST1VAFE6AX_GY_VERY_STRONG,
  ST1VAFE6AX_HP_ODR_DIV_400            = ST1VAFE6AX_GY_AGGRESSIVE,
  ST1VAFE6AX_HP_ODR_DIV_800            = ST1VAFE6AX_GY_XTREME,

  ST1VAFE6AX_HP_REF_MD_ODR_DIV_10      = 0x31,
  ST1VAFE6AX_HP_REF_MD_ODR_DIV_20      = 0x32,
  ST1VAFE6AX_HP_REF_MD_ODR_DIV_45      = 0x33,
  ST1VAFE6AX_HP_REF_MD_ODR_DIV_100     = 0x34,
  ST1VAFE6AX_HP_REF_MD_ODR_DIV_200     = 0x35,
  ST1VAFE6AX_HP_REF_MD_ODR_DIV_400     = 0x36,
  ST1VAFE6AX_HP_REF_MD_ODR_DIV_800     = 0x37,
  ST1VAFE6AX_LP_ODR_DIV_10             = 0x01,
  ST1VAFE6AX_LP_ODR_DIV_20             = 0x02,
  ST1VAFE6AX_LP_ODR_DIV_45             = 0x03,
  ST1VAFE6AX_LP_ODR_DIV_100            = 0x04,
  ST1VAFE6AX_LP_ODR_DIV_200            = 0x05,
  ST1VAFE6AX_LP_ODR_DIV_400            = 0x06,
  ST1VAFE6AX_LP_ODR_DIV_800            = 0x07,
} st1vafe6ax_hp_slope_xl_en_t;

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
