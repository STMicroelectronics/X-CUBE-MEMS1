/**
 ******************************************************************************
 * @file    iks02a1_mems_control.h
 * @author  MEMS Application Team
 * @brief   This file contains the MEMS sensors interface for X-NUCLEO-IKS02A1
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
#ifndef IKS02A1_MEMS_CONTROL_H
#define IKS02A1_MEMS_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "iks02a1_motion_sensors.h"
#include "iks02a1_motion_sensors_ex.h"
#include "sensor_unicleo_id.h"

#define ACC_GYR_UNICLEO_ID    ISM330DHCX_UNICLEO_ID
#define MAG_UNICLEO_ID        IIS2MDC_UNICLEO_ID
#define HUM_TEMP_UNICLEO_ID   UNKNOWN_UNICLEO_ID
#define PRESS_UNICLEO_ID      UNKNOWN_UNICLEO_ID

#define MOTION_SENSOR_Axes_t IKS02A1_MOTION_SENSOR_Axes_t

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

void BSP_SENSOR_ACC_GetAxes(IKS02A1_MOTION_SENSOR_Axes_t *Axes);
void BSP_SENSOR_GYR_GetAxes(IKS02A1_MOTION_SENSOR_Axes_t *Axes);
void BSP_SENSOR_MAG_GetAxes(IKS02A1_MOTION_SENSOR_Axes_t *Axes);
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

#endif /* IKS02A1_MEMS_CONTROL_H */
