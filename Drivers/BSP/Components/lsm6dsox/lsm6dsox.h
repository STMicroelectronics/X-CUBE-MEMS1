/**
 ******************************************************************************
 * @file    lsm6dsox.h
 * @author  MEMS Software Solutions Team
 * @brief   LSM6DSOX header driver file
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#ifndef LSM6DSOX_H
#define LSM6DSOX_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "lsm6dsox_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @addtogroup LSM6DSOX LSM6DSOX
 * @{
 */

/** @defgroup LSM6DSOX_Exported_Types LSM6DSOX Exported Types
 * @{
 */

typedef int32_t (*LSM6DSOX_Init_Func)(void);
typedef int32_t (*LSM6DSOX_DeInit_Func)(void);
typedef int32_t (*LSM6DSOX_GetTick_Func)(void);
typedef int32_t (*LSM6DSOX_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*LSM6DSOX_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  LSM6DSOX_INT1_PIN,
  LSM6DSOX_INT2_PIN,
} LSM6DSOX_SensorIntPin_t;

typedef enum
{
  LSM6DSOX_ACC_HIGH_PERFORMANCE_MODE,
  LSM6DSOX_ACC_LOW_POWER_NORMAL_MODE,
  LSM6DSOX_ACC_ULTRA_LOW_POWER_MODE
} LSM6DSOX_ACC_Operating_Mode_t;

typedef enum
{
  LSM6DSOX_GYRO_HIGH_PERFORMANCE_MODE,
  LSM6DSOX_GYRO_LOW_POWER_NORMAL_MODE
} LSM6DSOX_GYRO_Operating_Mode_t;

typedef struct
{
  LSM6DSOX_Init_Func         Init;
  LSM6DSOX_DeInit_Func       DeInit;
  uint32_t                   BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
  uint8_t                    Address;
  LSM6DSOX_WriteReg_Func     WriteReg;
  LSM6DSOX_ReadReg_Func      ReadReg;
  LSM6DSOX_GetTick_Func      GetTick;
} LSM6DSOX_IO_t;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} LSM6DSOX_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} LSM6DSOX_Axes_t;

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
} LSM6DSOX_Event_Status_t;

typedef struct
{
  LSM6DSOX_IO_t       IO;
  stmdev_ctx_t        Ctx;
  uint8_t             is_initialized;
  uint8_t             acc_is_enabled;
  uint8_t             gyro_is_enabled;
  lsm6dsox_odr_xl_t   acc_odr;
  lsm6dsox_odr_g_t    gyro_odr;
} LSM6DSOX_Object_t;

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
} LSM6DSOX_Capabilities_t;

typedef struct
{
  int32_t (*Init)(LSM6DSOX_Object_t *);
  int32_t (*DeInit)(LSM6DSOX_Object_t *);
  int32_t (*ReadID)(LSM6DSOX_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(LSM6DSOX_Object_t *, LSM6DSOX_Capabilities_t *);
} LSM6DSOX_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(LSM6DSOX_Object_t *);
  int32_t (*Disable)(LSM6DSOX_Object_t *);
  int32_t (*GetSensitivity)(LSM6DSOX_Object_t *, float *);
  int32_t (*GetOutputDataRate)(LSM6DSOX_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LSM6DSOX_Object_t *, float);
  int32_t (*GetFullScale)(LSM6DSOX_Object_t *, int32_t *);
  int32_t (*SetFullScale)(LSM6DSOX_Object_t *, int32_t);
  int32_t (*GetAxes)(LSM6DSOX_Object_t *, LSM6DSOX_Axes_t *);
  int32_t (*GetAxesRaw)(LSM6DSOX_Object_t *, LSM6DSOX_AxesRaw_t *);
} LSM6DSOX_ACC_Drv_t;

typedef struct
{
  int32_t (*Enable)(LSM6DSOX_Object_t *);
  int32_t (*Disable)(LSM6DSOX_Object_t *);
  int32_t (*GetSensitivity)(LSM6DSOX_Object_t *, float *);
  int32_t (*GetOutputDataRate)(LSM6DSOX_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LSM6DSOX_Object_t *, float);
  int32_t (*GetFullScale)(LSM6DSOX_Object_t *, int32_t *);
  int32_t (*SetFullScale)(LSM6DSOX_Object_t *, int32_t);
  int32_t (*GetAxes)(LSM6DSOX_Object_t *, LSM6DSOX_Axes_t *);
  int32_t (*GetAxesRaw)(LSM6DSOX_Object_t *, LSM6DSOX_AxesRaw_t *);
} LSM6DSOX_GYRO_Drv_t;

typedef union{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} lsm6dsox_axis3bit16_t;

typedef union{
  int16_t i16bit;
  uint8_t u8bit[2];
} lsm6dsox_axis1bit16_t;

