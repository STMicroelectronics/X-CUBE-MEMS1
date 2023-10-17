/**
  ******************************************************************************
  * @file    motion_gc_manager.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains definitions for the motion_gc_manager.c file
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
#ifndef MOTION_GC_MANAGER_H
#define MOTION_GC_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "motion_gc.h"
#include "main.h"
#include "iks01a3_mems_control.h"

/* Extern variables ----------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void MotionGC_manager_init(float freq);
void MotionGC_manager_update(MGC_input_t *data_in, MGC_output_t *gyro_bias, int *bias_update);
void MotionGC_manager_get_knobs(MGC_knobs_t *knobs);
void MotionGC_manager_set_knobs(MGC_knobs_t *knobs);
void MotionGC_manager_get_params(MGC_output_t *gyro_bias);
void MotionGC_manager_set_params(MGC_output_t *gyro_bias);
void MotionGC_manager_set_frequency(float freq);
void MotionGC_manager_get_version(char *version, int *length);
void MotionGC_manager_compensate(MOTION_SENSOR_Axes_t *DataIn, MOTION_SENSOR_Axes_t *DataOut);

int16_t gyro_bias_to_mdps(float gyro_bias);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_GC_MANAGER_H */
