/**
 ******************************************************************************
 * @file    motion_id_manager.h
 * @author  MEMS Software Solutions Team
 * @brief   This file contains definitions for the motion_id_manager.c file
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
#ifndef MOTION_ID_MANAGER_H
#define MOTION_ID_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "motion_id.h"
#include "main.h"

/* Extern variables ----------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void MotionID_manager_init(void);
void MotionID_manager_run(MID_input_t *data_in, MID_output_t *data_out);
void MotionID_manager_get_version(char *version, int *length);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_ID_MANAGER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
