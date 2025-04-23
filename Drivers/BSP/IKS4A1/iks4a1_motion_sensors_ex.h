/**
  ******************************************************************************
  * @file    iks4a1_motion_sensors_ex.h
  * @author  MEMS Software Solutions Team
  * @brief   This file provides a set of extended functions needed to manage the motion sensors
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IKS4A1_MOTION_SENSOR_EX_H
#define IKS4A1_MOTION_SENSOR_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "iks4a1_motion_sensors.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup IKS4A1 IKS4A1
  * @{
  */

/** @addtogroup IKS4A1_MOTION_SENSOR_EX IKS4A1 MOTION SENSOR EX
  * @{
  */

/** @defgroup IKS4A1_MOTION_SENSOR_EX_Exported_Types IKS4A1 MOTION SENSOR EX Exported Types
  * @{
  */

typedef enum
{
  IKS4A1_MOTION_SENSOR_INT1_PIN = 0,
  IKS4A1_MOTION_SENSOR_INT2_PIN
} IKS4A1_MOTION_SENSOR_IntPin_t;

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
} IKS4A1_MOTION_SENSOR_Event_Status_t;

/**
  * @}
  */

/** @addtogroup IKS4A1_MOTION_SENSOR_EX_Exported_Functions IKS4A1 MOTION SENSOR EX Exported Functions
  * @{
  */

