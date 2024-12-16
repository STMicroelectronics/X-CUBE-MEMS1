/**
  ******************************************************************************
  * @file    st1vafe3bx.h
  * @author  MEMS Software Solutions Team
  * @brief   ST1VAFE3BX header driver file
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
#ifndef ST1VAFE3BX_H
#define ST1VAFE3BX_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "st1vafe3bx_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup ST1VAFE3BX ST1VAFE3BX
  * @{
  */

/** @defgroup ST1VAFE3BX_Exported_Types ST1VAFE3BX Exported Types
  * @{
  */

typedef int32_t (*ST1VAFE3BX_Init_Func)(void);
typedef int32_t (*ST1VAFE3BX_DeInit_Func)(void);
typedef int32_t (*ST1VAFE3BX_GetTick_Func)(void);
typedef void (*ST1VAFE3BX_Delay_Func)(uint32_t);
typedef int32_t (*ST1VAFE3BX_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*ST1VAFE3BX_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  ST1VAFE3BX_INT1_PIN,
  ST1VAFE3BX_INT2_PIN,
} ST1VAFE3BX_SensorIntPin_t;

typedef struct
{
  ST1VAFE3BX_Init_Func       Init;
  ST1VAFE3BX_DeInit_Func     DeInit;
  uint32_t                   BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires, 3 means I3C */
  uint8_t                    Address;
  ST1VAFE3BX_WriteReg_Func   WriteReg;
  ST1VAFE3BX_ReadReg_Func    ReadReg;
  ST1VAFE3BX_GetTick_Func    GetTick;
  ST1VAFE3BX_Delay_Func      Delay;
} ST1VAFE3BX_IO_t;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} ST1VAFE3BX_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} ST1VAFE3BX_Axes_t;

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
} ST1VAFE3BX_Event_Status_t;

typedef enum
{
  ST1VAFE3BX_ULTRA_LOW_POWER,
  ST1VAFE3BX_LOW_POWER,
  ST1VAFE3BX_HIGH_PERFORMANCE,
} ST1VAFE3BX_Power_Mode_t;

typedef struct
{
  ST1VAFE3BX_IO_t         IO;
  stmdev_ctx_t            Ctx;
  uint8_t                 is_initialized;
  uint8_t                 acc_is_enabled;
  float                   acc_odr;
  ST1VAFE3BX_Power_Mode_t power_mode;
} ST1VAFE3BX_Object_t;

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
} ST1VAFE3BX_Capabilities_t;

typedef struct
{
  int32_t (*Init)(ST1VAFE3BX_Object_t *);
  int32_t (*DeInit)(ST1VAFE3BX_Object_t *);
  int32_t (*ReadID)(ST1VAFE3BX_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(ST1VAFE3BX_Object_t *, ST1VAFE3BX_Capabilities_t *);
} ST1VAFE3BX_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(ST1VAFE3BX_Object_t *);
  int32_t (*Disable)(ST1VAFE3BX_Object_t *);
  int32_t (*GetSensitivity)(ST1VAFE3BX_Object_t *, float *);
  int32_t (*GetOutputDataRate)(ST1VAFE3BX_Object_t *, float *);
  int32_t (*SetOutputDataRate)(ST1VAFE3BX_Object_t *, float);
  int32_t (*GetFullScale)(ST1VAFE3BX_Object_t *, int32_t *);
  int32_t (*SetFullScale)(ST1VAFE3BX_Object_t *, int32_t);
  int32_t (*GetAxes)(ST1VAFE3BX_Object_t *, ST1VAFE3BX_Axes_t *);
  int32_t (*GetAxesRaw)(ST1VAFE3BX_Object_t *, ST1VAFE3BX_AxesRaw_t *);
} ST1VAFE3BX_ACC_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} st1vafe3bx_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} st1vafe3bx_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} st1vafe3bx_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} st1vafe3bx_axis1bit32_t;

/**
  * @}
  */

