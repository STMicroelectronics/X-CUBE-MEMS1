/**
  ******************************************************************************
  * @file    lsm6dsv16x.h
  * @author  MEMS Software Solutions Team
  * @brief   LSM6DSV16X header driver file
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
#ifndef LSM6DSV16X_H
#define LSM6DSV16X_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "lsm6dsv16x_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup LSM6DSV16X LSM6DSV16X
  * @{
  */

/** @defgroup LSM6DSV16X_Exported_Types LSM6DSV16X Exported Types
  * @{
  */

typedef int32_t (*LSM6DSV16X_Init_Func)(void);
typedef int32_t (*LSM6DSV16X_DeInit_Func)(void);
typedef int32_t (*LSM6DSV16X_GetTick_Func)(void);
typedef void (*LSM6DSV16X_Delay_Func)(uint32_t);
typedef int32_t (*LSM6DSV16X_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*LSM6DSV16X_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  LSM6DSV16X_INT1_PIN,
  LSM6DSV16X_INT2_PIN,
} LSM6DSV16X_SensorIntPin_t;

typedef enum
{
  LSM6DSV16X_ACC_HIGH_PERFORMANCE_MODE,
  LSM6DSV16X_ACC_HIGH_ACCURACY_MODE,
  LSM6DSV16X_ACC_NORMAL_MODE,
  LSM6DSV16X_ACC_LOW_POWER_MODE1,
  LSM6DSV16X_ACC_LOW_POWER_MODE2,
  LSM6DSV16X_ACC_LOW_POWER_MODE3
} LSM6DSV16X_ACC_Operating_Mode_t;

typedef enum
{
  LSM6DSV16X_GYRO_HIGH_PERFORMANCE_MODE,
  LSM6DSV16X_GYRO_HIGH_ACCURACY_MODE,
  LSM6DSV16X_GYRO_SLEEP_MODE,
  LSM6DSV16X_GYRO_LOW_POWER_MODE
} LSM6DSV16X_GYRO_Operating_Mode_t;

typedef enum
{
  LSM6DSV16X_ULTRA_LIGHT  = 0,
  LSM6DSV16X_VERY_LIGHT   = 1,
  LSM6DSV16X_LIGHT        = 2,
  LSM6DSV16X_MEDIUM       = 3,
  LSM6DSV16X_STRONG       = 4,
  LSM6DSV16X_VERY_STRONG  = 5,
  LSM6DSV16X_AGGRESSIVE   = 6,
  LSM6DSV16X_XTREME       = 7,
} lsm6dsv16x_ftype_t;

typedef enum
{
  LSM6DSV16X_HP_FILTER_NONE     = 0x00,
  LSM6DSV16X_HP_FILTER_16mHz    = 0x80,
  LSM6DSV16X_HP_FILTER_65mHz    = 0x81,
  LSM6DSV16X_HP_FILTER_260mHz   = 0x82,
  LSM6DSV16X_HP_FILTER_1Hz04    = 0x83,
} lsm6dsv16x_hpm_g_t;

typedef enum
{
  LSM6DSV16X_HP_PATH_DISABLE_ON_OUT    = 0x00,
  LSM6DSV16X_SLOPE_ODR_DIV_4           = LSM6DSV16X_GY_ULTRA_LIGHT,
  LSM6DSV16X_HP_ODR_DIV_10             = LSM6DSV16X_GY_VERY_LIGHT,
  LSM6DSV16X_HP_ODR_DIV_20             = LSM6DSV16X_GY_LIGHT,
  LSM6DSV16X_HP_ODR_DIV_45             = LSM6DSV16X_GY_MEDIUM,
  LSM6DSV16X_HP_ODR_DIV_100            = LSM6DSV16X_GY_STRONG,
  LSM6DSV16X_HP_ODR_DIV_200            = LSM6DSV16X_GY_VERY_STRONG,
  LSM6DSV16X_HP_ODR_DIV_400            = LSM6DSV16X_GY_AGGRESSIVE,
  LSM6DSV16X_HP_ODR_DIV_800            = LSM6DSV16X_GY_XTREME,

  LSM6DSV16X_HP_REF_MD_ODR_DIV_10      = 0x31,
  LSM6DSV16X_HP_REF_MD_ODR_DIV_20      = 0x32,
  LSM6DSV16X_HP_REF_MD_ODR_DIV_45      = 0x33,
  LSM6DSV16X_HP_REF_MD_ODR_DIV_100     = 0x34,
  LSM6DSV16X_HP_REF_MD_ODR_DIV_200     = 0x35,
  LSM6DSV16X_HP_REF_MD_ODR_DIV_400     = 0x36,
  LSM6DSV16X_HP_REF_MD_ODR_DIV_800     = 0x37,
  LSM6DSV16X_LP_ODR_DIV_10             = 0x01,
  LSM6DSV16X_LP_ODR_DIV_20             = 0x02,
  LSM6DSV16X_LP_ODR_DIV_45             = 0x03,
  LSM6DSV16X_LP_ODR_DIV_100            = 0x04,
  LSM6DSV16X_LP_ODR_DIV_200            = 0x05,
  LSM6DSV16X_LP_ODR_DIV_400            = 0x06,
  LSM6DSV16X_LP_ODR_DIV_800            = 0x07,
} lsm6dsv16x_hp_slope_xl_en_t;

