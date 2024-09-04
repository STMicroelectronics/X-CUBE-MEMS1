/**
  ******************************************************************************
  * @file    motion_ec_manager.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains definitions for the motion_ec_manager.c file
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
#ifndef MOTION_EC_MANAGER_H
#define MOTION_EC_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "motion_ec.h"
#include "main.h"
#include "iks01a3_mems_control.h"

/* Extern variables ----------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void MotionEC_manager_init(float freq);
void MotionEC_manager_transform_orientation(MOTION_SENSOR_Axes_t *acc_in, MOTION_SENSOR_Axes_t *mag_in, float acc_out[], float mag_out[]);
void MotionEC_manager_run(MEC_input_t *data_in, MEC_output_t *data_out);
void MotionEC_manager_calc_heading(float quaternion[], float *heading, int32_t *heading_valid);
void MotionEC_manager_get_version(char *version, int32_t *length);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_EC_MANAGER_H */
