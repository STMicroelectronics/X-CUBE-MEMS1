/**
  ******************************************************************************
  * @file    ism330is.h
  * @author  MEMS Software Solutions Team
  * @brief   ISM330IS header driver file
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
#ifndef ISM330IS_H
#define ISM330IS_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "ism330is_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup ISM330IS ISM330IS
  * @{
  */

/** @defgroup ISM330IS_Exported_Types ISM330IS Exported Types
  * @{
  */

typedef int32_t (*ISM330IS_Init_Func)(void);
typedef int32_t (*ISM330IS_DeInit_Func)(void);
typedef int32_t (*ISM330IS_GetTick_Func)(void);
typedef void    (*ISM330IS_Delay_Func)(uint32_t);
typedef int32_t (*ISM330IS_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*ISM330IS_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  ISM330IS_INT1_PIN,
  ISM330IS_INT2_PIN,
} ISM330IS_SensorIntPin_t;

typedef struct
{
  ISM330IS_Init_Func         Init;
  ISM330IS_DeInit_Func       DeInit;
  uint32_t                   BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
  uint8_t                    Address;
  ISM330IS_WriteReg_Func     WriteReg;
  ISM330IS_ReadReg_Func      ReadReg;
  ISM330IS_GetTick_Func      GetTick;
  ISM330IS_Delay_Func        Delay;
} ISM330IS_IO_t;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} ISM330IS_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} ISM330IS_Axes_t;

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
} ISM330IS_Event_Status_t;

typedef struct
{
  ISM330IS_IO_t           IO;
  stmdev_ctx_t            Ctx;
  uint8_t                 is_initialized;
  uint8_t                 acc_is_enabled;
  uint8_t                 gyro_is_enabled;
  ism330is_xl_data_rate_t acc_odr;
  ism330is_gy_data_rate_t gyro_odr;
} ISM330IS_Object_t;

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
} ISM330IS_Capabilities_t;

typedef struct
{
  int32_t (*Init)(ISM330IS_Object_t *);
  int32_t (*DeInit)(ISM330IS_Object_t *);
  int32_t (*ReadID)(ISM330IS_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(ISM330IS_Object_t *, ISM330IS_Capabilities_t *);
} ISM330IS_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(ISM330IS_Object_t *);
  int32_t (*Disable)(ISM330IS_Object_t *);
  int32_t (*GetSensitivity)(ISM330IS_Object_t *, float *);
  int32_t (*GetOutputDataRate)(ISM330IS_Object_t *, float *);
  int32_t (*SetOutputDataRate)(ISM330IS_Object_t *, float);
  int32_t (*GetFullScale)(ISM330IS_Object_t *, int32_t *);
  int32_t (*SetFullScale)(ISM330IS_Object_t *, int32_t);
  int32_t (*GetAxes)(ISM330IS_Object_t *, ISM330IS_Axes_t *);
  int32_t (*GetAxesRaw)(ISM330IS_Object_t *, ISM330IS_AxesRaw_t *);
} ISM330IS_ACC_Drv_t;

typedef struct
{
  int32_t (*Enable)(ISM330IS_Object_t *);
  int32_t (*Disable)(ISM330IS_Object_t *);
  int32_t (*GetSensitivity)(ISM330IS_Object_t *, float *);
  int32_t (*GetOutputDataRate)(ISM330IS_Object_t *, float *);
  int32_t (*SetOutputDataRate)(ISM330IS_Object_t *, float);
  int32_t (*GetFullScale)(ISM330IS_Object_t *, int32_t *);
  int32_t (*SetFullScale)(ISM330IS_Object_t *, int32_t);
  int32_t (*GetAxes)(ISM330IS_Object_t *, ISM330IS_Axes_t *);
  int32_t (*GetAxesRaw)(ISM330IS_Object_t *, ISM330IS_AxesRaw_t *);
} ISM330IS_GYRO_Drv_t;

/**
  * @}
  */

/** @defgroup ISM330IS_Exported_Constants ISM330IS Exported Constants
  * @{
  */

#define ISM330IS_OK                       0
#define ISM330IS_ERROR                   -1

#define ISM330IS_I2C_BUS                 0U
#define ISM330IS_SPI_4WIRES_BUS          1U
#define ISM330IS_SPI_3WIRES_BUS          2U

