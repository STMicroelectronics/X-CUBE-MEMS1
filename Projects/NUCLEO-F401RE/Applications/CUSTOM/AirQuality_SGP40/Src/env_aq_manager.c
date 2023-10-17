/**
  ******************************************************************************
  * @file    env_aq_manager.c
  * @author  MEMS Software Solutions Team
  * @brief   This file contains Air Quality interface functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "env_aq_manager.h"
#include "sensirion_gas_index_algorithm.h"

/** @addtogroup ENV_APPLICATIONS ENV APPLICATIONS
  * @{
  */

/** @addtogroup AIR_QUALITY AIR QUALITY
  * @{
  */

/* Extern variables ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static GasIndexAlgorithmParams params;  /* Instance of the algorithm */

/* Private typedef -----------------------------------------------------------*/
/* Imported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
  * @brief  Initialize Air Quality algorithm
  * @retval None
  */
void EnvAQ_manager_init(void)
{
  GasIndexAlgorithm_init(&params, GasIndexAlgorithm_ALGORITHM_TYPE_VOC);
}

/**
  * @brief  Run Air Quality algorithm
  * @param  voc_raw_value  VOC raw value
  * @param  voc_index_value  VOC index value
  * @retval None
  */
void EnvAQ_manager_run(int32_t voc_raw_value, int32_t *voc_index_value)
{
  GasIndexAlgorithm_process(&params, voc_raw_value, voc_index_value);
}

/**
  * @brief  Get the algorithm version
  * @param  version  Algorithm version string (must be array of 35 char)
  * @param  length  Algorithm version string length
  * @retval none
  */
void EnvAQ_manager_get_version(char *version, int *length)
{
  char str1[35] = "ST EnvAQ v1.0.0";
  strcpy(version, str1);
  *length = strlen(str1);
}

/**
  * @}
  */

/**
  * @}
  */
