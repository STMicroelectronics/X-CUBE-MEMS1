/**
  ******************************************************************************
  * @file    lsm6dsrx.h
  * @author  MEMS Software Solutions Team
  * @brief   LSM6DSRX header driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LSM6DSRX_H
#define LSM6DSRX_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "lsm6dsrx_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup LSM6DSRX LSM6DSRX
  * @{
  */

/** @defgroup LSM6DSRX_Exported_Types LSM6DSRX Exported Types
  * @{
  */

typedef int32_t (*LSM6DSRX_Init_Func)(void);
typedef int32_t (*LSM6DSRX_DeInit_Func)(void);
typedef int32_t (*LSM6DSRX_GetTick_Func)(void);
typedef void    (*LSM6DSRX_Delay_Func)(uint32_t);
typedef int32_t (*LSM6DSRX_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*LSM6DSRX_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  LSM6DSRX_INT1_PIN,
  LSM6DSRX_INT2_PIN,
} LSM6DSRX_SensorIntPin_t;

typedef struct
{
  LSM6DSRX_Init_Func         Init;
  LSM6DSRX_DeInit_Func       DeInit;
  uint32_t                   BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
  uint8_t                    Address;
  LSM6DSRX_WriteReg_Func     WriteReg;
  LSM6DSRX_ReadReg_Func      ReadReg;
  LSM6DSRX_GetTick_Func      GetTick;
  LSM6DSRX_Delay_Func        Delay;
} LSM6DSRX_IO_t;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} LSM6DSRX_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} LSM6DSRX_Axes_t;

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
} LSM6DSRX_Event_Status_t;

typedef struct
{
  LSM6DSRX_IO_t       IO;
  stmdev_ctx_t        Ctx;
  uint8_t             is_initialized;
  uint8_t             acc_is_enabled;
  uint8_t             gyro_is_enabled;
  lsm6dsrx_odr_xl_t   acc_odr;
  lsm6dsrx_odr_g_t    gyro_odr;
} LSM6DSRX_Object_t;

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
} LSM6DSRX_Capabilities_t;

typedef struct
{
  int32_t (*Init)(LSM6DSRX_Object_t *);
  int32_t (*DeInit)(LSM6DSRX_Object_t *);
  int32_t (*ReadID)(LSM6DSRX_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(LSM6DSRX_Object_t *, LSM6DSRX_Capabilities_t *);
} LSM6DSRX_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(LSM6DSRX_Object_t *);
  int32_t (*Disable)(LSM6DSRX_Object_t *);
  int32_t (*GetSensitivity)(LSM6DSRX_Object_t *, float *);
  int32_t (*GetOutputDataRate)(LSM6DSRX_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LSM6DSRX_Object_t *, float);
  int32_t (*GetFullScale)(LSM6DSRX_Object_t *, int32_t *);
  int32_t (*SetFullScale)(LSM6DSRX_Object_t *, int32_t);
  int32_t (*GetAxes)(LSM6DSRX_Object_t *, LSM6DSRX_Axes_t *);
  int32_t (*GetAxesRaw)(LSM6DSRX_Object_t *, LSM6DSRX_AxesRaw_t *);
} LSM6DSRX_ACC_Drv_t;

typedef struct
{
  int32_t (*Enable)(LSM6DSRX_Object_t *);
  int32_t (*Disable)(LSM6DSRX_Object_t *);
  int32_t (*GetSensitivity)(LSM6DSRX_Object_t *, float *);
  int32_t (*GetOutputDataRate)(LSM6DSRX_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LSM6DSRX_Object_t *, float);
  int32_t (*GetFullScale)(LSM6DSRX_Object_t *, int32_t *);
  int32_t (*SetFullScale)(LSM6DSRX_Object_t *, int32_t);
  int32_t (*GetAxes)(LSM6DSRX_Object_t *, LSM6DSRX_Axes_t *);
  int32_t (*GetAxesRaw)(LSM6DSRX_Object_t *, LSM6DSRX_AxesRaw_t *);
} LSM6DSRX_GYRO_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} lsm6dsrx_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} lsm6dsrx_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} lsm6dsrx_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} lsm6dsrx_axis1bit32_t;

