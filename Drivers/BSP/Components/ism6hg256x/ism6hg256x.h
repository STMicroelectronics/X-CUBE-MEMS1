/**
  ******************************************************************************
  * @file    ism6hg256x.h
  * @author  MEMS Software Solutions Team
  * @brief   ISM6HG256X header driver file
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
#ifndef ISM6HG256X_H
#define ISM6HG256X_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "ism6hg256x_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup ISM6HG256X ISM6HG256X
  * @{
  */

/** @defgroup ISM6HG256X_Exported_Types ISM6HG256X Exported Types
  * @{
  */

typedef int32_t (*ISM6HG256X_Init_Func)(void);
typedef int32_t (*ISM6HG256X_DeInit_Func)(void);
typedef int32_t (*ISM6HG256X_GetTick_Func)(void);
typedef void    (*ISM6HG256X_Delay_Func)(uint32_t);
typedef int32_t (*ISM6HG256X_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*ISM6HG256X_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  ISM6HG256X_INT1_PIN,
  ISM6HG256X_INT2_PIN,
} ISM6HG256X_SensorIntPin_t;

typedef enum
{
  ISM6HG256X_ACC_HIGH_PERFORMANCE_MODE,
  ISM6HG256X_ACC_HIGH_ACCURACY_ODR_MODE,
  ISM6HG256X_ACC_ODR_TRIGGERED_MODE,
  ISM6HG256X_ACC_LOW_POWER_MODE1,
  ISM6HG256X_ACC_LOW_POWER_MODE2,
  ISM6HG256X_ACC_LOW_POWER_MODE3,
  ISM6HG256X_ACC_NORMAL_MODE,
} ISM6HG256X_ACC_Operating_Mode_t;

typedef enum
{
  ISM6HG256X_GYRO_HIGH_PERFORMANCE_MODE,
  ISM6HG256X_GYRO_HIGH_ACCURACY_ODR_MODE,
  ISM6HG256X_GYRO_ODR_TRIGGERED_MODE,
  ISM6HG256X_GYRO_SLEEP_MODE,
  ISM6HG256X_GYRO_LOW_POWER_MODE,
} ISM6HG256X_GYRO_Operating_Mode_t;

typedef struct
{
  ISM6HG256X_Init_Func     Init;
  ISM6HG256X_DeInit_Func   DeInit;
  uint32_t                  BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires, 3 means I3C */
  uint8_t                   Address;
  ISM6HG256X_WriteReg_Func WriteReg;
  ISM6HG256X_ReadReg_Func  ReadReg;
  ISM6HG256X_GetTick_Func  GetTick;
  ISM6HG256X_Delay_Func    Delay;
} ISM6HG256X_IO_t;

typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} ISM6HG256X_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} ISM6HG256X_Axes_t;

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
} ISM6HG256X_Event_Status_t;

typedef struct
{
  ISM6HG256X_IO_t               IO;
  stmdev_ctx_t                  Ctx;
  uint8_t                       is_initialized;
  uint8_t                       acc_is_enabled;
  uint8_t                       acc_hg_is_enabled;
  uint8_t                       gyro_is_enabled;
  ism6hg256x_data_rate_t        acc_odr;
  ism6hg256x_hg_xl_data_rate_t  acc_hg_odr;
  ism6hg256x_data_rate_t        gyro_odr;
} ISM6HG256X_Object_t;

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
} ISM6HG256X_Capabilities_t;

typedef struct
{
  int32_t (*Init)(ISM6HG256X_Object_t *);
  int32_t (*DeInit)(ISM6HG256X_Object_t *);
  int32_t (*ReadID)(ISM6HG256X_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(ISM6HG256X_Object_t *, ISM6HG256X_Capabilities_t *);
} ISM6HG256X_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(ISM6HG256X_Object_t *);
  int32_t (*Disable)(ISM6HG256X_Object_t *);
  int32_t (*GetSensitivity)(ISM6HG256X_Object_t *, float_t *);
  int32_t (*GetOutputDataRate)(ISM6HG256X_Object_t *, float_t *);
  int32_t (*SetOutputDataRate)(ISM6HG256X_Object_t *, float_t);
  int32_t (*GetFullScale)(ISM6HG256X_Object_t *, int32_t *);
  int32_t (*SetFullScale)(ISM6HG256X_Object_t *, int32_t);
  int32_t (*GetAxes)(ISM6HG256X_Object_t *, ISM6HG256X_Axes_t *);
  int32_t (*GetAxesRaw)(ISM6HG256X_Object_t *, ISM6HG256X_AxesRaw_t *);
} ISM6HG256X_ACC_Drv_t;

