/**
  ******************************************************************************
  * @file    custom_mems_control_ex.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains the MEMS sensors interface for custom board
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CUSTOM_MEMS_CONTROL_EX_H
#define CUSTOM_MEMS_CONTROL_EX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "RTE_Components.h"

#if (defined BSP_MOTION_SENSORS)
#include "custom_motion_sensors.h"
#include "custom_motion_sensors_ex.h"
#endif /* BSP_MOTION_SENSORS */

#if (defined BSP_ENV_SENSORS)
#include "custom_env_sensors.h"
#include "custom_env_sensors_ex.h"
#endif /* BSP_ENV_SENSORS */

#if (defined BSP_HYBRID_SENSORS)
#include "custom_hybrid_sensors.h"
#include "custom_hybrid_sensors_ex.h"
#endif /* BSP_HYBRID_SENSORS */

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

#if ((defined CUSTOM_ALGOBUILDER_FW_TEMPLATE) || (defined CUSTOM_DATALOGFUSION_DEMO) || (defined CUSTOM_TILTSENSING_DEMO) || (defined CUSTOM_GESTURERECOGNITION_DEMO) || (defined CUSTOM_CARRYPOSITION_DEMO) || (defined CUSTOM_ECOMPASS_DEMO) || (defined CUSTOM_DYNAMICINCLINOMETER_DEMO) || (defined CUSTOM_ACTIVITYRECOGNITION_DEMO) || (defined CUSTOM_ACTIVITYRECOGNITIONWRIST_DEMO) || (defined CUSTOM_FITNESSACTIVITIES_DEMO) || (defined CUSTOM_PEDOMETERWRIST_DEMO) || (defined CUSTOM_POSEESTIMATION_DEMO) || (defined CUSTOM_STANDINGSITTINGDESK_DEMO))
void BSP_SENSOR_ACC_GetOrientation(char *Orientation);
#endif /* CUSTOM */
#if ((defined CUSTOM_ALGOBUILDER_FW_TEMPLATE) || (defined CUSTOM_DATALOGFUSION_DEMO) || (defined CUSTOM_DYNAMICINCLINOMETER_DEMO) || (defined CUSTOM_FITNESSACTIVITIES_DEMO))
void BSP_SENSOR_GYR_GetOrientation(char *Orientation);
#endif /* CUSTOM */
#if ((defined CUSTOM_ALGOBUILDER_FW_TEMPLATE) || (defined CUSTOM_DATALOGFUSION_DEMO) || (defined CUSTOM_ECOMPASS_DEMO))
void BSP_SENSOR_MAG_GetOrientation(char *Orientation);
#endif /* CUSTOM */

#if (defined CUSTOM_ALGOBUILDER_FW_TEMPLATE)
void BSP_SENSOR_ACC_SetDRDYMode(uint8_t Mode);
void BSP_SENSOR_GYR_SetDRDYMode(uint8_t Mode);
void BSP_SENSOR_ACC_SetDRDYInt(uint8_t Enable);
void BSP_SENSOR_GYR_SetDRDYInt(uint8_t Enable);

void BSP_ACC_GYR_Read_FSM_Data(uint8_t *Data);
void BSP_ACC_GYR_Read_MLC_Data(uint8_t *Data);
#endif /* CUSTOM_ALGOBUILDER_FW_TEMPLATE */

#if (defined PRESENCEDETECTION_STHS34PF80_DEMO)
void BSP_SENSOR_TEMP_GetTAmbRaw(int16_t *Value);
void BSP_SENSOR_TEMP_GetTObjRaw(int16_t *Value);
void BSP_SENSOR_TEMP_GetAvgTmos(uint16_t *Value);
void BSP_SENSOR_TEMP_SetAvgTmos(uint16_t Value);
void BSP_SENSOR_TEMP_GetAvgT(uint8_t *Value);
void BSP_SENSOR_TEMP_GetGainFactor(uint8_t *Value);
void BSP_SENSOR_TEMP_GetSensitivity(uint16_t *Value);
#endif /* PRESENCEDETECTION_STHS34PF80_DEMO */

#if (defined APPROACHLEAVE_STHS34PF80_DEMO)
void BSP_SENSOR_TEMP_GetTObjRaw(int16_t *Value);
#endif /* APPROACHLEAVE_STHS34PF80_DEMO */

#if (defined AIRQUALITY_SGP40_DEMO)
void BSP_SENSOR_Gas_Set_Humidity(float Value);
void BSP_SENSOR_Gas_Set_Temperature(float Value);
#endif /* AIRQUALITY_SGP40_DEMO */

#ifdef __cplusplus
}
#endif

#endif /* CUSTOM_MEMS_CONTROL_EX_H */
