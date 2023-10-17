/**
  ******************************************************************************
  * @file    motion_pm_manager.c
  * @author  MEMS Software Solutions Team
  * @brief   This file contains Pedometer interface functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "motion_pm_manager.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup PEDOMETER PEDOMETER
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize the MotionPM engine
 * @param  None
 * @retval None
 */
void MotionPM_manager_init(void)
{
  MotionPM_Initialize();
}

/**
 * @brief  Run Pedometer algorithm
 * @param  data_in  Structure containing input data
 * @param  data_out Structure containing output data
 * @retval None
 */
void MotionPM_manager_run(MPM_input_t *data_in, MPM_output_t *data_out)
{
  MotionPM_Update(data_in, data_out);
}

/**
 * @brief  Get the library version
 * @param  version Library version string (must be array of 35 char)
 * @param  length  Library version string length
 * @retval None
 */
void MotionPM_manager_get_version(char *version, int *length)
{
  *length = (int)MotionPM_GetLibVersion(version);
}

/**
 * @}
 */

/**
 * @}
 */

