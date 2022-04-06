/**
 ******************************************************************************
 * @file    motion_at_manager.c
 * @author  MEMS Software Solutions Team
 * @brief   This file contains Active Time interface functions
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

/* Includes ------------------------------------------------------------------*/
#include "motion_at_manager.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup ACTIVE_TIME ACTIVE TIME
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize the MotionAT engine
 * @param  None
 * @retval None
 */
void MotionAT_manager_init(void)
{
  MotionAT_Initialize();
}

/**
 * @brief  Run Active Time algorithm
 * @param  data_in  Structure containing input data
 * @param  data_out Structure containing output data
 * @retval None
 */
void MotionAT_manager_run(MAT_input_t *data_in, MAT_output_t *data_out)
{
  MotionAT_Update(data_in, data_out);
}

/**
 * @brief  Get the library version
 * @param  version Library version string (must be array of 35 char)
 * @param  length  Library version string length
 * @retval None
 */
void MotionAT_manager_get_version(char *version, int *length)
{
  *length = (int)MotionAT_GetLibVersion(version);
}

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
