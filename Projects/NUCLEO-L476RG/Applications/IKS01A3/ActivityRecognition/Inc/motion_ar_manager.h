/**
 ******************************************************************************
 * @file    motion_ar_manager.h
 * @author  MEMS Software Solutions Team
 * @brief   This file contains definitions for the motion_ar_manager.c file
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
#ifndef MOTION_AR_MANAGER_H
#define MOTION_AR_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "motion_ar.h"
#include "main.h"
#include "iks01a3_mems_control.h"
#include "iks01a3_mems_control_ex.h"

/* Extern variables ----------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void MotionAR_manager_init(void);
void MotionAR_manager_run(MAR_input_t *data_in, MAR_output_t *data_out, int64_t timestamp);
void MotionAR_manager_get_version(char *version, int *length);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_AR_MANAGER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
