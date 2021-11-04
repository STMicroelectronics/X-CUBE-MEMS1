/**
 ******************************************************************************
 * @file    lis2dw12.h
 * @author  MEMS Software Solutions Team
 * @brief   LIS2DW12 header driver file
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
#ifndef LIS2DW12_H
#define LIS2DW12_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "lis2dw12_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @addtogroup LIS2DW12 LIS2DW12
 * @{
 */

/** @defgroup LIS2DW12_Exported_Types LIS2DW12 Exported Types
 * @{
 */

typedef int32_t (*LIS2DW12_Init_Func)(void);
typedef int32_t (*LIS2DW12_DeInit_Func)(void);
typedef int32_t (*LIS2DW12_GetTick_Func)(void);
typedef int32_t (*LIS2DW12_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*LIS2DW12_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  LIS2DW12_INT1_PIN,
  LIS2DW12_INT2_PIN,
} LIS2DW12_SensorIntPin_t;

typedef struct
{
  LIS2DW12_Init_Func         Init;
  LIS2DW12_DeInit_Func       DeInit;
  uint32_t                   BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
  uint8_t                    Address;
  LIS2DW12_WriteReg_Func     WriteReg;
  LIS2DW12_ReadReg_Func      ReadReg;
  LIS2DW12_GetTick_Func      GetTick;
} LIS2DW12_IO_t;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} LIS2DW12_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} LIS2DW12_Axes_t;

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
} LIS2DW12_Event_Status_t;

typedef enum
{
  LIS2DW12_HIGH_PERFORMANCE_MODE,
  LIS2DW12_LOW_POWER_MODE4,
  LIS2DW12_LOW_POWER_MODE3,
  LIS2DW12_LOW_POWER_MODE2,
  LIS2DW12_LOW_POWER_MODE1
} LIS2DW12_Operating_Mode_t;

typedef enum
{
  LIS2DW12_LOW_NOISE_DISABLE,
  LIS2DW12_LOW_NOISE_ENABLE
} LIS2DW12_Low_Noise_t;

typedef struct
{
  LIS2DW12_IO_t             IO;
  stmdev_ctx_t              Ctx;
  uint8_t                   is_initialized;
  uint8_t                   acc_is_enabled;
  float                     acc_odr;
  LIS2DW12_Operating_Mode_t acc_operating_mode;
  LIS2DW12_Low_Noise_t      acc_low_noise;
} LIS2DW12_Object_t;

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
} LIS2DW12_Capabilities_t;

typedef struct
{
  int32_t (*Init)(LIS2DW12_Object_t *);
  int32_t (*DeInit)(LIS2DW12_Object_t *);
  int32_t (*ReadID)(LIS2DW12_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(LIS2DW12_Object_t *, LIS2DW12_Capabilities_t *);
} LIS2DW12_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(LIS2DW12_Object_t *);
  int32_t (*Disable)(LIS2DW12_Object_t *);
  int32_t (*GetSensitivity)(LIS2DW12_Object_t *, float *);
  int32_t (*GetOutputDataRate)(LIS2DW12_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LIS2DW12_Object_t *, float);
  int32_t (*GetFullScale)(LIS2DW12_Object_t *, int32_t *);
  int32_t (*SetFullScale)(LIS2DW12_Object_t *, int32_t);
  int32_t (*GetAxes)(LIS2DW12_Object_t *, LIS2DW12_Axes_t *);
  int32_t (*GetAxesRaw)(LIS2DW12_Object_t *, LIS2DW12_AxesRaw_t *);
} LIS2DW12_ACC_Drv_t;

typedef union{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} lis2dw12_axis3bit16_t;

typedef union{
  int16_t i16bit;
  uint8_t u8bit[2];
} lis2dw12_axis1bit16_t;

typedef union{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} lis2dw12_axis3bit32_t;

typedef union{
  int32_t i32bit;
  uint8_t u8bit[4];
} lis2dw12_axis1bit32_t;

/**
 * @}
 */

/** @defgroup LIS2DW12_Exported_Constants LIS2DW12 Exported Constants
 * @{
 */

