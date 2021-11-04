/**
 ******************************************************************************
 * @file    motion_gc_manager.c
 * @author  MEMS Software Solutions Team
 * @brief   This file contains Gyroscope Calibration interface functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Software License Agreement SLA0077,
 * the â€œLicenseâ€?. You may not use this component except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        www.st.com/sla0077
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "motion_gc_manager.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup GYROSCOPE_CALIBRATION GYROSCOPE CALIBRATION
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize the MotionGC engine
 * @param  freq  sampling frequency
 * @retval None
 */
void MotionGC_manager_init(float freq)
{
  MotionGC_Initialize(MGC_MCU_STM32, &freq);
}

/**
 * @brief  Get the knobs setting of the library
 * @param  knobs pointer to knobs setting structure
 * @retval none
 */
void MotionGC_manager_get_knobs(MGC_knobs_t *knobs)
{
  MotionGC_GetKnobs(knobs);
}

/**
 * @brief  Set the knobs setting of the library
 * @param  knobs pointer to knobs setting structure
 * @retval none
 */
void MotionGC_manager_set_knobs(MGC_knobs_t *knobs)
{
  MotionGC_SetKnobs(knobs);
}

/**
 * @brief  Run Gyroscope Calibration algorithm and return compensation parameters
 * @param  data_in  pointer to acceleration [g] and angular rate values [dps]
 * @param  gyro_bias  pointer to actual gyroscope offset value in [dps]
 * @param  bias_update  pointer to an integer that is set to 1 if the gyroscope bias was updated, 0 otherwise
 * @retval none
 */
void MotionGC_manager_update(MGC_input_t *data_in, MGC_output_t *gyro_bias, int *bias_update)
{
  MotionGC_Update(data_in, gyro_bias, bias_update);
}

/**
 * @brief  Get the gyroscope compensation parameters
 * @param  bias_mdps  pointer to array of 3 elements containing the offset, one for each axis, in milli degree per second [mdps]
 * @retval none
 */
void MotionGC_manager_get_params(MGC_output_t *gyro_bias)
{
  MotionGC_GetCalParams(gyro_bias);
}

/**
 * @brief  Set the gyroscope compensation parameters
 * @param  bias_mdps  pointer to array of 3 elements containing the offset, one for each axis, in milli degree per second [mdps]
 * @retval none
 */
void MotionGC_manager_set_params(MGC_output_t *gyro_bias)
{
  MotionGC_SetCalParams(gyro_bias);
}

/**
 * @brief  Set new sample frequency
 * @param  freq  new sample frequency in Herz [Hz]
 * @retval none
 */
void MotionGC_manager_set_frequency(float freq)
{
  MotionGC_SetFrequency(&freq);
}

/**
 * @brief  Get the library version
 * @param  version  Library version string (must be array of 35 char)
 * @param  length  Library version string length
 * @retval none
 */
void MotionGC_manager_get_version(char *version, int *length)
{
  *length = (int)MotionGC_GetLibVersion(version);
}

/**
 * @brief  Do offset calibration
 * @param  dataIn  raw gyroscope data
 * @param  dataOut  calibrated data
 * @retval none
 */
void MotionGC_manager_compensate(MOTION_SENSOR_Axes_t *DataIn, MOTION_SENSOR_Axes_t *DataOut)
{
  MGC_output_t gyro_bias;

  MotionGC_GetCalParams(&gyro_bias);

  DataOut->x = (int32_t)(DataIn->x - gyro_bias_to_mdps(gyro_bias.GyroBiasX));
  DataOut->y = (int32_t)(DataIn->y - gyro_bias_to_mdps(gyro_bias.GyroBiasY));
  DataOut->z = (int32_t)(DataIn->z - gyro_bias_to_mdps(gyro_bias.GyroBiasZ));
}

int16_t gyro_bias_to_mdps(float gyro_bias)
{
  float mdps_float;

  if (gyro_bias >= 0.0f)
  {
    mdps_float = gyro_bias * 1000.0f + 0.5f;
    return (int16_t)mdps_float;
  }
  else
  {
    mdps_float = gyro_bias * 1000.0f - 0.5f;
    return (int16_t)mdps_float;
  }
}

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
