/**
  ******************************************************************************
  * @file    lsm6dsv80x.h
  * @author  MEMS Software Solutions Team
  * @brief   LSM6DSV80X header driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LSM6DSV80X_H
#define LSM6DSV80X_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "lsm6dsv80x_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup LSM6DSV80X LSM6DSV80X
  * @{
  */

/** @defgroup LSM6DSV80X_Exported_Types LSM6DSV80X Exported Types
  * @{
  */

typedef int32_t (*LSM6DSV80X_Init_Func)(void);
typedef int32_t (*LSM6DSV80X_DeInit_Func)(void);
typedef int32_t (*LSM6DSV80X_GetTick_Func)(void);
typedef void    (*LSM6DSV80X_Delay_Func)(uint32_t);
typedef int32_t (*LSM6DSV80X_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*LSM6DSV80X_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  LSM6DSV80X_INT1_PIN,
  LSM6DSV80X_INT2_PIN,
} LSM6DSV80X_SensorIntPin_t;

typedef enum
{
  LSM6DSV80X_ACC_HIGH_PERFORMANCE_MODE,
  LSM6DSV80X_ACC_HIGH_ACCURACY_ODR_MODE,
  LSM6DSV80X_ACC_ODR_TRIGGERED_MODE,
  LSM6DSV80X_ACC_LOW_POWER_MODE1,
  LSM6DSV80X_ACC_LOW_POWER_MODE2,
  LSM6DSV80X_ACC_LOW_POWER_MODE3,
  LSM6DSV80X_ACC_NORMAL_MODE,
} LSM6DSV80X_ACC_Operating_Mode_t;

typedef enum
{
  LSM6DSV80X_GYRO_HIGH_PERFORMANCE_MODE,
  LSM6DSV80X_GYRO_HIGH_ACCURACY_ODR_MODE,
  LSM6DSV80X_GYRO_ODR_TRIGGERED_MODE,
  LSM6DSV80X_GYRO_SLEEP_MODE,
  LSM6DSV80X_GYRO_LOW_POWER_MODE,
} LSM6DSV80X_GYRO_Operating_Mode_t;

typedef struct
{
  LSM6DSV80X_Init_Func     Init;
  LSM6DSV80X_DeInit_Func   DeInit;
  uint32_t                 BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires, 3 means I3C */
  uint8_t                  Address;
  LSM6DSV80X_WriteReg_Func WriteReg;
  LSM6DSV80X_ReadReg_Func  ReadReg;
  LSM6DSV80X_GetTick_Func  GetTick;
  LSM6DSV80X_Delay_Func    Delay;
} LSM6DSV80X_IO_t;

typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} LSM6DSV80X_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} LSM6DSV80X_Axes_t;

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
} LSM6DSV80X_Event_Status_t;

typedef struct
{
  LSM6DSV80X_IO_t              IO;
  stmdev_ctx_t                 Ctx;
  uint8_t                      is_initialized;
  uint8_t                      acc_is_enabled;
  uint8_t                      acc_hg_is_enabled;
  uint8_t                      gyro_is_enabled;
  lsm6dsv80x_data_rate_t       acc_odr;
  lsm6dsv80x_hg_xl_data_rate_t acc_hg_odr;
  lsm6dsv80x_data_rate_t       gyro_odr;
} LSM6DSV80X_Object_t;

typedef struct
{
  uint8_t  Acc;
  uint8_t  Gyro;
  uint8_t  Magneto;
  uint8_t  LowPower;
  uint32_t GyroMaxFS;
  uint32_t AccMaxFS;
  uint32_t MagMaxFS;
  float_t  GyroMaxOdr;
  float_t  AccMaxOdr;
  float_t  MagMaxOdr;
} LSM6DSV80X_Capabilities_t;