typedef struct
{
  LSM6DSV16X_Init_Func       Init;
  LSM6DSV16X_DeInit_Func     DeInit;
  uint32_t                   BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires, 3 means I3C */
  uint8_t                    Address;
  LSM6DSV16X_WriteReg_Func   WriteReg;
  LSM6DSV16X_ReadReg_Func    ReadReg;
  LSM6DSV16X_GetTick_Func    GetTick;
  LSM6DSV16X_Delay_Func      Delay;
} LSM6DSV16X_IO_t;

typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} LSM6DSV16X_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} LSM6DSV16X_Axes_t;

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
} LSM6DSV16X_Event_Status_t;

typedef struct
{
  LSM6DSV16X_IO_t        IO;
  stmdev_ctx_t           Ctx;
  uint8_t                is_initialized;
  uint8_t                acc_is_enabled;
  uint8_t                gyro_is_enabled;
  lsm6dsv16x_data_rate_t acc_odr;
  lsm6dsv16x_data_rate_t gyro_odr;
} LSM6DSV16X_Object_t;

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
} LSM6DSV16X_Capabilities_t;

typedef struct
{
  int32_t (*Init)(LSM6DSV16X_Object_t *);
  int32_t (*DeInit)(LSM6DSV16X_Object_t *);
  int32_t (*ReadID)(LSM6DSV16X_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(LSM6DSV16X_Object_t *, LSM6DSV16X_Capabilities_t *);
} LSM6DSV16X_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(LSM6DSV16X_Object_t *);
  int32_t (*Disable)(LSM6DSV16X_Object_t *);
  int32_t (*GetSensitivity)(LSM6DSV16X_Object_t *, float *);
  int32_t (*GetOutputDataRate)(LSM6DSV16X_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LSM6DSV16X_Object_t *, float);
  int32_t (*GetFullScale)(LSM6DSV16X_Object_t *, int32_t *);
  int32_t (*SetFullScale)(LSM6DSV16X_Object_t *, int32_t);
  int32_t (*GetAxes)(LSM6DSV16X_Object_t *, LSM6DSV16X_Axes_t *);
  int32_t (*GetAxesRaw)(LSM6DSV16X_Object_t *, LSM6DSV16X_AxesRaw_t *);
} LSM6DSV16X_ACC_Drv_t;

typedef struct
{
  int32_t (*Enable)(LSM6DSV16X_Object_t *);
  int32_t (*Disable)(LSM6DSV16X_Object_t *);
  int32_t (*GetSensitivity)(LSM6DSV16X_Object_t *, float *);
  int32_t (*GetOutputDataRate)(LSM6DSV16X_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LSM6DSV16X_Object_t *, float);
  int32_t (*GetFullScale)(LSM6DSV16X_Object_t *, int32_t *);
  int32_t (*SetFullScale)(LSM6DSV16X_Object_t *, int32_t);
  int32_t (*GetAxes)(LSM6DSV16X_Object_t *, LSM6DSV16X_Axes_t *);
  int32_t (*GetAxesRaw)(LSM6DSV16X_Object_t *, LSM6DSV16X_AxesRaw_t *);
} LSM6DSV16X_GYRO_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} lsm6dsv16x_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} lsm6dsv16x_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} lsm6dsv16x_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} lsm6dsv16x_axis1bit32_t;

/**
  * @}
  */

/** @defgroup LSM6DSV16X_Exported_Constants LSM6DSV16X Exported Constants
  * @{
  */

#define LSM6DSV16X_OK                       0
#define LSM6DSV16X_ERROR                   -1

#define LSM6DSV16X_I2C_BUS                 0U
#define LSM6DSV16X_SPI_4WIRES_BUS          1U
#define LSM6DSV16X_SPI_3WIRES_BUS          2U
#define LSM6DSV16X_I3C_BUS                 3U

