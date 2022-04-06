/**
 ******************************************************************************
 * @file    motion_ad_manager.c
 * @author  MEMS Software Solutions Team
 * @brief   This file contains AirplaneDetection interface functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Software License Agreement SLA0077,
 * the "License". You may not use this component except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        www.st.com/sla0077
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
void MotionAD_manager_init(int xl_odr)
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
void MotionAD_manager_get_version(char *version, int *length)
{
  *length = (int)MotionAD_GetLibVersion(version);
}

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
