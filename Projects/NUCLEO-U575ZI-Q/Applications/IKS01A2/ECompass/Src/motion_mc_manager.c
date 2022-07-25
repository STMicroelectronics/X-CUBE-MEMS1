/**
  ******************************************************************************
  * @file    motion_mc_manager.c
  * @author  MEMS Software Solutions Team
  * @brief   This file contains Magnetometer Calibration interface functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "motion_mc_manager.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup E_COMPASS E_COMPASS
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
// NOTE: Must be implemented for each platform separately, because its implementation
//       is platform dependent. No need to call this function, library call this
//       function automatically.
char MotionMC_LoadCalFromNVM(unsigned short int datasize, unsigned int *data);

// NOTE: Must be implemented for each platform separately, because its implementation
//       is platform dependent. No need to call this function, library call this
//       function automatically.
char MotionMC_SaveCalInNVM(unsigned short int datasize, unsigned int *data);

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize the MotionMC engine
 * @param  sampletime period in milliseconds [ms] between the update function call
 * @param  enable enable (1) or disable (0) library
 * @retval None
 */
void MotionMC_manager_init(int sampletime, unsigned short int enable)
{
  MotionMC_Initialize(sampletime, enable);
}

/**
 * @brief  Run Magnetometer Calibration algorithm
 * @param  data_in  Structure containing input data
 * @retval None
 */
void MotionMC_manager_update(MMC_Input_t *data_in)
{
  MotionMC_Update(data_in);
}

/**
 * @brief  Get the magnetometer calibration values for hard/soft iron
 * @param  data_out  Structure containing output data
 * @retval None
 */
void MotionMC_manager_get_params(MMC_Output_t *data_out)
{
  MotionMC_GetCalParams(data_out);
}

/**
 * @brief  Get the library version
 * @param  version  Library version string (must be array of 35 char)
 * @param  length  Library version string length
 * @retval None
 */
void MotionMC_manager_get_version(char *version, int *length)
{
  *length = (int)MotionMC_GetLibVersion(version);
}

/**
 * @brief  Do hard & soft iron calibration
 * @param  data_raw  Raw magnetometer data [mGauss]
 * @param  data_comp  Calibrated (compensated) data (hard & soft iron calibration) [mGauss]
 * @retval None
 */
void MotionMC_manager_compensate(MOTION_SENSOR_Axes_t *data_raw, MOTION_SENSOR_Axes_t *data_comp)
{
  MMC_Output_t data_out;
  MotionMC_GetCalParams(&data_out);

  float mag_raw_mG[3];
  float mag_comp_mG[3];

  mag_raw_mG[0] = (float)data_raw->x;
  mag_raw_mG[1] = (float)data_raw->y;
  mag_raw_mG[2] = (float)data_raw->z;

  /* Compensate magnetometer data */
  /* NOTE: Convert hard iron coefficients [uT] to [mGauss] */
  for (int i = 0; i < 3; i++)
  {
    mag_comp_mG[i] = 0.0f;
    for (int j = 0; j < 3; j++)
    {
      mag_comp_mG[i] += (mag_raw_mG[j]  -  data_out.HI_Bias[j] * 10.0f)  *  data_out.SF_Matrix[i][j];
    }

    mag_comp_mG[i] += (mag_comp_mG[i] >= 0.0f) ? 0.5f : -0.5f;
  }

  data_comp->x = (int32_t)mag_comp_mG[0];
  data_comp->y = (int32_t)mag_comp_mG[1];
  data_comp->z = (int32_t)mag_comp_mG[2];
}

/**
 * @brief  Convert magnetometer data value from [uT] to [mGauss]
 * @param  mag_val_uT  magnetometer data value [uT]
 * @retval magnetometer data value [mGauss]
 */
int32_t mag_val_to_mGauss(float mag_val_uT)
{
  float mGauss_float;

  if (mag_val_uT >= 0.0f)
  {
    mGauss_float = mag_val_uT * 10.0f  +  0.5f;
    return (int32_t)mGauss_float;
  }
  else
  {
    mGauss_float = mag_val_uT * 10.0f  -  0.5f;
    return (int32_t)mGauss_float;
  }
}

// NOTE: Must be implemented for each platform separately, because its implementation
//       is platform dependent. No need to call this function, library call this
//       function automatically.
/**
 * @brief  Load the calibration parameters from storage
 * @param  dataSize  size of data
 * @param  data  pointer to data
 * @retval Will return 0 the if it is success and 1 if it is failure
 */
char MotionMC_LoadCalFromNVM(unsigned short int datasize, unsigned int *data)
{
  return 1; /* FAILURE: Read from NVM not implemented. */
}

// NOTE: Must be implemented for each platform separately, because its implementation
//       is platform dependent. No need to call this function, library call this
//       function automatically.
/**
 * @brief  Save the calibration parameters in storage
 * @param  dataSize  size of data
 * @param  data  pointer to data
 * @retval Will return 0 the if it is success and 1 if it is failure
 */
char MotionMC_SaveCalInNVM(unsigned short int datasize, unsigned int *data)
{
  return 1; /* FAILURE: Write to NVM not implemented. */
}

/**
 * @}
 */

/**
 * @}
 */

