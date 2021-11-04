/**
 ******************************************************************************
 * @file    motion_ar_manager.c
 * @author  MEMS Software Solutions Team
 * @brief   This file contains Activity Recognition interface functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Software License Agreement SLA0077,
 * the “License�?. You may not use this component except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        www.st.com/sla0077
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "motion_ar_manager.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup ACTIVITY_RECOGNITION ACTIVITY RECOGNITION
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize the MotionAR engine
 * @param  None
 * @retval None
 */
void MotionAR_manager_init(void)
{
  char acc_orientation[4];

  MotionAR_Initialize();

  BSP_SENSOR_ACC_GetOrientation(acc_orientation);
  MotionAR_SetOrientation_Acc(acc_orientation);
}

/**
 * @brief  Run Activity Recognition algorithm
 * @param  data_in  Structure containing input data
 * @param  data_out Structure containing output data
 * @param  timestamp  Timestamp in [ms]
 * @retval None
 */
void MotionAR_manager_run(MAR_input_t *data_in, MAR_output_t *data_out, int64_t timestamp)
{
  MotionAR_Update(data_in, data_out, timestamp);
}

/**
 * @brief  Get the library version
 * @param  version Library version string (must be array of 35 char)
 * @param  length  Library version string length
 * @retval None
 */
void MotionAR_manager_get_version(char *version, int *length)
{
  *length = (int)MotionAR_GetLibVersion(version);
}

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