/** @defgroup ST1VAFE3BX_Exported_Constants ST1VAFE3BX Exported Constants
  * @{
  */

#define ST1VAFE3BX_OK                       0
#define ST1VAFE3BX_ERROR                   -1

#define ST1VAFE3BX_I2C_BUS                 0U
#define ST1VAFE3BX_SPI_4WIRES_BUS          1U
#define ST1VAFE3BX_SPI_3WIRES_BUS          2U
#define ST1VAFE3BX_I3C_BUS                 3U

#define ST1VAFE3BX_ACC_SENSITIVITY_FOR_FS_2G   0.061f  /**< Sensitivity value for 2g full scale, Low-power1 mode [mg/LSB] */
#define ST1VAFE3BX_ACC_SENSITIVITY_FOR_FS_4G   0.122f  /**< Sensitivity value for 4g full scale, Low-power1 mode [mg/LSB] */
#define ST1VAFE3BX_ACC_SENSITIVITY_FOR_FS_8G   0.244f  /**< Sensitivity value for 8g full scale, Low-power1 mode [mg/LSB] */
#define ST1VAFE3BX_ACC_SENSITIVITY_FOR_FS_16G  0.488f  /**< Sensitivity value for 16g full scale, Low-power1 mode [mg/LSB] */

/**
  * @}
  */

/** @addtogroup ST1VAFE3BX_Exported_Functions ST1VAFE3BX Exported Functions
  * @{
  */

int32_t ST1VAFE3BX_RegisterBusIO(ST1VAFE3BX_Object_t *pObj, ST1VAFE3BX_IO_t *pIO);
int32_t ST1VAFE3BX_Init(ST1VAFE3BX_Object_t *pObj);
int32_t ST1VAFE3BX_DeInit(ST1VAFE3BX_Object_t *pObj);
int32_t ST1VAFE3BX_ExitDeepPowerDownI2C(ST1VAFE3BX_Object_t *pObj);
int32_t ST1VAFE3BX_ExitDeepPowerDownSPI(ST1VAFE3BX_Object_t *pObj);
int32_t ST1VAFE3BX_ReadID(ST1VAFE3BX_Object_t *pObj, uint8_t *Id);
int32_t ST1VAFE3BX_GetCapabilities(ST1VAFE3BX_Object_t *pObj, ST1VAFE3BX_Capabilities_t *Capabilities);

int32_t ST1VAFE3BX_ACC_Enable(ST1VAFE3BX_Object_t *pObj);
int32_t ST1VAFE3BX_ACC_Disable(ST1VAFE3BX_Object_t *pObj);
int32_t ST1VAFE3BX_ACC_GetSensitivity(ST1VAFE3BX_Object_t *pObj, float *Sensitivity);
int32_t ST1VAFE3BX_ACC_GetOutputDataRate(ST1VAFE3BX_Object_t *pObj, float *Odr);
int32_t ST1VAFE3BX_ACC_SetOutputDataRate(ST1VAFE3BX_Object_t *pObj, float Odr);
int32_t ST1VAFE3BX_ACC_SetOutputDataRate_With_Mode(ST1VAFE3BX_Object_t *pObj, float Odr, ST1VAFE3BX_Power_Mode_t Power);
int32_t ST1VAFE3BX_ACC_GetFullScale(ST1VAFE3BX_Object_t *pObj, int32_t *FullScale);
int32_t ST1VAFE3BX_ACC_SetFullScale(ST1VAFE3BX_Object_t *pObj, int32_t FullScale);
int32_t ST1VAFE3BX_ACC_GetAxesRaw(ST1VAFE3BX_Object_t *pObj, ST1VAFE3BX_AxesRaw_t *Value);
int32_t ST1VAFE3BX_ACC_GetAxes(ST1VAFE3BX_Object_t *pObj, ST1VAFE3BX_Axes_t *Acceleration);

