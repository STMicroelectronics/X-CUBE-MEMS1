/**
  ******************************************************************************
  * @file    infrared_al_manager.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains definitions for the infrared_al_manager.c file
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
#ifndef INFRARED_AL_MANAGER_H
#define INFRARED_AL_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "infrared_al.h"
#include "main.h"

/* Extern variables ----------------------------------------------------------*/
extern uint8_t Odr;

/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void InfraredAL_manager_init(void);
void InfraredAL_manager_run(IAL_input_t *data_in, IAL_output_t *data_out);
void InfraredAL_manager_get_version(char *version, int32_t *length);

#ifdef __cplusplus
}
#endif

#endif /* INFRARED_AL_MANAGER_H */
