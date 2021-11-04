/**
 ******************************************************************************
 * @file    motion_fa_manager.h
 * @author  MEMS Software Solutions Team
 * @brief   This file contains definitions for the motion_fa_manager.c file
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Software License Agreement SLA0077,
 * the â€œLicenseâ€?. You may not use this component except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        www.st.com/sla0077
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
void MotionFA_manager_get_version(char *version, int *length);
void MotionFA_manager_reset_counter(void);
void MotionFA_manager_set_activity(MFA_activity_t activity);
void MotionFA_manager_get_activity(MFA_activity_t *activity);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_FA_MANAGER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
