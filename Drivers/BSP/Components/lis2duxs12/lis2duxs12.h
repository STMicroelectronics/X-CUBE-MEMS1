/**
  ******************************************************************************
  * @file    lis2duxs12.h
  * @author  MEMS Software Solutions Team
  * @brief   LIS2DUXS12 header driver file
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
#ifndef LIS2DUXS12_H
#define LIS2DUXS12_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "lis2duxs12_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup LIS2DUXS12 LIS2DUXS12
  * @{
  */

/** @defgroup LIS2DUXS12_Exported_Types LIS2DUXS12 Exported Types
  * @{
  */

typedef int32_t (*LIS2DUXS12_Init_Func)(void);
typedef int32_t (*LIS2DUXS12_DeInit_Func)(void);
typedef int32_t (*LIS2DUXS12_GetTick_Func)(void);
typedef void (*LIS2DUXS12_Delay_Func)(uint32_t);
typedef int32_t (*LIS2DUXS12_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*LIS2DUXS12_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  LIS2DUXS12_INT1_PIN,
  LIS2DUXS12_INT2_PIN,
} LIS2DUXS12_SensorIntPin_t;

typedef struct
{
  LIS2DUXS12_Init_Func       Init;
  LIS2DUXS12_DeInit_Func     DeInit;
  uint32_t                   BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires, 3 means I3C */
  uint8_t                    Address;
  LIS2DUXS12_WriteReg_Func   WriteReg;
  LIS2DUXS12_ReadReg_Func    ReadReg;
  LIS2DUXS12_GetTick_Func    GetTick;
  LIS2DUXS12_Delay_Func      Delay;
} LIS2DUXS12_IO_t;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} LIS2DUXS12_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} LIS2DUXS12_Axes_t;

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
} LIS2DUXS12_Event_Status_t;

typedef enum
{
  LIS2DUXS12_ULTRA_LOW_POWER,
  LIS2DUXS12_LOW_POWER,
  LIS2DUXS12_HIGH_PERFORMANCE,
} LIS2DUXS12_Power_Mode_t;

typedef struct
{
  LIS2DUXS12_IO_t         IO;
  stmdev_ctx_t            Ctx;
  uint8_t                 is_initialized;
  uint8_t                 acc_is_enabled;
  float                   acc_odr;
  LIS2DUXS12_Power_Mode_t power_mode;
} LIS2DUXS12_Object_t;

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
} LIS2DUXS12_Capabilities_t;

typedef struct
{
  int32_t (*Init)(LIS2DUXS12_Object_t *);
  int32_t (*DeInit)(LIS2DUXS12_Object_t *);
  int32_t (*ReadID)(LIS2DUXS12_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(LIS2DUXS12_Object_t *, LIS2DUXS12_Capabilities_t *);
} LIS2DUXS12_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(LIS2DUXS12_Object_t *);
  int32_t (*Disable)(LIS2DUXS12_Object_t *);
  int32_t (*GetSensitivity)(LIS2DUXS12_Object_t *, float *);
  int32_t (*GetOutputDataRate)(LIS2DUXS12_Object_t *, float *);
  int32_t (*SetOutputDataRate)(LIS2DUXS12_Object_t *, float);
  int32_t (*GetFullScale)(LIS2DUXS12_Object_t *, int32_t *);
  int32_t (*SetFullScale)(LIS2DUXS12_Object_t *, int32_t);
  int32_t (*GetAxes)(LIS2DUXS12_Object_t *, LIS2DUXS12_Axes_t *);
  int32_t (*GetAxesRaw)(LIS2DUXS12_Object_t *, LIS2DUXS12_AxesRaw_t *);
} LIS2DUXS12_ACC_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} lis2duxs12_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} lis2duxs12_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} lis2duxs12_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} lis2duxs12_axis1bit32_t;

/**
  * @}
  */

/** @defgroup LIS2DUXS12_Exported_Constants LIS2DUXS12 Exported Constants
  * @{
  */

#define LIS2DUXS12_OK                       0
#define LIS2DUXS12_ERROR                   -1

