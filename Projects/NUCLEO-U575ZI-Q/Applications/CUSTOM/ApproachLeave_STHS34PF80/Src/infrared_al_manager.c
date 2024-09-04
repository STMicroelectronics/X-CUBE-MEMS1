/**
  ******************************************************************************
  * @file    infrared_al_manager.c
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
#include "infrared_al_manager.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
  * @{
  */

/** @addtogroup APPROACH_LEAVE APPROACH LEAVE
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static IAL_Instance_t IAL_Instance; /* Instance of the algorithm */

/* Exported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
  * @brief  Initialize the InfraredAL engine
  * @param  None
  * @retval None
  */
void InfraredAL_manager_init(void)
{
  IAL_device_conf_t device_conf;
  IAL_algo_conf_t algo_conf;

  InfraredAL_Initialize(IAL_MCU_STM32);

  device_conf.odr = Odr;
  device_conf.tau = 1.0f; /* 1.0f if no optical system is used */

  IAL_Instance = InfraredAL_CreateInstance(&algo_conf);

  /* User can modify the algo_conf settings here */

  if (InfraredAL_Start(IAL_Instance, &device_conf, &algo_conf) != IAL_INIT_OK)
  {
    for (;;)
    {
      /* The library was not correctly started */
    }
  }
}

/**
  * @brief  Run Approach Leave algorithm
  * @param  data_in  Structure containing input data
  * @param  data_out Structure containing output data
  * @retval None
  */
void InfraredAL_manager_run(IAL_input_t *data_in, IAL_output_t *data_out)
{
  InfraredAL_Update(IAL_Instance, data_in, data_out);
}

/**
  * @brief  Get the library version
  * @param  version Library version string (must be array of 35 char)
  * @param  length  Library version string length
  * @retval None
  */
void InfraredAL_manager_get_version(char *version, int32_t *length)
{
  *length = (int)InfraredAL_GetLibVersion(version);
}

/**
  * @}
  */

/**
  * @}
  */