int32_t ST1VAFE3BX_Read_Reg(ST1VAFE3BX_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t ST1VAFE3BX_Write_Reg(ST1VAFE3BX_Object_t *pObj, uint8_t reg, uint8_t Data);
int32_t ST1VAFE3BX_Set_Interrupt_Latch(ST1VAFE3BX_Object_t *pObj, uint8_t Status);

int32_t ST1VAFE3BX_ACC_Enable_DRDY_Interrupt(ST1VAFE3BX_Object_t *pObj);
int32_t ST1VAFE3BX_ACC_Disable_DRDY_Interrupt(ST1VAFE3BX_Object_t *pObj);

int32_t ST1VAFE3BX_ACC_Set_SelfTest(ST1VAFE3BX_Object_t *pObj, uint8_t Val);
int32_t ST1VAFE3BX_ACC_Get_DRDY_Status(ST1VAFE3BX_Object_t *pObj, uint8_t *Status);
int32_t ST1VAFE3BX_ACC_Get_Init_Status(ST1VAFE3BX_Object_t *pObj, uint8_t *Status);

int32_t ST1VAFE3BX_ACC_Get_Event_Status(ST1VAFE3BX_Object_t *pObj, ST1VAFE3BX_Event_Status_t *Status);

int32_t ST1VAFE3BX_ACC_Enable_Wake_Up_Detection(ST1VAFE3BX_Object_t *pObj, ST1VAFE3BX_SensorIntPin_t IntPin);
int32_t ST1VAFE3BX_ACC_Disable_Wake_Up_Detection(ST1VAFE3BX_Object_t *pObj);
int32_t ST1VAFE3BX_ACC_Set_Wake_Up_Threshold(ST1VAFE3BX_Object_t *pObj, uint32_t Threshold);
int32_t ST1VAFE3BX_ACC_Set_Wake_Up_Duration(ST1VAFE3BX_Object_t *pObj, uint8_t Duration);

int32_t ST1VAFE3BX_ACC_Enable_6D_Orientation(ST1VAFE3BX_Object_t *pObj, ST1VAFE3BX_SensorIntPin_t IntPin);
int32_t ST1VAFE3BX_ACC_Disable_6D_Orientation(ST1VAFE3BX_Object_t *pObj);
int32_t ST1VAFE3BX_ACC_Set_6D_Orientation_Threshold(ST1VAFE3BX_Object_t *pObj, uint8_t Threshold);
int32_t ST1VAFE3BX_ACC_Get_6D_Orientation_XL(ST1VAFE3BX_Object_t *pObj, uint8_t *XLow);
int32_t ST1VAFE3BX_ACC_Get_6D_Orientation_XH(ST1VAFE3BX_Object_t *pObj, uint8_t *XHigh);
int32_t ST1VAFE3BX_ACC_Get_6D_Orientation_YL(ST1VAFE3BX_Object_t *pObj, uint8_t *YLow);
int32_t ST1VAFE3BX_ACC_Get_6D_Orientation_YH(ST1VAFE3BX_Object_t *pObj, uint8_t *YHigh);
int32_t ST1VAFE3BX_ACC_Get_6D_Orientation_ZL(ST1VAFE3BX_Object_t *pObj, uint8_t *ZLow);
int32_t ST1VAFE3BX_ACC_Get_6D_Orientation_ZH(ST1VAFE3BX_Object_t *pObj, uint8_t *ZHigh);

int32_t ST1VAFE3BX_Set_Mem_Bank(ST1VAFE3BX_Object_t *pObj, uint8_t Val);
int32_t ST1VAFE3BX_BIO_GetVafe(ST1VAFE3BX_Object_t *pObj, int16_t  *vafe);


/**
  * @}
  */

/** @addtogroup ST1VAFE3BX_Exported_Variables ST1VAFE3BX Exported Variables
  * @{
  */

extern ST1VAFE3BX_CommonDrv_t ST1VAFE3BX_COMMON_Driver;
extern ST1VAFE3BX_ACC_Drv_t ST1VAFE3BX_ACC_Driver;

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