#define LIS2DW12_OK                       0
#define LIS2DW12_ERROR                   -1

#define LIS2DW12_I2C_BUS                 0U
#define LIS2DW12_SPI_4WIRES_BUS          1U
#define LIS2DW12_SPI_3WIRES_BUS          2U

#define LIS2DW12_ACC_SENSITIVITY_FOR_FS_2G_LOPOW1_MODE   0.976f  /**< Sensitivity value for 2g full scale, Low-power1 mode [mg/LSB] */
#define LIS2DW12_ACC_SENSITIVITY_FOR_FS_2G_OTHER_MODES   0.244f  /**< Sensitivity value for 2g full scale, all other modes except Low-power1 [mg/LSB] */

#define LIS2DW12_ACC_SENSITIVITY_FOR_FS_4G_LOPOW1_MODE   1.952f  /**< Sensitivity value for 4g full scale, Low-power1 mode [mg/LSB] */
#define LIS2DW12_ACC_SENSITIVITY_FOR_FS_4G_OTHER_MODES   0.488f  /**< Sensitivity value for 4g full scale, all other modes except Low-power1 [mg/LSB] */

#define LIS2DW12_ACC_SENSITIVITY_FOR_FS_8G_LOPOW1_MODE   3.904f  /**< Sensitivity value for 8g full scale, Low-power1 mode [mg/LSB] */
#define LIS2DW12_ACC_SENSITIVITY_FOR_FS_8G_OTHER_MODES   0.976f  /**< Sensitivity value for 8g full scale, all other modes except Low-power1 [mg/LSB] */

#define LIS2DW12_ACC_SENSITIVITY_FOR_FS_16G_LOPOW1_MODE  7.808f  /**< Sensitivity value for 16g full scale, Low-power1 mode [mg/LSB] */
#define LIS2DW12_ACC_SENSITIVITY_FOR_FS_16G_OTHER_MODES  1.952f  /**< Sensitivity value for 16g full scale, all other modes except Low-power1 [mg/LSB] */

/**
 * @}
 */

/** @addtogroup LIS2DW12_Exported_Functions LIS2DW12 Exported Functions
 * @{
 */

int32_t LIS2DW12_RegisterBusIO(LIS2DW12_Object_t *pObj, LIS2DW12_IO_t *pIO);
int32_t LIS2DW12_Init(LIS2DW12_Object_t *pObj);
int32_t LIS2DW12_DeInit(LIS2DW12_Object_t *pObj);
int32_t LIS2DW12_ReadID(LIS2DW12_Object_t *pObj, uint8_t *Id);
int32_t LIS2DW12_GetCapabilities(LIS2DW12_Object_t *pObj, LIS2DW12_Capabilities_t *Capabilities);

int32_t LIS2DW12_ACC_Enable(LIS2DW12_Object_t *pObj);
int32_t LIS2DW12_ACC_Disable(LIS2DW12_Object_t *pObj);
int32_t LIS2DW12_ACC_GetSensitivity(LIS2DW12_Object_t *pObj, float *Sensitivity);
int32_t LIS2DW12_ACC_GetOutputDataRate(LIS2DW12_Object_t *pObj, float *Odr);
int32_t LIS2DW12_ACC_SetOutputDataRate(LIS2DW12_Object_t *pObj, float Odr);
int32_t LIS2DW12_ACC_SetOutputDataRate_With_Mode(LIS2DW12_Object_t *pObj, float Odr, LIS2DW12_Operating_Mode_t Mode, LIS2DW12_Low_Noise_t Noise);
int32_t LIS2DW12_ACC_GetFullScale(LIS2DW12_Object_t *pObj, int32_t *FullScale);
int32_t LIS2DW12_ACC_SetFullScale(LIS2DW12_Object_t *pObj, int32_t FullScale);
int32_t LIS2DW12_ACC_GetAxesRaw(LIS2DW12_Object_t *pObj, LIS2DW12_AxesRaw_t *Value);
int32_t LIS2DW12_ACC_GetAxes(LIS2DW12_Object_t *pObj, LIS2DW12_Axes_t *Acceleration);

