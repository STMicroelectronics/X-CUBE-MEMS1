/**
 ******************************************************************************
 * @file    custom_mems_control.h
 * @author  MEMS Application Team
 * @brief   This file contains the MEMS sensors interface for custom board
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Software License Agreement SLA0077,
 * the "License". You may not use this component except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        www.st.com/sla0077
 *
 ******************************************************************************
 */

 /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CUSTOM_MEMS_CONTROL_H
#define CUSTOM_MEMS_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "RTE_Components.h"

#if (defined BSP_MOTION_SENSORS)
#include "custom_motion_sensors.h"
#endif

#if (defined BSP_ENV_SENSORS)
#include "custom_env_sensors.h"
#endif

#if (defined BSP_HYBRID_SENSORS)
#include "custom_hybrid_sensors.h"
#endif

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} MOTION_SENSOR_Axes_t;

void BSP_SENSOR_ACC_Init(void);
void BSP_SENSOR_GYR_Init(void);
void BSP_SENSOR_MAG_Init(void);
void BSP_SENSOR_PRESS_Init(void);
void BSP_SENSOR_TEMP_Init(void);
void BSP_SENSOR_HUM_Init(void);

void BSP_SENSOR_ACC_Enable(void);
void BSP_SENSOR_GYR_Enable(void);
void BSP_SENSOR_MAG_Enable(void);
void BSP_SENSOR_PRESS_Enable(void);
void BSP_SENSOR_TEMP_Enable(void);
void BSP_SENSOR_HUM_Enable(void);

void BSP_SENSOR_ACC_Disable(void);
void BSP_SENSOR_GYR_Disable(void);
void BSP_SENSOR_MAG_Disable(void);
void BSP_SENSOR_PRESS_Disable(void);
void BSP_SENSOR_TEMP_Disable(void);
void BSP_SENSOR_HUM_Disable(void);

void BSP_SENSOR_ACC_GetAxes(MOTION_SENSOR_Axes_t *Axes);
void BSP_SENSOR_GYR_GetAxes(MOTION_SENSOR_Axes_t *Axes);
void BSP_SENSOR_MAG_GetAxes(MOTION_SENSOR_Axes_t *Axes);
void BSP_SENSOR_PRESS_GetValue(float *Value);
void BSP_SENSOR_TEMP_GetValue(float *Value);
void BSP_SENSOR_HUM_GetValue(float *Value);

void BSP_SENSOR_ACC_SetOutputDataRate(float Odr);
void BSP_SENSOR_GYR_SetOutputDataRate(float Odr);
void BSP_SENSOR_MAG_SetOutputDataRate(float Odr);
void BSP_SENSOR_PRESS_SetOutputDataRate(float Odr);
void BSP_SENSOR_TEMP_SetOutputDataRate(float Odr);
void BSP_SENSOR_HUM_SetOutputDataRate(float Odr);

void BSP_SENSOR_ACC_GetOutputDataRate(float *Odr);
void BSP_SENSOR_GYR_GetOutputDataRate(float *Odr);
void BSP_SENSOR_MAG_GetOutputDataRate(float *Odr);
void BSP_SENSOR_PRESS_GetOutputDataRate(float *Odr);
void BSP_SENSOR_TEMP_GetOutputDataRate(float *Odr);
void BSP_SENSOR_HUM_GetOutputDataRate(float *Odr);

void BSP_SENSOR_ACC_SetFullScale(int32_t Fullscale);
void BSP_SENSOR_GYR_SetFullScale(int32_t Fullscale);
void BSP_SENSOR_MAG_SetFullScale(int32_t Fullscale);

void BSP_SENSOR_ACC_GetFullScale(int32_t *Fullscale);
void BSP_SENSOR_GYR_GetFullScale(int32_t *Fullscale);
void BSP_SENSOR_MAG_GetFullScale(int32_t *Fullscale);

#endif /* CUSTOM_MEMS_CONTROL_H */