#define LIS2DUXS12_I2C_BUS                 0U
#define LIS2DUXS12_SPI_4WIRES_BUS          1U
#define LIS2DUXS12_SPI_3WIRES_BUS          2U
#define LIS2DUXS12_I3C_BUS                 3U

#define LIS2DUXS12_ACC_SENSITIVITY_FOR_FS_2G   0.061f  /**< Sensitivity value for 2g full scale, Low-power1 mode [mg/LSB] */
#define LIS2DUXS12_ACC_SENSITIVITY_FOR_FS_4G   0.122f  /**< Sensitivity value for 4g full scale, Low-power1 mode [mg/LSB] */
#define LIS2DUXS12_ACC_SENSITIVITY_FOR_FS_8G   0.244f  /**< Sensitivity value for 8g full scale, Low-power1 mode [mg/LSB] */
#define LIS2DUXS12_ACC_SENSITIVITY_FOR_FS_16G  0.488f  /**< Sensitivity value for 16g full scale, Low-power1 mode [mg/LSB] */

#define LIS2DUXS12_QVAR_GAIN  74.4f  /**< Gain value for bits AH_QVAR_GAIN_[1:0] == 1 in AH_QVAR_CFG register, 16 bits [LSB/mV] */

#define LIS2DUXS12_QVAR_GAIN_MULTIPL_0_5X  0.5f  /**< Gain multiplier for bits AH_QVAR_GAIN_[1:0] == 0 in AH_QVAR_CFG register [-] */
#define LIS2DUXS12_QVAR_GAIN_MULTIPL_1X    1.0f  /**< Gain multiplier for bits AH_QVAR_GAIN_[1:0] == 1 in AH_QVAR_CFG register [-] */
#define LIS2DUXS12_QVAR_GAIN_MULTIPL_2X    2.0f  /**< Gain multiplier for bits AH_QVAR_GAIN_[1:0] == 2 in AH_QVAR_CFG register [-] */
#define LIS2DUXS12_QVAR_GAIN_MULTIPL_4X    4.0f  /**< Gain multiplier for bits AH_QVAR_GAIN_[1:0] == 3 in AH_QVAR_CFG register [-] */

/**
  * @}
  */

/** @addtogroup LIS2DUXS12_Exported_Functions LIS2DUXS12 Exported Functions
  * @{
  */

int32_t LIS2DUXS12_RegisterBusIO(LIS2DUXS12_Object_t *pObj, LIS2DUXS12_IO_t *pIO);
int32_t LIS2DUXS12_Init(LIS2DUXS12_Object_t *pObj);
int32_t LIS2DUXS12_DeInit(LIS2DUXS12_Object_t *pObj);
int32_t LIS2DUXS12_ExitDeepPowerDownI2C(LIS2DUXS12_Object_t *pObj);
int32_t LIS2DUXS12_ExitDeepPowerDownSPI(LIS2DUXS12_Object_t *pObj);
int32_t LIS2DUXS12_ReadID(LIS2DUXS12_Object_t *pObj, uint8_t *Id);
int32_t LIS2DUXS12_GetCapabilities(LIS2DUXS12_Object_t *pObj, LIS2DUXS12_Capabilities_t *Capabilities);

int32_t LIS2DUXS12_ACC_Enable(LIS2DUXS12_Object_t *pObj);
int32_t LIS2DUXS12_ACC_Disable(LIS2DUXS12_Object_t *pObj);
int32_t LIS2DUXS12_ACC_GetSensitivity(LIS2DUXS12_Object_t *pObj, float *Sensitivity);
int32_t LIS2DUXS12_ACC_GetOutputDataRate(LIS2DUXS12_Object_t *pObj, float *Odr);
int32_t LIS2DUXS12_ACC_SetOutputDataRate(LIS2DUXS12_Object_t *pObj, float Odr);
int32_t LIS2DUXS12_ACC_SetOutputDataRate_With_Mode(LIS2DUXS12_Object_t *pObj, float Odr, LIS2DUXS12_Power_Mode_t Power);
int32_t LIS2DUXS12_ACC_GetFullScale(LIS2DUXS12_Object_t *pObj, int32_t *FullScale);
int32_t LIS2DUXS12_ACC_SetFullScale(LIS2DUXS12_Object_t *pObj, int32_t FullScale);
int32_t LIS2DUXS12_ACC_GetAxesRaw(LIS2DUXS12_Object_t *pObj, LIS2DUXS12_AxesRaw_t *Value);
int32_t LIS2DUXS12_ACC_GetAxes(LIS2DUXS12_Object_t *pObj, LIS2DUXS12_Axes_t *Acceleration);

