/**
  ******************************************************************************
  * @file    motion_ar_manager.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains definitions for the motion_ar_manager.c file
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
#ifndef MOTION_AR_MANAGER_H
#define MOTION_AR_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "motion_ar.h"
#include "main.h"
#include "iks4a1_mems_control.h"
#include "iks4a1_mems_control_ex.h"

/* Extern variables ----------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void MotionAR_manager_init(void);
void MotionAR_manager_run(MAR_input_t *data_in, MAR_output_t *data_out, int64_t timestamp);
void MotionAR_manager_get_version(char *version, int32_t *length);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_AR_MANAGER_H */