int32_t IKS4A1_MOTION_SENSOR_Read_Register(uint32_t Instance, uint8_t Reg, uint8_t *Data);
int32_t IKS4A1_MOTION_SENSOR_Write_Register(uint32_t Instance, uint8_t Reg, uint8_t Data);
int32_t IKS4A1_MOTION_SENSOR_ACC_HG_Enable(uint32_t Instance, uint32_t Function);
int32_t IKS4A1_MOTION_SENSOR_ACC_HG_Disable(uint32_t Instance, uint32_t Function);
int32_t IKS4A1_MOTION_SENSOR_ACC_HG_GetSensitivity(uint32_t Instance, uint32_t Function, float_t *Sensitivity);
int32_t IKS4A1_MOTION_SENSOR_ACC_HG_GetOutputDataRate(uint32_t Instance, uint32_t Function, float_t *Odr);
int32_t IKS4A1_MOTION_SENSOR_ACC_HG_SetOutputDataRate(uint32_t Instance, uint32_t Function, float_t Odr);
int32_t IKS4A1_MOTION_SENSOR_ACC_HG_GetFullScale(uint32_t Instance, uint32_t Function, int32_t *FullScale);
int32_t IKS4A1_MOTION_SENSOR_ACC_HG_SetFullScale(uint32_t Instance, uint32_t Function, int32_t FullScale);
int32_t IKS4A1_MOTION_SENSOR_ACC_HG_GetAxesRaw(uint32_t Instance, uint32_t Function, IKS4A1_MOTION_SENSOR_AxesRaw_t *AxesRaw);
int32_t IKS4A1_MOTION_SENSOR_ACC_HG_GetAxes(uint32_t Instance, uint32_t Function, IKS4A1_MOTION_SENSOR_Axes_t *Axes);
int32_t IKS4A1_MOTION_SENSOR_Get_DRDY_Status(uint32_t Instance, uint32_t Function, uint8_t *Status);
int32_t IKS4A1_MOTION_SENSOR_Get_Event_Status(uint32_t Instance, IKS4A1_MOTION_SENSOR_Event_Status_t *Status);
int32_t IKS4A1_MOTION_SENSOR_Enable_Free_Fall_Detection(uint32_t Instance, IKS4A1_MOTION_SENSOR_IntPin_t IntPin);
int32_t IKS4A1_MOTION_SENSOR_Disable_Free_Fall_Detection(uint32_t Instance);
int32_t IKS4A1_MOTION_SENSOR_Set_Free_Fall_Threshold(uint32_t Instance, uint8_t Threshold);
int32_t IKS4A1_MOTION_SENSOR_Set_Free_Fall_Duration(uint32_t Instance, uint8_t Duration);
int32_t IKS4A1_MOTION_SENSOR_Enable_Pedometer(uint32_t Instance, IKS4A1_MOTION_SENSOR_IntPin_t IntPin);
int32_t IKS4A1_MOTION_SENSOR_Disable_Pedometer(uint32_t Instance);
int32_t IKS4A1_MOTION_SENSOR_Reset_Step_Counter(uint32_t Instance);
int32_t IKS4A1_MOTION_SENSOR_Get_Step_Count(uint32_t Instance, uint16_t *StepCount);
int32_t IKS4A1_MOTION_SENSOR_Enable_Tilt_Detection(uint32_t Instance, IKS4A1_MOTION_SENSOR_IntPin_t IntPin);
int32_t IKS4A1_MOTION_SENSOR_Disable_Tilt_Detection(uint32_t Instance);
int32_t IKS4A1_MOTION_SENSOR_Enable_Wake_Up_Detection(uint32_t Instance, IKS4A1_MOTION_SENSOR_IntPin_t IntPin);
int32_t IKS4A1_MOTION_SENSOR_Disable_Wake_Up_Detection(uint32_t Instance);
int32_t IKS4A1_MOTION_SENSOR_Set_Wake_Up_Threshold(uint32_t Instance, uint8_t Threshold);
int32_t IKS4A1_MOTION_SENSOR_Set_Wake_Up_Duration(uint32_t Instance, uint8_t Duration);
int32_t IKS4A1_MOTION_SENSOR_Enable_Single_Tap_Detection(uint32_t Instance, IKS4A1_MOTION_SENSOR_IntPin_t IntPin);
int32_t IKS4A1_MOTION_SENSOR_Disable_Single_Tap_Detection(uint32_t Instance);
int32_t IKS4A1_MOTION_SENSOR_Enable_Double_Tap_Detection(uint32_t Instance, IKS4A1_MOTION_SENSOR_IntPin_t IntPin);
int32_t IKS4A1_MOTION_SENSOR_Disable_Double_Tap_Detection(uint32_t Instance);
int32_t IKS4A1_MOTION_SENSOR_Set_Tap_Threshold(uint32_t Instance, uint8_t Threshold);
int32_t IKS4A1_MOTION_SENSOR_Set_Tap_Shock_Time(uint32_t Instance, uint8_t Time);
int32_t IKS4A1_MOTION_SENSOR_Set_Tap_Quiet_Time(uint32_t Instance, uint8_t Time);
int32_t IKS4A1_MOTION_SENSOR_Set_Tap_Duration_Time(uint32_t Instance, uint8_t Time);
int32_t IKS4A1_MOTION_SENSOR_Enable_6D_Orientation(uint32_t Instance, IKS4A1_MOTION_SENSOR_IntPin_t IntPin);
int32_t IKS4A1_MOTION_SENSOR_Disable_6D_Orientation(uint32_t Instance);
int32_t IKS4A1_MOTION_SENSOR_Set_6D_Orientation_Threshold(uint32_t Instance, uint8_t Threshold);
int32_t IKS4A1_MOTION_SENSOR_Get_6D_Orientation_XL(uint32_t Instance, uint8_t *xl);
int32_t IKS4A1_MOTION_SENSOR_Get_6D_Orientation_XH(uint32_t Instance, uint8_t *xh);
int32_t IKS4A1_MOTION_SENSOR_Get_6D_Orientation_YL(uint32_t Instance, uint8_t *yl);
int32_t IKS4A1_MOTION_SENSOR_Get_6D_Orientation_YH(uint32_t Instance, uint8_t *yh);
int32_t IKS4A1_MOTION_SENSOR_Get_6D_Orientation_ZL(uint32_t Instance, uint8_t *zl);
int32_t IKS4A1_MOTION_SENSOR_Get_6D_Orientation_ZH(uint32_t Instance, uint8_t *zh);
int32_t IKS4A1_MOTION_SENSOR_FIFO_Get_Num_Samples(uint32_t Instance, uint16_t *NumSamples);
int32_t IKS4A1_MOTION_SENSOR_FIFO_Get_Full_Status(uint32_t Instance, uint8_t *Status);
int32_t IKS4A1_MOTION_SENSOR_FIFO_Set_BDR(uint32_t Instance, uint32_t Function, float_t Bdr);
int32_t IKS4A1_MOTION_SENSOR_FIFO_Set_INT1_FIFO_Full(uint32_t Instance, uint8_t Status);
int32_t IKS4A1_MOTION_SENSOR_FIFO_Set_Watermark_Level(uint32_t Instance, uint16_t Watermark);
int32_t IKS4A1_MOTION_SENSOR_FIFO_Set_Stop_On_Fth(uint32_t Instance, uint8_t Status);
int32_t IKS4A1_MOTION_SENSOR_FIFO_Set_Mode(uint32_t Instance, uint8_t Mode);
int32_t IKS4A1_MOTION_SENSOR_FIFO_Get_Tag(uint32_t Instance, uint8_t *Tag);
int32_t IKS4A1_MOTION_SENSOR_FIFO_Get_Axes(uint32_t Instance, uint32_t Function, IKS4A1_MOTION_SENSOR_Axes_t *Data);
int32_t IKS4A1_MOTION_SENSOR_Set_SelfTest(uint32_t Instance, uint32_t Function, uint8_t Mode);
int32_t IKS4A1_MOTION_SENSOR_DRDY_Set_Mode(uint32_t Instance, uint8_t Mode);
int32_t IKS4A1_MOTION_SENSOR_DRDY_Enable_Interrupt(uint32_t Instance, uint32_t Function,
                                                   IKS4A1_MOTION_SENSOR_IntPin_t IntPin);
int32_t IKS4A1_MOTION_SENSOR_DRDY_Disable_Interrupt(uint32_t Instance, uint32_t Function,
                                                    IKS4A1_MOTION_SENSOR_IntPin_t IntPin);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* IKS4A1_MOTION_SENSOR_EX_H */
