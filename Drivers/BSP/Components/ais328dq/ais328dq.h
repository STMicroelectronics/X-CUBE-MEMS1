/**
  ******************************************************************************
  * @file    ais328dq.h
  * @author  MEMS Software Solutions Team
  * @brief   AIS328DQ header driver file
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
#ifndef AIS328DQ_H
#define AIS328DQ_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "ais328dq_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup AIS328DQ AIS328DQ
  * @{
  */

/** @defgroup AIS328DQ_Exported_Types AIS328DQ Exported Types
  * @{
  */

typedef int32_t (*AIS328DQ_Init_Func)(void);
typedef int32_t (*AIS328DQ_DeInit_Func)(void);
typedef int32_t (*AIS328DQ_GetTick_Func)(void);
typedef int32_t (*AIS328DQ_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*AIS328DQ_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  AIS328DQ_INT1_PIN,
  AIS328DQ_INT2_PIN,
} AIS328DQ_SensorIntPin_t;

typedef struct
{
  AIS328DQ_Init_Func        Init;
  AIS328DQ_DeInit_Func      DeInit;
  uint32_t                  BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
  uint8_t                   Address;
  AIS328DQ_WriteReg_Func    WriteReg;
  AIS328DQ_ReadReg_Func     ReadReg;
  AIS328DQ_GetTick_Func     GetTick;
} AIS328DQ_IO_t;

typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} AIS328DQ_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} AIS328DQ_Axes_t;

typedef struct
{
  AIS328DQ_IO_t      IO;
  stmdev_ctx_t       Ctx;
  uint8_t            is_initialized;
  uint8_t            acc_is_enabled;
  ais328dq_dr_t      acc_odr;
} AIS328DQ_Object_t;

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
} AIS328DQ_Capabilities_t;

typedef struct
{
  int32_t (*Init)(AIS328DQ_Object_t *);
  int32_t (*DeInit)(AIS328DQ_Object_t *);
  int32_t (*ReadID)(AIS328DQ_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(AIS328DQ_Object_t *, AIS328DQ_Capabilities_t *);
} AIS328DQ_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(AIS328DQ_Object_t *);
  int32_t (*Disable)(AIS328DQ_Object_t *);
  int32_t (*GetSensitivity)(AIS328DQ_Object_t *, float *);
  int32_t (*GetOutputDataRate)(AIS328DQ_Object_t *, float *);
  int32_t (*SetOutputDataRate)(AIS328DQ_Object_t *, float);
  int32_t (*GetFullScale)(AIS328DQ_Object_t *, int32_t *);
  int32_t (*SetFullScale)(AIS328DQ_Object_t *, int32_t);
  int32_t (*GetAxes)(AIS328DQ_Object_t *, AIS328DQ_Axes_t *);
  int32_t (*GetAxesRaw)(AIS328DQ_Object_t *, AIS328DQ_AxesRaw_t *);
} AIS328DQ_ACC_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} ais328dq_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} ais328dq_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} ais328dq_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} ais328dq_axis1bit32_t;

/**
  * @}
  */

/** @defgroup AIS328DQ_Exported_Constants AIS328DQ Exported Constants
  * @{
  */

#define AIS328DQ_OK                       0
#define AIS328DQ_ERROR                   -1

#define AIS328DQ_I2C_BUS                 0U
#define AIS328DQ_SPI_4WIRES_BUS          1U
#define AIS328DQ_SPI_3WIRES_BUS          2U

#define AIS328DQ_ACC_SENSITIVITY_FOR_FS_2G   0.98f  /**< Sensitivity value for 2g full scale [mg/LSB] */
#define AIS328DQ_ACC_SENSITIVITY_FOR_FS_4G   1.95f  /**< Sensitivity value for 4g full scale [mg/LSB] */
#define AIS328DQ_ACC_SENSITIVITY_FOR_FS_8G   3.91f  /**< Sensitivity value for 8g full scale [mg/LSB] */

/**
  * @}
  */

/** @addtogroup AIS328DQ_Exported_Functions AIS328DQ Exported Functions
  * @{
  */

int32_t AIS328DQ_RegisterBusIO(AIS328DQ_Object_t *pObj, AIS328DQ_IO_t *pIO);
int32_t AIS328DQ_Init(AIS328DQ_Object_t *pObj);
int32_t AIS328DQ_DeInit(AIS328DQ_Object_t *pObj);
int32_t AIS328DQ_ReadID(AIS328DQ_Object_t *pObj, uint8_t *Id);
int32_t AIS328DQ_GetCapabilities(AIS328DQ_Object_t *pObj, AIS328DQ_Capabilities_t *Capabilities);

int32_t AIS328DQ_ACC_Enable(AIS328DQ_Object_t *pObj);
int32_t AIS328DQ_ACC_Disable(AIS328DQ_Object_t *pObj);
int32_t AIS328DQ_ACC_GetSensitivity(AIS328DQ_Object_t *pObj, float *Sensitivity);
int32_t AIS328DQ_ACC_GetOutputDataRate(AIS328DQ_Object_t *pObj, float *Odr);
int32_t AIS328DQ_ACC_SetOutputDataRate(AIS328DQ_Object_t *pObj, float Odr);
int32_t AIS328DQ_ACC_GetFullScale(AIS328DQ_Object_t *pObj, int32_t *FullScale);
int32_t AIS328DQ_ACC_SetFullScale(AIS328DQ_Object_t *pObj, int32_t FullScale);
int32_t AIS328DQ_ACC_GetAxesRaw(AIS328DQ_Object_t *pObj, AIS328DQ_AxesRaw_t *Value);
int32_t AIS328DQ_ACC_GetAxes(AIS328DQ_Object_t *pObj, AIS328DQ_Axes_t *Acceleration);

int32_t AIS328DQ_Read_Reg(AIS328DQ_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t AIS328DQ_Write_Reg(AIS328DQ_Object_t *pObj, uint8_t reg, uint8_t Data);

int32_t AIS328DQ_ACC_Get_DRDY_Status(AIS328DQ_Object_t *pObj, uint8_t *Status);

/**
  * @}
  */

/** @addtogroup AIS328DQ_Exported_Variables AIS328DQ Exported Variables
  * @{
  */

extern AIS328DQ_CommonDrv_t AIS328DQ_COMMON_Driver;
extern AIS328DQ_ACC_Drv_t AIS328DQ_ACC_Driver;

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
