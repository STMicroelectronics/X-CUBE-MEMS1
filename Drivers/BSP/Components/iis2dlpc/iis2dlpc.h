/**
  ******************************************************************************
  * @file    iis2dlpc.h
  * @author  MEMS Software Solutions Team
  * @brief   IIS2DLPC header driver file
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
#ifndef IIS2DLPC_H
#define IIS2DLPC_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "iis2dlpc_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup IIS2DLPC IIS2DLPC
  * @{
  */

/** @defgroup IIS2DLPC_Exported_Types IIS2DLPC Exported Types
  * @{
  */

typedef int32_t (*IIS2DLPC_Init_Func)(void);
typedef int32_t (*IIS2DLPC_DeInit_Func)(void);
typedef int32_t (*IIS2DLPC_GetTick_Func)(void);
typedef void    (*IIS2DLPC_Delay_Func)(uint32_t);
typedef int32_t (*IIS2DLPC_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*IIS2DLPC_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  IIS2DLPC_INT1_PIN,
  IIS2DLPC_INT2_PIN,
} IIS2DLPC_SensorIntPin_t;

typedef struct
{
  IIS2DLPC_Init_Func         Init;
  IIS2DLPC_DeInit_Func       DeInit;
  uint32_t                   BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
  uint8_t                    Address;
  IIS2DLPC_WriteReg_Func     WriteReg;
  IIS2DLPC_ReadReg_Func      ReadReg;
  IIS2DLPC_GetTick_Func      GetTick;
  IIS2DLPC_Delay_Func        Delay;
} IIS2DLPC_IO_t;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} IIS2DLPC_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} IIS2DLPC_Axes_t;

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
} IIS2DLPC_Event_Status_t;

typedef enum
{
  IIS2DLPC_HIGH_PERFORMANCE_MODE,
  IIS2DLPC_LOW_POWER_MODE4,
  IIS2DLPC_LOW_POWER_MODE3,
  IIS2DLPC_LOW_POWER_MODE2,
  IIS2DLPC_LOW_POWER_MODE1
} IIS2DLPC_Operating_Mode_t;

typedef enum
{
  IIS2DLPC_LOW_NOISE_DISABLE,
  IIS2DLPC_LOW_NOISE_ENABLE
} IIS2DLPC_Low_Noise_t;

typedef struct
{
  IIS2DLPC_IO_t             IO;
  stmdev_ctx_t              Ctx;
  uint8_t                   is_initialized;
  uint8_t                   acc_is_enabled;
  float                     acc_odr;
  IIS2DLPC_Operating_Mode_t acc_operating_mode;
  IIS2DLPC_Low_Noise_t      acc_low_noise;
} IIS2DLPC_Object_t;

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
} IIS2DLPC_Capabilities_t;

typedef struct
{
  int32_t (*Init)(IIS2DLPC_Object_t *);
  int32_t (*DeInit)(IIS2DLPC_Object_t *);
  int32_t (*ReadID)(IIS2DLPC_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(IIS2DLPC_Object_t *, IIS2DLPC_Capabilities_t *);
} IIS2DLPC_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(IIS2DLPC_Object_t *);
  int32_t (*Disable)(IIS2DLPC_Object_t *);
  int32_t (*GetSensitivity)(IIS2DLPC_Object_t *, float *);
  int32_t (*GetOutputDataRate)(IIS2DLPC_Object_t *, float *);
  int32_t (*SetOutputDataRate)(IIS2DLPC_Object_t *, float);
  int32_t (*GetFullScale)(IIS2DLPC_Object_t *, int32_t *);
  int32_t (*SetFullScale)(IIS2DLPC_Object_t *, int32_t);
  int32_t (*GetAxes)(IIS2DLPC_Object_t *, IIS2DLPC_Axes_t *);
  int32_t (*GetAxesRaw)(IIS2DLPC_Object_t *, IIS2DLPC_AxesRaw_t *);
} IIS2DLPC_ACC_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} iis2dlpc_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} iis2dlpc_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} iis2dlpc_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} iis2dlpc_axis1bit32_t;

/**
  * @}
  */

/** @defgroup IIS2DLPC_Exported_Constants IIS2DLPC Exported Constants
  * @{
  */

#define IIS2DLPC_OK                       0
#define IIS2DLPC_ERROR                   -1

#define IIS2DLPC_I2C_BUS                 0U
#define IIS2DLPC_SPI_4WIRES_BUS          1U
#define IIS2DLPC_SPI_3WIRES_BUS          2U

