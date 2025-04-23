/**
  ******************************************************************************
  * @file    motion_ac2_manager.c
  * @author  MEMS Software Solutions Team
  * @brief   This file contains Accelerometer Calibration 2-axis interface functions
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
#include "motion_ac2_manager.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
  * @{
  */

/** @addtogroup ACCELEROMETER_CALIBRATION2 ACCELEROMETER CALIBRATION 2
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
  * @brief  Initialize the MotionAC2 engine
  * @param  freq  pointer to accelerometer sample frequency
  * @retval None
  */
void MotionAC2_manager_init(float *freq)
{
  MotionAC2_Init(MAC2_MCU_STM32, freq);
}

/**
  * @brief  Run Accelerometer Calibration 2-axis algorithm
  * @param  data_in  pointer to acceleration [g]
  * @param  timestamp_ms  timestamp [ms]
  * @param  is_calibrated  pointer value returns 1 if calibration is done with current sample, 0 otherwise
  * @retval None
  */
void MotionAC2_manager_update(MAC2_input_t *data_in, uint64_t timestamp_ms, uint8_t *is_calibrated)
{
  *is_calibrated = MotionAC2_Update(data_in, timestamp_ms);
}

/**
  * @brief  Get the algorithm parameters
  * @param  data_out  pointer to actual accelerometer offset [g], scale factor [-] and quality factor
  * @retval None
  */
void MotionAC2_manager_get_params(MAC2_cal_params_t *data_out)
{
  MotionAC2_GetCalParams(data_out);
}

/**
  * @brief  Get the library version
  * @param  version  Library version string (must be array of 35 char)
  * @param  length  Library version string length
  * @retval none
  */
void MotionAC2_manager_get_version(char *version, int32_t *length)
{
  *length = (int)MotionAC2_GetLibVersion(version);
}

/**
  * @brief  Do offset & scale factor calibration
  * @param  data_in  raw accelerometer data
  * @param  data_out  calibrated data
  * @retval none
  */
void MotionAC2_manager_compensate(MOTION_SENSOR_Axes_t *data_in, float data_out[2])
{
  MAC2_cal_params_t cal_params;

  MotionAC2_GetCalParams(&cal_params);

  data_out[0] = ((float)data_in->x / 1000.0f - cal_params.Bias[0]) * cal_params.SF[0];
  data_out[1] = ((float)data_in->y / 1000.0f - cal_params.Bias[1]) * cal_params.SF[1];
}

/**
  * @}
  */

/**
  * @}
  */

