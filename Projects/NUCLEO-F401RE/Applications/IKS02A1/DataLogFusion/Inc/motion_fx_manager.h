/**
 ******************************************************************************
 * @file    motion_fx_manager.h
 * @author  MEMS Software Solutions Team
 * @brief   This file contains definitions for the motion_fx_manager.c file
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
#ifndef MOTION_FX_MANAGER_H
#define MOTION_FX_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "motion_fx.h"
#include "main.h"

/* Extern variables ----------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void MotionFX_manager_init(void);
void MotionFX_manager_run(MFX_input_t *data_in, MFX_output_t *data_out, float delta_time);
void MotionFX_manager_start_6X(void);
void MotionFX_manager_stop_6X(void);
void MotionFX_manager_start_9X(void);
void MotionFX_manager_stop_9X(void);
void MotionFX_manager_get_version(char *version, int *length);
void MotionFX_manager_MagCal_run(MFX_MagCal_input_t *data_in, MFX_MagCal_output_t *data_out);
void MotionFX_manager_MagCal_start(int sampletime);
void MotionFX_manager_MagCal_stop(int sampletime);

char MotionFX_LoadMagCalFromNVM(unsigned short int dataSize, unsigned int *data);
char MotionFX_SaveMagCalInNVM(unsigned short int dataSize, unsigned int *data);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_FX_MANAGER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