#define IIS2DLPC_ACC_SENSITIVITY_FOR_FS_2G_LOPOW1_MODE   0.976f  /**< Sensitivity value for 2g full scale, Low-power1 mode [mg/LSB] */
#define IIS2DLPC_ACC_SENSITIVITY_FOR_FS_2G_OTHER_MODES   0.244f  /**< Sensitivity value for 2g full scale, all other modes except Low-power1 [mg/LSB] */

#define IIS2DLPC_ACC_SENSITIVITY_FOR_FS_4G_LOPOW1_MODE   1.952f  /**< Sensitivity value for 4g full scale, Low-power1 mode [mg/LSB] */
#define IIS2DLPC_ACC_SENSITIVITY_FOR_FS_4G_OTHER_MODES   0.488f  /**< Sensitivity value for 4g full scale, all other modes except Low-power1 [mg/LSB] */

#define IIS2DLPC_ACC_SENSITIVITY_FOR_FS_8G_LOPOW1_MODE   3.904f  /**< Sensitivity value for 8g full scale, Low-power1 mode [mg/LSB] */
#define IIS2DLPC_ACC_SENSITIVITY_FOR_FS_8G_OTHER_MODES   0.976f  /**< Sensitivity value for 8g full scale, all other modes except Low-power1 [mg/LSB] */

#define IIS2DLPC_ACC_SENSITIVITY_FOR_FS_16G_LOPOW1_MODE  7.808f  /**< Sensitivity value for 16g full scale, Low-power1 mode [mg/LSB] */
#define IIS2DLPC_ACC_SENSITIVITY_FOR_FS_16G_OTHER_MODES  1.952f  /**< Sensitivity value for 16g full scale, all other modes except Low-power1 [mg/LSB] */

/**
  * @}
  */

/** @addtogroup IIS2DLPC_Exported_Functions IIS2DLPC Exported Functions
  * @{
  */

int32_t IIS2DLPC_RegisterBusIO(IIS2DLPC_Object_t *pObj, IIS2DLPC_IO_t *pIO);
int32_t IIS2DLPC_Init(IIS2DLPC_Object_t *pObj);
int32_t IIS2DLPC_DeInit(IIS2DLPC_Object_t *pObj);
int32_t IIS2DLPC_ReadID(IIS2DLPC_Object_t *pObj, uint8_t *Id);
int32_t IIS2DLPC_GetCapabilities(IIS2DLPC_Object_t *pObj, IIS2DLPC_Capabilities_t *Capabilities);

int32_t IIS2DLPC_ACC_Enable(IIS2DLPC_Object_t *pObj);
int32_t IIS2DLPC_ACC_Disable(IIS2DLPC_Object_t *pObj);
int32_t IIS2DLPC_ACC_GetSensitivity(IIS2DLPC_Object_t *pObj, float *Sensitivity);
int32_t IIS2DLPC_ACC_GetOutputDataRate(IIS2DLPC_Object_t *pObj, float *Odr);
int32_t IIS2DLPC_ACC_SetOutputDataRate(IIS2DLPC_Object_t *pObj, float Odr);
int32_t IIS2DLPC_ACC_SetOutputDataRate_With_Mode(IIS2DLPC_Object_t *pObj, float Odr, IIS2DLPC_Operating_Mode_t Mode,
                                                 IIS2DLPC_Low_Noise_t Noise);
int32_t IIS2DLPC_ACC_GetFullScale(IIS2DLPC_Object_t *pObj, int32_t *FullScale);
int32_t IIS2DLPC_ACC_SetFullScale(IIS2DLPC_Object_t *pObj, int32_t FullScale);
int32_t IIS2DLPC_ACC_GetAxesRaw(IIS2DLPC_Object_t *pObj, IIS2DLPC_AxesRaw_t *Value);
int32_t IIS2DLPC_ACC_GetAxes(IIS2DLPC_Object_t *pObj, IIS2DLPC_Axes_t *Acceleration);

int32_t IIS2DLPC_Read_Reg(IIS2DLPC_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t IIS2DLPC_Write_Reg(IIS2DLPC_Object_t *pObj, uint8_t reg, uint8_t Data);

int32_t IIS2DLPC_ACC_Get_DRDY_Status(IIS2DLPC_Object_t *pObj, uint8_t *Status);

/**
  * @}
  */

/** @addtogroup IIS2DLPC_Exported_Variables IIS2DLPC Exported Variables
  * @{
  */

extern IIS2DLPC_CommonDrv_t IIS2DLPC_COMMON_Driver;
extern IIS2DLPC_ACC_Drv_t IIS2DLPC_ACC_Driver;

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
