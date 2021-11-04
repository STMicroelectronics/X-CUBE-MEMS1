/**
 ******************************************************************************
 * @file    motion_ac_manager.h
 * @author  MEMS Software Solutions Team
 * @brief   This file contains definitions for the motion_ac_manager.c file
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
#ifndef MOTION_AC_MANAGER_H
#define MOTION_AC_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "motion_ac.h"
#include "main.h"
#include "iks02a1_mems_control.h"

/* Extern variables ----------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
typedef enum
{
  DYNAMIC_CALIBRATION = 0,
  SIX_POINT_CALIBRATION = 1
} MAC_calibration_mode_t;

typedef enum
{
  MAC_DISABLE_LIB = 0,
  MAC_ENABLE_LIB = 1
} MAC_enable_lib_t;

/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void MotionAC_manager_init(MAC_enable_lib_t enable);
void MotionAC_manager_update(MAC_input_t *data_in, uint8_t *is_calibrated);
void MotionAC_manager_get_params(MAC_output_t *data_out);
void MotionAC_manager_get_version(char *version, int *length);
void MotionAC_manager_compensate(MOTION_SENSOR_Axes_t *DataIn, MOTION_SENSOR_Axes_t *DataOut);

int16_t acc_bias_to_mg(float acc_bias);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_AC_MANAGER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
