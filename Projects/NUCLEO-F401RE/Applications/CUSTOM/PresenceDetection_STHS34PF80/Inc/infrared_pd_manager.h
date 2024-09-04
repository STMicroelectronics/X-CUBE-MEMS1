/**
  ******************************************************************************
  * @file    infrared_pd_manager.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains definitions for the infrared_pd_manager.c file
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
#ifndef INFRARED_PD_MANAGER_H
#define INFRARED_PD_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "infrared_pd.h"
#include "main.h"

/* Extern variables ----------------------------------------------------------*/
extern uint8_t Odr;
extern uint16_t AvgTmos;
extern uint8_t AvgT;
extern uint8_t GainFactor;
extern uint16_t Sensitivity;

/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void InfraredPD_manager_init(void);
void InfraredPD_manager_run(IPD_input_t *data_in, IPD_output_t *data_out);
void InfraredPD_manager_get_version(char *version, int32_t *length);

#ifdef __cplusplus
}
#endif

#endif /* INFRARED_PD_MANAGER_H */
