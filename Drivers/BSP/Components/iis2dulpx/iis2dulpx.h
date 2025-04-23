/**
  ******************************************************************************
  * @file    iis2dulpx.h
  * @author  MEMS Software Solutions Team
  * @brief   IIS2DULPX header driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IIS2DULPX_H
#define IIS2DULPX_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "iis2dulpx_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup IIS2DULPX IIS2DULPX
  * @{
  */

/** @defgroup IIS2DULPX_Exported_Types IIS2DULPX Exported Types
  * @{
  */

typedef int32_t (*IIS2DULPX_Init_Func)(void);
typedef int32_t (*IIS2DULPX_DeInit_Func)(void);
typedef int32_t (*IIS2DULPX_GetTick_Func)(void);
typedef void (*IIS2DULPX_Delay_Func)(uint32_t);
typedef int32_t (*IIS2DULPX_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*IIS2DULPX_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  IIS2DULPX_INT1_PIN,
  IIS2DULPX_INT2_PIN,
} IIS2DULPX_SensorIntPin_t;

typedef struct
{
  IIS2DULPX_Init_Func       Init;
  IIS2DULPX_DeInit_Func     DeInit;
  uint32_t                   BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires, 3 means I3C */
  uint8_t                    Address;
  IIS2DULPX_WriteReg_Func   WriteReg;
  IIS2DULPX_ReadReg_Func    ReadReg;
  IIS2DULPX_GetTick_Func    GetTick;
  IIS2DULPX_Delay_Func      Delay;
} IIS2DULPX_IO_t;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} IIS2DULPX_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} IIS2DULPX_Axes_t;

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
} IIS2DULPX_Event_Status_t;

typedef enum
{
  IIS2DULPX_ULTRA_LOW_POWER,
  IIS2DULPX_LOW_POWER,
  IIS2DULPX_HIGH_PERFORMANCE,
} IIS2DULPX_Power_Mode_t;

typedef struct
{
  IIS2DULPX_IO_t         IO;
  stmdev_ctx_t            Ctx;
  uint8_t                 is_initialized;
  uint8_t                 acc_is_enabled;
  float                   acc_odr;
  IIS2DULPX_Power_Mode_t power_mode;
} IIS2DULPX_Object_t;

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
} IIS2DULPX_Capabilities_t;

typedef struct
{
  int32_t (*Init)(IIS2DULPX_Object_t *);
  int32_t (*DeInit)(IIS2DULPX_Object_t *);
  int32_t (*ReadID)(IIS2DULPX_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(IIS2DULPX_Object_t *, IIS2DULPX_Capabilities_t *);
} IIS2DULPX_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(IIS2DULPX_Object_t *);
  int32_t (*Disable)(IIS2DULPX_Object_t *);
  int32_t (*GetSensitivity)(IIS2DULPX_Object_t *, float *);
  int32_t (*GetOutputDataRate)(IIS2DULPX_Object_t *, float *);
  int32_t (*SetOutputDataRate)(IIS2DULPX_Object_t *, float);
  int32_t (*GetFullScale)(IIS2DULPX_Object_t *, int32_t *);
  int32_t (*SetFullScale)(IIS2DULPX_Object_t *, int32_t);
  int32_t (*GetAxes)(IIS2DULPX_Object_t *, IIS2DULPX_Axes_t *);
  int32_t (*GetAxesRaw)(IIS2DULPX_Object_t *, IIS2DULPX_AxesRaw_t *);
} IIS2DULPX_ACC_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} iis2dulpx_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} iis2dulpx_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} iis2dulpx_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} iis2dulpx_axis1bit32_t;

/**
  * @}
  */

/** @defgroup IIS2DULPX_Exported_Constants IIS2DULPX Exported Constants
  * @{
  */

#define IIS2DULPX_OK                       0
#define IIS2DULPX_ERROR                   -1

#define IIS2DULPX_I2C_BUS                 0U
#define IIS2DULPX_SPI_4WIRES_BUS          1U
#define IIS2DULPX_SPI_3WIRES_BUS          2U
#define IIS2DULPX_I3C_BUS                 3U