#define LSM6DSV16X_ACC_SENSITIVITY_FS_2G   0.061f
#define LSM6DSV16X_ACC_SENSITIVITY_FS_4G   0.122f
#define LSM6DSV16X_ACC_SENSITIVITY_FS_8G   0.244f
#define LSM6DSV16X_ACC_SENSITIVITY_FS_16G  0.488f

#define LSM6DSV16X_GYRO_SENSITIVITY_FS_125DPS     4.375f
#define LSM6DSV16X_GYRO_SENSITIVITY_FS_250DPS     8.750f
#define LSM6DSV16X_GYRO_SENSITIVITY_FS_500DPS    17.500f
#define LSM6DSV16X_GYRO_SENSITIVITY_FS_1000DPS   35.000f
#define LSM6DSV16X_GYRO_SENSITIVITY_FS_2000DPS   70.000f
#define LSM6DSV16X_GYRO_SENSITIVITY_FS_4000DPS  140.000f

#define LSM6DSV16X_QVAR_GAIN  78.000f

/**
  * @}
  */

/** @addtogroup LSM6DSV16X_Exported_Functions LSM6DSV16X Exported Functions
  * @{
  */

int32_t LSM6DSV16X_RegisterBusIO(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_IO_t *pIO);
int32_t LSM6DSV16X_Init(LSM6DSV16X_Object_t *pObj);
int32_t LSM6DSV16X_DeInit(LSM6DSV16X_Object_t *pObj);
int32_t LSM6DSV16X_ReadID(LSM6DSV16X_Object_t *pObj, uint8_t *Id);
int32_t LSM6DSV16X_GetCapabilities(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_Capabilities_t *Capabilities);

int32_t LSM6DSV16X_ACC_Enable(LSM6DSV16X_Object_t *pObj);
int32_t LSM6DSV16X_ACC_Disable(LSM6DSV16X_Object_t *pObj);
int32_t LSM6DSV16X_ACC_GetSensitivity(LSM6DSV16X_Object_t *pObj, float *Sensitivity);
int32_t LSM6DSV16X_ACC_GetOutputDataRate(LSM6DSV16X_Object_t *pObj, float *Odr);
int32_t LSM6DSV16X_ACC_SetOutputDataRate(LSM6DSV16X_Object_t *pObj, float Odr);
int32_t LSM6DSV16X_ACC_SetOutputDataRate_With_Mode(LSM6DSV16X_Object_t *pObj, float Odr,
                                                   LSM6DSV16X_ACC_Operating_Mode_t Mode);
int32_t LSM6DSV16X_ACC_GetFullScale(LSM6DSV16X_Object_t *pObj, int32_t *FullScale);
int32_t LSM6DSV16X_ACC_SetFullScale(LSM6DSV16X_Object_t *pObj, int32_t FullScale);
int32_t LSM6DSV16X_ACC_GetAxesRaw(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_AxesRaw_t *Value);
int32_t LSM6DSV16X_ACC_GetAxes(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_Axes_t *Acceleration);

int32_t LSM6DSV16X_ACC_Get_Event_Status(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_Event_Status_t *Status);

int32_t LSM6DSV16X_ACC_Enable_Free_Fall_Detection(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_SensorIntPin_t IntPin);
int32_t LSM6DSV16X_ACC_Disable_Free_Fall_Detection(LSM6DSV16X_Object_t *pObj);
int32_t LSM6DSV16X_ACC_Set_Free_Fall_Threshold(LSM6DSV16X_Object_t *pObj, uint8_t Threshold);
int32_t LSM6DSV16X_ACC_Set_Free_Fall_Duration(LSM6DSV16X_Object_t *pObj, uint8_t Duration);

int32_t LSM6DSV16X_ACC_Enable_Wake_Up_Detection(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_SensorIntPin_t IntPin);
int32_t LSM6DSV16X_ACC_Disable_Wake_Up_Detection(LSM6DSV16X_Object_t *pObj);
int32_t LSM6DSV16X_ACC_Set_Wake_Up_Threshold(LSM6DSV16X_Object_t *pObj, uint32_t Threshold);
int32_t LSM6DSV16X_ACC_Set_Wake_Up_Duration(LSM6DSV16X_Object_t *pObj, uint8_t Duration);

