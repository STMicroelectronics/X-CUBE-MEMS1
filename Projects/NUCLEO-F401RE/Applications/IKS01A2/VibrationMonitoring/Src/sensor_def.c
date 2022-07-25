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

/* LSM6DSL */
static const float Lsm6dslOdr[] = {6, 208, 416, 833, 1660, 3330, 6660};
static const int32_t Lsm6dslFs[] = {4, 2, 4, 8, 16};
static const ACTIVE_AXIS_t Lsm6dslAxis[] = {X_AXIS, Y_AXIS, Z_AXIS, ALL_ACTIVE}; /* Simulation only (not supported by sensor) */
static const uint32_t Lsm6dslSamplesList[] = {2, 256, 512};

const sensor_setting_t SensorSetting = {"LSM6DSL", Lsm6dslOdr, Lsm6dslFs, Lsm6dslAxis, Lsm6dslSamplesList, 1};