int32_t LIS2DUXS12_Read_Reg(LIS2DUXS12_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t LIS2DUXS12_Write_Reg(LIS2DUXS12_Object_t *pObj, uint8_t reg, uint8_t Data);
int32_t LIS2DUXS12_Set_Interrupt_Latch(LIS2DUXS12_Object_t *pObj, uint8_t Status);

int32_t LIS2DUXS12_ACC_Enable_DRDY_Interrupt(LIS2DUXS12_Object_t *pObj);
int32_t LIS2DUXS12_ACC_Disable_DRDY_Interrupt(LIS2DUXS12_Object_t *pObj);

int32_t LIS2DUXS12_ACC_Set_SelfTest(LIS2DUXS12_Object_t *pObj, uint8_t Val);
int32_t LIS2DUXS12_ACC_Get_DRDY_Status(LIS2DUXS12_Object_t *pObj, uint8_t *Status);
int32_t LIS2DUXS12_ACC_Get_Init_Status(LIS2DUXS12_Object_t *pObj, uint8_t *Status);

int32_t LIS2DUXS12_ACC_Get_Event_Status(LIS2DUXS12_Object_t *pObj, LIS2DUXS12_Event_Status_t *Status);

int32_t LIS2DUXS12_ACC_Enable_Wake_Up_Detection(LIS2DUXS12_Object_t *pObj, LIS2DUXS12_SensorIntPin_t IntPin);
int32_t LIS2DUXS12_ACC_Disable_Wake_Up_Detection(LIS2DUXS12_Object_t *pObj);
int32_t LIS2DUXS12_ACC_Set_Wake_Up_Threshold(LIS2DUXS12_Object_t *pObj, uint32_t Threshold);
int32_t LIS2DUXS12_ACC_Set_Wake_Up_Duration(LIS2DUXS12_Object_t *pObj, uint8_t Duration);

int32_t LIS2DUXS12_ACC_Enable_6D_Orientation(LIS2DUXS12_Object_t *pObj, LIS2DUXS12_SensorIntPin_t IntPin);
int32_t LIS2DUXS12_ACC_Disable_6D_Orientation(LIS2DUXS12_Object_t *pObj);
int32_t LIS2DUXS12_ACC_Set_6D_Orientation_Threshold(LIS2DUXS12_Object_t *pObj, uint8_t Threshold);
int32_t LIS2DUXS12_ACC_Get_6D_Orientation_XL(LIS2DUXS12_Object_t *pObj, uint8_t *XLow);
int32_t LIS2DUXS12_ACC_Get_6D_Orientation_XH(LIS2DUXS12_Object_t *pObj, uint8_t *XHigh);
int32_t LIS2DUXS12_ACC_Get_6D_Orientation_YL(LIS2DUXS12_Object_t *pObj, uint8_t *YLow);
int32_t LIS2DUXS12_ACC_Get_6D_Orientation_YH(LIS2DUXS12_Object_t *pObj, uint8_t *YHigh);
int32_t LIS2DUXS12_ACC_Get_6D_Orientation_ZL(LIS2DUXS12_Object_t *pObj, uint8_t *ZLow);
int32_t LIS2DUXS12_ACC_Get_6D_Orientation_ZH(LIS2DUXS12_Object_t *pObj, uint8_t *ZHigh);

/**
  * @}
  */

/** @addtogroup LIS2DUXS12_Exported_Variables LIS2DUXS12 Exported Variables
  * @{
  */

extern LIS2DUXS12_CommonDrv_t LIS2DUXS12_COMMON_Driver;
extern LIS2DUXS12_ACC_Drv_t LIS2DUXS12_ACC_Driver;

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
