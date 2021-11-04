/**
 ******************************************************************************
 * @file    motion_tl2_manager.h
 * @author  MEMS Software Solutions Team
 * @brief   This file contains definitions for the motion_tl2_manager.c file
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
#ifndef MOTION_TL2_MANAGER_H
#define MOTION_TL2_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "motion_tl2.h"
#include "main.h"
#include "custom_mems_control.h"

/* Extern variables ----------------------------------------------------------*/
extern MTL2_knobs_t MTL2_Knobs;

/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void MotionTL2_manager_init(void);
void MotionTL2_manager_run(MTL2_input_t *data_in, uint64_t timestamp_ms, MTL2_output_t *data_out);
void MotionTL2_manager_get_version(char *version, int *length);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_TL2_MANAGER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