#define ISM330IS_ACC_SENSITIVITY_FS_2G   0.061f
#define ISM330IS_ACC_SENSITIVITY_FS_4G   0.122f
#define ISM330IS_ACC_SENSITIVITY_FS_8G   0.244f
#define ISM330IS_ACC_SENSITIVITY_FS_16G  0.488f

#define ISM330IS_GYRO_SENSITIVITY_FS_125DPS    4.375f
#define ISM330IS_GYRO_SENSITIVITY_FS_250DPS    8.750f
#define ISM330IS_GYRO_SENSITIVITY_FS_500DPS   17.500f
#define ISM330IS_GYRO_SENSITIVITY_FS_1000DPS  35.000f
#define ISM330IS_GYRO_SENSITIVITY_FS_2000DPS  70.000f

/**
  * @}
  */

/** @addtogroup ISM330IS_Exported_Functions ISM330IS Exported Functions
  * @{
  */

int32_t ISM330IS_RegisterBusIO(ISM330IS_Object_t *pObj, ISM330IS_IO_t *pIO);
int32_t ISM330IS_Init(ISM330IS_Object_t *pObj);
int32_t ISM330IS_DeInit(ISM330IS_Object_t *pObj);
int32_t ISM330IS_ReadID(ISM330IS_Object_t *pObj, uint8_t *Id);
int32_t ISM330IS_GetCapabilities(ISM330IS_Object_t *pObj, ISM330IS_Capabilities_t *Capabilities);