typedef struct
{
  int32_t (*Init)(LSM6DSV80X_Object_t *);
  int32_t (*DeInit)(LSM6DSV80X_Object_t *);
  int32_t (*ReadID)(LSM6DSV80X_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(LSM6DSV80X_Object_t *, LSM6DSV80X_Capabilities_t *);
} LSM6DSV80X_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(LSM6DSV80X_Object_t *);
  int32_t (*Disable)(LSM6DSV80X_Object_t *);
  int32_t (*GetSensitivity)(LSM6DSV80X_Object_t *, float_t *);
  int32_t (*GetOutputDataRate)(LSM6DSV80X_Object_t *, float_t *);
  int32_t (*SetOutputDataRate)(LSM6DSV80X_Object_t *, float_t);
  int32_t (*GetFullScale)(LSM6DSV80X_Object_t *, int32_t *);
  int32_t (*SetFullScale)(LSM6DSV80X_Object_t *, int32_t);
  int32_t (*GetAxes)(LSM6DSV80X_Object_t *, LSM6DSV80X_Axes_t *);
  int32_t (*GetAxesRaw)(LSM6DSV80X_Object_t *, LSM6DSV80X_AxesRaw_t *);
} LSM6DSV80X_ACC_Drv_t;

typedef struct
{
  int32_t (*Enable)(LSM6DSV80X_Object_t *);
  int32_t (*Disable)(LSM6DSV80X_Object_t *);
  int32_t (*GetSensitivity)(LSM6DSV80X_Object_t *, float_t *);
  int32_t (*GetOutputDataRate)(LSM6DSV80X_Object_t *, float_t *);
  int32_t (*SetOutputDataRate)(LSM6DSV80X_Object_t *, float_t);
  int32_t (*GetFullScale)(LSM6DSV80X_Object_t *, int32_t *);
  int32_t (*SetFullScale)(LSM6DSV80X_Object_t *, int32_t);
  int32_t (*GetAxes)(LSM6DSV80X_Object_t *, LSM6DSV80X_Axes_t *);
  int32_t (*GetAxesRaw)(LSM6DSV80X_Object_t *, LSM6DSV80X_AxesRaw_t *);
} LSM6DSV80X_GYRO_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} lsm6dsv80x_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} lsm6dsv80x_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} lsm6dsv80x_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} lsm6dsv80x_axis1bit32_t;

/**
  * @}
  */

/** @defgroup LSM6DSV80X_Exported_Constants LSM6DSV80X Exported Constants
  * @{
  */

#define LSM6DSV80X_OK                       0
#define LSM6DSV80X_ERROR                   -1

#define LSM6DSV80X_I2C_BUS                 0U
#define LSM6DSV80X_SPI_4WIRES_BUS          1U
#define LSM6DSV80X_SPI_3WIRES_BUS          2U
#define LSM6DSV80X_I3C_BUS                 3U

#define LSM6DSV80X_ACC_SENSITIVITY_FS_2G   0.061f
#define LSM6DSV80X_ACC_SENSITIVITY_FS_4G   0.122f
#define LSM6DSV80X_ACC_SENSITIVITY_FS_8G   0.244f
#define LSM6DSV80X_ACC_SENSITIVITY_FS_16G  0.488f
#define LSM6DSV80X_ACC_SENSITIVITY_FS_32G  0.976f
#define LSM6DSV80X_ACC_SENSITIVITY_FS_64G  1.952f
#define LSM6DSV80X_ACC_SENSITIVITY_FS_80G  3.904f

#define LSM6DSV80X_GYRO_SENSITIVITY_FS_250DPS     8.750f
#define LSM6DSV80X_GYRO_SENSITIVITY_FS_500DPS    17.500f
#define LSM6DSV80X_GYRO_SENSITIVITY_FS_1000DPS   35.000f
#define LSM6DSV80X_GYRO_SENSITIVITY_FS_2000DPS   70.000f
#define LSM6DSV80X_GYRO_SENSITIVITY_FS_4000DPS  140.000f

/**
  * @}
  */

/** @addtogroup LSM6DSV80X_Exported_Functions LSM6DSV80X Exported Functions
  * @{
  */

int32_t LSM6DSV80X_RegisterBusIO(LSM6DSV80X_Object_t *pObj, LSM6DSV80X_IO_t *pIO);
int32_t LSM6DSV80X_Init(LSM6DSV80X_Object_t *pObj);
int32_t LSM6DSV80X_DeInit(LSM6DSV80X_Object_t *pObj);
int32_t LSM6DSV80X_ReadID(LSM6DSV80X_Object_t *pObj, uint8_t *Id);
int32_t LSM6DSV80X_GetCapabilities(LSM6DSV80X_Object_t *pObj, LSM6DSV80X_Capabilities_t *Capabilities);

