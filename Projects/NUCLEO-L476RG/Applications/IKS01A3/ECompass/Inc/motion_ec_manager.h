/**
 ******************************************************************************
 * @file    motion_ec_manager.h
 * @author  MEMS Software Solutions Team
 * @brief   This file contains definitions for the motion_ec_manager.c file
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
#ifndef MOTION_EC_MANAGER_H
#define MOTION_EC_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "motion_ec.h"
#include "main.h"
#include "iks01a3_mems_control.h"

/* Extern variables ----------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void MotionEC_manager_init(float freq);
void MotionEC_manager_transform_orientation(MOTION_SENSOR_Axes_t *acc_in, MOTION_SENSOR_Axes_t *mag_in, float acc_out[], float mag_out[]);
void MotionEC_manager_run(MEC_input_t *data_in, MEC_output_t *data_out);
void MotionEC_manager_calc_heading(float quaternion[], float *heading, int *heading_valid);
void MotionEC_manager_get_version(char *version, int *length);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_EC_MANAGER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
