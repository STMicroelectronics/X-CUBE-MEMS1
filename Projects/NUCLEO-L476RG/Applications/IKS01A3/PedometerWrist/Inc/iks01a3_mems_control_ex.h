/**
 ******************************************************************************
 * @file    iks01a3_mems_control_ex.h
 * @author  MEMS Application Team
 * @brief   This file contains the MEMS sensors interface for X-NUCLEO-IKS01A3
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Software License Agreement SLA0077,
 * the â€œLicenseâ€?. You may not use this component except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        www.st.com/sla0077
 *
 ******************************************************************************
 */

 /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IKS01A3_MEMS_CONTROL_EX_H
#define IKS01A3_MEMS_CONTROL_EX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "iks01a3_motion_sensors.h"
#include "iks01a3_motion_sensors_ex.h"
#include "iks01a3_env_sensors.h"
#include "iks01a3_env_sensors_ex.h"

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

#endif /* IKS01A3_MEMS_CONTROL_EX_H */
