/**
  ******************************************************************************
  * @file    motion_sd_manager.c
  * @author  MEMS Software Solutions Team
  * @brief   This file contains Standing vs Sitting Desk Detection interface functions
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
#include "motion_sd_manager.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
  * @{
  */

/** @addtogroup STANDING_SITTING_DESK STANDING SITTING DESK
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
  * @brief  Initialize the MotionSD engine
  * @param  None
  * @retval None
  */
void MotionSD_manager_init(void)
{
  char acc_orientation[4];

  MotionSD_Initialize();

  BSP_SENSOR_ACC_GetOrientation(acc_orientation);
  MotionSD_SetOrientation_Acc(acc_orientation);
}

/**
  * @brief  Run Standing vs Sitting Desk Detection algorithm
  * @param  data_in  Structure containing input data
  * @param  data_out Structure containing output data
  * @retval None
  */
void MotionSD_manager_run(MSD_input_t *data_in, MSD_output_t *data_out)
{
  MotionSD_Update(data_in, data_out);
}

/**
  * @brief  Reset algorithm
  * @param  None
  * @retval None
  */
void MotionSD_manager_reset(void)
{
  MotionSD_Reset();
}

/**
  * @brief  Get the library version
  * @param  version Library version string (must be array of 35 char)
  * @param  length  Library version string length
  * @retval None
  */
void MotionSD_manager_get_version(char *version, int32_t *length)
{
  *length = (int)MotionSD_GetLibVersion(version);
}

/**
  * @}
  */

/**
  * @}
  */

