/**
  ******************************************************************************
  * @file    motion_di_manager.c
  * @author  MEMS Software Solutions Team
  * @brief   This file contains Dynamic Inclinometer interface functions
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
#include "motion_di_manager.h"
#include "iks01a3_mems_control_ex.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup DYNAMIC_INCLINOMETER DYNAMIC INCLINOMETER
 * @{
 */

/* Extern variables ----------------------------------------------------------*/
extern MDI_cal_type_t AccCalMode;
extern MDI_cal_type_t GyrCalMode;

/* Private defines -----------------------------------------------------------*/
#define SAMPLETODISCARD  15
#define DECIMATION  4U

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static MDI_knobs_t iKnobs;
static MDI_knobs_t *ipKnobs = &iKnobs;

static volatile int32_t sampleToDiscard = SAMPLETODISCARD;
static int32_t discardedCount = 0;

/* Private function prototypes -----------------------------------------------*/
static void float_array_set(float array[], float value, uint32_t count);

/* Exported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize the MotionDI engine
 * @param  freq frequency of input data
 * @retval None
 */
void MotionDI_manager_init(float freq)
{
  MotionDI_Initialize(&freq);

  MotionDI_getKnobs(ipKnobs);

  ipKnobs->AccKnob.CalType = MDI_CAL_CONTINUOUS;
  ipKnobs->GyrKnob.CalType = MDI_CAL_CONTINUOUS;

  BSP_SENSOR_ACC_GetOrientation(ipKnobs->AccOrientation);
  BSP_SENSOR_GYR_GetOrientation(ipKnobs->GyroOrientation);

  ipKnobs->SFKnob.output_type = MDI_ENGINE_OUTPUT_ENU;
  ipKnobs->SFKnob.modx = DECIMATION;

  MotionDI_setKnobs(ipKnobs);

  AccCalMode = ipKnobs->AccKnob.CalType;
  GyrCalMode = ipKnobs->GyrKnob.CalType;
}

/**
 * @brief  Get calibration mode for accelerometer
 * @param  mode Calibration mode
 * @retval None
 */
void MotionDI_get_acc_calibration_mode(MDI_cal_type_t *mode)
{
  MotionDI_getKnobs(ipKnobs);
  *mode = ipKnobs->AccKnob.CalType;
}

/**
 * @brief  Set calibration mode for accelerometer
 * @param  mode Calibration mode
 * @retval None
 */
void MotionDI_set_acc_calibration_mode(MDI_cal_type_t mode)
{
  MotionDI_getKnobs(ipKnobs);
  ipKnobs->AccKnob.CalType = mode;
  MotionDI_setKnobs(ipKnobs);
}

/**
 * @brief  Get calibration parameters for accelerometer
 * @param  acc_cal Structure containing calibration data for accelerometer
 * @retval None
 */
void MotionDI_get_acc_calibration(MDI_cal_output_t *acc_cal)
{
  MotionDI_AccCal_getParams(acc_cal);
}

/**
 * @brief  Reset calibration parameters for accelerometer
 * @retval None
 */
void MotionDI_reset_acc_calibration(void)
{
  MotionDI_AccCal_reset();
}

/**
 * @brief  Get calibration mode for gyroscope
 * @param  mode Calibration mode
 * @retval None
 */
void MotionDI_get_gyro_calibration_mode(MDI_cal_type_t *mode)
{
  MotionDI_getKnobs(ipKnobs);
  *mode = ipKnobs->GyrKnob.CalType;
}

/**
 * @brief  Set calibration mode for gyroscope
 * @param  mode Calibration mode
 * @retval None
 */
void MotionDI_set_gyro_calibration_mode(MDI_cal_type_t mode)
{
  MotionDI_getKnobs(ipKnobs);
  ipKnobs->GyrKnob.CalType = mode;
  MotionDI_setKnobs(ipKnobs);
}

/**
 * @brief  Get calibration parameters for gyroscope
 * @param  gyro_cal Structure containing calibration data for gyroscope
 * @retval None
 */
void MotionDI_get_gyro_calibration(MDI_cal_output_t *gyro_cal)
{
  MotionDI_GyrCal_getParams(gyro_cal);
}

/**
 * @brief  Reset calibration parameters for gyroscope
 * @retval None
 */
void MotionDI_reset_gyro_calibration(void)
{
  MotionDI_GyrCal_reset();
}

/**
 * @brief  Run Motion Sensor Data Fusion algorithm
 * @param  data_in  Structure containing input data
 * @param  data_out Structure containing output data
 * @retval None
 */
void MotionDI_manager_run(MDI_input_t *data_in, MDI_output_t *data_out)
{
  if (discardedCount == sampleToDiscard)
  {
    MotionDI_update(data_out, data_in);
  }
  else
  {
    discardedCount++;
    float_array_set(data_out->quaternion, 0, MDI_QNUM_AXES);
    float_array_set(data_out->rotation, 0, MDI_NUM_AXES);
    float_array_set(data_out->gravity, 0, MDI_NUM_AXES);
    float_array_set(data_out->linear_acceleration, 0, MDI_NUM_AXES);
  }
}

/**
 * @brief  Get the library version
 * @param  version  Library version string (must be array of 35 char)
 * @param  length  Library version string length
 * @retval None
 */
void MotionDI_manager_get_version(char *version, int32_t *length)
{
  *length = (int)MotionDI_GetLibVersion(version);
}

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Set float array items to value
 * @param  array Destination float array
 * @param  value Set to this value
 * @param  count Number of items to be set
 * @retval None
 */
static void float_array_set(float array[], float value, uint32_t count)
{
  for (uint32_t i = 0; i < count; i++)
  {
    array[i] = value;
  }
}

/**
 * @}
 */

/**
 * @}
 */
