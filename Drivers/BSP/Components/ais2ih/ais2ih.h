/**
  ******************************************************************************
  * @file    ais2ih.h
  * @author  MEMS Software Solutions Team
  * @brief   AIS2IH header driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef AIS2IH_H
#define AIS2IH_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "ais2ih_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup AIS2IH AIS2IH
  * @{
  */

/** @defgroup AIS2IH_Exported_Types AIS2IH Exported Types
  * @{
  */

typedef int32_t (*AIS2IH_Init_Func)(void);
typedef int32_t (*AIS2IH_DeInit_Func)(void);
typedef int32_t (*AIS2IH_GetTick_Func)(void);
typedef void    (*AIS2IH_Delay_Func)(uint32_t);
typedef int32_t (*AIS2IH_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*AIS2IH_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  AIS2IH_INT1_PIN,
  AIS2IH_INT2_PIN,
} AIS2IH_SensorIntPin_t;

typedef struct
{
  AIS2IH_Init_Func         Init;
  AIS2IH_DeInit_Func       DeInit;
  uint32_t                 BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
  uint8_t                  Address;
  AIS2IH_WriteReg_Func     WriteReg;
  AIS2IH_ReadReg_Func      ReadReg;
  AIS2IH_GetTick_Func      GetTick;
  AIS2IH_Delay_Func        Delay;
} AIS2IH_IO_t;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} AIS2IH_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} AIS2IH_Axes_t;

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
} AIS2IH_Event_Status_t;

typedef enum
{
  AIS2IH_HIGH_PERFORMANCE_MODE,
  AIS2IH_LOW_POWER_MODE4,
  AIS2IH_LOW_POWER_MODE3,
  AIS2IH_LOW_POWER_MODE2,
  AIS2IH_LOW_POWER_MODE1
} AIS2IH_Operating_Mode_t;

typedef enum
{
  AIS2IH_LOW_NOISE_DISABLE,
  AIS2IH_LOW_NOISE_ENABLE
} AIS2IH_Low_Noise_t;

typedef struct
{
  AIS2IH_IO_t             IO;
  stmdev_ctx_t            Ctx;
  uint8_t                 is_initialized;
  uint8_t                 acc_is_enabled;
  float                   acc_odr;
  AIS2IH_Operating_Mode_t acc_operating_mode;
  AIS2IH_Low_Noise_t      acc_low_noise;
} AIS2IH_Object_t;

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
} AIS2IH_Capabilities_t;

typedef struct
{
  int32_t (*Init)(AIS2IH_Object_t *);
  int32_t (*DeInit)(AIS2IH_Object_t *);
  int32_t (*ReadID)(AIS2IH_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(AIS2IH_Object_t *, AIS2IH_Capabilities_t *);
} AIS2IH_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(AIS2IH_Object_t *);
  int32_t (*Disable)(AIS2IH_Object_t *);
  int32_t (*GetSensitivity)(AIS2IH_Object_t *, float *);
  int32_t (*GetOutputDataRate)(AIS2IH_Object_t *, float *);
  int32_t (*SetOutputDataRate)(AIS2IH_Object_t *, float);
  int32_t (*GetFullScale)(AIS2IH_Object_t *, int32_t *);
  int32_t (*SetFullScale)(AIS2IH_Object_t *, int32_t);
  int32_t (*GetAxes)(AIS2IH_Object_t *, AIS2IH_Axes_t *);
  int32_t (*GetAxesRaw)(AIS2IH_Object_t *, AIS2IH_AxesRaw_t *);
} AIS2IH_ACC_Drv_t;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} ais2ih_axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} ais2ih_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} ais2ih_axis3bit32_t;

typedef union
{
  int32_t i32bit;
  uint8_t u8bit[4];
} ais2ih_axis1bit32_t;

/**
  * @}
  */

/** @defgroup AIS2IH_Exported_Constants AIS2IH Exported Constants
  * @{
  */

