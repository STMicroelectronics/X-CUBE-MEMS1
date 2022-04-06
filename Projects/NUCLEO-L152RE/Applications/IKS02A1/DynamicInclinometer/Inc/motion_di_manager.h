/**
 ******************************************************************************
 * @file    motion_di_manager.h
 * @author  MEMS Software Solutions Team
 * @brief   This file contains definitions for the motion_di_manager.c file
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
#ifndef MOTION_DI_MANAGER_H
#define MOTION_DI_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "motion_di.h"
#include "main.h"

/* Extern variables ----------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void MotionDI_manager_init(float freq);
void MotionDI_get_acc_calibration_mode(MDI_cal_type_t *mode);
void MotionDI_set_acc_calibration_mode(MDI_cal_type_t mode);
void MotionDI_get_acc_calibration(MDI_cal_output_t *acc_cal);
void MotionDI_reset_acc_calibration(void);
void MotionDI_get_gyro_calibration_mode(MDI_cal_type_t *mode);
void MotionDI_set_gyro_calibration_mode(MDI_cal_type_t mode);
void MotionDI_get_gyro_calibration(MDI_cal_output_t *gyro_cal);
void MotionDI_reset_gyro_calibration(void);
void MotionDI_manager_run(MDI_input_t *data_in, MDI_output_t *data_out);
void MotionDI_manager_get_version(char *version, int *length);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_DI_MANAGER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
