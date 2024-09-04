/**
  ******************************************************************************
  * @file    motion_cp_manager.c
  * @author  MEMS Software Solutions Team
  * @brief   This file contains Carry Position interface functions
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
#include "motion_cp_manager.h"

#include "iks4a1_mems_control_ex.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup CARRY_POSITION CARRY POSITION
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize the MotionCP engine
 * @param  None
 * @retval None
 */
void MotionCP_manager_init(void)
{
  char acc_orientation[4];

  MotionCP_Initialize();

  BSP_SENSOR_ACC_GetOrientation(acc_orientation);
  MotionCP_SetOrientation_Acc(acc_orientation);
}

/**
 * @brief  Run Carry Position algorithm
 * @param  data_in  Structure containing input data
 * @param  data_out Structure containing output data
 * @retval None
 */
void MotionCP_manager_run(MCP_input_t *data_in, MCP_output_t *data_out)
{
  MotionCP_Update(data_in, data_out);
}

/**
 * @brief  Get the library version
 * @param  version Library version string (must be array of 35 char)
 * @param  length  Library version string length
 * @retval None
 */
void MotionCP_manager_get_version(char *version, int32_t *length)
{
  *length = (int)MotionCP_GetLibVersion(version);
}

/**
 * @}
 */

/**
 * @}
 */