#define IIS2DULPX_ACC_SENSITIVITY_FOR_FS_2G   0.061f  /**< Sensitivity value for 2g full scale, Low-power1 mode [mg/LSB] */
#define IIS2DULPX_ACC_SENSITIVITY_FOR_FS_4G   0.122f  /**< Sensitivity value for 4g full scale, Low-power1 mode [mg/LSB] */
#define IIS2DULPX_ACC_SENSITIVITY_FOR_FS_8G   0.244f  /**< Sensitivity value for 8g full scale, Low-power1 mode [mg/LSB] */
#define IIS2DULPX_ACC_SENSITIVITY_FOR_FS_16G  0.488f  /**< Sensitivity value for 16g full scale, Low-power1 mode [mg/LSB] */

#define IIS2DULPX_QVAR_GAIN  74.4f  /**< Gain value for bits AH_QVAR_GAIN_[1:0] == 1 in AH_QVAR_CFG register, 16 bits [LSB/mV] */

#define IIS2DULPX_QVAR_GAIN_MULTIPL_0_5X  0.5f  /**< Gain multiplier for bits AH_QVAR_GAIN_[1:0] == 0 in AH_QVAR_CFG register [-] */
#define IIS2DULPX_QVAR_GAIN_MULTIPL_1X    1.0f  /**< Gain multiplier for bits AH_QVAR_GAIN_[1:0] == 1 in AH_QVAR_CFG register [-] */
#define IIS2DULPX_QVAR_GAIN_MULTIPL_2X    2.0f  /**< Gain multiplier for bits AH_QVAR_GAIN_[1:0] == 2 in AH_QVAR_CFG register [-] */
#define IIS2DULPX_QVAR_GAIN_MULTIPL_4X    4.0f  /**< Gain multiplier for bits AH_QVAR_GAIN_[1:0] == 3 in AH_QVAR_CFG register [-] */

/**
  * @}
  */

/** @addtogroup IIS2DULPX_Exported_Functions IIS2DULPX Exported Functions
  * @{
  */

int32_t IIS2DULPX_RegisterBusIO(IIS2DULPX_Object_t *pObj, IIS2DULPX_IO_t *pIO);
int32_t IIS2DULPX_Init(IIS2DULPX_Object_t *pObj);
int32_t IIS2DULPX_DeInit(IIS2DULPX_Object_t *pObj);
int32_t IIS2DULPX_ExitDeepPowerDownI2C(IIS2DULPX_Object_t *pObj);
int32_t IIS2DULPX_ExitDeepPowerDownSPI(IIS2DULPX_Object_t *pObj);
int32_t IIS2DULPX_ReadID(IIS2DULPX_Object_t *pObj, uint8_t *Id);
int32_t IIS2DULPX_GetCapabilities(IIS2DULPX_Object_t *pObj, IIS2DULPX_Capabilities_t *Capabilities);

int32_t IIS2DULPX_ACC_Enable(IIS2DULPX_Object_t *pObj);
int32_t IIS2DULPX_ACC_Disable(IIS2DULPX_Object_t *pObj);
int32_t IIS2DULPX_ACC_GetSensitivity(IIS2DULPX_Object_t *pObj, float *Sensitivity);
int32_t IIS2DULPX_ACC_GetOutputDataRate(IIS2DULPX_Object_t *pObj, float *Odr);
int32_t IIS2DULPX_ACC_SetOutputDataRate(IIS2DULPX_Object_t *pObj, float Odr);
int32_t IIS2DULPX_ACC_SetOutputDataRate_With_Mode(IIS2DULPX_Object_t *pObj, float Odr, IIS2DULPX_Power_Mode_t Power);
int32_t IIS2DULPX_ACC_GetFullScale(IIS2DULPX_Object_t *pObj, int32_t *FullScale);
int32_t IIS2DULPX_ACC_SetFullScale(IIS2DULPX_Object_t *pObj, int32_t FullScale);
int32_t IIS2DULPX_ACC_GetAxesRaw(IIS2DULPX_Object_t *pObj, IIS2DULPX_AxesRaw_t *Value);
int32_t IIS2DULPX_ACC_GetAxes(IIS2DULPX_Object_t *pObj, IIS2DULPX_Axes_t *Acceleration);

