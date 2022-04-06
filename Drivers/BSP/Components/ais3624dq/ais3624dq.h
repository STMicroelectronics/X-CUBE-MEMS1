/**
  ******************************************************************************
  * @file    ais3624dq.h
  * @author  MEMS Software Solutions Team
  * @brief   AIS3624DQ header driver file
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
#ifndef AIS3624DQ_H
#define AIS3624DQ_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "ais3624dq_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup AIS3624DQ AIS3624DQ
  * @{
  */

/** @defgroup AIS3624DQ_Exported_Types AIS3624DQ Exported Types
  * @{
  */

typedef int32_t (*AIS3624DQ_Init_Func)(void);
typedef int32_t (*AIS3624DQ_DeInit_Func)(void);
typedef int32_t (*AIS3624DQ_GetTick_Func)(void);
typedef int32_t (*AIS3624DQ_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*AIS3624DQ_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  AIS3624DQ_INT1_PIN,
  AIS3624DQ_INT2_PIN,
} AIS3624DQ_SensorIntPin_t;

typedef struct
{
  AIS3624DQ_Init_Func       Init;
  AIS3624DQ_DeInit_Func     DeInit;
  uint32_t                  BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
  uint8_t                   Address;
  AIS3624DQ_WriteReg_Func   WriteReg;
  AIS3624DQ_ReadReg_Func    ReadReg;
  AIS3624DQ_GetTick_Func    GetTick;
} AIS3624DQ_IO_t;

typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} AIS3624DQ_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} AIS3624DQ_Axes_t;

typedef struct
{
  AIS3624DQ_IO_t     IO;
  stmdev_ctx_t       Ctx;
  uint8_t            is_initialized;
  uint8_t            acc_is_enabled;
  ais3624dq_dr_t     acc_odr;
} AIS3624DQ_Object_t;

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
} AIS3624DQ_Capabilities_t;

typedef struct
{
  int32_t (*Init)(AIS3624DQ_Object_t *);
  int32_t (*DeInit)(AIS3624DQ_Object_t *);
  int32_t (*ReadID)(AIS3624DQ_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(AIS3624DQ_Object_t *, AIS3624DQ_Capabilities_t *);
} AIS3624DQ_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(AIS3624DQ_Object_t *);
  int32_t (*Disable)(AIS3624DQ_Object_t *);
  int32_t (*GetSensitivity)(AIS3624DQ_Object_t *, float *);
  int32_t (*GetOutputDataRate)(AIS3624DQ_Object_t *, float *);
  int32_t (*SetOutputDataRate)(AIS3624DQ_Object_t *, float);
  int32_t (*GetFullScale)(AIS3624DQ_Object_t *, int32_t *);
  int32_t (*SetFullScale)(AIS3624DQ_Object_t *, int32_t);
  int32_t (*GetAxes)(AIS3624DQ_Object_t *, AIS3624DQ_Axes_t *);
  int32_t (*GetAxesRaw)(AIS3624DQ_Object_t *, AIS3624DQ_AxesRaw_t *);
} AIS3624DQ_ACC_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} ais3624dq_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} ais3624dq_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} ais3624dq_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} ais3624dq_axis1bit32_t;

/**
  * @}
  */

/** @defgroup AIS3624DQ_Exported_Constants AIS3624DQ Exported Constants
  * @{
  */

#define AIS3624DQ_OK                       0
#define AIS3624DQ_ERROR                   -1

#define AIS3624DQ_I2C_BUS                 0U
#define AIS3624DQ_SPI_4WIRES_BUS          1U
#define AIS3624DQ_SPI_3WIRES_BUS          2U

#define AIS3624DQ_ACC_SENSITIVITY_FOR_FS_6G     2.9f  /**< Sensitivity value for  6g full scale [mg/LSB] */
#define AIS3624DQ_ACC_SENSITIVITY_FOR_FS_12G    5.9f  /**< Sensitivity value for 12g full scale [mg/LSB] */
#define AIS3624DQ_ACC_SENSITIVITY_FOR_FS_24G   11.7f  /**< Sensitivity value for 24g full scale [mg/LSB] */

/**
  * @}
  */

/** @addtogroup AIS3624DQ_Exported_Functions AIS3624DQ Exported Functions
  * @{
  */

int32_t AIS3624DQ_RegisterBusIO(AIS3624DQ_Object_t *pObj, AIS3624DQ_IO_t *pIO);
int32_t AIS3624DQ_Init(AIS3624DQ_Object_t *pObj);
int32_t AIS3624DQ_DeInit(AIS3624DQ_Object_t *pObj);
int32_t AIS3624DQ_ReadID(AIS3624DQ_Object_t *pObj, uint8_t *Id);
int32_t AIS3624DQ_GetCapabilities(AIS3624DQ_Object_t *pObj, AIS3624DQ_Capabilities_t *Capabilities);

int32_t AIS3624DQ_ACC_Enable(AIS3624DQ_Object_t *pObj);
int32_t AIS3624DQ_ACC_Disable(AIS3624DQ_Object_t *pObj);
int32_t AIS3624DQ_ACC_GetSensitivity(AIS3624DQ_Object_t *pObj, float *Sensitivity);
int32_t AIS3624DQ_ACC_GetOutputDataRate(AIS3624DQ_Object_t *pObj, float *Odr);
int32_t AIS3624DQ_ACC_SetOutputDataRate(AIS3624DQ_Object_t *pObj, float Odr);
int32_t AIS3624DQ_ACC_GetFullScale(AIS3624DQ_Object_t *pObj, int32_t *FullScale);
int32_t AIS3624DQ_ACC_SetFullScale(AIS3624DQ_Object_t *pObj, int32_t FullScale);
int32_t AIS3624DQ_ACC_GetAxesRaw(AIS3624DQ_Object_t *pObj, AIS3624DQ_AxesRaw_t *Value);
int32_t AIS3624DQ_ACC_GetAxes(AIS3624DQ_Object_t *pObj, AIS3624DQ_Axes_t *Acceleration);

int32_t AIS3624DQ_Read_Reg(AIS3624DQ_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t AIS3624DQ_Write_Reg(AIS3624DQ_Object_t *pObj, uint8_t reg, uint8_t Data);

int32_t AIS3624DQ_ACC_Get_DRDY_Status(AIS3624DQ_Object_t *pObj, uint8_t *Status);

/**
  * @}
  */

/** @addtogroup AIS3624DQ_Exported_Variables AIS3624DQ Exported Variables
  * @{
  */

extern AIS3624DQ_CommonDrv_t AIS3624DQ_COMMON_Driver;
extern AIS3624DQ_ACC_Drv_t AIS3624DQ_ACC_Driver;

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