typedef struct
{
  int32_t (*Enable)(ISM6HG256X_Object_t *);
  int32_t (*Disable)(ISM6HG256X_Object_t *);
  int32_t (*GetSensitivity)(ISM6HG256X_Object_t *, float_t *);
  int32_t (*GetOutputDataRate)(ISM6HG256X_Object_t *, float_t *);
  int32_t (*SetOutputDataRate)(ISM6HG256X_Object_t *, float_t);
  int32_t (*GetFullScale)(ISM6HG256X_Object_t *, int32_t *);
  int32_t (*SetFullScale)(ISM6HG256X_Object_t *, int32_t);
  int32_t (*GetAxes)(ISM6HG256X_Object_t *, ISM6HG256X_Axes_t *);
  int32_t (*GetAxesRaw)(ISM6HG256X_Object_t *, ISM6HG256X_AxesRaw_t *);
} ISM6HG256X_GYRO_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} ism6hg256x_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} ism6hg256x_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} ism6hg256x_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} ism6hg256x_axis1bit32_t;

/**
  * @}
  */

/** @defgroup ISM6HG256X_Exported_Constants ISM6HG256X Exported Constants
  * @{
  */

#define ISM6HG256X_OK                       0
#define ISM6HG256X_ERROR                   -1

#define ISM6HG256X_I2C_BUS                 0U
#define ISM6HG256X_SPI_4WIRES_BUS          1U
#define ISM6HG256X_SPI_3WIRES_BUS          2U
#define ISM6HG256X_I3C_BUS                 3U

#define ISM6HG256X_ACC_SENSITIVITY_FS_2G    0.061f
#define ISM6HG256X_ACC_SENSITIVITY_FS_4G    0.122f
#define ISM6HG256X_ACC_SENSITIVITY_FS_8G    0.244f
#define ISM6HG256X_ACC_SENSITIVITY_FS_16G   0.488f
#define ISM6HG256X_ACC_SENSITIVITY_FS_32G   0.976f
#define ISM6HG256X_ACC_SENSITIVITY_FS_64G   1.952f
#define ISM6HG256X_ACC_SENSITIVITY_FS_128G  3.904f
#define ISM6HG256X_ACC_SENSITIVITY_FS_256G  7.808f

#define ISM6HG256X_GYRO_SENSITIVITY_FS_250DPS     8.750f
#define ISM6HG256X_GYRO_SENSITIVITY_FS_500DPS    17.500f
#define ISM6HG256X_GYRO_SENSITIVITY_FS_1000DPS   35.000f
#define ISM6HG256X_GYRO_SENSITIVITY_FS_2000DPS   70.000f
#define ISM6HG256X_GYRO_SENSITIVITY_FS_4000DPS  140.000f

/**
  * @}
  */

/** @addtogroup ISM6HG256X_Exported_Functions ISM6HG256X Exported Functions
  * @{
  */

int32_t ISM6HG256X_RegisterBusIO(ISM6HG256X_Object_t *pObj, ISM6HG256X_IO_t *pIO);
int32_t ISM6HG256X_Init(ISM6HG256X_Object_t *pObj);
int32_t ISM6HG256X_DeInit(ISM6HG256X_Object_t *pObj);
int32_t ISM6HG256X_ReadID(ISM6HG256X_Object_t *pObj, uint8_t *Id);
int32_t ISM6HG256X_GetCapabilities(ISM6HG256X_Object_t *pObj, ISM6HG256X_Capabilities_t *Capabilities);

