/**
  ******************************************************************************
  * @file    motion_fa_manager.c
  * @author  MEMS Software Solutions Team
  * @brief   This file contains Fitness Activities interface functions
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
#include "motion_fa_manager.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup FITNESS_ACTIVITIES FITNESS ACTIVITIES
 * @{
 */

/* Private variables ---------------------------------------------------------*/
static MFA_activity_t SelectedActivity = MFA_BICEPCURL;

/* Private typedef -----------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize MotionFA algorithm
 * @param  None
 * @retval None
 */
void MotionFA_manager_init(void)
{
  char acc_orientation[4];
  char gyr_orientation[4];

  MotionFA_Initialize();

  BSP_SENSOR_ACC_GetOrientation(acc_orientation);
  BSP_SENSOR_GYR_GetOrientation(gyr_orientation);
  MotionFA_SetOrientation_Acc(acc_orientation);
  MotionFA_SetOrientation_Gyr(gyr_orientation);
}

/**
 * @brief  Run Fitness Activities algorithm
 * @param  data_in  Structure containing input data
 * @param  data_out Structure containing output data
 * @retval None
 */
void MotionFA_manager_run(MFA_input_t *data_in, MFA_output_t *data_out)
{
  switch (SelectedActivity)
  {
    case MFA_BICEPCURL:
      MotionFA_BicepCurl_Update(data_in, data_out);
      break;

    case MFA_SQUAT:
      MotionFA_Squat_Update(data_in, data_out);
      break;

    case MFA_PUSHUP:
      MotionFA_Pushup_Update(data_in, data_out);
      break;

    default:
      break;
  }
}

/**
 * @brief  Get the library version
 * @param  version Library version string (must be array of 35 char)
 * @param  length  Library version string length
 * @retval None
 */
void MotionFA_manager_get_version(char *version, int32_t *length)
{
  *length = (int)MotionFA_GetLibVersion(version);
}

/**
 * @brief  Reset counter or current activity
 * @param  None
 * @retval None
 */
void MotionFA_manager_reset_counter(void)
{
  switch (SelectedActivity)
  {
    case MFA_BICEPCURL:
      MotionFA_BicepCurl_Reset();
      break;

    case MFA_SQUAT:
      MotionFA_Squat_Reset();
      break;

    case MFA_PUSHUP:
      MotionFA_Pushup_Reset();
      break;

    default:
      break;
  }
}

/**
 * @brief  Set activity
 * @param  activity Activity type
 * @retval None
 */
void MotionFA_manager_set_activity(MFA_activity_t activity)
{
  SelectedActivity = activity;
}

/**
 * @brief  Get activity
 * @param  None
 * @retval activity type
 */
void MotionFA_manager_get_activity(MFA_activity_t *activity)
{
  *activity = SelectedActivity;
}

/**
 * @}
 */

/**
 * @}
 */

