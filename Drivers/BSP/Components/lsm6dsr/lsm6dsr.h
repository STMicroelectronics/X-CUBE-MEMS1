/**
 ******************************************************************************
 * @file    lsm6dsr.h
 * @author  MEMS Software Solutions Team
 * @brief   LSM6DSR header driver file
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
#ifndef LSM6DSR_H
#define LSM6DSR_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "lsm6dsr_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @addtogroup LSM6DSR LSM6DSR
 * @{
 */

/** @defgroup LSM6DSR_Exported_Types LSM6DSR Exported Types
 * @{
 */

typedef int32_t (*LSM6DSR_Init_Func)(void);
typedef int32_t (*LSM6DSR_DeInit_Func)(void);
typedef int32_t (*LSM6DSR_GetTick_Func)(void);
typedef int32_t (*LSM6DSR_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*LSM6DSR_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  LSM6DSR_INT1_PIN,
  LSM6DSR_INT2_PIN,
} LSM6DSR_SensorIntPin_t;

typedef struct
{
  LSM6DSR_Init_Func          Init;
  LSM6DSR_DeInit_Func        DeInit;
  uint32_t                   BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
  uint8_t                    Address;
  LSM6DSR_WriteReg_Func      WriteReg;
  LSM6DSR_ReadReg_Func       ReadReg;
  LSM6DSR_GetTick_Func       GetTick;
} LSM6DSR_IO_t;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} LSM6DSR_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} LSM6DSR_Axes_t;

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
} LSM6DSR_Event_Status_t;

typedef struct
{
  LSM6DSR_IO_t        IO;
  stmdev_ctx_t        Ctx;
  uint8_t             is_initialized;
  uint8_t             acc_is_enabled;
  uint8_t             gyro_is_enabled;
  lsm6dsr_odr_xl_t    acc_odr;
  lsm6dsr_odr_g_t     gyro_odr;
} LSM6DSR_Object_t;

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
} LSM6DSR_Capabilities_t;

typedef struct
{
  int32_t (*Init)(LSM6DSR_Object_t *);
  int32_t (*DeInit)(LSM6DSR_Object_t *);
  int32_t (*ReadID)(LSM6DSR_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(LSM6DSR_Object_t *, LSM6DSR_Capabilities_t *);
} LSM6DSR_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(LSM6DSR_Object_t *);
  int32_t (*Disable)(LSM6DSR_Object_t *);
  int32_t (*GetSensitivity)(LSM6DSR_Object_t *, float *);
  int32_t (*GetOutputDataRate)(LSM6DSR_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LSM6DSR_Object_t *, float);
  int32_t (*GetFullScale)(LSM6DSR_Object_t *, int32_t *);
  int32_t (*SetFullScale)(LSM6DSR_Object_t *, int32_t);
  int32_t (*GetAxes)(LSM6DSR_Object_t *, LSM6DSR_Axes_t *);
  int32_t (*GetAxesRaw)(LSM6DSR_Object_t *, LSM6DSR_AxesRaw_t *);
} LSM6DSR_ACC_Drv_t;

typedef struct
{
  int32_t (*Enable)(LSM6DSR_Object_t *);
  int32_t (*Disable)(LSM6DSR_Object_t *);
  int32_t (*GetSensitivity)(LSM6DSR_Object_t *, float *);
  int32_t (*GetOutputDataRate)(LSM6DSR_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LSM6DSR_Object_t *, float);
  int32_t (*GetFullScale)(LSM6DSR_Object_t *, int32_t *);
  int32_t (*SetFullScale)(LSM6DSR_Object_t *, int32_t);
  int32_t (*GetAxes)(LSM6DSR_Object_t *, LSM6DSR_Axes_t *);
  int32_t (*GetAxesRaw)(LSM6DSR_Object_t *, LSM6DSR_AxesRaw_t *);
} LSM6DSR_GYRO_Drv_t;

typedef union{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} lsm6dsr_axis3bit16_t;

typedef union{
  int16_t i16bit;
  uint8_t u8bit[2];
} lsm6dsr_axis1bit16_t;

