/**
  ******************************************************************************
  * @file    motion_fd_manager.c
  * @author  MEMS Software Solutions Team
  * @brief   This file contains Fall Detection interface functions
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
#include "motion_fd_manager.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup FALL_DETECTION FALL DETECTION
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize the MotionFD engine
 * @param  None
 * @retval None
 */
void MotionFD_manager_init(void)
{
  MotionFD_Initialize();
}

/**
 * @brief  Run Fall Detection algorithm
 * @param  data_in  Structure containing input data
 * @param  data_out Structure containing output data
 * @retval None
 */
void MotionFD_manager_run(MFD_input_t *data_in, MFD_output_t *data_out)
{
  MotionFD_Update(data_in, data_out);
}

/**
 * @brief  Get the library version
 * @param  version Library version string (must be array of 35 char)
 * @param  length  Library version string length
 * @retval None
 */
void MotionFD_manager_get_version(char *version, int *length)
{
  *length = (int)MotionFD_GetLibVersion(version);
}

/**
 * @}
 */

/**
 * @}
 */

