/**
 ******************************************************************************
 * @file    motion_mc_manager.c
 * @author  MEMS Software Solutions Team
 * @brief   This file contains Magnetometer Calibration interface functions
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
#include "motion_mc_manager.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup E_COMPASS E_COMPASS
 * @{
 */

/* Private variables ---------------------------------------------------------*/
static void *mmc_algo; /* NOTE: Algorithm instance requires
                                about 1600 bytes in heap */

/* Private typedef -----------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize the MotionMC engine
 * @param  enable enable (1) or disable (0) library
 * @retval None
 */
void MotionMC_manager_init(unsigned short int enable)
{
  mmc_algo = MotionMC_CM0P_Initialize(MMC_CM0P_MCU_STM32, mmc_algo, MMC_CM0P_HI_AND_SI, enable);
}

/**
 * @brief  Run Magnetometer Calibration algorithm
 * @param  data_in  Structure containing input data
 * @retval None
 */
void MotionMC_manager_update(MMC_CM0P_Input_t *data_in)
{
  MotionMC_CM0P_Update(mmc_algo, data_in);
}

/**
 * @brief  Get the magnetometer calibration values for hard/soft iron
 * @param  data_out  Structure containing output data
 * @retval None
 */
void MotionMC_manager_get_params(MMC_CM0P_Output_t *data_out)
{
  MotionMC_CM0P_GetCalParams(data_out);
}

/**
 * @brief  Get the library version
 * @param  version  Library version string (must be array of 35 char)
 * @param  length  Library version string length
 * @retval None
 */
void MotionMC_manager_get_version(char *version, int *length)
{
  *length = (int)MotionMC_CM0P_GetLibVersion(version);
}

/**
 * @brief  Do hard & soft iron calibration
 * @param  data_raw  Raw magnetometer data [mGauss]
 * @param  data_comp  Calibrated (compensated) data (hard & soft iron calibration) [mGauss]
 * @retval None
 */
void MotionMC_manager_compensate(MOTION_SENSOR_Axes_t *data_raw, MOTION_SENSOR_Axes_t *data_comp)
{
  MMC_CM0P_Output_t data_out;
  MotionMC_CM0P_GetCalParams(&data_out);

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
      mag_comp_mG[i] += (mag_raw_mG[j]  -  data_out.HI_Bias[j] * 10.0f)  *  data_out.SI_Matrix[i][j];
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

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
