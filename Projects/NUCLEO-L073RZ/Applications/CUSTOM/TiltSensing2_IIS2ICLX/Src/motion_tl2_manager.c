/**
  ******************************************************************************
  * @file    motion_tl2_manager.c
  * @author  MEMS Software Solutions Team
  * @brief   This file contains Tilt Sensing 2-axis interface functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "motion_tl2_manager.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup TILT_SENSING2 TILT SENSING 2
 * @{
 */

/* Extern variables ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize the MotionTL2 engine
 * @param  None
 * @retval None
 */
void MotionTL2_manager_init(void)
{
  MotionTL2_Init(MTL2_MCU_STM32);
}

/**
 * @brief  Run Tilt Sensing algorithm
 * @param  data_in  Structure containing input data
 * @param  timestamp_ms  Timestamp in [ms]
 * @param  data_out  Structure containing output data
 * @retval None
 */
void MotionTL2_manager_run(MTL2_input_t *data_in, uint64_t timestamp_ms, MTL2_output_t *data_out)
{
  MotionTL2_Update(data_in, timestamp_ms, data_out);
}

/**
 * @brief  Get the library version
 * @param  version  Library version string (must be array of 35 char)
 * @param  length  Library version string length
 * @retval None
 */
void MotionTL2_manager_get_version(char *version, int32_t *length)
{
  *length = (int)MotionTL2_GetLibVersion(version);
}

/**
 * @}
 */

/**
 * @}
 */