/**
  * @}
  */

/** @defgroup LSM6DSRX_Exported_Constants LSM6DSRX Exported Constants
  * @{
  */

#define LSM6DSRX_OK                       0
#define LSM6DSRX_ERROR                   -1

#define LSM6DSRX_I2C_BUS                 0U
#define LSM6DSRX_SPI_4WIRES_BUS          1U
#define LSM6DSRX_SPI_3WIRES_BUS          2U

#define LSM6DSRX_ACC_SENSITIVITY_FS_2G   0.061f
#define LSM6DSRX_ACC_SENSITIVITY_FS_4G   0.122f
#define LSM6DSRX_ACC_SENSITIVITY_FS_8G   0.244f
#define LSM6DSRX_ACC_SENSITIVITY_FS_16G  0.488f

#define LSM6DSRX_GYRO_SENSITIVITY_FS_125DPS     4.375f
#define LSM6DSRX_GYRO_SENSITIVITY_FS_250DPS     8.750f
#define LSM6DSRX_GYRO_SENSITIVITY_FS_500DPS    17.500f
#define LSM6DSRX_GYRO_SENSITIVITY_FS_1000DPS   35.000f
#define LSM6DSRX_GYRO_SENSITIVITY_FS_2000DPS   70.000f
#define LSM6DSRX_GYRO_SENSITIVITY_FS_4000DPS  140.000f

/**
  * @}
  */

/** @addtogroup LSM6DSRX_Exported_Functions LSM6DSRX Exported Functions
  * @{
  */

int32_t LSM6DSRX_RegisterBusIO(LSM6DSRX_Object_t *pObj, LSM6DSRX_IO_t *pIO);
int32_t LSM6DSRX_Init(LSM6DSRX_Object_t *pObj);
int32_t LSM6DSRX_DeInit(LSM6DSRX_Object_t *pObj);
int32_t LSM6DSRX_ReadID(LSM6DSRX_Object_t *pObj, uint8_t *Id);
int32_t LSM6DSRX_GetCapabilities(LSM6DSRX_Object_t *pObj, LSM6DSRX_Capabilities_t *Capabilities);

int32_t LSM6DSRX_ACC_Enable(LSM6DSRX_Object_t *pObj);
int32_t LSM6DSRX_ACC_Disable(LSM6DSRX_Object_t *pObj);
int32_t LSM6DSRX_ACC_GetSensitivity(LSM6DSRX_Object_t *pObj, float *Sensitivity);
int32_t LSM6DSRX_ACC_GetOutputDataRate(LSM6DSRX_Object_t *pObj, float *Odr);
int32_t LSM6DSRX_ACC_SetOutputDataRate(LSM6DSRX_Object_t *pObj, float Odr);
int32_t LSM6DSRX_ACC_GetFullScale(LSM6DSRX_Object_t *pObj, int32_t *FullScale);
int32_t LSM6DSRX_ACC_SetFullScale(LSM6DSRX_Object_t *pObj, int32_t FullScale);
int32_t LSM6DSRX_ACC_GetAxesRaw(LSM6DSRX_Object_t *pObj, LSM6DSRX_AxesRaw_t *Value);
int32_t LSM6DSRX_ACC_GetAxes(LSM6DSRX_Object_t *pObj, LSM6DSRX_Axes_t *Acceleration);

int32_t LSM6DSRX_GYRO_Enable(LSM6DSRX_Object_t *pObj);
int32_t LSM6DSRX_GYRO_Disable(LSM6DSRX_Object_t *pObj);
int32_t LSM6DSRX_GYRO_GetSensitivity(LSM6DSRX_Object_t *pObj, float *Sensitivity);
int32_t LSM6DSRX_GYRO_GetOutputDataRate(LSM6DSRX_Object_t *pObj, float *Odr);
int32_t LSM6DSRX_GYRO_SetOutputDataRate(LSM6DSRX_Object_t *pObj, float Odr);
int32_t LSM6DSRX_GYRO_GetFullScale(LSM6DSRX_Object_t *pObj, int32_t *FullScale);
int32_t LSM6DSRX_GYRO_SetFullScale(LSM6DSRX_Object_t *pObj, int32_t FullScale);
int32_t LSM6DSRX_GYRO_GetAxesRaw(LSM6DSRX_Object_t *pObj, LSM6DSRX_AxesRaw_t *Value);
int32_t LSM6DSRX_GYRO_GetAxes(LSM6DSRX_Object_t *pObj, LSM6DSRX_Axes_t *AngularRate);

