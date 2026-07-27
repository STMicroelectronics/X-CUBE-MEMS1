/**
  ******************************************************************************
  * @file    iis3dwb10is.h
  * @author  MEMS Software Solutions Team
  * @brief   IIS3DWB10IS header driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IIS3DWB10IS_H
#define IIS3DWB10IS_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "iis3dwb10is_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @addtogroup IIS3DWB10IS IIS3DWB10IS
 * @{
 */

/** @defgroup IIS3DWB10IS_Exported_Types IIS3DWB10IS Exported Types
 * @{
 */

typedef int32_t (*IIS3DWB10IS_Init_Func)(void);
typedef int32_t (*IIS3DWB10IS_DeInit_Func)(void);
typedef int32_t (*IIS3DWB10IS_GetTick_Func)(void);
typedef void    (*IIS3DWB10IS_Delay_Func)(uint32_t);
typedef int32_t (*IIS3DWB10IS_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*IIS3DWB10IS_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  IIS3DWB10IS_INT1_PIN,
  IIS3DWB10IS_INT2_PIN,
} IIS3DWB10IS_SensorIntPin_t;

typedef struct
{
  IIS3DWB10IS_Init_Func      Init;
  IIS3DWB10IS_DeInit_Func    DeInit;
  uint32_t                   BusType; /* 0 means SPI 4-Wires, 1 means SPI-3-Wires, 2 means I3C */
  uint8_t                    Address;
  IIS3DWB10IS_WriteReg_Func  WriteReg;
  IIS3DWB10IS_ReadReg_Func   ReadReg;
  IIS3DWB10IS_GetTick_Func   GetTick;
  IIS3DWB10IS_Delay_Func     Delay;
} IIS3DWB10IS_IO_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} IIS3DWB10IS_AxesRaw_t;

typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} IIS3DWB10IS_AxesRaw16_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} IIS3DWB10IS_Axes_t;

typedef struct
{
  IIS3DWB10IS_IO_t        IO;
  stmdev_ctx_t            Ctx;
  uint8_t                 is_initialized;
  uint8_t                 acc_is_enabled;
  iis3dwb10is_data_rate_t acc_data_rate;
} IIS3DWB10IS_Object_t;

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
} IIS3DWB10IS_Capabilities_t;

typedef struct
{
  int32_t (*Init)(IIS3DWB10IS_Object_t *);
  int32_t (*DeInit)(IIS3DWB10IS_Object_t *);
  int32_t (*ReadID)(IIS3DWB10IS_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(IIS3DWB10IS_Object_t *, IIS3DWB10IS_Capabilities_t *);
} IIS3DWB10IS_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(IIS3DWB10IS_Object_t *);
  int32_t (*Disable)(IIS3DWB10IS_Object_t *);
  int32_t (*GetSensitivity)(IIS3DWB10IS_Object_t *, float *);
  int32_t (*GetOutputDataRate)(IIS3DWB10IS_Object_t *, float *);
  int32_t (*SetOutputDataRate)(IIS3DWB10IS_Object_t *, float);
  int32_t (*GetFullScale)(IIS3DWB10IS_Object_t *, int32_t *);
  int32_t (*SetFullScale)(IIS3DWB10IS_Object_t *, int32_t);
  int32_t (*GetAxes)(IIS3DWB10IS_Object_t *, IIS3DWB10IS_Axes_t *);
  int32_t (*GetAxesRaw)(IIS3DWB10IS_Object_t *, IIS3DWB10IS_AxesRaw16_t *);
} IIS3DWB10IS_ACC_Drv_t;

typedef union{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} iis3dwb10is_axis3bit16_t;

typedef union{
  int16_t i16bit;
  uint8_t u8bit[2];
} iis3dwb10is_axis1bit16_t;

typedef union{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} iis3dwb10is_axis3bit32_t;

typedef union{
  int32_t i32bit;
  uint8_t u8bit[4];
} iis3dwb10is_axis1bit32_t;

/**
 * @}
 */

/** @defgroup IIS3DWB10IS_Exported_Constants IIS3DWB10IS Exported Constants
 * @{
 */

#define IIS3DWB10IS_OK                       0
#define IIS3DWB10IS_ERROR                   -1

#define IIS3DWB10IS_SPI_4WIRES_BUS          0U
#define IIS3DWB10IS_SPI_3WIRES_BUS          1U
#define IIS3DWB10IS_I3C_BUS                 2U