typedef union{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} lsm6dsr_axis3bit32_t;

typedef union{
  int32_t i32bit;
  uint8_t u8bit[4];
} lsm6dsr_axis1bit32_t;

/**
 * @}
 */

/** @defgroup LSM6DSR_Exported_Constants LSM6DSR Exported Constants
 * @{
 */

#define LSM6DSR_OK                       0
#define LSM6DSR_ERROR                   -1

#define LSM6DSR_I2C_BUS                 0U
#define LSM6DSR_SPI_4WIRES_BUS          1U
#define LSM6DSR_SPI_3WIRES_BUS          2U

#define LSM6DSR_ACC_SENSITIVITY_FS_2G   0.061f
#define LSM6DSR_ACC_SENSITIVITY_FS_4G   0.122f
#define LSM6DSR_ACC_SENSITIVITY_FS_8G   0.244f
#define LSM6DSR_ACC_SENSITIVITY_FS_16G  0.488f

#define LSM6DSR_GYRO_SENSITIVITY_FS_125DPS     4.375f
#define LSM6DSR_GYRO_SENSITIVITY_FS_250DPS     8.750f
#define LSM6DSR_GYRO_SENSITIVITY_FS_500DPS    17.500f
#define LSM6DSR_GYRO_SENSITIVITY_FS_1000DPS   35.000f
#define LSM6DSR_GYRO_SENSITIVITY_FS_2000DPS   70.000f
#define LSM6DSR_GYRO_SENSITIVITY_FS_4000DPS  140.000f

/**
 * @}
 */

/** @addtogroup LSM6DSR_Exported_Functions LSM6DSR Exported Functions
 * @{
 */

int32_t LSM6DSR_RegisterBusIO(LSM6DSR_Object_t *pObj, LSM6DSR_IO_t *pIO);
int32_t LSM6DSR_Init(LSM6DSR_Object_t *pObj);
int32_t LSM6DSR_DeInit(LSM6DSR_Object_t *pObj);
int32_t LSM6DSR_ReadID(LSM6DSR_Object_t *pObj, uint8_t *Id);
int32_t LSM6DSR_GetCapabilities(LSM6DSR_Object_t *pObj, LSM6DSR_Capabilities_t *Capabilities);

int32_t LSM6DSR_ACC_Enable(LSM6DSR_Object_t *pObj);
int32_t LSM6DSR_ACC_Disable(LSM6DSR_Object_t *pObj);
int32_t LSM6DSR_ACC_GetSensitivity(LSM6DSR_Object_t *pObj, float *Sensitivity);
int32_t LSM6DSR_ACC_GetOutputDataRate(LSM6DSR_Object_t *pObj, float *Odr);
int32_t LSM6DSR_ACC_SetOutputDataRate(LSM6DSR_Object_t *pObj, float Odr);
int32_t LSM6DSR_ACC_GetFullScale(LSM6DSR_Object_t *pObj, int32_t *FullScale);
int32_t LSM6DSR_ACC_SetFullScale(LSM6DSR_Object_t *pObj, int32_t FullScale);
int32_t LSM6DSR_ACC_GetAxesRaw(LSM6DSR_Object_t *pObj, LSM6DSR_AxesRaw_t *Value);
int32_t LSM6DSR_ACC_GetAxes(LSM6DSR_Object_t *pObj, LSM6DSR_Axes_t *Acceleration);

int32_t LSM6DSR_GYRO_Enable(LSM6DSR_Object_t *pObj);
int32_t LSM6DSR_GYRO_Disable(LSM6DSR_Object_t *pObj);
int32_t LSM6DSR_GYRO_GetSensitivity(LSM6DSR_Object_t *pObj, float *Sensitivity);
int32_t LSM6DSR_GYRO_GetOutputDataRate(LSM6DSR_Object_t *pObj, float *Odr);
int32_t LSM6DSR_GYRO_SetOutputDataRate(LSM6DSR_Object_t *pObj, float Odr);
int32_t LSM6DSR_GYRO_GetFullScale(LSM6DSR_Object_t *pObj, int32_t *FullScale);
int32_t LSM6DSR_GYRO_SetFullScale(LSM6DSR_Object_t *pObj, int32_t FullScale);
int32_t LSM6DSR_GYRO_GetAxesRaw(LSM6DSR_Object_t *pObj, LSM6DSR_AxesRaw_t *Value);
int32_t LSM6DSR_GYRO_GetAxes(LSM6DSR_Object_t *pObj, LSM6DSR_Axes_t *AngularRate);

