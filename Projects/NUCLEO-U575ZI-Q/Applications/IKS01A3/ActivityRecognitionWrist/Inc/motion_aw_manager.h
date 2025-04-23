/**
  ******************************************************************************
  * @file    motion_aw_manager.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains definitions for the motion_aw_manager.c file
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
#ifndef MOTION_AW_MANAGER_H
#define MOTION_AW_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "motion_aw.h"
#include "main.h"
#include "iks01a3_mems_control.h"
#include "iks01a3_mems_control_ex.h"

/* Extern variables ----------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void MotionAW_manager_init(void);
void MotionAW_manager_run(MAW_input_t *data_in, MAW_output_t *data_out, int64_t timestamp);
void MotionAW_manager_get_version(char *version, int32_t *length);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_AW_MANAGER_H */
