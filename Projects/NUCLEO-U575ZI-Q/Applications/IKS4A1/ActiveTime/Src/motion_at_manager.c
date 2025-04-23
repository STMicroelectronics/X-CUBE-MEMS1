/**
  ******************************************************************************
  * @file    motion_at_manager.c
  * @author  MEMS Software Solutions Team
  * @brief   This file contains Active Time interface functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
void MotionAT_manager_get_version(char *version, int32_t *length)
{
  *length = (int)MotionAT_GetLibVersion(version);
}

/**
  * @}
  */

/**
  * @}
  */

