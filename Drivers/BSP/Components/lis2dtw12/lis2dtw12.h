/**
  ******************************************************************************
  * @file    lis2dtw12.h
  * @author  MEMS Software Solutions Team
  * @brief   LIS2DTW12 header driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LIS2DTW12_H
#define LIS2DTW12_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "lis2dtw12_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup LIS2DTW12 LIS2DTW12
  * @{
  */

/** @defgroup LIS2DTW12_Exported_Types LIS2DTW12 Exported Types
  * @{
  */

typedef int32_t (*LIS2DTW12_Init_Func)(void);
typedef int32_t (*LIS2DTW12_DeInit_Func)(void);
typedef int32_t (*LIS2DTW12_GetTick_Func)(void);
typedef int32_t (*LIS2DTW12_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*LIS2DTW12_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  LIS2DTW12_INT1_PIN,
  LIS2DTW12_INT2_PIN,
} LIS2DTW12_SensorIntPin_t;

typedef struct
{
  LIS2DTW12_Init_Func     Init;
  LIS2DTW12_DeInit_Func   DeInit;
  uint32_t                BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
  uint8_t                 Address;
  LIS2DTW12_WriteReg_Func WriteReg;
  LIS2DTW12_ReadReg_Func  ReadReg;
  LIS2DTW12_GetTick_Func  GetTick;
} LIS2DTW12_IO_t;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} LIS2DTW12_ACC_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} LIS2DTW12_ACC_Axes_t;

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
} LIS2DTW12_Event_Status_t;

typedef enum
{
  LIS2DTW12_HIGH_PERFORMANCE_MODE,
  LIS2DTW12_LOW_POWER_MODE4,
  LIS2DTW12_LOW_POWER_MODE3,
  LIS2DTW12_LOW_POWER_MODE2,
  LIS2DTW12_LOW_POWER_MODE1
} LIS2DTW12_Operating_Mode_t;

typedef enum
{
  LIS2DTW12_LOW_NOISE_DISABLE,
  LIS2DTW12_LOW_NOISE_ENABLE
} LIS2DTW12_Low_Noise_t;

typedef struct
{
  LIS2DTW12_IO_t             IO;
  stmdev_ctx_t               Ctx;
  uint8_t                    is_initialized;
  uint8_t                    acc_is_enabled;
  uint8_t                    temp_is_enabled;
  float                      acc_odr;
  LIS2DTW12_Operating_Mode_t acc_operating_mode;
  LIS2DTW12_Low_Noise_t      acc_low_noise;
  float                      temp_odr;
} LIS2DTW12_Object_t;

typedef struct
{
  uint8_t   Acc;
  uint8_t   Gyro;
  uint8_t   Magneto;
  uint8_t   Temperature;
  uint8_t   Pressure;
  uint8_t   Humidity;
  uint8_t   LowPower;
  uint32_t  GyroMaxFS;
  uint32_t  AccMaxFS;
  uint32_t  MagMaxFS;
  float     GyroMaxOdr;
  float     AccMaxOdr;
  float     MagMaxOdr;
  float     HumMaxOdr;
  float     TempMaxOdr;
  float     PressMaxOdr;
} LIS2DTW12_Capabilities_t;

typedef struct
{
  int32_t (*Init)(LIS2DTW12_Object_t *);
  int32_t (*DeInit)(LIS2DTW12_Object_t *);
  int32_t (*ReadID)(LIS2DTW12_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(LIS2DTW12_Object_t *, LIS2DTW12_Capabilities_t *);
} LIS2DTW12_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(LIS2DTW12_Object_t *);
  int32_t (*Disable)(LIS2DTW12_Object_t *);
  int32_t (*GetSensitivity)(LIS2DTW12_Object_t *, float *);
  int32_t (*GetOutputDataRate)(LIS2DTW12_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LIS2DTW12_Object_t *, float);
  int32_t (*GetFullScale)(LIS2DTW12_Object_t *, int32_t *);
  int32_t (*SetFullScale)(LIS2DTW12_Object_t *, int32_t);
  int32_t (*GetAxes)(LIS2DTW12_Object_t *, LIS2DTW12_ACC_Axes_t *);
  int32_t (*GetAxesRaw)(LIS2DTW12_Object_t *, LIS2DTW12_ACC_AxesRaw_t *);
} LIS2DTW12_ACC_Drv_t;

typedef struct
{
  int32_t (*Enable)(LIS2DTW12_Object_t *);
  int32_t (*Disable)(LIS2DTW12_Object_t *);
  int32_t (*GetOutputDataRate)(LIS2DTW12_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LIS2DTW12_Object_t *, float);
  int32_t (*GetValue)(LIS2DTW12_Object_t *, float *);
} LIS2DTW12_TEMP_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} lis2dtw12_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} lis2dtw12_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} lis2dtw12_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} lis2dtw12_axis1bit32_t;

