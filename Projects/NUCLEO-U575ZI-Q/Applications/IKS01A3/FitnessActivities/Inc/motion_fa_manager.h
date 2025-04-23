/**
  ******************************************************************************
  * @file    motion_fa_manager.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains definitions for the motion_fa_manager.c file
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
#ifndef MOTION_FA_MANAGER_H
#define MOTION_FA_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "motion_fa.h"
#include "main.h"
#include "iks01a3_mems_control.h"
#include "iks01a3_mems_control_ex.h"

/* Extern variables ----------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void MotionFA_manager_init(void);
void MotionFA_manager_run(MFA_input_t *data_in, MFA_output_t *data_out);
void MotionFA_manager_get_version(char *version, int32_t *length);
void MotionFA_manager_reset_counter(void);
void MotionFA_manager_set_activity(MFA_activity_t activity);
void MotionFA_manager_get_activity(MFA_activity_t *activity);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_FA_MANAGER_H */
