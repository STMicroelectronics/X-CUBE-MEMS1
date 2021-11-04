/**
 ******************************************************************************
 * @file    motion_tl_manager.h
 * @author  MEMS Software Solutions Team
 * @brief   This file contains definitions for the motion_tl_manager.c file
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Software License Agreement SLA0077,
 * the “License�?. You may not use this component except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        www.st.com/sla0077
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MOTION_TL_MANAGER_H
#define MOTION_TL_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "motion_tl.h"
#include "main.h"
#include "iks01a3_mems_control.h"
#include "iks01a3_mems_control_ex.h"

/* Extern variables ----------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void MotionTL_manager_init(void);
void MotionTL_manager_run(MTL_input_t *data_in, uint64_t timestamp_ms, MTL_output_t *data_out);

void MotionTL_manager_getAngleMode(MTL_angle_mode_t *mode);
void MotionTL_manager_setAngleMode(MTL_angle_mode_t mode);

void MotionTL_manager_get_version(char *version, int *length);

void MotionTL_manager_calibratePosition(MTL_cal_position_t cal_position);
MTL_cal_result_t MotionTL_manager_getCalibrationValues(MTL_acc_cal_t *acc_cal);
void MotionTL_manager_setCalibrationValues(MTL_acc_cal_t *acc_cal);

void MotionTL_manager_getEstimatedMeasTime(float *time_s);

char MotionTL_LoadMagCalFromNVM(unsigned short int dataSize, unsigned int *data);
char MotionTL_SaveMagCalInNVM(unsigned short int dataSize, unsigned int *data);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_TL_MANAGER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
