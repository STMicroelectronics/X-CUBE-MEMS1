/**
  ******************************************************************************
  * @file    motion_ac_manager.c
  * @author  MEMS Software Solutions Team
  * @brief   This file contains AccelerometerCalibration interface functions
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
#include "motion_ac_manager.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
  * @{
  */

/** @addtogroup ACCELEROMETER_CALIBRATION ACCELEROMETER CALIBRATION
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* NOTE: Must be implemented for each platform separately, because its implementation is platform dependent.
         No need to call this function, library call this function automatically */
char MotionAC_LoadCalFromNVM(unsigned short int dataSize, unsigned int *data);

/* NOTE: Must be implemented for each platform separately, because its implementation is platform dependent.
         No need to call this function, library call this function automatically */
char MotionAC_SaveCalInNVM(unsigned short int dataSize, unsigned int *data);

/* Exported functions --------------------------------------------------------*/
/**
  * @brief  Initialize the MotionAC engine
  * @param  enable  0 to disable library, 1 to enable library
  * @retval None
  */
void MotionAC_manager_init(MAC_enable_lib_t enable)
{
  MotionAC_Initialize((uint8_t)enable);
}

/**
  * @brief  Run Accelerometer Calibration algorithm
  * @param  data_in  pointer to acceleration [g] and timestamp values [ms]
  * @param  is_calibrated  pointer value returns 1 if calibration is done with current sample, 0 otherwise
  * @retval None
  */
void MotionAC_manager_update(MAC_input_t *data_in, uint8_t *is_calibrated)
{
  MotionAC_Update(data_in, is_calibrated);
}

/**
  * @brief  Run Accelerometer Calibration algorithm
  * @param  data_out  pointer to actual accelerometer offset [g], scale factor correction matrix [-] and quality factor
  * @retval None
  */
void MotionAC_manager_get_params(MAC_output_t *data_out)
{
  MotionAC_GetCalParams(data_out);
}

/**
  * @brief  Get the library version
  * @param  version  Library version string (must be array of 35 char)
  * @param  length  Library version string length
  * @retval none
  */
void MotionAC_manager_get_version(char *version, int32_t *length)
{
  *length = (int)MotionAC_GetLibVersion(version);
}

/**
  * @brief  Do offset & scale factor calibration
  * @param  data_in  raw accelerometer data
  * @param  data_out  calibrated data
  * @retval none
  */
void MotionAC_manager_compensate(MOTION_SENSOR_Axes_t *data_in, MOTION_SENSOR_Axes_t *data_out)
{
  MAC_output_t acc_calib;
  float ans_float[3];
  int32_t ans_int32[3];

  MotionAC_GetCalParams(&acc_calib);

  /* To be MISRA C-2012 compliant the original calculation:
     data_out->x = (int32_t)((data_in->x - acc_bias_to_mg(acc_calib.AccBias[0])) * acc_calib.SF_Matrix[0][0]);
     data_out->y = (int32_t)((data_in->y - acc_bias_to_mg(acc_calib.AccBias[1])) * acc_calib.SF_Matrix[1][1]);
     data_out->z = (int32_t)((data_in->z - acc_bias_to_mg(acc_calib.AccBias[2])) * acc_calib.SF_Matrix[2][2]);
     has been split to separate expressions */
  ans_int32[0] = data_in->x - acc_bias_to_mg(acc_calib.AccBias[0]);
  ans_int32[1] = data_in->y - acc_bias_to_mg(acc_calib.AccBias[1]);
  ans_int32[2] = data_in->z - acc_bias_to_mg(acc_calib.AccBias[2]);

  ans_float[0] = (float)ans_int32[0] * acc_calib.SF_Matrix[0][0];
  ans_float[1] = (float)ans_int32[1] * acc_calib.SF_Matrix[1][1];
  ans_float[2] = (float)ans_int32[2] * acc_calib.SF_Matrix[2][2];

  data_out->x = (int32_t)ans_float[0];
  data_out->y = (int32_t)ans_float[1];
  data_out->z = (int32_t)ans_float[2];
}

/**
  * @brief  Convert accelerometer bias from [g] to [mg]
  * @param  acc_bias  accelerometer bias in [g]
  * @retval accelerometer bias in [mg]
  */
int16_t acc_bias_to_mg(float acc_bias)
{
  float ans_float;

  if (acc_bias >= 0.0f)
  {
    /* To be MISRA C-2012 compliant the original calculation:
       return (int16_t)(acc_bias * 1000.0f + 0.5f);
       has been split to separate expressions */
    ans_float = acc_bias * 1000.0f + 0.5f;
    return (int16_t)ans_float;
  }
  else
  {
    /* To be MISRA C-2012 compliant the original calculation:
       return (int16_t)(acc_bias * 1000.0f - 0.5f);
       has been split to separate expressions */
    ans_float = acc_bias * 1000.0f - 0.5f;
    return (int16_t)ans_float;
  }
}

/* NOTE: Must be implemented for each platform separately, because its implementation is platform dependent.
         No need to call this function, library call this function automatically */
/**
  * @brief  Load the calibration parameters from storage
  * @param  data_size  size of data
  * @param  data  pointer of data
  * @retval Will return 0 the if it is success and 1 if it is failure
  */
char MotionAC_LoadCalFromNVM(unsigned short int data_size, unsigned int *data)
{
  return (char)1; /* FAILURE: Read from NVM not implemented. */
}

/* NOTE: Must be implemented for each platform separately, because its implementation is platform dependent.
         No need to call this function, library call this function automatically */
/**
  * @brief  Save the calibration parameters in storage
  * @param  data_size  size of data
  * @param  data  pointer of data
  * @retval Will return 0 the if it is success and 1 if it is failure
  */
char MotionAC_SaveCalInNVM(unsigned short int data_size, unsigned int *data)
{
  return (char)1; /* FAILURE: Write to NVM not implemented. */
}

/**
  * @}
  */

/**
  * @}
  */

