/**
  ******************************************************************************
  * @file    motion_fd_manager.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains definitions for the motion_fd_manager.c file
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
#ifndef MOTION_FD_MANAGER_H
#define MOTION_FD_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "motion_fd.h"
#include "main.h"
#include "iks01a3_mems_control.h"
#include "iks01a3_mems_control_ex.h"

/* Extern variables ----------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void MotionFD_manager_init(void);
void MotionFD_manager_run(MFD_input_t *data_in, MFD_output_t *data_out);
void MotionFD_manager_get_version(char *version, int32_t *length);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_FD_MANAGER_H */
