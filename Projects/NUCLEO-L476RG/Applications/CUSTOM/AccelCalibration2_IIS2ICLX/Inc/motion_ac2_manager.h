/**
 ******************************************************************************
 * @file    motion_ac2_manager.h
 * @author  MEMS Software Solutions Team
 * @brief   This file contains definitions for the motion_ac2_manager.c file
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
#ifndef MOTION_AC2_MANAGER_H
#define MOTION_AC2_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "motion_ac2.h"
#include "main.h"
#include "custom_mems_control.h"

/* Extern variables ----------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void MotionAC2_manager_init(float *freq);
void MotionAC2_manager_update(MAC2_input_t *data_in, uint64_t timestamp_ms, uint8_t *is_calibrated);
void MotionAC2_manager_get_params(MAC2_cal_params_t *data_out);
void MotionAC2_manager_get_version(char *version, int *length);
void MotionAC2_manager_compensate(MOTION_SENSOR_Axes_t *data_in, float data_out[2]);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_AC2_MANAGER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
