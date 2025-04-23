/**
  ******************************************************************************
  * @file    iks4a1_mems_control_ex.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains the MEMS sensors interface for X-NUCLEO-IKS4A1
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IKS4A1_MEMS_CONTROL_EX_H
#define IKS4A1_MEMS_CONTROL_EX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "iks4a1_motion_sensors.h"
#include "iks4a1_motion_sensors_ex.h"
#include "iks4a1_env_sensors.h"
#include "iks4a1_env_sensors_ex.h"

void BSP_SENSOR_ACC_GetOrientation(char *Orientation);
void BSP_SENSOR_GYR_GetOrientation(char *Orientation);
void BSP_SENSOR_MAG_GetOrientation(char *Orientation);

void BSP_SENSOR_ACC_Read_Register(uint8_t Reg, uint8_t *Data);
void BSP_SENSOR_GYR_Read_Register(uint8_t Reg, uint8_t *Data);
void BSP_SENSOR_MAG_Read_Register(uint8_t Reg, uint8_t *Data);
void BSP_SENSOR_PRESS_Read_Register(uint8_t Reg, uint8_t *Data);
void BSP_SENSOR_TEMP_Read_Register(uint8_t Reg, uint8_t *Data);
void BSP_SENSOR_HUM_Read_Register(uint8_t Reg, uint8_t *Data);

void BSP_SENSOR_ACC_Write_Register(uint8_t Reg, uint8_t Data);
void BSP_SENSOR_GYR_Write_Register(uint8_t Reg, uint8_t Data);
void BSP_SENSOR_MAG_Write_Register(uint8_t Reg, uint8_t Data);
void BSP_SENSOR_PRESS_Write_Register(uint8_t Reg, uint8_t Data);
void BSP_SENSOR_TEMP_Write_Register(uint8_t Reg, uint8_t Data);
void BSP_SENSOR_HUM_Write_Register(uint8_t Reg, uint8_t Data);

void BSP_SENSOR_ACC_SetDRDYMode(uint8_t Mode);
void BSP_SENSOR_GYR_SetDRDYMode(uint8_t Mode);
void BSP_SENSOR_ACC_SetDRDYInt(uint8_t Enable);
void BSP_SENSOR_GYR_SetDRDYInt(uint8_t Enable);
void BSP_SENSOR_ACC_GetDRDYStatus(uint8_t *Status);

void BSP_ACC_GYR_Read_FSM_Data(uint8_t *Data);
void BSP_ACC_GYR_Read_MLC_Data(uint8_t *Data);

#ifdef __cplusplus
}
#endif

#endif /* IKS4A1_MEMS_CONTROL_EX_H */
