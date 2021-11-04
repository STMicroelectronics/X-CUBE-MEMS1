/**
 ******************************************************************************
 * @file    h3lis331dl.h
 * @author  MEMS Software Solutions Team
 * @brief   H3LIS331DL header driver file
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
#ifndef H3LIS331DL_H
#define H3LIS331DL_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "h3lis331dl_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @addtogroup H3LIS331DL H3LIS331DL
 * @{
 */

/** @defgroup H3LIS331DL_Exported_Types H3LIS331DL Exported Types
 * @{
 */

typedef int32_t (*H3LIS331DL_Init_Func)(void);
typedef int32_t (*H3LIS331DL_DeInit_Func)(void);
typedef int32_t (*H3LIS331DL_GetTick_Func)(void);
typedef int32_t (*H3LIS331DL_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*H3LIS331DL_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  H3LIS331DL_INT1_PIN,
  H3LIS331DL_INT2_PIN,
} H3LIS331DL_SensorIntPin_t;

typedef struct
{
  H3LIS331DL_Init_Func      Init;
  H3LIS331DL_DeInit_Func    DeInit;
  uint32_t                  BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
  uint8_t                   Address;
  H3LIS331DL_WriteReg_Func  WriteReg;
  H3LIS331DL_ReadReg_Func   ReadReg;
  H3LIS331DL_GetTick_Func   GetTick;
} H3LIS331DL_IO_t;

typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} H3LIS331DL_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} H3LIS331DL_Axes_t;

typedef struct
{
  H3LIS331DL_IO_t    IO;
  stmdev_ctx_t       Ctx;
  uint8_t            is_initialized;
  uint8_t            acc_is_enabled;
  h3lis331dl_dr_t    acc_odr;
} H3LIS331DL_Object_t;

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
} H3LIS331DL_Capabilities_t;

typedef struct
{
  int32_t (*Init)(H3LIS331DL_Object_t *);
  int32_t (*DeInit)(H3LIS331DL_Object_t *);
  int32_t (*ReadID)(H3LIS331DL_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(H3LIS331DL_Object_t *, H3LIS331DL_Capabilities_t *);
} H3LIS331DL_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(H3LIS331DL_Object_t *);
  int32_t (*Disable)(H3LIS331DL_Object_t *);
  int32_t (*GetSensitivity)(H3LIS331DL_Object_t *, float *);
  int32_t (*GetOutputDataRate)(H3LIS331DL_Object_t *, float *);
  int32_t (*SetOutputDataRate)(H3LIS331DL_Object_t *, float);
  int32_t (*GetFullScale)(H3LIS331DL_Object_t *, int32_t *);
  int32_t (*SetFullScale)(H3LIS331DL_Object_t *, int32_t);
  int32_t (*GetAxes)(H3LIS331DL_Object_t *, H3LIS331DL_Axes_t *);
  int32_t (*GetAxesRaw)(H3LIS331DL_Object_t *, H3LIS331DL_AxesRaw_t *);
} H3LIS331DL_ACC_Drv_t;

typedef union{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} h3lis331dl_axis3bit16_t;

typedef union{
  int16_t i16bit;
  uint8_t u8bit[2];
} h3lis331dl_axis1bit16_t;

typedef union{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} h3lis331dl_axis3bit32_t;

typedef union{
  int32_t i32bit;
  uint8_t u8bit[4];
} h3lis331dl_axis1bit32_t;

/**
 * @}
 */

/** @defgroup H3LIS331DL_Exported_Constants H3LIS331DL Exported Constants
 * @{
 */

#define H3LIS331DL_OK                       0
#define H3LIS331DL_ERROR                   -1

#define H3LIS331DL_I2C_BUS                 0U
#define H3LIS331DL_SPI_4WIRES_BUS          1U
#define H3LIS331DL_SPI_3WIRES_BUS          2U

#define H3LIS331DL_ACC_SENSITIVITY_FOR_FS_100G    49.0f  /**< Sensitivity value for 100g full scale [mg/LSB] */
#define H3LIS331DL_ACC_SENSITIVITY_FOR_FS_200G    98.0f  /**< Sensitivity value for 200g full scale [mg/LSB] */
#define H3LIS331DL_ACC_SENSITIVITY_FOR_FS_400G   195.0f  /**< Sensitivity value for 400g full scale [mg/LSB] */

/**
 * @}
 */

/** @addtogroup H3LIS331DL_Exported_Functions H3LIS331DL Exported Functions
 * @{
 */

int32_t H3LIS331DL_RegisterBusIO(H3LIS331DL_Object_t *pObj, H3LIS331DL_IO_t *pIO);
int32_t H3LIS331DL_Init(H3LIS331DL_Object_t *pObj);
int32_t H3LIS331DL_DeInit(H3LIS331DL_Object_t *pObj);
int32_t H3LIS331DL_ReadID(H3LIS331DL_Object_t *pObj, uint8_t *Id);
int32_t H3LIS331DL_GetCapabilities(H3LIS331DL_Object_t *pObj, H3LIS331DL_Capabilities_t *Capabilities);

int32_t H3LIS331DL_ACC_Enable(H3LIS331DL_Object_t *pObj);
int32_t H3LIS331DL_ACC_Disable(H3LIS331DL_Object_t *pObj);
int32_t H3LIS331DL_ACC_GetSensitivity(H3LIS331DL_Object_t *pObj, float *Sensitivity);
int32_t H3LIS331DL_ACC_GetOutputDataRate(H3LIS331DL_Object_t *pObj, float *Odr);
int32_t H3LIS331DL_ACC_SetOutputDataRate(H3LIS331DL_Object_t *pObj, float Odr);
int32_t H3LIS331DL_ACC_GetFullScale(H3LIS331DL_Object_t *pObj, int32_t *FullScale);
int32_t H3LIS331DL_ACC_SetFullScale(H3LIS331DL_Object_t *pObj, int32_t FullScale);
int32_t H3LIS331DL_ACC_GetAxesRaw(H3LIS331DL_Object_t *pObj, H3LIS331DL_AxesRaw_t *Value);
int32_t H3LIS331DL_ACC_GetAxes(H3LIS331DL_Object_t *pObj, H3LIS331DL_Axes_t *Acceleration);

int32_t H3LIS331DL_Read_Reg(H3LIS331DL_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t H3LIS331DL_Write_Reg(H3LIS331DL_Object_t *pObj, uint8_t reg, uint8_t Data);

int32_t H3LIS331DL_ACC_Get_DRDY_Status(H3LIS331DL_Object_t *pObj, uint8_t *Status);

/**
 * @}
 */

/** @addtogroup H3LIS331DL_Exported_Variables H3LIS331DL Exported Variables
 * @{
 */

extern H3LIS331DL_CommonDrv_t H3LIS331DL_COMMON_Driver;
extern H3LIS331DL_ACC_Drv_t H3LIS331DL_ACC_Driver;

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
