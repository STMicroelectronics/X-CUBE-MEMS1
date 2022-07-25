/**
  ******************************************************************************
  * @file    ais2dw12.h
  * @author  MEMS Software Solutions Team
  * @brief   AIS2DW12 header driver file
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
#ifndef AIS2DW12_H
#define AIS2DW12_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "ais2dw12_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup AIS2DW12 AIS2DW12
  * @{
  */

/** @defgroup AIS2DW12_Exported_Types AIS2DW12 Exported Types
  * @{
  */

typedef int32_t (*AIS2DW12_Init_Func)(void);
typedef int32_t (*AIS2DW12_DeInit_Func)(void);
typedef int32_t (*AIS2DW12_GetTick_Func)(void);
typedef int32_t (*AIS2DW12_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*AIS2DW12_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  AIS2DW12_INT1_PIN,
  AIS2DW12_INT2_PIN,
} AIS2DW12_SensorIntPin_t;

typedef struct
{
  AIS2DW12_Init_Func         Init;
  AIS2DW12_DeInit_Func       DeInit;
  uint32_t                   BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
  uint8_t                    Address;
  AIS2DW12_WriteReg_Func     WriteReg;
  AIS2DW12_ReadReg_Func      ReadReg;
  AIS2DW12_GetTick_Func      GetTick;
} AIS2DW12_IO_t;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} AIS2DW12_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} AIS2DW12_Axes_t;

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
} AIS2DW12_Event_Status_t;

typedef enum
{
  AIS2DW12_POWER_MODE4,
  AIS2DW12_POWER_MODE3,
  AIS2DW12_POWER_MODE2,
  AIS2DW12_POWER_MODE1
} AIS2DW12_Operating_Mode_t;

typedef struct
{
  AIS2DW12_IO_t             IO;
  stmdev_ctx_t              Ctx;
  uint8_t                   is_initialized;
  uint8_t                   acc_is_enabled;
  float                     acc_odr;
  AIS2DW12_Operating_Mode_t acc_operating_mode;
} AIS2DW12_Object_t;

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
} AIS2DW12_Capabilities_t;

typedef struct
{
  int32_t (*Init)(AIS2DW12_Object_t *);
  int32_t (*DeInit)(AIS2DW12_Object_t *);
  int32_t (*ReadID)(AIS2DW12_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(AIS2DW12_Object_t *, AIS2DW12_Capabilities_t *);
} AIS2DW12_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(AIS2DW12_Object_t *);
  int32_t (*Disable)(AIS2DW12_Object_t *);
  int32_t (*GetSensitivity)(AIS2DW12_Object_t *, float *);
  int32_t (*GetOutputDataRate)(AIS2DW12_Object_t *, float *);
  int32_t (*SetOutputDataRate)(AIS2DW12_Object_t *, float);
  int32_t (*GetFullScale)(AIS2DW12_Object_t *, int32_t *);
  int32_t (*SetFullScale)(AIS2DW12_Object_t *, int32_t);
  int32_t (*GetAxes)(AIS2DW12_Object_t *, AIS2DW12_Axes_t *);
  int32_t (*GetAxesRaw)(AIS2DW12_Object_t *, AIS2DW12_AxesRaw_t *);
} AIS2DW12_ACC_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} ais2dw12_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} ais2dw12_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} ais2dw12_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} ais2dw12_axis1bit32_t;

/**
  * @}
  */

/** @defgroup AIS2DW12_Exported_Constants AIS2DW12 Exported Constants
  * @{
  */

#define AIS2DW12_OK                       0
#define AIS2DW12_ERROR                   -1

#define AIS2DW12_I2C_BUS                 0U
#define AIS2DW12_SPI_4WIRES_BUS          1U
#define AIS2DW12_SPI_3WIRES_BUS          2U

#define AIS2DW12_ACC_SENSITIVITY_FOR_FS_2G_LOPOW1_MODE   0.976f  /**< Sensitivity value for 2g full scale, Low-power1 mode [mg/LSB] */
#define AIS2DW12_ACC_SENSITIVITY_FOR_FS_2G_OTHER_MODES   0.244f  /**< Sensitivity value for 2g full scale, all other modes except Low-power1 [mg/LSB] */

#define AIS2DW12_ACC_SENSITIVITY_FOR_FS_4G_LOPOW1_MODE   1.952f  /**< Sensitivity value for 4g full scale, Low-power1 mode [mg/LSB] */
#define AIS2DW12_ACC_SENSITIVITY_FOR_FS_4G_OTHER_MODES   0.488f  /**< Sensitivity value for 4g full scale, all other modes except Low-power1 [mg/LSB] */

/**
  * @}
  */

/** @addtogroup AIS2DW12_Exported_Functions AIS2DW12 Exported Functions
  * @{
  */

int32_t AIS2DW12_RegisterBusIO(AIS2DW12_Object_t *pObj, AIS2DW12_IO_t *pIO);
int32_t AIS2DW12_Init(AIS2DW12_Object_t *pObj);
int32_t AIS2DW12_DeInit(AIS2DW12_Object_t *pObj);
int32_t AIS2DW12_ReadID(AIS2DW12_Object_t *pObj, uint8_t *Id);
int32_t AIS2DW12_GetCapabilities(AIS2DW12_Object_t *pObj, AIS2DW12_Capabilities_t *Capabilities);

