/**
 ******************************************************************************
 * @file    motion_sd_manager.h
 * @author  MEMS Software Solutions Team
 * @brief   This file contains definitions for the motion_sd_manager.c file
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Software License Agreement SLA0077,
 * the "License". You may not use this component except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        www.st.com/sla0077
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MOTION_SD_MANAGER_H
#define MOTION_SD_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "motion_sd.h"
#include "main.h"
#include "iks01a2_mems_control.h"
#include "iks01a2_mems_control_ex.h"

/* Extern variables ----------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void MotionSD_manager_init(void);
void MotionSD_manager_run(MSD_input_t *data_in, MSD_output_t *data_out);
void MotionSD_manager_reset(void);
void MotionSD_manager_get_version(char *version, int *length);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_SD_MANAGER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