#define AIS2IH_OK                       0
#define AIS2IH_ERROR                   -1

#define AIS2IH_I2C_BUS                 0U
#define AIS2IH_SPI_4WIRES_BUS          1U
#define AIS2IH_SPI_3WIRES_BUS          2U

#define AIS2IH_ACC_SENSITIVITY_FOR_FS_2G_LOPOW1_MODE   0.976f  /**< Sensitivity value for 2g full scale, Low-power1 mode [mg/LSB] */
#define AIS2IH_ACC_SENSITIVITY_FOR_FS_2G_OTHER_MODES   0.244f  /**< Sensitivity value for 2g full scale, all other modes except Low-power1 [mg/LSB] */

#define AIS2IH_ACC_SENSITIVITY_FOR_FS_4G_LOPOW1_MODE   1.952f  /**< Sensitivity value for 4g full scale, Low-power1 mode [mg/LSB] */
#define AIS2IH_ACC_SENSITIVITY_FOR_FS_4G_OTHER_MODES   0.488f  /**< Sensitivity value for 4g full scale, all other modes except Low-power1 [mg/LSB] */

#define AIS2IH_ACC_SENSITIVITY_FOR_FS_8G_LOPOW1_MODE   3.904f  /**< Sensitivity value for 8g full scale, Low-power1 mode [mg/LSB] */
#define AIS2IH_ACC_SENSITIVITY_FOR_FS_8G_OTHER_MODES   0.976f  /**< Sensitivity value for 8g full scale, all other modes except Low-power1 [mg/LSB] */

#define AIS2IH_ACC_SENSITIVITY_FOR_FS_16G_LOPOW1_MODE  7.808f  /**< Sensitivity value for 16g full scale, Low-power1 mode [mg/LSB] */
#define AIS2IH_ACC_SENSITIVITY_FOR_FS_16G_OTHER_MODES  1.952f  /**< Sensitivity value for 16g full scale, all other modes except Low-power1 [mg/LSB] */

/**
  * @}
  */

/** @addtogroup AIS2IH_Exported_Functions AIS2IH Exported Functions
  * @{
  */

int32_t AIS2IH_RegisterBusIO(AIS2IH_Object_t *pObj, AIS2IH_IO_t *pIO);
int32_t AIS2IH_Init(AIS2IH_Object_t *pObj);
int32_t AIS2IH_DeInit(AIS2IH_Object_t *pObj);
int32_t AIS2IH_ReadID(AIS2IH_Object_t *pObj, uint8_t *Id);
int32_t AIS2IH_GetCapabilities(AIS2IH_Object_t *pObj, AIS2IH_Capabilities_t *Capabilities);

int32_t AIS2IH_ACC_Enable(AIS2IH_Object_t *pObj);
int32_t AIS2IH_ACC_Disable(AIS2IH_Object_t *pObj);
int32_t AIS2IH_ACC_GetSensitivity(AIS2IH_Object_t *pObj, float *Sensitivity);
int32_t AIS2IH_ACC_GetOutputDataRate(AIS2IH_Object_t *pObj, float *Odr);
int32_t AIS2IH_ACC_SetOutputDataRate(AIS2IH_Object_t *pObj, float Odr);
int32_t AIS2IH_ACC_SetOutputDataRate_With_Mode(AIS2IH_Object_t *pObj, float Odr, AIS2IH_Operating_Mode_t Mode,
                                               AIS2IH_Low_Noise_t Noise);
int32_t AIS2IH_ACC_GetFullScale(AIS2IH_Object_t *pObj, int32_t *FullScale);
int32_t AIS2IH_ACC_SetFullScale(AIS2IH_Object_t *pObj, int32_t FullScale);
int32_t AIS2IH_ACC_GetAxesRaw(AIS2IH_Object_t *pObj, AIS2IH_AxesRaw_t *Value);
int32_t AIS2IH_ACC_GetAxes(AIS2IH_Object_t *pObj, AIS2IH_Axes_t *Acceleration);

