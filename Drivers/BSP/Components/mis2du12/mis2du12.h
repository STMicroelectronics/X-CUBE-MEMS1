/**
  ******************************************************************************
  * @file    mis2du12.h
  * @author  MEMS Software Solutions Team
  * @brief   MIS2DU12 header driver file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.</center></h2>
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
#ifndef MIS2DU12_H
#define MIS2DU12_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "mis2du12_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup MIS2DU12 MIS2DU12
  * @{
  */

/** @defgroup MIS2DU12_Exported_Types MIS2DU12 Exported Types
  * @{
  */

typedef int32_t (*MIS2DU12_Init_Func)(void);
typedef int32_t (*MIS2DU12_DeInit_Func)(void);
typedef int32_t (*MIS2DU12_GetTick_Func)(void);
typedef void    (*MIS2DU12_Delay_Func)(uint32_t);
typedef int32_t (*MIS2DU12_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*MIS2DU12_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  MIS2DU12_INT1_PIN,
  MIS2DU12_INT2_PIN,
} MIS2DU12_SensorIntPin_t;

typedef struct
{
  MIS2DU12_Init_Func         Init;
  MIS2DU12_DeInit_Func       DeInit;
  uint32_t                   BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires, 3 means I3C */
  uint8_t                    Address;
  MIS2DU12_WriteReg_Func     WriteReg;
  MIS2DU12_ReadReg_Func      ReadReg;
  MIS2DU12_GetTick_Func      GetTick;
  MIS2DU12_Delay_Func        Delay;
} MIS2DU12_IO_t;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} MIS2DU12_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} MIS2DU12_Axes_t;

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
} MIS2DU12_Event_Status_t;

typedef enum
{
  MIS2DU12_ULTRA_LOW_POWER_DISABLE,
  MIS2DU12_ULTRA_LOW_POWER_ENABLE
} MIS2DU12_Ultra_Low_Power_t;

typedef struct
{
  MIS2DU12_IO_t              IO;
  stmdev_ctx_t               Ctx;
  uint8_t                    is_initialized;
  uint8_t                    acc_is_enabled;
  float                      acc_odr;
  MIS2DU12_Ultra_Low_Power_t acc_ultra_low_power;
} MIS2DU12_Object_t;

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
} MIS2DU12_Capabilities_t;

typedef struct
{
  int32_t (*Init)(MIS2DU12_Object_t *);
  int32_t (*DeInit)(MIS2DU12_Object_t *);
  int32_t (*ReadID)(MIS2DU12_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(MIS2DU12_Object_t *, MIS2DU12_Capabilities_t *);
} MIS2DU12_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(MIS2DU12_Object_t *);
  int32_t (*Disable)(MIS2DU12_Object_t *);
  int32_t (*GetSensitivity)(MIS2DU12_Object_t *, float *);
  int32_t (*GetOutputDataRate)(MIS2DU12_Object_t *, float *);
  int32_t (*SetOutputDataRate)(MIS2DU12_Object_t *, float);
  int32_t (*GetFullScale)(MIS2DU12_Object_t *, int32_t *);
  int32_t (*SetFullScale)(MIS2DU12_Object_t *, int32_t);
  int32_t (*GetAxes)(MIS2DU12_Object_t *, MIS2DU12_Axes_t *);
  int32_t (*GetAxesRaw)(MIS2DU12_Object_t *, MIS2DU12_AxesRaw_t *);
} MIS2DU12_ACC_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} mis2du12_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} mis2du12_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} mis2du12_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} mis2du12_axis1bit32_t;

/**
  * @}
  */

/** @defgroup MIS2DU12_Exported_Constants MIS2DU12 Exported Constants
  * @{
  */

#define MIS2DU12_OK                       0
#define MIS2DU12_ERROR                   -1

#define MIS2DU12_I2C_BUS                 0U
#define MIS2DU12_SPI_4WIRES_BUS          1U
#define MIS2DU12_SPI_3WIRES_BUS          2U
#define MIS2DU12_I3C_BUS                 3U