int32_t AIS2DW12_ACC_Enable(AIS2DW12_Object_t *pObj);
int32_t AIS2DW12_ACC_Disable(AIS2DW12_Object_t *pObj);
int32_t AIS2DW12_ACC_GetSensitivity(AIS2DW12_Object_t *pObj, float *Sensitivity);
int32_t AIS2DW12_ACC_GetOutputDataRate(AIS2DW12_Object_t *pObj, float *Odr);
int32_t AIS2DW12_ACC_SetOutputDataRate(AIS2DW12_Object_t *pObj, float Odr);
int32_t AIS2DW12_ACC_SetOutputDataRate_With_Mode(AIS2DW12_Object_t *pObj, float Odr, AIS2DW12_Operating_Mode_t Mode);
int32_t AIS2DW12_ACC_GetFullScale(AIS2DW12_Object_t *pObj, int32_t *FullScale);
int32_t AIS2DW12_ACC_SetFullScale(AIS2DW12_Object_t *pObj, int32_t FullScale);
int32_t AIS2DW12_ACC_GetAxesRaw(AIS2DW12_Object_t *pObj, AIS2DW12_AxesRaw_t *Value);
int32_t AIS2DW12_ACC_GetAxes(AIS2DW12_Object_t *pObj, AIS2DW12_Axes_t *Acceleration);

int32_t AIS2DW12_Read_Reg(AIS2DW12_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t AIS2DW12_Write_Reg(AIS2DW12_Object_t *pObj, uint8_t reg, uint8_t Data);
int32_t AIS2DW12_Set_Interrupt_Latch(AIS2DW12_Object_t *pObj, uint8_t Status);

int32_t AIS2DW12_ACC_Enable_DRDY_Interrupt(AIS2DW12_Object_t *pObj);
int32_t AIS2DW12_ACC_Disable_DRDY_Interrupt(AIS2DW12_Object_t *pObj);

int32_t AIS2DW12_ACC_Set_Filter_Mode(AIS2DW12_Object_t *pObj, uint8_t filterMode);

int32_t AIS2DW12_ACC_Enable_Wake_Up_Detection(AIS2DW12_Object_t *pObj);
int32_t AIS2DW12_ACC_Disable_Wake_Up_Detection(AIS2DW12_Object_t *pObj);
int32_t AIS2DW12_ACC_Set_Wake_Up_Threshold(AIS2DW12_Object_t *pObj, uint8_t Threshold);
int32_t AIS2DW12_ACC_Set_Wake_Up_Duration(AIS2DW12_Object_t *pObj, uint8_t Duration);

int32_t AIS2DW12_ACC_Enable_Inactivity_Detection(AIS2DW12_Object_t *pObj);
int32_t AIS2DW12_ACC_Disable_Inactivity_Detection(AIS2DW12_Object_t *pObj);
int32_t AIS2DW12_ACC_Set_Sleep_Duration(AIS2DW12_Object_t *pObj, uint8_t Duration);

int32_t AIS2DW12_ACC_Enable_6D_Orientation(AIS2DW12_Object_t *pObj);
int32_t AIS2DW12_ACC_Disable_6D_Orientation(AIS2DW12_Object_t *pObj);
int32_t AIS2DW12_ACC_Set_6D_Orientation_Threshold(AIS2DW12_Object_t *pObj, uint8_t Threshold);
int32_t AIS2DW12_ACC_Get_6D_Orientation_XL(AIS2DW12_Object_t *pObj, uint8_t *XLow);
int32_t AIS2DW12_ACC_Get_6D_Orientation_XH(AIS2DW12_Object_t *pObj, uint8_t *XHigh);
int32_t AIS2DW12_ACC_Get_6D_Orientation_YL(AIS2DW12_Object_t *pObj, uint8_t *YLow);
int32_t AIS2DW12_ACC_Get_6D_Orientation_YH(AIS2DW12_Object_t *pObj, uint8_t *YHigh);
int32_t AIS2DW12_ACC_Get_6D_Orientation_ZL(AIS2DW12_Object_t *pObj, uint8_t *ZLow);
int32_t AIS2DW12_ACC_Get_6D_Orientation_ZH(AIS2DW12_Object_t *pObj, uint8_t *ZHigh);

int32_t AIS2DW12_ACC_Get_Event_Status(AIS2DW12_Object_t *pObj, AIS2DW12_Event_Status_t *Status);
int32_t AIS2DW12_ACC_Set_SelfTest(AIS2DW12_Object_t *pObj, uint8_t Val);
int32_t AIS2DW12_ACC_Get_DRDY_Status(AIS2DW12_Object_t *pObj, uint8_t *Status);
int32_t AIS2DW12_ACC_Get_Init_Status(AIS2DW12_Object_t *pObj, uint8_t *Status);

int32_t AIS2DW12_FIFO_Get_Num_Samples(AIS2DW12_Object_t *pObj, uint16_t *NumSamples);
int32_t AIS2DW12_FIFO_Set_Mode(AIS2DW12_Object_t *pObj, uint8_t Mode);

/**
  * @}
  */

/** @addtogroup AIS2DW12_Exported_Variables AIS2DW12 Exported Variables
  * @{
  */

extern AIS2DW12_CommonDrv_t AIS2DW12_COMMON_Driver;
extern AIS2DW12_ACC_Drv_t AIS2DW12_ACC_Driver;

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
