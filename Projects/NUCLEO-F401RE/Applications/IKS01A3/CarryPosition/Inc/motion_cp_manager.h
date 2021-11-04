/**
 ******************************************************************************
 * @file    motion_cp_manager.h
 * @author  MEMS Software Solutions Team
 * @brief   This file contains definitions for the motion_cp_manager.c file
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
#ifndef MOTION_CP_MANAGER_H
#define MOTION_CP_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "motion_cp.h"
#include "main.h"

/* Extern variables ----------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void MotionCP_manager_init(void);
void MotionCP_manager_run(MCP_input_t *data_in, MCP_output_t *data_out);
void MotionCP_manager_get_version(char *version, int *length);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_CP_MANAGER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
