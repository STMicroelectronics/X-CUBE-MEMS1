/**
  ******************************************************************************
  * @file    motion_pw_manager.c
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
#include "motion_pw_manager.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup PEDOMETER_WRIST PEDOMETER WRIST
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize the MotionPW engine
 * @param  None
 * @retval None
 */
void MotionPW_manager_init(void)
{
  MotionPW_Initialize();
  MotionPW_ResetStepCount();
  MotionPW_ResetPedometerLibrary();
}

/**
 * @brief  Run Pedometer algorithm
 * @param  data_in  Structure containing input data
 * @param  data_out Structure containing output data
 * @retval None
 */
void MotionPW_manager_run(MPW_input_t *data_in, MPW_output_t *data_out)
{
  MotionPW_Update(data_in, data_out);
}

/**
 * @brief  Get the library version
 * @param  version Library version string (must be array of 35 char)
 * @param  length  Library version string length
 * @retval None
 */
void MotionPW_manager_get_version(char *version, int *length)
{
  *length = (int)MotionPW_GetLibVersion(version);
}

/**
 * @}
 */

/**
 * @}
 */