int32_t LIS2DW12_Read_Reg(LIS2DW12_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t LIS2DW12_Write_Reg(LIS2DW12_Object_t *pObj, uint8_t reg, uint8_t Data);
int32_t LIS2DW12_Set_Interrupt_Latch(LIS2DW12_Object_t *pObj, uint8_t Status);

int32_t LIS2DW12_ACC_Enable_DRDY_Interrupt(LIS2DW12_Object_t *pObj);
int32_t LIS2DW12_ACC_Disable_DRDY_Interrupt(LIS2DW12_Object_t *pObj);

int32_t LIS2DW12_ACC_Set_Filter_Mode(LIS2DW12_Object_t *pObj, uint8_t filterMode);

int32_t LIS2DW12_ACC_Enable_Wake_Up_Detection(LIS2DW12_Object_t *pObj);
int32_t LIS2DW12_ACC_Disable_Wake_Up_Detection(LIS2DW12_Object_t *pObj);
int32_t LIS2DW12_ACC_Set_Wake_Up_Threshold(LIS2DW12_Object_t *pObj, uint8_t Threshold);
int32_t LIS2DW12_ACC_Set_Wake_Up_Duration(LIS2DW12_Object_t *pObj, uint8_t Duration);

int32_t LIS2DW12_ACC_Enable_Inactivity_Detection(LIS2DW12_Object_t *pObj);
int32_t LIS2DW12_ACC_Disable_Inactivity_Detection(LIS2DW12_Object_t *pObj);
int32_t LIS2DW12_ACC_Set_Sleep_Duration(LIS2DW12_Object_t *pObj, uint8_t Duration);

int32_t LIS2DW12_ACC_Enable_6D_Orientation(LIS2DW12_Object_t *pObj);
int32_t LIS2DW12_ACC_Disable_6D_Orientation(LIS2DW12_Object_t *pObj);
int32_t LIS2DW12_ACC_Set_6D_Orientation_Threshold(LIS2DW12_Object_t *pObj, uint8_t Threshold);
int32_t LIS2DW12_ACC_Get_6D_Orientation_XL(LIS2DW12_Object_t *pObj, uint8_t *XLow);
int32_t LIS2DW12_ACC_Get_6D_Orientation_XH(LIS2DW12_Object_t *pObj, uint8_t *XHigh);
int32_t LIS2DW12_ACC_Get_6D_Orientation_YL(LIS2DW12_Object_t *pObj, uint8_t *YLow);
int32_t LIS2DW12_ACC_Get_6D_Orientation_YH(LIS2DW12_Object_t *pObj, uint8_t *YHigh);
int32_t LIS2DW12_ACC_Get_6D_Orientation_ZL(LIS2DW12_Object_t *pObj, uint8_t *ZLow);
int32_t LIS2DW12_ACC_Get_6D_Orientation_ZH(LIS2DW12_Object_t *pObj, uint8_t *ZHigh);

int32_t LIS2DW12_ACC_Get_Event_Status(LIS2DW12_Object_t *pObj, LIS2DW12_Event_Status_t *Status);
int32_t LIS2DW12_ACC_Set_SelfTest(LIS2DW12_Object_t *pObj, uint8_t Val);
int32_t LIS2DW12_ACC_Get_DRDY_Status(LIS2DW12_Object_t *pObj, uint8_t *Status);
int32_t LIS2DW12_ACC_Get_Init_Status(LIS2DW12_Object_t *pObj, uint8_t *Status);

int32_t LIS2DW12_FIFO_Get_Num_Samples(LIS2DW12_Object_t *pObj, uint16_t *NumSamples);
int32_t LIS2DW12_FIFO_Set_Mode(LIS2DW12_Object_t *pObj, uint8_t Mode);

/**
 * @}
 */

/** @addtogroup LIS2DW12_Exported_Variables LIS2DW12 Exported Variables
 * @{
 */

extern LIS2DW12_CommonDrv_t LIS2DW12_COMMON_Driver;
extern LIS2DW12_ACC_Drv_t LIS2DW12_ACC_Driver;

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
