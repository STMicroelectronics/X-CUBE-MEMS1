/**
  ******************************************************************************
  * @file    sensor_def.c
  * @author  Martin Polacek
  * @brief   This file contains definitions of sensor settings
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "sensor_def.h"

/* Global variables ----------------------------------------------------------*/

/*
 * Format of lists:
 * "name{ n, a_1, a_2, ... , a_n}"
 */

/* LSM6DSO */
static const float Lsm6dsoOdr[] = {6.0f, 208.0f, 417.0f, 833.0f, 1667.0f, 3333.0f, 6667.0f};
static const int32_t Lsm6dsoFs[] = {4, 2, 4, 8, 16};
static const ACTIVE_AXIS_t Lsm6dsoAxis[] = {X_AXIS, Y_AXIS, Z_AXIS, ALL_ACTIVE}; /* Simulation only (not supported by sensor) */
static const uint32_t Lsm6dsoSamplesList[] = {2, 256, 512};

const sensor_setting_t SensorSetting = {"LSM6DSO", Lsm6dsoOdr, Lsm6dsoFs, Lsm6dsoAxis, Lsm6dsoSamplesList, 1};