int32_t LSM6DSV80X_ACC_Enable(LSM6DSV80X_Object_t *pObj);
int32_t LSM6DSV80X_ACC_Disable(LSM6DSV80X_Object_t *pObj);
int32_t LSM6DSV80X_ACC_GetSensitivity(LSM6DSV80X_Object_t *pObj, float_t *Sensitivity);
int32_t LSM6DSV80X_ACC_GetOutputDataRate(LSM6DSV80X_Object_t *pObj, float_t *Odr);
int32_t LSM6DSV80X_ACC_SetOutputDataRate(LSM6DSV80X_Object_t *pObj, float_t Odr);
int32_t LSM6DSV80X_ACC_SetOutputDataRate_With_Mode(LSM6DSV80X_Object_t *pObj, float_t Odr,
                                                   LSM6DSV80X_ACC_Operating_Mode_t Mode);
int32_t LSM6DSV80X_ACC_GetFullScale(LSM6DSV80X_Object_t *pObj, int32_t *FullScale);
int32_t LSM6DSV80X_ACC_SetFullScale(LSM6DSV80X_Object_t *pObj, int32_t FullScale);
int32_t LSM6DSV80X_ACC_GetAxesRaw(LSM6DSV80X_Object_t *pObj, LSM6DSV80X_AxesRaw_t *Value);
int32_t LSM6DSV80X_ACC_GetAxes(LSM6DSV80X_Object_t *pObj, LSM6DSV80X_Axes_t *Acceleration);

int32_t LSM6DSV80X_ACC_Get_Event_Status(LSM6DSV80X_Object_t *pObj, LSM6DSV80X_Event_Status_t *Status);

int32_t LSM6DSV80X_ACC_Enable_Free_Fall_Detection(LSM6DSV80X_Object_t *pObj, LSM6DSV80X_SensorIntPin_t IntPin);
int32_t LSM6DSV80X_ACC_Disable_Free_Fall_Detection(LSM6DSV80X_Object_t *pObj);
int32_t LSM6DSV80X_ACC_Set_Free_Fall_Threshold(LSM6DSV80X_Object_t *pObj, uint8_t Threshold);
int32_t LSM6DSV80X_ACC_Set_Free_Fall_Duration(LSM6DSV80X_Object_t *pObj, uint8_t Duration);

int32_t LSM6DSV80X_ACC_Enable_Wake_Up_Detection(LSM6DSV80X_Object_t *pObj, LSM6DSV80X_SensorIntPin_t IntPin);
int32_t LSM6DSV80X_ACC_Disable_Wake_Up_Detection(LSM6DSV80X_Object_t *pObj);
int32_t LSM6DSV80X_ACC_Set_Wake_Up_Threshold(LSM6DSV80X_Object_t *pObj, uint32_t Threshold);
int32_t LSM6DSV80X_ACC_Set_Wake_Up_Duration(LSM6DSV80X_Object_t *pObj, uint8_t Duration);

int32_t LSM6DSV80X_ACC_Enable_Single_Tap_Detection(LSM6DSV80X_Object_t *pObj, LSM6DSV80X_SensorIntPin_t IntPin);
int32_t LSM6DSV80X_ACC_Disable_Single_Tap_Detection(LSM6DSV80X_Object_t *pObj);
int32_t LSM6DSV80X_ACC_Enable_Double_Tap_Detection(LSM6DSV80X_Object_t *pObj, LSM6DSV80X_SensorIntPin_t IntPin);
int32_t LSM6DSV80X_ACC_Disable_Double_Tap_Detection(LSM6DSV80X_Object_t *pObj);
int32_t LSM6DSV80X_ACC_Set_Tap_Threshold(LSM6DSV80X_Object_t *pObj, uint8_t Threshold);
int32_t LSM6DSV80X_ACC_Set_Tap_Shock_Time(LSM6DSV80X_Object_t *pObj, uint8_t Time);
int32_t LSM6DSV80X_ACC_Set_Tap_Quiet_Time(LSM6DSV80X_Object_t *pObj, uint8_t Time);
int32_t LSM6DSV80X_ACC_Set_Tap_Duration_Time(LSM6DSV80X_Object_t *pObj, uint8_t Time);

