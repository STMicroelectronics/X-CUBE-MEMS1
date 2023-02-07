/**
  ******************************************************************************
  * @file    motion_gr_manager.c
  * @author  MEMS Software Solutions Team
  * @brief   This file contains GestureRecognition interface functions
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
#include "motion_gr_manager.h"

#include "iks01a3_mems_control_ex.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup GESTURE_RECOGNITION GESTURE RECOGNITION
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize the MotionGR engine
 * @param  None
 * @retval None
 */
void MotionGR_manager_init(void)
{
  char acc_orientation[4];

  MotionGR_Initialize();

  BSP_SENSOR_ACC_GetOrientation(acc_orientation);
  MotionGR_SetOrientation_Acc(acc_orientation);
}

/**
 * @brief  Run GestureRecognition algorithm
 * @param  data_in  Structure containing input data
 * @param  data_out Structure containing output data
 * @retval None
 */
void MotionGR_manager_run(MGR_input_t *data_in, MGR_output_t *data_out)
{
  MotionGR_Update(data_in, data_out);
}

/**
 * @brief  Get the library version
 * @param  version Library version string (must be array of 35 char)
 * @param  length  Library version string length
 * @retval None
 */
void MotionGR_manager_get_version(char *version, int *length)
{
  *length = (int)MotionGR_GetLibVersion(version);
}

/**
 * @}
 */

/**
 * @}
 */