int32_t IIS2DULPX_Read_Reg(IIS2DULPX_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t IIS2DULPX_Write_Reg(IIS2DULPX_Object_t *pObj, uint8_t reg, uint8_t Data);
int32_t IIS2DULPX_Set_Interrupt_Latch(IIS2DULPX_Object_t *pObj, uint8_t Status);

int32_t IIS2DULPX_ACC_Enable_DRDY_Interrupt(IIS2DULPX_Object_t *pObj);
int32_t IIS2DULPX_ACC_Disable_DRDY_Interrupt(IIS2DULPX_Object_t *pObj);

int32_t IIS2DULPX_ACC_Set_SelfTest(IIS2DULPX_Object_t *pObj, uint8_t Val);
int32_t IIS2DULPX_ACC_Get_DRDY_Status(IIS2DULPX_Object_t *pObj, uint8_t *Status);
int32_t IIS2DULPX_ACC_Get_Init_Status(IIS2DULPX_Object_t *pObj, uint8_t *Status);

int32_t IIS2DULPX_ACC_Get_Event_Status(IIS2DULPX_Object_t *pObj, IIS2DULPX_Event_Status_t *Status);

int32_t IIS2DULPX_ACC_Enable_Wake_Up_Detection(IIS2DULPX_Object_t *pObj, IIS2DULPX_SensorIntPin_t IntPin);
int32_t IIS2DULPX_ACC_Disable_Wake_Up_Detection(IIS2DULPX_Object_t *pObj);
int32_t IIS2DULPX_ACC_Set_Wake_Up_Threshold(IIS2DULPX_Object_t *pObj, uint32_t Threshold);
int32_t IIS2DULPX_ACC_Set_Wake_Up_Duration(IIS2DULPX_Object_t *pObj, uint8_t Duration);

int32_t IIS2DULPX_ACC_Enable_6D_Orientation(IIS2DULPX_Object_t *pObj, IIS2DULPX_SensorIntPin_t IntPin);
int32_t IIS2DULPX_ACC_Disable_6D_Orientation(IIS2DULPX_Object_t *pObj);
int32_t IIS2DULPX_ACC_Set_6D_Orientation_Threshold(IIS2DULPX_Object_t *pObj, uint8_t Threshold);
int32_t IIS2DULPX_ACC_Get_6D_Orientation_XL(IIS2DULPX_Object_t *pObj, uint8_t *XLow);
int32_t IIS2DULPX_ACC_Get_6D_Orientation_XH(IIS2DULPX_Object_t *pObj, uint8_t *XHigh);
int32_t IIS2DULPX_ACC_Get_6D_Orientation_YL(IIS2DULPX_Object_t *pObj, uint8_t *YLow);
int32_t IIS2DULPX_ACC_Get_6D_Orientation_YH(IIS2DULPX_Object_t *pObj, uint8_t *YHigh);
int32_t IIS2DULPX_ACC_Get_6D_Orientation_ZL(IIS2DULPX_Object_t *pObj, uint8_t *ZLow);
int32_t IIS2DULPX_ACC_Get_6D_Orientation_ZH(IIS2DULPX_Object_t *pObj, uint8_t *ZHigh);

int32_t IIS2DULPX_Set_Mem_Bank(IIS2DULPX_Object_t *pObj, uint8_t Val);

/**
  * @}
  */

/** @addtogroup IIS2DULPX_Exported_Variables IIS2DULPX Exported Variables
  * @{
  */

extern IIS2DULPX_CommonDrv_t IIS2DULPX_COMMON_Driver;
extern IIS2DULPX_ACC_Drv_t IIS2DULPX_ACC_Driver;

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