typedef union{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} lsm6dsox_axis3bit32_t;

typedef union{
  int32_t i32bit;
  uint8_t u8bit[4];
} lsm6dsox_axis1bit32_t;

/**
 * @}
 */

/** @defgroup LSM6DSOX_Exported_Constants LSM6DSOX Exported Constants
 * @{
 */

#define LSM6DSOX_OK                       0
#define LSM6DSOX_ERROR                   -1

#define LSM6DSOX_I2C_BUS                 0U
#define LSM6DSOX_SPI_4WIRES_BUS          1U
#define LSM6DSOX_SPI_3WIRES_BUS          2U

#define LSM6DSOX_ACC_SENSITIVITY_FS_2G   0.061f
#define LSM6DSOX_ACC_SENSITIVITY_FS_4G   0.122f
#define LSM6DSOX_ACC_SENSITIVITY_FS_8G   0.244f
#define LSM6DSOX_ACC_SENSITIVITY_FS_16G  0.488f

#define LSM6DSOX_GYRO_SENSITIVITY_FS_125DPS    4.375f
#define LSM6DSOX_GYRO_SENSITIVITY_FS_250DPS    8.750f
#define LSM6DSOX_GYRO_SENSITIVITY_FS_500DPS   17.500f
#define LSM6DSOX_GYRO_SENSITIVITY_FS_1000DPS  35.000f
#define LSM6DSOX_GYRO_SENSITIVITY_FS_2000DPS  70.000f

/**
 * @}
 */

/** @addtogroup LSM6DSOX_Exported_Functions LSM6DSOX Exported Functions
 * @{
 */

int32_t LSM6DSOX_RegisterBusIO(LSM6DSOX_Object_t *pObj, LSM6DSOX_IO_t *pIO);
int32_t LSM6DSOX_Init(LSM6DSOX_Object_t *pObj);
int32_t LSM6DSOX_DeInit(LSM6DSOX_Object_t *pObj);
int32_t LSM6DSOX_ReadID(LSM6DSOX_Object_t *pObj, uint8_t *Id);
int32_t LSM6DSOX_GetCapabilities(LSM6DSOX_Object_t *pObj, LSM6DSOX_Capabilities_t *Capabilities);

int32_t LSM6DSOX_ACC_Enable(LSM6DSOX_Object_t *pObj);
int32_t LSM6DSOX_ACC_Disable(LSM6DSOX_Object_t *pObj);
int32_t LSM6DSOX_ACC_GetSensitivity(LSM6DSOX_Object_t *pObj, float *Sensitivity);
int32_t LSM6DSOX_ACC_GetOutputDataRate(LSM6DSOX_Object_t *pObj, float *Odr);
int32_t LSM6DSOX_ACC_SetOutputDataRate(LSM6DSOX_Object_t *pObj, float Odr);
int32_t LSM6DSOX_ACC_SetOutputDataRate_With_Mode(LSM6DSOX_Object_t *pObj, float Odr, LSM6DSOX_ACC_Operating_Mode_t Mode);
int32_t LSM6DSOX_ACC_GetFullScale(LSM6DSOX_Object_t *pObj, int32_t *FullScale);
int32_t LSM6DSOX_ACC_SetFullScale(LSM6DSOX_Object_t *pObj, int32_t FullScale);
int32_t LSM6DSOX_ACC_GetAxesRaw(LSM6DSOX_Object_t *pObj, LSM6DSOX_AxesRaw_t *Value);
int32_t LSM6DSOX_ACC_GetAxes(LSM6DSOX_Object_t *pObj, LSM6DSOX_Axes_t *Acceleration);

int32_t LSM6DSOX_GYRO_Enable(LSM6DSOX_Object_t *pObj);
int32_t LSM6DSOX_GYRO_Disable(LSM6DSOX_Object_t *pObj);
int32_t LSM6DSOX_GYRO_GetSensitivity(LSM6DSOX_Object_t *pObj, float *Sensitivity);
int32_t LSM6DSOX_GYRO_GetOutputDataRate(LSM6DSOX_Object_t *pObj, float *Odr);
int32_t LSM6DSOX_GYRO_SetOutputDataRate(LSM6DSOX_Object_t *pObj, float Odr);
int32_t LSM6DSOX_GYRO_SetOutputDataRate_With_Mode(LSM6DSOX_Object_t *pObj, float Odr, LSM6DSOX_GYRO_Operating_Mode_t Mode);
int32_t LSM6DSOX_GYRO_GetFullScale(LSM6DSOX_Object_t *pObj, int32_t *FullScale);
int32_t LSM6DSOX_GYRO_SetFullScale(LSM6DSOX_Object_t *pObj, int32_t FullScale);
int32_t LSM6DSOX_GYRO_GetAxesRaw(LSM6DSOX_Object_t *pObj, LSM6DSOX_AxesRaw_t *Value);
int32_t LSM6DSOX_GYRO_GetAxes(LSM6DSOX_Object_t *pObj, LSM6DSOX_Axes_t *AngularRate);

