/**
 ******************************************************************************
 * @file    sensor_def.c
 * @author  Martin Polacek
 * @brief   This file contains definitions of sensor settings
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed under Software License Agreement
 * SLA0077, (the "License"). You may not use this file except in compliance
 * with the License. You may obtain a copy of the License at:
 *
 *     www.st.com/content/st_com/en/search.html#q=SLA0077-t=keywords-page=1
 *
 *******************************************************************************
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