int32_t LSM6DSV80X_ACC_Enable_6D_Orientation(LSM6DSV80X_Object_t *pObj, LSM6DSV80X_SensorIntPin_t IntPin);
int32_t LSM6DSV80X_ACC_Disable_6D_Orientation(LSM6DSV80X_Object_t *pObj);
int32_t LSM6DSV80X_ACC_Set_6D_Orientation_Threshold(LSM6DSV80X_Object_t *pObj, uint8_t Threshold);
int32_t LSM6DSV80X_ACC_Get_6D_Orientation_XL(LSM6DSV80X_Object_t *pObj, uint8_t *XLow);
int32_t LSM6DSV80X_ACC_Get_6D_Orientation_XH(LSM6DSV80X_Object_t *pObj, uint8_t *XHigh);
int32_t LSM6DSV80X_ACC_Get_6D_Orientation_YL(LSM6DSV80X_Object_t *pObj, uint8_t *YLow);
int32_t LSM6DSV80X_ACC_Get_6D_Orientation_YH(LSM6DSV80X_Object_t *pObj, uint8_t *YHigh);
int32_t LSM6DSV80X_ACC_Get_6D_Orientation_ZL(LSM6DSV80X_Object_t *pObj, uint8_t *ZLow);
int32_t LSM6DSV80X_ACC_Get_6D_Orientation_ZH(LSM6DSV80X_Object_t *pObj, uint8_t *ZHigh);

int32_t LSM6DSV80X_ACC_Enable_Tilt_Detection(LSM6DSV80X_Object_t *pObj, LSM6DSV80X_SensorIntPin_t IntPin);
int32_t LSM6DSV80X_ACC_Disable_Tilt_Detection(LSM6DSV80X_Object_t *pObj);

int32_t LSM6DSV80X_ACC_Enable_Pedometer(LSM6DSV80X_Object_t *pObj, LSM6DSV80X_SensorIntPin_t IntPin);
int32_t LSM6DSV80X_ACC_Disable_Pedometer(LSM6DSV80X_Object_t *pObj);
int32_t LSM6DSV80X_ACC_Get_Step_Count(LSM6DSV80X_Object_t *pObj, uint16_t *StepCount);
int32_t LSM6DSV80X_ACC_Step_Counter_Reset(LSM6DSV80X_Object_t *pObj);

int32_t LSM6DSV80X_ACC_HG_Enable(LSM6DSV80X_Object_t *pObj);
int32_t LSM6DSV80X_ACC_HG_Disable(LSM6DSV80X_Object_t *pObj);
int32_t LSM6DSV80X_ACC_HG_GetSensitivity(LSM6DSV80X_Object_t *pObj, float_t *Sensitivity);
int32_t LSM6DSV80X_ACC_HG_GetOutputDataRate(LSM6DSV80X_Object_t *pObj, float_t *Odr);
int32_t LSM6DSV80X_ACC_HG_SetOutputDataRate(LSM6DSV80X_Object_t *pObj, float_t Odr);
int32_t LSM6DSV80X_ACC_HG_GetFullScale(LSM6DSV80X_Object_t *pObj, int32_t *FullScale);
int32_t LSM6DSV80X_ACC_HG_SetFullScale(LSM6DSV80X_Object_t *pObj, int32_t FullScale);
int32_t LSM6DSV80X_ACC_HG_GetAxesRaw(LSM6DSV80X_Object_t *pObj, LSM6DSV80X_AxesRaw_t *Value);
int32_t LSM6DSV80X_ACC_HG_GetAxes(LSM6DSV80X_Object_t *pObj, LSM6DSV80X_Axes_t *Acceleration);