/**
  * @}
  */

/** @defgroup LIS2DTW12_Exported_Constants LIS2DTW12 Exported Constants
  * @{
  */

#define LIS2DTW12_OK                       0
#define LIS2DTW12_ERROR                   -1

#define LIS2DTW12_I2C_BUS                 0U
#define LIS2DTW12_SPI_4WIRES_BUS          1U
#define LIS2DTW12_SPI_3WIRES_BUS          2U

#define LIS2DTW12_ACC_SENSITIVITY_FOR_FS_2G_LOPOW1_MODE   0.976f  /**< Sensitivity value for 2g full scale, Low-power1 mode [mg/LSB] */
#define LIS2DTW12_ACC_SENSITIVITY_FOR_FS_2G_OTHER_MODES   0.244f  /**< Sensitivity value for 2g full scale, all other modes except Low-power1 [mg/LSB] */

#define LIS2DTW12_ACC_SENSITIVITY_FOR_FS_4G_LOPOW1_MODE   1.952f  /**< Sensitivity value for 4g full scale, Low-power1 mode [mg/LSB] */
#define LIS2DTW12_ACC_SENSITIVITY_FOR_FS_4G_OTHER_MODES   0.488f  /**< Sensitivity value for 4g full scale, all other modes except Low-power1 [mg/LSB] */

#define LIS2DTW12_ACC_SENSITIVITY_FOR_FS_8G_LOPOW1_MODE   3.904f  /**< Sensitivity value for 8g full scale, Low-power1 mode [mg/LSB] */
#define LIS2DTW12_ACC_SENSITIVITY_FOR_FS_8G_OTHER_MODES   0.976f  /**< Sensitivity value for 8g full scale, all other modes except Low-power1 [mg/LSB] */

#define LIS2DTW12_ACC_SENSITIVITY_FOR_FS_16G_LOPOW1_MODE  7.808f  /**< Sensitivity value for 16g full scale, Low-power1 mode [mg/LSB] */
#define LIS2DTW12_ACC_SENSITIVITY_FOR_FS_16G_OTHER_MODES  1.952f  /**< Sensitivity value for 16g full scale, all other modes except Low-power1 [mg/LSB] */

/**
  * @}
  */

/** @addtogroup LIS2DTW12_Exported_Functions LIS2DTW12 Exported Functions
  * @{
  */

int32_t LIS2DTW12_RegisterBusIO(LIS2DTW12_Object_t *pObj, LIS2DTW12_IO_t *pIO);
int32_t LIS2DTW12_Init(LIS2DTW12_Object_t *pObj);
int32_t LIS2DTW12_DeInit(LIS2DTW12_Object_t *pObj);
int32_t LIS2DTW12_ReadID(LIS2DTW12_Object_t *pObj, uint8_t *Id);
int32_t LIS2DTW12_GetCapabilities(LIS2DTW12_Object_t *pObj, LIS2DTW12_Capabilities_t *Capabilities);