int32_t LSM6DSRX_Read_Reg(LSM6DSRX_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t LSM6DSRX_Write_Reg(LSM6DSRX_Object_t *pObj, uint8_t reg, uint8_t Data);
int32_t LSM6DSRX_Set_Interrupt_Latch(LSM6DSRX_Object_t *pObj, uint8_t Status);

int32_t LSM6DSRX_ACC_Enable_Free_Fall_Detection(LSM6DSRX_Object_t *pObj, LSM6DSRX_SensorIntPin_t IntPin);
int32_t LSM6DSRX_ACC_Disable_Free_Fall_Detection(LSM6DSRX_Object_t *pObj);
int32_t LSM6DSRX_ACC_Set_Free_Fall_Threshold(LSM6DSRX_Object_t *pObj, uint8_t Threshold);
int32_t LSM6DSRX_ACC_Set_Free_Fall_Duration(LSM6DSRX_Object_t *pObj, uint8_t Duration);

int32_t LSM6DSRX_ACC_Enable_Pedometer(LSM6DSRX_Object_t *pObj);
int32_t LSM6DSRX_ACC_Disable_Pedometer(LSM6DSRX_Object_t *pObj);
int32_t LSM6DSRX_ACC_Get_Step_Count(LSM6DSRX_Object_t *pObj, uint16_t *StepCount);
int32_t LSM6DSRX_ACC_Step_Counter_Reset(LSM6DSRX_Object_t *pObj);

int32_t LSM6DSRX_ACC_Enable_Tilt_Detection(LSM6DSRX_Object_t *pObj, LSM6DSRX_SensorIntPin_t IntPin);
int32_t LSM6DSRX_ACC_Disable_Tilt_Detection(LSM6DSRX_Object_t *pObj);

int32_t LSM6DSRX_ACC_Enable_Wake_Up_Detection(LSM6DSRX_Object_t *pObj, LSM6DSRX_SensorIntPin_t IntPin);
int32_t LSM6DSRX_ACC_Disable_Wake_Up_Detection(LSM6DSRX_Object_t *pObj);
int32_t LSM6DSRX_ACC_Set_Wake_Up_Threshold(LSM6DSRX_Object_t *pObj, uint8_t Threshold);
int32_t LSM6DSRX_ACC_Set_Wake_Up_Duration(LSM6DSRX_Object_t *pObj, uint8_t Duration);

int32_t LSM6DSRX_ACC_Enable_Single_Tap_Detection(LSM6DSRX_Object_t *pObj, LSM6DSRX_SensorIntPin_t IntPin);
int32_t LSM6DSRX_ACC_Disable_Single_Tap_Detection(LSM6DSRX_Object_t *pObj);
int32_t LSM6DSRX_ACC_Enable_Double_Tap_Detection(LSM6DSRX_Object_t *pObj, LSM6DSRX_SensorIntPin_t IntPin);
int32_t LSM6DSRX_ACC_Disable_Double_Tap_Detection(LSM6DSRX_Object_t *pObj);
int32_t LSM6DSRX_ACC_Set_Tap_Threshold(LSM6DSRX_Object_t *pObj, uint8_t Threshold);
int32_t LSM6DSRX_ACC_Set_Tap_Shock_Time(LSM6DSRX_Object_t *pObj, uint8_t Time);
int32_t LSM6DSRX_ACC_Set_Tap_Quiet_Time(LSM6DSRX_Object_t *pObj, uint8_t Time);
int32_t LSM6DSRX_ACC_Set_Tap_Duration_Time(LSM6DSRX_Object_t *pObj, uint8_t Time);

int32_t LSM6DSRX_ACC_Enable_6D_Orientation(LSM6DSRX_Object_t *pObj, LSM6DSRX_SensorIntPin_t IntPin);
int32_t LSM6DSRX_ACC_Disable_6D_Orientation(LSM6DSRX_Object_t *pObj);
int32_t LSM6DSRX_ACC_Set_6D_Orientation_Threshold(LSM6DSRX_Object_t *pObj, uint8_t Threshold);
int32_t LSM6DSRX_ACC_Get_6D_Orientation_XL(LSM6DSRX_Object_t *pObj, uint8_t *XLow);
int32_t LSM6DSRX_ACC_Get_6D_Orientation_XH(LSM6DSRX_Object_t *pObj, uint8_t *XHigh);
int32_t LSM6DSRX_ACC_Get_6D_Orientation_YL(LSM6DSRX_Object_t *pObj, uint8_t *YLow);
int32_t LSM6DSRX_ACC_Get_6D_Orientation_YH(LSM6DSRX_Object_t *pObj, uint8_t *YHigh);
int32_t LSM6DSRX_ACC_Get_6D_Orientation_ZL(LSM6DSRX_Object_t *pObj, uint8_t *ZLow);
int32_t LSM6DSRX_ACC_Get_6D_Orientation_ZH(LSM6DSRX_Object_t *pObj, uint8_t *ZHigh);

int32_t LSM6DSRX_ACC_Get_DRDY_Status(LSM6DSRX_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSRX_ACC_Get_Event_Status(LSM6DSRX_Object_t *pObj, LSM6DSRX_Event_Status_t *Status);
int32_t LSM6DSRX_ACC_Set_SelfTest(LSM6DSRX_Object_t *pObj, uint8_t Status);

int32_t LSM6DSRX_GYRO_Get_DRDY_Status(LSM6DSRX_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSRX_GYRO_Set_SelfTest(LSM6DSRX_Object_t *pObj, uint8_t Status);

int32_t LSM6DSRX_FIFO_Get_Num_Samples(LSM6DSRX_Object_t *pObj, uint16_t *NumSamples);
int32_t LSM6DSRX_FIFO_Get_Full_Status(LSM6DSRX_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSRX_FIFO_Set_INT1_FIFO_Full(LSM6DSRX_Object_t *pObj, uint8_t Status);
int32_t LSM6DSRX_FIFO_Set_Watermark_Level(LSM6DSRX_Object_t *pObj, uint16_t Watermark);
int32_t LSM6DSRX_FIFO_Set_Stop_On_Fth(LSM6DSRX_Object_t *pObj, uint8_t Status);
int32_t LSM6DSRX_FIFO_Set_Mode(LSM6DSRX_Object_t *pObj, uint8_t Mode);
int32_t LSM6DSRX_FIFO_Get_Tag(LSM6DSRX_Object_t *pObj, uint8_t *Tag);
int32_t LSM6DSRX_FIFO_Get_Data(LSM6DSRX_Object_t *pObj, uint8_t *Data);
int32_t LSM6DSRX_FIFO_Get_Empty_Status(LSM6DSRX_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSRX_FIFO_Get_Overrun_Status(LSM6DSRX_Object_t *pObj, uint8_t *Status);
int32_t LSM6DSRX_FIFO_ACC_Get_Axes(LSM6DSRX_Object_t *pObj, LSM6DSRX_Axes_t *Acceleration);
int32_t LSM6DSRX_FIFO_ACC_Set_BDR(LSM6DSRX_Object_t *pObj, float Bdr);
int32_t LSM6DSRX_FIFO_GYRO_Get_Axes(LSM6DSRX_Object_t *pObj, LSM6DSRX_Axes_t *AngularVelocity);
int32_t LSM6DSRX_FIFO_GYRO_Set_BDR(LSM6DSRX_Object_t *pObj, float Bdr);

/**
  * @}
  */

/** @addtogroup LSM6DSRX_Exported_Variables LSM6DSRX Exported Variables
  * @{
  */

extern LSM6DSRX_CommonDrv_t LSM6DSRX_COMMON_Driver;
extern LSM6DSRX_ACC_Drv_t LSM6DSRX_ACC_Driver;
extern LSM6DSRX_GYRO_Drv_t LSM6DSRX_GYRO_Driver;

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
