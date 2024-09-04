/**
  ******************************************************************************
  * @file    motion_fx_manager.c
  * @author  MEMS Software Solutions Team
  * @brief   This file contains Datalog Fusion interface functions
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
#include "motion_fx_manager.h"
#include "iks02a1_mems_control_ex.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup DATALOG_FUSION DATALOG FUSION
 * @{
 */

/* Extern variables ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define STATE_SIZE                      (size_t)(2432)

#define GBIAS_ACC_TH_SC                 (2.0f*0.000765f)
#define GBIAS_GYRO_TH_SC                (2.0f*0.002f)
#define GBIAS_MAG_TH_SC                 (2.0f*0.001500f)

#define DECIMATION                      1U

/* Private variables ---------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize the MotionFX engine
 * @param  None
 * @retval None
 */
void MotionFX_manager_init(void)
{
  char acc_orientation[4];
  char gyro_orientation[4];
  char mag_orientation[4];

  BSP_SENSOR_ACC_GetOrientation(acc_orientation);
  BSP_SENSOR_GYR_GetOrientation(gyro_orientation);
  BSP_SENSOR_MAG_GetOrientation(mag_orientation);

  MotionFX_CM0P_initialize(MFX_CM0P_MCU_STM32);

  MotionFX_CM0P_setOrientation(acc_orientation, gyro_orientation, mag_orientation);

  MotionFX_CM0P_enable_gbias(MFX_CM0P_ENGINE_ENABLE);
  MotionFX_CM0P_enable_euler(MFX_CM0P_ENGINE_ENABLE);
  MotionFX_CM0P_enable_6X(MFX_CM0P_ENGINE_DISABLE);
  MotionFX_CM0P_enable_9X(MFX_CM0P_ENGINE_DISABLE);
}

/**
 * @brief  Run Motion Sensor Data Fusion algorithm
 * @param  data_in  Structure containing input data
 * @param  data_out Structure containing output data
 * @param  delta_time Delta time
 * @retval None
 */
void MotionFX_manager_run(MFX_CM0P_input_t *data_in, MFX_CM0P_output_t *data_out, float delta_time)
{
  MotionFX_CM0P_update(data_out, data_in, delta_time);
}

/**
 * @brief  Start 6 axes MotionFX engine
 * @param  None
 * @retval None
 */
void MotionFX_manager_start_6X(void)
{
  MotionFX_CM0P_enable_6X(MFX_CM0P_ENGINE_ENABLE);
}

/**
 * @brief  Stop 6 axes MotionFX engine
 * @param  None
 * @retval None
 */
void MotionFX_manager_stop_6X(void)
{
  MotionFX_CM0P_enable_6X(MFX_CM0P_ENGINE_DISABLE);
}

/**
 * @brief  Start 9 axes MotionFX engine
 * @param  None
 * @retval None
 */
void MotionFX_manager_start_9X(void)
{
  MotionFX_CM0P_enable_9X(MFX_CM0P_ENGINE_ENABLE);
}

/**
 * @brief  Stop 9 axes MotionFX engine
 * @param  None
 * @retval None
 */
void MotionFX_manager_stop_9X(void)
{
  MotionFX_CM0P_enable_9X(MFX_CM0P_ENGINE_DISABLE);
}

/**
 * @brief  Get the library version
 * @param  version  Library version string (must be array of 35 char)
 * @param  length  Library version string length
 * @retval None
 */
void MotionFX_manager_get_version(char *version, int32_t *length)
{
  *length = (int)MotionFX_CM0P_GetLibVersion(version);
}

/**
 * @brief  Run magnetometer calibration algorithm
 * @param  None
 * @retval None
 */
void MotionFX_manager_MagCal_run(MFX_CM0P_MagCal_input_t *data_in, MFX_CM0P_MagCal_output_t *data_out)
{
  MotionFX_CM0P_MagCal_run(data_in);
  MotionFX_CM0P_MagCal_getParams(data_out);
}

/**
 * @brief  Start magnetometer calibration
 * @param  None
 * @retval None
 */
void MotionFX_manager_MagCal_start(int32_t sampletime)
{
  MotionFX_CM0P_MagCal_init(sampletime, 1);
}

/**
 * @brief  Stop magnetometer calibration
 * @param  None
 * @retval None
 */
void MotionFX_manager_MagCal_stop(int32_t sampletime)
{
  MotionFX_CM0P_MagCal_init(sampletime, 0);
}

/**
 * @brief  Load calibration parameter from memory
 * @param  dataSize length of the data
 * @param  data pointer to the data
 * @retval (1) fail, (0) success
 */
char MotionFX_LoadMagCalFromNVM(unsigned short int dataSize, unsigned int *data)
{
  return (char)1;
}

/**
 * @brief  Save calibration parameter to memory
 * @param  dataSize length of the data
 * @param  data pointer to the data
 * @retval (1) fail, (0) success
 */
char MotionFX_SaveMagCalInNVM(unsigned short int dataSize, unsigned int *data)
{
  return (char)1;
}

/**
 * @}
 */

/**
 * @}
 */