int32_t ISM330IS_Read_Reg(ISM330IS_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t ISM330IS_Write_Reg(ISM330IS_Object_t *pObj, uint8_t reg, uint8_t Data);
int32_t ISM330IS_Set_Interrupt_Latch(ISM330IS_Object_t *pObj, uint8_t Status);

int32_t ISM330IS_ACC_Enable(ISM330IS_Object_t *pObj);
int32_t ISM330IS_ACC_Disable(ISM330IS_Object_t *pObj);
int32_t ISM330IS_ACC_GetSensitivity(ISM330IS_Object_t *pObj, float *Sensitivity);
int32_t ISM330IS_ACC_GetOutputDataRate(ISM330IS_Object_t *pObj, float *Odr);
int32_t ISM330IS_ACC_SetOutputDataRate(ISM330IS_Object_t *pObj, float Odr);
int32_t ISM330IS_ACC_GetFullScale(ISM330IS_Object_t *pObj, int32_t *FullScale);
int32_t ISM330IS_ACC_SetFullScale(ISM330IS_Object_t *pObj, int32_t FullScale);
int32_t ISM330IS_ACC_GetAxesRaw(ISM330IS_Object_t *pObj, ISM330IS_AxesRaw_t *Value);
int32_t ISM330IS_ACC_GetAxes(ISM330IS_Object_t *pObj, ISM330IS_Axes_t *Acceleration);

int32_t ISM330IS_GYRO_Enable(ISM330IS_Object_t *pObj);
int32_t ISM330IS_GYRO_Disable(ISM330IS_Object_t *pObj);
int32_t ISM330IS_GYRO_GetSensitivity(ISM330IS_Object_t *pObj, float *Sensitivity);
int32_t ISM330IS_GYRO_GetOutputDataRate(ISM330IS_Object_t *pObj, float *Odr);
int32_t ISM330IS_GYRO_SetOutputDataRate(ISM330IS_Object_t *pObj, float Odr);
int32_t ISM330IS_GYRO_GetFullScale(ISM330IS_Object_t *pObj, int32_t *FullScale);
int32_t ISM330IS_GYRO_SetFullScale(ISM330IS_Object_t *pObj, int32_t FullScale);
int32_t ISM330IS_GYRO_GetAxesRaw(ISM330IS_Object_t *pObj, ISM330IS_AxesRaw_t *Value);
int32_t ISM330IS_GYRO_GetAxes(ISM330IS_Object_t *pObj, ISM330IS_Axes_t *AngularRate);

int32_t ISM330IS_ACC_Enable_Free_Fall_Detection(ISM330IS_Object_t *pObj, ISM330IS_SensorIntPin_t IntPin);
int32_t ISM330IS_ACC_Disable_Free_Fall_Detection(ISM330IS_Object_t *pObj);
int32_t ISM330IS_ACC_Set_Free_Fall_Threshold(ISM330IS_Object_t *pObj, uint8_t Threshold);
int32_t ISM330IS_ACC_Set_Free_Fall_Duration(ISM330IS_Object_t *pObj, uint8_t Duration);

int32_t ISM330IS_ACC_Enable_Pedometer(ISM330IS_Object_t *pObj);
int32_t ISM330IS_ACC_Disable_Pedometer(ISM330IS_Object_t *pObj);
int32_t ISM330IS_ACC_Get_Step_Count(ISM330IS_Object_t *pObj, uint16_t *StepCount);
int32_t ISM330IS_ACC_Enable_Step_Counter_Reset(ISM330IS_Object_t *pObj);
int32_t ISM330IS_ACC_Disable_Step_Counter_Reset(ISM330IS_Object_t *pObj);
int32_t ISM330IS_ACC_Set_Pedometer_Threshold(ISM330IS_Object_t *pObj, uint8_t Threshold);

int32_t ISM330IS_ACC_Enable_Tilt_Detection(ISM330IS_Object_t *pObj, ISM330IS_SensorIntPin_t IntPin);
int32_t ISM330IS_ACC_Disable_Tilt_Detection(ISM330IS_Object_t *pObj);

int32_t ISM330IS_ACC_Enable_Wake_Up_Detection(ISM330IS_Object_t *pObj, ISM330IS_SensorIntPin_t IntPin);
int32_t ISM330IS_ACC_Disable_Wake_Up_Detection(ISM330IS_Object_t *pObj);
int32_t ISM330IS_ACC_Set_Wake_Up_Threshold(ISM330IS_Object_t *pObj, uint8_t Threshold);
int32_t ISM330IS_ACC_Set_Wake_Up_Duration(ISM330IS_Object_t *pObj, uint8_t Duration);

int32_t ISM330IS_ACC_Enable_Inactivity_Detection(ISM330IS_Object_t *pObj, ISM330IS_SensorIntPin_t IntPin);
int32_t ISM330IS_ACC_Disable_Inactivity_Detection(ISM330IS_Object_t *pObj);
int32_t ISM330IS_ACC_Set_Sleep_Duration(ISM330IS_Object_t *pObj, uint8_t Duration);

int32_t ISM330IS_ACC_Enable_Single_Tap_Detection(ISM330IS_Object_t *pObj, ISM330IS_SensorIntPin_t IntPin);
int32_t ISM330IS_ACC_Disable_Single_Tap_Detection(ISM330IS_Object_t *pObj);
int32_t ISM330IS_ACC_Enable_Double_Tap_Detection(ISM330IS_Object_t *pObj, ISM330IS_SensorIntPin_t IntPin);
int32_t ISM330IS_ACC_Disable_Double_Tap_Detection(ISM330IS_Object_t *pObj);
int32_t ISM330IS_ACC_Set_Tap_Threshold(ISM330IS_Object_t *pObj, uint8_t Threshold);
int32_t ISM330IS_ACC_Set_Tap_Shock_Time(ISM330IS_Object_t *pObj, uint8_t Time);
int32_t ISM330IS_ACC_Set_Tap_Quiet_Time(ISM330IS_Object_t *pObj, uint8_t Time);
int32_t ISM330IS_ACC_Set_Tap_Duration_Time(ISM330IS_Object_t *pObj, uint8_t Time);

int32_t ISM330IS_ACC_Enable_6D_Orientation(ISM330IS_Object_t *pObj, ISM330IS_SensorIntPin_t IntPin);
int32_t ISM330IS_ACC_Disable_6D_Orientation(ISM330IS_Object_t *pObj);
int32_t ISM330IS_ACC_Set_6D_Orientation_Threshold(ISM330IS_Object_t *pObj, uint8_t Threshold);
int32_t ISM330IS_ACC_Get_6D_Orientation_XL(ISM330IS_Object_t *pObj, uint8_t *XLow);
int32_t ISM330IS_ACC_Get_6D_Orientation_XH(ISM330IS_Object_t *pObj, uint8_t *XHigh);
int32_t ISM330IS_ACC_Get_6D_Orientation_YL(ISM330IS_Object_t *pObj, uint8_t *YLow);
int32_t ISM330IS_ACC_Get_6D_Orientation_YH(ISM330IS_Object_t *pObj, uint8_t *YHigh);
int32_t ISM330IS_ACC_Get_6D_Orientation_ZL(ISM330IS_Object_t *pObj, uint8_t *ZLow);
int32_t ISM330IS_ACC_Get_6D_Orientation_ZH(ISM330IS_Object_t *pObj, uint8_t *ZHigh);

int32_t ISM330IS_ACC_Get_Event_Status(ISM330IS_Object_t *pObj, ISM330IS_Event_Status_t *Status);
int32_t ISM330IS_ACC_Set_SelfTest(ISM330IS_Object_t *pObj, uint8_t Val);
int32_t ISM330IS_ACC_Get_DRDY_Status(ISM330IS_Object_t *pObj, uint8_t *Status);
int32_t ISM330IS_ACC_Get_Init_Status(ISM330IS_Object_t *pObj, uint8_t *Status);
int32_t ISM330IS_ACC_Set_INT1_DRDY(ISM330IS_Object_t *pObj, uint8_t Val);

int32_t ISM330IS_GYRO_Set_SelfTest(ISM330IS_Object_t *pObj, uint8_t Val);
int32_t ISM330IS_GYRO_Get_DRDY_Status(ISM330IS_Object_t *pObj, uint8_t *Status);
int32_t ISM330IS_GYRO_Get_Init_Status(ISM330IS_Object_t *pObj, uint8_t *Status);
int32_t ISM330IS_GYRO_Set_INT1_DRDY(ISM330IS_Object_t *pObj, uint8_t Val);

int32_t ISM330IS_Set_DRDY_Mode(ISM330IS_Object_t *pObj, uint8_t Val);

int32_t ISM330IS_FIFO_Get_Num_Samples(ISM330IS_Object_t *pObj, uint16_t *NumSamples);
int32_t ISM330IS_FIFO_Get_Full_Status(ISM330IS_Object_t *pObj, uint8_t *Status);
int32_t ISM330IS_FIFO_Set_ODR_Value(ISM330IS_Object_t *pObj, float Odr);
int32_t ISM330IS_FIFO_Set_INT1_FIFO_Full(ISM330IS_Object_t *pObj, uint8_t Status);
int32_t ISM330IS_FIFO_Set_Watermark_Level(ISM330IS_Object_t *pObj, uint16_t Watermark);
int32_t ISM330IS_FIFO_Set_Stop_On_Fth(ISM330IS_Object_t *pObj, uint8_t Status);
int32_t ISM330IS_FIFO_Set_Mode(ISM330IS_Object_t *pObj, uint8_t Mode);
int32_t ISM330IS_FIFO_Get_Pattern(ISM330IS_Object_t *pObj, uint16_t *Pattern);
int32_t ISM330IS_FIFO_Get_Data(ISM330IS_Object_t *pObj, uint8_t *Data);
int32_t ISM330IS_FIFO_Get_Empty_Status(ISM330IS_Object_t *pObj, uint8_t *Status);
int32_t ISM330IS_FIFO_Get_Overrun_Status(ISM330IS_Object_t *pObj, uint8_t *Status);
int32_t ISM330IS_FIFO_ACC_Set_Decimation(ISM330IS_Object_t *pObj, uint8_t Decimation);
int32_t ISM330IS_FIFO_ACC_Get_Axis(ISM330IS_Object_t *pObj, int32_t *Acceleration);
int32_t ISM330IS_FIFO_GYRO_Set_Decimation(ISM330IS_Object_t *pObj, uint8_t Decimation);
int32_t ISM330IS_FIFO_GYRO_Get_Axis(ISM330IS_Object_t *pObj, int32_t *AngularVelocity);

int32_t ISM330IS_Set_Mem_Bank(ISM330IS_Object_t *pObj, uint8_t Val);

/**
  * @}
  */

/** @addtogroup ISM330IS_Exported_Variables ISM330IS Exported Variables
  * @{
  */

extern ISM330IS_CommonDrv_t ISM330IS_COMMON_Driver;
extern ISM330IS_ACC_Drv_t ISM330IS_ACC_Driver;
extern ISM330IS_GYRO_Drv_t ISM330IS_GYRO_Driver;

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