int32_t LSM6DSV16X_ACC_Enable_Single_Tap_Detection(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_SensorIntPin_t IntPin);
int32_t LSM6DSV16X_ACC_Disable_Single_Tap_Detection(LSM6DSV16X_Object_t *pObj);
int32_t LSM6DSV16X_ACC_Enable_Double_Tap_Detection(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_SensorIntPin_t IntPin);
int32_t LSM6DSV16X_ACC_Disable_Double_Tap_Detection(LSM6DSV16X_Object_t *pObj);
int32_t LSM6DSV16X_ACC_Set_Tap_Threshold(LSM6DSV16X_Object_t *pObj, uint8_t Threshold);
int32_t LSM6DSV16X_ACC_Set_Tap_Shock_Time(LSM6DSV16X_Object_t *pObj, uint8_t Time);
int32_t LSM6DSV16X_ACC_Set_Tap_Quiet_Time(LSM6DSV16X_Object_t *pObj, uint8_t Time);
int32_t LSM6DSV16X_ACC_Set_Tap_Duration_Time(LSM6DSV16X_Object_t *pObj, uint8_t Time);

int32_t LSM6DSV16X_ACC_Enable_6D_Orientation(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_SensorIntPin_t IntPin);
int32_t LSM6DSV16X_ACC_Disable_6D_Orientation(LSM6DSV16X_Object_t *pObj);
int32_t LSM6DSV16X_ACC_Set_6D_Orientation_Threshold(LSM6DSV16X_Object_t *pObj, uint8_t Threshold);
int32_t LSM6DSV16X_ACC_Get_6D_Orientation_XL(LSM6DSV16X_Object_t *pObj, uint8_t *XLow);
int32_t LSM6DSV16X_ACC_Get_6D_Orientation_XH(LSM6DSV16X_Object_t *pObj, uint8_t *XHigh);
int32_t LSM6DSV16X_ACC_Get_6D_Orientation_YL(LSM6DSV16X_Object_t *pObj, uint8_t *YLow);
int32_t LSM6DSV16X_ACC_Get_6D_Orientation_YH(LSM6DSV16X_Object_t *pObj, uint8_t *YHigh);
int32_t LSM6DSV16X_ACC_Get_6D_Orientation_ZL(LSM6DSV16X_Object_t *pObj, uint8_t *ZLow);
int32_t LSM6DSV16X_ACC_Get_6D_Orientation_ZH(LSM6DSV16X_Object_t *pObj, uint8_t *ZHigh);

int32_t LSM6DSV16X_ACC_Enable_Tilt_Detection(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_SensorIntPin_t IntPin);
int32_t LSM6DSV16X_ACC_Disable_Tilt_Detection(LSM6DSV16X_Object_t *pObj);

int32_t LSM6DSV16X_ACC_Enable_Pedometer(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_SensorIntPin_t IntPin);
int32_t LSM6DSV16X_ACC_Disable_Pedometer(LSM6DSV16X_Object_t *pObj);
int32_t LSM6DSV16X_ACC_Get_Step_Count(LSM6DSV16X_Object_t *pObj, uint16_t *StepCount);
int32_t LSM6DSV16X_ACC_Step_Counter_Reset(LSM6DSV16X_Object_t *pObj);

