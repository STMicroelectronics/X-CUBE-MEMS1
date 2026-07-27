/**
  ******************************************************************************
  * @file    motion_xlf_manager.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains definitions for the motion_xlf_manager.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MOTION_XLF_MANAGER_H
#define MOTION_XLF_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "motion_xlf.h"
#include "main.h"
#include "custom_mems_control.h"
/* Extern variables ----------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void MotionXLF_manager_init(XLF_accel_fusion_config_t *config);
void MotionXLF_manager_run(XLF_in_t *data_in, XLF_out_t *data_out);
void MotionXLF_manager_get_version(char *version, int32_t *length);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_XLF_MANAGER_H */
