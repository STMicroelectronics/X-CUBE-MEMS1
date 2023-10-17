/**
  ******************************************************************************
  * @file    motion_di_manager.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains definitions for the motion_di_manager.c file
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
#ifndef MOTION_DI_MANAGER_H
#define MOTION_DI_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "motion_di.h"
#include "main.h"

/* Extern variables ----------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void MotionDI_manager_init(float freq);
void MotionDI_get_acc_calibration_mode(MDI_cal_type_t *mode);
void MotionDI_set_acc_calibration_mode(MDI_cal_type_t mode);
void MotionDI_get_acc_calibration(MDI_cal_output_t *acc_cal);
void MotionDI_reset_acc_calibration(void);
void MotionDI_get_gyro_calibration_mode(MDI_cal_type_t *mode);
void MotionDI_set_gyro_calibration_mode(MDI_cal_type_t mode);
void MotionDI_get_gyro_calibration(MDI_cal_output_t *gyro_cal);
void MotionDI_reset_gyro_calibration(void);
void MotionDI_manager_run(MDI_input_t *data_in, MDI_output_t *data_out);
void MotionDI_manager_get_version(char *version, int *length);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_DI_MANAGER_H */