int32_t LSM6DSV16X_FIFO_Get_Num_Samples(LSM6DSV16X_Object_t *pObj, uint16_t *NumSamples);
int32_t LSM6DSV16X_FIFO_Get_Full_Status(LSM6DSV16X_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSV16X_FIFO_Set_INT1_FIFO_Full(LSM6DSV16X_Object_t *pObj, uint8_t Status);
int32_t LSM6DSV16X_FIFO_Set_INT2_FIFO_Full(LSM6DSV16X_Object_t *pObj, uint8_t Status);
int32_t LSM6DSV16X_FIFO_Set_Watermark_Level(LSM6DSV16X_Object_t *pObj, uint8_t Watermark);
int32_t LSM6DSV16X_FIFO_Set_Stop_On_Fth(LSM6DSV16X_Object_t *pObj, uint8_t Status);
int32_t LSM6DSV16X_FIFO_Set_Mode(LSM6DSV16X_Object_t *pObj, uint8_t Mode);
int32_t LSM6DSV16X_FIFO_Get_Tag(LSM6DSV16X_Object_t *pObj, uint8_t *Tag);
int32_t LSM6DSV16X_FIFO_Get_Data(LSM6DSV16X_Object_t *pObj, uint8_t *Data);
int32_t LSM6DSV16X_FIFO_ACC_Get_Axes(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_Axes_t *Acceleration);
int32_t LSM6DSV16X_FIFO_ACC_Set_BDR(LSM6DSV16X_Object_t *pObj, float_t Bdr);
int32_t LSM6DSV16X_FIFO_GYRO_Get_Axes(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_Axes_t *AngularVelocity);
int32_t LSM6DSV16X_FIFO_GYRO_Set_BDR(LSM6DSV16X_Object_t *pObj, float_t Bdr);

int32_t LSM6DSV16X_GYRO_Enable(LSM6DSV16X_Object_t *pObj);
int32_t LSM6DSV16X_GYRO_Disable(LSM6DSV16X_Object_t *pObj);
int32_t LSM6DSV16X_GYRO_GetSensitivity(LSM6DSV16X_Object_t *pObj, float *Sensitivity);
int32_t LSM6DSV16X_GYRO_GetOutputDataRate(LSM6DSV16X_Object_t *pObj, float *Odr);
int32_t LSM6DSV16X_GYRO_SetOutputDataRate(LSM6DSV16X_Object_t *pObj, float Odr);
int32_t LSM6DSV16X_GYRO_SetOutputDataRate_With_Mode(LSM6DSV16X_Object_t *pObj, float Odr,
                                                    LSM6DSV16X_GYRO_Operating_Mode_t Mode);
int32_t LSM6DSV16X_GYRO_GetFullScale(LSM6DSV16X_Object_t *pObj, int32_t *FullScale);
int32_t LSM6DSV16X_GYRO_SetFullScale(LSM6DSV16X_Object_t *pObj, int32_t FullScale);
int32_t LSM6DSV16X_GYRO_GetAxesRaw(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_AxesRaw_t *Value);
int32_t LSM6DSV16X_GYRO_GetAxes(LSM6DSV16X_Object_t *pObj, LSM6DSV16X_Axes_t *AngularRate);

int32_t LSM6DSV16X_Read_Reg(LSM6DSV16X_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t LSM6DSV16X_Write_Reg(LSM6DSV16X_Object_t *pObj, uint8_t reg, uint8_t Data);

int32_t LSM6DSV16X_ACC_Get_DRDY_Status(LSM6DSV16X_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSV16X_GYRO_Get_DRDY_Status(LSM6DSV16X_Object_t *pObj, uint8_t *Status);

int32_t LSM6DSV16X_ACC_Set_Power_Mode(LSM6DSV16X_Object_t *pObj, uint8_t PowerMode);
int32_t LSM6DSV16X_GYRO_Set_Power_Mode(LSM6DSV16X_Object_t *pObj, uint8_t PowerMode);
int32_t LSM6DSV16X_ACC_Set_Filter_Mode(LSM6DSV16X_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode);
int32_t LSM6DSV16X_GYRO_Set_Filter_Mode(LSM6DSV16X_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode);

int32_t LSM6DSV16X_QVAR_GetData(LSM6DSV16X_Object_t *pObj, int16_t *Data);
int32_t LSM6DSV16X_QVAR_GetDRDY(LSM6DSV16X_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSV16X_QVAR_GetZIn(LSM6DSV16X_Object_t *pObj, uint16_t *ZInMOhm);
int32_t LSM6DSV16X_QVAR_SetZIn(LSM6DSV16X_Object_t *pObj, uint16_t ZInMOhm);

int32_t LSM6DSV16X_ACC_Enable_DRDY_On_INT1(LSM6DSV16X_Object_t *pObj);
int32_t LSM6DSV16X_ACC_Disable_DRDY_On_INT1(LSM6DSV16X_Object_t *pObj);
int32_t LSM6DSV16X_GYRO_Enable_DRDY_On_INT2(LSM6DSV16X_Object_t *pObj);
int32_t LSM6DSV16X_GYRO_Disable_DRDY_On_INT2(LSM6DSV16X_Object_t *pObj);
int32_t LSM6DSV16X_DRDY_Set_Mode(LSM6DSV16X_Object_t *pObj, uint8_t Mode);

int32_t LSM6DSV16X_Set_Mem_Bank(LSM6DSV16X_Object_t *pObj, uint8_t Val);

/**
  * @}
  */

/** @addtogroup LSM6DSV16X_Exported_Variables LSM6DSV16X Exported Variables
  * @{
  */

extern LSM6DSV16X_CommonDrv_t LSM6DSV16X_COMMON_Driver;
extern LSM6DSV16X_ACC_Drv_t LSM6DSV16X_ACC_Driver;
extern LSM6DSV16X_GYRO_Drv_t LSM6DSV16X_GYRO_Driver;

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
