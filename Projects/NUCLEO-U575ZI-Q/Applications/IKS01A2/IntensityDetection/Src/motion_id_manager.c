/**
  ******************************************************************************
  * @file    motion_id_manager.c
  * @author  MEMS Software Solutions Team
  * @brief   This file contains Intensity Detection interface functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "motion_id_manager.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup INTENSITY_DETECTION INTENSITY DETECTION
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize the MotionID engine
 * @param  None
 * @retval None
 */
void MotionID_manager_init(void)
{
  MotionID_Initialize(MID_MCU_STM32);
}

/**
 * @brief  Run Intensity Detection algorithm
 * @param  data_in  Structure containing input data
 * @param  data_out Structure containing output data
 * @retval None
 */
void MotionID_manager_run(MID_input_t *data_in, MID_output_t *data_out)
{
  MotionID_Update(data_in, data_out);
}

/**
 * @brief  Get the library version
 * @param  version Library version string (must be array of 35 char)
 * @param  length  Library version string length
 * @retval None
 */
void MotionID_manager_get_version(char *version, int *length)
{
  *length = (int)MotionID_GetLibVersion(version);
}

/**
 * @}
 */

/**
 * @}
 */