int32_t AIS2IH_Read_Reg(AIS2IH_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t AIS2IH_Write_Reg(AIS2IH_Object_t *pObj, uint8_t reg, uint8_t Data);
int32_t AIS2IH_Set_Interrupt_Latch(AIS2IH_Object_t *pObj, uint8_t Status);

int32_t AIS2IH_ACC_Enable_DRDY_Interrupt(AIS2IH_Object_t *pObj);
int32_t AIS2IH_ACC_Disable_DRDY_Interrupt(AIS2IH_Object_t *pObj);

int32_t AIS2IH_ACC_Set_Filter_Mode(AIS2IH_Object_t *pObj, uint8_t filterMode);

int32_t AIS2IH_ACC_Enable_Wake_Up_Detection(AIS2IH_Object_t *pObj);
int32_t AIS2IH_ACC_Disable_Wake_Up_Detection(AIS2IH_Object_t *pObj);
int32_t AIS2IH_ACC_Set_Wake_Up_Threshold(AIS2IH_Object_t *pObj, uint8_t Threshold);
int32_t AIS2IH_ACC_Set_Wake_Up_Duration(AIS2IH_Object_t *pObj, uint8_t Duration);

int32_t AIS2IH_ACC_Enable_Inactivity_Detection(AIS2IH_Object_t *pObj);
int32_t AIS2IH_ACC_Disable_Inactivity_Detection(AIS2IH_Object_t *pObj);
int32_t AIS2IH_ACC_Set_Sleep_Duration(AIS2IH_Object_t *pObj, uint8_t Duration);

int32_t AIS2IH_ACC_Enable_6D_Orientation(AIS2IH_Object_t *pObj);
int32_t AIS2IH_ACC_Disable_6D_Orientation(AIS2IH_Object_t *pObj);
int32_t AIS2IH_ACC_Set_6D_Orientation_Threshold(AIS2IH_Object_t *pObj, uint8_t Threshold);
int32_t AIS2IH_ACC_Get_6D_Orientation_XL(AIS2IH_Object_t *pObj, uint8_t *XLow);
int32_t AIS2IH_ACC_Get_6D_Orientation_XH(AIS2IH_Object_t *pObj, uint8_t *XHigh);
int32_t AIS2IH_ACC_Get_6D_Orientation_YL(AIS2IH_Object_t *pObj, uint8_t *YLow);
int32_t AIS2IH_ACC_Get_6D_Orientation_YH(AIS2IH_Object_t *pObj, uint8_t *YHigh);
int32_t AIS2IH_ACC_Get_6D_Orientation_ZL(AIS2IH_Object_t *pObj, uint8_t *ZLow);
int32_t AIS2IH_ACC_Get_6D_Orientation_ZH(AIS2IH_Object_t *pObj, uint8_t *ZHigh);

int32_t AIS2IH_ACC_Get_Event_Status(AIS2IH_Object_t *pObj, AIS2IH_Event_Status_t *Status);
int32_t AIS2IH_ACC_Set_SelfTest(AIS2IH_Object_t *pObj, uint8_t Val);
int32_t AIS2IH_ACC_Get_DRDY_Status(AIS2IH_Object_t *pObj, uint8_t *Status);
int32_t AIS2IH_ACC_Get_Init_Status(AIS2IH_Object_t *pObj, uint8_t *Status);

int32_t AIS2IH_FIFO_Get_Num_Samples(AIS2IH_Object_t *pObj, uint16_t *NumSamples);
int32_t AIS2IH_FIFO_Set_Mode(AIS2IH_Object_t *pObj, uint8_t Mode);

/**
  * @}
  */

/** @addtogroup AIS2IH_Exported_Variables AIS2IH Exported Variables
  * @{
  */

extern AIS2IH_CommonDrv_t AIS2IH_COMMON_Driver;
extern AIS2IH_ACC_Drv_t AIS2IH_ACC_Driver;

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