int32_t LSM6DSV80X_FIFO_Get_Num_Samples(LSM6DSV80X_Object_t *pObj, uint16_t *NumSamples);
int32_t LSM6DSV80X_FIFO_Get_Full_Status(LSM6DSV80X_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSV80X_FIFO_Set_INT1_FIFO_Full(LSM6DSV80X_Object_t *pObj, uint8_t Status);
int32_t LSM6DSV80X_FIFO_Set_INT2_FIFO_Full(LSM6DSV80X_Object_t *pObj, uint8_t Status);
int32_t LSM6DSV80X_FIFO_Set_Watermark_Level(LSM6DSV80X_Object_t *pObj, uint8_t Watermark);
int32_t LSM6DSV80X_FIFO_Set_Stop_On_Fth(LSM6DSV80X_Object_t *pObj, uint8_t Status);
int32_t LSM6DSV80X_FIFO_Set_Mode(LSM6DSV80X_Object_t *pObj, uint8_t Mode);
int32_t LSM6DSV80X_FIFO_Get_Tag(LSM6DSV80X_Object_t *pObj, uint8_t *Tag);
int32_t LSM6DSV80X_FIFO_Get_Data(LSM6DSV80X_Object_t *pObj, uint8_t *Data);
int32_t LSM6DSV80X_FIFO_ACC_Get_Axes(LSM6DSV80X_Object_t *pObj, LSM6DSV80X_Axes_t *Acceleration);
int32_t LSM6DSV80X_FIFO_ACC_Set_BDR(LSM6DSV80X_Object_t *pObj, float_t Bdr);
int32_t LSM6DSV80X_FIFO_GYRO_Get_Axes(LSM6DSV80X_Object_t *pObj, LSM6DSV80X_Axes_t *AngularVelocity);
int32_t LSM6DSV80X_FIFO_GYRO_Set_BDR(LSM6DSV80X_Object_t *pObj, float_t Bdr);

int32_t LSM6DSV80X_GYRO_Enable(LSM6DSV80X_Object_t *pObj);
int32_t LSM6DSV80X_GYRO_Disable(LSM6DSV80X_Object_t *pObj);
int32_t LSM6DSV80X_GYRO_GetSensitivity(LSM6DSV80X_Object_t *pObj, float_t *Sensitivity);
int32_t LSM6DSV80X_GYRO_GetOutputDataRate(LSM6DSV80X_Object_t *pObj, float_t *Odr);
int32_t LSM6DSV80X_GYRO_SetOutputDataRate(LSM6DSV80X_Object_t *pObj, float_t Odr);
int32_t LSM6DSV80X_GYRO_SetOutputDataRate_With_Mode(LSM6DSV80X_Object_t *pObj, float_t Odr,
                                                    LSM6DSV80X_GYRO_Operating_Mode_t Mode);
int32_t LSM6DSV80X_GYRO_GetFullScale(LSM6DSV80X_Object_t *pObj, int32_t *FullScale);
int32_t LSM6DSV80X_GYRO_SetFullScale(LSM6DSV80X_Object_t *pObj, int32_t FullScale);
int32_t LSM6DSV80X_GYRO_GetAxesRaw(LSM6DSV80X_Object_t *pObj, LSM6DSV80X_AxesRaw_t *Value);
int32_t LSM6DSV80X_GYRO_GetAxes(LSM6DSV80X_Object_t *pObj, LSM6DSV80X_Axes_t *AngularRate);

int32_t LSM6DSV80X_Read_Reg(LSM6DSV80X_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t LSM6DSV80X_Write_Reg(LSM6DSV80X_Object_t *pObj, uint8_t reg, uint8_t Data);

int32_t LSM6DSV80X_ACC_Get_DRDY_Status(LSM6DSV80X_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSV80X_ACC_HG_Get_DRDY_Status(LSM6DSV80X_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSV80X_GYRO_Get_DRDY_Status(LSM6DSV80X_Object_t *pObj, uint8_t *Status);

int32_t LSM6DSV80X_ACC_Set_Power_Mode(LSM6DSV80X_Object_t *pObj, uint8_t PowerMode);
int32_t LSM6DSV80X_GYRO_Set_Power_Mode(LSM6DSV80X_Object_t *pObj, uint8_t PowerMode);
int32_t LSM6DSV80X_ACC_Set_Filter_Mode(LSM6DSV80X_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode);
int32_t LSM6DSV80X_GYRO_Set_Filter_Mode(LSM6DSV80X_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode);

int32_t LSM6DSV80X_Set_Mem_Bank(LSM6DSV80X_Object_t *pObj, uint8_t Val);

/**
  * @}
  */

/** @addtogroup LSM6DSV80X_Exported_Variables LSM6DSV80X Exported Variables
  * @{
  */

extern LSM6DSV80X_CommonDrv_t LSM6DSV80X_COMMON_Driver;
extern LSM6DSV80X_ACC_Drv_t LSM6DSV80X_ACC_Driver;
extern LSM6DSV80X_GYRO_Drv_t LSM6DSV80X_GYRO_Driver;

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
