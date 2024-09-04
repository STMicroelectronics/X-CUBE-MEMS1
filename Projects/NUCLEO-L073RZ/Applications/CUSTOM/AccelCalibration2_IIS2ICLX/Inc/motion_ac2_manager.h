/**
  ******************************************************************************
  * @file    motion_ac2_manager.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains definitions for the motion_ac2_manager.c file
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
void MotionAC2_manager_get_version(char *version, int32_t *length);
void MotionAC2_manager_compensate(MOTION_SENSOR_Axes_t *data_in, float data_out[2]);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_AC2_MANAGER_H */