int32_t ISM6HG256X_ACC_Enable(ISM6HG256X_Object_t *pObj);
int32_t ISM6HG256X_ACC_Disable(ISM6HG256X_Object_t *pObj);
int32_t ISM6HG256X_ACC_GetSensitivity(ISM6HG256X_Object_t *pObj, float_t *Sensitivity);
int32_t ISM6HG256X_ACC_GetOutputDataRate(ISM6HG256X_Object_t *pObj, float_t *Odr);
int32_t ISM6HG256X_ACC_SetOutputDataRate(ISM6HG256X_Object_t *pObj, float_t Odr);
int32_t ISM6HG256X_ACC_SetOutputDataRate_With_Mode(ISM6HG256X_Object_t *pObj, float_t Odr,
                                                   ISM6HG256X_ACC_Operating_Mode_t Mode);
int32_t ISM6HG256X_ACC_GetFullScale(ISM6HG256X_Object_t *pObj, int32_t *FullScale);
int32_t ISM6HG256X_ACC_SetFullScale(ISM6HG256X_Object_t *pObj, int32_t FullScale);
int32_t ISM6HG256X_ACC_GetAxesRaw(ISM6HG256X_Object_t *pObj, ISM6HG256X_AxesRaw_t *Value);
int32_t ISM6HG256X_ACC_GetAxes(ISM6HG256X_Object_t *pObj, ISM6HG256X_Axes_t *Acceleration);

int32_t ISM6HG256X_ACC_Get_Event_Status(ISM6HG256X_Object_t *pObj, ISM6HG256X_Event_Status_t *Status);

int32_t ISM6HG256X_ACC_Enable_Free_Fall_Detection(ISM6HG256X_Object_t *pObj, ISM6HG256X_SensorIntPin_t IntPin);
int32_t ISM6HG256X_ACC_Disable_Free_Fall_Detection(ISM6HG256X_Object_t *pObj);
int32_t ISM6HG256X_ACC_Set_Free_Fall_Threshold(ISM6HG256X_Object_t *pObj, uint8_t Threshold);
int32_t ISM6HG256X_ACC_Set_Free_Fall_Duration(ISM6HG256X_Object_t *pObj, uint8_t Duration);

int32_t ISM6HG256X_ACC_Enable_Wake_Up_Detection(ISM6HG256X_Object_t *pObj, ISM6HG256X_SensorIntPin_t IntPin);
int32_t ISM6HG256X_ACC_Disable_Wake_Up_Detection(ISM6HG256X_Object_t *pObj);
int32_t ISM6HG256X_ACC_Set_Wake_Up_Threshold(ISM6HG256X_Object_t *pObj, uint32_t Threshold);
int32_t ISM6HG256X_ACC_Set_Wake_Up_Duration(ISM6HG256X_Object_t *pObj, uint8_t Duration);

int32_t ISM6HG256X_ACC_Enable_Single_Tap_Detection(ISM6HG256X_Object_t *pObj, ISM6HG256X_SensorIntPin_t IntPin);
int32_t ISM6HG256X_ACC_Disable_Single_Tap_Detection(ISM6HG256X_Object_t *pObj);
int32_t ISM6HG256X_ACC_Enable_Double_Tap_Detection(ISM6HG256X_Object_t *pObj, ISM6HG256X_SensorIntPin_t IntPin);
int32_t ISM6HG256X_ACC_Disable_Double_Tap_Detection(ISM6HG256X_Object_t *pObj);
int32_t ISM6HG256X_ACC_Set_Tap_Threshold(ISM6HG256X_Object_t *pObj, uint8_t Threshold);
int32_t ISM6HG256X_ACC_Set_Tap_Shock_Time(ISM6HG256X_Object_t *pObj, uint8_t Time);
int32_t ISM6HG256X_ACC_Set_Tap_Quiet_Time(ISM6HG256X_Object_t *pObj, uint8_t Time);
int32_t ISM6HG256X_ACC_Set_Tap_Duration_Time(ISM6HG256X_Object_t *pObj, uint8_t Time);