#define MIS2DU12_ACC_SENSITIVITY_FOR_FS_2G   0.976f  /**< Sensitivity value for 2g full scale, Low-power1 mode [mg/LSB] */
#define MIS2DU12_ACC_SENSITIVITY_FOR_FS_4G   1.952f  /**< Sensitivity value for 4g full scale, Low-power1 mode [mg/LSB] */
#define MIS2DU12_ACC_SENSITIVITY_FOR_FS_8G   3.904f  /**< Sensitivity value for 8g full scale, Low-power1 mode [mg/LSB] */
#define MIS2DU12_ACC_SENSITIVITY_FOR_FS_16G  7.808f  /**< Sensitivity value for 16g full scale, Low-power1 mode [mg/LSB] */

/**
  * @}
  */

/** @addtogroup MIS2DU12_Exported_Functions MIS2DU12 Exported Functions
  * @{
  */

int32_t MIS2DU12_RegisterBusIO(MIS2DU12_Object_t *pObj, MIS2DU12_IO_t *pIO);
int32_t MIS2DU12_Init(MIS2DU12_Object_t *pObj);
int32_t MIS2DU12_DeInit(MIS2DU12_Object_t *pObj);
int32_t MIS2DU12_ReadID(MIS2DU12_Object_t *pObj, uint8_t *Id);
int32_t MIS2DU12_GetCapabilities(MIS2DU12_Object_t *pObj, MIS2DU12_Capabilities_t *Capabilities);

int32_t MIS2DU12_ACC_Enable(MIS2DU12_Object_t *pObj);
int32_t MIS2DU12_ACC_Disable(MIS2DU12_Object_t *pObj);
int32_t MIS2DU12_ACC_GetSensitivity(MIS2DU12_Object_t *pObj, float *Sensitivity);
int32_t MIS2DU12_ACC_GetOutputDataRate(MIS2DU12_Object_t *pObj, float *Odr);
int32_t MIS2DU12_ACC_SetOutputDataRate(MIS2DU12_Object_t *pObj, float Odr);
int32_t MIS2DU12_ACC_SetOutputDataRate_With_Mode(MIS2DU12_Object_t *pObj, float Odr, MIS2DU12_Ultra_Low_Power_t Power);
int32_t MIS2DU12_ACC_GetFullScale(MIS2DU12_Object_t *pObj, int32_t *FullScale);
int32_t MIS2DU12_ACC_SetFullScale(MIS2DU12_Object_t *pObj, int32_t FullScale);
int32_t MIS2DU12_ACC_GetAxesRaw(MIS2DU12_Object_t *pObj, MIS2DU12_AxesRaw_t *Value);
int32_t MIS2DU12_ACC_GetAxes(MIS2DU12_Object_t *pObj, MIS2DU12_Axes_t *Acceleration);

int32_t MIS2DU12_Read_Reg(MIS2DU12_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t MIS2DU12_Write_Reg(MIS2DU12_Object_t *pObj, uint8_t reg, uint8_t Data);
int32_t MIS2DU12_Set_Interrupt_Latch(MIS2DU12_Object_t *pObj, uint8_t Status);

int32_t MIS2DU12_ACC_Enable_DRDY_Interrupt(MIS2DU12_Object_t *pObj);
int32_t MIS2DU12_ACC_Disable_DRDY_Interrupt(MIS2DU12_Object_t *pObj);

int32_t MIS2DU12_ACC_Set_SelfTest(MIS2DU12_Object_t *pObj, uint8_t Val);
int32_t MIS2DU12_ACC_Get_DRDY_Status(MIS2DU12_Object_t *pObj, uint8_t *Status);
int32_t MIS2DU12_ACC_Get_Init_Status(MIS2DU12_Object_t *pObj, uint8_t *Status);

int32_t MIS2DU12_ACC_Set_Filter_Mode(MIS2DU12_Object_t *pObj, uint8_t FilterMode);
/**
  * @}
  */

/** @addtogroup MIS2DU12_Exported_Variables MIS2DU12 Exported Variables
  * @{
  */

extern MIS2DU12_CommonDrv_t MIS2DU12_COMMON_Driver;
extern MIS2DU12_ACC_Drv_t MIS2DU12_ACC_Driver;

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
