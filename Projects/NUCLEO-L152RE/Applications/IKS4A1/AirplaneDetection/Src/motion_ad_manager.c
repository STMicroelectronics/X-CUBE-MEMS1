/**
  ******************************************************************************
  * @file    motion_ad_manager.c
  * @author  MEMS Software Solutions Team
  * @brief   This file contains AirplaneDetection interface functions
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
#include "motion_ad_manager.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
  * @{
  */

/** @addtogroup AIRPLANE_DETECTION AIRPLANE DETECTION
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
  * @brief  Initialize the MotionAD engine
  * @param  xl_odr  Accelerometer ODR in Hz (nearrest int)
  * @retval None
  */
void MotionAD_manager_init(int32_t xl_odr)
{
  MotionAD_Initialize(xl_odr);
}

/**
  * @brief  Run AiplaneDetection algorithm
  * @param  data_in  Structure containing input data
  * @param  data_out Structure containing output data
  * @retval None
  */
void MotionAD_manager_run(MAD_input_t *data_in, MAD_output_t *data_out)
{
  MotionAD_Update(data_in, data_out);
}

/**
  * @brief  Get the library version
  * @param  version Library version string (must be array of 35 char)
  * @param  length  Library version string length
  * @retval None
  */
void MotionAD_manager_get_version(char *version, int32_t *length)
{
  *length = (int)MotionAD_GetLibVersion(version);
}

/**
  * @}
  */

/**
  * @}
  */