int32_t LIS2DTW12_Read_Reg(LIS2DTW12_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t LIS2DTW12_Write_Reg(LIS2DTW12_Object_t *pObj, uint8_t reg, uint8_t Data);
int32_t LIS2DTW12_Set_Interrupt_Latch(LIS2DTW12_Object_t *pObj, uint8_t Status);

int32_t LIS2DTW12_ACC_Enable(LIS2DTW12_Object_t *pObj);
int32_t LIS2DTW12_ACC_Disable(LIS2DTW12_Object_t *pObj);
int32_t LIS2DTW12_ACC_GetSensitivity(LIS2DTW12_Object_t *pObj, float *Sensitivity);
int32_t LIS2DTW12_ACC_GetOutputDataRate(LIS2DTW12_Object_t *pObj, float *Odr);
int32_t LIS2DTW12_ACC_SetOutputDataRate(LIS2DTW12_Object_t *pObj, float Odr);
int32_t LIS2DTW12_ACC_SetOutputDataRate_With_Mode(LIS2DTW12_Object_t *pObj, float Odr, LIS2DTW12_Operating_Mode_t Mode,
                                                  LIS2DTW12_Low_Noise_t Noise);
int32_t LIS2DTW12_ACC_GetFullScale(LIS2DTW12_Object_t *pObj, int32_t *FullScale);
int32_t LIS2DTW12_ACC_SetFullScale(LIS2DTW12_Object_t *pObj, int32_t FullScale);
int32_t LIS2DTW12_ACC_GetAxesRaw(LIS2DTW12_Object_t *pObj, LIS2DTW12_ACC_AxesRaw_t *Value);
int32_t LIS2DTW12_ACC_GetAxes(LIS2DTW12_Object_t *pObj, LIS2DTW12_ACC_Axes_t *Acceleration);

int32_t LIS2DTW12_ACC_Enable_DRDY_Interrupt(LIS2DTW12_Object_t *pObj);
int32_t LIS2DTW12_ACC_Disable_DRDY_Interrupt(LIS2DTW12_Object_t *pObj);

int32_t LIS2DTW12_ACC_Set_Filter_Mode(LIS2DTW12_Object_t *pObj, uint8_t filterMode);

int32_t LIS2DTW12_ACC_Enable_Wake_Up_Detection(LIS2DTW12_Object_t *pObj);
int32_t LIS2DTW12_ACC_Disable_Wake_Up_Detection(LIS2DTW12_Object_t *pObj);
int32_t LIS2DTW12_ACC_Set_Wake_Up_Threshold(LIS2DTW12_Object_t *pObj, uint8_t Threshold);
int32_t LIS2DTW12_ACC_Set_Wake_Up_Duration(LIS2DTW12_Object_t *pObj, uint8_t Duration);

int32_t LIS2DTW12_ACC_Enable_Inactivity_Detection(LIS2DTW12_Object_t *pObj);
int32_t LIS2DTW12_ACC_Disable_Inactivity_Detection(LIS2DTW12_Object_t *pObj);
int32_t LIS2DTW12_ACC_Set_Sleep_Duration(LIS2DTW12_Object_t *pObj, uint8_t Duration);

int32_t LIS2DTW12_ACC_Enable_6D_Orientation(LIS2DTW12_Object_t *pObj);
int32_t LIS2DTW12_ACC_Disable_6D_Orientation(LIS2DTW12_Object_t *pObj);
int32_t LIS2DTW12_ACC_Set_6D_Orientation_Threshold(LIS2DTW12_Object_t *pObj, uint8_t Threshold);
int32_t LIS2DTW12_ACC_Get_6D_Orientation_XL(LIS2DTW12_Object_t *pObj, uint8_t *XLow);
int32_t LIS2DTW12_ACC_Get_6D_Orientation_XH(LIS2DTW12_Object_t *pObj, uint8_t *XHigh);
int32_t LIS2DTW12_ACC_Get_6D_Orientation_YL(LIS2DTW12_Object_t *pObj, uint8_t *YLow);
int32_t LIS2DTW12_ACC_Get_6D_Orientation_YH(LIS2DTW12_Object_t *pObj, uint8_t *YHigh);
int32_t LIS2DTW12_ACC_Get_6D_Orientation_ZL(LIS2DTW12_Object_t *pObj, uint8_t *ZLow);
int32_t LIS2DTW12_ACC_Get_6D_Orientation_ZH(LIS2DTW12_Object_t *pObj, uint8_t *ZHigh);

int32_t LIS2DTW12_ACC_Get_Event_Status(LIS2DTW12_Object_t *pObj, LIS2DTW12_Event_Status_t *Status);
int32_t LIS2DTW12_ACC_Set_SelfTest(LIS2DTW12_Object_t *pObj, uint8_t Val);
int32_t LIS2DTW12_ACC_Get_DRDY_Status(LIS2DTW12_Object_t *pObj, uint8_t *Status);
int32_t LIS2DTW12_ACC_Get_Init_Status(LIS2DTW12_Object_t *pObj, uint8_t *Status);

int32_t LIS2DTW12_TEMP_Enable(LIS2DTW12_Object_t *pObj);
int32_t LIS2DTW12_TEMP_Disable(LIS2DTW12_Object_t *pObj);
int32_t LIS2DTW12_TEMP_GetOutputDataRate(LIS2DTW12_Object_t *pObj, float *Odr);
int32_t LIS2DTW12_TEMP_SetOutputDataRate(LIS2DTW12_Object_t *pObj, float Odr);
int32_t LIS2DTW12_TEMP_GetValue(LIS2DTW12_Object_t *pObj, float *Temperature);

int32_t LIS2DTW12_FIFO_Get_Num_Samples(LIS2DTW12_Object_t *pObj, uint16_t *NumSamples);
int32_t LIS2DTW12_FIFO_Set_Mode(LIS2DTW12_Object_t *pObj, uint8_t Mode);

/**
  * @}
  */

/** @addtogroup LIS2DTW12_Exported_Variables LIS2DTW12 Exported Variables
  * @{
  */

extern LIS2DTW12_CommonDrv_t LIS2DTW12_COMMON_Driver;
extern LIS2DTW12_ACC_Drv_t LIS2DTW12_ACC_Driver;
extern LIS2DTW12_TEMP_Drv_t LIS2DTW12_TEMP_Driver;

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
