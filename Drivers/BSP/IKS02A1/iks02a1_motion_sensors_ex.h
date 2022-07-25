/**
  ******************************************************************************
  * @file    iks02a1_motion_sensors_ex.h
  * @author  MEMS Software Solutions Team
  * @brief   This file provides a set of extended functions needed to manage the motion sensors
  ******************************************************************************
  * @attention
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
#ifndef IKS02A1_MOTION_SENSOR_EX_H
#define IKS02A1_MOTION_SENSOR_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "iks02a1_motion_sensors.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup IKS02A1 IKS02A1
  * @{
  */

/** @addtogroup IKS02A1_MOTION_SENSOR_EX IKS02A1 MOTION SENSOR EX
  * @{
  */

/** @defgroup IKS02A1_MOTION_SENSOR_EX_Exported_Types IKS02A1 MOTION SENSOR EX Exported Types
  * @{
  */

typedef enum
{
  IKS02A1_MOTION_SENSOR_INT1_PIN = 0,
  IKS02A1_MOTION_SENSOR_INT2_PIN
} IKS02A1_MOTION_SENSOR_IntPin_t;

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
} IKS02A1_MOTION_SENSOR_Event_Status_t;

/**
  * @}
  */

/** @addtogroup IKS02A1_MOTION_SENSOR_EX_Exported_Functions IKS02A1 MOTION SENSOR EX Exported Functions
  * @{
  */

int32_t IKS02A1_MOTION_SENSOR_Get_DRDY_Status(uint32_t Instance, uint32_t Function, uint8_t *Status);
int32_t IKS02A1_MOTION_SENSOR_Read_Register(uint32_t Instance, uint8_t Reg, uint8_t *Data);
int32_t IKS02A1_MOTION_SENSOR_Write_Register(uint32_t Instance, uint8_t Reg, uint8_t Data);
int32_t IKS02A1_MOTION_SENSOR_FIFO_Get_Num_Samples(uint32_t Instance, uint16_t *NumSamples);
int32_t IKS02A1_MOTION_SENSOR_FIFO_Set_BDR(uint32_t Instance, uint32_t Function, float Bdr);
int32_t IKS02A1_MOTION_SENSOR_FIFO_Set_INT1_FIFO_Full(uint32_t Instance, uint8_t Status);
int32_t IKS02A1_MOTION_SENSOR_FIFO_Set_Watermark_Level(uint32_t Instance, uint16_t Watermark);
int32_t IKS02A1_MOTION_SENSOR_FIFO_Set_Stop_On_Fth(uint32_t Instance, uint8_t Status);
int32_t IKS02A1_MOTION_SENSOR_FIFO_Set_Mode(uint32_t Instance, uint8_t Mode);
int32_t IKS02A1_MOTION_SENSOR_FIFO_Get_Tag(uint32_t Instance, uint8_t *Tag);
int32_t IKS02A1_MOTION_SENSOR_FIFO_Get_Axes(uint32_t Instance, uint32_t Function, IKS02A1_MOTION_SENSOR_Axes_t *Data);
int32_t IKS02A1_MOTION_SENSOR_DRDY_Set_Mode(uint32_t Instance, uint8_t Mode);
int32_t IKS02A1_MOTION_SENSOR_DRDY_Enable_Interrupt(uint32_t Instance, uint32_t Function,
                                                    IKS02A1_MOTION_SENSOR_IntPin_t IntPin);
int32_t IKS02A1_MOTION_SENSOR_DRDY_Disable_Interrupt(uint32_t Instance, uint32_t Function,
                                                     IKS02A1_MOTION_SENSOR_IntPin_t IntPin);

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

#endif /* IKS02A1_MOTION_SENSOR_EX_H */