int32_t ISM6HG256X_ACC_Enable_6D_Orientation(ISM6HG256X_Object_t *pObj, ISM6HG256X_SensorIntPin_t IntPin);
int32_t ISM6HG256X_ACC_Disable_6D_Orientation(ISM6HG256X_Object_t *pObj);
int32_t ISM6HG256X_ACC_Set_6D_Orientation_Threshold(ISM6HG256X_Object_t *pObj, uint8_t Threshold);
int32_t ISM6HG256X_ACC_Get_6D_Orientation_XL(ISM6HG256X_Object_t *pObj, uint8_t *XLow);
int32_t ISM6HG256X_ACC_Get_6D_Orientation_XH(ISM6HG256X_Object_t *pObj, uint8_t *XHigh);
int32_t ISM6HG256X_ACC_Get_6D_Orientation_YL(ISM6HG256X_Object_t *pObj, uint8_t *YLow);
int32_t ISM6HG256X_ACC_Get_6D_Orientation_YH(ISM6HG256X_Object_t *pObj, uint8_t *YHigh);
int32_t ISM6HG256X_ACC_Get_6D_Orientation_ZL(ISM6HG256X_Object_t *pObj, uint8_t *ZLow);
int32_t ISM6HG256X_ACC_Get_6D_Orientation_ZH(ISM6HG256X_Object_t *pObj, uint8_t *ZHigh);

int32_t ISM6HG256X_ACC_Enable_Tilt_Detection(ISM6HG256X_Object_t *pObj, ISM6HG256X_SensorIntPin_t IntPin);
int32_t ISM6HG256X_ACC_Disable_Tilt_Detection(ISM6HG256X_Object_t *pObj);

int32_t ISM6HG256X_ACC_Enable_Pedometer(ISM6HG256X_Object_t *pObj, ISM6HG256X_SensorIntPin_t IntPin);
int32_t ISM6HG256X_ACC_Disable_Pedometer(ISM6HG256X_Object_t *pObj);
int32_t ISM6HG256X_ACC_Get_Step_Count(ISM6HG256X_Object_t *pObj, uint16_t *StepCount);
int32_t ISM6HG256X_ACC_Step_Counter_Reset(ISM6HG256X_Object_t *pObj);

int32_t ISM6HG256X_ACC_HG_Enable(ISM6HG256X_Object_t *pObj);
int32_t ISM6HG256X_ACC_HG_Disable(ISM6HG256X_Object_t *pObj);
int32_t ISM6HG256X_ACC_HG_GetSensitivity(ISM6HG256X_Object_t *pObj, float_t *Sensitivity);
int32_t ISM6HG256X_ACC_HG_GetOutputDataRate(ISM6HG256X_Object_t *pObj, float_t *Odr);
int32_t ISM6HG256X_ACC_HG_SetOutputDataRate(ISM6HG256X_Object_t *pObj, float_t Odr);
int32_t ISM6HG256X_ACC_HG_GetFullScale(ISM6HG256X_Object_t *pObj, int32_t *FullScale);
int32_t ISM6HG256X_ACC_HG_SetFullScale(ISM6HG256X_Object_t *pObj, int32_t FullScale);
int32_t ISM6HG256X_ACC_HG_GetAxesRaw(ISM6HG256X_Object_t *pObj, ISM6HG256X_AxesRaw_t *Value);
int32_t ISM6HG256X_ACC_HG_GetAxes(ISM6HG256X_Object_t *pObj, ISM6HG256X_Axes_t *Acceleration);

