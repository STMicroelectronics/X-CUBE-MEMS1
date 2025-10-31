/**
  ******************************************************************************
  * @file    motion_xlf_manager.c
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
#include "motion_xlf_manager.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
  * @{
  */

/** @addtogroup APPROACH_LEAVE APPROACH LEAVE
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
  * @brief  Initialize the MotionXLF engine
  * @param  None
  * @retval None
  */
void MotionXLF_manager_init(void)
{
  MotionXLF_Initialize(MOTION_XLF_MCU_STM32);

  if (MotionXLF_Start() != MOTION_XLF_ALGO_OK)
  {
    for (;;)
    {
      /* The library was not correctly started */
    }
  }
}

/**
  * @brief  Run High-g Low-g algorithm
  * @param  data_in  Structure containing input data
  * @param  data_out Structure containing output data
  * @param  enable_high_g  function pointer for enabling high-g sensor
  * @param  disable_high_g  function pointer for disabling high-g sensor
  * @retval None
  */
void MotionXLF_manager_run(XLF_in_t *data_in, XLF_out_t *data_out, XLF_high_g_enable_disable_pointer_t enable_high_g,
                           XLF_high_g_enable_disable_pointer_t disable_high_g, XLF_algo_settings *algo_set)
{
  MotionXLF_Update(data_in, data_out, enable_high_g, disable_high_g, algo_set);
}

/**
  * @brief  Get the library version
  * @param  version  Library version string (must be array of 35 char)
  * @param  length  Library version string length
  * @retval none
  */
void MotionXLF_manager_get_version(char *version, int32_t *length)
{
  *length = (int)MotionXLF_GetLibVersion(version);
}

/**
  * @}
  */

/**
  * @}
  */
