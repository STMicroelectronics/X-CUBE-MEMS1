/**
  ******************************************************************************
  * @file    env_aq_manager.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains definitions for the env_aq_manager.c file
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
#ifndef ENV_AQ_MANAGER_H
#define ENV_AQ_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "main.h"
#include "custom_mems_control.h"
#include "custom_mems_control_ex.h"

/* Extern variables ----------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void EnvAQ_manager_init(void);
void EnvAQ_manager_run(int32_t voc_raw_value, int32_t *voc_index_value);
void EnvAQ_manager_get_version(char *version, int *length);

#ifdef __cplusplus
}
#endif

#endif /* ENV_AQ_MANAGER_H */