int32_t ISM6HG256X_FIFO_Get_Num_Samples(ISM6HG256X_Object_t *pObj, uint16_t *NumSamples);
int32_t ISM6HG256X_FIFO_Get_Full_Status(ISM6HG256X_Object_t *pObj, uint8_t *Status);
int32_t ISM6HG256X_FIFO_Set_INT1_FIFO_Full(ISM6HG256X_Object_t *pObj, uint8_t Status);
int32_t ISM6HG256X_FIFO_Set_INT2_FIFO_Full(ISM6HG256X_Object_t *pObj, uint8_t Status);
int32_t ISM6HG256X_FIFO_Set_Watermark_Level(ISM6HG256X_Object_t *pObj, uint8_t Watermark);
int32_t ISM6HG256X_FIFO_Set_Stop_On_Fth(ISM6HG256X_Object_t *pObj, uint8_t Status);
int32_t ISM6HG256X_FIFO_Set_Mode(ISM6HG256X_Object_t *pObj, uint8_t Mode);
int32_t ISM6HG256X_FIFO_Get_Tag(ISM6HG256X_Object_t *pObj, uint8_t *Tag);
int32_t ISM6HG256X_FIFO_Get_Data(ISM6HG256X_Object_t *pObj, uint8_t *Data);
int32_t ISM6HG256X_FIFO_ACC_Get_Axes(ISM6HG256X_Object_t *pObj, ISM6HG256X_Axes_t *Acceleration);
int32_t ISM6HG256X_FIFO_ACC_Set_BDR(ISM6HG256X_Object_t *pObj, float_t Bdr);
int32_t ISM6HG256X_FIFO_GYRO_Get_Axes(ISM6HG256X_Object_t *pObj, ISM6HG256X_Axes_t *AngularVelocity);
int32_t ISM6HG256X_FIFO_GYRO_Set_BDR(ISM6HG256X_Object_t *pObj, float_t Bdr);

int32_t ISM6HG256X_GYRO_Enable(ISM6HG256X_Object_t *pObj);
int32_t ISM6HG256X_GYRO_Disable(ISM6HG256X_Object_t *pObj);
int32_t ISM6HG256X_GYRO_GetSensitivity(ISM6HG256X_Object_t *pObj, float_t *Sensitivity);
int32_t ISM6HG256X_GYRO_GetOutputDataRate(ISM6HG256X_Object_t *pObj, float_t *Odr);
int32_t ISM6HG256X_GYRO_SetOutputDataRate(ISM6HG256X_Object_t *pObj, float_t Odr);
int32_t ISM6HG256X_GYRO_SetOutputDataRate_With_Mode(ISM6HG256X_Object_t *pObj, float_t Odr,
                                                    ISM6HG256X_GYRO_Operating_Mode_t Mode);
int32_t ISM6HG256X_GYRO_GetFullScale(ISM6HG256X_Object_t *pObj, int32_t *FullScale);
int32_t ISM6HG256X_GYRO_SetFullScale(ISM6HG256X_Object_t *pObj, int32_t FullScale);
int32_t ISM6HG256X_GYRO_GetAxesRaw(ISM6HG256X_Object_t *pObj, ISM6HG256X_AxesRaw_t *Value);
int32_t ISM6HG256X_GYRO_GetAxes(ISM6HG256X_Object_t *pObj, ISM6HG256X_Axes_t *AngularRate);

int32_t ISM6HG256X_Read_Reg(ISM6HG256X_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t ISM6HG256X_Write_Reg(ISM6HG256X_Object_t *pObj, uint8_t reg, uint8_t Data);

int32_t ISM6HG256X_ACC_Get_DRDY_Status(ISM6HG256X_Object_t *pObj, uint8_t *Status);
int32_t ISM6HG256X_ACC_HG_Get_DRDY_Status(ISM6HG256X_Object_t *pObj, uint8_t *Status);
int32_t ISM6HG256X_GYRO_Get_DRDY_Status(ISM6HG256X_Object_t *pObj, uint8_t *Status);

int32_t ISM6HG256X_ACC_Set_Power_Mode(ISM6HG256X_Object_t *pObj, uint8_t PowerMode);
int32_t ISM6HG256X_GYRO_Set_Power_Mode(ISM6HG256X_Object_t *pObj, uint8_t PowerMode);
int32_t ISM6HG256X_ACC_Set_Filter_Mode(ISM6HG256X_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode);
int32_t ISM6HG256X_GYRO_Set_Filter_Mode(ISM6HG256X_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode);

int32_t ISM6HG256X_Set_Mem_Bank(ISM6HG256X_Object_t *pObj, uint8_t Val);

/**
  * @}
  */

/** @addtogroup ISM6HG256X_Exported_Variables ISM6HG256X Exported Variables
  * @{
  */

extern ISM6HG256X_CommonDrv_t ISM6HG256X_COMMON_Driver;
extern ISM6HG256X_ACC_Drv_t ISM6HG256X_ACC_Driver;
extern ISM6HG256X_GYRO_Drv_t ISM6HG256X_GYRO_Driver;

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
