/**
  ******************************************************************************
  * @file    infrared_pd_manager.c
  * @author  MEMS Software Solutions Team
  * @brief   This file contains Presence Detection interface functions
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
#include "infrared_pd_manager.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
  * @{
  */

/** @addtogroup PRESENCE_DETECTION PRESENCE DETECTION
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static IPD_Instance_t IPD_Instance; /* Instance of the algorithm */

/* Exported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
  * @brief  Initialize the InfraredPD engine
  * @param  None
  * @retval None
  */
void InfraredPD_manager_init(void)
{
  IPD_device_conf_t device_conf;
  IPD_algo_conf_t algo_conf;

  InfraredPD_Initialize(IPD_MCU_STM32);

  device_conf.odr = Odr;
  device_conf.avg_tmos = AvgTmos;
  device_conf.avg_t = AvgT;
  device_conf.gain_factor = GainFactor;
  device_conf.sens_data = Sensitivity;
  device_conf.transmittance = 1.0f; /* 1.0f if no optical system is used */

  IPD_Instance = InfraredPD_CreateInstance(&algo_conf);

  /* User can modify the algo_conf settings here */

  if (InfraredPD_Start(IPD_Instance, &device_conf, &algo_conf) != IPD_INIT_OK)
  {
    for (;;)
    {
      /* The library was not correctly started */
    }
  }
}

/**
  * @brief  Run Presence Detection algorithm
  * @param  data_in  Structure containing input data
  * @param  data_out Structure containing output data
  * @retval None
  */
void InfraredPD_manager_run(IPD_input_t *data_in, IPD_output_t *data_out)
{
  InfraredPD_Update(IPD_Instance, data_in, data_out);
}

/**
  * @brief  Get the library version
  * @param  version Library version string (must be array of 35 char)
  * @param  length  Library version string length
  * @retval None
  */
void InfraredPD_manager_get_version(char *version, int32_t *length)
{
  *length = (int)InfraredPD_GetLibVersion(version);
}

/**
  * @}
  */

/**
  * @}
  */