int32_t LSM6DSR_Read_Reg(LSM6DSR_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t LSM6DSR_Write_Reg(LSM6DSR_Object_t *pObj, uint8_t reg, uint8_t Data);
int32_t LSM6DSR_Set_Interrupt_Latch(LSM6DSR_Object_t *pObj, uint8_t Status);

int32_t LSM6DSR_ACC_Enable_Free_Fall_Detection(LSM6DSR_Object_t *pObj, LSM6DSR_SensorIntPin_t IntPin);
int32_t LSM6DSR_ACC_Disable_Free_Fall_Detection(LSM6DSR_Object_t *pObj);
int32_t LSM6DSR_ACC_Set_Free_Fall_Threshold(LSM6DSR_Object_t *pObj, uint8_t Threshold);
int32_t LSM6DSR_ACC_Set_Free_Fall_Duration(LSM6DSR_Object_t *pObj, uint8_t Duration);

int32_t LSM6DSR_ACC_Enable_Pedometer(LSM6DSR_Object_t *pObj);
int32_t LSM6DSR_ACC_Disable_Pedometer(LSM6DSR_Object_t *pObj);
int32_t LSM6DSR_ACC_Get_Step_Count(LSM6DSR_Object_t *pObj, uint16_t *StepCount);
int32_t LSM6DSR_ACC_Step_Counter_Reset(LSM6DSR_Object_t *pObj);

int32_t LSM6DSR_ACC_Enable_Tilt_Detection(LSM6DSR_Object_t *pObj, LSM6DSR_SensorIntPin_t IntPin);
int32_t LSM6DSR_ACC_Disable_Tilt_Detection(LSM6DSR_Object_t *pObj);

int32_t LSM6DSR_ACC_Enable_Wake_Up_Detection(LSM6DSR_Object_t *pObj, LSM6DSR_SensorIntPin_t IntPin);
int32_t LSM6DSR_ACC_Disable_Wake_Up_Detection(LSM6DSR_Object_t *pObj);
int32_t LSM6DSR_ACC_Set_Wake_Up_Threshold(LSM6DSR_Object_t *pObj, uint8_t Threshold);
int32_t LSM6DSR_ACC_Set_Wake_Up_Duration(LSM6DSR_Object_t *pObj, uint8_t Duration);

int32_t LSM6DSR_ACC_Enable_Single_Tap_Detection(LSM6DSR_Object_t *pObj, LSM6DSR_SensorIntPin_t IntPin);
int32_t LSM6DSR_ACC_Disable_Single_Tap_Detection(LSM6DSR_Object_t *pObj);
int32_t LSM6DSR_ACC_Enable_Double_Tap_Detection(LSM6DSR_Object_t *pObj, LSM6DSR_SensorIntPin_t IntPin);
int32_t LSM6DSR_ACC_Disable_Double_Tap_Detection(LSM6DSR_Object_t *pObj);
int32_t LSM6DSR_ACC_Set_Tap_Threshold(LSM6DSR_Object_t *pObj, uint8_t Threshold);
int32_t LSM6DSR_ACC_Set_Tap_Shock_Time(LSM6DSR_Object_t *pObj, uint8_t Time);
int32_t LSM6DSR_ACC_Set_Tap_Quiet_Time(LSM6DSR_Object_t *pObj, uint8_t Time);
int32_t LSM6DSR_ACC_Set_Tap_Duration_Time(LSM6DSR_Object_t *pObj, uint8_t Time);

int32_t LSM6DSR_ACC_Enable_6D_Orientation(LSM6DSR_Object_t *pObj, LSM6DSR_SensorIntPin_t IntPin);
int32_t LSM6DSR_ACC_Disable_6D_Orientation(LSM6DSR_Object_t *pObj);
int32_t LSM6DSR_ACC_Set_6D_Orientation_Threshold(LSM6DSR_Object_t *pObj, uint8_t Threshold);
int32_t LSM6DSR_ACC_Get_6D_Orientation_XL(LSM6DSR_Object_t *pObj, uint8_t *XLow);
int32_t LSM6DSR_ACC_Get_6D_Orientation_XH(LSM6DSR_Object_t *pObj, uint8_t *XHigh);
int32_t LSM6DSR_ACC_Get_6D_Orientation_YL(LSM6DSR_Object_t *pObj, uint8_t *YLow);
int32_t LSM6DSR_ACC_Get_6D_Orientation_YH(LSM6DSR_Object_t *pObj, uint8_t *YHigh);
int32_t LSM6DSR_ACC_Get_6D_Orientation_ZL(LSM6DSR_Object_t *pObj, uint8_t *ZLow);
int32_t LSM6DSR_ACC_Get_6D_Orientation_ZH(LSM6DSR_Object_t *pObj, uint8_t *ZHigh);

int32_t LSM6DSR_ACC_Get_DRDY_Status(LSM6DSR_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSR_ACC_Get_Event_Status(LSM6DSR_Object_t *pObj, LSM6DSR_Event_Status_t *Status);
int32_t LSM6DSR_ACC_Set_SelfTest(LSM6DSR_Object_t *pObj, uint8_t Status);

int32_t LSM6DSR_GYRO_Get_DRDY_Status(LSM6DSR_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSR_GYRO_Set_SelfTest(LSM6DSR_Object_t *pObj, uint8_t Status);

int32_t LSM6DSR_FIFO_Get_Num_Samples(LSM6DSR_Object_t *pObj, uint16_t *NumSamples);
int32_t LSM6DSR_FIFO_Get_Full_Status(LSM6DSR_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSR_FIFO_Set_INT1_FIFO_Full(LSM6DSR_Object_t *pObj, uint8_t Status);
int32_t LSM6DSR_FIFO_Set_Watermark_Level(LSM6DSR_Object_t *pObj, uint16_t Watermark);
int32_t LSM6DSR_FIFO_Set_Stop_On_Fth(LSM6DSR_Object_t *pObj, uint8_t Status);
int32_t LSM6DSR_FIFO_Set_Mode(LSM6DSR_Object_t *pObj, uint8_t Mode);
int32_t LSM6DSR_FIFO_Get_Tag(LSM6DSR_Object_t *pObj, uint8_t *Tag);
int32_t LSM6DSR_FIFO_Get_Data(LSM6DSR_Object_t *pObj, uint8_t *Data);
int32_t LSM6DSR_FIFO_Get_Empty_Status(LSM6DSR_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSR_FIFO_Get_Overrun_Status(LSM6DSR_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSR_FIFO_ACC_Get_Axes(LSM6DSR_Object_t *pObj, LSM6DSR_Axes_t *Acceleration);
int32_t LSM6DSR_FIFO_ACC_Set_BDR(LSM6DSR_Object_t *pObj, float Bdr);
int32_t LSM6DSR_FIFO_GYRO_Get_Axes(LSM6DSR_Object_t *pObj, LSM6DSR_Axes_t *AngularVelocity);
int32_t LSM6DSR_FIFO_GYRO_Set_BDR(LSM6DSR_Object_t *pObj, float Bdr);

/**
 * @}
 */

/** @addtogroup LSM6DSR_Exported_Variables LSM6DSR Exported Variables
 * @{
 */

extern LSM6DSR_CommonDrv_t LSM6DSR_COMMON_Driver;
extern LSM6DSR_ACC_Drv_t LSM6DSR_ACC_Driver;
extern LSM6DSR_GYRO_Drv_t LSM6DSR_GYRO_Driver;

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