#define IIS3DWB10IS_ACC_SENSITIVITY_FOR_FS_50G_20_BIT_FORMAT   0.095f  /**< Sensitivity value for  50g full scale, 20-bit format [mg/LSB] */
#define IIS3DWB10IS_ACC_SENSITIVITY_FOR_FS_100G_20_BIT_FORMAT  0.191f  /**< Sensitivity value for 100g full scale, 20-bit format [mg/LSB] */
#define IIS3DWB10IS_ACC_SENSITIVITY_FOR_FS_200G_20_BIT_FORMAT  0.381f  /**< Sensitivity value for 200g full scale, 20-bit format [mg/LSB] */

#define IIS3DWB10IS_ACC_SENSITIVITY_FOR_FS_50G_16_BIT_FORMAT   1.526f  /**< Sensitivity value for  50g full scale, 16-bit format [mg/LSB] */
#define IIS3DWB10IS_ACC_SENSITIVITY_FOR_FS_100G_16_BIT_FORMAT  3.052f  /**< Sensitivity value for 100g full scale, 16-bit format [mg/LSB] */
#define IIS3DWB10IS_ACC_SENSITIVITY_FOR_FS_200G_16_BIT_FORMAT  6.104f  /**< Sensitivity value for 200g full scale, 16-bit format [mg/LSB] */

/**
 * @}
 */

/** @addtogroup IIS3DWB10IS_Exported_Functions IIS3DWB10IS Exported Functions
 * @{
 */

int32_t IIS3DWB10IS_RegisterBusIO(IIS3DWB10IS_Object_t *pObj, IIS3DWB10IS_IO_t *pIO);
int32_t IIS3DWB10IS_Init(IIS3DWB10IS_Object_t *pObj);
int32_t IIS3DWB10IS_DeInit(IIS3DWB10IS_Object_t *pObj);
int32_t IIS3DWB10IS_ReadID(IIS3DWB10IS_Object_t *pObj, uint8_t *Id);
int32_t IIS3DWB10IS_GetCapabilities(IIS3DWB10IS_Object_t *pObj, IIS3DWB10IS_Capabilities_t *Capabilities);

int32_t IIS3DWB10IS_ACC_Enable(IIS3DWB10IS_Object_t *pObj);
int32_t IIS3DWB10IS_ACC_Disable(IIS3DWB10IS_Object_t *pObj);
int32_t IIS3DWB10IS_ACC_GetSensitivity(IIS3DWB10IS_Object_t *pObj, float *Sensitivity);
int32_t IIS3DWB10IS_ACC_GetOutputDataRate(IIS3DWB10IS_Object_t *pObj, float *Odr);
int32_t IIS3DWB10IS_ACC_SetOutputDataRate(IIS3DWB10IS_Object_t *pObj, float Odr);
int32_t IIS3DWB10IS_ACC_GetFullScale(IIS3DWB10IS_Object_t *pObj, int32_t *FullScale);
int32_t IIS3DWB10IS_ACC_SetFullScale(IIS3DWB10IS_Object_t *pObj, int32_t FullScale);
int32_t IIS3DWB10IS_ACC_GetAxesRaw(IIS3DWB10IS_Object_t *pObj, IIS3DWB10IS_AxesRaw16_t *Value);
int32_t IIS3DWB10IS_ACC_GetAxes(IIS3DWB10IS_Object_t *pObj, IIS3DWB10IS_Axes_t *Acceleration);

int32_t IIS3DWB10IS_Read_Reg(IIS3DWB10IS_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t IIS3DWB10IS_Write_Reg(IIS3DWB10IS_Object_t *pObj, uint8_t reg, uint8_t Data);
int32_t IIS3DWB10IS_ACC_Get_DRDY_Status(IIS3DWB10IS_Object_t *pObj, uint8_t *Status);

int32_t IIS3DWB10IS_Boot(void *Obj, uint32_t Timeout);
int32_t IIS3DWB10IS_SW_Reset(void *Obj, uint32_t Timeout);

/**
 * @}
 */

/** @addtogroup IIS3DWB10IS_Exported_Variables IIS3DWB10IS Exported Variables
 * @{
 */

extern IIS3DWB10IS_CommonDrv_t IIS3DWB10IS_COMMON_Driver;
extern IIS3DWB10IS_ACC_Drv_t IIS3DWB10IS_ACC_Driver;

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