int32_t LSM6DSOX_Read_Reg(LSM6DSOX_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t LSM6DSOX_Write_Reg(LSM6DSOX_Object_t *pObj, uint8_t reg, uint8_t Data);
int32_t LSM6DSOX_Set_Interrupt_Latch(LSM6DSOX_Object_t *pObj, uint8_t Status);

int32_t LSM6DSOX_ACC_Enable_Free_Fall_Detection(LSM6DSOX_Object_t *pObj, LSM6DSOX_SensorIntPin_t IntPin);
int32_t LSM6DSOX_ACC_Disable_Free_Fall_Detection(LSM6DSOX_Object_t *pObj);
int32_t LSM6DSOX_ACC_Set_Free_Fall_Threshold(LSM6DSOX_Object_t *pObj, uint8_t Threshold);
int32_t LSM6DSOX_ACC_Set_Free_Fall_Duration(LSM6DSOX_Object_t *pObj, uint8_t Duration);

int32_t LSM6DSOX_ACC_Enable_Pedometer(LSM6DSOX_Object_t *pObj);
int32_t LSM6DSOX_ACC_Disable_Pedometer(LSM6DSOX_Object_t *pObj);
int32_t LSM6DSOX_ACC_Get_Step_Count(LSM6DSOX_Object_t *pObj, uint16_t *StepCount);
int32_t LSM6DSOX_ACC_Step_Counter_Reset(LSM6DSOX_Object_t *pObj);

int32_t LSM6DSOX_ACC_Enable_Tilt_Detection(LSM6DSOX_Object_t *pObj, LSM6DSOX_SensorIntPin_t IntPin);
int32_t LSM6DSOX_ACC_Disable_Tilt_Detection(LSM6DSOX_Object_t *pObj);

int32_t LSM6DSOX_ACC_Enable_Wake_Up_Detection(LSM6DSOX_Object_t *pObj, LSM6DSOX_SensorIntPin_t IntPin);
int32_t LSM6DSOX_ACC_Disable_Wake_Up_Detection(LSM6DSOX_Object_t *pObj);
int32_t LSM6DSOX_ACC_Set_Wake_Up_Threshold(LSM6DSOX_Object_t *pObj, uint8_t Threshold);
int32_t LSM6DSOX_ACC_Set_Wake_Up_Duration(LSM6DSOX_Object_t *pObj, uint8_t Duration);

int32_t LSM6DSOX_ACC_Enable_Single_Tap_Detection(LSM6DSOX_Object_t *pObj, LSM6DSOX_SensorIntPin_t IntPin);
int32_t LSM6DSOX_ACC_Disable_Single_Tap_Detection(LSM6DSOX_Object_t *pObj);
int32_t LSM6DSOX_ACC_Enable_Double_Tap_Detection(LSM6DSOX_Object_t *pObj, LSM6DSOX_SensorIntPin_t IntPin);
int32_t LSM6DSOX_ACC_Disable_Double_Tap_Detection(LSM6DSOX_Object_t *pObj);
int32_t LSM6DSOX_ACC_Set_Tap_Threshold(LSM6DSOX_Object_t *pObj, uint8_t Threshold);
int32_t LSM6DSOX_ACC_Set_Tap_Shock_Time(LSM6DSOX_Object_t *pObj, uint8_t Time);
int32_t LSM6DSOX_ACC_Set_Tap_Quiet_Time(LSM6DSOX_Object_t *pObj, uint8_t Time);
int32_t LSM6DSOX_ACC_Set_Tap_Duration_Time(LSM6DSOX_Object_t *pObj, uint8_t Time);

int32_t LSM6DSOX_ACC_Enable_6D_Orientation(LSM6DSOX_Object_t *pObj, LSM6DSOX_SensorIntPin_t IntPin);
int32_t LSM6DSOX_ACC_Disable_6D_Orientation(LSM6DSOX_Object_t *pObj);
int32_t LSM6DSOX_ACC_Set_6D_Orientation_Threshold(LSM6DSOX_Object_t *pObj, uint8_t Threshold);
int32_t LSM6DSOX_ACC_Get_6D_Orientation_XL(LSM6DSOX_Object_t *pObj, uint8_t *XLow);
int32_t LSM6DSOX_ACC_Get_6D_Orientation_XH(LSM6DSOX_Object_t *pObj, uint8_t *XHigh);
int32_t LSM6DSOX_ACC_Get_6D_Orientation_YL(LSM6DSOX_Object_t *pObj, uint8_t *YLow);
int32_t LSM6DSOX_ACC_Get_6D_Orientation_YH(LSM6DSOX_Object_t *pObj, uint8_t *YHigh);
int32_t LSM6DSOX_ACC_Get_6D_Orientation_ZL(LSM6DSOX_Object_t *pObj, uint8_t *ZLow);
int32_t LSM6DSOX_ACC_Get_6D_Orientation_ZH(LSM6DSOX_Object_t *pObj, uint8_t *ZHigh);

int32_t LSM6DSOX_ACC_Get_DRDY_Status(LSM6DSOX_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSOX_ACC_Get_Event_Status(LSM6DSOX_Object_t *pObj, LSM6DSOX_Event_Status_t *Status);
int32_t LSM6DSOX_ACC_Set_SelfTest(LSM6DSOX_Object_t *pObj, uint8_t Status);

int32_t LSM6DSOX_GYRO_Get_DRDY_Status(LSM6DSOX_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSOX_GYRO_Set_SelfTest(LSM6DSOX_Object_t *pObj, uint8_t Status);

int32_t LSM6DSOX_FIFO_Get_Num_Samples(LSM6DSOX_Object_t *pObj, uint16_t *NumSamples);
int32_t LSM6DSOX_FIFO_Get_Full_Status(LSM6DSOX_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSOX_FIFO_Set_INT1_FIFO_Full(LSM6DSOX_Object_t *pObj, uint8_t Status);
int32_t LSM6DSOX_FIFO_Set_Watermark_Level(LSM6DSOX_Object_t *pObj, uint16_t Watermark);
int32_t LSM6DSOX_FIFO_Set_Stop_On_Fth(LSM6DSOX_Object_t *pObj, uint8_t Status);
int32_t LSM6DSOX_FIFO_Set_Mode(LSM6DSOX_Object_t *pObj, uint8_t Mode);
int32_t LSM6DSOX_FIFO_Get_Tag(LSM6DSOX_Object_t *pObj, uint8_t *Tag);
int32_t LSM6DSOX_FIFO_Get_Data(LSM6DSOX_Object_t *pObj, uint8_t *Data);
int32_t LSM6DSOX_FIFO_Get_Empty_Status(LSM6DSOX_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSOX_FIFO_Get_Overrun_Status(LSM6DSOX_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSOX_FIFO_ACC_Get_Axes(LSM6DSOX_Object_t *pObj, LSM6DSOX_Axes_t *Acceleration);
int32_t LSM6DSOX_FIFO_ACC_Set_BDR(LSM6DSOX_Object_t *pObj, float Bdr);
int32_t LSM6DSOX_FIFO_GYRO_Get_Axes(LSM6DSOX_Object_t *pObj, LSM6DSOX_Axes_t *AngularVelocity);
int32_t LSM6DSOX_FIFO_GYRO_Set_BDR(LSM6DSOX_Object_t *pObj, float Bdr);

int32_t LSM6DSOX_GYRO_Enable_DRDY_Interrupt(LSM6DSOX_Object_t *pObj);
int32_t LSM6DSOX_ACC_Enable_DRDY_Interrupt(LSM6DSOX_Object_t *pObj);
int32_t LSM6DSOX_ACC_Set_Power_Mode(LSM6DSOX_Object_t *pObj, uint8_t PowerMode);
int32_t LSM6DSOX_GYRO_Set_Power_Mode(LSM6DSOX_Object_t *pObj, uint8_t PowerMode);

int32_t LSM6DSOX_ACC_Set_Filter_Mode(LSM6DSOX_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode);
int32_t LSM6DSOX_GYRO_Set_Filter_Mode(LSM6DSOX_Object_t *pObj, uint8_t LowHighPassFlag, uint8_t FilterMode);
int32_t LSM6DSOX_ACC_Enable_Inactivity_Detection(LSM6DSOX_Object_t *pObj, lsm6dsox_inact_en_t InactMode, LSM6DSOX_SensorIntPin_t IntPin);
int32_t LSM6DSOX_ACC_Disable_Inactivity_Detection(LSM6DSOX_Object_t *pObj);
int32_t LSM6DSOX_ACC_Set_Sleep_Duration(LSM6DSOX_Object_t *pObj, uint8_t Duration);

/**
 * @}
 */

/** @addtogroup LSM6DSOX_Exported_Variables LSM6DSOX Exported Variables
 * @{
 */

extern LSM6DSOX_CommonDrv_t LSM6DSOX_COMMON_Driver;
extern LSM6DSOX_ACC_Drv_t LSM6DSOX_ACC_Driver;
extern LSM6DSOX_GYRO_Drv_t LSM6DSOX_GYRO_Driver;

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
